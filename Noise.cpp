#include "Noise.h"

Noise::Noise()
{
}

Noise::~Noise()
{
}

void Noise::generate(float radius)
{
  anl::CMWC4096 rnd;
  rnd.setSeedTime();

  anl::CImplicitSphere sphere;
  sphere.setRadius(radius);
  

  /********
  terraintree=
    {
      {name="ground_gradient",          type="gradient",           x1=0, x2=0, y1=0, y2=1},
      {name="ground_shape_fractal",     type="fractal",            fractaltype=anl.FBM, basistype=anl.GRADIENT, interptype=anl.QUINTIC, octaves=6, frequency=2},
      {name="ground_scale",             type="scaleoffset",        scale=0.5, offset=0, source="ground_shape_fractal"},
      {name="ground_scale_y",           type="scaledomain",        source="ground_scale", scaley=0},
      {name="ground_perturb",           type="translatedomain",    source="ground_gradient", ty="ground_scale_y"},
	
      {name="ground_select",            type="select",             low=0, high=1, threshold=0.5, control="ground_perturb"}
    }
  */
}
