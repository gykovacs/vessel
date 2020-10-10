/* 
 * File:   RegionTransform2Generator.h
 * Author: gykovacs
 *
 * Created on September 30, 2011, 4:50 PM
 */

#ifndef REGIONTRANSFORM2GENERATOR_H
#define	REGIONTRANSFORM2GENERATOR_H

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
#include <openipDS/RegionFeature2s.h>

#include <openipLL/FourierDifferenceFilter2.h>
#include <openipLL/Transform2Chain.h>
#include <openipLL/imageCorrection.h>
#include <openipLL/LowLevelTransforms.h>
#include <openipLL/Scaling.h>
#include <openipLL/Transform2Generator.h>

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
        template<typename INPUT>
        RegionFeature2<INPUT>* generateRegionFeature2(std::string s)
        {
            std::string name;
            vector<float> parameters;
            VectorTransform2<INPUT, INPUT>* vt= NULL;
            vector<std::string> descriptors;

            int c= s.find(std::string("transform"));
            
            if ( c == -1 )
                descriptors.push_back(s);
            else
            {
                std::string transformDescriptor= s.substr(c + 10);
                tprintf("generate support transform...\n");
                vt= generateTransform2<INPUT, INPUT>(transformDescriptor);
                descriptors.push_back(s.substr(0, c-1));
            }
            
            RegionFeature2<INPUT>* result= NULL;

            for ( unsigned int i= 0; i < descriptors.size(); ++i )
            {
            if ( ! getTransformNameAndParameters(descriptors[i], name, parameters) )
            {
                //tprintf("generating: %s\n", descriptors[i].c_str()); fflush(stdout);
                //printf("%p,%p\n", vt, result);

                if ( name.compare("RegionAreaFeature2") == 0 )
                    result= new RegionAreaFeature2<INPUT>();
                else if ( name.compare("RegionPerimeterFeature2") == 0 )
                    result= new RegionPerimeterFeature2<INPUT>();
                else if ( name.compare("RegionMeanFeature2") == 0 )
                    result= new RegionMeanFeature2<INPUT>();
                else if ( name.compare("RegionCircularityFeature2") == 0 )
                    result= new RegionCircularityFeature2<INPUT>();
                else if ( name.compare("RegionAreaToImageFeature2") == 0 )
                    result= new RegionAreaToImageFeature2<INPUT>();
                else if ( name.compare("RegionRelativeMeanFeature2") == 0 )
                    result= new RegionRelativeMeanFeature2<INPUT>();
                else if ( name.compare("RegionMaximumFeature2") == 0 )
                    result= new RegionMaximumFeature2<INPUT>();
                else if ( name.compare("RegionStandardDeviationFeature2") == 0 )
                    result= new RegionStandardDeviationFeature2<INPUT>();
                else if ( name.compare("RegionSNRFeature2") == 0 )
                    result= new RegionSNRFeature2<INPUT>();
                else if ( name.compare("RegionEntropyFeature2") == 0 )
                    result= new RegionEntropyFeature2<INPUT>(parameters[0]);
                else if ( name.compare("RegionLineMagnitudeFeature2") == 0 )
                    result= new RegionLineMagnitudeFeature2<INPUT>();
                else if ( name.compare("RegionMaxLineMagnitudeFeature2") == 0 )
                    result= new RegionMaxLineMagnitudeFeature2<INPUT>();
                else if ( name.compare("RegionEnergyFeature2") == 0 )
                    result= new RegionEnergyFeature2<INPUT>();
                else if ( name.compare("RegionGlobalRelativeMeanFeature2") == 0 )
                    result= new RegionGlobalRelativeMeanFeature2<INPUT>();
                else if ( name.compare("RegionTotalVariationFeature2") == 0 )
                    result= new RegionTotalVariationFeature2<INPUT>();
                else if ( name.compare("RegionThresholdFeature2") == 0 )
                    result= new RegionThresholdFeature2<INPUT>(parameters[0]);
                else if ( name.compare("RegionCenterFeature2") == 0 )
                    result= new RegionCenterFeature2<INPUT>();
                else if ( name.compare("RegionPerimeterMeanDifferenceFeature2") == 0 )
                    result= new RegionPerimeterMeanDifferenceFeature2<INPUT>(parameters[0]);
                else if ( name.compare("RegionPerimeterMinDifferenceFeature2") == 0 )
                    result= new RegionPerimeterMinDifferenceFeature2<INPUT>(parameters[0]);
                else if ( name.compare("RegionPerimeterMaxDifferenceFeature2") == 0 )
                    result= new RegionPerimeterMaxDifferenceFeature2<INPUT>(parameters[0]);
                else if ( name.compare("RegionPerimeterVarianceDifferenceFeature2") == 0 )
                    result= new RegionPerimeterVarianceDifferenceFeature2<INPUT>(parameters[0]);
                else if ( name.compare("RegionPerimeterStandardDeviationDifferenceFeature2") == 0 )
                    result= new RegionPerimeterStandardDeviationDifferenceFeature2<INPUT>(parameters[0]);
                else if ( name.compare("RegionPerimeterSkewnessDifferenceFeature2") == 0 )
                    result= new RegionPerimeterSkewnessDifferenceFeature2<INPUT>(parameters[0]);
                else if ( name.compare("RegionPerimeterKurtosisDifferenceFeature2") == 0 )
                    result= new RegionPerimeterKurtosisDifferenceFeature2<INPUT>(parameters[0]);
                else if ( name.compare("RegionPerimeterRangeDifferenceFeature2") == 0 )
                    result= new RegionPerimeterRangeDifferenceFeature2<INPUT>(parameters[0]);
                else if ( name.compare("RegionRangeFeature2") == 0 )
                    result= new RegionRangeFeature2<INPUT>();
                else if ( name.compare("RegionHistogramFeature2") == 0 )
                    result= new RegionHistogramFeature2<INPUT>(parameters[0]);
                else if ( name.compare("RegionNumberOfRegionsThreshold") == 0 )
                    result= new RegionNumberOfRegionsThreshold<INPUT>(parameters[0]);
                else if ( name.compare("RegionAverageSizeOfRegionsThreshold") == 0 )
                    result= new RegionAverageSizeOfRegionsThreshold<INPUT>(parameters[0]);
                else if ( name.compare("RegionOtsuNumberOfRegions") == 0 )
                    result= new RegionOtsuNumberOfRegions<INPUT>();
                else if ( name.compare("RegionOtsuAverageSizeOfRegions") == 0 )
                    result= new RegionOtsuAverageSizeOfRegions<INPUT>();
                else if ( name.compare("RegionOtsuAveragePerimeterOfRegions") == 0 )
                    result= new RegionOtsuAveragePerimeterOfRegions<INPUT>();
                else if ( name.compare("RegionOtsuSizeHistogramOfRegions") == 0 )
                    result= new RegionOtsuSizeHistogramOfRegions<INPUT>(parameters[0]);
                else if ( name.compare("RegionOtsuPerimeterHistogramOfRegions") == 0 )
                    result= new RegionOtsuPerimeterHistogramOfRegions<INPUT>(parameters[0]);
                else if ( name.compare("RegionGeneralThresholdNumberOfRegions") == 0 )
                    result= new RegionGeneralThresholdNumberOfRegions<INPUT>(parameters[0]);
                else if ( name.compare("RegionGeneralThresholdAverageSizeOfRegions") == 0 )
                    result= new RegionGeneralThresholdAverageSizeOfRegions<INPUT>(parameters[0]);
                else if ( name.compare("RegionGeneralThresholdAveragePerimeterOfRegions") == 0 )
                    result= new RegionGeneralThresholdAveragePerimeterOfRegions<INPUT>(parameters[0]);
                else if ( name.compare("RegionGeneralThresholdSizeHistogramOfRegions") == 0 )
                    result= new RegionGeneralThresholdSizeHistogramOfRegions<INPUT>(parameters[0], parameters[1]);
                else if ( name.compare("RegionGeneralThresholdPerimeterHistogramOfRegions") == 0 )
                    result= new RegionGeneralThresholdPerimeterHistogramOfRegions<INPUT>(parameters[0], parameters[1]);
                else if ( name.compare("RegionCooccuranceDiagonalFeature2") == 0 )
                    result= new RegionCooccuranceDiagonalFeature2<INPUT>(parameters[0]);
                else if ( name.compare("RegionCooccuranceFeature2") == 0 )
                    result= new RegionCooccuranceFeature2<INPUT>(parameters[0]);
                else if ( name.compare("RegionCooccuranceALBPFeature2") == 0 )
                    result= new RegionCooccuranceALBPFeature2<INPUT>(parameters[0], parameters[1], parameters[2]);
                else if ( name.compare("RegionCooccuranceARadialLBPFeature2") == 0 )
                    result= new RegionCooccuranceARadialLBPFeature2<INPUT>(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4]);
                else if ( name.compare("RegionCooccuranceRALBPFeature2") == 0 )
                    result= new RegionCooccuranceALBPFeature2<INPUT>(parameters[0], parameters[1], parameters[2]);
                else if ( name.compare("RegionCooccuranceThresholdFeature2") == 0 )
                    result= new RegionCooccuranceThresholdFeature2<INPUT>(parameters[0], parameters[1]);
                else if ( name.compare("RegionShellsMeanFeature2") == 0 )
                    result= new RegionShellsMeanFeature2<INPUT>(parameters[0], parameters[1]);
                else if ( name.compare("RegionShellsVarianceFeature2") == 0 )
                    result= new RegionShellsVarianceFeature2<INPUT>(parameters[0], parameters[1]);
                else if ( name.compare("RegionShellsStandardDeviationFeature2") == 0 )
                    result= new RegionShellsStandardDeviationFeature2<INPUT>(parameters[0], parameters[1]);
                else if ( name.compare("RegionGeneralThresholdStdDevSizeOfRegions") == 0 )
                    result= new RegionGeneralThresholdStdDevSizeOfRegions<INPUT>(parameters[0]);
                else if ( name.compare("RegionGeneralThresholdStdDevPerimeterOfRegions") == 0 )
                    result= new RegionGeneralThresholdStdDevPerimeterOfRegions<INPUT>(parameters[0]);
                else if ( name.compare("RegionGeneralThresholdNumberOfRegionsSizeInRange") == 0 )
                    result= new RegionGeneralThresholdNumberOfRegionsSizeInRange<INPUT>(parameters[0], parameters[1], parameters[2]);
                else if ( name.compare("RegionGeneralThresholdAverageCircularity") == 0 )
                    result= new RegionGeneralThresholdAverageCircularity<INPUT>(parameters[0]);
                else if ( name.compare("RegionHistogramOfMaximaGaussianResponse") == 0 )
                    result= new RegionHistogramOfMaximaGaussianResponse<INPUT>(parameters[0]);

                //tprintf("tr: %s\n", name.c_str());
                //tprintf("%p,%p\n", vt, result);
                if ( vt != NULL || (result && result->requiresTransform()) )
                    result->transform= vt;
            }
            }
            //printf("%p,%p\n", vt, result);
            return result;
        }

        template<typename INPUT>
        RegionFeature2Set<INPUT>* generateRegionTransform2Set(std::string fdf, std::string keyword)
        {
            
            RegionFeature2Set<INPUT>* result= new RegionFeature2Set<INPUT>();
            RegionFeature2<INPUT>* tmp;

            FeatureDescriptorParameterFile fdpf(fdf);

            while ( fdpf.next(keyword) )
            {
                tmp= generateRegionFeature2<INPUT>(fdpf.tokenString);
                if ( tmp )
                    result->push_back(tmp);
            }

            return result;
        }
        
        template<typename INPUT>
        RegionFeature2Set<INPUT>* generateRegionTransform2Set(std::string fdf, std::string keyword, std::string fdf2, std::string keyword2)
        {
            Transform2Set<INPUT, INPUT>* t2s= generateTransform2Set<INPUT, INPUT>(fdf2, keyword2);
            
            RegionFeature2Set<INPUT>* result= new RegionFeature2Set<INPUT>();
            RegionFeature2<INPUT>* tmp;

            FeatureDescriptorParameterFile fdpf(fdf);

            while ( fdpf.next(keyword) )
            {
                if ( t2s->size() == 0 )
                {
                    tmp= generateRegionFeature2<INPUT>(fdpf.tokenString);
                    if ( tmp )
                        result->push_back(tmp);
                }
                else
                    for ( unsigned int i= 0; i < t2s->size(); ++i )
                    {
                            tmp= generateRegionFeature2<INPUT>(fdpf.tokenString);
                            
                            if ( tmp->requiresTransform() == true && tmp )
                                tmp->transform= (*t2s)(i);
                            if ( tmp )
                                result->push_back(tmp);
                    }
            }

            return result;
        }

        template<typename INPUT>
        RegionFeature2Set<INPUT>* generateRegionTransform2Set(Vector<std::string>& descriptors)
        {
            RegionFeature2Set<INPUT>* tmp= new RegionFeature2Set<INPUT>();
            RegionFeature2<INPUT>* t;
            for ( unsigned int i= 0; i < descriptors.size(); ++i )
            {
                t= generateRegionFeature2<INPUT>(descriptors[i]);
                if ( t )
                    tmp->push_back(t);
            }

            return tmp;
        }
}

#endif	/* REGIONTRANSFORM2GENERATOR_H */

