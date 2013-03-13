#include "implicittranslatedomain.h"

namespace anl
{
    CImplicitTranslateDomain::CImplicitTranslateDomain() : m_source(0),
        m_ax(0), m_ay(0), m_az(0), m_aw(0), m_au(0), m_av(0){}
    CImplicitTranslateDomain::~CImplicitTranslateDomain(){}

    void CImplicitTranslateDomain::setXAxisSource(CImplicitModuleBase *m)
    {
        m_ax.set(m);
    }
    void CImplicitTranslateDomain::setYAxisSource(CImplicitModuleBase *m)
    {
        m_ay.set(m);
    }
    void CImplicitTranslateDomain::setZAxisSource(CImplicitModuleBase *m)
    {
        m_az.set(m);
    }
    void CImplicitTranslateDomain::setWAxisSource(CImplicitModuleBase *m)
    {
        m_aw.set(m);
    }
    void CImplicitTranslateDomain::setUAxisSource(CImplicitModuleBase *m)
    {
        m_au.set(m);
    }
    void CImplicitTranslateDomain::setVAxisSource(CImplicitModuleBase *m)
    {
        m_av.set(m);
    }
    void CImplicitTranslateDomain::setXAxisSource(double v)
    {
        m_ax.set(v);
    }
    void CImplicitTranslateDomain::setYAxisSource(double v)
    {
        m_ay.set(v);
    }
    void CImplicitTranslateDomain::setZAxisSource(double v)
    {
        m_az.set(v);
    }
    void CImplicitTranslateDomain::setWAxisSource(double v)
    {
        m_aw.set(v);
    }
    void CImplicitTranslateDomain::setUAxisSource(double v)
    {
        m_au.set(v);
    }
    void CImplicitTranslateDomain::setVAxisSource(double v)
    {
        m_av.set(v);
    }
    void CImplicitTranslateDomain::setSource(CImplicitModuleBase *m)
    {
        m_source.set(m);
    }
    void CImplicitTranslateDomain::setSource(double v)
    {
        m_source.set(v);
    }

    double CImplicitTranslateDomain::get(double x, double y)
    {
        return m_source.get(x+m_ax.get(x,y), y+m_ay.get(x,y));
    }
    double CImplicitTranslateDomain::get(double x, double y, double z)
    {
        return m_source.get(x+m_ax.get(x,y,z), y+m_ay.get(x,y,z), z+m_az.get(x,y,z));
    }
    double CImplicitTranslateDomain::get(double x, double y, double z, double w)
    {
        return m_source.get(x+m_ax.get(x,y,z,w), y+m_ay.get(x,y,z,w), z+m_az.get(x,y,z,w), w+m_aw.get(x,y,z,w));
    }
    double CImplicitTranslateDomain::get(double x, double y, double z, double w, double u, double v)
    {
        return m_source.get(x+m_ax.get(x,y,z,w,u,v), y+m_ay.get(x,y,z,w,u,v), z+m_az.get(x,y,z,w,u,v),
            w+m_aw.get(x,y,z,w,u,v), u+m_au.get(x,y,z,w,u,v), v+m_av.get(x,y,z,w,u,v));
    }
};
