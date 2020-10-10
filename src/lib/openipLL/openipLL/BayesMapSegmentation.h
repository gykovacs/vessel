#ifndef BAYESMAPSEGMENTATION_H
#define BAYESMAPSEGMENTATION_H

#include <openipLL/imageIO.h>

#include <stdlib.h>
#include <omp.h>

#include <openipDS/Image.h>
#include <openipDS/Matrix.h>
#include <openipDS/MatrixNM.h>
#include <openipDS/MatrixNN.h>
#include <openipDS/Matrix2.h>
#include <openipDS/Matrix3.h>
#include <openipDS/CoordinateTransform2.h>
#include <openipDS/Transform3.h>
#include <openipDS/Pixel.h>
#include <openipDS/PixelSet1.h>
#include <openipDS/Filter2.h>
#include <openipDS/Filter1s.h>
#include <openipDS/PixelSet.h>
#include <openipDS/FourierVector.h>
#include <openipDS/FilterSet2.h>
#include <openipDS/FilterSystem2.h>
#include <openipDS/FourierMatrix.h>
#include <openipDS/FourierVector.h>
#include <openipDS/mathFunctions.h>
#include <openipDS/Mask2s.h>
#include <openipDS/MaskSet2.h>
#include <openipDS/BinaryMask2.h>
#include <openipDS/BinaryMask2s.h>
#include <openipDS/MaskSystem2.h>
#include <openipDS/BinaryMaskSet2.h>
#include <openipDS/BinaryMaskSystem2.h>
#include <openipDS/drawing.h>
#include <openipDS/OptionTable.h>
#include <openipDS/Image.h>
#include <openipDS/FilterSystem2.h>
#include <openipDS/Histogram.h>
#include <openipDS/drawing.h>
#include <openipDS/Filter2s.h>
#include <openipDS/FilterSet2.h>
#include <openipDS/IntegerFilter.h>
#include <openipDS/GaussianScaleSpace.h>
#include <openipDS/GaborFilter2.h>
#include <openipDS/FeatureVector.h>
#include <openipDS/StatisticalFeature2s.h>
#include <openipDS/Volume.h>

#include <openipSC/fourierTransform.h>
#include <openipSC/FourierTransformer2.h>
#include <openipSC/FourierTransformer1.h>

#include <openipLL/colorSpaces.h>
#include <openipLL/FourierFilter1.h>
#include <openipLL/FourierFilter2.h>
#include <openipLL/thinning.h>
#include <openipLL/FourierFilterSet2.h>
#include <openipLL/localThresholding.h>
#include <openipLL/morphology.h>
#include <openipLL/imageCorrection.h>
#include <openipLL/localThresholding.h>
#include <openipLL/Transform2Set.h>
#include <openipLL/Transform2Chain.h>
#include <openipLL/Transform3Set.h>
#include <openipLL/Transform3Chain.h>
#include <openipLL/Transform3Generator.h>

#include <openipDS/DiscreteFeatureVector.h>
#include <openipDS/ContinuousFeatureVector.h>
#include <openipDS/DiscreteDataSet.h>
#include <openipDS/ContinuousDataSet.h>

#include <openipML/ContinuousNaiveBayesClassifier.h>
#include <openipML/DiscreteNaiveBayesClassifier.h>
#include <openipML/DiscreteMultivariateBayesClassifier.h>
#include <openipML/KNNClassifier.h>
#include <openipML/AODEClassifier.h>
#include <openipML/EpsilonSVMClassifier.h>
#include <openipML/NuSVMClassifier.h>
#include <openipML/KFoldCrossValidator.h>
#include <openipML/LeaveOneOutCrossValidator.h>
#include <openipML/RandomSubSamplingValidator.h>
#include <openipML/DiscreteCorrelationFeatureSelection.h>
#include <openipML/ForwardFeatureSubsetSelection.h>
#include <openipML/BackwardFeatureSubsetSelection.h>
#include <openipML/SimulatedAnnealingFeatureSubsetSelection.h>
#include <openipDS/Feature2Generator.h>
#include <openipDS/VectorFeature2s.h>
#include <openipDS/Stopper.h>

#include <openipML/ClassifierIO.h>
#include <openipLL/ROIDetector.h>
#include <openipLL/HMRFSegmentation.h>
#include <openipLL/Transform2Generator.h>
#include <openipLL/HMRFSegmentation3.h>
#include <openipLL/HMRFSegmentation3HexaSlice.h>
#include <openipLL/RegionTransform2Set.h>
#include <openipLL/RegionTransform2Generator.h>
#include <openipDS/VectorTransform2.h>
#include <openipDS/Transform2.h>

#include <openipLL/BayesSegmentation.h>
#include <openipLL/thresholding.h>

#include <iostream>
#include <fstream>
#include <string>

namespace openip
{
    template<typename INPUT>
    class BayesMapSegmentation: public Transform2<INPUT, unsigned char>
    {
    public:
        BayesMapSegmentation();

        BayesMapSegmentation(char* classifiers);

        BayesMapSegmentation(const BayesMapSegmentation& bs);

        ~BayesMapSegmentation();

        void apply(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        Image<float> probabilityMapForeground;
        Image<float> probabilityMapBackground;
        int chainMode;
        bool gauss;
        bool sorting;
        bool shiftmax;
        bool shiftmin;
        bool pixelwise;
        bool thcorr;
        char* classifiers;
    };

    template<typename INPUT>
    BayesMapSegmentation<INPUT>::BayesMapSegmentation()
        : Transform2<INPUT, unsigned char>()
    {
        chainMode= 0;
        gauss= false;
        sorting= false;
        shiftmax= false;
        shiftmin= false;
        thcorr= false;
    }

