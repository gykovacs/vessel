/**
 * @file ContinuousMultivariateBayesClassifier.h
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
 * ContinuousMultivariateBayesClassifier represents the continuous multivariate
 * Bayes classification method.
 */

#ifndef CONTINUOUSMULTIVARIATEBAYESCLASSIFIER_H
#define	CONTINUOUSMULTIVARIATEBAYESCLASSIFIER_H

#include <openipML/Classifier.h>
#include <openipML/GaussianDistribution.h>

#include "Classifier.h"

namespace openip
{
    /**
     * ContinuousMultivariateBayesClassifier represents the continuous multivariate
     * Bayes classification method.
     */
    template<typename T>
    class ContinuousMultivariateBayesClassifier: public Classifier<T>
    {
    public:
        using Classifier<T>::probabilities;
        using Classifier<T>::saveModelToFile;
        using Classifier<T>::openModelFromFile;

        /**
         * default constructor
         */
        ContinuousMultivariateBayesClassifier();

        /**
         * copy constructor
         * @param c instance to copy
         */
        ContinuousMultivariateBayesClassifier(const ContinuousMultivariateBayesClassifier& c);

        /**
         * destructor
         */
        ~ContinuousMultivariateBayesClassifier();

        /**
         * training of the classifier
         * @param data training dataset
         * @param mask only those elements are trained, which have non-zero correspondent value in the mask
         * @return non-zero in case of errors
         */
        virtual int train(DataSet<T>& data, Vector<unsigned char>* mask= NULL);

        /**
         * classifies the parameter vector
         * @param c vector to classify
         * @param probabilities probability estimates
         * @return class of the parameter vector
         */
        virtual int classify(Vector<T>& c, Vector<float>* probabilities= NULL);

        /** model of the parameter space */
        Vector<GaussianDistribution<T>* > model;
    };

    template<typename T>
    ContinuousMultivariateBayesClassifier<T>::ContinuousMultivariateBayesClassifier()
    {
    }

    template<typename T>
    ContinuousMultivariateBayesClassifier<T>::ContinuousMultivariateBayesClassifier(const ContinuousMultivariateBayesClassifier& c)
    {
    }

    template<typename T>
    ContinuousMultivariateBayesClassifier<T>::~ContinuousMultivariateBayesClassifier()
    {
        for ( int i= 0; i < model.size(); ++i )
            delete model(i);
    }

    template<typename T>
    int ContinuousMultivariateBayesClassifier<T>::train(DataSet<T>& data, Vector<unsigned char>* mask)
    {
        this->classNames= data.classNames;
        this->featureNames= data.featureNames;
        this->preprocessingNames= data.preprocessingNames;

        data.init();
        probabilities.resize(data.numberOfClasses);

        Vector<Vector<T> > means;
        Vector<Matrix<T> > covars;
        Vector<T> counts;
        Vector<T> tmp;

        for ( int i= 0; i < data.numberOfClasses; ++i )
        {
            means.push_back(Vector<T>(data.dimension));
            covars.push_back(Matrix<T>(data.dimension, data.dimension));
            counts.push_back(0);
            means(i)= 0;
            covars(i)= 0;
        }

        for ( unsigned int i= 0; i < data.size(); ++i )
        {
            if ( !mask || (*mask)(i) )
            {
                counts(data(i).classIndex)+= 1;
                means(data(i).classIndex)+= data(i);
            }
        }

        for ( int i= 0; i < data.numberOfClasses; ++i )
            means(i)= means(i)/counts(i);

        for ( unsigned int i= 0; i < data.size(); ++i )
        {
            if ( !mask || (*mask)(i) )
            {
                tmp= means(data(i).classIndex) - data(i);
                covars(data(i).classIndex)+= tmp.columnRowProduct(tmp);
            }
        }

        for ( int i= 0; i < data.numberOfClasses; ++i )
            covars(i)= covars(i)/counts(i);

        for ( int i= 0; i < data.numberOfClasses; ++i )
            model.push_back(new GaussianDistribution<T>(means(i), covars(i)));

        return 0;
    }

    template<typename T>
    int ContinuousMultivariateBayesClassifier<T>::classify(Vector<T>& c, Vector<float>* probabilities)
    {
        if ( probabilities == NULL )
            probabilities= &(this->probabilities);

        for ( unsigned int i= 0; i < model.size(); ++i )
            (*probabilities)(i)= model(i)->probability(c);

        int maxC= 0;
        for ( unsigned int i= 1; i < probabilities->size(); ++i )
            if ( (*probabilities)(i) > (*probabilities)(maxC) )
                maxC= i;

        return maxC;
    }
}

#endif	/* CONTINUOUSMULTIVARIATEBAYESCLASSIFIER_H */

