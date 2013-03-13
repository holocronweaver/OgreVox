#ifndef IMPLICIT_NORMALIZE_COORDS_H
#define IMPLICIT_NORMALIZE_COORDS_H

#include "implicitmodulebase.h"

namespace anl
{
    class CImplicitNormalizeCoords : public CImplicitModuleBase
    {
        public:
        CImplicitNormalizeCoords();
        CImplicitNormalizeCoords(float length);

        void setSource(double v);
        void setSource(CImplicitModuleBase *v);

        void setLength(double v);
        void setLength(CImplicitModuleBase *v);

        double get(double x, double y);
        double get(double x, double y, double z);
        double get(double x, double y, double z, double w);
        double get(double x, double y, double z, double w, double u, double v);

        protected:
        CScalarParameter m_source;
        CScalarParameter m_length;
    };
};

#endif
