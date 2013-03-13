// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// Copyrighted under the MPLv2 and LGPLv3 by Jesse Johnson.

#ifndef __VoxelTerrain_h_
#define __VoxelTerrain_h_

#include "OgreManualObject.h"

#include "PolyVoxCore/CubicSurfaceExtractorWithNormals.h"
#include "PolyVoxCore/CubicSurfaceExtractor.h"
#include "PolyVoxCore/MarchingCubesSurfaceExtractor.h"
#include "PolyVoxCore/SurfaceMesh.h"
#include "PolyVoxCore/LargeVolume.h"

// Accidental Noise Library
#include "anl.h" 

typedef uint8_t BYTE;

class VoxelTerrain : public Ogre::ManualObject
{
 public:
  VoxelTerrain(const Ogre::String& name);
  virtual ~VoxelTerrain();

  virtual void generate(float radius);
  virtual void mesh();
 
 private:
  PolyVox::LargeVolume<BYTE>* mVolumeData;
  static void requestVolume(const PolyVox::ConstVolumeProxy<BYTE>& volume,
                     const PolyVox::Region& region);
  static void storeVolume(const PolyVox::ConstVolumeProxy<BYTE>& volume,
                   const PolyVox::Region& region);
  static void generateNoise();
};

#endif // #ifndef __VoxelTerrain_h_
