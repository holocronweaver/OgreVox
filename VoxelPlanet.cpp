// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
// Copyrighted under the MPLv2 and LGPLv3 by Jesse Johnson.

#include "VoxelPlanet.h"

#include <math.h>
#include <vector>

anl::CMWC4096 rnd2;
anl::CMWC4096 rnd3;

anl::CImplicitFractal p_lowland_fractal(anl::FBM, anl::GRADIENT, anl::QUINTIC);
anl::CImplicitAutoCorrect p_lowland_autocorrect(0.7, 1);
//anl::CImplicitScaleOffset p_lowland_scale(40, 1);
anl::CImplicitScaleOffset p_lowland_scale(150, 1);
anl::CImplicitNormalizeCoords p_lowland_normalize(1);

anl::CImplicitFractal p_highland_fractal(anl::RIDGEDMULTI, anl::GRADIENT, anl::QUINTIC);
anl::CImplicitAutoCorrect p_highland_autocorrect(0.8, 1);
//anl::CImplicitScaleOffset p_highland_scale(45, 1);
anl::CImplicitScaleOffset p_highland_scale(200, 1);
anl::CImplicitNormalizeCoords p_highland_normalize(1);

anl::CImplicitFractal p_mountains_fractal(anl::BILLOW, anl::GRADIENT, anl::QUINTIC);
anl::CImplicitAutoCorrect p_mountains_autocorrect(0.4, 1);
//anl::CImplicitScaleOffset p_mountains_scale(50, 1);
anl::CImplicitScaleOffset p_mountains_scale(220, 1);
anl::CImplicitNormalizeCoords p_mountains_normalize(1);

anl::CImplicitFractal p_terrain_type_fractal(anl::FBM, anl::GRADIENT, anl::QUINTIC);
anl::CImplicitAutoCorrect p_terrain_autocorrect(0, 1);
anl::CImplicitNormalizeCoords p_terrain_normalize(1);
anl::CImplicitCache p_terrain_type_cache;
anl::CImplicitSelect p_highland_mountain_select;
anl::CImplicitSelect p_highland_lowland_select;
anl::CImplicitCache p_surface_terrain;

anl::CImplicitFractal p_cave_shape1(anl::RIDGEDMULTI, anl::GRADIENT, anl::QUINTIC);
anl::CImplicitFractal p_cave_shape2(anl::RIDGEDMULTI, anl::GRADIENT, anl::QUINTIC);
anl::CImplicitBias p_cave_attenuate_bias(0.45);
anl::CImplicitCombiner p_cave_shape_attenuate(anl::MULT);
anl::CImplicitAutoCorrect p_cave_shape_autocorrect(0.8, 1);
anl::CImplicitScaleOffset p_cave_shape_scale(40, 1);
//anl::CImplicitNormalizeCoords p_cave_shape_normalize(1);
anl::CImplicitFractal p_cave_perturb_fractal(anl::FBM, anl::GRADIENT, anl::QUINTIC);
anl::CImplicitAutoCorrect p_cave_perturb_autocorrect(0.8, 1);
anl::CImplicitScaleOffset p_cave_perturb_scale(40, 1);
//anl::CImplicitNormalizeCoords p_cave_perturb_normalize(1);
anl::CImplicitTranslateDomain p_cave_perturb;
anl::CImplicitCache p_cave_terrain;

anl::CImplicitCombiner p_surface_cave_multiply(anl::MULT);

anl::CImplicitSphere p_sphere_gradient;

VoxelPlanet::VoxelPlanet(const Ogre::String& name)//:
  //Ogre::ManualObject(name)
{
  // newMesh = false;
  shutdownThreads = false;
  // setDynamic(false);
}

VoxelPlanet::~VoxelPlanet()
{
  shutdownThreads = true;
  for (auto& thread : threads) {
    thread.join();
  }
}

