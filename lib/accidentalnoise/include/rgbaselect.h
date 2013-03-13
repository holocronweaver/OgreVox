#ifndef RGBA_SELECT_H
#define RGBA_SELECT_H
#include "rgbamodulebase.h"
#include "implicitmodulebase.h"

namespace anl
{
    class CRGBASelect : public CRGBAModuleBase
    {
        public:
        CRGBASelect();
        CRGBASelect(double thresh, double fall);

        void setLowSource(CRGBAModuleBase *m);
        void setHighSource(CRGBAModuleBase *m);
        void setLowSource(float r, float g, float b, float a);
        void setHighSource(float r, float g, float b, float a);
        void setControlSource(CImplicitModuleBase *m);
        void setThreshold(CImplicitModuleBase *m);
        void setFalloff(CImplicitModuleBase *m);
        void setControlSource(double v);
        void setThreshold(double v);
        void setFalloff(double v);

        SRGBA get(double x, double y);
        SRGBA get(double x, double y, double z);
        SRGBA get(double x, double y, double z, double w);
        SRGBA get(double x, double y, double z, double w, double u, double v);

        protected:
        CRGBAParameter m_low, m_high;
        CScalarParameter m_control, m_threshold, m_falloff;
    };
};

#endif
