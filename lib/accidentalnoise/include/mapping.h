#ifndef MAPPING_H
#define MAPPING_H
#include "templates/tarrays.h"
#include "implicitmodulebase.h"
#include "rgbamodulebase.h"

#include "vectortypes.h"

namespace anl
{
    typedef TArray2D<double> CArray2Dd;
    typedef TArray3D<double> CArray3Dd;
    typedef TArray2D<SRGBA> CArray2Drgba;
    typedef TArray3D<SRGBA> CArray3Drgba;
    enum EMappingModes
    {
        SEAMLESS_NONE,
        SEAMLESS_X,
        SEAMLESS_Y,
        SEAMLESS_Z,
        SEAMLESS_XY,
        SEAMLESS_XZ,
        SEAMLESS_YZ,
        SEAMLESS_XYZ
    };

    struct SMappingRanges
    {
        double mapx0,mapy0,mapz0, mapx1,mapy1,mapz1;
        double loopx0,loopy0,loopz0, loopx1,loopy1,loopz1;

        SMappingRanges()
        {
            mapx0=mapy0=mapz0=loopx0=loopy0=loopz0=-1;
            mapx1=mapy1=mapz1=loopx1=loopy1=loopz1=1;
        };
    };

    void map2D(int seamlessmode, CArray2Dd &a, CImplicitModuleBase &m, SMappingRanges &ranges, double z);
    void map2DNoZ(int seamlessmode, CArray2Dd &a, CImplicitModuleBase &m, SMappingRanges &ranges);
    void map3D(int seamlessmode, CArray3Dd &a, CImplicitModuleBase &m, SMappingRanges &ranges);

    void mapRGBA2D(int seamlessmode, CArray2Drgba &a, CRGBAModuleBase &m, SMappingRanges &ranges, double z);
    void mapRGBA2DNoZ(int seamlessmode, CArray2Drgba &a, CRGBAModuleBase &m, SMappingRanges &ranges);
    void mapRGBA3D(int seamlessmode, CArray3Drgba &a, CRGBAModuleBase &m, SMappingRanges &ranges);

    void saveDoubleArray(char *filename, TArray2D<double> *array);
    void saveRGBAArray(char *filename, TArray2D<anl::SRGBA> *array);
};

#endif
