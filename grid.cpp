/**
 * Implementation of the MAC grid. Most of your edits should be in particles.cpp instead.
 *
 * @author Ante Qu, 2017
 * Based on Bridson's simple_flip2d starter code at http://www.cs.ubc.ca/~rbridson/
 */

#include <cmath>
#include <vector>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include "grid.h"

using namespace std;

void Grid::
init(float gravity_, int cell_nx, int cell_ny, int cell_nz, float lx_)
{
   gravity=gravity_;
   lx=lx_;
   ly=cell_ny*lx/cell_nx;
   h=lx/cell_nx;
   overh=cell_nx/lx;
   // allocate all the grid variables
   u.init(cell_nx+1, cell_ny, cell_nz);
   v.init(cell_nx, cell_ny+1, cell_nz);
   w.init(cell_nx, cell_ny, cell_nz+1);
   pressure.init(cell_nx, cell_ny, cell_nz);
   marker.init(cell_nx, cell_ny, cell_nz);
   phi.init(cell_nx, cell_ny, cell_nz);
   du.init(cell_nx+1, cell_ny, cell_nz);
   dv.init(cell_nx, cell_ny+1, cell_nz);
   dw.init(cell_nx, cell_ny, cell_nz+1);
   poisson.init(cell_nx, cell_ny, cell_nz);
   preconditioner.init(cell_nx, cell_ny, cell_nz);
   m.init(cell_nx, cell_ny, cell_nz);
   r.init(cell_nx, cell_ny, cell_nz);
   z.init(cell_nx, cell_ny, cell_nz);
   s.init(cell_nx, cell_ny, cell_nz);
}

float Grid::
CFL(void)
{
   float maxv3=max(h*gravity, sqr(u.infnorm())+sqr(v.infnorm()) + sqr(w.infnorm()));
   if(maxv3<1e-16) maxv3=1e-16;
   return h/sqrt(maxv3);
}

void Grid::
save_velocities(void)
{
   u.copy_to(du);
   v.copy_to(dv);
   w.copy_to(dw);
}

/* centered gravity is the spherical gravity I added. */
/* for the normal uniform gravity, see lines 107-108 */
void Grid::
add_gravity(float dt, bool centered, float cx, float cy, float cz)
{
   float dtg=dt*gravity;
   if( false ) // TODO maybe fix later, was: centered, added 0s to u,v so it doesn't scream
   {
      for(int i = 0; i < u.nx; ++i)
      {
         for( int j = 0; j < v.ny; ++j)
         {
            float x, y, dx, dy, dr2, dr;
            if ( j < u.ny)
            {
               x = ( i ) * h;
               y = ( 0.5 + j ) * h;
               dx = x - cx;
               dy = y - cy;
               dr2 = dx * dx + dy * dy;
               if (dr2 < 0.0001 * h * h){
                  printf("dr2 too small: %f \n", dr2);
                  dr2 = 0.0001 * h * h;
               }
               dr = sqrt(dr2);
               dx /= dr;
               dy /= dr;
               u(i,j,0) -= dtg * dx / dr2;
            }
            if( i < v.nx )
            {
               x = ( 0.5 + i ) * h;
               y = ( j ) * h;
               dx = x - cx;
               dy = y - cy;
               dr2 = dx * dx + dy * dy;
               if (dr2 < 0.0001 * h * h){
                  printf("dr2 too small: %f \n", dr2);
                  dr2 = 0.0001 * h * h;
               }
               dr = sqrt(dr2);
               dx /= dr;
               dy /= dr;
               v(i,j,0) -= dtg * dy / dr2;
            }

         }
      }
   }
   else
   {
      for(int i=0; i<v.size; ++i)
         v.data[i]-=dtg;
   }
}

void Grid::
compute_distance_to_fluid(void) // TODO see if needs index adjustment, i think it's fine though
{
   init_phi();
   for(int i=0; i<8; ++i)
      sweep_phi();
}

void Grid::
extend_velocity(void)
{
   for(int i=0; i<8; ++i)
      sweep_velocity();
}

