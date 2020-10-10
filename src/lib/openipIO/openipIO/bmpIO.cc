#include <openipIO/configIO.h>
#include <openipIO/bmpIO.h>

#ifdef USE_BMP

namespace openip
{
    int readBMPInfo(const char* filename, ImageInfo& ii)
    {
        BMP bmpIn;
        bmpIn.ReadFromFile(filename);

        ii.columns= bmpIn.TellWidth();
        ii.rows= bmpIn.TellHeight();
        ii.bitDepth= 8;
        ii.colorType= COLOR_TYPE_RGB_ALPHA;

        return 0;
    }
}

#endif