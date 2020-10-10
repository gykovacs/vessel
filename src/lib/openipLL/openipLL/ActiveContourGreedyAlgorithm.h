/* 
 * File:   ActiveContourGreedyAlgorithm.h
 * Author: gykovacs
 *
 * Created on 2011. január 4., 22:28
 */

#ifndef ACTIVECONTOURGREEDYALGORITHM_H
#define	ACTIVECONTOURGREEDYALGORITHM_H

#include <openipDS/Image.h>
#include <openipLL/ActiveContour.h>
#include <openipLL/imageIO.h>
#include <openipDS/StructuringElement2s.h>
#include <openipDS/Filter2s.h>
#include <openipDS/Stopper.h>

namespace openip
{
    template<typename INPUT>
    class ActiveContourGreedyAlgorithm
    {
    public:
        ActiveContourGreedyAlgorithm(StructuringElement22 se);

        ActiveContourGreedyAlgorithm(const ActiveContourGreedyAlgorithm& a);

        ~ActiveContourGreedyAlgorithm();

        void solve(Image<INPUT>& image, ActiveContour& ac, float threshold1, float threshold2);

        float computeAverageDistance(ActiveContour& ac);

        void computeEnergyContinuity(ActiveContour& ac, int j, Vector<float>& result);

        void computeEnergyCurvature(ActiveContour& ac, int j, Vector<float>& result);

        void computeEnergyImage(ActiveContour& ac, int j, Vector<float>& result);

        void computeAccurateCurvature(ActiveContour& ac);

        void relaxContourPoints(ActiveContour& ac, float threshold1, float threshold2);

        Image<float>* gradientMagnitude;

        StructuringElement22 nh;
        float d;
    };

    template<typename INPUT>
    ActiveContourGreedyAlgorithm<INPUT>::ActiveContourGreedyAlgorithm(StructuringElement22 se)
    {
        nh= StructuringElement22(se);
    }

    template<typename INPUT>
    ActiveContourGreedyAlgorithm<INPUT>::ActiveContourGreedyAlgorithm(const ActiveContourGreedyAlgorithm&)
    {
    }

    template<typename INPUT>
    ActiveContourGreedyAlgorithm<INPUT>::~ActiveContourGreedyAlgorithm()
    {
    }

    template<typename INPUT>
    float ActiveContourGreedyAlgorithm<INPUT>::computeAverageDistance(ActiveContour& ac)
    {
        float dist= 0;
        for ( unsigned int i= 0; i < ac.size(); ++i )
            dist+= ac(i).dist(ac(i+1));
        return dist/= ac.size();
    }

    template<typename INPUT>
    void ActiveContourGreedyAlgorithm<INPUT>::computeEnergyContinuity(ActiveContour& ac, int j, Vector<float>& result)
    {
        result.resize(nh.size());
        //Pixel2 ptmp;

        for ( unsigned int i= 0; i < nh.size(); ++i )
        {
            result(i)= d - (ac(j) + nh(i)).dist(ac(j-1));
        }

        float min, max;
        result.getMinMax(min, max);

        for ( int i= 0; i < result.size(); ++i )
            result(i)= (result(i) - min)/(max - min);
    }

    template<typename INPUT>
    void ActiveContourGreedyAlgorithm<INPUT>::computeEnergyCurvature(ActiveContour& ac, int j, Vector<float>& result)
    {
        result.resize(nh.size());
        Pixel2 ptmp;

        for ( unsigned int i= 0; i < nh.size(); ++i )
        {
            ptmp= ac(j - 1);
            ptmp-= (ac(j) + nh(i)) * 2;
            ptmp+= ac(j + 1);
            result(i)= ptmp.norm();
        }

        float min, max;
        result.getMinMax(min, max);

        if ( min == max )
        {
            result= 0;
            return;
        }

        for ( int i= 0; i < result.size(); ++i )
            result(i)= (result(i) - min)/(max - min);
    }

    template<typename INPUT>
    void ActiveContourGreedyAlgorithm<INPUT>::computeEnergyImage(ActiveContour& ac, int j, Vector<float>& result)
    {
        result.resize(nh.size());
        Pixel2 ptmp;

        for ( int i= 0; i < nh.size(); ++i )
        {
            ptmp= nh(i);
            ptmp+= ac(j);
            result(i)= (*gradientMagnitude)(ptmp);
        }
        float min, max;
        result.getMinMax(min, max);
        printf("%f %f\n", min, max);

        if ( (max - min) < 5 )
            min= max - 5;

        for ( int i= 0; i < result.size(); ++i )
        {
            result(i)= (min - result(i))/(max - min);
        }
    }

