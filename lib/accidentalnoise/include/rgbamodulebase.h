#ifndef RGBA_MODULE_BASE_H
#define RGBA_MODULE_BASE_H
//#include "rgba.h"
#include "vectortypes.h"

namespace anl
{
    class CRGBAModuleBase
    {
        public:
        CRGBAModuleBase(){}
        virtual ~CRGBAModuleBase(){}

        void setSeed(unsigned int){};

        virtual SRGBA get(double x, double y)=0;
        virtual SRGBA get(double x, double y, double z)=0;
        virtual SRGBA get(double x, double y, double z, double w)=0;
        virtual SRGBA get(double x, double y, double z, double w, double u, double v)=0;
    };

    class CRGBAParameter
    {
        public:
        CRGBAParameter() : m_source(0), m_constant(0,0,0,0){}
        CRGBAParameter(float r, float g, float b, float a) : m_source(0), m_constant(r,g,b,a){}

        void set(float r, float g, float b, float a)
        {
            m_source=0;
            m_constant=SRGBA(r,g,b,a);
        }
        void set(CRGBAModuleBase *m)
        {
            m_source=m;
        }

        SRGBA get(double x, double y)
        {
            if(m_source) return m_source->get(x,y);
            else return m_constant;
        }

        SRGBA get(double x, double y, double z)
        {
            if(m_source) return m_source->get(x,y,z);
            else return m_constant;
        }

        SRGBA get(double x, double y, double z, double w)
        {
            if(m_source) return m_source->get(x,y,z,w);
            else return m_constant;
        }

        SRGBA get(double x, double y, double z, double w, double u, double v)
        {
            if(m_source) return m_source->get(x,y,z,w,u,v);
            else return m_constant;
        }


        protected:
        CRGBAModuleBase *m_source;
        SRGBA m_constant;

    };

};

#endif
