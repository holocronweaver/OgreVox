#include "cellulargen.h"
#include "noise_gen.h"
namespace anl
{
 CCellularGenerator::CCellularGenerator()
 {
     setSeed(1000);
 }

CCellularGenerator::~CCellularGenerator()
{
}

SCellularCache &CCellularGenerator::get(double x, double y)
{
    if(!m_cache2.valid || x!=m_cache2.x || y!=m_cache2.y)
    {
        cellular_function2D(x,y,m_seed,m_cache2.f,m_cache2.d);
        m_cache2.x=x;
        m_cache2.y=y;
        m_cache2.valid=true;
    }
    return m_cache2;
}

SCellularCache &CCellularGenerator::get(double x, double y, double z)
{
    if(!m_cache3.valid || x!=m_cache3.x || y!=m_cache3.y || z!=m_cache3.z)
    {
        cellular_function3D(x,y,z,m_seed,m_cache3.f,m_cache3.d);
        m_cache3.x=x;
        m_cache3.y=y;
        m_cache3.z=z;
        m_cache3.valid=true;
    }
    return m_cache3;
}

SCellularCache &CCellularGenerator::get(double x, double y, double z, double w)
{
    if(!m_cache4.valid || x!=m_cache4.x || y!=m_cache4.y || z!=m_cache4.z || w!=m_cache4.w)
    {
        cellular_function4D(x,y,z,w,m_seed,m_cache4.f,m_cache4.d);
        m_cache4.x=x;
        m_cache4.y=y;
        m_cache4.z=z;
        m_cache4.w=w;
        m_cache4.valid=true;
    }
    return m_cache4;
}

SCellularCache &CCellularGenerator::get(double x, double y, double z, double w, double u, double v)
{
    if(!m_cache6.valid || x!=m_cache6.x || y!=m_cache6.y || z!=m_cache6.z || w!=m_cache6.w || u!=m_cache6.u || v!=m_cache6.v)
    {
        cellular_function6D(x,y,z,w,u,v,m_seed,m_cache6.f,m_cache6.d);
        m_cache6.x=x;
        m_cache6.y=y;
        m_cache6.z=z;
        m_cache6.w=w;
        m_cache6.u=u;
        m_cache6.v=v;
        m_cache6.valid=true;
    }

    return m_cache6;
}

void CCellularGenerator::setSeed(unsigned int seed)
{
    m_seed=seed;
    m_cache2.valid=false;
    m_cache3.valid=false;
    m_cache4.valid=false;
    m_cache6.valid=false;
}

};