void Grid::
apply_boundary_conditions(void)
{
   int i, j, k;
   // first mark where solid is
   for(j=0; j<marker.ny; ++j)
      for(k=0; k<marker.nz; ++k)
         marker(0,j,k)=marker(marker.nx-1,j,k)=SOLIDCELL;
   for(j=0; j<marker.ny; ++j)
      for(i=0; i<marker.nx; ++i)
         marker(i,j,0)=marker(i,j,marker.nz-1)=SOLIDCELL;
   for(k=0; k<marker.nz; ++k)
      for(i=0; i<marker.nx; ++i)
         marker(i,0,k)=marker(i,marker.ny-1,k)=SOLIDCELL;
   // now make sure nothing leaves the domain
   for(j=0; j<marker.ny; ++j)
      for(k=0; k<marker.nz; ++k)
         u(0,j,k)=u(1,j,k)=u(u.nx-1,j,k)=u(u.nx-2,j,k)=0;
   for(j=0; j<marker.ny; ++j)
      for(i=0; i<marker.nx; ++i)
         w(i,j,0)=w(i,j,1)=w(i,j,w.nz-1)=w(i,j,w.nz-2)=0;
   for(k=0; k<marker.nz; ++k)
      for(i=0; i<marker.nx; ++i)
         v(i,0,k)=v(i,1,k)=v(i,v.ny-1,k)=v(i,v.ny-2,k)=0;
}

void Grid::
make_incompressible(void)
{
   find_divergence();
   form_poisson();
   form_preconditioner();
   solve_pressure(100, 1e-5);
   add_gradient();
}

void Grid::
get_velocity_update(void)
{
   int i;
   for(i=0; i<u.size; ++i)
      du.data[i]=u.data[i]-du.data[i];
   for(i=0; i<v.size; ++i)
      dv.data[i]=v.data[i]-dv.data[i];
   for(i=0; i<w.size; ++i)
      dw.data[i]=w.data[i]-dw.data[i];
}

//====================================== private helper functions ============================

void Grid::
init_phi(void)
{
   int i,j,k;
   // start off with indicator inside the fluid and overestimates of distance outside
   float large_distance=phi.nx+phi.ny+phi.nz+2;
   for(i=0; i<phi.size; ++i)
      phi.data[i]=large_distance;
   for(j=1; j<phi.ny-1; ++j) for(i=1; i<phi.nx-1; ++i) for(k=0; k<phi.nz-1; ++k) {
      if(marker(i,j,k)==FLUIDCELL){
         phi(i,j,k)=-0.5f;
      }
   }
}

static inline void solve_distance(float p, float q, float t, float &r)
{
    float pq_min = fmin(p,q), pq_max = fmax(p,q);
    float min3 = fmin(t,pq_min);
    float max3 = fmax(t,pq_max);
    float mid3 = fmax(pq_min,fmin(pq_max,t));

    float d = min3 + 1;
    if (d > mid3) {
        d=(min3+mid3+sqrt(2-sqr(min3-mid3)))/2;
        if (d > max3) {
            d=(min3+mid3+max3+sqrt(3-2*(min3*min3-min3*mid3-min3*max3+mid3*mid3-mid3*max3+max3*max3)))/3;
        }
    }
    if (d < r)
        r=d;
}

void Grid::
sweep_phi(void)
{
   // fast sweeping outside the fluid in all four sweep directions
   int i, j, k;
   for(j=1; j<phi.ny; ++j) for(i=1; i<phi.nx; ++i) for(k=1; k < phi.nz; ++k)
      if(marker(i,j,k)!=FLUIDCELL)
         solve_distance(phi(i-1,j,k), phi(i,j-1,k), phi(i,j,k-1), phi(i,j,k));

   for(j=phi.ny-2; j>=0; --j) for(i=1; i<phi.nx; ++i) for(k=1; k < phi.nz; ++k)
      if(marker(i,j,k)!=FLUIDCELL)
         solve_distance(phi(i-1,j,k), phi(i,j+1,k), phi(i,j,k-1), phi(i,j,k));

   for(j=1; j<phi.ny; ++j) for(i=phi.nx-2; i>=0; --i) for(k=1; k < phi.nz; ++k)
      if(marker(i,j,k)!=FLUIDCELL)
         solve_distance(phi(i+1,j,k), phi(i,j-1,k), phi(i,j,k-1), phi(i,j,k));

   for(j=phi.ny-2; j>=0; --j) for(i=phi.nx-2; i>=0; --i) for(k=1; k < phi.nz; ++k)
      if(marker(i,j,k)!=FLUIDCELL)
         solve_distance(phi(i+1,j,k), phi(i,j+1,k), phi(i,j,k-1), phi(i,j,k));

   for(j=1; j<phi.ny; ++j) for(i=1; i<phi.nx; ++i) for(k=phi.nz-2; k>=0; --k)
      if(marker(i,j,k)!=FLUIDCELL)
         solve_distance(phi(i-1,j,k), phi(i,j-1,k), phi(i,j,k+1), phi(i,j,k));

   for(j=phi.ny-2; j>=0; --j) for(i=1; i<phi.nx; ++i) for(k=phi.nz-2; k>=0; --k)
      if(marker(i,j,k)!=FLUIDCELL)
         solve_distance(phi(i-1,j,k), phi(i,j+1,k), phi(i,j,k+1), phi(i,j,k));

   for(j=1; j<phi.ny; ++j) for(i=phi.nx-2; i>=0; --i) for(k=phi.nz-2; k>=0; --k)
      if(marker(i,j,k)!=FLUIDCELL)
         solve_distance(phi(i+1,j,k), phi(i,j-1,k), phi(i,j,k+1), phi(i,j,k));

    for(j=phi.ny-2; j>=0; --j) for(i=phi.nx-2; i>=0; --i) for(k=phi.nz-2; k>=0; --k)
      if(marker(i,j,k)!=FLUIDCELL)
         solve_distance(phi(i+1,j,k), phi(i,j+1,k), phi(i,j,k+1), phi(i,j,k));
}

