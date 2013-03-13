OgreVox
==========

Provides a framework for creating cross-platform 3D voxel projects 
in Ogre 1.9 using the PolyVox and Accidental Noise libraries.  

A simple example project is included to get you started.  Facing the
Ogre head at the center of the screen, volumes are placed in four
quadrants:
* upper-right: VoxelSphere
* lower-right: LargeVoxelSphere
* upper-left: VoxelTerrain
* lower-left: VoxelPlanet

Includes simple FPS controls via holocronweaver/BareOgre.

CMake is used to provide cross-platform compilation.  

Usage Instructions
-------------------
1. Install Ogre 1.9 and PolyVox 0.3.  The Accidental Noise library is
included with OgreVox.
2. Modify CMakeLists.txt and configuration files in dist/bin for your
project and system.
3. Use `cmake .` in the root project folder to produce the appropriate 
compilation or project files for your system.
5. Compile the code using the files produced by CMake.
6. Either run the exectable or the Python script `run.py` in dist/bin
to start the program.
7. ...
8. Profit.

License
------------------
The code is tweaked and enhanced from the Ogre Tutorial Framework available
on the Ogre Wiki as a Public Domain work.  

PolyVox portions are partially derived from the PolyVox Basic example
which is under the zlib license.

The Accidental Noise Library included in this project is under the
zlib license.

This code is dual licensed under the MPLv2 and LGPLv3 licenses (see license folder).

<!--
Need MPLv2 logo.  Make my own?
![MPLv2 logo](https://raw.github.com/holocronweaver/OgreVox/master/license/MPLv2.png)

![LGPLv3 logo](https://raw.github.com/holocronweaver/OgreVox/master/license/LGPLv3.png)
-->

