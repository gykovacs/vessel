#ifndef FLOATIO_H
#define FLOATIO_H

//#ifdef USE_FLOATIO

#include <openipDS/Image.h>

/**
 * openip namespace
 */
namespace openip
{
    /**
     * reads the image info of a bmp file
     * @param filename filename
     * @param ii image info object
     * @return ImageIOError error code
     */
    int readFLOATInfo(const char* filename, ImageInfo& ii);

    /**
     * reads the three channels of a bmp file into the r, g and b images
     * @param filename bmp image to read
     * @param r red channel
     * @param g green channel
     * @param b blue channel
     * @param topBorder number of top border rows
     * @param bottomBorder number of bottom border rows
     * @param leftBorder number of left border columns
     * @param rightBorder number of right border columns
     * @param borderMode border mode
     * @return ImageIOError error code
     */
    template<typename T>
    int readFLOAT(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);

    /**
     * reads one channel of a bmp file into the img image
     * @param filename bmp image to read
     * @param img image to read into
     * @param channel channel identifier
     * @param topBorder number of top border rows
     * @param bottomBorder number of bottom border rows
     * @param leftBorder number of left border columns
     * @param rightBorder number of right border columns
     * @param borderMode border mode
     * @return ImageIOError error code
     */
    template<typename T>
    int readFLOAT(const char* filename, Image<T>& img, int channel, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);

    /**
     * writes columns*rows elements from the r g and b images into the red, green and blue channels of the file
     * @param filename filename to write the bmp into
     * @param r red channel
     * @param g green channel
     * @param b blue channel
     * @return ImageIOError error code
     */
    template<typename T>
    int writeFLOAT(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b);

    /**
     * writes columns*rows elements from the img image into the file
     * @param filename filename to write the bmp into
     * @param img intensity image to write into file
     * @return ImageIOError error code
     */
    template<typename T>
    int writeFLOAT(const char* filename, Image<T>& img);

    template<typename T>
    int readFLOAT(const char* filename, Image<T>& img, int channel, int topBorder, int bottomBorder, int leftBorder, int rightBorder, int borderMode)
    {
        ifstream file;
        file.open(filename, ios::in | ios::binary);

        unsigned int columns;
        unsigned int rows;

        file.read((char*)&rows, sizeof(unsigned int));
        file.read((char*)&columns, sizeof(unsigned int));

        int n= 0;
        int width= columns + leftBorder + rightBorder;
        n+= leftBorder + topBorder * width;

        //printf("%d %d\n", rows, columns);
        img.resizeImage(rows, columns, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);

        //float x;
	float* data= (float*)malloc(sizeof(float)*rows*columns);
	file.read((char*)data, sizeof(float)*rows*columns);	
	int m= 0;
        switch (channel)
        {
            case READ_CHANNEL_GRAYSCALE:
                for ( unsigned int j= 0; j < rows; ++j )
                {
                    for ( unsigned int i= 0; i < columns; ++i )
                    {
                         //file.read((char*)&x, sizeof(float));
                         img(n)= data[m++];
                         ++n;
                    }
                    n+= rightBorder + leftBorder;
                }
                break;
        }

        file.close();
	free(data);
        return 0;
    }

    template<typename T>
    int readFLOAT(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b, int topBorder, int bottomBorder, int leftBorder, int rightBorder, int borderMode)
    {
        ifstream file;
        file.open(filename, ios::in | ios::binary);

        unsigned int columns;
        unsigned int rows;

        file.read((char*)&rows, sizeof(unsigned int));
        file.read((char*)&columns, sizeof(unsigned int));

        int n= 0;
        int width= columns + leftBorder + rightBorder;
        n+= leftBorder + topBorder * width;

        r.resizeImage(rows, columns, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
        g.resizeImage(rows, columns, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
        b.resizeImage(rows, columns, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);

	float* data= (float*)malloc(sizeof(float)*rows*columns);
	file.read((char*)data, sizeof(float)*rows*columns);
	
	int m= 0;
        {
                for ( unsigned int i= 0; i < rows; ++i )
                {
                    for ( unsigned int j= 0; j < columns; ++j )
                    {
                         //file.read((char*)&x, sizeof(float));
                         r(n)= g(n)= b(n)= data[m++];
                         ++n;
                    }
                    n+= rightBorder + leftBorder;
                }
        }

        file.close();
	free(data);
        return 0;
    }

    template<typename T>
    int writeFLOAT(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b)
    {
        ofstream file;
        file.open(filename, ios::out | ios::binary);

        unsigned int rows= r.rows - r.topBorder - r.bottomBorder;
        unsigned int columns= r.columns - r.leftBorder - r.rightBorder;

        file.write((char*)&rows, sizeof(unsigned int));
        file.write((char*)&columns, sizeof(unsigned int));

        float x;

        vector<float> tmp(rows*columns);

        int n= 0;
        for ( int i= r.leftBorder; i < r.columns - r.rightBorder; ++i )
            for ( int j= r.topBorder; j < r.rows - r.bottomBorder; ++j )
            {
                x= (r(j*r.columns + i) + g(j*r.columns + i) + b(j*r.columns + i));
                tmp[n]= x;
                ++n;
            }

        file.write((char*)(&(tmp[0])), rows*columns*sizeof(float));
        file.close();
        return 0;
    }

    template<typename T>
    int writeFLOAT(const char* filename, Image<T>& r)
    {
        ofstream file;
        file.open(filename, ios::out | ios::binary);

        unsigned int rows= r.rows - r.topBorder - r.bottomBorder;
        unsigned int columns= r.columns - r.leftBorder - r.rightBorder;

        file.write((char*)&rows, sizeof(unsigned int));
        file.write((char*)&columns, sizeof(unsigned int));

        float x;

        vector<float> tmp(rows*columns);

        int n= 0;
        for ( int j= r.topBorder; j < r.rows - r.bottomBorder; ++j )
            for ( int i= r.leftBorder; i < r.columns - r.rightBorder; ++i )
            {
                x= r(j*r.columns + i);
                tmp[n]= x;
                ++n;
            }

        //printf("%d %d %d %d\n", rows, columns, rows*columns, n);
        file.write((char*)(&(tmp[0])), rows*columns*sizeof(float));
        file.close();
        return 0;
    }
}

//#endif USEFLOAT_IO


#endif // FLOATIO_H
