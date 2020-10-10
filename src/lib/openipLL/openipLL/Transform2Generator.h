/**
 * @file Transform2Generator.h
 * @author Gyorgy Kovacs <gyuriofkovacs@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * Transform2Generator can be used to generate Transform2 objects from descriptor strings.
 */

#ifndef TRANSFORM2GENERATOR_H
#define	TRANSFORM2GENERATOR_H

#include <openipDS/Image.h>
#include <openipDS/Filter2s.h>
#include <openipDS/StatisticalFeature2s.h>
#include <openipDS/RatioFeature2s.h>
#include <openipDS/DifferenceFeature2s.h>
#include <openipDS/Transform2.h>
#include <openipDS/VisualWordSet.h>
#include <openipDS/DoubleRingFeature2s.h>
#include <openipDS/Transform2s.h>
#include <openipDS/MatchedFilter2s.h>
#include <openipDS/RunningStatisticalFeatures.h>
#include <openipDS/VectorFeature2s.h>
#include <openipDS/HistogramVectorFeature2.h>

#include <openipLL/FourierDifferenceFilter2.h>
#include <openipLL/Transform2Chain.h>
#include <openipLL/imageCorrection.h>
#include <openipLL/LowLevelTransforms.h>
#include <openipLL/Scaling.h>

#include <openipLL/FourierFeature2.h>
#include <openipLL/AnisotropicDiffusionPeronaMalik.h>
#include <openipDS/VectorTransform2s.h>

#include <openipLL/ColorTransform2s.h>
#include <openipDS/SimmetryFeature2s.h>
#include <openipDS/GaussGaborFilter2.h>
#include <openipDS/ShiftedFeatureFilter2.h>
#include <openipDS/ShiftedFeatureFilter2s.h>
#include <openipDS/OrientationFilterFeature2s.h>
#include <openipDS/GradientVectorFlow.h>

#include <openipDS/MatchedGaborFilter2.h>
#include <openipDS/PowerGaborFilter2.h>
#include <openipDS/OrientationMultiscaleFilterFeature2s.h>
#include <openipDS/LBPFeature2s.h>
#include <openipLL/GaborRGLineSegmentTransform2.h>

namespace openip
{
        /**
         * tokenizes the descriptor string
         * @param input descriptor string
         * @param name output parameter, name of the transform
         * @param parameters output parameter, parameters of the transform
         * @return non-zero in case of errors, zero otherwise
         */
        int getTransformNameAndParameters(std::string& input, std::string& name, vector<float>& parameters);

