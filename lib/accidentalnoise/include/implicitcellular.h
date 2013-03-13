#ifndef IMPLICIT_CELLULAR_H
#define IMPLICIT_CELLULAR_H
#include "implicitmodulebase.h"
#include "cellulargen.h"

namespace anl
{
class CImplicitCellular : public CImplicitModuleBase
{
    public:
    CImplicitCellular();
    CImplicitCellular(double a, double b, double c, double d);
    ~CImplicitCellular(){}

    void setCoefficients(double a, double b, double c, double d);
    void setCellularSource(CCellularGenerator *m);

    double get(double x, double y);
    double get(double x, double y, double z);
    double get(double x, double y, double z, double w);
    double get(double x, double y, double z, double w, double u, double v);

    protected:
    CCellularGenerator *m_generator;
    double m_coefficients[4];
};
};

#endif