void Grid::
sweep_u(int i0, int i1, int j0, int j1, int k0, int k1) // TODO : this needs fixing
{
   int di=(i0<i1) ? 1 : -1, dj=(j0<j1) ? 1 : -1, dk=(k0<k1) ? 1 : -1;
   float dp, dq, dr, alpha, beta;
   for(int j=j0; j!=j1; j+=dj) for(int i=i0; i!=i1; i+=di) for(int k=k0; k!=k1; k+=dk)
      if(marker(i-1,j,k)==AIRCELL && marker(i,j,k)==AIRCELL){
         dp=di*(phi(i,j,k)-phi(i-1,j,k));
         if(dp<0) continue; // not useful on this sweep direction
         dq=0.5*(phi(i-1,j,k)+phi(i,j,k)-phi(i-1,j-dj,k)-phi(i,j-dj,k));
         if(dq<0) continue; // not useful on this sweep direction
         dr=0.5*(phi(i-1,j,k)+phi(i,j,k)-phi(i-1,j,k-dk)-phi(i,j,k-dk));
         if (dr<0) continue;
         if(dp+dq+dr==0) {
            alpha=1/3.0;
         } else {
            alpha=dp/(dp+dq+dr);
            beta=dq/(dp+dq+dr);
         }
         u(i,j,k)=alpha*u(i-di,j,k)+beta*u(i,j-dj,k)+(1-alpha-beta)*u(i,j,k-dk);
      }
}

void Grid::
sweep_v(int i0, int i1, int j0, int j1, int k0, int k1) // TODO : so does this
{
   int di=(i0<i1) ? 1 : -1, dj=(j0<j1) ? 1 : -1, dk=(k0<k1) ? 1 : -1;
   float dp, dq, dr, alpha, beta;
   for(int j=j0; j!=j1; j+=dj) for(int i=i0; i!=i1; i+=di) for(int k=k0; k!=k1; k+=dk)
      if(marker(i,j-1,k)==AIRCELL && marker(i,j,k)==AIRCELL){
         dq=dj*(phi(i,j,k)-phi(i,j-1,k));
         if(dq<0) continue; // not useful on this sweep direction
         dp=0.5*(phi(i,j-1,k)+phi(i,j,k)-phi(i-di,j-1,k)-phi(i-di,j,k));
         if(dp<0) continue; // not useful on this sweep direction
         dr=0.5*(phi(i-1,j,k)+phi(i,j,k)-phi(i-1,j,k-dk)-phi(i,j,k-dk));
         if (dr<0) continue;
         if(dp+dq+dr==0) {
            alpha=1/3.0;
         } else {
            alpha=dp/(dp+dq+dr);
            beta=dq/(dp+dq+dr);
         }
         v(i,j,k)=alpha*v(i-di,j,k)+beta*v(i,j-dj,k)+(1-alpha-beta)*v(i,j-dj,k);
      }
}