        /**
         * generates Transform2 object from descriptor string
         * @param s transform descriptor string
         * @return generated Transform2 object
         */
        template<typename INPUT, typename OUTPUT>
        VectorTransform2<INPUT, OUTPUT>* generateTransform2(std::string s)
        {
            std::string name;
            vector<float> parameters;

            int c= s.find(std::string("chain"));
            int begin, end;

            vector<std::string> descriptors;

            if ( c == -1 )
                descriptors.push_back(s);

	    //tprintf("generate transform2s\n");
            while ( 1 )
            {
                if ( c == -1 )
                    break;
                for ( begin= c + 5; s[begin] == ' ' || s[begin] == '\t' || s[begin] == '\n'; ++begin );
                end= s.find(std::string("chain"), begin);
                c= end;
                if ( end == -1 )
                    end= s.size();
                descriptors.push_back(s.substr(begin, end - begin));
            }

            VectorTransform2<INPUT, OUTPUT>* result= NULL;
            Transform2Chain<INPUT, OUTPUT>* t2chain= NULL;

            if ( descriptors.size() > 1 )
                t2chain= new Transform2Chain<INPUT, OUTPUT>();

	    //tprintf("interpretation of descriptors\n");
            for ( unsigned int i= 0; i < descriptors.size(); ++i )
            {
            if ( ! getTransformNameAndParameters(descriptors[i], name, parameters) )
            {
                //tprintf("generating: %s\n", descriptors[i].c_str()); fflush(stdout);

                if ( descriptors.size() > 1 )
                {
                    t2chain->tvoo.push_back(generateTransform2<OUTPUT, OUTPUT>(descriptors[i]));
                    continue;
                }

                if ( name.substr(name.size()-3, name.size()).compare(string(".vw")) == 0 )
                {
                    std::string tmp;
                    std::ifstream file;
                    file.open(name.c_str());
                    getline(file, tmp);
                    result= new CorrelationFilter2<INPUT, OUTPUT, float>(VisualWordDisk<INPUT, OUTPUT>(tmp));
                    result->descriptor= name;
                }

                if ( name.substr(name.size()-4, name.size()).compare(string(".vws")) == 0 )
                {
                    std::string tmp;
                    std::ifstream file;
                    std::stringstream ss;
                    file.open(name.c_str());
                    while ( !file.eof() )
                    {
                        getline(file, tmp);
                        ss << tmp;
                        ss << endl;
                    }
                    VisualWordSet<INPUT, OUTPUT>* vws= new VisualWordSet<INPUT, OUTPUT>(ss.str());
                    vws->descriptor= name;
                    result= vws;
                }

                if ( name.compare("IdenticalVectorFeature2") == 0 )
                    result= new IdenticalVectorFeature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("LocalBinaryPatternVectorFeature2") == 0 )
                    result= new LocalBinaryPatternVectorFeature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("MatchedGaborFilterR2") == 0 )
                    result= new MatchedGaborFilterR2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("MatchedCorrelationGaborFilterR2") == 0 )
                    result= new MatchedCorrelationGaborFilterR2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("MatchedCCorrelationGaborFilterR2") == 0 )
                    result= new MatchedCCorrelationGaborFilterR2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("MatchedPowerGaborFilterR2") == 0 )
                    result= new MatchedPowerGaborFilterR2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);
                if ( name.compare("MatchedCorrelationPowerGaborFilterR2") == 0 )
                    result= new MatchedCorrelationPowerGaborFilterR2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);
                if ( name.compare("MatchedCCorrelationPowerGaborFilterR2") == 0 )
                    result= new MatchedCCorrelationPowerGaborFilterR2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);
                if ( name.compare("MatchedPowerGaborFilterSimpleR2") == 0 )
                    result= new MatchedPowerGaborFilterSimpleR2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);
                if ( name.compare("MatchedCorrelationPowerGaborFilterSimpleR2") == 0 )
                    result= new MatchedCorrelationPowerGaborFilterSimpleR2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);
                if ( name.compare("MatchedCCorrelationPowerGaborFilterSimpleR2") == 0 )
                    result= new MatchedCCorrelationPowerGaborFilterSimpleR2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);
                if ( name.compare("MatchedCCorrelationJunctionGaborFilterR2") == 0 )
                    result= new MatchedCCorrelationJunctionGaborFilterR2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("MatchedMultiScaleGaborFilterR2") == 0 )
                    result= new MatchedMultiScaleGaborFilterR2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("MatchedMultiScaleCorrelationGaborFilterR2") == 0 )
                    result= new MatchedMultiScaleCorrelationGaborFilterR2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("MatchedMultiScaleCCorrelationGaborFilterR2") == 0 )
                    result= new MatchedMultiScaleCCorrelationGaborFilterR2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                /*if ( name.compare("MatchedGaussGaborFilterR2") == 0 )
                    result= new MatchedGaussGaborFilterR2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("MatchedCorrelationGaussGaborFilterR2") == 0 )
                    result= new MatchedCorrelationGaussGaborFilterR2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("MatchedCCorrelationGaussGaborFilterR2") == 0 )
                    result= new MatchedCCorrelationGaussGaborFilterR2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);*/
                if ( name.compare("MatchedGaborFilter2Magnitude") == 0 )
                    result= new MatchedGaborFilter2Magnitude<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("DifferenceMatchedGaborRFeature2") == 0 )
                    result= new DifferenceMatchedGaborRFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);

                if ( name.compare("GaborFilter2Magnitude") == 0 )
                    result= new GaborFilter2Magnitude<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4]);
                if ( name.compare("GaborFilterR2") == 0 )
                    result= new GaborFilterR2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4]);
                if ( name.compare("JunctionGaborFilterR2") == 0 )
                    result= new JunctionGaborFilterR2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5]);
                if ( name.compare("CorrelationJunctionGaborFilterR2") == 0 )
                    result= new JunctionGaborFilterR2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5]);
                if ( name.compare("CCorrelationJunctionGaborFilterR2") == 0 )
                    result= new JunctionGaborFilterR2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5]);
                if ( name.compare("GaborFilterI2") == 0 )
                    result= new GaborFilterI2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4]);
                if ( name.compare("CorrelationGaborFilterR2") == 0 )
                    result= new CorrelationGaborFilterR2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4]);
                if ( name.compare("CCorrelationGaborFilterR2") == 0 )
                    result= new CCorrelationGaborFilterR2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4]);
                if ( name.compare("PowerGaborFilterR2") == 0 )
                    result= new PowerGaborFilterR2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5]);
                if ( name.compare("CorrelationPowerGaborFilterR2") == 0 )
                    result= new CorrelationPowerGaborFilterR2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5]);
                if ( name.compare("CCorrelationPowerGaborFilterR2") == 0 )
                    result= new CCorrelationPowerGaborFilterR2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5]);
                if ( name.compare("PowerGaborFilterSimpleR2") == 0 )
                    result= new PowerGaborFilterSimpleR2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5]);
                if ( name.compare("CorrelationPowerGaborFilterSimpleR2") == 0 )
                    result= new CorrelationPowerGaborFilterSimpleR2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5]);
                if ( name.compare("CCorrelationPowerGaborFilterSimpleR2") == 0 )
                    result= new CCorrelationPowerGaborFilterSimpleR2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5]);
                if ( name.compare("MorphologicalContrastEnhancement") == 0 )
                    result= new MorphologicalContrastEnhancement<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("ContrastLimitedAdaptiveHistogramEqualization") == 0 )
                    result= new ContrastLimitedAdaptiveHistogramEqualization<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("EstimationOfBackgroundLuminosityAndContrastVariability") == 0 )
                    result= new EstimationOfBackgroundLuminosityAndContrastVariability<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("DivisionByAnOverSmoothed") == 0 )
                    result= new DivisionByAnOverSmoothed<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("DivisionByAnOverSmoothedMean") == 0 )
                    result= new DivisionByAnOverSmoothedMean<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("DesiredAverageIntensity") == 0 )
                    result= new DesiredAverageIntensity<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("AdaptiveHistogramEqualization") == 0 )
                    result= new AdaptiveHistogramEqualization<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("BackgroundSubtraction") == 0 )
                    result= new BackgroundSubtraction<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("HistogramEqualization") == 0 )
                    result= new HistogramEqualization<INPUT, OUTPUT>();
                if ( name.compare("AdaptiveLocalContrastEnhancement") == 0 )
                    result= new AdaptiveLocalContrastEnhancement<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("ExponentialHistogramEqualization") == 0 )
                    result= new ExponentialHistogramEqualization<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("RayleighHistogramEqualization") == 0 )
                    result= new RayleighHistogramEqualization<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("HyperbolicHistogramEqualization") == 0 )
                    result= new HyperbolicHistogramEqualization<INPUT, OUTPUT>();

                if ( name.compare("SobelFilter") == 0 )
                    result= new SobelFilter<INPUT, OUTPUT>();
                if ( name.compare("SobelFilterX") == 0 )
                    result= new SobelFilterX<INPUT, OUTPUT>();
                if ( name.compare("SobelFilterY") == 0 )
                    result= new SobelFilterY<INPUT, OUTPUT>();
                if ( name.compare("SobelFilterMagnitude") == 0 )
                    result= new SobelFilterMagnitude<INPUT, OUTPUT>();
                if ( name.compare("SobelFilterOrientation") == 0 )
                    result= new SobelFilterOrientation<INPUT, OUTPUT>();
                if ( name.compare("LaplacianFilter") == 0 )
                    result= new LaplacianFilter<INPUT, OUTPUT>();
                if ( name.compare("PrewittFilter") == 0 )
                    result= new PrewittFilter<INPUT, OUTPUT>();
                if ( name.compare("GaussianFilter2") == 0 )
                    result= new GaussianFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5]);
                if ( name.compare("GaussianDxFilter2") == 0)
                    result= new GaussianDxFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5]);
                if ( name.compare("GaussianDyFilter2") == 0 )
                    result= new GaussianDyFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5]);
                if ( name.compare("GaussianDxyFilter2") == 0 )
                    result= new GaussianDxyFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5]);
                if ( name.compare("GaussianDxxFilter2") == 0 )
                    result= new GaussianDxxFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5]);
                if ( name.compare("GaussianDyyFilter2") == 0 )
                    result= new GaussianDyyFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5]);
                if ( name.compare("MatchedCorrelationGaussianFilter2") == 0 )
                {
                    MatchedCorrelationGaussianFilter2<INPUT, OUTPUT>* f= new MatchedCorrelationGaussianFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5]);
                    f->setMatchType(parameters[6], parameters[7], parameters[8], (openip::MATCHED_GAUSSIAN_FILTER_TYPE)(int)(parameters[9]));
                    result= f;
                }
                if ( name.compare("MatchedCCorrelationGaussianFilter2") == 0 )
                {
                    MatchedCCorrelationGaussianFilter2<INPUT, OUTPUT>* f= new MatchedCCorrelationGaussianFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5]);
                    f->setMatchType(parameters[6], parameters[7], parameters[8], (openip::MATCHED_GAUSSIAN_FILTER_TYPE)(int)(parameters[9]));
                    result= f;
                }
                if ( name.compare("MatchedGaussianFilter2") == 0 )
                {
                    MatchedGaussianFilter2<INPUT, OUTPUT>* f= new MatchedGaussianFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5]);
                    f->setMatchType(parameters[6], parameters[7], parameters[8], (openip::MATCHED_GAUSSIAN_FILTER_TYPE)(int)(parameters[9]));
                    result= f;
                }
                if ( name.compare("MatchedCorrelationLineSegmentFilter2") == 0 )
                    result= new MatchedCorrelationLineSegmentFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9]);
                if ( name.compare("DifferenceMatchedCorrelationLineSegmentFeature2") == 0 )
                    result= new DifferenceMatchedCorrelationLineSegmentFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9], parameters[10]);
                if ( name.compare("DifferenceMatchedLineSegmentFeature2") == 0 )
                    result= new DifferenceMatchedLineSegmentFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9], parameters[10]);
                if ( name.compare("MatchedLineSegmentFilter2") == 0 )
                    result= new MatchedLineSegmentFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9]);
                if ( name.compare("LineSegmentFilter") == 0 )
                    result= new LineSegmentFilter<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("CorrelationLineSegmentFilter") == 0 )
                    result= new CorrelationLineSegmentFilter<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("TJunctionFilter2") == 0 )
                    result= new TJunctionFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("MatchedTJunctionFilter2") == 0 )
                {
                    MatchedTJunctionFilter2<INPUT, OUTPUT>* mtjf= new MatchedTJunctionFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9], parameters[10], parameters[11], parameters[12], parameters[13]);

                    Image<float> img;
                    char filename[30];

                    for ( unsigned int i= 0; i < mtjf->size(); ++i )
                    {
                        sprintf(filename, "tjunction%d.jpg", i);
                        (*mtjf)[i]->dumpFilter(img);
                        img.normalize(0, 255);
                        writeImage(filename, img);
                    }
                    result= mtjf;
                }
                if ( name.compare("SNRFeature2") == 0 )
                    result= new SNRFeature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("VarianceFeature2") == 0 )
                    result= new VarianceFeature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("InvariantHuMoment1Feature2") == 0 )
                    result= new InvariantHuMoment1Feature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("InvariantHuMoment2Feature2") == 0 )
                    result= new InvariantHuMoment2Feature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("InvariantHuMoment3Feature2") == 0 )
                    result= new InvariantHuMoment3Feature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("InvariantHuMoment4Feature2") == 0 )
                    result= new InvariantHuMoment4Feature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("InvariantHuMoment5Feature2") == 0 )
                    result= new InvariantHuMoment5Feature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("InvariantHuMoment6Feature2") == 0 )
                    result= new InvariantHuMoment6Feature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("InvariantHuMoment7Feature2") == 0 )
                    result= new InvariantHuMoment7Feature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("InvariantHuMoment8Feature2") == 0 )
                    result= new InvariantHuMoment8Feature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("EccentricityFeature2") == 0 )
                    result= new EccentricityFeature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("OrientationFeature2") == 0 )
                    result= new OrientationFeature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("CentralMomentFeature2") == 0 )
                    result= new CentralMomentFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
                if ( name.compare("MaxFeature2") == 0 )
                    result= new MaxFeature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("MinFeature2") == 0 )
                    result= new MinFeature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("MedianFeature2") == 0 )
                    result= new MedianFeature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("MeanFeature2") == 0 )
                    result= new MeanFeature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("VarianceFeature2") == 0 )
                    result= new VarianceFeature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("SkewnessFeature2") == 0 )
                    result= new SkewnessFeature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("KurtosisFeature2") == 0 )
                    result= new KurtosisFeature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("StandardDeviationFeature2") == 0 )
                    result= new StandardDeviationFeature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("NormalizationFeature2") == 0 )
                    result= new NormalizationFeature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("NormalizationTransform2") == 0 )
                    result= new NormalizationTransform2<INPUT, OUTPUT>();
                if ( name.compare("NormalizationIntoRange") == 0 )
                    result= new NormalizationIntoRange<INPUT, OUTPUT>(parameters[0], parameters[1]);
                
                if ( name.compare("DifferenceOfGaussianDyyFeature2") == 0 )
                    result= new DifferenceOfGaussianDyyFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("DifferenceOfGaussianDxxFeature2") == 0 )
                    result= new DifferenceOfGaussianDxxFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("DifferenceOfGaussianDxyFeature2") == 0 )
                    result= new DifferenceOfGaussianDxyFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("DifferenceOfGaussianDyFeature2") == 0 )
                    result= new DifferenceOfGaussianDyFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("DifferenceOfGaussianDxFeature2") == 0 )
                    result= new DifferenceOfGaussianDxFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("DifferenceOfGaussianFeature2") == 0 )
                    result= new DifferenceOfGaussianFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("DifferenceOfMinFeature2") == 0 )
                    result= new DifferenceOfMinFeature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("DifferenceOfMaxFeature2") == 0 )
                    result= new DifferenceOfMaxFeature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("DifferenceOfMeanFeature2") == 0 )
                    result= new DifferenceOfMeanFeature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("DifferenceOfVarianceFeature2") == 0 )
                    result= new DifferenceOfVarianceFeature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("DifferenceOfStandardDeviationFeature2") == 0 )
                    result= new DifferenceOfStandardDeviationFeature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("DifferenceOfCentralMomentFeature2") == 0 )
                    result= new DifferenceOfCentralMomentFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5]);
                if ( name.compare("DifferenceOfInvariantHuMoment1Feature2") == 0 )
                    result= new DifferenceOfInvariantHuMoment1Feature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("DifferenceOfInvariantHuMoment2Feature2") == 0 )
                    result= new DifferenceOfInvariantHuMoment2Feature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("DifferenceOfInvariantHuMoment3Feature2") == 0 )
                    result= new DifferenceOfInvariantHuMoment3Feature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("DifferenceOfInvariantHuMoment4Feature2") == 0 )
                    result= new DifferenceOfInvariantHuMoment4Feature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("DifferenceOfInvariantHuMoment5Feature2") == 0 )
                    result= new DifferenceOfInvariantHuMoment5Feature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("DifferenceOfInvariantHuMoment6Feature2") == 0 )
                    result= new DifferenceOfInvariantHuMoment6Feature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("DifferenceOfInvariantHuMoment7Feature2") == 0 )
                    result= new DifferenceOfInvariantHuMoment7Feature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("DifferenceOfEccentricityFeature2") == 0 )
                    result= new DifferenceOfEccentricityFeature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("DifferenceOfOrientationFeature2") == 0 )
                    result= new DifferenceOfOrientationFeature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("DifferenceOfSNRFeature2") == 0 )
                    result= new DifferenceOfSNRFeature2<INPUT, OUTPUT>(parameters[0], parameters[1]);

                if ( name.compare("RatioOfGaussianDyyFeature2") == 0 )
                    result= new RatioOfGaussianDyyFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("RatioOfGaussianDxxFeature2") == 0 )
                    result= new RatioOfGaussianDxxFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("RatioOfGaussianDxyFeature2") == 0 )
                    result= new RatioOfGaussianDxyFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("RatioOfGaussianDyFeature2") == 0 )
                    result= new RatioOfGaussianDyFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("RatioOfGaussianDxFeature2") == 0 )
                    result= new RatioOfGaussianDxFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("RatioOfGaussianFeature2") == 0 )
                    result= new RatioOfGaussianFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("RatioOfMinFeature2") == 0 )
                    result= new RatioOfMinFeature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("RatioOfMaxFeature2") == 0 )
                    result= new RatioOfMaxFeature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("RatioOfMeanFeature2") == 0 )
                    result= new RatioOfMeanFeature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("RatioOfVarianceFeature2") == 0 )
                    result= new RatioOfVarianceFeature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("RatioOfStandardDeviationFeature2") == 0 )
                    result= new RatioOfStandardDeviationFeature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("RatioOfCentralMomentFeature2") == 0 )
                    result= new RatioOfCentralMomentFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5]);
                if ( name.compare("RatioOfInvariantHuMoment1Feature2") == 0 )
                    result= new RatioOfInvariantHuMoment1Feature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("RatioOfInvariantHuMoment2Feature2") == 0 )
                    result= new RatioOfInvariantHuMoment2Feature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("RatioOfInvariantHuMoment3Feature2") == 0 )
                    result= new RatioOfInvariantHuMoment3Feature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("RatioOfInvariantHuMoment4Feature2") == 0 )
                    result= new RatioOfInvariantHuMoment4Feature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("RatioOfInvariantHuMoment5Feature2") == 0 )
                    result= new RatioOfInvariantHuMoment5Feature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("RatioOfInvariantHuMoment6Feature2") == 0 )
                    result= new RatioOfInvariantHuMoment6Feature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("RatioOfInvariantHuMoment7Feature2") == 0 )
                    result= new RatioOfInvariantHuMoment7Feature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("RatioOfEccentricityFeature2") == 0 )
                    result= new RatioOfEccentricityFeature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("RatioOfOrientationFeature2") == 0 )
                    result= new RatioOfOrientationFeature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("RatioOfSNRFeature2") == 0 )
                    result= new RatioOfSNRFeature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("IdenticalFilter2") == 0 )
                    result= new IdenticalFilter2<INPUT, OUTPUT>();
                if ( name.compare("InversionFilter2") == 0 )
                    result= new InversionFilter2<INPUT, OUTPUT>();
                if ( name.compare("Normalization2") == 0 )
                    result= new Normalization2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("DoubleRingMeanFeature2") == 0 )
                    result= new DoubleRingMeanFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3]);
                if ( name.compare("DoubleRingMeanRatioFeature2") == 0 )
                    result= new DoubleRingMeanRatioFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3]);
                if ( name.compare("DoubleRingMaxFeature2") == 0 )
                    result= new DoubleRingMaxFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3]);
                if ( name.compare("DoubleRingMinFeature2") == 0 )
                    result= new DoubleRingMinFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3]);
                if ( name.compare("RunningRingMeanFeature2") == 0 )
                    result= new RunningRingMeanFeature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("RunningRingVarianceFeature2") == 0 )
                    result= new RunningRingVarianceFeature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("RunningRingMinFeature2") == 0 )
                    result= new RunningRingMinFeature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("RunningRingMaxFeature2") == 0 )
                    result= new RunningRingMaxFeature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("RunningDifferenceRingMeanFeature2") == 0 )
                    result= new RunningDifferenceRingMeanFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3]);
                if ( name.compare("RunningDifferenceRingVarianceFeature2") == 0 )
                    result= new RunningDifferenceRingVarianceFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3]);
                if ( name.compare("RunningDifferenceRingMinFeature2") == 0 )
                    result= new RunningDifferenceRingMinFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3]);
                if ( name.compare("RunningDifferenceRingMaxFeature2") == 0 )
                    result= new RunningDifferenceRingMaxFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3]);
                if ( name.compare("RunningDifferenceRingMaxMeanFeature2") == 0 )
                    result= new RunningDifferenceRingMaxMeanFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3]);
                if ( name.compare("RunningDifferenceRingMaxMinFeature2") == 0 )
                    result= new RunningDifferenceRingMaxMinFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3]);
                if ( name.compare("RunningDifferenceRingMeanRatioFeature2") == 0 )
                    result= new RunningDifferenceRingMeanRatioFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3]);
                if ( name.compare("RunningDifferenceRingVarianceRatioFeature2") == 0 )
                    result= new RunningDifferenceRingVarianceRatioFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3]);
                if ( name.compare("RunningDifferenceRingMinRatioFeature2") == 0 )
                    result= new RunningDifferenceRingMinRatioFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3]);
                if ( name.compare("RunningDifferenceRingMaxRatioFeature2") == 0 )
                    result= new RunningDifferenceRingMaxRatioFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3]);
                if ( name.compare("GaussianDerivedMagnitude") == 0 )
                    result= new GaussianDerivedMagnitude<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5]);
                if ( name.compare("MatchedGaussianDerivedMagnitude") == 0 )
                    result= new MatchedGaussianDerivedMagnitude<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);
                if ( name.compare("HistogramVectorFeature2") == 0 )
                    result= new HistogramVectorFeature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("GammaCorrection") == 0 )
                    result= new GammaCorrection<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("AdaptiveMedianFilter") == 0 )
                    result= new AdaptiveMedianFilter<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("GrayscaleAreaOpening") == 0 )
                    result= new GrayscaleAreaOpening<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("GrayscaleAreaClosing") == 0 )
                    result= new GrayscaleAreaClosing<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("WhiteAreaTopHat") == 0 )
                    result= new WhiteAreaTopHat<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("BlackAreaTopHat") == 0 )
                    result= new BlackAreaTopHat<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("GrayscaleOpen") == 0 )
                    result= new GrayscaleOpen<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("GrayscaleClose") == 0 )
                    result= new GrayscaleClose<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("GrayscaleWhiteTopHat") == 0 )
                    result= new GrayscaleWhiteTopHat<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("GrayscaleBlackTopHat") == 0 )
                    result= new GrayscaleBlackTopHat<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("GrayscaleBrightReconstruction") == 0 )
                    result= new GrayscaleBrightReconstruction<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("GrayscaleDarkReconstruction") == 0 )
                    result= new GrayscaleDarkReconstruction<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("GrayscaleBrightReconstructionTopHat") == 0 )
                    result= new GrayscaleBrightReconstructionTopHat<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("GrayscaleDarkReconstructionTopHat") == 0 )
                    result= new GrayscaleDarkReconstructionTopHat<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("LoGFilter2") == 0 )
                    result= new LoGFilter2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("CorrelationLoGFilter2") == 0 )
                    result= new CorrelationLoGFilter2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("MatchedLoGFilter2") == 0 )
                    result= new MatchedLoGFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3]);
                if ( name.compare("MatchedCorrelationLoGFilter2") == 0 )
                    result= new MatchedCorrelationLoGFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3]);
                if ( name.compare("AdaptiveLocalGammaCorrection") == 0 )
                    result= new AdaptiveLocalGammaCorrection<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("AdaptiveGlobalGammaCorrection") == 0 )
                    result= new AdaptiveGlobalGammaCorrection<INPUT, OUTPUT>();
                /*fourier filters*/
                if ( name.compare("FourierMatchedLineSegmentFilter2") == 0 )
                    result= new FourierMatchedLineSegmentFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9]);
                if ( name.compare("FourierMatchedCorrelationLineSegmentFilter2") == 0 )
                    result= new FourierMatchedCorrelationLineSegmentFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9]);
                if ( name.compare("FourierMatchedGaussianFilter2") == 0 )
                    result= new FourierMatchedGaussianFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);
                if ( name.compare("FourierMatchedCorrelationGaussianFilter2") == 0 )
                    result= new FourierMatchedCorrelationGaussianFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);
                if ( name.compare("FourierMatchedLoGFilter2") == 0 )
                    result= new FourierMatchedLoGFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3]);
                if ( name.compare("FourierMatchedCorrelationLoGFilter2") == 0 )
                    result= new FourierMatchedLoGFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3]);
                if ( name.compare("FourierMatchedGaborFilter2") == 0 )
                    result= new FourierMatchedGaborFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("FourierMatchedGaborFilterR2") == 0 )
                    result= new FourierMatchedGaborFilterR2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("FourierMatchedCorrelationGaborFilter2") == 0 )
                    result= new FourierMatchedCorrelationGaborFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("DifferenceFourierMatchedLineSegmentFilter2") == 0 )
                    result= new DifferenceFourierMatchedLineSegmentFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9], parameters[10]);
                if ( name.compare("DifferenceFourierMatchedCorrelationLineSegmentFilter2") == 0 )
                    result= new DifferenceFourierMatchedCorrelationLineSegmentFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9], parameters[10]);
                if ( name.compare("DifferenceFourierMatchedGaussianFilter2") == 0 )
                    result= new DifferenceFourierMatchedGaussianFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9], parameters[10]);
                if ( name.compare("DifferenceFourierMatchedCorrelationGaussianFilter2") == 0 )
                    result= new DifferenceFourierMatchedCorrelationGaussianFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9], parameters[10]);
                if ( name.compare("DifferenceFourierMatchedLoGFilter2") == 0 )
                    result= new DifferenceFourierMatchedLoGFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("DifferenceFourierMatchedCorrelationLoGFilter2") == 0 )
                    result= new DifferenceFourierMatchedCorrelationLoGFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("DifferenceFourierMatchedGaborFilter2") == 0 )
                    result= new DifferenceFourierMatchedGaborFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);
                if ( name.compare("DifferenceFourierMatchedCorrelationGaborFilter2") == 0 )
                    result= new DifferenceFourierMatchedCorrelationGaborFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);
                if ( name.compare("ScalingToRoiDiameter") == 0 )
                    result= new ScalingToRoiDiameter<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("NormalizationInto01Feature2") == 0 )
                    result= new NormalizationInto01Feature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("LocalMinimumGlobalMaximum") == 0 )
                    result= new LocalMinimumGlobalMaximum<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("LocalMaximumGlobalMinimum") == 0 )
                    result= new LocalMaximumGlobalMinimum<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("GreaterThanFeature2") == 0 )
                    result= new GreaterThanFeature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("AFeature2") == 0 )
                    result= new AFeature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("BFeature2") == 0 )
                    result= new BFeature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("CFeature2") == 0 )
                    result= new CFeature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("ParametricMorphologicalReconstruction2") == 0 )
                    result= new ParametricMorphologicalReconstruction2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("ParametricMorphologicalReconstructionROI2") == 0 )
                    result= new ParametricMorphologicalReconstructionROI2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("ATransform") == 0 )
                    result= new ATransform<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("BTransform") == 0 )
                    result= new BTransform<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("DFeature2") == 0 )
                    result= new DFeature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("EFeature2") == 0 )
                    result= new EFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
                if ( name.compare("FFeature2") == 0 )
                    result= new FFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
                if ( name.compare("GFeature2") == 0 )
                    result= new GFeature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("HFeature2") == 0 )
                    result= new HFeature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("HorizontalFourierMaxMagnitudeFeature2") == 0 )
                    result= new HorizontalFourierMaxMagnitudeFeature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("BackgroundEstimation") == 0 )
                    result= new BackgroundEstimation<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("NormalizedGammaCorrection") == 0 )
                    result= new NormalizedGammaCorrection<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("BrightRotatedForegroundFeature2") == 0 )
                    result= new BrightRotatedForegroundFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("DarkRotatedForegroundFeature2") == 0 )
                    result= new DarkRotatedForegroundFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("FillSupport") == 0 )
                    result= new FillSupport<INPUT, OUTPUT>();
                if ( name.compare("AnisotropicDiffusionPeronaMalik") == 0 )
                    result= new AnisotropicDiffusionPeronaMalik<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
                if ( name.compare("ThinVesselTransform") == 0 )
                    result= new ThinVesselTransform<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("LaplacianSharpening") == 0 )
                    result= new LaplacianSharpening<INPUT, OUTPUT>();
                /*if ( name.compare("GaborFilterR2V") == 0 )
                    result= new GaborFilterR2V<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("CorrelationGaborFilterR2V") == 0 )
                    result= new CorrelationGaborFilterR2V<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("OrthogonalGaborFilterR2V") == 0 )
                    result= new OrthogonalGaborFilterR2V<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("RotatedOrthogonalGaborFilterR2V") == 0 )
                    result= new RotatedOrthogonalGaborFilterR2V<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("MaxPhasedOrthogonalGaborFilterR2V") == 0 )
                    result= new MaxPhasedOrthogonalGaborFilterR2V<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("RotatedOrthogonalCorrelationGaborFilterR2V") == 0 )
                    result= new RotatedOrthogonalCorrelationGaborFilterR2V<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("OrthogonalLineSegmentFilter2V") == 0 )
                    result= new OrthogonalLineSegmentFilter2V<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9], parameters[10]);
                if ( name.compare("RotatedOrthogonalLineSegmentFilter2V") == 0 )
                    result= new RotatedOrthogonalLineSegmentFilter2V<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9], parameters[10]);
                if ( name.compare("RotatedOrthogonalCorrelationScalesGaborFilterR2V") == 0 )
                    result= new RotatedOrthogonalCorrelationScalesGaborFilterR2V<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("RotatedOrthogonalCorrelationLambdasGaborFilterR2V") == 0 )
                    result= new RotatedOrthogonalCorrelationLambdasGaborFilterR2V<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("RotatedCorrelationLambdasGaborFilterR2V") == 0 )
                    result= new RotatedCorrelationLambdasGaborFilterR2V<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("RotatedCorrelationGammasGaborFilterR2V") == 0 )
                    result= new RotatedCorrelationGammasGaborFilterR2V<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("RotatedOrthogonalCorrelationPhasesGaborFilterR2V") == 0 )
                    result= new RotatedOrthogonalCorrelationPhasesGaborFilterR2V<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("RotatedCorrelationGaborFilterR2V") == 0 )
                    result= new RotatedCorrelationGaborFilterR2V<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);*/

                if ( name.compare("ChannelTransform2") == 0 )
                    result= new ChannelTransform2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("HSVHTransform2") == 0 )
                    result= new HSVHTransform2<INPUT, OUTPUT>();
                if ( name.compare("LUVLTransform2") == 0 )
                    result= new LUVLTransform2<INPUT, OUTPUT>();
                if ( name.compare("LUVUTransform2") == 0 )
                    result= new LUVUTransform2<INPUT, OUTPUT>();
                if ( name.compare("LUVVTransform2") == 0 )
                    result= new LUVVTransform2<INPUT, OUTPUT>();

                if ( name.compare("AxisSimmetryFeature2") == 0 )
                    result= new AxisSimmetryFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
                if ( name.compare("AxisAntiSimmetryFeature2") == 0 )
                    result= new AxisSimmetryFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
                if ( name.compare("DistanceOfCenterOfGravityFeature2") == 0 )
                    result= new DistanceOfCenterOfGravityFeature2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("MatchedAxisSimmetryFeature2") == 0 )
                    result= new MatchedAxisSimmetryFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4]);
                if ( name.compare("MatchedAxisAntiSimmetryFeature2") == 0 )
                    result= new MatchedAxisAntiSimmetryFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4]);
                if ( name.compare("FullLocalRange") == 0 )
                    result= new FullLocalRange<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("MatchedMaxNormalizationFeature2") == 0 )
                    result= new MatchedMaxNormalizationFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
                if ( name.compare("MatchedMinNormalizationFeature2") == 0 )
                    result= new MatchedMinNormalizationFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
                if ( name.compare("MatchedMaxMeanFeature2") == 0 )
                    result= new MatchedMaxMeanFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
                if ( name.compare("MatchedMinMeanFeature2") == 0 )
                    result= new MatchedMinMeanFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
                if ( name.compare("MatchedMaxVarianceFeature2") == 0 )
                    result= new MatchedMaxVarianceFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
                if ( name.compare("MatchedMinVarianceFeature2") == 0 )
                    result= new MatchedMinVarianceFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
                if ( name.compare("MatchedMaxStandardDeviationFeature2") == 0 )
                    result= new MatchedMaxStandardDeviationFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
                if ( name.compare("MatchedMinStandardDeviationFeature2") == 0 )
                    result= new MatchedMinStandardDeviationFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
                if ( name.compare("MatchedMaxSNRFeature2") == 0 )
                    result= new MatchedMaxSNRFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
                if ( name.compare("MatchedMinSNRFeature2") == 0 )
                    result= new MatchedMinSNRFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
                if ( name.compare("MatchedMinKurtosisFeature2") == 0 )
                    result= new MatchedMinKurtosisFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
                if ( name.compare("MatchedMaxKurtosisFeature2") == 0 )
                    result= new MatchedMaxKurtosisFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
                if ( name.compare("MatchedMinSkewnessFeature2") == 0 )
                    result= new MatchedMinSkewnessFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
                if ( name.compare("MatchedMaxSkewnessFeature2") == 0 )
                    result= new MatchedMaxSkewnessFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
                if ( name.compare("IdenticalFeature2") == 0 )
                    result= new IdenticalFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
                if ( name.compare("MatchedMinGaussianFilter2") == 0 )
                    result= new MatchedMinGaussianFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("MatchedMaxGaussianFilter2") == 0 )
                    result= new MatchedMaxGaussianFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("LimitRangeTransform2") == 0 )
                    result= new LimitRangeTransform2<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("AverageBackgroundSubtraction") == 0 )
                    result= new AverageBackgroundSubtraction<INPUT, OUTPUT>(parameters[0], parameters[1]);
                /*if ( name.compare("RotatedCorrelationPhaseShiftedGaborFilterR2V") == 0 )
                    result= new RotatedCorrelationPhaseShiftedGaborFilterR2V<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("RotatedPhaseShiftedGaborFilterR2V") == 0 )
                    result= new RotatedPhaseShiftedGaborFilterR2V<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("FittedPhaseCorrelationGaborFilterR2V") == 0 )
                    result= new FittedPhaseCorrelationGaborFilterR2V<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9]);
                if ( name.compare("MatchedMinShiftedGaussianFilter2") == 0 )
                    result= new MatchedMinShiftedGaussianFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5]);
                if ( name.compare("MatchedMaxShiftedGaussianFilter2") == 0 )
                    result= new MatchedMaxShiftedGaussianFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5]);
                if ( name.compare("MatchedMinShiftedCorrelationGaussianFilter2") == 0 )
                    result= new MatchedMinShiftedCorrelationGaussianFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5]);
                if ( name.compare("MatchedMaxShiftedCorrelationGaussianFilter2") == 0 )
                    result= new MatchedMaxShiftedCorrelationGaussianFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5]);
                if ( name.compare("RotatedShiftedCorrelationGaborFilterR2V") == 0 )
                    result= new RotatedShiftedCorrelationGaborFilterR2V<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("RotatedShiftedGaborFilterR2V") == 0 )
                    result= new RotatedShiftedGaborFilterR2V<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("OrthogonalSectionFilter2V") == 0 )
                    result= new OrthogonalSectionFilter2V<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);*/
                if ( name.compare("PCADirectionFeature2") == 0 )
                    result= new PCADirectionFeature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("RotatedLambdaSpaceGaborFilterSet2") == 0 )
                    result= new RotatedLambdaSpaceGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("RotatedLambdaSpaceCorrelationGaborFilterSet2") == 0 )
                    result= new RotatedLambdaSpaceCorrelationGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("RotatedLambdaSpaceCCorrelationGaborFilterSet2") == 0 )
                    result= new RotatedLambdaSpaceCCorrelationGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("OrthogonalSectionFilterSet2") == 0 )
                    result= new OrthogonalSectionFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4]);
                if ( name.compare("GradientVectorFlowU") == 0 )
                    result= new GradientVectorFlowU<INPUT, OUTPUT>();
                if ( name.compare("GradientVectorFlowV") == 0 )
                    result= new GradientVectorFlowV<INPUT, OUTPUT>();
                if ( name.compare("GradientVectorFlow") == 0 )
                    result= new GradientVectorFlow<INPUT, OUTPUT>();

                if ( name.compare("GaborFilterSet2") == 0 )
                    result= new GaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("CorrelationGaborFilterSet2") == 0 )
                    result= new CorrelationGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("CCorrelationGaborFilterSet2") == 0 )
                    result= new CCorrelationGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("RotatedGaborFilterSet2") == 0 )
                    result= new RotatedGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("RotatedGaborFilterSet2") == 0 )
                    result= new RotatedGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("RotatedCorrelationGaborFilterSet2") == 0 )
                    result= new RotatedCorrelationGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("RotatedCCorrelationGaborFilterSet2") == 0 )
                    result= new RotatedCCorrelationGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("ShiftedGaborFilterSet2") == 0 )
                    result= new ShiftedGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);
                if ( name.compare("ShiftedCorrelationGaborFilterSet2") == 0 )
                    result= new ShiftedCorrelationGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);
                if ( name.compare("RotatedShiftedGaborFilterSet2") == 0 )
                    result= new RotatedShiftedGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("RotatedShiftedCorrelationGaborFilterSet2") == 0 )
                    result= new RotatedShiftedCorrelationGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("RotatedShiftedCCorrelationGaborFilterSet2") == 0 )
                    result= new RotatedShiftedCCorrelationGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("RotatedPhaseShiftedGaborFilterSet2") == 0 )
                    result= new RotatedPhaseShiftedGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("RotatedPhaseShiftedCorrelationGaborFilterSet2") == 0 )
                    result= new RotatedPhaseShiftedCorrelationGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("RotatedPhaseShiftedCCorrelationGaborFilterSet2") == 0 )
                    result= new RotatedPhaseShiftedCCorrelationGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("RotatedMultiScaleGaborFilterSet2") == 0 )
                    result= new RotatedMultiScaleGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("RotatedMultiScaleCorrelationGaborFilterSet2") == 0 )
                    result= new RotatedMultiScaleCorrelationGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("RotatedMultiScaleCCorrelationGaborFilterSet2") == 0 )
                    result= new RotatedMultiScaleCCorrelationGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("RotatedMultiScaleShiftedGaborFilterSet2") == 0 )
                    result= new RotatedMultiScaleShiftedGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9], parameters[10]);
                if ( name.compare("RotatedMultiScaleShiftedCorrelationGaborFilterSet2") == 0 )
                    result= new RotatedMultiScaleShiftedCorrelationGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9], parameters[10]);
                if ( name.compare("RotatedMultiScaleShiftedCCorrelationGaborFilterSet2") == 0 )
                    result= new RotatedMultiScaleShiftedCCorrelationGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9], parameters[10]);
                if ( name.compare("RotatedMultiScalePhaseShiftedGaborFilterSet2") == 0 )
                    result= new RotatedMultiScalePhaseShiftedGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9], parameters[10]);
                if ( name.compare("RotatedMultiScalePhaseShiftedCorrelationGaborFilterSet2") == 0 )
                    result= new RotatedMultiScalePhaseShiftedCorrelationGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9], parameters[10]);
                if ( name.compare("RotatedMultiScalePhaseShiftedCCorrelationGaborFilterSet2") == 0 )
                    result= new RotatedMultiScalePhaseShiftedCCorrelationGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9], parameters[10]);

                if ( name.compare("GaussGaborFilterSet2") == 0 )
                    result= new GaussGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("CorrelationGaussGaborFilterSet2") == 0 )
                    result= new CorrelationGaussGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("CCorrelationGaussGaborFilterSet2") == 0 )
                    result= new CCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("RotatedGaussGaborFilterSet2") == 0 )
                    result= new RotatedGaussGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("RotatedCorrelationGaussGaborFilterSet2") == 0 )
                    result= new RotatedCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("RotatedCCorrelationGaussGaborFilterSet2") == 0 )
                    result= new RotatedCCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                if ( name.compare("ShiftedGaussGaborFilterSet2") == 0 )
                    result= new ShiftedGaussGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);
                if ( name.compare("ShiftedCorrelationGaussGaborFilterSet2") == 0 )
                    result= new ShiftedCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);
                if ( name.compare("RotatedShiftedGaussGaborFilterSet2") == 0 )
                    result= new RotatedShiftedGaussGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("RotatedShiftedCorrelationGaussGaborFilterSet2") == 0 )
                    result= new RotatedShiftedCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("RotatedShiftedCCorrelationGaussGaborFilterSet2") == 0 )
                    result= new RotatedShiftedCCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("RotatedPhaseShiftedGaussGaborFilterSet2") == 0 )
                    result= new RotatedPhaseShiftedGaussGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("RotatedPhaseShiftedCorrelationGaussGaborFilterSet2") == 0 )
                    result= new RotatedPhaseShiftedCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("RotatedPhaseShiftedCCorrelationGaussGaborFilterSet2") == 0 )
                    result= new RotatedPhaseShiftedCCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("RotatedMultiScaleGaussGaborFilterSet2") == 0 )
                    result= new RotatedMultiScaleGaussGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("RotatedMultiScaleCorrelationGaussGaborFilterSet2") == 0 )
                    result= new RotatedMultiScaleCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("RotatedMultiScaleCCorrelationGaussGaborFilterSet2") == 0 )
                    result= new RotatedMultiScaleCCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("RotatedMultiScaleShiftedGaussGaborFilterSet2") == 0 )
                    result= new RotatedMultiScaleShiftedGaussGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9], parameters[10]);
                if ( name.compare("RotatedMultiScaleShiftedCorrelationGaussGaborFilterSet2") == 0 )
                    result= new RotatedMultiScaleShiftedCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9], parameters[10]);
                if ( name.compare("RotatedMultiScaleShiftedCCorrelationGaussGaborFilterSet2") == 0 )
                    result= new RotatedMultiScaleShiftedCCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9], parameters[10]);
                if ( name.compare("RotatedMultiScalePhaseShiftedGaussGaborFilterSet2") == 0 )
                    result= new RotatedMultiScalePhaseShiftedGaussGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9], parameters[10]);
                if ( name.compare("RotatedMultiScalePhaseShiftedCorrelationGaussGaborFilterSet2") == 0 )
                    result= new RotatedMultiScalePhaseShiftedCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9], parameters[10]);
                if ( name.compare("RotatedMultiScalePhaseShiftedCCorrelationGaussGaborFilterSet2") == 0 )
                    result= new RotatedMultiScalePhaseShiftedCCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9], parameters[10]);

                if ( name.compare("LBPFeature2") == 0 )
                    result= new LBPFeature2<INPUT, OUTPUT>(parameters[0], parameters[1]);
                if ( name.compare("RotationInvariantLBPFeature2") == 0 )
                    result= new RotationInvariantLBPFeature2<INPUT, OUTPUT>(parameters[0], parameters[1]);

                if ( name.compare("GaborRGLineSegmentTransform2") == 0 )
                    result= new GaborRGLineSegmentTransform2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
                if ( name.compare("PowerGaborRGLineSegmentTransform2") == 0 )
                    result= new PowerGaborRGLineSegmentTransform2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9]);
                if ( name.compare("PowerGaborSimpleRGLineSegmentTransform2") == 0 )
                    result= new PowerGaborSimpleRGLineSegmentTransform2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9]);


                //tprintf("%s\n", name.c_str());
            }
            }

            if ( descriptors.size() > 1 )
                return t2chain;
            else
            {
                if ( result == NULL )
                    tprintf("transform chain generation did not succeed, probably misspelled transform name: %s\n", descriptors[0].c_str());
                return result;
            }
        }

        template<typename INPUT, typename OUTPUT>
        Transform2Set<INPUT, OUTPUT>* generateTransform2Set(std::string fdf, std::string keyword)
        {
            Transform2Set<INPUT, OUTPUT>* result= new Transform2Set<INPUT, OUTPUT>();
            VectorTransform2<INPUT, OUTPUT>* tmp;

            FeatureDescriptorParameterFile fdpf(fdf);

            while ( fdpf.next(keyword) )
            {
		//cout << fdpf.tokenString << endl;
                tmp= generateTransform2<INPUT, OUTPUT>(fdpf.tokenString);
                if ( tmp )
                    result->push_back(tmp);
            }

            fdpf.close();
            
            return result;
        }

        template<typename INPUT, typename OUTPUT>
        Transform2Set<INPUT, OUTPUT>* generateTransform2Set(Vector<std::string>& descriptors)
        {
            Transform2Set<INPUT, OUTPUT>* tmp= new Transform2Set<INPUT, OUTPUT>();
            VectorTransform2<INPUT, OUTPUT>* t;
            for ( unsigned int i= 0; i < descriptors.size(); ++i )
            {
                t= generateTransform2<INPUT, OUTPUT>(descriptors[i]);
                if ( t )
                    tmp->push_back(t);
            }

            return tmp;
        }

        template<typename INPUT, typename OUTPUT>
        Transform2Chain<INPUT, OUTPUT>* generateTransform2Chain(std::string fdf, std::string keyword)
        {
            Transform2Chain<INPUT, OUTPUT>* result= new Transform2Chain<INPUT, OUTPUT>();
            VectorTransform2<OUTPUT, OUTPUT>* tmp;

            FeatureDescriptorParameterFile fdpf(fdf);

            while ( fdpf.next(keyword) )
            {
                tmp= /*t2g.*/generateTransform2<OUTPUT, OUTPUT>(fdpf.tokenString);
                if ( tmp )
                    result->tvoo.push_back(tmp);
            }

            fdpf.close();
            
            return result;
        }

        template<typename INPUT, typename OUTPUT>
        Transform2Chain<INPUT, OUTPUT>* generateTransform2Chain(Vector<std::string>& descriptors)
        {
            Transform2Chain<INPUT, OUTPUT>* tmp= new Transform2Chain<INPUT, OUTPUT>();
            VectorTransform2<OUTPUT, OUTPUT>* t;
            for ( unsigned int i= 0; i < descriptors.size(); ++i )
            {
                t= generateTransform2<OUTPUT, OUTPUT>(descriptors[i]);
                if ( t )
                    tmp->tvoo.push_back(t);
            }

            return tmp;
        }
}

#endif	/* TRANSFORM2GENERATOR_H */

