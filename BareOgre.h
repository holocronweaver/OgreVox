// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// Copyrighted under the MPLv2 and LGPLv3 by Jesse Johnson.

#ifndef __BareOgre_h_
#define __BareOgre_h_

#include <OgreRoot.h>
#include <OgreWindowEventUtilities.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

class BareOgre : public Ogre::WindowEventListener, 
  public Ogre::FrameListener, 
  public OIS::KeyListener, 
  public OIS::MouseListener
{
 public:
  BareOgre(void);
  virtual ~BareOgre(void);
  virtual bool go(void);
 protected:
  virtual void createScene();
  virtual void initInput();
  virtual void setupResources();

  virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

  // Ogre::WindowEventListener
  virtual void windowResized(Ogre::RenderWindow* rw);
  virtual void windowClosed(Ogre::RenderWindow* rw);

  // OIS::KeyListener
  virtual bool keyPressed( const OIS::KeyEvent &arg );
  virtual bool keyReleased( const OIS::KeyEvent &arg );
  // OIS::MouseListener
  virtual bool mouseMoved( const OIS::MouseEvent &arg );
  virtual bool mousePressed( const OIS::MouseEvent &arg,
                             OIS::MouseButtonID id );
  virtual bool mouseReleased( const OIS::MouseEvent &arg,
                              OIS::MouseButtonID id );
  // private:
  Ogre::String mPluginsCfg;
  Ogre::String mResourcesCfg;
  Ogre::Root* mRoot;
  Ogre::LogManager* mLog;
  Ogre::RenderWindow* mWindow;
  Ogre::SceneManager* mSceneMgr;
  bool mShutdown;

  Ogre::Camera* mCamera;
  Ogre::SceneNode *mCamNode;
  Ogre::Real mRotate;
  Ogre::Real mMove;
  Ogre::Vector3 mDirection;

  OIS::InputManager* mInputManager;
  OIS::Mouse* mMouse;
  OIS::Keyboard* mKeyboard;  
};

#endif // #ifndef __BareOgre_h_
