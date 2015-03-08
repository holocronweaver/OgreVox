// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// Copyrighted under the MPLv2 and LGPLv3 by Jesse Johnson.
#ifndef __LargeVoxelSphere_h_
#define __LargeVoxelSphere_h_

#include "OgreManualObject.h"

#include "PolyVox/CubicSurfaceExtractor.h"
#include "PolyVox/MarchingCubesSurfaceExtractor.h"
#include "PolyVox/PagedVolume.h"

typedef uint8_t BYTE;

class LargeVoxelSphere : public Ogre::ManualObject
{
 public:
  LargeVoxelSphere(const Ogre::String& name);
  virtual ~LargeVoxelSphere();

  virtual void generate(float radius);
  virtual void mesh();
 
 private:
  PolyVox::LargeVolume<BYTE>* mVolumeData;
  //static PolyVox::Vector3DFloat mVolCenter;
  //static int mRadius;
    
    class VoxelSpherePager : public PolyVox::PagedVolume<BYTE>::Pager
    {
        void pageIn(const PolyVox::Region& region, PolyVox::PagedVolume<BYTE>::Chunk* chunk);
        void pageOut(const PolyVox::Region& region, PolyVox::PagedVolume<BYTE>::Chunk* chunk);
    };
};

#endif // #ifndef __LargeVoxelSphere_h_
