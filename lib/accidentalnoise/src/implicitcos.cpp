#include "implicitcos.h"
#include <math.h>

namespace anl
{
    CImplicitCos::CImplicitCos() : m_source(0){}
    CImplicitCos::~CImplicitCos(){}

    void CImplicitCos::setSource(double v){m_source.set(v);}
    void CImplicitCos::setSource(CImplicitModuleBase *m){m_source.set(m);}

    double CImplicitCos::get(double x, double y)
    {
        return cos(m_source.get(x,y));
    }
    double CImplicitCos::get(double x, double y, double z)
    {
        return cos(m_source.get(x,y,z));
    }
    double CImplicitCos::get(double x, double y, double z, double w)
    {
        return cos(m_source.get(x,y,z,w));
    }
    double CImplicitCos::get(double x, double y, double z, double w, double u, double v)
    {
        return cos(m_source.get(x,y,z,w,u,v));
    }
};
