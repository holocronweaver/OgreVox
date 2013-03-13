#ifndef RGBA_NORMALIZE_H
#define RGBA_NORMALIZE_H
#include "rgbamodulebase.h"

namespace anl
{
    class CRGBANormalize : public CRGBAModuleBase
    {
        public:
        CRGBANormalize();
        ~CRGBANormalize();

        void setSource(CRGBAModuleBase *m);
        void setSource(float r, float g, float b, float a);

        SRGBA get(double x, double y);
        SRGBA get(double x, double y, double z);
        SRGBA get(double x, double y, double z, double w);
        SRGBA get(double x, double y, double z, double w, double u, double v);
        protected:
        CRGBAParameter m_source;
    };
}


#endif
