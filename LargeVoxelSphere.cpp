// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// Copyrighted under the MPLv2 and LGPLv3 by Jesse Johnson.
#include "LargeVoxelSphere.h"

#include <cstdlib>
#include <vector>

#include "PolyVox/Mesh.h"

LargeVoxelSphere::LargeVoxelSphere(const Ogre::String& name):
  Ogre::ManualObject(name)
{
}


LargeVoxelSphere::~LargeVoxelSphere()
{
}


void LargeVoxelSphere::VoxelSpherePager::pageIn(
    const PolyVox::Region& region,
    PolyVox::PagedVolume<BYTE>::Chunk* chunk)
{
 for (int z = region.getLowerCorner().getZ();
      z <= region.getUpperCorner().getZ(); z++) {
   for (int y = region.getLowerCorner().getY();
        y <= region.getUpperCorner().getY(); y++) {
     for (int x = region.getLowerCorner().getX();
          x <= region.getUpperCorner().getX(); x++) {
        PolyVox::Vector3DFloat current_pos(x,y,z);
        PolyVox::Vector3DFloat volume_center(0,0,0);
        float dist_to_center = (current_pos - volume_center).length();

        BYTE voxel_value = 0;
        if(dist_to_center <= 100) {
          voxel_value = 255;
        }

        chunk->setVoxel(x, y, z, voxel_value);
      }
    }
  }
}


void LargeVoxelSphere::VoxelSpherePager::pageOut(
    const PolyVox::Region& region,
    PolyVox::PagedVolume<BYTE>::Chunk* chunk)
{
  // Ignore data which can no longer fit into memory.
  //TODO Create save data for modified voxels.
  std::cout << "Deleting volume." << std::endl;
}

void LargeVoxelSphere::generate(float radius)
{
  mVolumeData = new PolyVox::LargeVolume<BYTE>(new VoxelSpherePager);

  //mRadius = radius;

  mesh();
}

void LargeVoxelSphere::mesh()
{
    PolyVox::Region region(PolyVox::Vector3DInt32(-120,-120,-120),
                           PolyVox::Vector3DInt32(120, 120, 120));

    auto encodedMesh = extractCubicMesh(mVolumeData, region);
    auto mesh = decodeMesh(encodedMesh);
    auto vertexOffset = static_cast<PolyVox::Vector3DFloat>(mesh.getOffset());
  
    delete mVolumeData;
    mVolumeData = nullptr;

    // clear();
    begin("BaseWhite", Ogre::RenderOperation::OT_TRIANGLE_LIST);
    {
        auto& vertices = mesh.getVertices();
        auto& indices = mesh.getIndices();

        for (auto index : indices) {
            auto& vertex = vertices[index];
            auto vertexPos = vertex.position + vertexOffset;
        
            position(vertexPos.getX(), vertexPos.getY(), vertexPos.getZ());
      
            // uint8_t mat = vertex.getMaterial() + 0.5;
            // uint8_t red = mat & 0xF0;
            // uint8_t green = mat & 0x03;
            // uint8_t blue = mat & 0x0C;
            // colour(red*2, green*4, blue*4);
            colour(1, 0, 0, 0.5);
        }
    }
    end();
}
