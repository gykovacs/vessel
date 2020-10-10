/**
 * @file AODEClassifier.h
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
 * AODEClassifier is an implementation of the Averaged One-Dependence Estimator Classifier.
 */

#ifndef AODEClassifier_H
#define	AODEClassifier_H

#include <iostream>
#include <fstream>

#include <openipDS/Vector.h>
#include <openipDS/Volume.h>
#include <openipDS/DataSet.h>

#include <openipML/Classifier.h>
#include <openipML/DiscreteClassifier.h>
#include <openipML/EqualWidthDiscretizer.h>
#include <openipML/EqualFrequencyDiscretizer.h>
#include <openipML/KMeansClusteringDiscretizer.h>
#include <openipML/EntropyBasedDiscretizer.h>
#include <openipML/PrincipalComponentAnalysis.h>

namespace openip
{
    /**
     * AODEClassifier is an implementation of the Averaged One-Dependence Estimator Classifier
     */
    template<typename T>
    class AODEClassifier: public DiscreteClassifier<T>
    {
    public:
        using DiscreteClassifier<T>::probabilities;
        using DiscreteClassifier<T>::classNames;
        using DiscreteClassifier<T>::featureNames;
        using DiscreteClassifier<T>::transformAttributesToOneRange;
        using DiscreteClassifier<T>::saveModelToFile;
        using DiscreteClassifier<T>::openModelFromFile;
        using DiscreteClassifier<T>::discretizer;
        using DiscreteClassifier<T>::data;

        /**
         * constructor
         * @param limit only those categories are considered, which have larger number of samples than the limit
         * @param d discretizer object
         */
        AODEClassifier(int limit= 1, Discretizer<T>* d= new KMeansClusteringDiscretizer<T>(20), int pca= 0);

        /**
         * copy constructor
         * @param c instance to copy
         */
        AODEClassifier(const AODEClassifier& c);

        /**
         * destructor
         */
        ~AODEClassifier();

        /**
         * probability estimation
         * @param xi sample coordinate
         * @param y class index
         * @param a attribute index
         * @return the estimated probability
         */
        float mEstimatePxiy(int xi, int y, int a);

        /**
         * probability estimation
         * @param xi sample coordinate
         * @param xj sample coordinate
         * @param y class index
         * @return the estimated probability
         */
        float mEstimatePxixjy(int xi, int xj, int y);

        /**
         * training method of the classifier
         * @param data dataset
         * @param mask only those vectors are trained, whose corresponding mask element is non-zero
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int train(DataSet<T>& data, Vector<unsigned char>* mask= NULL);

        /**
         * classifier method
         * @param c vector to classify
         * @param probabilities vector for probability estimates
         * @return predicted class index
         */
        virtual int classify(Vector<T>& c, Vector<float>* probabilities= NULL);

        /**
         * frequency estimation
         * @param data dataset
         * @param mask only those vectors are trained, whose corresponding mask element is non-zero
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int estimateFrequencies(DataSet<int>& data, Vector<unsigned char>* mask= NULL);

        /**
         * saves the model to file
         * @param file filename
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int saveModelToFile(std::ofstream& file);

        /**
         * opens the model from file
         * @param file filename
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int openModelFromFile(std::ifstream& file);

        /** frequency table */
        Volume<float> freq;
        /** class frequency table */
        Vector<float> cfreq;
        /** attribute frequency table */
        Vector<float> afreq;
        /** v frequency table */
        Vector<float> vfreq;

        /** limit */
        int limit;

        int pca;
    };

    template<typename T>
    AODEClassifier<T>::AODEClassifier(int limit, Discretizer<T>* d, int pca)
    : DiscreteClassifier<T>(28)
    {
        this->limit= limit;
        this->discretizer= d;
        this->pca= pca;
    }

    template<typename T>
    AODEClassifier<T>::AODEClassifier(const AODEClassifier& a)
    : DiscreteClassifier<T>(a)
    {
        this->limit= a.limit;
        this->discretizer= a.discretizer;
        this->pca= a.pca;
    }

    template<typename T>
    AODEClassifier<T>::~AODEClassifier()
    {
    }

    template<typename T>
    int AODEClassifier<T>::train(DataSet<T>& d, Vector<unsigned char>* mask)
    {
        this->classNames= d.classNames;
        this->featureNames= d.featureNames;
        this->preprocessingNames= d.preprocessingNames;
        d.init();
        //printf("discretize dataset\n"); fflush(stdout);
        discretizer->discretize(d, data);
        data.init();
        transformAttributesToOneRange();
        probabilities.resize(data.numberOfClasses);
        //printf("estimate frequencies\n"); fflush(stdout);
        estimateFrequencies(data, mask);

        return 0;
    }

