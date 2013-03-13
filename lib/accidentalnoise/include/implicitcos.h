#ifndef IMPLICIT_COS_H
#define IMPLICIT_COS_H
#include "implicitmodulebase.h"

namespace anl
{
    class CImplicitCos : public CImplicitModuleBase
    {
        public:
        CImplicitCos();
        ~CImplicitCos();

        void setSource(double v);
        void setSource(CImplicitModuleBase *m);

        double get(double x, double y);
        double get(double x, double y, double z);
        double get(double x, double y, double z, double w);
        double get(double x, double y, double z, double w, double u, double v);

        protected:
        CScalarParameter m_source;
    };
};

#endif
