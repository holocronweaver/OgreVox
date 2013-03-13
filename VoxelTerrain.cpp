// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// Copyrighted under the MPLv2 and LGPLv3 by Jesse Johnson.

#include "VoxelTerrain.h"

#include <vector>

anl::CMWC4096 rnd;
anl::CMWC4096 rnd_cave2;

anl::CImplicitGradient ground_gradient;

anl::CImplicitFractal lowland_shape_fractal(anl::BILLOW, anl::GRADIENT, anl::QUINTIC);
anl::CImplicitAutoCorrect lowland_autocorrect(0, 1);
anl::CImplicitScaleOffset lowland_scale(0.125, -0.45);
anl::CImplicitScaleDomain lowland_y_scale;
anl::CImplicitTranslateDomain lowland_terrain;

anl::CImplicitFractal highland_shape_fractal(anl::FBM, anl::GRADIENT, anl::QUINTIC);
anl::CImplicitAutoCorrect highland_autocorrect(-1, 1);
anl::CImplicitScaleOffset highland_scale(0.25, 0);
anl::CImplicitScaleDomain highland_y_scale;
anl::CImplicitTranslateDomain highland_terrain;

anl::CImplicitFractal mountain_shape_fractal(anl::RIDGEDMULTI, anl::GRADIENT, anl::QUINTIC);
anl::CImplicitAutoCorrect mountain_autocorrect(-1, 1);
anl::CImplicitScaleOffset mountain_scale(0.45, 0.15);
anl::CImplicitScaleDomain mountain_y_scale;
anl::CImplicitTranslateDomain mountain_terrain;

anl::CImplicitFractal terrain_type_fractal(anl::FBM, anl::GRADIENT, anl::QUINTIC);
anl::CImplicitAutoCorrect terrain_autocorrect(0, 1);
anl::CImplicitScaleDomain terrain_type_y_scale;
anl::CImplicitCache terrain_type_cache;
anl::CImplicitSelect highland_mountain_select;
anl::CImplicitSelect highland_lowland_select;
anl::CImplicitCache highland_lowland_select_cache;
anl::CImplicitSelect ground_select;

anl::CImplicitBias cave_attenuate_bias(0.45);
anl::CImplicitFractal cave_shape1(anl::RIDGEDMULTI, anl::GRADIENT, anl::QUINTIC);
anl::CImplicitFractal cave_shape2(anl::RIDGEDMULTI, anl::GRADIENT, anl::QUINTIC);
anl::CImplicitCombiner cave_shape_attenuate(anl::MULT);
anl::CImplicitFractal cave_perturb_fractal(anl::FBM, anl::GRADIENT, anl::QUINTIC);
anl::CImplicitScaleOffset cave_perturb_scale(0.5, 0);
anl::CImplicitTranslateDomain cave_perturb;
anl::CImplicitSelect cave_select;

anl::CImplicitCombiner ground_cave_multiply(anl::MULT);

// cave test
anl::CImplicitFractal test_cave_shape(anl::RIDGEDMULTI, anl::GRADIENT, anl::QUINTIC);
anl::CImplicitSelect test_cave_select;


VoxelTerrain::VoxelTerrain(const Ogre::String& name):
  Ogre::ManualObject(name)
{
}

VoxelTerrain::~VoxelTerrain()
{
}

