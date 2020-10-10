/**
 * @file DiscreteMultivariateBayesClassifier.h
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
 * DiscreteMultivariateBayesClassifier represents the discrete multivariate Bayes
 * classification method.
 */

#ifndef DISCRETEMULTIVARIATEBAYESCLASSIFIER_H
#define	DISCRETEMULTIVARIATEBAYESCLASSIFIER_H

#include <openipML/Classifier.h>
#include <openipML/Discretizer.h>
#include <openipML/DiscreteClassifier.h>
#include <openipML/EqualWidthDiscretizer.h>
#include <openipML/EntropyBasedDiscretizer.h>

#include <openipDS/NDimensionalContainer.h>

namespace openip
{
    /**
     * DiscreteMultivariateBayesClassifier represents the discrete multivariate
     * Bayes classification method.
     */

    template<typename T>
    class DiscreteMultivariateBayesClassifier: public DiscreteClassifier<T>
    {
    public:
        using DiscreteClassifier<T>::probabilities;
        using DiscreteClassifier<T>::classNames;
        using DiscreteClassifier<T>::featureNames;
        using DiscreteClassifier<T>::transformAttributesToOneRange;
        using DiscreteClassifier<T>::saveModelToFile;
        using DiscreteClassifier<T>::openModelFromFile;
        using DiscreteClassifier<T>::cumulativeRange;
        using DiscreteClassifier<T>::transformedAttributeRange;
        using DiscreteClassifier<T>::data;
        using DiscreteClassifier<T>::discretizer;

        /**
         * constructor
         * @param smoothing smoothing of historgrams
         * @param d discretizer object
         */
        DiscreteMultivariateBayesClassifier(Discretizer<T>* d= new EqualWidthDiscretizer<T>(2), int pca= 0);

        /**
         * copy constructor
         * @param c instance to copy
         */
        DiscreteMultivariateBayesClassifier(const DiscreteMultivariateBayesClassifier& c);

        /**
         * destructor
         */
        ~DiscreteMultivariateBayesClassifier();

        /**
         * training of the classifier
         * @param data training dataset
         * @param mask only those elements are trained, which have non-zero corresponding element in the mask
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int train(DataSet<T>& data, Vector<unsigned char>* mask= NULL);

        /**
         * classifies the parameter vector
         * @param v vector to classify
         * @param probabilities probability estimates
         * @return predicted class index
         */
        virtual int classify(Vector<T>& v, Vector<float>* probabilities= NULL);

        /**
         * estimates histograms
         * @param data training dataset
         * @param mask only those elements are trained, which have non-zero corresponding element in the mask
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int estimateFrequencies(DataSet<int>& data, Vector<unsigned char>* mask= NULL);

        /**
         * saves the model to output file stream
         * @param file output file stream
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int saveModelToFile(std::ofstream& file);

        /**
         * opens the model from input file stream
         * @param file input file stream
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int openModelFromFile(std::ifstream& file);

        /** frequencies */
        Matrix<float> frequencies;
        /** class frequencies */
        Vector<float> classFrequencies;
        /** smoothing of the histogram */
        float smoothing;

        int pca;
        PrincipalComponentAnalysis<T> principalComponentAnalysis;
    };

    template<typename T>
    DiscreteMultivariateBayesClassifier<T>::DiscreteMultivariateBayesClassifier(Discretizer<T>* d, int pca)
    : DiscreteClassifier<T>(28)
    {
        this->smoothing= smoothing;
        this->discretizer= d;
        this->pca= pca;
    }

    template<typename T>
    DiscreteMultivariateBayesClassifier<T>::DiscreteMultivariateBayesClassifier(const DiscreteMultivariateBayesClassifier& c)
    : DiscreteClassifier<T>(c)
    {
        this->smoothing= c.smoothing;
        this->discretizer= c.discretizer;
        this->frequencies= c.frequencies;
        this->classFrequencies= c.classFrequencies;
        this->pca= c.pca;
    }

    template<typename T>
    DiscreteMultivariateBayesClassifier<T>::~DiscreteMultivariateBayesClassifier()
    {
    }

    template<typename T>
    int DiscreteMultivariateBayesClassifier<T>::train(DataSet<T>& cdata, Vector<unsigned char>* mask)
    {
        this->classNames= cdata.classNames;
        this->featureNames= cdata.featureNames;
        this->preprocessingNames= cdata.preprocessingNames;

        DataSet<T> cdata2;

        if ( pca )
        {
            printf("applying PCA\n");
            principalComponentAnalysis.train(cdata, pca);
            principalComponentAnalysis.transform(cdata, cdata2);
            discretizer->discretize(cdata2, data);
        }
        else
        {

        //printf("discretize dataset\n"); fflush(stdout);
        discretizer->discretize(cdata, data);
        }

        //printf("data.init\n"); fflush(stdout);
        data.init();
        probabilities.resize(data.numberOfClasses);
        //printf("transform attributes\n"); fflush(stdout);
        transformAttributesToOneRange();
        //printf("estimate frequencies\n"); fflush(stdout);
        estimateFrequencies(data, mask);

        return 0;
    }

