#ifndef IMPLICIT_SELECT_H
#define IMPLICIT_SELECT_H
#include "implicitmodulebase.h"

namespace anl
{
class CImplicitSelect : public CImplicitModuleBase
{
public:
	CImplicitSelect();
	~CImplicitSelect();
	void setLowSource(CImplicitModuleBase *b);
	void setHighSource(CImplicitModuleBase *b);
	void setControlSource(CImplicitModuleBase *b);
	void setLowSource(double v);
	void setHighSource(double v);
	void setControlSource(double v);
	void setThreshold(double t);
	void setFalloff(double f);
	void setThreshold(CImplicitModuleBase *m);
	void setFalloff(CImplicitModuleBase *m);
	double get(double x, double y);
    double get(double x, double y, double z);
	double get(double x, double y, double z, double w);
	double get(double x, double y, double z, double w, double u, double v);

    protected:
	CScalarParameter m_low, m_high, m_control;
	CScalarParameter m_threshold, m_falloff;

};
};
#endif
