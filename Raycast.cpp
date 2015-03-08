// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// Copyrighted under the MPLv2 and LGPLv3 by Jesse Johnson.

#include "Raycast.h"

Raycast::Raycast()
{
}

bool Raycast::operator()(
  const PolyVox::PagedVolume<BYTE>::Sampler& sampler)
{
  // If the ray is inside the volume then check if it has hit a solid voxel.
  //if(!sampler.isCurrentPositionValid())
  //{
  if(sampler.getVoxel() > 0) { // You can replace this with your own condition if you want to.
    // setVoxel
    // getVoxel
    position = sampler.getPosition();
    // We've hit something, so stop traversing.
    return false;
  }
  //}

  // By default we continue traversing.
  return true;
}

/** Raycast from a given position and direction for a specific
    distance.
**/
Raycast Raycast::raycast(
    Ogre::Vector3 origin, Ogre::Vector3 direction, float distance,
    PolyVox::PagedVolume<BYTE>* volume)
{
    Raycast raycast;
    //origin = origin / 90;
    PolyVox::Vector3DFloat orig(origin.x,origin.y,origin.z);
    PolyVox::Vector3DFloat dir(direction.x,direction.y,direction.z);
    dir.normalise();
    dir *= distance;

    PolyVox::RaycastResult raycast_result = PolyVox::raycastWithDirection(
        volume, orig, dir, raycast
    );
    raycast.result = raycast_result;

    return raycast;
}
