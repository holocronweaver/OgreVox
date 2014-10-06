// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// Copyrighted under the MPLv2 and LGPLv3 by Jesse Johnson.

#include "OgreVox.h"

#include <OgreCamera.h>
#include <OgreOptionsFile.h>
#include <OgreEntity.h>
#include <OgreException.h>
#include <OgreMesh.h>
#include <OgreRenderWindow.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreSubMesh.h>
#include <OgreViewport.h>
#include <OgreWindowEventUtilities.h>

OgreVox::OgreVox()
{
    //mChunk = new PolyVox::Vector3DInt32(-1,-1,-1);
    mChunk = nullptr;
}

OgreVox::~OgreVox()
{
}

//------------------------------------------------------------------------------
bool OgreVox::frameRenderingQueued(const Ogre::FrameEvent& evt) {
    bool repeat_loop = BareOgre::frameRenderingQueued(evt);

    // distance the camera must move before the surface
    // is reextracted
    float reextraction_distance = 32.0;

    //std::cout << "\t frameRenderingQueued: " << repeat_loop << std::endl;

    // OpenGL state machine requires all OpenGL calls to
    // occur in main thread.  Thus Ogre::ManualObject must
    // end() here to fill OpenGL buffers.
    //TODO When switching to OpenGL 3+, the main object
    // can be ended by other threads than main.
    // if (mVoxelPlanet->newMesh) {
    if (not mVoxelPlanet->surfaceRenderQueue.empty()) {
        //std::cout << "new mesh!" << std::endl;
        mVoxelPlanet->surfaceRenderMutex.lock();
        Ogre::ManualObject* man_obj = mVoxelPlanet->surfaceRenderQueue.front();
        //Ogre::SceneNode* man_obj_node;
        man_obj->end();
        mVoxelPlanet->surfaceRenderQueue.pop();
        //TODO destroy previous manual object
        if(mSceneMgr->hasSceneNode(man_obj->getName()))
        {
            mSceneMgr->destroyManualObject(man_obj->getName());
            //mManualObjectNode = mSceneMgr->getSceneNode(man->getName());
        }
        // mVoxelPlanet->convertToMesh("VoxelPlanetMesh");
        // Ogre::Entity * voxelPlanetEntity =
        //   mSceneMgr->createEntity("VoxelPlanetEntity","VoxelPlanetMesh");
        // mVoxelPlanetNode->attachObject(voxelPlanetEntity);
        mVoxelPlanetNode->detachAllObjects();
        mVoxelPlanetNode->attachObject(man_obj);
        //mVoxelPlanetNode->setVisible(true);
        mVoxelPlanet->surfaceRenderMutex.unlock();
    }

    // Load PolyVox chunks within player view distance as
    // player moves around.
    Ogre::Vector3 pos = mCamNode->getPosition() - mVoxelPlanetNode->getPosition();

    PolyVox::Vector3DInt32 chunk(
        floor(pos.x / mVoxelPlanet->chunkSize / 10),
        floor(pos.y / mVoxelPlanet->chunkSize / 10),
        floor(pos.z / mVoxelPlanet->chunkSize / 10)
    );

    if (mChunk) {
        if (chunk == *mChunk || abs((chunk - *mChunk).length()) < reextraction_distance) {
            return repeat_loop;
        }
    }

    std::cout << "current pos: " << pos << std::endl;
    std::cout << "current chunk: " << chunk << std::endl;

    // size of the cube whose surface is extracted
    //PolyVox::Vector3DInt32 view_distance(65,65,65);
    PolyVox::Vector3DInt32 view_distance(128,128,128);

    PolyVox::Region region(chunk - view_distance,
                           chunk + view_distance);

    mVoxelPlanet->enqueueSurfaceExtraction(region);

    if (not mChunk) {
        mChunk = new PolyVox::Vector3DInt32();
    }

    *mChunk = chunk;

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
    // vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Set camera aspect ratio to match viewport.
    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth() / Ogre::Real(vp->getActualHeight())));

    // Add lighting.
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    Ogre::Light* l = mSceneMgr->createLight("MainLight");
    l->setType(Ogre::Light::LT_DIRECTIONAL);
    //l->setPosition(20,80,50);
    l->setDirection(Ogre::Vector3(0,-1,-1));

    // // Add meshes.
    // Ogre::Entity* ogreHead = mSceneMgr->createEntity("Head", "ogrehead.mesh");
    // Ogre::SceneNode* headNode =
    //     mSceneMgr->getRootSceneNode()->createChildSceneNode();
    // headNode->attachObject(ogreHead);

    // Set player control parameters.
    mRotationSpeed = 0.13;
    mMaxSpeed = 20000;
    mSlowMaxSpeed = mMaxSpeed / 15;
    mAccelerationRate = 200;
    // mDirection = Ogre::Vector3::ZERO;

    // Add voxels.
    // mVoxelSphere = new VoxelSphere("VoxelSphere");
    // mVoxelSphere->generate(40);
    // mVoxelSphereNode =
    //   mSceneMgr->getRootSceneNode()->createChildSceneNode("VoxelSphereNode");
    // mVoxelSphereNode->setPosition(200,200,200);
    // mVoxelSphereNode->attachObject(mVoxelSphere);

    // mLargeVoxelSphere = new LargeVoxelSphere("LargeVoxelSphere");
    // mLargeVoxelSphere->generate(200);
    // mLargeVoxelSphereNode =
    //   mSceneMgr->getRootSceneNode()->createChildSceneNode("LargeVoxelSphereNode");
    // mLargeVoxelSphereNode->setPosition(-2000,-2000,-2000);
    // mLargeVoxelSphereNode->scale(5,5,5);
    // mLargeVoxelSphereNode->attachObject(mLargeVoxelSphere);

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
        mSceneMgr->getRootSceneNode()->createChildSceneNode("VoxelPlanet");
    // mVoxelPlanetNode->getEntity()->setMaterialName("SimpleLighting/UniformWhite");
    //mVoxelPlanetNode->setPosition(250,-1000,-3000);
    mVoxelPlanetNode->setPosition(250,-1000,-10000);
    mVoxelPlanetNode->setPosition(0,0,0);

    //mVoxelPlanetNode->scale(50, 50, 50);
    //mVoxelPlanetNode->scale(150, 150, 150);
    mVoxelPlanetNode->scale(500, 500, 500);

    // mMarkerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("MarkerNode");
    // Ogre::Entity* marker_entity = mSceneMgr->createEntity("mySphere", Ogre::SceneManager::PT_SPHERE);
    // mMarkerNode->attachObject(marker_entity);
    // mMarkerNode->setScale(0.1,0.1,0.1);
    // mMarkerNode->setPosition(0,0,0);
    // mMarkerNode->setVisible(false);
}


