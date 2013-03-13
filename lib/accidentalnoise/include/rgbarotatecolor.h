#ifndef RGBA_ROTATECOLOR_H
#define RGBA_ROTATECOLOR_H
#include "rgbamodulebase.h"
#include "implicitmodulebase.h"

namespace anl
{
    class CRGBARotateColor : public CRGBAModuleBase
    {
        public:
        CRGBARotateColor();
        void setAxis(double ax, double ay, double az);
        void setAxis(CImplicitModuleBase *ax, CImplicitModuleBase *ay, CImplicitModuleBase *az);
        void setAxisX(double ax);
        void setAxisY(double ay);
        void setAxisZ(double az);
        void setAxisX(CImplicitModuleBase *ax);
        void setAxisY(CImplicitModuleBase *ay);
        void setAxisZ(CImplicitModuleBase *az);

        void setNormalizeAxis(bool n){m_normalizeaxis=n;}

        void setAngle(double a);
        void setAngle(CImplicitModuleBase *a);

        void setSource(CRGBAModuleBase *m);
        void setSource(float r, float g, float b, float a);

        SRGBA get(double x, double y);
        SRGBA get(double x, double y, double z);
        SRGBA get(double x, double y, double z, double w);
        SRGBA get(double x, double y, double z, double w, double u, double v);

        protected:
        CScalarParameter m_ax, m_ay, m_az, m_angledeg;
        CRGBAParameter m_source;
        bool m_normalizeaxis;
        double m_rotmatrix[3][3];
        void calculateRotMatrix(double x, double y);
        void calculateRotMatrix(double x, double y, double z);
        void calculateRotMatrix(double x, double y, double z, double w);
        void calculateRotMatrix(double x, double y, double z, double w, double u, double v);
    };
};

#endif
