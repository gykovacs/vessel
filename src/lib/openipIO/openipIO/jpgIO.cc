#include <openipIO/ImageInfo.h>
#include <openipIO/jpgIO.h>

#ifdef USE_JPEG

namespace openip
{
    int readJPGInfo(const char* filename, ImageInfo& ii)
    {
        std::FILE* f= std::fopen(filename, "rb");

        struct jpeg_decompress_struct cinfo;

        struct jpeg_error_mgr jerr;

        cinfo.err = jpeg_std_error(&jerr);

        jpeg_create_decompress(&cinfo);

        jpeg_stdio_src(&cinfo, f);

        jpeg_read_header(&cinfo, TRUE);
        jpeg_start_decompress(&cinfo);

        ii.rows= cinfo.image_height;
        ii.columns= cinfo.image_width;
        ii.colorType= cinfo.num_components;

        switch (ii.colorType)
        {
            case JCS_GRAYSCALE:
                ii.colorType= COLOR_TYPE_GRAYSCALE;
                break;
            case JCS_RGB:
                ii.colorType= COLOR_TYPE_RGB;
                break;
            case JCS_YCbCr:
                ii.colorType= COLOR_TYPE_YCbCr;
                break;
            case JCS_CMYK:
                ii.colorType= COLOR_TYPE_CMYK;
                break;
            case JCS_YCCK:
                ii.colorType= COLOR_TYPE_YCCK;
                break;
        }

        jpeg_destroy_decompress(&cinfo);

        fclose(f);

        return SUCCESSFUL;
    }
}

#endif