void OgreVox::mouseMoved(const SDL_MouseMotionEvent& event)
{
    BareOgre::mouseMoved(event);

    // const float rate = 0.001;
    // mCamNode->yaw(Ogre::Radian( * event.xrel),
    //               Ogre::Node::TS_WORLD);
    // mCamNode->pitch(Ogre::Degree(-mRotationSpeed * event.yrel),
    //                 Ogre::Node::TS_LOCAL);
    //}

    // Raycast to find the voxel we are looking at.
    // Ogre::Vector3 origin = mCamNode->getPosition() - mVoxelPlanetNode->getPosition();
    // Ogre::Vector3 direction = mCamNode->getOrientation() * Ogre::Vector3(0, 0, -1);
    // std::cout << origin << std::endl;
    // Raycast raycast = mVoxelPlanet->raycast(origin, direction, 10.0);

    // if(raycast.result == PolyVox::RaycastResults::Interupted) {
    //   std::cout << "\t\tGood touch :-)" << std::endl;
    //   Ogre::Vector3 marker_position(
    //     raycast.position.getX(),
    //     raycast.position.getY(),
    //     raycast.position.getZ()
    //   );
    //   std::cout << "camera_position: " << mCamNode->getPosition() << std::endl;
    //   std::cout << "marker_position: " << marker_position << std::endl;
    //   std::cout << "raycast_position: " << raycast.position << std::endl;
    //   mMarkerNode->setPosition(marker_position); //* 10); //mVoxelPlanetNode->getPosition());
    //   //std::cout << "raycast_position_updated: " << marker_position * 90 - mVoxelPlanetNode->getPosition() << std::endl;
    //   std::cout << "raycast_position_updated: " << marker_position * 10 << std::endl;
    //   mMarkerNode->setVisible(true);
    //   mSceneMgr->getSceneNode("MarkerNode")->
    //     setPosition(raycastResult.intersectionVoxel.getX(),
    //                 raycastResult.intersectionVoxel.getY(),
    //                 raycastResult.intersectionVoxel.getZ());
    //   // set visible = true
    //   std::cout << raycastResult.intersectionVoxel << std::endl;
    //   // If user clicks, add (left) or subtract (right) density
    //   // along voxel normal surface.
    // }
    // else {
    //   std::cout << "\t\tBad touch :-(" << std::endl;
    //   mMarkerNode->setVisible(false);
    // }
}


void OgreVox::mousePressed(const SDL_MouseButtonEvent& event)
{
    switch (event.button)
    {
    case SDL_BUTTON_LEFT:
        break;
    case SDL_BUTTON_RIGHT:
        break;
    }
}


void OgreVox::mouseReleased(const SDL_MouseButtonEvent& event)
{
    return;
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

        std::cout << "All done!  Exiting normally." << std::endl;

        return 0;
    }

#ifdef __cplusplus
}
#endif
