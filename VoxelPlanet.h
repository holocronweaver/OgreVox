// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// Copyrighted under the MPLv2 and LGPLv3 by Jesse Johnson.

#ifndef __VoxelPlanet_h_
#define __VoxelPlanet_h_

//#include <stack>
#include <queue>
#include <vector>

// C++11 libraries
#include <condition_variable>
#include <mutex>
#include <thread>

#include "OgreManualObject.h"

#include "PolyVoxCore/CubicSurfaceExtractorWithNormals.h"
#include "PolyVoxCore/CubicSurfaceExtractor.h"
#include "PolyVoxCore/MarchingCubesSurfaceExtractor.h"
#include "PolyVoxCore/SurfaceMesh.h"
#include "PolyVoxCore/LargeVolume.h"

// Accidental Noise Library
#include "anl.h" 

typedef uint8_t BYTE;

class VoxelPlanet //: public Ogre::ManualObject
{
 public:
  VoxelPlanet(const Ogre::String& name);
  virtual ~VoxelPlanet();

  virtual void generate(float radius, Ogre::Vector3 origin);
  virtual void enqueueSurfaceExtraction(PolyVox::Region region);
  virtual void extractSurface();//PolyVox::Region region);
  //virtual void update(PolyVox::Vector3DInt32 chunk);

  virtual Ogre::Vector3 getOrigin();
  virtual float getRadius();

  static const int chunkSize = 32;

  std::vector<std::thread> threads;
  std::mutex surfaceExtractionMutex;
  std::mutex surfaceExtractionQueueMutex;
  std::mutex surfaceRenderMutex;
  std::queue<PolyVox::Region> surfaceExtractionQueue;
  std::queue<Ogre::ManualObject*> surfaceRenderQueue;
  // bool newMesh;


 protected:
  PolyVox::LargeVolume<BYTE>* volumeData;
  Ogre::Vector3 volumeOrigin;
  float radius;

  static void requestVolume(const PolyVox::ConstVolumeProxy<BYTE>& volume,
                     const PolyVox::Region& region);
  static void storeVolume(const PolyVox::ConstVolumeProxy<BYTE>& volume,
                   const PolyVox::Region& region);
  static void generateNoise();

 private:
  std::condition_variable extractionQueueNotEmpty;
  bool shutdownThreads;
  void _extractSurface(PolyVox::Region region, Ogre::ManualObject* man_obj);

  //Ogre::ManualObject* tempManualObject;

  static const bool DEBUG = false;
};

#endif // #ifndef __VoxelPlanet_h_
