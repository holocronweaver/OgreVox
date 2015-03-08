// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// Copyrighted under the MPLv2 and LGPLv3 by Jesse Johnson.

#ifndef __Raycast_h_
#define __Raycast_h_

#include "OgreRoot.h"

#include "PolyVox/PagedVolume.h"
#include "PolyVox/Raycast.h"

typedef uint8_t BYTE;

struct Raycast
{
public:
    //bool foundIntersection;
    PolyVox::Vector3DInt32 position;
    PolyVox::RaycastResult result;

    Raycast();

    bool operator()(const PolyVox::PagedVolume<BYTE>::Sampler& sampler);

    //TODO: Will probably need to be moved back into VoxelPlanet once
    // octree is complete.
    Raycast raycast(
        Ogre::Vector3 origin, Ogre::Vector3 direction, float distance,
        PolyVox::LargeVolume<BYTE>* volume);
};

#endif // #ifndef __Raycast_h_
