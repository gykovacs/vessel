#ifndef BAYESSEGMENTATION_H
#define BAYESSEGMENTATION_H

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

#include <iostream>
#include <fstream>
#include <string>

namespace openip
{
    template<typename INPUT>
    class BayesSegmentation: public Transform2<INPUT, unsigned char>
    {
    public:
        BayesSegmentation();

        BayesSegmentation(char* classifier);

        BayesSegmentation(std::string classifier);

        BayesSegmentation(Classifier<INPUT>* classifier);

        BayesSegmentation(const BayesSegmentation& bs);

        ~BayesSegmentation();

        void apply(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        Image<float> probabilityMapForeground;
        Image<float> probabilityMapBackground;
        Classifier<float>* classifier;
        int chainMode;
        bool gauss;
        bool sorting;
        bool shiftmax;
        bool shiftmin;
        bool thcorr;
        bool fgimage;
        bool bgimage;
        bool normalize;
    };

    template<typename INPUT>
    BayesSegmentation<INPUT>::BayesSegmentation()
        : Transform2<INPUT, unsigned char>()
    {
        chainMode= 0;
        gauss= false;
        sorting= false;
        shiftmax= false;
        shiftmin= false;
        thcorr= false;
        fgimage= false;
        bgimage= false;
        normalize= false;
    }

    template<typename INPUT>
    BayesSegmentation<INPUT>::BayesSegmentation(char* classifier)
        : Transform2<INPUT, unsigned char>()
    {
        ClassifierIO<float> cio;
        this->classifier= cio.openClassifierFromFile(classifier);
        chainMode= 0;
        gauss= false;
        sorting= false;
        shiftmax= false;
        shiftmin= false;
        thcorr= false;
        fgimage= false;
        bgimage= false;
        normalize= false;
    }

    template<typename INPUT>
    BayesSegmentation<INPUT>::BayesSegmentation(std::string classifier)
        : Transform2<INPUT, unsigned char>()
    {
        ClassifierIO<float> cio;
        char filename[1000];
        strcpy(filename, classifier.c_str());
        this->classifier= cio.openClassifierFromFile(filename);
        chainMode= 0;
        gauss= false;
        sorting= false;
        shiftmax= false;
        shiftmin= false;
        thcorr= false;
        fgimage= false;
        bgimage= false;
        normalize= false;
    }

    template<typename INPUT>
    BayesSegmentation<INPUT>::BayesSegmentation(Classifier<INPUT>* classifier)
        : Transform2<INPUT, unsigned char>()
    {
        this->classifier= classifier;
        chainMode= 0;
        gauss= false;
        sorting= false;
        shiftmax= false;
        shiftmin= false;
        thcorr= false;
        fgimage= false;
        bgimage= false;
        normalize= false;
    }

    template<typename INPUT>
    BayesSegmentation<INPUT>::BayesSegmentation(const BayesSegmentation& bs)
        : Transform2<INPUT, unsigned char>(bs)
    {
        this->classifier= bs.classifier;
        this->probabilityMapBackground= bs.probabilityMapBackground;
        this->probabilityMapForeground= bs.probabilityMapForeground;
        this->chainMode= bs.chainMode;
        this->gauss= bs.gauss;
        this->sorting= bs.sorting;
        this->shiftmax= bs.shiftmax;
        this->shiftmin= bs.shiftmin;
        this->thcorr= bs.thcorr;
        this->fgimage= bs.fgimage;
        this->bgimage= bs.bgimage;
        this->normalize= bs.normalize;
    }

    template<typename INPUT>
    BayesSegmentation<INPUT>::~BayesSegmentation()
    {
        delete classifier;
    }