    template<typename T>
    int DiscreteMultivariateBayesClassifier<T>::estimateFrequencies(DataSet<int>& d, Vector<unsigned char>* mask)
    {
        classFrequencies.resize(d.numberOfClasses);
        frequencies.resize(d.numberOfClasses, d.transformedAttributeRange);

        classFrequencies= 0;
        frequencies= 0;

        float numberOfRealSamples= d.size();

        if ( !mask )
        {
            for ( typename DataSet<int>::iterator it= d.begin(); it != d.end(); ++it )
            {
                classFrequencies(it->classIndex)+= 1;

                for ( int i= 0; i < d.dimension; ++i )
                    frequencies(it->classIndex, (*it)(i))+= 1;
            }
        }
        else
        {
            numberOfRealSamples= 0;
            for ( unsigned int i= 0; i < d.size(); ++i )
            {
                if ( (*mask)(i) )
                {
                    //cout << d(i)(0) << " ";
                    classFrequencies(d(i).classIndex)+= 1;

                    for ( int j= 0; j < d.dimension; ++j )
                        frequencies(d(i).classIndex, d(i)(j))+= 1;

                    numberOfRealSamples= numberOfRealSamples + 1.0;
                }
            }
        }

        int v= 0;

        /*for ( int i= 0; i < d.numberOfClasses; ++i )
            printf("%f ", classFrequencies(i));
        printf("\n");

        for ( int i= 0; i < d.numberOfClasses; ++i )
        {
            v= 0;
            for ( int j= 0; j < d.dimension; ++j )
                for ( int k= 0; k < d.range(j) + 1; ++k )
                {
                    printf("%.2f ", frequencies(i,v));
                    ++v;
                }
            printf("\n");
        }*/

        for ( int i= 0; i < d.numberOfClasses; ++i )
        {
            v= 0;
            for ( int j= 0; j < d.dimension; ++j )
                for ( int k= 0; k < d.range(j) + 1; ++k )
                {
                    frequencies(i,v)= (frequencies(i,v) + smoothing)/(float)((classFrequencies(i) + smoothing * d.range(j)));
                    ++v;
                }
        }



        for ( unsigned int i= 0; i < classFrequencies.size(); ++i )
            classFrequencies(i)= classFrequencies(i)/numberOfRealSamples;

        return 0;
    }

    template<typename T>
    int DiscreteMultivariateBayesClassifier<T>::classify(Vector<T>& v, Vector<float>* probabilities)
    {
        Vector<int> c;

        if ( pca )
        {
            Vector<T> v2= principalComponentAnalysis.transform(v);
            c.resize(v2.size());
            discretizer->discretize(v2, c);
        }
        else
        {
            c.resize(v.size());
            discretizer->discretize(v, c);
        }

        if ( probabilities == NULL )
            probabilities= &(this->probabilities);

        float tmp;
        *probabilities= 0;

        transformAttributesToOneRange(c);

        float sum= 0;
        for ( unsigned int i= 0; i < classFrequencies.size(); ++i )
        {
            tmp= 1.0;
            //tmp*= classFrequencies(i);
            for ( unsigned int j= 0; j < c.size(); ++j )
            {
                tmp*= frequencies(i, c(j));
            }

            (*probabilities)(i)= tmp;
            sum+= tmp;
        }

        int maxC= 0;

        for ( unsigned int i= 0; i < probabilities->size(); ++i )
            (*probabilities)(i)/= sum;

        for ( unsigned int i= 1; i < probabilities->size(); ++i )
            if ( (*probabilities)(i) > (*probabilities)(maxC) )
                maxC= i;

        return maxC;
    }

    template<typename T>
    int DiscreteMultivariateBayesClassifier<T>::saveModelToFile(std::ofstream& file)
    {
        file << "DiscreteMultivariateBayesClassifier" << std::endl;

        this->DiscreteClassifier<T>::saveModelToFile(file);
        this->principalComponentAnalysis.saveToFile(file);

        file << pca << std::endl;
        file << smoothing << std::endl;
        file << frequencies;
        file << classFrequencies;

        return 0;
    }

    template<typename T>
    int DiscreteMultivariateBayesClassifier<T>::openModelFromFile(std::ifstream& file)
    {
        std::string tmp;

        getline(file, tmp);

        if ( tmp.find("DiscreteNaiveBayesClassifier") == 0 )
        {
            this->DiscreteClassifier<T>::openModelFromFile(file);
            this->principalComponentAnalysis.openFromFile(file);

            file >> pca;
            file >> smoothing;
            file >> frequencies;
            file >> classFrequencies;

            printf("%d\n", pca);

            probabilities.resize(classFrequencies.size());

            return 0;
        }

        return 1;
    }
    
}

#endif	/* DISCRETEMULTIVARIATEBAYESCLASSIFIER_H */

