#include "implicitbias.h"
#include "utility.h"

namespace anl
{
    CImplicitBias::CImplicitBias(double b) : m_source(0), m_bias(b){}
    CImplicitBias::~CImplicitBias(){}

    void CImplicitBias::setSource(CImplicitModuleBase *b){m_source.set(b);}
    void CImplicitBias::setSource(double v){m_source.set(v);}
    void CImplicitBias::setBias(double b){m_bias.set(b);}
    void CImplicitBias::setBias(CImplicitModuleBase *m){m_bias.set(m);}

    double CImplicitBias::get(double x, double y)
    {
        double va=m_source.get(x,y);
        return bias(m_bias.get(x,y), va);
    }

    double CImplicitBias::get(double x, double y, double z)
    {
        double va=m_source.get(x,y,z);
        return bias(m_bias.get(x,y,z), va);
    }

    double CImplicitBias::get(double x, double y, double z, double w)
    {
        double va=m_source.get(x,y,z,w);
        return bias(m_bias.get(x,y,z,w), va);
    }

    double CImplicitBias::get(double x, double y, double z, double w, double u, double v)
    {
        double va=m_source.get(x,y,z,w, u, v);
        return bias(m_bias.get(x,y,z,w,u,v), va);
    }
};
