#include "implicitsawtooth.h"
#include <cmath>
namespace anl
{
CImplicitSawtooth::CImplicitSawtooth(double period) :
    m_source(0), m_period(period)
{
}
CImplicitSawtooth::~CImplicitSawtooth()
{
}

void CImplicitSawtooth::setSource(CImplicitModuleBase *s)
{
    m_source.set(s);
}

void CImplicitSawtooth::setSource(double s)
{
    m_source.set(s);
}

void CImplicitSawtooth::setPeriod(CImplicitModuleBase *p)
{
    m_period.set(p);
}

void CImplicitSawtooth::setPeriod(double p)
{
    m_period.set(p);
}

double CImplicitSawtooth::get(double x, double y)
{
    double val=m_source.get(x,y);
    double p=m_period.get(x,y);

    return 2.0*(val/p - floor(0.5 + val/p));
}

double CImplicitSawtooth::get(double x, double y, double z)
{
    double val=m_source.get(x,y,z);
    double p=m_period.get(x,y,z);

    return 2.0*(val/p - floor(0.5 + val/p));
}

double CImplicitSawtooth::get(double x, double y, double z, double w)
{
    double val=m_source.get(x,y,z,w);
    double p=m_period.get(x,y,z,w);

    return 2.0*(val/p - floor(0.5 + val/p));
}

double CImplicitSawtooth::get(double x, double y, double z, double w, double u, double v)
{
    double val=m_source.get(x,y,z,w,u,v);
    double p=m_period.get(x,y,z,w,u,v);

    return 2.0*(val/p - floor(0.5 + val/p));
}
};
