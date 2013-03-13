#include "rgbaimplicitgrayscale.h"

namespace anl
{
    CRGBAImplicitGrayscale::CRGBAImplicitGrayscale() : m_source(0){}
    CRGBAImplicitGrayscale::~CRGBAImplicitGrayscale(){}
    void CRGBAImplicitGrayscale::setSource(CImplicitModuleBase *m)
    {
        m_source=m;
    }

    SRGBA CRGBAImplicitGrayscale::get(double x, double y)
    {
        if(m_source==0) return SRGBA(0,0,0,0);

        double val=m_source->get(x,y);
        return SRGBA((float)val,(float)val,(float)val,1.0f);
    }
    SRGBA CRGBAImplicitGrayscale::get(double x, double y, double z)
    {
        if(m_source==0) return SRGBA(0,0,0,0);

        double val=m_source->get(x,y,z);
        return SRGBA((float)val,(float)val,(float)val,1.0f);
    }

    SRGBA CRGBAImplicitGrayscale::get(double x, double y, double z, double w)
    {
        if(m_source==0) return SRGBA(0,0,0,0);

        double val=m_source->get(x,y,z,w);
        return SRGBA((float)val,(float)val,(float)val,1.0f);
    }
    SRGBA CRGBAImplicitGrayscale::get(double x, double y, double z, double w, double u, double v)
    {
        if(m_source==0) return SRGBA(0,0,0,0);

        double val=m_source->get(x,y,z,w,u,v);
        return SRGBA((float)val,(float)val,(float)val,1.0f);
    }
};
