#include "implicitfloor.h"
#include <math.h>

namespace anl
{
    CImplicitFloor::CImplicitFloor() : m_source(0){}
    CImplicitFloor::~CImplicitFloor(){}

    void CImplicitFloor::setSource(double v){m_source.set(v);}
    void CImplicitFloor::setSource(CImplicitModuleBase *m){m_source.set(m);}
    double CImplicitFloor::get(double x, double y)
    {
        return floor(m_source.get(x,y));
    }
    double CImplicitFloor::get(double x, double y, double z)
    {
        return floor(m_source.get(x,y,z));
    }
    double CImplicitFloor::get(double x, double y, double z, double w)
    {
        return floor(m_source.get(x,y,z,w));
    }
    double CImplicitFloor::get(double x, double y, double z, double w, double u, double v)
    {
        return floor(m_source.get(x,y,z,w,u,v));
    }
};
