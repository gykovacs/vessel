#include <iostream>
#include <fstream>

#include <openipIO/configIO.h>
#include <openipIO/pgmIO.h>

#ifdef USE_PGM

namespace openip
{
    int readPGMInfo(const char* filename, ImageInfo& ii)
    {
        std::ifstream file;
        file.open(filename, std::ios::in);

        if ( !file )
            return NO_SUCH_FILE;

        std::string line;
        char c;

        file >> c;
        if ( c != 'P' )
            return NOT_SUPPORTED_INNER_IMAGE_FORMAT;
        file >> c;
        file >> c;

        if ( c == '#' )
            std::getline(file, line);
        else
            file.putback(c);

        unsigned int columns;
        unsigned int rows;
        
        file >> columns;
        file >> rows;

        ii.columns= columns;
        ii.rows= rows;
        ii.colorType= COLOR_TYPE_GRAYSCALE;

        return SUCCESSFUL;
    }
}

#endif