// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// Copyrighted under the MPLv2 and LGPLv3 by Jesse Johnson.
#ifndef __LargeVoxelSphere_h_
#define __LargeVoxelSphere_h_

#include "OgreManualObject.h"

#include "PolyVoxCore/CubicSurfaceExtractorWithNormals.h"
#include "PolyVoxCore/MarchingCubesSurfaceExtractor.h"
#include "PolyVoxCore/SurfaceMesh.h"
#include "PolyVoxCore/LargeVolume.h"

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
  static void requestVolume(const PolyVox::ConstVolumeProxy<BYTE>& volume,
                     const PolyVox::Region& region);
  static void storeVolume(const PolyVox::ConstVolumeProxy<BYTE>& volume,
                   const PolyVox::Region& region);
};

#endif // #ifndef __LargeVoxelSphere_h_