void VoxelPlanet::generateNoise()
{
  std::cout << "Begin generating noise." << std::endl;

  rnd2.setSeedTime();

  // Lowland
  p_lowland_fractal.setNumOctaves(2);
  p_lowland_fractal.setFrequency(1);
  p_lowland_fractal.setSeed(rnd2.get());

  p_lowland_autocorrect.setSource(&p_lowland_fractal);

  p_lowland_normalize.setSource(&p_lowland_autocorrect);

  p_lowland_scale.setSource(&p_lowland_normalize);

  // Highland
  p_highland_fractal.setNumOctaves(2);
  p_highland_fractal.setFrequency(1);
  p_highland_fractal.setSeed(rnd2.get());

  p_highland_autocorrect.setSource(&p_highland_fractal);

  p_highland_normalize.setSource(&p_highland_autocorrect);

  p_highland_scale.setSource(&p_highland_normalize);

  // Mountains
  p_mountains_fractal.setNumOctaves(4);
  p_mountains_fractal.setFrequency(1.8);
  p_mountains_fractal.setSeed(rnd2.get());

  p_mountains_autocorrect.setSource(&p_mountains_fractal);

  p_mountains_normalize.setSource(&p_mountains_autocorrect);

  p_mountains_scale.setSource(&p_mountains_normalize);


  // Surface Terrain Mix
  p_terrain_type_fractal.setNumOctaves(3);
  p_terrain_type_fractal.setFrequency(0.5);
  p_terrain_type_fractal.setSeed(rnd2.get()); //FIXME use multiple seeds!

  p_terrain_autocorrect.setSource(&p_terrain_type_fractal);

  p_terrain_normalize.setSource(&p_terrain_autocorrect);

  p_terrain_type_cache.setSource(&p_terrain_normalize);

  p_highland_mountain_select.setLowSource(&p_highland_scale);
  p_highland_mountain_select.setHighSource(&p_mountains_scale);
  p_highland_mountain_select.setControlSource(&p_terrain_type_cache);
  p_highland_mountain_select.setThreshold(0.85);
  p_highland_mountain_select.setFalloff(0.15);

  p_highland_lowland_select.setLowSource(&p_lowland_scale);
  p_highland_lowland_select.setHighSource(&p_highland_mountain_select);
  p_highland_lowland_select.setControlSource(&p_terrain_type_cache);
  p_highland_lowland_select.setThreshold(0.70);
  p_highland_lowland_select.setFalloff(0.15);

  p_surface_terrain.setSource(&p_highland_lowland_select);

  // Caves
  rnd3.setSeedTime();

  p_cave_attenuate_bias.setSource(&p_surface_terrain);

  p_cave_shape1.setNumOctaves(1);
  p_cave_shape1.setFrequency(4);
  p_cave_shape1.setSeed(rnd2.get());
  p_cave_shape2.setNumOctaves(1);
  p_cave_shape2.setFrequency(4);
  p_cave_shape2.setSeed(rnd3.get());

  p_cave_shape_attenuate.setSource(0, &p_cave_shape1);
  p_cave_shape_attenuate.setSource(1, &p_cave_attenuate_bias);
  p_cave_shape_attenuate.setSource(2, &p_cave_shape2);

  p_cave_shape_autocorrect.setSource(&p_cave_shape_attenuate);

  p_cave_shape_scale.setSource(&p_cave_shape_autocorrect);

  //p_cave_shape_normalize.setSource(&p_cave_shape_scale);

  p_cave_perturb_fractal.setNumOctaves(6);
  p_cave_perturb_fractal.setFrequency(3);
  p_cave_perturb_fractal.setSeed(rnd2.get());

  p_cave_perturb_autocorrect.setSource(&p_cave_perturb_fractal);

  p_cave_perturb_scale.setSource(&p_cave_perturb_autocorrect);

  //p_cave_perturb_normalize.setSource(&p_cave_perturb_scale);

  p_cave_perturb.setSource(&p_cave_shape_scale);
  p_cave_perturb.setXAxisSource(&p_cave_perturb_scale);

  p_cave_terrain.setSource(&p_cave_perturb);

  // Combine surface and caves.
  p_surface_cave_multiply.setSource(0, &p_cave_terrain);
  p_surface_cave_multiply.setSource(1, &p_surface_terrain);

  // Sphere Gradient
  p_sphere_gradient.setCenter(0,0,0);
  p_sphere_gradient.setRadius(&p_surface_terrain);

  std::cout << "Done generating noise." << std::endl;
}

void VoxelPlanet::requestVolume(
  const PolyVox::ConstVolumeProxy<BYTE>& volume,
  const PolyVox::Region& region)
{
  const float scale = 2;
  //std::cout << "Begin filling VoxelPlanet." << std::endl;
  for (int z = region.getLowerCorner().getZ();
       z <= region.getUpperCorner().getZ(); z++) {
    for (int y = region.getLowerCorner().getY();
         y <= region.getUpperCorner().getY(); y++) {
      for (int x = region.getLowerCorner().getX();
           x <= region.getUpperCorner().getX(); x++) {
        BYTE voxel_value = p_sphere_gradient.get(x,y,z) * 255;

        // if (static_cast<double>(voxel_value) != 0)
        //   std::cout << static_cast<double>(voxel_value) << std::endl;

        volume.setVoxelAt(x, y, z, voxel_value);
      }
    }
  }
  //std::cout << "Done filling VoxelPlanet." << std::endl;
}

void VoxelPlanet::storeVolume(
  const PolyVox::ConstVolumeProxy<BYTE>& volume,
  const PolyVox::Region& region)
{
  std::cout << "Deleting VoxelPlanet volume." << std::endl;
}

// void VoxelPlanet::update(PolyVox::Vector3DInt32 chunk)
// {
//   chunk *= 32;
//   PolyVox::Region region(chunk,
//                          chunk + PolyVox::Vector3DInt32(32,32,32));

//   // update ManualObject or generate new one

//   //Ogre::ManualObject manual;
// }

