#ifndef RGBA_BLENDOPS_H
#define RGBA_BLENDOPS_H
#include "rgbamodulebase.h"

/*********************************************************
CRGBABlendOps

    Provide raster-ops-style blending of two RGBA channels.
**********************************************************/

namespace anl
{
    enum EBlendOps
    {
        SRC1_ALPHA,
        SRC2_ALPHA,
        ONE_MINUS_SRC1_ALPHA,
        ONE_MINUS_SRC2_ALPHA,
        ONE,
        ZERO
    };

    class CRGBABlendOps : public CRGBAModuleBase
    {
        public:
        CRGBABlendOps();
        CRGBABlendOps(int src1mode, int src2mode);
        ~CRGBABlendOps();

        void setSrc1Mode(int mode);
        void setSrc2Mode(int mode);
        void setSource1(CRGBAModuleBase *m);
        void setSource2(CRGBAModuleBase *m);
        void setSource1(float r, float g, float b, float a);
        void setSource2(float r, float g, float b, float a);

        SRGBA get(double x, double y);
        SRGBA get(double x, double y, double z);
        SRGBA get(double x, double y, double z, double w);
        SRGBA get(double x, double y, double z, double w, double u, double v);

        protected:
        CRGBAParameter m_source1, m_source2;
        int m_src1blend, m_src2blend;

        SRGBA blendRGBAs(SRGBA &s1, SRGBA &s2);
    };
};

#endif
