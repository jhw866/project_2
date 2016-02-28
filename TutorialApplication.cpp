/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#include "TutorialApplication.h"
#include <iostream>
#include <OgreLogManager.h>
// #include "userInterface.h"

//---------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void): score(0)
{
}
//---------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
    if (b)
    {
        delete b;
    }

    if (room)
    {
        delete room;
    }
    // mTrayMgr->destroyWidget("Pause");
    // mTrayMgr->destroyWidget("Score");
}

bool TutorialApplication::soundInit(void)
{
    if( SDL_Init( SDL_INIT_AUDIO ) < 0 ) {
        /* Failed, exit. */
        return false;
    }
      //Initialize SDL_mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        return false;
    }
    return true;

}

// mouse event
bool TutorialApplication::mousePressed(
  const OIS::MouseEvent& me, OIS::MouseButtonID id) 
{
    if (id == OIS::MB_Left)
    {
        b->setKinematic(false);
    }

  return true; 
}

// keyboard event
bool TutorialApplication::keyPressed(const OIS::KeyEvent& ke) 
{
    Ogre::SceneNode *node = b->getSceneNode();
    if (ke.key == OIS::KC_W)
    {
        Ogre::Vector3 mvVector = 
        room->checkBoundary(node, Ogre::Vector3(0, 10, 0), 5);
        b->moveAround(mvVector);
    }
    if (ke.key == OIS::KC_A)
    {
        Ogre::Vector3 mvVector = 
        room->checkBoundary(node, Ogre::Vector3(-10, 0, 0), 4);
        b->moveAround(mvVector);
    }
    if (ke.key == OIS::KC_S)
    {
        Ogre::Vector3 mvVector = 
        room->checkBoundary(node, Ogre::Vector3(0, -10, 0), 0);
        b->moveAround(mvVector);
    }
    if (ke.key == OIS::KC_D)
    {
        Ogre::Vector3 mvVector = 
        room->checkBoundary(node, Ogre::Vector3(10, 0, 0), 3);
        b->moveAround(mvVector);
    }
    if (ke.key == OIS::KC_ESCAPE)
    {
        mShutDown = true;
    }
  return true; 
}
 
bool TutorialApplication::keyReleased(const OIS::KeyEvent& ke) 
{ 
  return true; 
}

void TutorialApplication::createFrameListener(void)
{
    BaseApplication::createFrameListener();
    mTrayMgr->showCursor();
    std::string Score("score: " + std::to_string(score));
    mTrayMgr->createLabel(OgreBites::TL_BOTTOMLEFT, "Score: ", Ogre::String(Score), 150);
    mTrayMgr->createButton(OgreBites::TL_BOTTOMLEFT, "Pause", "Pause", 150);
}
 
//---------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{
    // Create your scene here :)
    if (!soundInit()) {
        Ogre::LogManager::getSingleton().logMessage ("init failed");
    }
    mCameraMan->getCamera()->setPosition(0, 300, 500);
    mCameraMan->getCamera()->lookAt(0, 0, 0);
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
    mSceneMgr->setSkyBox(true, "Examples/EveningSkyBox");
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    Ogre::Light *diffuseLight = mSceneMgr->createLight("diffuse light");
    // make this light a point light
    diffuseLight->setType(Ogre::Light::LT_POINT);            
    diffuseLight->setDiffuseColour(20.0, 20.0, 20.0);
    diffuseLight->setPosition(0, 1000, 0);

    sim = new Simulator();
    btScalar mass = 10.0;
    btScalar resist = 1.1;
    btScalar friction = 0.50;
    Ogre::Vector3 initialPoint (0, 100, 0);

    
    b = new ball("sphere.mesh", mSceneMgr, sim, mass, 
                resist, friction, initialPoint, "OceanHLSL_GLSL");

    room = new Room(mSceneMgr, sim);
}


bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    sim->stepSimulation(evt.timeSinceLastEvent, 1);
    b->update(evt.timeSinceLastEvent);
    bool ret = BaseApplication::frameRenderingQueued(evt);
    return ret;
}
//---------------------------------------------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        TutorialApplication app;

        try {
            app.go();
        } catch(Ogre::Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occurred: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
