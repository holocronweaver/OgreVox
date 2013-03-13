#include "rgbahsvtorgba.h"
#include "hsv.h"

namespace anl
{
    CRGBAHSVToRGBA::CRGBAHSVToRGBA() : m_source(0,0,0,0){}

    void CRGBAHSVToRGBA::setSource(CRGBAModuleBase *m)
    {
        m_source.set(m);
    }
    void CRGBAHSVToRGBA::setSource(float r, float g, float b, float a)
    {
        m_source.set(r,g,b,a);
    }

    SRGBA CRGBAHSVToRGBA::get(double x, double y)
    {
        SRGBA s=m_source.get(x,y);
        SRGBA d;
        HSVtoRGBA(s,d);
        return d;
    }
    SRGBA CRGBAHSVToRGBA::get(double x, double y, double z)
    {
        SRGBA s=m_source.get(x,y,z);
        SRGBA d;
        HSVtoRGBA(s,d);
        return d;
    }
    SRGBA CRGBAHSVToRGBA::get(double x, double y, double z, double w)
    {
        SRGBA s=m_source.get(x,y,z,w);
        SRGBA d;
        HSVtoRGBA(s,d);
        return d;
    }
    SRGBA CRGBAHSVToRGBA::get(double x, double y, double z, double w, double u, double v)
    {
        SRGBA s=m_source.get(x,y,z,w,u,v);
        SRGBA d;
        HSVtoRGBA(s,d);
        return d;
    }
};

