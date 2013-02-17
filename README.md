OgreVox
==========

Provides a bare framework for creating cross-platform Ogre 1.9 projects.  
A simple example project is included to get you started.

Includes simple FPS controls via holocronweaver/BareOgre.

CMake is used to provide cross-platform compilation.  

Usage Instructions
-------------------
1. Install Ogre 1.9 and PolyVox 0.3.
2. Modify CMakeLists.txt and configuration files in dist/bin for your
project and system.
3. Navigate into the folder storing your project and BareOgre.
4. Use `cmake .` to produce the appropriate compilation or project files for
your system.
5. Compile the code using the files produced by CMake.
6. Either run the exectable or the Python script `run.py` in dist/bin
to start the program.
7. ...
8. Profit.

License
------------------
The code is tweaked and enhanced from the Ogre Tutorial Framework available
on the Ogre Wiki as a Public Domain work.  PolyVox portions are partially
derived from the PolyVox Basic example which is under the zlib license.

This code is released under the LGPLv3 license (see license/LICENSE.txt).

![LGPLv3 logo](https://raw.github.com/holocronweaver/OgreVox/master/license/LGPLv3.png)
