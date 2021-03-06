/**
 * Definition of the Grid struct, which holds the MAC grid.
 *
 * @author Ante Qu, 2017
 * Based on Bridson's simple_flip2d starter code at http://www.cs.ubc.ca/~rbridson/
 */

#ifndef GRID_H
#define GRID_H

#include "array2.h"
#include "array3.h"
#include "util.h"

#define AIRCELL 0
#define FLUIDCELL 1
#define SOLIDCELL 2

struct Grid{
   float gravity;
   float lx, ly, lz;
   float h, overh;

   // active variables
   Array3f u, v, w; // staggered MAC grid of velocities
   Array3f du, dv, dw; // saved velocities and differences for particle update
   Array3c marker; // identifies what sort of cell we have
   Array3f phi; // decays away from water into air (used for extrapolating velocity)
   Array3d pressure;
   // stuff for the pressure solve
   Array3x4f poisson;
   Array3d preconditioner;
   Array3d m;
   Array3d r, z, s;

   Grid(void)
   {}

   Grid(float gravity_, int cell_nx, int cell_ny, int cell_nz, float lx_)
   { init(gravity_, cell_nx, cell_ny, cell_nz, lx_); }

   void init(float gravity_, int cell_nx, int cell_ny, int cell_nz, float lx_);
   float CFL(void);
   void save_velocities(void);
   void add_gravity(float dt, bool centered, float cx, float cy, float cz);
   void compute_distance_to_fluid(void);
   void extend_velocity(void);
   void apply_boundary_conditions(void);
   void make_incompressible(void);
   void get_velocity_update(void);

   void bary_x(float x, int &i, float &fx)
   {
      float sx=x*overh;
      i=(int)sx;
      fx=sx-floor(sx);
   }

   void bary_x_centre(float x, int &i, float &fx)
   {
      float sx=x*overh-0.5;
      i=(int)sx;
      if(i<0){ i=0; fx=0.0; }
      else if(i>pressure.nx-2){ i=pressure.nx-2; fx=1.0; }
      else{ fx=sx-floor(sx); }
   }

   void bary_y(float y, int &j, float &fy)
   {
      float sy=y*overh;
      j=(int)sy;
      fy=sy-floor(sy);
   }

   void bary_y_centre(float y, int &j, float &fy)
   {
      float sy=y*overh-0.5;
      j=(int)sy;
      if(j<0){ j=0; fy=0.0; }
      else if(j>pressure.ny-2){ j=pressure.ny-2; fy=1.0; }
      else{ fy=sy-floor(sy); }
   }

   void bary_z(float z, int &k, float &fz)
   {
       float sz=z*overh;
       k=(int)sz;
       fz=sz-floor(sz);
   }

    void bary_z_centre(float z, int &k, float &fz)
    {
        float sz=z*overh-0.5;
        k=(int)sz;
        if(k<0){ k=0; fz=0.0; }
        else if(k>pressure.ny-2){ k=pressure.nz-2; fz=1.0; }
        else{ fz=sz-floor(sz); }
    }

    void trilerp_uvw(float px, float py, float pz, float &pu, float &pv, float &pw)
    {
        int i, j, k;
        float fx, fy, fz;

        bary_x(px, i, fx);
        bary_y_centre(py, j, fy);
        bary_z_centre(pz, k, fz);

        pu=u.trilerp(i, j, k, fx, fy, fz);

        bary_x_centre(px, i, fx);
        bary_y(py, j, fy);
        bary_z_centre(pz, k, fz);

        pv=v.trilerp(i, j, k, fx, fy, fz);

        bary_x_centre(px, i, fx);
        bary_y_centre(py, j, fy);
        bary_z(pz, k, fz);

        pw=w.trilerp(i, j, k, fx, fy, fz);
    }

   private:
   void init_phi(void);
   void sweep_phi(void);
   void sweep_u(int i0, int i1, int j0, int j1, int k0, int k1);
   void sweep_v(int i0, int i1, int j0, int j1, int k0, int k1);
   void sweep_w(int i0, int i1, int j0, int j1, int k0, int k1);
   void sweep_velocity(void);
   void find_divergence(void);
   void form_poisson(void);
   void form_preconditioner(void);
   void apply_poisson(const Array3d &x, Array3d &y);
   void apply_preconditioner(const Array3d &x, Array3d &y, Array3d &temp);
   void solve_pressure(int maxits, double tolerance);
   void add_gradient(void);
};

#endif