void VoxelPlanet::generate(float radius, Ogre::Vector3 origin)
{
  //FIXME do something with the radius and origin!
  volumeOrigin = origin;
  radius = radius;

  generateNoise();

  volumeData = new PolyVox::LargeVolume<BYTE>(
    &VoxelPlanet::requestVolume,
    &VoxelPlanet::storeVolume);
  volumeData->setMaxNumberOfBlocksInMemory(8192);
  volumeData->setMaxNumberOfUncompressedBlocks(8192);

  // PolyVox::Region region(PolyVox::Vector3DInt32(-65,-65,-65),
  //                        PolyVox::Vector3DInt32(65,65,65));
  // extractSurface(region);

  threads.push_back(
    std::thread(&VoxelPlanet::extractSurface, this)
  );
}

// producer
void VoxelPlanet::enqueueSurfaceExtraction(PolyVox::Region region) 
{
  surfaceExtractionQueue.push(region);

  // wake consumer
  extractionQueueNotEmpty.notify_one();
}

// consumer
void VoxelPlanet::extractSurface()
{
  while (not shutdownThreads) {
    std::unique_lock<std::mutex> s(surfaceExtractionQueueMutex);
    while (not surfaceExtractionQueue.empty()) {
      //std::lock_guard<std::recursive_mutex> guard(mutex);
      surfaceExtractionMutex.lock();
      PolyVox::Region region = surfaceExtractionQueue.front();
      Ogre::ManualObject * man_obj = new Ogre::ManualObject("VoxelPlanet");
      _extractSurface(region, man_obj);
      surfaceExtractionQueue.pop();
      surfaceRenderQueue.push(man_obj);
      surfaceExtractionMutex.unlock();
    }                                         
    // sleep until woken by producer
    extractionQueueNotEmpty.wait(
      s, 
      [this](){
        return not surfaceExtractionQueue.empty(); 
      }
    );
  }  
}

// Surface extraction occurs on a cubic region whose center follows
// the camera's position.  As the camera moves, so too does
// the cube.  Once the camera has moved a certain distance,
// the surface of the relocated cube is extracted.
// So long as the cube is sufficiently large and the surface extraction
// occurs often enough (i.e., the re-extraction distance is small),
// the player should never see popping or a clipped voxel surface.
void VoxelPlanet::_extractSurface(
  PolyVox::Region region, 
  Ogre::ManualObject* man_obj
)
{
  //std::cout << "\t _extractSurface" << std::endl;

  PolyVox::SurfaceMesh<PolyVox::PositionMaterialNormal> mesh;

  if (DEBUG) { std::cout << "Begin extracting surface." << std::endl; }
  PolyVox::MarchingCubesSurfaceExtractor<PolyVox::LargeVolume<BYTE> >
    //PolyVox::CubicSurfaceExtractorWithNormals<PolyVox::LargeVolume<BYTE> >
    surface_extractor(volumeData,
                      region,
                      &mesh);
  surface_extractor.execute();
  if (DEBUG) { std::cout << "Done extracting surface." << std::endl; }

  man_obj->setDynamic(false);
  man_obj->clear();
  if (DEBUG) { std::cout << "Begin building manual object." << std::endl; }
  man_obj->begin("SimpleTexture", 
                          Ogre::RenderOperation::OT_TRIANGLE_LIST);
  //man_obj->begin("BareWhite", 
  //               Ogre::RenderOperation::OT_TRIANGLE_LIST);
  {
    const std::vector<PolyVox::PositionMaterialNormal>& vecVertices =
      mesh.getVertices();
    const std::vector<uint32_t>& vecIndices = mesh.getIndices();
    unsigned int uLodLevel = 0;
    int beginIndex = mesh.m_vecLodRecords[uLodLevel].beginIndex;
    int endIndex = mesh.m_vecLodRecords[uLodLevel].endIndex;

    for (int index = beginIndex; index < endIndex; ++index) {
      const PolyVox::PositionMaterialNormal& vertex =
        vecVertices[vecIndices[index]];
      const PolyVox::Vector3DFloat& v3dVertexPos = vertex.getPosition();
      const PolyVox::Vector3DFloat& v3dVertexNormal = vertex.getNormal();

      const PolyVox::Vector3DFloat v3dFinalVertexPos = v3dVertexPos +
        static_cast<PolyVox::Vector3DFloat>(mesh.m_Region.getLowerCorner());
      man_obj->position(v3dFinalVertexPos.getX(),
                        v3dFinalVertexPos.getY(),
                        v3dFinalVertexPos.getZ());
      man_obj->normal(v3dVertexNormal.getX(),
                      v3dVertexNormal.getY(),
                      v3dVertexNormal.getZ());
      man_obj->colour(1, 0, 0, 0.5);
      // Repeat mesh every 4 units.
      man_obj->textureCoord(v3dFinalVertexPos.getX()/4.0f,
                            v3dFinalVertexPos.getY()/4.0f,
                            v3dFinalVertexPos.getZ()/4.0f);
    }
  }
  if (DEBUG) { std::cout << "Done building manual object." << std::endl; }
  //end();
  //newMesh = true;
}

// Ogre::ManualObject* VoxelPlanet::fillManualObject()
// {
// }

float VoxelPlanet::getRadius()
{
  return radius;
}

Ogre::Vector3 VoxelPlanet::getOrigin()
{
  return volumeOrigin;
}

// void VoxelPlanet::swapManualObjects() {
//   this = tempManualObject;
//} 
