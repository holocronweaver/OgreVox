#include "implicitpow.h"
#include <math.h>

namespace anl
{
    CImplicitPow::CImplicitPow() : m_source(0), m_power(1){}
    CImplicitPow::~CImplicitPow(){}

    void CImplicitPow::setSource(double v){m_source.set(v);}
    void CImplicitPow::setSource(CImplicitModuleBase *m){m_source.set(m);}
    void CImplicitPow::setPower(double v){m_power.set(v);}
    void CImplicitPow::setPower(CImplicitModuleBase *m){m_power.set(m);}

    double CImplicitPow::get(double x, double y)
    {
        return pow(m_source.get(x,y), m_power.get(x,y));
    }
    double CImplicitPow::get(double x, double y, double z)
    {
        return pow(m_source.get(x,y,z), m_power.get(x,y,z));
    }
    double CImplicitPow::get(double x, double y, double z, double w)
    {
        return pow(m_source.get(x,y,z,w), m_power.get(x,y,z,w));
    }
    double CImplicitPow::get(double x, double y, double z, double w, double u, double v)
    {
        return pow(m_source.get(x,y,z,w,u,v), m_power.get(x,y,z,w,u,v));
    }
};