    template<typename INPUT>
    void ActiveContourGreedyAlgorithm<INPUT>::computeAccurateCurvature(ActiveContour& ac)
    {
        Vector2<float> a;
        Vector2<float> b;
        Vector2<int> at;
        Vector2<int> bt;
        for ( int i= 0; i < ac.size(); ++i )
        {
            at= ac(i);
            bt= ac(i+1);
            at-= ac(i-1);
            bt-= ac(i);
            a= at;
            b= bt;
            ac(i).curv= (a.normalize() - b.normalize()).norm();
        }
    }

    template<typename INPUT>
    void ActiveContourGreedyAlgorithm<INPUT>::relaxContourPoints(ActiveContour& ac, float threshold1, float threshold2)
    {
        for ( int i= 0; i < ac.size(); ++i )
            if ( ac(i).curv > ac(i-1).curv && ac(i).curv > ac(i+1).curv && ac(i).curv > threshold1 && (*gradientMagnitude)(i) > threshold2 )
                ac(i).beta= 0.0f;
    }

    template<typename INPUT>
    void ActiveContourGreedyAlgorithm<INPUT>::solve(Image<INPUT>& image, ActiveContour& ac, float threshold1, float threshold2)
    {
        Image<unsigned char> tmp;
        gradientMagnitude= new Image<float>(image.rows, image.columns);
        SobelFilter<INPUT, float> sf(image.columns);
        sf.updateStride(image.columns);
        sf.computeMinMax();
        tprintf("computing gradient magnitude\n");
        sf.applyMagnitude(image, *gradientMagnitude);
        /*gradientMagnitude->normalize(0, 255);
        writeImage("gradient.png", *gradientMagnitude);*/

        int pointsMoved;
        Vector<float> eContinuity;
        Vector<float> eCurvature;
        Vector<float> eImage;

        tprintf("compute average distance\n");
        
        tprintf("average distance: %f\n", d);

        int minIdx;
        float minEnergy;
        float tmpEnergy;

        tprintf("starting iteration\n");
        int iteration= 0;
        do
        {
            ++iteration;
            if ( iteration % 20 == 0 )
            {
                tmp= image;
                for ( int i= 0; i < ac.size(); ++i )
                    tmp(ac(i))= 255;
                printf("SAVE TMP ________________________________________"); fflush(stdout);
                writeImage("tmp.png", tmp);
            }
            d= computeAverageDistance(ac);
            tprintf("average distance: %f\n", d);
            pointsMoved= 0;

            tprintf("contour: \n");
            for ( int i= 0; i < ac.size(); ++i )
                printf("%d %d\t", ac(i).r(), ac(i).c());
            printf("\n");

            for ( int i= 0; i < ac.size(); ++i )
            {
                //tprintf("computing continuity energy\n");
                computeEnergyContinuity(ac, i, eContinuity);
                /*tprintf("continuity energies: \n");
                for ( int j= 0; j < eContinuity.size(); ++j )
                    printf("%f\t", eContinuity(j));
                printf("\n");*/

                //tprintf("computing curvature energy\n");
                computeEnergyCurvature(ac, i, eCurvature);
                /*tprintf("curvature energies: \n");
                for ( int j= 0; j < eCurvature.size(); ++j )
                    printf("%f\t", eCurvature(j));
                printf("\n");*/

                //tprintf("computing image energy\n");
                computeEnergyImage(ac, i, eImage);
                /*tprintf("image energies: \n");
                for ( int j= 0; j < eImage.size(); ++j )
                    printf("%f\t", eImage(j));
                printf("\n");*/

                minIdx= 0;
                minEnergy= FLT_MAX;
                for ( int j= 0; j < eContinuity.size(); ++j )
                {
                    tmpEnergy= ac(i).alpha * eContinuity(j) + ac(i).beta * eCurvature(j) + ac(i).gamma * eImage(j);
                    if ( tmpEnergy < minEnergy )
                    {
                        minEnergy= tmpEnergy;
                        minIdx= j;
                    }
                }

                tprintf("min energy: %f %d %f %f %f\n", minEnergy, minIdx, eContinuity(minIdx), eCurvature(minIdx), eImage(minIdx));

                if ( nh(minIdx).r() != 0 || nh(minIdx).c() != 0 )
                {
                    if ( nh(minIdx).r() + ac(i).r() > 0 && nh(minIdx).r() + ac(i).r() < image.rows )
                        if ( nh(minIdx).c() + ac(i).c() > 0 && nh(minIdx).c() + ac(i).c() < image.columns )
                        {
                            ac(i)+= nh(minIdx);
                            ++pointsMoved;
                        }
                }
            }
            computeAccurateCurvature(ac);
            relaxContourPoints(ac, threshold1, threshold2);
            tprintf("number of points moved: %d\n", pointsMoved);
        }
        while ( pointsMoved > 0 );
    }
}

#endif	/* ACTIVECONTOURGREEDYALGORITHM_H */