void VoxelTerrain::generateNoise()
{
  std::cout << "Begin generating noise." << std::endl;

  rnd.setSeedTime();

  ground_gradient.setGradient(0,0,0,1);

  rnd_cave2.setSeedTime();

  // lowland
  lowland_shape_fractal.setNumOctaves(2);
  lowland_shape_fractal.setFrequency(0.25);
  lowland_shape_fractal.setSeed(rnd.get());

  lowland_autocorrect.setSource(&lowland_shape_fractal);

  lowland_scale.setSource(&lowland_autocorrect);

  lowland_y_scale.setYScale(0.0);
  lowland_y_scale.setSource(&lowland_scale);

  lowland_terrain.setSource(&ground_gradient);
  lowland_terrain.setYAxisSource(&lowland_y_scale);

  // highland
  highland_shape_fractal.setNumOctaves(4);
  highland_shape_fractal.setFrequency(2);
  highland_shape_fractal.setSeed(rnd.get());

  highland_autocorrect.setSource(&highland_shape_fractal);

  highland_scale.setSource(&highland_autocorrect);

  highland_y_scale.setYScale(0.0);
  highland_y_scale.setSource(&highland_scale);

  highland_terrain.setSource(&ground_gradient);
  highland_terrain.setYAxisSource(&highland_y_scale);
  
  // mountain
  mountain_shape_fractal.setNumOctaves(8);
  mountain_shape_fractal.setFrequency(1);
  mountain_shape_fractal.setSeed(rnd.get());

  mountain_autocorrect.setSource(&mountain_shape_fractal);

  mountain_scale.setSource(&mountain_autocorrect);

  
  mountain_y_scale.setYScale(0.25);
  mountain_y_scale.setSource(&mountain_scale);

  mountain_terrain.setSource(&ground_gradient);
  mountain_terrain.setYAxisSource(&mountain_y_scale);
  
  // terrain select
  terrain_type_fractal.setNumOctaves(3);
  terrain_type_fractal.setFrequency(0.125);
  terrain_type_fractal.setSeed(rnd.get());

  terrain_autocorrect.setSource(&terrain_type_fractal);
  
  terrain_type_y_scale.setYScale(0.25);
  terrain_type_y_scale.setSource(&terrain_autocorrect);

  terrain_type_cache.setSource(&terrain_type_y_scale);

  highland_mountain_select.setLowSource(&highland_terrain);
  highland_mountain_select.setHighSource(&mountain_terrain);
  highland_mountain_select.setThreshold(0.55);
  highland_mountain_select.setFalloff(0.2);
  highland_mountain_select.setControlSource(&terrain_type_cache);

  highland_lowland_select.setLowSource(&lowland_terrain);
  highland_lowland_select.setHighSource(&highland_mountain_select);
  highland_lowland_select.setThreshold(0.25);
  highland_lowland_select.setFalloff(0.15);
  highland_lowland_select.setControlSource(&terrain_type_cache);

  highland_lowland_select_cache.setSource(&highland_lowland_select);

  ground_select.setLowSource(0.0);
  ground_select.setHighSource(1.0);
  ground_select.setThreshold(0.5);
  ground_select.setControlSource(&highland_lowland_select_cache);
    
  // caves
  cave_attenuate_bias.setSource(&highland_lowland_select_cache);

  cave_shape1.setNumOctaves(1);
  cave_shape1.setFrequency(4);
  cave_shape1.setSeed(rnd.get());

  cave_shape2.setNumOctaves(1);
  cave_shape2.setFrequency(4);
  cave_shape2.setSeed(rnd_cave2.get());

  cave_shape_attenuate.setSource(0, &cave_shape1);
  cave_shape_attenuate.setSource(1, &cave_attenuate_bias);
  cave_shape_attenuate.setSource(2, &cave_shape2);
  
  cave_perturb_fractal.setNumOctaves(6);
  cave_perturb_fractal.setFrequency(3);
  cave_perturb_fractal.setSeed(rnd.get());

  cave_perturb_scale.setSource(&cave_perturb_fractal);

  cave_perturb.setSource(&cave_shape_attenuate);
  cave_perturb.setXAxisSource(&cave_perturb_scale);

  cave_select.setLowSource(1.0);
  cave_select.setHighSource(0.0);
  cave_select.setThreshold(0.48);
  cave_select.setFalloff(0.0);
  cave_select.setControlSource(&cave_perturb);

  // Combine above and below ground.
  ground_cave_multiply.setSource(0, &cave_select);
  ground_cave_multiply.setSource(1, &ground_select);


  // noise test
  test_cave_shape.setNumOctaves(1);
  test_cave_shape.setFrequency(2);
  
  test_cave_select.setLowSource(1.0);
  test_cave_select.setHighSource(0.0);
  test_cave_select.setThreshold(0.8);
  test_cave_select.setFalloff(0.0);
  test_cave_select.setControlSource(&cave_perturb);

  std::cout << "Done generating noise." << std::endl;
}

