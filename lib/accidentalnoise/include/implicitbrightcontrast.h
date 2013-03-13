#ifndef IMPLICIT_BRIGHTCONTRAST_H
#define IMPLICIT_BRIGHTCONTRAST_H

#include "implicitmodulebase.h"
namespace anl
{
    class CImplicitBrightContrast : public CImplicitModuleBase
    {
        public:
        CImplicitBrightContrast();
        ~CImplicitBrightContrast();

        void setSource(CImplicitModuleBase *m);
        void setSource(double v);

        void setBrightness(double b);
        void setContrastThreshold(double t);
        void setContrastFactor(double t);
        void setBrightness(CImplicitModuleBase *m);
        void setContrastThreshold(CImplicitModuleBase *m);
        void setContrastFactor(CImplicitModuleBase *m);

        double get(double x, double y);
        double get(double x, double y, double z);
        double get(double x, double y, double z, double w);
        double get(double x, double y, double z, double w, double u, double v);
        protected:
        CScalarParameter m_source;
        CScalarParameter m_bright, m_threshold, m_factor;
    };
};

#endif
