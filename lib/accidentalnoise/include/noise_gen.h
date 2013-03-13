#ifndef NOISE_GEN_H
#define NOISE_GEN_H

namespace anl
{

    typedef double (*interp_func)(double);
    typedef double (*noise_func2)(double, double, unsigned int, interp_func);
    typedef double (*noise_func3)(double,double,double,unsigned int, interp_func);
    typedef double (*noise_func4)(double,double,double,double,unsigned int, interp_func);
    typedef double (*noise_func6)(double,double,double,double,double,double,unsigned int,interp_func);

    // Interpolation functions
    double noInterp(double t);
    double linearInterp(double t);
    double hermiteInterp(double t);
    double quinticInterp(double t);


    // Noise generators
    double value_noise2D(double x, double y, unsigned int seed, interp_func interp);
    double value_noise3D(double x, double y, double z, unsigned int seed, interp_func interp);
    double value_noise4D(double x, double y, double z, double w, unsigned int seed, interp_func interp);
    double value_noise6D(double x, double y, double z, double w, double u, double v, unsigned int seed, interp_func interp);

    double gradient_noise2D(double x, double y, unsigned int seed, interp_func interp);
    double gradient_noise3D(double x, double y, double z, unsigned int seed, interp_func interp);
    double gradient_noise4D(double x, double y, double z, double w, unsigned int seed, interp_func interp);
    double gradient_noise6D(double x, double y, double z, double w, double u, double v, unsigned int seed, interp_func interp);

    double gradval_noise2D(double x, double y, unsigned int seed, interp_func interp);
    double gradval_noise3D(double x, double y, double z, unsigned int seed, interp_func interp);
    double gradval_noise4D(double x, double y, double z, double w, unsigned int seed, interp_func interp);
    double gradval_noise6D(double x, double y, double z, double w, double u, double v, unsigned int seed, interp_func interp);

    double white_noise2D(double x, double y, unsigned int seed, interp_func interp);
    double white_noise3D(double x, double y, double z, unsigned int seed, interp_func interp);
    double white_noise4D(double x, double y, double z, double w, unsigned int seed, interp_func interp);
    double white_noise6D(double x, double y, double z, double w, double u, double v, unsigned int seed, interp_func interp);

    double simplex_noise2D(double x, double y, unsigned int seed, interp_func interp);
    double simplex_noise3D(double x, double y, double z, unsigned int seed, interp_func interp);
    double simplex_noise4D(double x, double y, double z, double w, unsigned int seed, interp_func interp);
    double simplex_noise6D(double x, double y, double z, double w, double u, double v, unsigned int seed, interp_func interp);
    double new_simplex_noise4D(double x, double y, double z, double w, unsigned int seed, interp_func interp);

    void cellular_function2D(double x, double y, unsigned int seed, double *f, double *disp);
    void cellular_function3D(double x, double y, double z, unsigned int seed, double *f, double *disp);
    void cellular_function4D(double x, double y, double z, double w, unsigned int seed, double *f, double *disp);
    void cellular_function6D(double x, double y, double z, double w, double u, double v, unsigned int seed, double *f, double *disp);
    // Hash
    unsigned int FNV1A_3d(double x, double y, double z, unsigned int seed);

};
#endif
