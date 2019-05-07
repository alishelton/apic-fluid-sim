#ifndef ARRAY3
#define ARRAY3

#include <cstdio>
#include <cmath>
#include <cstring>

template<class T>
struct Array3{
    int nx, ny, nz;
    int size;
    T *data;

    Array3()
            :nx(0), ny(0), nz(0), size(0), data(0)
    {}

    Array3(int nx_, int ny_, int nz_)
            :nx(0), ny(0), nz(0), size(0), data(0)
    { init(nx_, ny_, nz_); }

    void init(int nx_, int ny_, int nz_)
    {
       delete_memory();
       nx=nx_;
       ny=ny_;
       nz = nz_;
       size=nx*ny*nz;
       data=new T[size];
       zero();
    }

    ~Array3()
    { delete_memory(); }

    void delete_memory()
    {
       delete[] data; data=0;
       nx=ny=nz=size=0;
    }

    const T &operator() (int i, int j, int k) const
    { return data[i+nx*(j+ny*k)]; }

    T &operator() (int i, int j, int k)
    { return data[i+nx*(j+ny*k)]; }

    T trilerp(int i, int j, int k, T fx, T fy, T fz)
    { return (1-fz)*((1-fy)*((1-fx)*(*this)(i,j,k)
               +fx*(*this)(i+1,j,k))
               +fy*((1-fx)*(*this)(i,j+1,k)
               +fx*(*this)(i+1,j+1,k)))
               +fz*((1-fy)*((1-fx)*(*this)(i,j,k+1)
               +fx*(*this)(i+1,j,k+1))
               +fy*((1-fx)*(*this)(i,j+1,k+1)
               +fx*(*this)(i+1,j+1,k+1))); }

    void copy_to(Array3 &a) const
    { std::memcpy(a.data, data, size*sizeof(T)); }

    T infnorm() const
    {
       T r=0;
       for(int i=0; i<size; ++i)
          if(!(std::fabs(data[i])<=r)) r=std::fabs(data[i]);
       return r;
    }

    void zero()
    { std::memset(data, 0, size*sizeof(T)); }

    double dot(const Array3 &a) const
    {
       double r=0;
       for(int i=0; i<size; ++i)
          r+=data[i]*a.data[i];
       return r;
    }

    void increment(double scale, const Array3 &a)
    { for(int i=0; i<size; ++i) data[i]+=scale*a.data[i]; }

    void scale_and_increment(double scale, const Array3 &a)
    { for(int i=0; i<size; ++i) data[i]=scale*data[i]+a.data[i]; }
};

typedef Array3<float> Array3f;
typedef Array3<double> Array3d;
typedef Array3<char> Array3c;

template<class T>
struct Array3x4{
    int nx, ny, nz;
    int size;
    T *data;

    Array3x4()
            :nx(0), ny(0), nz(0), size(0), data(0)
    {}

    Array3x4(int nx_, int ny_, int nz_)
            :nx(0), ny(0), nz(0), size(0), data(0)
    { init(nx_, ny_, nz_); }

    void init(int nx_, int ny_, int nz_)
    {
        delete_memory();
        nx=nx_;
        ny=ny_;
        nz=nz_;
        size=3*nx*ny*nz;
        data=new T[size];
        zero();
    }

    ~Array3x4()
    { delete_memory(); }

    void delete_memory()
    {
        delete[] data; data=0;
        nx=ny=nz=size=0;
    }

    const T &operator() (int i, int j, int k, int l) const
    { return data[(i+nx*(j+ny*k))*4+l]; }

    T &operator() (int i, int j, int k, int l)
    { return data[(i+nx*(j+ny*k))*4+l]; }

    void zero()
    { std::memset(data, 0, size*sizeof(T)); }

    T infnorm() const
    {
        T r=0;
        for(int i=0; i<size; ++i)
            if(!(std::fabs(data[i])<=r)) r=std::fabs(data[i]);
        return r;
    }

    void write_matlab(FILE *fp, const char *variable_name)
    {
        fprintf(fp, "%s=reshape([", variable_name);
        for(int i=0; i<size; ++i)
            fprintf(fp, "%lg ", (double)data[i]);
        fprintf(fp,"],[3 %d %d]);\n", nx, ny);
    }
};

typedef Array3x4<float> Array3x4f;
typedef Array3x4<double> Array3x4d;

#endif
