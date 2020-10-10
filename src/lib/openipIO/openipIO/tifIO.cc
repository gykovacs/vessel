#include <openipIO/configIO.h>
#include <openipIO/tifIO.h>
#include <openipIO/ImageInfo.h>

#ifdef USE_TIFF

namespace openip
{
    int readTIFInfo(const char* filename, ImageInfo& ii)
    {
        TIFF* tif= TIFFOpen(filename, "r");

        TIFFRGBAImage img;
        char emsg[1024];

        if ( TIFFRGBAImageBegin(&img, tif, 0, emsg))
        {
            ii.columns= img.width;
            ii.rows= img.height;
            ii.bitDepth= img.bitspersample;
            ii.colorType= COLOR_TYPE_RGB_ALPHA;

            TIFFRGBAImageEnd(&img);
        }

        TIFFClose(tif);

        return 0;
    }
}

#endif
