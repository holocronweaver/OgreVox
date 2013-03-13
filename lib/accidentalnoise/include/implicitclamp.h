#ifndef IMPLICIT_CLAMP_H
#define IMPLICIT_CLAMP_H
#include "implicitmodulebase.h"

namespace anl
{
    class CImplicitClamp : public CImplicitModuleBase
    {
        public:
        CImplicitClamp(double low, double high);
        ~CImplicitClamp();

        void setRange(double low, double high);
        void setSource(CImplicitModuleBase *b);

        double get(double x, double y);
        double get(double x, double y, double z);
        double get(double x, double y, double z, double w);
        double get(double x, double y, double z, double w, double u, double v);

        protected:
        CImplicitModuleBase *m_source;
        double m_low, m_high;
    };
};
#endif