    template<typename INPUT>
    BayesMapSegmentation<INPUT>::BayesMapSegmentation(char* classifiers)
        : Transform2<INPUT, unsigned char>()
    {
        chainMode= 0;
        gauss= false;
        sorting= false;
        shiftmax= false;
        shiftmin= false;
        thcorr= false;
        this->classifiers= classifiers;
    }

    template<typename INPUT>
    BayesMapSegmentation<INPUT>::BayesMapSegmentation(const BayesMapSegmentation& bs)
        : Transform2<INPUT, unsigned char>(bs)
    {
        this->probabilityMapBackground= bs.probabilityMapBackground;
        this->probabilityMapForeground= bs.probabilityMapForeground;
        this->chainMode= bs.chainMode;
        this->gauss= bs.gauss;
        this->sorting= bs.sorting;
        this->shiftmax= bs.shiftmax;
        this->shiftmin= bs.shiftmin;
        this->thcorr= bs.thcorr;
    }

    template<typename INPUT>
    BayesMapSegmentation<INPUT>::~BayesMapSegmentation()
    {
    }

    template<typename INPUT>
    void BayesMapSegmentation<INPUT>::apply(Image<INPUT> &input, Image<unsigned char> &output, Image<unsigned char> *roi, Image<unsigned char> *support)
    {
        ifstream file;
        tprintf("openning file: %s\n", classifiers);
        file.open(classifiers);
        string line;

        Vector<BayesSegmentation<INPUT>* > bss;

        tprintf("creating bayes segmentation objects...\n");
        while ( !file.eof() && getline(file, line) )
        {
            cout << line << std::endl;
            bss.push_back(new BayesSegmentation<INPUT>(line));
        }

        Vector<Image<float> > probfg;
        Vector<Image<float> > probbg;
        Vector<Image<unsigned char> > result;
        Vector<float> maps;

        tprintf("resizing images\n");
        probfg.resize(bss.size());
        probbg.resize(bss.size());
        result.resize(bss.size());
        maps.resize(bss.size());

        tprintf("starting the segmentation %d\n", bss.size());
        //char filename[100];
        for ( unsigned int i= 0; i < bss.size(); ++i )
        {
            bss(i)->gauss= gauss;
            bss(i)->shiftmax= shiftmax;
            bss(i)->shiftmin= shiftmin;
            bss(i)->sorting= sorting;
            result(i).resizeImage(input);
            probfg(i).resizeImage(input);
            probbg(i).resizeImage(input);

            tprintf("applying segmentation %d\n", i);
            bss(i)->apply(input, result(i), roi, support);
            probfg(i)= bss(i)->probabilityMapForeground;
            probbg(i)= bss(i)->probabilityMapBackground;

            tprintf("normalizing and writing images\n");
            //probfg(i).normalize(0, 255);
            //probbg(i).normalize(0, 255);
            result(i).normalize(0, 255);

            /*sprintf(filename, "probfg%d.png", i);
            writeImage(filename, probfg(i));
            sprintf(filename, "probbg%d.png", i);
            writeImage(filename, probbg(i));
            sprintf(filename, "result%d.png", i);
            writeImage(filename, result(i));*/

            tprintf("computing map value\n");
            maps(i)= 0;
            for ( unsigned int j= 0; j < result(i).n; ++j )
            {
                if ( result(i)(j) == 0 )
                    maps(i)+= probbg(i)(j);
                else
                    maps(i)+= probfg(i)(j);
            }
        }

        tprintf("map values\n");
        for ( unsigned int i= 0; i < maps.size(); ++i )
            printf("%f ", maps(i));
        printf("\n");

        Image<unsigned char> pixelwise;
        Image<float> bg;
        Image<float> fg;
        pixelwise.resizeImage(input);
        fg.resizeImage(input);
        bg.resizeImage(input);
        fg= 0;
        bg= 0;
        tprintf("union\n");
        for ( unsigned int i= 0; i < input.n; ++i )
        {
            for ( unsigned int j= 0; j < result.size(); ++j )
            {
                if ( probfg(j)(i) > fg(i) )
                    fg(i)= probfg(j)(i);
                if ( probbg(j)(i) > bg(i) )
                    bg(i)= probbg(j)(i);
            }
        }
        pixelwise= 0;
        roi->resizeImage(input);
        tprintf("pixelwise\n");
        for ( unsigned int i= 0; i < input.n; ++i )
            //if ( !roi || (*roi)(i) > 0 )
                if ( fg(i) > bg(i) )
                    pixelwise(i)= 1;
                else
                    pixelwise(i)= 0;

        //fg.normalize(0, 255);
        //bg.normalize(0, 255);
        //writeImage("fg.png", fg);
        //writeImage("bg.png", bg);

        tprintf("determining map\n");
        int maxIdx= 0;
        for ( unsigned int i= 1; i < maps.size(); ++i )
            if ( maps(i) > maps(maxIdx) )
                maxIdx= i;

        tprintf("giving back results %d\n", thcorr);

        if ( thcorr )
        {
            ThresholdAtCorrelation<float, unsigned char> tac(100);
            tac.apply(probfg(maxIdx), output, roi, support);
        }
        else
            output= result(maxIdx);

        probfg(maxIdx).normalize(0, 255);
        probbg(maxIdx).normalize(0, 255);
        writeImage("backgroundProbability.png", probbg(maxIdx));
        writeImage("foregroundProbability.png", probfg(maxIdx));
        fg.normalize(0, 255);
        bg.normalize(0, 255);
        writeImage("backgroundProbability2.png", bg);
        writeImage("foregroundProbability2.png", fg);

        //output= result(maxIdx);

        output= pixelwise;
    }
}


#endif // BAYESMAPSEGMENTATION_H
