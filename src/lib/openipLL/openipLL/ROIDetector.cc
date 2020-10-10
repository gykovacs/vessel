#include <openipLL/ROIDetector.h>
#include <openipDS/Histogram.h>
#include <openipDS/Filter1.h>
#include <openipLL/imageIO.h>
#include <openipLL/morphology.h>
#include <openipDS/StructuringElement2s.h>
#include <openipLL/imageCorrection.h>

void openip::roiDetectionSimple(Image<unsigned char>* input, Image<unsigned char>* output)
{
    Image<unsigned char> tmp(*input);
    tmp= 0;

    float mean= 0;
    int n= 0;

    for ( int i= input->topBorder; i < input->rows - input->bottomBorder; ++i )
        for ( int j= input->leftBorder; j < input->columns - input->rightBorder; ++j )
        {
            mean+= (*input)(i,j);
            ++n;
        }
    mean/= n;
    printf("%f\n", mean);

    for ( int i= input->topBorder; i < input->rows - input->bottomBorder; ++i )
        for ( int j= input->leftBorder; j < input->columns - input->rightBorder; ++j )
            if ( (*input)(i,j) > mean )
                (*output)(i,j)= 255;
            else
                (*output)(i,j)= 0;


    StructuringElementDisk se(5);
    se.updateStride(input->columns);

    binaryDilate(output, &tmp, se);
    binaryErode(&tmp, output, se);

    binaryErode(output, &tmp, se);
    binaryErode(&tmp, output, se);
    binaryDilate(output, &tmp, se);
    binaryDilate(&tmp, output, se);
    binaryErode(output, &tmp, se);
    binaryErode(&tmp, output, se);

}
