#include "rgbargbatohsv.h"
#include "hsv.h"

namespace anl
{
    CRGBARGBAToHSV::CRGBARGBAToHSV() : m_source(0,0,0,0){}

    void CRGBARGBAToHSV::setSource(CRGBAModuleBase *m)
    {
        m_source.set(m);
    }
    void CRGBARGBAToHSV::setSource(float r, float g, float b, float a)
    {
        m_source.set(r,g,b,a);
    }

    SRGBA CRGBARGBAToHSV::get(double x, double y)
    {
        SRGBA s=m_source.get(x,y);
        SRGBA d;
        RGBAtoHSV(s,d);
        return d;
    }
    SRGBA CRGBARGBAToHSV::get(double x, double y, double z)
    {
        SRGBA s=m_source.get(x,y,z);
        SRGBA d;
        RGBAtoHSV(s,d);
        return d;
    }
    SRGBA CRGBARGBAToHSV::get(double x, double y, double z, double w)
    {
        SRGBA s=m_source.get(x,y,z,w);
        SRGBA d;
        RGBAtoHSV(s,d);
        return d;
    }
    SRGBA CRGBARGBAToHSV::get(double x, double y, double z, double w, double u, double v)
    {
        SRGBA s=m_source.get(x,y,z,w,u,v);
        SRGBA d;
        RGBAtoHSV(s,d);
        return d;
    }
};