void VoxelTerrain::requestVolume(
  const PolyVox::ConstVolumeProxy<BYTE>& volume,
  const PolyVox::Region& region)
{
  float interval = 1;
  //std::cout << "Begin filling VoxelTerrain." << std::endl;
  for (float z = region.getLowerCorner().getZ();
       z <= region.getUpperCorner().getZ(); z+=interval) {
    for (float y = region.getLowerCorner().getY();
         y <= region.getUpperCorner().getY(); y+=interval) {
      for (float x = region.getLowerCorner().getX();
           x <= region.getUpperCorner().getX(); x+=interval) {
        //std::cout << x/region.getUpperCorner().getX() << std::endl;

        BYTE voxel_value = ground_cave_multiply.get(
          5*x/region.getUpperCorner().getX(),
          y/region.getUpperCorner().getY(),
          5*z/region.getUpperCorner().getZ())*255;
        
        // if (static_cast<double>(voxel_value) != 0)
        //   std::cout << static_cast<double>(voxel_value) << std::endl;

        volume.setVoxelAt(x, y, z, voxel_value);
      }
    }
  }
  //std::cout << "Done filling VoxelTerrain." << std::endl;
}

void VoxelTerrain::storeVolume(
  const PolyVox::ConstVolumeProxy<BYTE>& volume,
  const PolyVox::Region& region)
{
  std::cout << "Deleting VoxelTerrain volume." << std::endl;
}

void VoxelTerrain::generate(float radius)
{
  generateNoise();

  mVolumeData = new PolyVox::LargeVolume<BYTE>(
    &VoxelTerrain::requestVolume,
    &VoxelTerrain::storeVolume);
  mVolumeData->setMaxNumberOfBlocksInMemory(2048);
  mVolumeData->setMaxNumberOfUncompressedBlocks(2048);

  mesh();
}

void VoxelTerrain::mesh()
{
  PolyVox::SurfaceMesh<PolyVox::PositionMaterialNormal> mesh;

  PolyVox::Region region(PolyVox::Vector3DInt32(0,0,0),
                         PolyVox::Vector3DInt32(120, 60, 120));
                         //PolyVox::Vector3DInt32(120, 130, 120));

  std::cout << "Begin extracting surface." << std::endl;
  //PolyVox::MarchingCubesSurfaceExtractor<PolyVox::LargeVolume<BYTE> >
    PolyVox::CubicSurfaceExtractorWithNormals<PolyVox::LargeVolume<BYTE> >
    surface_extractor(mVolumeData,
                      region,
                      &mesh);
  surface_extractor.execute();
  std::cout << "Done extracting surface." << std::endl;

  begin("Terrain", Ogre::RenderOperation::OT_TRIANGLE_LIST);
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
      const PolyVox::Vector3DFloat v3dFinalVertexPos =
        v3dVertexPos +
        static_cast<PolyVox::Vector3DFloat>(mesh.m_Region.getLowerCorner());
      position(v3dFinalVertexPos.getX(), v3dFinalVertexPos.getY(), v3dFinalVertexPos.getZ());
      normal(v3dVertexNormal.getX(), v3dVertexNormal.getY(), v3dVertexNormal.getZ());
      //colour(1, 0, 0, 0.5);
      textureCoord(v3dFinalVertexPos.getX()/4.0f, 
                   v3dFinalVertexPos.getY()/4.0f, 
                   v3dFinalVertexPos.getZ()/4.0f);
    }
  }
  end();
}