void Grid::
sweep_w(int i0, int i1, int j0, int j1, int k0, int k1) // TODO : so does this
{
   int di=(i0<i1) ? 1 : -1, dj=(j0<j1) ? 1 : -1, dk=(k0<k1) ? 1 : -1;
   float dp, dq, dr, alpha, beta;
   for(int j=j0; j!=j1; j+=dj) for(int i=i0; i!=i1; i+=di) for(int k=k0; k!=k1; k+=dk)
      if(marker(i,j,k-1)==AIRCELL && marker(i,j,k)==AIRCELL){
         dr=dk*(phi(i,j,k)-phi(i,j,k-1));
         if(dr<0) continue; // not useful on this sweep direction
         dq=0.5*(phi(i,j-1,k)+phi(i,j,k)-phi(i,j-1,k-dk)-phi(i,j-1,k-dk));
         if(dq<0) continue; // not useful on this sweep direction
         dp=0.5*(phi(i-1,j,k)+phi(i,j,k)-phi(i-1,j,k-dk)-phi(i-1,j,k-dk));
         if (dp<0) continue;
         if(dp+dq+dr==0) {
            alpha=1/3.0;
         } else {
            alpha=dp/(dp+dq+dr);
            beta=dq/(dp+dq+dr);
         }
         w(i,j,k)=alpha*w(i-di,j,k)+beta*w(i,j-dj,k)+(1-beta-alpha)*w(i,j,k-dk);
      }
}

void Grid::
sweep_velocity(void) // TODO fix sweep
{
   int i, j, k;
   // sweep u, only into the air
   sweep_u(1, u.nx-1, 1, u.ny-1, 1, u.nz-1);
   sweep_u(1, u.nx-1, u.ny-2, 0, 1, u.nz-1);
   sweep_u(u.nx-2, 0, 1, u.ny-1, 1, u.nz-1);
   sweep_u(u.nx-2, 0, u.ny-2, 0, 1, u.nz-1);
   sweep_u(1, u.nx-1, 1, u.ny-1, u.nz-2, 0);
   sweep_u(1, u.nx-1, u.ny-2, 0, u.nz-2, 0);
   sweep_u(u.nx-2, 0, 1, u.ny-1, u.nz-2, 0);
   sweep_u(u.nx-2, 0, u.ny-2, 0, u.nz-2, 0);
   for(i=0; i<u.nx; ++i){
      for(k=0; k<u.nz; ++k)
         u(i,0,k)=u(i,1,k); u(i,u.ny-1,k)=u(i,u.ny-2,k);
   }
   for(j=0; j<u.ny; ++j){
      for(k=0; k<u.nz; ++k)
         u(0,j,k)=u(1,j,k); u(u.nx-1,j,k)=u(u.nx-2,j,k);
   }
   for(i=0; i<u.nx; ++i){
      for(j=0; j<u.ny; ++j)
         u(i,j,0)=u(i,j,1); u(i,j,u.nz-1)=u(i,j,u.nz-2);
   }

   // now the same for v
   sweep_v(1, v.nx-1, 1, v.ny-1, 1, v.nz-1);
   sweep_v(1, v.nx-1, v.ny-2, 0, 1, v.nz-1);
   sweep_v(v.nx-2, 0, 1, v.ny-1, 1, v.nz-1);
   sweep_v(v.nx-2, 0, v.ny-2, 0, 1, v.nz-1);
   sweep_v(1, v.nx-1, 1, v.ny-1, v.nz-2, 0);
   sweep_v(1, v.nx-1, v.ny-2, 0, v.nz-2, 0);
   sweep_v(v.nx-2, 0, 1, v.ny-1, v.nz-2, 0);
   sweep_v(v.nx-2, 0, v.ny-2, 0, v.nz-2, 0);
   for(i=0; i<v.nx; ++i){
      for(k=0; k<v.nz; ++k)
         v(i,0,k)=v(i,1,k); v(i,v.ny-1,k)=v(i,v.ny-2,k);
   }
   for(j=0; j<v.ny; ++j){
      for(k=0; k<v.nz; ++k)
         v(0,j,k)=v(1,j,k); v(v.nx-1,j,k)=v(v.nx-2,j,k);
   }
   for(i=0; i<v.nx; ++i){
      for(j=0; j<v.ny; ++j)
         v(i,j,0)=v(i,j,1); v(i,j,v.nz-1)=v(i,j,v.nz-2);
   }

   // now for w
   sweep_w(1, w.nx-1, 1, w.ny-1, 1, w.nz-1);
   sweep_w(1, w.nx-1, w.ny-2, 0, 1, w.nz-1);
   sweep_w(w.nx-2, 0, 1, w.ny-1, 1, w.nz-1);
   sweep_w(w.nx-2, 0, w.ny-2, 0, 1, w.nz-1);
   sweep_w(1, w.nx-1, 1, w.ny-1, w.nz-2, 0);
   sweep_w(1, w.nx-1, w.ny-2, 0, w.nz-2, 0);
   sweep_w(w.nx-2, 0, 1, w.ny-1, w.nz-2, 0);
   sweep_w(w.nx-2, 0, w.ny-2, 0, w.nz-2, 0);
   for(i=0; i<w.nx; ++i){
      for(k=0; k<w.nz; ++k)
         w(i,0,k)=w(i,1,k); w(i,w.ny-1,k)=w(i,w.ny-2,k);
   }
   for(j=0; j<w.ny; ++j){
      for(k=0; k<w.nz; ++k)
         w(0,j,k)=w(1,j,k); w(w.nx-1,j,k)=w(w.nx-2,j,k);
   }
   for(i=0; i<w.nx; ++i){
      for(j=0; j<w.ny; ++j)
         w(i,j,0)=w(i,j,1); w(i,j,w.nz-1)=w(i,j,w.nz-2);
   }
}

