// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// Copyrighted under the MPLv2 and LGPLv3 by Jesse Johnson.

#include "VoxelSphere.h"

#include <vector>

VoxelSphere::VoxelSphere(const Ogre::String& name): 
  Ogre::ManualObject(name)
{
}

VoxelSphere::~VoxelSphere()
{
}

void VoxelSphere::generate(float radius)
{
  int buffer = 4*radius;
  PolyVox::Region region(PolyVox::Vector3DInt32(0,0,0), 
                         PolyVox::Vector3DInt32(buffer, buffer, buffer));
  mVolumeData = new PolyVox::SimpleVolume<BYTE>(region);

  // Center of the volume.
  PolyVox::Vector3DFloat vol_center(mVolumeData->getWidth() / 2, 
                             mVolumeData->getHeight() / 2, 
                             mVolumeData->getDepth() / 2);

  for (int z = 0; z < mVolumeData->getDepth(); z++) {
    for (int y = 0; y < mVolumeData->getHeight(); y++) {
      for (int x = 0; x < mVolumeData->getWidth(); x++) {
        PolyVox::Vector3DFloat current_pos(x,y,z);
        float dist_to_center = (current_pos - vol_center).length();

        BYTE voxel_value = 0;
        if(dist_to_center <= radius) {
          voxel_value = 255;
        }

        mVolumeData->setVoxelAt(x, y, z, voxel_value);
      }
    }
  }

  mesh();
}

void VoxelSphere::mesh() 
{
  PolyVox::SurfaceMesh<PolyVox::PositionMaterialNormal> mesh;
  PolyVox::MarchingCubesSurfaceExtractor<PolyVox::SimpleVolume<BYTE> >
    surface_extractor(mVolumeData, 
                      mVolumeData->getEnclosingRegion(), 
                      &mesh);
  surface_extractor.execute();

  delete mVolumeData;
  mVolumeData = nullptr;

  //clear();
  begin("BaseWhite", Ogre::RenderOperation::OT_TRIANGLE_LIST);
  {
    const std::vector<PolyVox::PositionMaterialNormal>& vecVertices = mesh.getVertices();
    const std::vector<uint32_t>& vecIndices = mesh.getIndices();
    unsigned int uLodLevel = 0;
    int beginIndex = mesh.m_vecLodRecords[uLodLevel].beginIndex;
    int endIndex = mesh.m_vecLodRecords[uLodLevel].endIndex;

    for(int index = beginIndex; index < endIndex; ++index) {
      const PolyVox::PositionMaterialNormal& vertex = vecVertices[vecIndices[index]];
      const PolyVox::Vector3DFloat& v3dVertexPos = vertex.getPosition();
      const PolyVox::Vector3DFloat& v3dVertexNormal = vertex.getNormal();
      //const Vector3DFloat v3dRegionOffset(uRegionX * g_uRegionSideLength, uRegionY * g_uRegionSideLength, uRegionZ * g_uRegionSideLength);
      const PolyVox::Vector3DFloat v3dFinalVertexPos = v3dVertexPos + static_cast<PolyVox::Vector3DFloat>(mesh.m_Region.getLowerCorner());
      position(v3dFinalVertexPos.getX(), v3dFinalVertexPos.getY(), v3dFinalVertexPos.getZ());
      normal(v3dVertexNormal.getX(), v3dVertexNormal.getY(), v3dVertexNormal.getZ());
      // uint8_t mat = vertex.getMaterial() + 0.5;
      // uint8_t red = mat & 0xF0;
      // uint8_t green = mat & 0x03;
      // uint8_t blue = mat & 0x0C;
      // colour(red*2, green*4, blue*4);// just some random colors, I'm too lazy for hsv
      colour(1, 0, 0, 0.5);
    }
  }
  end();
}