    template<typename INPUT>
    void BayesSegmentation<INPUT>::apply(Image<INPUT> &input, Image<unsigned char> &output, Image<unsigned char> *roi, Image<unsigned char> *support)
    {
        int originalRows= input.rows;
        int originalColumns= input.columns;

        tprintf("creating preprocessing chain...\n");

        Transform2Chain<INPUT, float>& t2c= *(generateTransform2Chain<INPUT, float>(classifier->preprocessingNames));
        for ( unsigned int i= 0; i < t2c.tvoo.size(); ++i )
            tprintf("%s\n", t2c.tvoo(i)->descriptor.c_str());

        tprintf("creating transform set... %d\n", classifier->featureNames.size()); fflush(stdout);
        for ( unsigned int i= 0; i < classifier->featureNames.size(); ++i )
            cout << classifier->featureNames(i) << endl;

        Transform2Set<float, float>& t2s= *(generateTransform2Set<float, float>(classifier->featureNames));
        tprintf("size and dimensions of generated transform set: %d %d\n", t2s.size(), t2s.dimensions());
        for ( unsigned int i= 0; i < t2s.size(); ++i )
            tprintf("%s\n", t2s(i)->getDescriptor().c_str());

        Border2 b1= t2c.getProposedBorder();
        Border2 b2= t2s.getProposedBorder();
        Border2 b= maximizeBorders(b1, b2);

        tprintf("border: %d %d %d %d %d\n", b.topBorder, b.bottomBorder, b.leftBorder, b.rightBorder, b.borderMode);
        b.borderMode= BORDER_MODE_MIRRORED;
        input.resizeBorder(b);

        b.borderMode= BORDER_MODE_ZERO;
        output.resizeBorder(b);
        if ( roi )
            roi->resizeBorder(b);
        if ( support )
            support->resizeBorder(b);

        Image<float> tmp;
        tmp.resizeImage(output);

        tprintf("creating result images...\n"); fflush(stdout);
        ImageVector<float> result;
        int d= t2s.dimensions();
        result.resize(d);

        FeatureVector<float> v(d);

        tprintf("preprocessing... %p %p\n", roi, support);
        t2c.apply(input, tmp, support, support);

        result.resizeImages(input);
        output.resizeImage(input);
        output= 0;

        tprintf("applying transforms...\n");
        tprintf("%d %d\n", tmp.rows, tmp.columns);
        tprintf("%d %d %d\n", result(0).rows, result(0).columns, result.size());
        tprintf("%d %d\n", support->rows, support->columns);
        if ( chainMode == 0 )
            t2s.apply(tmp, result, support, support);

        if ( normalize )
        {
            tprintf("doing normalization...\n");
            for ( unsigned int i= 0; i < result.size(); ++i )
            {
                double mean= result(i).getMean(roi);
                double stdev= result(i).getStandardDeviation(roi);
                for ( unsigned int j= 0; j < result(i).n; ++j )
                    result(i)(j)= (result(i)(j) - mean)/stdev;
            }
        }

        tprintf("resizing probability maps\n");

        probabilityMapForeground.resizeImage(input);
        probabilityMapBackground.resizeImage(input);
        probabilityMapForeground= 0;
        probabilityMapBackground= 0;

        Vector<float> probabilities(2);

        tprintf("classification of pixels...\n"); fflush(stdout);
        for ( unsigned int i= 0; i < input.size(); ++i )
            if ( roi == 0 || (*roi)(i) > 0 )
            {
                for ( int j= 0; j < d; ++j )
                    v(j)= result(j)(i);
                if ( gauss )
                    v.gauss();
                if ( sorting )
                    v.sort();
                if ( shiftmax )
                    v.shiftToMax();
                if ( shiftmin )
                    v.shiftToMin();
                output(i)= classifier->classify(v, &probabilities);
                probabilityMapBackground(i)= probabilities(0);
                probabilityMapForeground(i)= probabilities(1);
            }
            else
                output(i)= 0;

        tprintf("classification finished...\n"); fflush(stdout);

        output.removeBorder();
        input.removeBorder();
        if ( roi )
            roi->removeBorder();
        if ( support )
            support->removeBorder();
        probabilityMapBackground.removeBorder();
        probabilityMapForeground.removeBorder();

        binearestNeighborResize(output, originalRows, originalColumns);
        bicubicResize(probabilityMapBackground, originalRows, originalColumns);
        bicubicResize(probabilityMapForeground, originalRows, originalColumns);

        if ( fgimage )
        {
            string filename= output.filename;
            filename.append("-fg.png");
            probabilityMapForeground.normalize(0, 255);
            tprintf("write foreground probability map %s\n", filename.c_str());
            writeImage(filename.c_str(), probabilityMapForeground);
        }
        if ( bgimage )
        {
            string filename= output.filename;
            filename.append("-bg.png");
            probabilityMapBackground.normalize(0, 255);
            tprintf("write background probability map %s\n", filename.c_str());
            writeImage(filename.c_str(), probabilityMapBackground);
        }
    }
}

#endif // BAYESSEGMENTATION_H
