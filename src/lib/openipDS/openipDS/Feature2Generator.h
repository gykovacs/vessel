/**
 * @file Feature2Generator.h
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
 * The Feature2Generator can be used to generate Feature objects from
 * descriptor strings.
 */

#ifndef FEATURE2GENERATOR_H
#define	FEATURE2GENERATOR_H

#include <string>

#include <openipDS/Filter2s.h>
#include <openipDS/GaborFilter2.h>
#include <openipDS/StatisticalFeature2s.h>
#include <openipDS/DifferenceFeature2s.h>
#include <openipDS/RatioFeature2s.h>
#include <openipDS/MatchedFilter2s.h>

namespace openip
{
    /**
     * Feature2Generator can be used to generate Feature2 objects from descriptor strings
     */
    template<typename INPUT, typename OUTPUT>
    class Feature2Generator
    {
    public:
        /**
         * default constructor
         */
        Feature2Generator();

        /**
         * copy constructor
         * @param f instance to copy
         */
        Feature2Generator(const Feature2Generator& f);

        /**
         * destructor
         */
        ~Feature2Generator();

        /**
         * processes the descriptor string
         * @param input descriptor string
         * @param name output parameter, the name of the feature
         * @param parameters output parameter, the numeric parameters
         * @return non-zero in case of errors
         */
        virtual int getFeatureNameAndParameters(std::string& input, std::string& name, vector<float>& parameters);

        /**
         * generates the Feature2 object accordint to the descriptor string
         * @param s descriptor string
         * @return pointer to the generated Feature2 object
         */
        virtual Feature2<INPUT, OUTPUT>* generate(std::string& s);
    };

    template<typename INPUT, typename OUTPUT>
    Feature2Generator<INPUT, OUTPUT>::Feature2Generator()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Feature2Generator<INPUT, OUTPUT>::Feature2Generator(const Feature2Generator& f)
    {
    }

    template<typename INPUT, typename OUTPUT>
    Feature2Generator<INPUT, OUTPUT>::~Feature2Generator()
    {
    }

    template<typename INPUT, typename OUTPUT>
    int Feature2Generator<INPUT, OUTPUT>::getFeatureNameAndParameters(std::string& input, std::string& name, vector<float>& parameters)
    {
        float p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12;

        int f= input.find_first_of(' ');
        if ( f == -1 )
            return -1;

        name= input.substr(0,f);

        parameters.clear();

        int number= sscanf(input.c_str() + f, "%f %f %f %f %f %f %f %f %f %f %f", &p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12);

        if ( number >= 1 )
            parameters.push_back(p1);
        if ( number >= 2 )
            parameters.push_back(p2);
        if ( number >= 3 )
            parameters.push_back(p3);
        if ( number >= 4 )
            parameters.push_back(p4);
        if ( number >= 5 )
            parameters.push_back(p5);
        if ( number >= 6 )
            parameters.push_back(p6);
        if ( number >= 7 )
            parameters.push_back(p7);
        if ( number >= 8 )
            parameters.push_back(p8);
        if ( number >= 9 )
            parameters.push_back(p9);
        if ( number >= 10 )
            parameters.push_back(p10);
        if ( number >= 11 )
            parameters.push_back(p11);
        if ( number >= 12 )
            parameters.push_back(p12);

        return 0;
    }

