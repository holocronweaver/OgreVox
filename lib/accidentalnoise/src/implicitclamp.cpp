#include "implicitclamp.h"
#include "utility.h"

namespace anl
{
    CImplicitClamp::CImplicitClamp(double low, double high) : m_source(0), m_low(low), m_high(high){}
    CImplicitClamp::~CImplicitClamp(){}

    void CImplicitClamp::setRange(double low, double high){m_low=low; m_high=high;}
    void CImplicitClamp::setSource(CImplicitModuleBase *b){m_source=b;}

    double CImplicitClamp::get(double x, double y)
    {
        if(!m_source) return 0;

        return clamp(m_source->get(x,y),m_low,m_high);
    }

    double CImplicitClamp::get(double x, double y, double z)
    {
        if(!m_source) return 0;

        return clamp(m_source->get(x,y,z),m_low,m_high);
    }

    double CImplicitClamp::get(double x, double y, double z, double w)
    {
        if(!m_source) return 0;

        return clamp(m_source->get(x,y,z,w),m_low,m_high);
    }

    double CImplicitClamp::get(double x, double y, double z, double w, double u, double v)
    {
        if(!m_source) return 0;

        return clamp(m_source->get(x,y,z,w,u,v),m_low,m_high);
    }
};
