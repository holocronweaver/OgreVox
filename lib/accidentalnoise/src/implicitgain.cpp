#include "implicitgain.h"
#include "utility.h"

namespace anl
{
    CImplicitGain::CImplicitGain(double b) : m_source(0), m_gain(b){}
    CImplicitGain::~CImplicitGain(){}

    void CImplicitGain::setSource(double v)
    {
        m_source.set(v);
    }
    void CImplicitGain::setSource(CImplicitModuleBase *b){m_source.set(b);}
    void CImplicitGain::setGain(double b){m_gain.set(b);}
    void CImplicitGain::setGain(CImplicitModuleBase *m){m_gain.set(m);}

    double CImplicitGain::get(double x, double y)
    {
        double v=m_source.get(x,y);
        return gain(m_gain.get(x,y), v);
    }

    double CImplicitGain::get(double x, double y, double z)
    {

        double v=m_source.get(x,y,z);
        return gain(m_gain.get(x,y,z), v);
    }

    double CImplicitGain::get(double x, double y, double z, double w)
    {

        double v=m_source.get(x,y,z,w);
        return gain(m_gain.get(x,y,z,w), v);
    }

    double CImplicitGain::get(double x, double y, double z, double w, double u, double v)
    {
        double va=m_source.get(x,y,z,w, u, v);
        return gain(m_gain.get(x,y,z,w,u,v), va);
    }
};