    template<typename INPUT, typename OUTPUT>
    Feature2<INPUT, OUTPUT>* Feature2Generator<INPUT, OUTPUT>::generate(std::string& s)
    {
        std::string name;
        vector<float> parameters;

        if ( ! getFeatureNameAndParameters(s, name, parameters) )
        {
            /*if ( name.compare("GaborFilter2") == 0 )
                return new GaborFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4]);
            if ( name.compare("GaborFilter2I") == 0 )
                return new GaborFilter2I<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4]);
            if ( name.compare("SobelFilter") == 0 )
                return new SobelFilter<INPUT, OUTPUT>();
            if ( name.compare("LaplacianFilter") == 0 )
                return new LaplacianFilter<INPUT, OUTPUT>();
            if ( name.compare("PrewittFilter") == 0 )
                return new PrewittFilter<INPUT, OUTPUT>();
            if ( name.compare("GaussianDxFilter2") == 0)
                return new GaussianDxFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
            if ( name.compare("GaussianDyFilter2") == 0 )
                return new GaussianDyFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
            if ( name.compare("GaussianDxyFilter2") == 0 )
                return new GaussianDxyFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
            if ( name.compare("GaussianDxxFilter2") == 0 )
                return new GaussianDxxFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
            if ( name.compare("GaussianDyyFilter2") == 0 )
                return new GaussianDyyFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
            if ( name.compare("MatchedCorrelationGaussianFilter2") == 0 )
            {
                MatchedCorrelationGaussianFilter2<INPUT, OUTPUT>* f= new MatchedCorrelationGaussianFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                f->setMatchType(parameters[7], parameters[8], parameters[9], (openip::MATCHED_GAUSSIAN_FILTER_TYPE)(parameters[10]));
                return f;
            }
            if ( name.compare("MatchedGaussianFilter2") == 0 )
            {
                MatchedGaussianFilter2<INPUT, OUTPUT>* f= new MatchedGaussianFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
                f->setMatchType(parameters[7], parameters[8], parameters[9], (openip::MATCHED_GAUSSIAN_FILTER_TYPE)(parameters[10]));
                return f;
            }
            if ( name.compare("MatchedCorrelationLineSegmentFilter2") == 0 )
                return new MatchedCorrelationLineSegmentFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9], parameters[10]);
            if ( name.compare("MatchedLineSegmentFilter2") == 0 )
                return new MatchedLineSegmentFilter2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9], parameters[10]);
            if ( name.compare("SNRFeature2") == 0 )
                return new SNRFeature2<INPUT, OUTPUT>(parameters[0]);
            if ( name.compare("VarianceFeature2") == 0 )
                return new VarianceFeature2<INPUT, OUTPUT>(parameters[0]);
            if ( name.compare("InvariantHuMoment1Feature2") == 0 )
                return new InvariantHuMoment1Feature2<INPUT, OUTPUT>(parameters[0]);
            if ( name.compare("InvariantHuMoment2Feature2") == 0 )
                return new InvariantHuMoment2Feature2<INPUT, OUTPUT>(parameters[0]);
            if ( name.compare("InvariantHuMoment3Feature2") == 0 )
                return new InvariantHuMoment3Feature2<INPUT, OUTPUT>(parameters[0]);
            if ( name.compare("InvariantHuMoment4Feature2") == 0 )
                return new InvariantHuMoment4Feature2<INPUT, OUTPUT>(parameters[0]);
            if ( name.compare("InvariantHuMoment5Feature2") == 0 )
                return new InvariantHuMoment5Feature2<INPUT, OUTPUT>(parameters[0]);
            if ( name.compare("InvariantHuMoment6Feature2") == 0 )
                return new InvariantHuMoment6Feature2<INPUT, OUTPUT>(parameters[0]);
            if ( name.compare("InvariantHuMoment7Feature2") == 0 )
                return new InvariantHuMoment7Feature2<INPUT, OUTPUT>(parameters[0]);
            if ( name.compare("EccentricityFeature2") == 0 )
                return new EccentricityFeature2<INPUT, OUTPUT>(parameters[0]);
            if ( name.compare("OrientationFeature2") == 0 )
                return new OrientationFeature2<INPUT, OUTPUT>(parameters[0]);
            if ( name.compare("CentralMomentFeature2") == 0 )
                return new CentralMomentFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3]);
            if ( name.compare("MaxFeature2") == 0 )
                return new MaxFeature2<INPUT, OUTPUT>(parameters[0]);
            if ( name.compare("MinFeature2") == 0 )
                return new MinFeature2<INPUT, OUTPUT>(parameters[0]);
            if ( name.compare("MedianFeature2") == 0 )
                return new MedianFeature2<INPUT, OUTPUT>(parameters[0]);
            if ( name.compare("MeanFeature2") == 0 )
                return new MeanFeature2<INPUT, OUTPUT>(parameters[0]);
            if ( name.compare("VarianceFeature2") == 0 )
                return new VarianceFeature2<INPUT, OUTPUT>(parameters[0]);
            if ( name.compare("StandardDeviationFeature2") == 0 )
                return new StandardDeviationFeature2<INPUT, OUTPUT>(parameters[0]);
            if ( name.compare("DifferenceOfGaussianDyyFeature2") == 0 )
                return new DifferenceOfGaussianDyyFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);
            if ( name.compare("DifferenceOfGaussianDxxFeature2") == 0 )
                return new DifferenceOfGaussianDxxFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);
            if ( name.compare("DifferenceOfGaussianDxyFeature2") == 0 )
                return new DifferenceOfGaussianDxyFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);
            if ( name.compare("DifferenceOfGaussianDyFeature2") == 0 )
                return new DifferenceOfGaussianDyFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);
            if ( name.compare("DifferenceOfGaussianDxFeature2") == 0 )
                return new DifferenceOfGaussianDxFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);
            if ( name.compare("DifferenceOfGaussianFeature2") == 0 )
                return new DifferenceOfGaussianFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);
            if ( name.compare("DifferenceOfMinFeature2") == 0 )
                return new DifferenceOfMinFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
            if ( name.compare("DifferenceOfMaxFeature2") == 0 )
                return new DifferenceOfMaxFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
            if ( name.compare("DifferenceOfMeanFeature2") == 0 )
                return new DifferenceOfMeanFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
            if ( name.compare("DifferenceOfVarianceFeature2") == 0 )
                return new DifferenceOfVarianceFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
            if ( name.compare("DifferenceOfStandardDeviationFeature2") == 0 )
                return new DifferenceOfStandardDeviationFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
            if ( name.compare("DifferenceOfCentralMomentFeature2") == 0 )
                return new DifferenceOfCentralMomentFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
            if ( name.compare("DifferenceOfInvariantHuMoment1Feature2") == 0 )
                return new DifferenceOfInvariantHuMoment1Feature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
            if ( name.compare("DifferenceOfInvariantHuMoment2Feature2") == 0 )
                return new DifferenceOfInvariantHuMoment2Feature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
            if ( name.compare("DifferenceOfInvariantHuMoment3Feature2") == 0 )
                return new DifferenceOfInvariantHuMoment3Feature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
            if ( name.compare("DifferenceOfInvariantHuMoment4Feature2") == 0 )
                return new DifferenceOfInvariantHuMoment4Feature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
            if ( name.compare("DifferenceOfInvariantHuMoment5Feature2") == 0 )
                return new DifferenceOfInvariantHuMoment5Feature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
            if ( name.compare("DifferenceOfInvariantHuMoment6Feature2") == 0 )
                return new DifferenceOfInvariantHuMoment6Feature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
            if ( name.compare("DifferenceOfInvariantHuMoment7Feature2") == 0 )
                return new DifferenceOfInvariantHuMoment7Feature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
            if ( name.compare("DifferenceOfEccentricityFeature2") == 0 )
                return new DifferenceOfEccentricityFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
            if ( name.compare("DifferenceOfOrientationFeature2") == 0 )
                return new DifferenceOfOrientationFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
            if ( name.compare("DifferenceOfSNRFeature2") == 0 )
                return new DifferenceOfSNRFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);

            if ( name.compare("RatioOfGaussianDyyFeature2") == 0 )
                return new RatioOfGaussianDyyFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);
            if ( name.compare("RatioOfGaussianDxxFeature2") == 0 )
                return new RatioOfGaussianDxxFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);
            if ( name.compare("RatioOfGaussianDxyFeature2") == 0 )
                return new RatioOfGaussianDxyFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);
            if ( name.compare("RatioOfGaussianDyFeature2") == 0 )
                return new RatioOfGaussianDyFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);
            if ( name.compare("RatioOfGaussianDxFeature2") == 0 )
                return new RatioOfGaussianDxFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);
            if ( name.compare("RatioOfGaussianFeature2") == 0 )
                return new RatioOfGaussianFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);
            if ( name.compare("RatioOfMinFeature2") == 0 )
                return new RatioOfMinFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
            if ( name.compare("RatioOfMaxFeature2") == 0 )
                return new RatioOfMaxFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
            if ( name.compare("RatioOfMeanFeature2") == 0 )
                return new RatioOfMeanFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
            if ( name.compare("RatioOfVarianceFeature2") == 0 )
                return new RatioOfVarianceFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
            if ( name.compare("RatioOfStandardDeviationFeature2") == 0 )
                return new RatioOfStandardDeviationFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
            if ( name.compare("RatioOfCentralMomentFeature2") == 0 )
                return new RatioOfCentralMomentFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
            if ( name.compare("RatioOfInvariantHuMoment1Feature2") == 0 )
                return new RatioOfInvariantHuMoment1Feature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
            if ( name.compare("RatioOfInvariantHuMoment2Feature2") == 0 )
                return new RatioOfInvariantHuMoment2Feature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
            if ( name.compare("RatioOfInvariantHuMoment3Feature2") == 0 )
                return new RatioOfInvariantHuMoment3Feature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
            if ( name.compare("RatioOfInvariantHuMoment4Feature2") == 0 )
                return new RatioOfInvariantHuMoment4Feature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
            if ( name.compare("RatioOfInvariantHuMoment5Feature2") == 0 )
                return new RatioOfInvariantHuMoment5Feature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
            if ( name.compare("RatioOfInvariantHuMoment6Feature2") == 0 )
                return new RatioOfInvariantHuMoment6Feature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
            if ( name.compare("RatioOfInvariantHuMoment7Feature2") == 0 )
                return new RatioOfInvariantHuMoment7Feature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
            if ( name.compare("RatioOfEccentricityFeature2") == 0 )
                return new RatioOfEccentricityFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
            if ( name.compare("RatioOfOrientationFeature2") == 0 )
                return new RatioOfOrientationFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);
            if ( name.compare("RatioOfSNRFeature2") == 0 )
                return new RatioOfSNRFeature2<INPUT, OUTPUT>(parameters[0], parameters[1], parameters[2]);*/
            if ( name.compare("IdenticalFilter2") == 0 )
                return new IdenticalFilter2<INPUT, OUTPUT>(parameters[0]);
        }

        return NULL;
    }
}

#endif	/* FEATURE2GENERATOR_H */

