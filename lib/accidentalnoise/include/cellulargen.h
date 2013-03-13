#ifndef CELLULAR_GEN_H
#define CELLULAR_GEN_H
#include "implicitmodulebase.h"

/********************************************************
CCellularGen
    Implement a class to generate and cache cellular noise. Multiple front-end classes can
call this, and as long as the input coords remain the same, the cached value will be returned.
Generates F1..F4 and V1..V4.
*********************************************************/

namespace anl
{
struct SCellularCache
{
    double f[4];
    double d[4];
    double x,y,z,w,u,v;
    bool valid;

    SCellularCache()
    {
        valid=false;
    }
};


class CCellularGenerator
{
    public:
    CCellularGenerator();
    ~CCellularGenerator();

    SCellularCache &get(double x, double y);
    SCellularCache &get(double x, double y, double z);
    SCellularCache &get(double x, double y, double z, double w);
    SCellularCache &get(double x, double y, double z, double w, double u, double v);

    void setSeed(unsigned int seed);


    protected:
    unsigned int m_seed;
    SCellularCache m_cache2, m_cache3, m_cache4, m_cache6;


};

};

#endif
