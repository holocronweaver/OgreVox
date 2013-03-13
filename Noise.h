#ifndef __Noise_h_
#define __Noise_h_

#include "anl.h"

typedef uint8_t BYTE;

class Noise
{
 public:
  Noise();
  virtual ~Noise();

  virtual void generate(float radius);
 
 private:
  //PolyVox::SimpleVolume<BYTE>* mVolumeData;
};

#endif // #ifndef __Noise_h_