void Grid::
find_divergence(void)
{
   r.zero();
   for(int j=0; j<r.ny; ++j) for(int i=0; i<r.nx; ++i) for(int k=0; k<r.nz; ++k) {
      if(marker(i,j,k)==FLUIDCELL)
         r(i,j,k)=u(i+1,j,k)-u(i,j,k)+v(i,j+1,k)-v(i,j,k)+w(i,j,k+1)-w(i,j,k);
   }
}

void Grid::
form_poisson(void)
{
   poisson.zero();
   for(int j=1; j<poisson.ny-1; ++j) for(int i=1; i<poisson.nx-1; ++i) for(int k=1; k<poisson.nz-1; ++k) {
      if(marker(i,j,k)==FLUIDCELL){
         if(marker(i-1,j,k)!=SOLIDCELL)
            poisson(i,j,k, 0)+=1;
         if(marker(i+1,j,k)!=SOLIDCELL){
            poisson(i,j,k,0)+=1;
            if(marker(i+1,j,k)==FLUIDCELL)
               poisson(i,j,k,1)=-1;
         }

         if(marker(i,j-1,k)!=SOLIDCELL)
            poisson(i,j,k,0)+=1;
         if(marker(i,j+1,k)!=SOLIDCELL){
            poisson(i,j,k,0)+=1;
            if(marker(i,j+1,k)==FLUIDCELL)
               poisson(i,j,k,2)=-1;
         }

         if(marker(i,j,k-1)!=SOLIDCELL)
            poisson(i,j,k,0)+=1;
         if(marker(i,j,k+1)!=SOLIDCELL){
            poisson(i,j,k,0)+=1;
            if(marker(i,j,k+1)==FLUIDCELL)
               poisson(i,j,k,3)=-1;
         }
      }
   }
}

void Grid::
apply_poisson(const Array3d &x, Array3d &y)
{
   y.zero();
   for(int j=1; j<poisson.ny-1; ++j) for(int i=1; i<poisson.nx-1; ++i) for(int k=1; k<poisson.nz-1; ++k){
      if(marker(i,j,k)==FLUIDCELL){
         y(i,j,k)=poisson(i,j,k,0)*x(i,j,k) + poisson(i-1,j,k,1)*x(i-1,j,k)
                                      + poisson(i,j,k,1)*x(i+1,j,k)
                                      + poisson(i,j-1,k,2)*x(i,j-1,k)
                                      + poisson(i,j,k,2)*x(i,j+1,k)
                                      + poisson(i,j,k-1,3)*x(i,j,k-1)
                                      + poisson(i,j,k,3)*x(i,j,k+1);
      }
   }
}

