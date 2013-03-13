#ifndef IMPLICIT_GAIN_H
#define IMPLICIT_GAIN_H
#include "implicitmodulebase.h"

namespace anl
{
    class CImplicitGain : public CImplicitModuleBase
{
    public:
    CImplicitGain(double b);
    ~CImplicitGain();

    void setSource(double v);
    void setSource(CImplicitModuleBase *b);
    void setGain(double b);
    void setGain(CImplicitModuleBase *m);

    double get(double x, double y);
    double get(double x, double y, double z);
    double get(double x, double y, double z, double w);
    double get(double x, double y, double z, double w, double u, double v);

    protected:
    CScalarParameter m_source;
    CScalarParameter m_gain;
};
};

#endif

