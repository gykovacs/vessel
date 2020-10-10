#include <openipLL/VisualWordExtractor.h>
#include <openipDS/ParameterFile.h>
#include <openipLL/morphology.h>
#include <openipLL/imageIO.h>

namespace openip
{
    VisualWordExtractor::VisualWordExtractor(int n)
    {
        this->n= n;
    }

    VisualWordExtractor::VisualWordExtractor(const VisualWordExtractor&)
    {
    }

    VisualWordExtractor::~VisualWordExtractor()
    {
    }

    void VisualWordExtractor::extract(char* /*filename*/)
    {
        /*DatabaseDescriptorParameterFile ddf(filename);

        Image<unsigned char> image;
        Image<unsigned char> mask;
        Image<unsigned char> eroded;

        StructuringElementDisk sed(1);

        int img=0;
        while ( ddf.next(string("image")) )
        {
            readImage(ddf.tokenString.c_str(), image);
            printf("image: %s\n", ddf.tokenString.c_str());
            ddf.next(string("mask"));
            printf("mask: %s\n", ddf.tokenString.c_str());
            readImage(ddf.tokenString.c_str(), mask);

            for ( unsigned int i= 0; i < mask.size(); ++i )
                if ( mask(i) > 0 )
                    mask(i)= 255;
                else
                    mask(i)= 0;

            writeImage("mask.png", mask);

            eroded.resizeImage(image);
            eroded= 0;
            sed.updateStride(image.columns);

            int level= 0;
            char filename[30];

            while ( 1 )
            {
                sprintf(filename, "level%d.png", level);
                writeImage(filename, mask);
                printf("level %d\n", level);
                eroded= 0;
                binaryErode(&mask, &eroded, sed);

                int sum= 0;
                for ( unsigned int i= 0; i < mask.size(); ++i )
                    if ( mask(i) != eroded(i) )
                        sum+= 1;
                printf("%d\n", sum);
                if ( sum == 0 )
                    break;
                
                sum= 0;
                for ( unsigned int i= 0; i < eroded.size(); ++i )
                    if ( eroded(i) == 255 )
                        sum+= 1;
                printf("eroded %d\n", sum);

                sum= 0;
                for ( unsigned int i= 0; i < eroded.size(); ++i )
                    if ( mask(i) == 255 )
                        sum+= 1;
                printf("mask %d\n", sum);

                for ( int i= 0; i < n && i < sum; ++i )
                {
                    int x= rand()%image.rows;
                    int y= rand()%image.columns;

                    while ( mask(x,y) == eroded(x,y) )
                    {
                        x= rand()%image.rows;
                        y= rand()%image.columns;
                    }

                    if ( int(vws.size()) <= level )
                        vws.push_back(VisualWordSet<unsigned char, float>());

                    printf("%d %d %d\n", img, x, y);
                    vws(level).push_back(new VisualWordSquare<unsigned char, float>(image, x, y, 17, 17));
                }

                ++level;
                mask= eroded;
            }
            img++;
        }*/
    }
}