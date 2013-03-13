// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// Copyrighted under the MPLv2 and LGPLv3 by Jesse Johnson.
#ifndef __VoxelSphere_h_
#define __VoxelSphere_h_

#include "OgreManualObject.h"

#include "PolyVoxCore/CubicSurfaceExtractorWithNormals.h"
#include "PolyVoxCore/MarchingCubesSurfaceExtractor.h"
#include "PolyVoxCore/SurfaceMesh.h"
#include "PolyVoxCore/SimpleVolume.h"

typedef uint8_t BYTE;

class VoxelSphere : public Ogre::ManualObject
{
 public:
  VoxelSphere(const Ogre::String& name);
  virtual ~VoxelSphere();

  virtual void generate(float radius);
  virtual void mesh();
 
 private:
  PolyVox::SimpleVolume<BYTE>* mVolumeData;
};

#endif // #ifndef __VoxelSphere_h_
