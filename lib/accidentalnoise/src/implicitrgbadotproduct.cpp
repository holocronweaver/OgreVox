#include "implicitrgbadotproduct.h"

namespace anl
{
    CImplicitRGBADotProduct::CImplicitRGBADotProduct() : m_source1(0,0,0,0), m_source2(0,0,0,0){}
    CImplicitRGBADotProduct::~CImplicitRGBADotProduct(){}

    void CImplicitRGBADotProduct::setSource1(CRGBAModuleBase *m){m_source1.set(m);}
    void CImplicitRGBADotProduct::setSource1(float r, float g, float b, float a){m_source1.set(r,g,b,a);}
    void CImplicitRGBADotProduct::setSource2(CRGBAModuleBase *m){m_source2.set(m);}
    void CImplicitRGBADotProduct::setSource2(float r, float g, float b, float a){m_source2.set(r,g,b,a);}
    double CImplicitRGBADotProduct::get(double x, double y)
    {
        SRGBA s1=m_source1.get(x,y), s2=m_source2.get(x,y);
        return s1[0]*s2[0] + s1[1]*s2[1] + s1[2]*s2[2] + s1[3]*s2[3];
    }
    double CImplicitRGBADotProduct::get(double x, double y, double z)
    {
        SRGBA s1=m_source1.get(x,y,z), s2=m_source2.get(x,y,z);
        return s1[0]*s2[0] + s1[1]*s2[1] + s1[2]*s2[2] + s1[3]*s2[3];
    }
    double CImplicitRGBADotProduct::get(double x, double y, double z, double w)
    {
        SRGBA s1=m_source1.get(x,y,z,w), s2=m_source2.get(x,y,z,w);
        return s1[0]*s2[0] + s1[1]*s2[1] + s1[2]*s2[2] + s1[3]*s2[3];
    }
    double CImplicitRGBADotProduct::get(double x, double y, double z, double w, double u, double v)
    {
        SRGBA s1=m_source1.get(x,y,z,w,u,v), s2=m_source2.get(x,y,z,w,u,v);
        return s1[0]*s2[0] + s1[1]*s2[1] + s1[2]*s2[2] + s1[3]*s2[3];
    }
};
