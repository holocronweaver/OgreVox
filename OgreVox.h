#ifndef __OgreVox_h_
#define __OgreVox_h_

#include "BareOgre.h"
#include "VoxelSphere.h"

class OgreVox : public BareOgre
{
 public:
  OgreVox(void);
  virtual ~OgreVox(void);

 protected:
  virtual void createScene(void);

 private:
  VoxelSphere* mVoxelSphere;
};

#endif // #ifndef __OgreVox_h_
