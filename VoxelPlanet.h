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
#include "OgreSceneManager.h"
#include "OgreSceneNode.h"

#include "PolyVoxCore/CubicSurfaceExtractorWithNormals.h"
#include "PolyVoxCore/CubicSurfaceExtractor.h"
#include "PolyVoxCore/LargeVolume.h"
#include "PolyVoxCore/MarchingCubesSurfaceExtractor.h"
#include "PolyVoxCore/Raycast.h"
#include "PolyVoxCore/SurfaceMesh.h"

// Accidental Noise Library
#include "anl.h" 

#include "Raycast.h"

typedef uint8_t BYTE;

class VoxelPlanet
{
 public:
    VoxelPlanet(const Ogre::String& name, Ogre::SceneNode* cameraSceneNode,
                Ogre::SceneManager* sceneMgr);
    virtual ~VoxelPlanet();

    virtual void generate(float radius, Ogre::Vector3 origin);
    virtual void enqueueSurfaceExtraction(PolyVox::Region region);
    
    /**
     * Surface extraction occurs on a cubic region whose center follows
     * the camera's position.  As the camera moves, so too does
     * the cube.  Once the camera has moved a critical distance,
     * the surface of the relocated cube is extracted.
     * So long as the cube is sufficiently large and the surface extraction
     * occurs often enough (i.e., the re-extraction distance is small),
     * the player should never see popping or a clipped voxel surface.
     **/
    virtual void extractSurface();//PolyVox::Region region);
    //virtual void update(PolyVox::Vector3DInt32 chunk);

    virtual Ogre::Vector3 getOrigin() { mVolumeOrigin; }
    virtual float getRadius() { mRadius; }

    // Render the next queued region and enqueue a new region if the camera
    // has moved far enough from the voxel of its previous positions.
    void update(Ogre::SceneManager* sceneMgr);

    Raycast raycast(
        Ogre::Vector3 position, Ogre::Vector3 direction, float distance);

    Ogre::SceneNode* getSceneNode() { return mSceneNode; }
    
 protected:
    PolyVox::LargeVolume<BYTE>* mVolumeData;
    Ogre::Vector3 mVolumeOrigin;
    float mRadius;

    static void requestVolume(const PolyVox::ConstVolumeProxy<BYTE>& volume,
                              const PolyVox::Region& region);
    static void storeVolume(const PolyVox::ConstVolumeProxy<BYTE>& volume,
                            const PolyVox::Region& region);
    static void generateNoise();

    // Get the chunk the camera currently occupies.
    PolyVox::Vector3DInt32 getCurrentChunk();

    // Execute the next pending surface extraction and add its mesh to
    // the render queue.
    void render(Ogre::SceneManager* sceneMgr);

    // If the camera has moved far enough from the voxel it occupied
    // during the previous surface extraction, enqueue a new region to
    // be extracted.
    void page();

 private:
    std::condition_variable extractionQueueNotEmpty;
    bool shutdownThreads;

    std::vector<std::thread> mThreads;
    std::mutex mSurfaceExtractionMutex;
    std::mutex mSurfaceExtractionQueueMutex;
    std::mutex mSurfaceRenderMutex;
    std::queue<PolyVox::Region> mSurfaceExtractionQueue;
    std::queue<Ogre::ManualObject*> mSurfaceRenderQueue;

    static const bool DEBUG = true;

    Ogre::SceneNode* mSceneNode;
    Ogre::SceneNode* mCamNode;

    // Most recently visited voxel.
    PolyVox::Vector3DInt32 mVoxel;

    // Distance (in voxel chunks) the camera must move before the
    // surface is re-extracted.
    double mPagingDistance;

    // Size (in voxel chunks) of the cubic region whose surface is
    // extracted.
    PolyVox::Vector3DInt32 mRegionDims;

    static const int mChunkSize = 32;

    void _extractSurface(PolyVox::Region region, Ogre::ManualObject* man_obj);
};

#endif // #ifndef __VoxelPlanet_h_
