#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>

#include <openipIO/openipIO.h>

namespace openip
{
    std::string toLowercase(std::string str)
    {
        for ( unsigned int i = 0; i < str.length(); i++)
            if (str[i] >= 0x41 && str[i] <= 0x5A)
                str[i]+= 0x20;
        return str;
    }

    int readImageInfo(const char* filename, ImageInfo& ii)
    {
        std::string fn(filename);
        fn= toLowercase(fn).substr(fn.length() - 3, 3);

        if ( fn == std::string("png") )
        {
            #ifdef USE_LIBPNG
                return readPNGInfo(filename, ii);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
        }
        if ( fn == std::string("jpg") )
        {
            #ifdef USE_JPEG
                return readJPGInfo(filename, ii);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
        }
        if ( fn == std::string("tif") )
        {
            #ifdef USE_TIFF
                return readTIFInfo(filename, ii);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
        }
        if ( fn == std::string("bmp") )
        {
            #ifdef USE_BMP
                return readBMPInfo(filename, ii);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
        }

        return UNKNOWN_EXTENSION;
    }
}
