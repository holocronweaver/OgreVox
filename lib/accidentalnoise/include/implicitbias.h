#ifndef IMPLICIT_BIAS_H
#define IMPLICIT_BIAS_H
#include "implicitmodulebase.h"

namespace anl
{
    class CImplicitBias : public CImplicitModuleBase
{
    public:
    CImplicitBias(double b);
    ~CImplicitBias();

    void setSource(CImplicitModuleBase *b);
    void setSource(double s);
    void setBias(double b);
    void setBias(CImplicitModuleBase *m);

    double get(double x, double y);
    double get(double x, double y, double z);
    double get(double x, double y, double z, double w);
    double get(double x, double y, double z, double w, double u, double v);

    protected:
    CScalarParameter m_source;
    CScalarParameter m_bias;
};
};

#endif
