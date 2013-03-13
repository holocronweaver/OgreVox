// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// Copyrighted under the MPLv2 and LGPLv3 by Jesse Johnson.
#include "LargeVoxelSphere.h"

#include <vector>

#include <stdlib.h>

//PolyVox::Vector3DFloat mVolCenter;

//int mRadius;

LargeVoxelSphere::LargeVoxelSphere(const Ogre::String& name):
  Ogre::ManualObject(name)
{
}

LargeVoxelSphere::~LargeVoxelSphere()
{
}

void LargeVoxelSphere::requestVolume(
  const PolyVox::ConstVolumeProxy<BYTE>& volume,
  const PolyVox::Region& region)
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

        volume.setVoxelAt(x, y, z, voxel_value);
      }
    }
  }
}

void LargeVoxelSphere::storeVolume(
  const PolyVox::ConstVolumeProxy<BYTE>& volume,
  const PolyVox::Region& region)
{
  // Ignore data which can no longer fit into memory.
  //TODO Create save data for modified voxels.
  std::cout << "Deleting volume." << std::endl;
}

void LargeVoxelSphere::generate(float radius)
{
  //float buffer = 4*radius;
  //mVolumeData = new PolyVox::LargeVolume<BYTE>(region);

  mVolumeData = new PolyVox::LargeVolume<BYTE>(
    &LargeVoxelSphere::requestVolume,
    &LargeVoxelSphere::storeVolume);
  // PolyVox::Region region(PolyVox::Vector3DInt32(-40,-40,-40),
  //                        PolyVox::Vector3DInt32(40, 40, 40));
  // mVolumeData->prefetch(region);

  // mVolCenter(mVolumeData.getWidth() / 2,
  //            mVolumeData.getHeight() / 2,
  //            mVolumeData.getDepth() / 2);
  //mVolCenter(buffer/2, buffer/2, buffer/2);
  //PolyVox::Vector3DFloat volCenter(buffer/2, buffer/2, buffer/2);

  //mRadius = radius;

  mesh();
}

void LargeVoxelSphere::mesh()
{
  PolyVox::SurfaceMesh<PolyVox::PositionMaterialNormal> mesh;

  PolyVox::Region testy = mVolumeData->getEnclosingRegion();

  PolyVox::Region region(PolyVox::Vector3DInt32(-120,-120,-120),
                         PolyVox::Vector3DInt32(120, 120, 120));

  PolyVox::MarchingCubesSurfaceExtractor<PolyVox::LargeVolume<BYTE> >
    surface_extractor(mVolumeData,
                      region,
                      &mesh);
  surface_extractor.execute();

  delete mVolumeData;
  mVolumeData = nullptr;

  //clear();
  begin("BaseWhite", Ogre::RenderOperation::OT_TRIANGLE_LIST);
  {
    const std::vector<PolyVox::PositionMaterialNormal>& vecVertices =
      mesh.getVertices();
    const std::vector<uint32_t>& vecIndices = mesh.getIndices();
    unsigned int uLodLevel = 0;
    int beginIndex = mesh.m_vecLodRecords[uLodLevel].beginIndex;
    int endIndex = mesh.m_vecLodRecords[uLodLevel].endIndex;

    for(int index = beginIndex; index < endIndex; ++index) {
      const PolyVox::PositionMaterialNormal& vertex =
        vecVertices[vecIndices[index]];
      const PolyVox::Vector3DFloat& v3dVertexPos = vertex.getPosition();
      const PolyVox::Vector3DFloat& v3dVertexNormal = vertex.getNormal();
      //const Vector3DFloat v3dRegionOffset(uRegionX * g_uRegionSideLength, uRegionY * g_uRegionSideLength, uRegionZ * g_uRegionSideLength);
      const PolyVox::Vector3DFloat v3dFinalVertexPos =
        v3dVertexPos +
        static_cast<PolyVox::Vector3DFloat>(mesh.m_Region.getLowerCorner());
      position(v3dFinalVertexPos.getX(), v3dFinalVertexPos.getY(), v3dFinalVertexPos.getZ());
      normal(v3dVertexNormal.getX(), v3dVertexNormal.getY(), v3dVertexNormal.getZ());
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
