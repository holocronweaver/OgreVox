// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// Copyrighted under the MPLv2 and LGPLv3 by Jesse Johnson.

#include "Raycast.h"

Raycast::Raycast()
{
}

bool Raycast::operator()(
  const PolyVox::LargeVolume<BYTE>::Sampler& sampler)
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
