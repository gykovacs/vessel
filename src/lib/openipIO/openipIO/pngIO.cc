#include <openipIO/pngIO.h>
#include <openipIO/configIO.h>

namespace openip
{
#ifdef USE_LIBPNG
    int readPNGInfo(const char* filename, ImageInfo& pnginfo)
    {
        FILE* fp= fopen(filename, "rb");

        png_byte header[8];

        int res= fread(header, 1, 8, fp);

        if ( res != 8 ){ printf("could not read file header\n"); return 0; }

        int is_png= !png_sig_cmp(header, 0, 8);

        if (!is_png){ printf("size not png\n"); return 0; }

        png_structp png_ptr= png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);

        if (!png_ptr){ printf("png ptr error\n"); return 0; }

        png_infop info_ptr= png_create_info_struct(png_ptr);

        if (!info_ptr){ printf("info ptr error\n"); png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL); return 0; }

        png_infop end_info= png_create_info_struct(png_ptr);

        if (!end_info){ printf("end info ptr error\n"); png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL); return 0; }

        png_init_io(png_ptr, fp);

        png_set_sig_bytes(png_ptr, 8);

        png_read_info(png_ptr, info_ptr);

        pnginfo.columns= (unsigned int)png_get_image_width(png_ptr, info_ptr);
        pnginfo.rows= (unsigned int)png_get_image_height(png_ptr, info_ptr);

        pnginfo.colorType= png_get_channels(png_ptr, info_ptr);
        switch (pnginfo.colorType)
        {
            case PNG_COLOR_TYPE_GRAY:
                pnginfo.colorType= COLOR_TYPE_GRAYSCALE;
                break;
            case PNG_COLOR_TYPE_PALETTE:
                pnginfo.colorType= COLOR_TYPE_INDEXED;
                break;
            case PNG_COLOR_TYPE_RGB:
                pnginfo.colorType= COLOR_TYPE_RGB;
                break;
            case PNG_COLOR_TYPE_GRAY_ALPHA:
                pnginfo.colorType= COLOR_TYPE_GRAYSCALE_ALPHA;
                break;
            case PNG_COLOR_TYPE_RGB_ALPHA:
                pnginfo.colorType= COLOR_TYPE_RGB_ALPHA;
                break;
        }

        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);

        fclose(fp);

        return 0;
    }
#endif
 }