void Grid::
form_preconditioner()
{
   const double mic_parameter=0.99;
   double d;
   preconditioner.zero();
   for(int j=1; j<preconditioner.ny-1; ++j) for(int i=1; i<preconditioner.nx-1; ++i) for(int k=1; k<preconditioner.nz-1; ++k) {
      if(marker(i,j,k)==FLUIDCELL){
         d=poisson(i,j,k,0) - sqr( poisson(i-1,j,k,1)*preconditioner(i-1,j,k) )
                          - sqr( poisson(i,j-1,k,2)*preconditioner(i,j-1,k) )
                          - sqr( poisson(i,j,k-1,3)*preconditioner(i,j,k-1) )
                          - mic_parameter*( poisson(i-1,j,k,1)*( poisson(i-1,j,k,2)+poisson(i-1,j,k,3) )*sqr(preconditioner(i-1,j,k))
                                           +poisson(i,j-1,k,2)*( poisson(i,j-1,k,1)+poisson(i,j-1,k,3) )*sqr(preconditioner(i,j-1,k))
                                           +poisson(i,j,k-1,3)*(poisson(i,j,k-1,1)+poisson(i,j,k-1,2) )*sqr(preconditioner(i,j,k-1)) );
         preconditioner(i,j,k)=1/sqrt(d+1e-6);
      }
   }
}

void Grid::
apply_preconditioner(const Array3d &x, Array3d &y, Array3d &m)
{
   int i, j,k;
   float d;
   m.zero();
   // solve L*m=x
   for(j=1; j<x.ny-1; ++j) for(i=1; i<x.nx-1; ++i) for(k=1; k<x.nz-1; ++k)
      if(marker(i,j,k)==FLUIDCELL){
         d=x(i,j,k) - poisson(i-1,j,k,1)*preconditioner(i-1,j,k)*m(i-1,j,k)
                    - poisson(i,j-1,k,2)*preconditioner(i,j-1,k)*m(i,j-1,k)
                    - poisson(i,j,k-1,3)*preconditioner(i,j,k-1)*m(i,j,k-1);
         m(i,j,k)=preconditioner(i,j,k)*d;
      }
   // solve L'*y=m
   y.zero();
   for(j=x.ny-2; j>0; --j) for(i=x.nx-2; i>0; --i) for(k=x.nz-2; k>0; --k)
      if(marker(i,j,k)==FLUIDCELL){
         d=m(i,j,k) - poisson(i,j,k,1)*preconditioner(i,j,k)*y(i+1,j,k)
                    - poisson(i,j,k,2)*preconditioner(i,j,k)*y(i,j+1,k)
                    - poisson(i,j,k,3)*preconditioner(i,j,k)*y(i,j,k+1);
         y(i,j,k)=preconditioner(i,j,k)*d;
      }
}

void Grid::
solve_pressure(int maxits, double tolerance)
{
   int its;
   double tol=tolerance*r.infnorm();
   pressure.zero();
   if(r.infnorm()==0)
      return;
   apply_preconditioner(r, z, m);
   z.copy_to(s);
   double rho=z.dot(r);
   if(rho==0)
      return;
   for(its=0; its<maxits; ++its){
      apply_poisson(s, z);
      double alpha=rho/s.dot(z);
      pressure.increment(alpha, s);
      r.increment(-alpha, z);
      if(r.infnorm()<=tol){
         printf("pressure converged to %g in %d iterations\n", r.infnorm(), its);
         return;
      }
      apply_preconditioner(r, z, m);
      double rhonew=z.dot(r);
      double beta=rhonew/rho;
      s.scale_and_increment(beta, z);
      rho=rhonew;
   }
   printf("Didn't converge in pressure solve (its=%d, tol=%g, |r|=%g)\n", its, tol, r.infnorm());
}

void Grid::
add_gradient(void) // TODO : is the 2 right? what does it mean?
{
   int i, j,k;
   for(j=1; j<u.ny-1; ++j) for(i=2; i<u.nx-2; ++i) for(k=1; k<u.nz-1; ++k) {
      if(marker(i-1,j,k)|marker(i,j,k)==FLUIDCELL){ // if at least one is FLUID, neither is SOLID
         u(i,j,k)+=pressure(i,j,k)-pressure(i-1,j,k);
      }
   }
   for(j=2; j<v.ny-2; ++j) for(i=1; i<v.nx-1; ++i) for(k=1; k<v.nz-1; ++k) {
      if(marker(i,j-1,k)|marker(i,j,k)==FLUIDCELL){ // if at least one is FLUID, neither is SOLID
         v(i,j,k)+=pressure(i,j,k)-pressure(i,j-1,k);
      }
   }
   for(j=1; j<w.ny-1; ++j) for(i=1; i<w.nx-1; ++i) for(k=2; k<w.nz-2; ++k) {
      if(marker(i,j,k-1)|marker(i,j,k)==FLUIDCELL){ // if at least one is FLUID, neither is SOLID
         w(i,j,k)+=pressure(i,j,k)-pressure(i,j,k-1);
      }
   }
}

