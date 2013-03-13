#ifndef RGBA_BLEND_H
#define RGBA_BLEND_H
#include "rgbamodulebase.h"
#include "implicitmodulebase.h"

namespace anl
{
    class CRGBABlend : public CRGBAModuleBase
    {
        public:
        CRGBABlend();
        ~CRGBABlend();

        void setLowSource(CRGBAModuleBase *m);
        void setHighSource(CRGBAModuleBase *m);
        void setLowSource(float r, float g, float b, float a);
        void setHighSource(float r, float g, float b, float a);
        void setControlSource(CImplicitModuleBase *m);
        void setControlSource(double v);

        SRGBA get(double x, double y);
        SRGBA get(double x, double y, double z);
        SRGBA get(double x, double y, double z, double w);
        SRGBA get(double x, double y, double z, double w, double u, double v);

        protected:
        CRGBAParameter m_low, m_high;
        CScalarParameter m_control;
    };
};

#endif