    template<typename T>
    int AODEClassifier<T>::estimateFrequencies(DataSet<int>& d, Vector<unsigned char>* mask)
    {
        freq.resizeVolume(d.numberOfClasses, d.transformedAttributeRange, d.transformedAttributeRange);
        cfreq.resize(d.numberOfClasses);
        afreq.resize(d.dimension);

        vfreq.resize(d.transformedAttributeRange);

        cfreq= 0;
        afreq= 0;
        vfreq= 0;
        freq= 0;

        int count= 0;

        for ( unsigned int i= 0; i < d.size(); ++i )
        {
            if ( !mask || (*mask)(i) )
            {
                ++count;
                cfreq(d(i).classIndex)= cfreq(d(i).classIndex)+1;
                for ( int k= 0; k < d.dimension; ++k )
                {
                    afreq(k)= afreq(k)+1;
                    vfreq(d(i)(k))= vfreq(d(i)(k)) + 1;
                    for ( int l= 0; l < d.dimension; ++l )
                    {
                        freq(d(i).classIndex, d(i)(k), d(i)(l))= freq(d(i).classIndex, d(i)(k), d(i)(l)) + 1;
                    }
                }
            }
        }

        return 0;
    }

    template<typename T>
    int AODEClassifier<T>::classify(Vector<T>& c, Vector<float>* probabilities)
    {
        Vector<int> tmp;
        tmp.resize(c.size());

        if ( probabilities == NULL )
            probabilities= &(this->probabilities);

        *probabilities= 0;

        discretizer->discretize(c, tmp);

        transformAttributesToOneRange(tmp);

        float sum= 0;
        int attcount;
        float p;

        for ( unsigned int y= 0; y < cfreq.size(); ++y )
        {
            attcount= 0;

            for ( unsigned int i= 0; i < tmp.size(); ++i )
            {
                if ( vfreq(tmp(i)) >= limit )
                {
                    ++attcount;
                    p= mEstimatePxiy(tmp(i), y, i);

                    for ( unsigned int j= 0; j < tmp.size(); ++j )
                    {
                        p= p * mEstimatePxixjy(tmp(i), tmp(j), y);
                    }
                    (*probabilities)(y)= (*probabilities)(y) + p;
                }
            }

            (*probabilities)(y)/=cfreq(y);
            if ( attcount > 0 )
                (*probabilities)(y)= (*probabilities)(y) / attcount;
            sum+= (*probabilities)(y);
        }

        for ( unsigned int i= 0; i < probabilities->size(); ++i )
            (*probabilities)(i)/= sum;

        int maxIdx= 0;
        for ( unsigned int i= 1; i < cfreq.size(); ++i )
            if ( (*probabilities)(i) > (*probabilities)(maxIdx) )
                maxIdx= i;

        return maxIdx;
    }

    template<typename T>
    float AODEClassifier<T>::mEstimatePxiy(int xi, int y, int a)
    {
        return (freq(y, xi, xi) + 0.5) / ( afreq(a) + 0.5 * vfreq(xi) * cfreq(y) );
    }

    template<typename T>
    float AODEClassifier<T>::mEstimatePxixjy(int xi, int xj, int y)
    {
        return (freq(y, xi, xj) + 0.5) / ((float)(freq(y, xj, xj)) + 0.5 * vfreq(xi));
    }

    template<typename T>
    int AODEClassifier<T>::saveModelToFile(std::ofstream& file)
    {
        file << "AODEClassifier" << std::endl;

        this->DiscreteClassifier<T>::saveModelToFile(file);

        file << limit << std::endl;
        file << freq << std::endl;
        file << cfreq << std::endl;
        file << afreq << std::endl;
        file << vfreq << std::endl;

        return 0;
    }

    template<typename T>
    int AODEClassifier<T>::openModelFromFile(std::ifstream& file)
    {
        std::string tmp;
        
        getline(file, tmp);
        
        if ( tmp.compare("AODEClassifier") == 0 )
        {
            this->DiscreteClassifier<T>::openModelFromFile(file);
            
            file >> limit;
            file >> freq;
            file >> cfreq;
            file >> afreq;
            file >> vfreq;

            probabilities.resize(cfreq.size());

            return 0;
        }

        return 1;
    }
}

#endif	/* AODEClassifier_H */

