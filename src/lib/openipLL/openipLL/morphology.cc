#include <vector>
#include <set>
#include <omp.h>
#include <algorithm>

#include <openipLL/morphology.h>

#include <openipDS/Image.h>
#include <openipDS/PixelSequence1.h>
#include <openipDS/PixelSequence2.h>
#include <openipDS/PixelSet1.h>
#include <openipDS/PixelSet2.h>

using namespace std;

namespace openip
{
PixelSequence1 binaryDilate(PixelSequence1& input, StructuringElement2& se)
{
    PixelSequence1 ps;

    StructuringElement2::iterator sit;

    for ( unsigned int i= 0; i < input.size(); ++i )
    {
        for ( sit= se.begin(); sit != se.end(); ++sit )
        {
                ps.push_back(input[i] + *sit);
        }
    }
    sort(ps.begin(), ps.end());
    ps.erase(unique(ps.begin(), ps.end()), ps.end());

    return ps;
}

PixelSet1 binaryDilate(PixelSet1& input, StructuringElement2& se)
{
    PixelSet1 ps;

    PixelSet1::iterator it= input.begin();
    StructuringElement2::iterator eit;

    while ( it != input.end() )
    {
        eit= se.begin();
        while ( eit != se.end() )
        {
//            ps.insert(*it + *eit);
            ps.push_back(*it + *eit);
            ++eit;
        }
        ++it;
    }
    sort(ps.begin(), ps.end());
    ps.erase(unique(ps.begin(), ps.end()), ps.end());

    return ps;
}

PixelSequence1 binaryErode(PixelSequence1& input, StructuringElement2& se)
{
    PixelSequence1 ps;

    PixelSequence1::iterator it= input.begin();
    StructuringElement2::iterator eit;

    while ( it != input.end() )
    {
        eit= se.begin();
        while( eit != se.end() && find(input.begin(), input.end(), *it + *eit) != input.end() )
        {
                ++eit;
        }
        if ( eit == se.end() )
            ps.push_back(*it);
        ++it;
    }

    return ps;
}

PixelSet1 binaryErode(PixelSet1& input, StructuringElement2& se)
{
    PixelSet1 ps;

    PixelSet1::iterator it= input.begin();
    StructuringElement2::iterator eit;

    while ( it != input.end() )
    {
        eit= se.begin();
        while ( eit != se.end() && input.find(*it + *eit) != input.end() )
        {
            ++eit;
        }
        if ( eit == se.end() )
            ps.insert(*it);
        ++it;
    }

    return ps;
}

/*void binaryBoundingBoxTopHat(Image<unsigned char>* input, Cache* c, float d, Image<unsigned char>* output)
{
    binaryBoundingBoxClose(input, c, d, output);
    *output -= *input;
}

void grayscaleBoundingBoxTopHat(Image<unsigned char>* input, Cache* c, float d, Image<unsigned char>* mask, Image<unsigned char>* output)
{
    grayscaleBoundingBoxClosePS(input, d, mask, output);
    *output -= *input;
}

void binaryBoundingBoxClose(Image<unsigned char>* input, Cache* c, float d, Image<unsigned char>* output)
{
    Image<int>* bi= c->getI();
    vector<Blob> v;

    componentLabeling(*input, v, *bi, true);

    *output= *input;

    set<int> labelsToDelete;

    for ( int i= 0; i < v.size(); ++i )
        if ( v[i].getBoundingSquareSize() < d )
        {
            labelsToDelete.insert(v[i].label);
        }

    for ( int i= 0; i < output->n; ++i )
        if ( labelsToDelete.find((*bi)[i]) != labelsToDelete.end() )
            (*output)[i]= 0;

    c->putI(bi);
}

void grayscaleBoundingBoxClosePS(Image<unsigned char>* input, float d, Image<unsigned char>* mask, Image<unsigned char>* output)
{
    *output= *input;
    PixelSet1D minimas;
    extractLocalMinimas(*input, *mask, minimas);

    RegionGrowingIntensity rgi(input->w);

    PixelSet1D ps, pstmp;
    int k= 0;

    //printf("aaaaaaaaaa"); fflush(stdout);
    for( PixelSet1D::iterator psit= minimas.begin(); psit != minimas.end(); ++psit)
    {
        //printf("%d %d\t", (k++),minimas.size());fflush(stdout);
        ps.clear();
        pstmp.clear();
        ps.insert(*psit);

        for ( int i= (*input)(*psit)+1; i < 256; i+= 2 )
        {
            rgi.grow(*input, ps, pstmp, i, d*d);
            
            pstmp.computeMinMax(input->w);

            if ( pstmp.getBoundingSquareSize() > d )
            {
                ps.set(*output, (unsigned char)(i-1));
                break;
            }

            if ( i == 255 )
                break;
            rgi.grow(*input, pstmp, ps, i+1, d*d);

            ps.computeMinMax(input->w);

            if ( ps.getBoundingSquareSize() > d )
            {
                pstmp.set(*output, (unsigned char)(i));
                break;
            }
        }
    }
}



void grayscaleBoundingBoxClose(Image<unsigned char>* input, Cache* c, float d, Image<unsigned char>* output)
{
    Image<int>* bi= c->getI();
    Image<unsigned char>* tmp= c->getUC();
    vector<Blob> v;
    set<int> labelsToShow;
    *output= 0;
    *tmp= 0;

    for ( int i= 0; i < 256; ++i )
    {
        for ( int j= 0; j < input->n; ++j )
            if ( (*input)[j] == i )
                (*tmp)[j]= 255;
                //writeImageJPG("tmp.jpg", tmp, tmp, tmp);
                printf("c"); fflush(stdout);
                writeImageJPG("toLabel.jpg", tmp, tmp, tmp);
        componentLabeling(*tmp, v, *bi, true);
        printf("d");fflush(stdout);

        for ( int j= 0; j < v.size(); ++j )
        {
            
            if ( v[j].getBoundingSquareSize() > d && v[j].area > 0 )
            {
                if ( v[j].label != 0 )
                    labelsToShow.insert(v[j].label);
            }
        }
        printf("e");fflush(stdout);
        for ( int j= 0; j < output->n; ++j )
            if ( labelsToShow.find((*bi)[j]) != labelsToShow.end() )
                if ( (*output)[j] == 0 )
                    (*output)[j]= i;
            printf("f");fflush(stdout);
        labelsToShow.clear();
        v.clear();
    }
    printf("g");fflush(stdout);
    c->putI(bi);
}*/

void binaryErode(Image<unsigned char>& input, StructuringElement2& se)
{
    Image<unsigned char> tmp(input);
    se.updateStride(input.columns);
    binaryErode(&input, &tmp, se);
    input= tmp;
}

void binaryErode(Image<unsigned char>* input, Image<unsigned char>* output, StructuringElement2& se, Image<unsigned char>* mask)
{
    int start= -se.min;
    int end= input->n - se.max;

    int i;

    if ( mask == NULL )
    {
        #pragma omp parallel for
        for ( i= start; i < end; ++i )
        {
            if ( (*input)(i) != BACKGROUND )
            {
                for ( StructuringElement2::iterator sit= se.begin(); sit != se.end(); ++sit )
                    if ( (*input)(i + *sit) == BACKGROUND )
                    {
                        (*output)(i)= BACKGROUND;
                        goto endLoop;
                    }
                (*output)(i)= FOREGROUND;
                endLoop:
                ;
            }
            else
            {
                (*output)(i)= BACKGROUND;
            }
        }
    }
    else
    {
        #pragma omp parallel for
        for ( i= start; i < end; ++i )
        {
            if ( (*mask)(i) > 0 )
            {
                if ( (*input)(i) == FOREGROUND )
                {
                    for ( StructuringElement2::iterator sit= se.begin(); sit != se.end(); ++sit )
                        if ( (*input)(i + *sit) == BACKGROUND )
                        {
                            (*output)(i)= BACKGROUND;
                            goto endLoop2;
                        }
                    (*output)(i)= FOREGROUND;
                    endLoop2:
                    ;
                }
                else
                {
                    (*output)(i)= BACKGROUND;
                }
            }
        }
    }
}
void binaryErodeAndReconstruct(Image<unsigned char>* input, Image<unsigned char>* output, StructuringElement2& se, Image<unsigned char>* mask)
{
    Image<unsigned char> tmp(input->rows, input->columns);
    binaryErode(input, &tmp, se, mask);
    binaryDilate(&tmp, output, se, mask);
    binaryDilate(output, &tmp, se, mask);
    for ( unsigned int i= 0; i < input->n; ++i )
        (*output)(i)= (*input)(i) < tmp(i) ? (*input)(i) : tmp(i);
}

void binaryDilate(Image<unsigned char>* input, Image<unsigned char>* output, StructuringElement2& se, Image<unsigned char>* mask)
{
    *output= (unsigned char)BACKGROUND;

    int start= - se.min;
    int end= (int)(input->n) - (int)(se.max);

    int i;

    if ( mask == NULL )
    {
        #pragma omp parallel for
        for ( i= start; i < end; ++i )
            if ( (*input)(i) == BACKGROUND )
            {
                for ( StructuringElement2::iterator sit= se.begin(); sit != se.end(); ++sit )
                    if ( (*input)(i + *sit) == FOREGROUND)
                    {
                        (*output)(i)= FOREGROUND;
                        goto endLoop;
                    }
                (*output)(i)= BACKGROUND;
                endLoop:
                ;
            }
            else
                (*output)(i)= FOREGROUND;
    }
    else
    {
        #pragma omp parallel for
        for ( i= start; i < end; ++i )
            if ( (*mask)(i) > 0 )
            {
                if ( (*input)(i) == BACKGROUND )
                {
                    for ( StructuringElement2::iterator sit= se.begin(); sit != se.end(); ++sit )
                        if ( (*input)(i + *sit) == FOREGROUND )
                        {
                            (*output)(i)= FOREGROUND;
                            goto endLoop2;
                        }
                    (*output)(i)= BACKGROUND;
                    endLoop2:
                    ;
                }
                else
                    (*output)(i)= FOREGROUND;
            }
    }
}

/*void grayscaleErode(Image<unsigned char>* input, Image<unsigned char>* output, StructuringElement& se, Image<unsigned char>* mask)
{
    int start= - se.min;
    int end= input->n - se.max;

    int i;

    if ( mask == NULL )
    {
        #pragma omp parallel for
        for ( i= start; i < end; ++i )
        {
            (*output)(i)= se.getMin(*input, i);
        }
    }
    else
    {
        #pragma omp parallel for
        for ( i= start; i < end; ++i )
            if ( (*mask)(i) > 0 )
            {
                unsigned char min= 255;
                for ( StructuringElement::iterator sit= se.begin(); sit != se.end(); ++sit )
                    if ( (*mask)(i + *sit) > 0 && (*input)(i + *sit) < min )
                        min= (*input)(i + *sit);
                (*output)(i)= min;
            }
    }
}*/


/*void grayscaleDilate(Image<unsigned char>* input, Image<unsigned char>* output, StructuringElement& se, Image<unsigned char>* mask)
{
    int start= - se.min;
    int end= input->n - se.max;

    int i;

    if ( mask == NULL )
    {
        #pragma omp parallel for
        for ( i= start; i < end; ++i )
        {
            (*output)(i)= se.getMax(*input, i);
        }
    }
    else
    {
        #pragma omp parallel for
        for ( i= start; i < end; ++i )
            if ( (*mask)(i) > 0 )
            {
                unsigned char max= 0;
                for ( StructuringElement::iterator sit= se.begin(); sit != se.end(); ++sit )
                    if ( (*mask)(i + *sit) > 0 && (*input)(i + *sit) > max )
                        max= (*input)(i + *sit);
                (*output)(i)= max;
            }
    }
}*/

void binaryOpen(Image<unsigned char>* /*input*/, Image<unsigned char>* /*output*/, StructuringElement2& /*se*/, int* /*c*/, Image<unsigned char>* /*mask*/)
{
/*    Image<unsigned char>* tmp;
    c->get(&tmp);
    binaryErode(input, tmp, se, mask);
    binaryDilate(tmp, output, se, mask);
    c->put(tmp);*/
}
void binaryClose(Image<unsigned char>* /*input*/, Image<unsigned char>* /*output*/, StructuringElement2& /*se*/, int* /*c*/, Image<unsigned char>* /*mask*/)
{
    /*Image<unsigned char>* tmp;
    c->get(&tmp);
    binaryDilate(input, tmp, se, mask);
    binaryErode(tmp, output, se, mask);
    c->put(tmp);*/
}

/*void grayscaleOpen(Image<unsigned char>* input, Image<unsigned char>* output, StructuringElement& se, Cache2* c, Image<unsigned char>* mask)

void grayscaleClose(Image<unsigned char>* input, Image<unsigned char>* output, StructuringElement& se, Cache2* c, Image<unsigned char>* mask);*/

}
