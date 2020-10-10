/* 
 * File:   Transform3Generator.h
 * Author: gykovacs
 *
 * Created on July 21, 2011, 4:18 PM
 */

#ifndef TRANSFORM3GENERATOR_H
#define	TRANSFORM3GENERATOR_H

#include <openipDS/Volume.h>
#include <openipDS/Filter2s.h>
#include <openipDS/StatisticalFeature3s.h>
#include <openipDS/Transform3s.h>

#include <openipLL/Transform3Chain.h>
#include <openipLL/Transform2Generator.h>
#include <openipLL/volumeCorrection.h>
#include <openipLL/morphology3.h>
#include <openipDS/VectorTransform3s.h>

namespace openip
{
        /**
         * tokenizes the descriptor string
         * @param input descriptor string
         * @param name output parameter, name of the transform
         * @param parameters output parameter, parameters of the transform
         * @return non-zero in case of errors, zero otherwise
         */
        //int getTransformNameAndParameters(std::string& input, std::string& name, vector<float>& parameters);

        /**
         * generates Transform2 object from descriptor string
         * @param s transform descriptor string
         * @return generated Transform2 object
         */
        template<typename INPUT, typename OUTPUT>
        VectorTransform3<INPUT, OUTPUT>* generateTransform3(std::string s)
        {
            std::string name;
            vector<float> parameters;

            int c= s.find(std::string("chain"));
            int begin, end;

            vector<std::string> descriptors;

            if ( c == -1 )
                descriptors.push_back(s);

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

            VectorTransform3<INPUT, OUTPUT>* result= NULL;
            Transform3Chain<INPUT, OUTPUT>* t3chain= NULL;

            if ( descriptors.size() > 1 )
                t3chain= new Transform3Chain<INPUT, OUTPUT>();

            for ( unsigned int i= 0; i < descriptors.size(); ++i )
            {
            if ( ! getTransformNameAndParameters(descriptors[i], name, parameters) )
            {
                tprintf("generating: %s\n", descriptors[i].c_str()); fflush(stdout);

                if ( descriptors.size() > 1 )
                {
                    t3chain->tvoo.push_back(generateTransform3<OUTPUT, OUTPUT>(descriptors[i]));
                    continue;
                }

                if ( name.compare("IdenticalTransform3") == 0 )
                    result= new IdenticalTransform3<INPUT, OUTPUT>();
                if ( name.compare("NormalizationTransform3") == 0 )
                    result= new NormalizationTransform3<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("MaxFeature3") == 0 )
                    result= new MaxFeature3<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("MinFeature3") == 0 )
                    result= new MinFeature3<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("MeanFeature3") == 0 )
                    result= new MeanFeature3<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("VarianceFeature3") == 0 )
                    result= new VarianceFeature3<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("StandardDeviationFeature3") == 0 )
                    result= new StandardDeviationFeature3<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("NormalizationFeature3") == 0 )
                    result= new NormalizationFeature3<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("SNRFeature3") == 0 )
                    result= new SNRFeature3<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("HistogramEqualization3") == 0 )
                    result= new HistogramEqualization3<INPUT, OUTPUT>();
                if ( name.compare("InvariantHuMoment1Feature3") == 0 )
                    result= new InvariantHuMoment1Feature3<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("InvariantHuMoment2Feature3") == 0 )
                    result= new InvariantHuMoment2Feature3<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("InvariantHuMoment3Feature3") == 0 )
                    result= new InvariantHuMoment3Feature3<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("InvariantHuMoment4Feature3") == 0 )
                    result= new InvariantHuMoment4Feature3<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("InvariantHuMoment5Feature3") == 0 )
                    result= new InvariantHuMoment5Feature3<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("GrayscaleDilate3") == 0 )
                    result= new GrayscaleDilate3<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("GrayscaleErode3") == 0 )
                    result= new GrayscaleErode3<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("GrayscaleOpen3") == 0 )
                    result= new GrayscaleOpen3<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("GrayscaleClose3") == 0 )
                    result= new GrayscaleClose3<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("GrayscaleBrightReconstruction3") == 0 )
                    result= new GrayscaleBrightReconstruction3<INPUT, OUTPUT>(parameters[0]);
                if ( name.compare("GrayscaleDarkReconstruction3") == 0 )
                    result= new GrayscaleDarkReconstruction3<INPUT, OUTPUT>(parameters[0]);
		if ( name.compare("ClosestFeatureVectorTransform3") == 0 )
		    result= new ClosestFeatureVectorTransform3<INPUT, OUTPUT>(parameters[0], parameters[1]);
                
                tprintf("%s\n", name.c_str());
            }
            }

            if ( descriptors.size() > 1 )
                return t3chain;
            else
                return result;
        }

        template<typename INPUT, typename OUTPUT>
        Transform3Set<INPUT, OUTPUT>* generateTransform3Set(std::string fdf, std::string keyword)
        {
            Transform3Set<INPUT, OUTPUT>* result= new Transform3Set<INPUT, OUTPUT>();
            VectorTransform3<INPUT, OUTPUT>* tmp;

            FeatureDescriptorParameterFile fdpf(fdf);

            while ( fdpf.next(keyword) )
            {
                tmp= generateTransform3<INPUT, OUTPUT>(fdpf.tokenString);
                if ( tmp )
                    result->push_back(tmp);
            }

            return result;
        }

        template<typename INPUT, typename OUTPUT>
        Transform3Set<INPUT, OUTPUT>* generateTransform3Set(Vector<std::string>& descriptors)
        {
            Transform3Set<INPUT, OUTPUT>* tmp= new Transform3Set<INPUT, OUTPUT>();
            VectorTransform3<INPUT, OUTPUT>* t;
            for ( unsigned int i= 0; i < descriptors.size(); ++i )
            {
                t= generateTransform3<INPUT, OUTPUT>(descriptors[i]);
                if ( t )
                    tmp->push_back(t);
            }

            return tmp;
        }

        template<typename INPUT, typename OUTPUT>
        Transform3Chain<INPUT, OUTPUT>* generateTransform3Chain(std::string fdf, std::string keyword)
        {
            Transform3Chain<INPUT, OUTPUT>* result= new Transform3Chain<INPUT, OUTPUT>();
            VectorTransform3<OUTPUT, OUTPUT>* tmp;

            FeatureDescriptorParameterFile fdpf(fdf);

            while ( fdpf.next(keyword) )
            {
                tmp= /*t2g.*/generateTransform3<OUTPUT, OUTPUT>(fdpf.tokenString);
                if ( tmp )
                    result->tvoo.push_back(tmp);
            }

            return result;
        }

        template<typename INPUT, typename OUTPUT>
        Transform3Chain<INPUT, OUTPUT>* generateTransform3Chain(Vector<std::string>& descriptors)
        {
            Transform3Chain<INPUT, OUTPUT>* tmp= new Transform3Chain<INPUT, OUTPUT>();
            VectorTransform3<OUTPUT, OUTPUT>* t;
            for ( unsigned int i= 0; i < descriptors.size(); ++i )
            {
                t= generateTransform3<OUTPUT, OUTPUT>(descriptors[i]);
                if ( t )
                    tmp->tvoo.push_back(t);
            }

            return tmp;
        }
}


#endif	/* TRANSFORM3GENERATOR_H */

