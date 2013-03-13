// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// Copyrighted under the MPLv2 and LGPLv3 by Jesse Johnson.

#ifndef __VoxelPlanet_h_
#define __VoxelPlanet_h_

#include "OgreManualObject.h"

#include "PolyVoxCore/CubicSurfaceExtractorWithNormals.h"
#include "PolyVoxCore/CubicSurfaceExtractor.h"
#include "PolyVoxCore/MarchingCubesSurfaceExtractor.h"
#include "PolyVoxCore/SurfaceMesh.h"
#include "PolyVoxCore/LargeVolume.h"

// Accidental Noise Library
#include "anl.h" 

typedef uint8_t BYTE;

class VoxelPlanet : public Ogre::ManualObject
{
 public:
  VoxelPlanet(const Ogre::String& name);
  virtual ~VoxelPlanet();

  virtual void generate(float radius, Ogre::Vector3 origin);
  virtual void mesh();
  virtual void update(PolyVox::Vector3DInt32 chunk);

  virtual Ogre::Vector3 getOrigin();
  virtual float getRadius();

  static const int chunkSize = 32;

 protected:
  PolyVox::LargeVolume<BYTE>* mVolumeData;
  Ogre::Vector3 mVolOrigin;
  float mRadius;

  static void requestVolume(const PolyVox::ConstVolumeProxy<BYTE>& volume,
                     const PolyVox::Region& region);
  static void storeVolume(const PolyVox::ConstVolumeProxy<BYTE>& volume,
                   const PolyVox::Region& region);
  static void generateNoise();
};

#endif // #ifndef __VoxelPlanet_h_
