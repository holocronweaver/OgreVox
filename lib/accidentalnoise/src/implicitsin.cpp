#include "implicitsin.h"
#include <math.h>

namespace anl
{
    CImplicitSin::CImplicitSin() : m_source(0){}
    CImplicitSin::~CImplicitSin(){}

    void CImplicitSin::setSource(double v){m_source.set(v);}
    void CImplicitSin::setSource(CImplicitModuleBase *m){m_source.set(m);}

    double CImplicitSin::get(double x, double y)
    {
        return sin(m_source.get(x,y));
    }
    double CImplicitSin::get(double x, double y, double z)
    {
        return sin(m_source.get(x,y,z));
    }
    double CImplicitSin::get(double x, double y, double z, double w)
    {
        return sin(m_source.get(x,y,z,w));
    }
    double CImplicitSin::get(double x, double y, double z, double w, double u, double v)
    {
        return sin(m_source.get(x,y,z,w,u,v));
    }
};

