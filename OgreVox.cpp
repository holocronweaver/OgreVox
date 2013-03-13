// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// Copyrighted under the MPLv2 and LGPLv3 by Jesse Johnson.

#include "OgreVox.h"

#include <OgreCamera.h>
#include <OgreConfigFile.h>
#include <OgreEntity.h>
#include <OgreException.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreWindowEventUtilities.h>

OgreVox::OgreVox()
{
}

OgreVox::~OgreVox()
{
}

//------------------------------------------------------------------------------
// Load PolyVox chunks within player view distance as player moves around. 
bool OgreVox::frameRenderingQueued(const Ogre::FrameEvent& evt) {
  bool repeat_loop = BareOgre::frameRenderingQueued(evt);

  Ogre::Vector3 pos = mCamNode->getPosition() - mVoxelPlanetNode->getPosition();

  Ogre::Vector3 chunk(
    (Ogre::Real) floor(pos.x / mVoxelPlanet->chunkSize),
    (Ogre::Real) floor(pos.y / mVoxelPlanet->chunkSize),
    (Ogre::Real) floor(pos.z / mVoxelPlanet->chunkSize)
  );

  if (chunk == mChunk) { 
    return repeat_loop;
  }

  std::cout << "current pos: " << pos << std::endl; 
  std::cout << "current chunk: " << chunk << std::endl; 

  //Ogre::Vector3 origin(0,0,0);
  mChunk = chunk;
  //mVoxelPlanet->update(chunk);
  for(int z = -mViewRadius; z <= mViewRadius; z++) {
    for(int y = -mViewRadius; y <= mViewRadius; y++) {
      for(int x = -mViewRadius; x <= mViewRadius; x++) {
        // PolyVox::Vector3DInt16 current(x,y,z);
        // double distance = (current - origin).length();
        // if(distance <= mViewRadius) {
        //   pageRegionMesh(Ogre::Vector3(pos.x + x,pos.y + y, pos.z + z));
        Ogre::ManualObject manobj = mVoxelPlanet->update(
          PolyVox::Vector3DInt32(pos.x + x,
                                 pos.y + y, 
                                 pos.z + z));
        // Add manual object to scene.
        mSceneMgr->getRootSceneNode()->createChildSceneNode("VoxelPlanetNode"+);
        
        // }
      }
    }
  }

  return repeat_loop;
}


void OgreVox::createScene()
{
  // Create SceneManager.
  mSceneMgr = mRoot->createSceneManager("DefaultSceneManager");

  // Create a camera.
  mCamera = mSceneMgr->createCamera("PlayerCam");
  mCamera->setPosition(Ogre::Vector3(0,0,180));
  mCamera->lookAt(Ogre::Vector3(0,0,-3));
  mCamera->setNearClipDistance(5);
  mCamNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode");
  mCamNode->attachObject(mCamera);

  // Create one viewport, entire window.
  Ogre::Viewport* vp = mWindow->addViewport(mCamera);
  vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

  // Set camera aspect ratio to match viewport.
  mCamera->setAspectRatio(
    Ogre::Real(vp->getActualWidth() / Ogre::Real(vp->getActualHeight())));

  // Add lighting.
  //mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5,0.5,0.5));

  Ogre::Light* l = mSceneMgr->createLight("MainLight");
  l->setType(Ogre::Light::LT_DIRECTIONAL);
  //l->setPosition(20,80,50);
  l->setDirection(Ogre::Vector3(0,-1,-1));

  // Add meshes.
  Ogre::Entity* ogreHead = mSceneMgr->createEntity("Head", "ogrehead.mesh");
  Ogre::SceneNode* headNode =
    mSceneMgr->getRootSceneNode()->createChildSceneNode();
  headNode->attachObject(ogreHead);

  // Set player control parameters.
  mRotate = 0.13;
  mMove = 250;
  mDirection = Ogre::Vector3::ZERO;

  // Add voxels.
  mVoxelSphere = new VoxelSphere("VoxelSphere");
  mVoxelSphere->generate(40);
  mVoxelSphereNode = 
    mSceneMgr->getRootSceneNode()->createChildSceneNode("VoxelSphereNode");
  mVoxelSphereNode->setPosition(200,200,200);
  mVoxelSphereNode->attachObject(mVoxelSphere);

  mLargeVoxelSphere = new LargeVoxelSphere("LargeVoxelSphere");
  mLargeVoxelSphere->generate(200);
  mLargeVoxelSphereNode = 
    mSceneMgr->getRootSceneNode()->createChildSceneNode("LargeVoxelSphereNode");
  mLargeVoxelSphereNode->setPosition(-2000,-2000,-2000);
  mLargeVoxelSphereNode->scale(5,5,5);
  mLargeVoxelSphereNode->attachObject(mLargeVoxelSphere);

  // mVoxelTerrain = new VoxelTerrain("VoxelTerrain");
  // mVoxelTerrain->generate(200);
  // mVoxelTerrainNode = 
  //   mSceneMgr->getRootSceneNode()->createChildSceneNode("VoxelTerrainNode");
  // //mVoxelTerrainNode->setPosition(-2500,-10000,-2500);
  // //mVoxelTerrainNode->scale(50, 50, 50);  
  // mVoxelTerrainNode->setPosition(0,-3000,-2500);
  // mVoxelTerrainNode->scale(50, 50, 50);
  // mVoxelTerrainNode->attachObject(mVoxelTerrain);

  mVoxelPlanet = new VoxelPlanet("VoxelPlanet");
  mVoxelPlanet->generate(200, Ogre::Vector3(0,0,0));
  mVoxelPlanetNode = 
    mSceneMgr->getRootSceneNode()->createChildSceneNode("VoxelPlanetNode");
  mVoxelPlanetNode->setPosition(250,-1000,-3000);
  mVoxelPlanetNode->scale(50, 50, 50);
  mVoxelPlanetNode->attachObject(mVoxelPlanet);
}

//==============================================================================
// cross-platform boilerplate main()
//==============================================================================
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
  INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
  int main(int argc, char *argv[]) 
#endif
  {
    OgreVox app;

    try {
      app.go();
    } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
      MessageBox( NULL, e.getFullDescription().c_str(), 
                  "An exception has occured!", 
                  MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
      std::cerr << "An exception has occured: " <<
        e.getFullDescription().c_str() << std::endl;
#endif
    }

    return 0;
  }

#ifdef __cplusplus
}
#endif
