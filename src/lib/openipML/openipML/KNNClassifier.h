/**
 * @file KNNClassifier.h
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
 * KNNClassifier represents the k-Nearest Neighbour classification method.
 */

#ifndef KNNClassifier_H
#define	KNNClassifier_H

#include <limits.h>
#include <omp.h>

#include <openipML/Classifier.h>
#include <openipML/Normalizer.h>

namespace openip
{
    /**
     * KNNClassifier represents the k-Nearest Neighbour classification method.
     */
    template<typename T>
    class KNNClassifier: public Classifier<T>
    {
    public:
        using Classifier<T>::probabilities;
        using Classifier<T>::classNames;
        using Classifier<T>::featureNames;
        using Classifier<T>::saveModelToFile;
        using Classifier<T>::openModelFromFile;

        /**
         * constructor
         * @param k number of neighbours
         */
        KNNClassifier(int k, bool norm= true);

        /**
         * copy constructor
         * @param k instance to copy
         */
        KNNClassifier(const KNNClassifier& k);

        /**
         * destructor
         */
        ~KNNClassifier();

        /**
         * classifier training method
         * @param data training dataset
         * @param mask only those elements are trained, which have a corresponding non-zero element in the mask
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int train(DataSet<T>& data, Vector<unsigned char>* mask= NULL);

        /**
         * classifiaction method
         * @param c vector to classify
         * @param probabilities probability estimates
         * @return predicted class index
         */
        virtual int classify(Vector<T>& c, Vector<float>* probabilities= NULL);

        /**
         * saves the model to output file stream
         * @param file output file stream
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int saveModelToFile(ofstream& file);

        /**
         * opens the model from input file stream
         * @param file input file stream
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int openModelFromFile(ifstream& file);

        /** number of nearest neighbours */
        int k;
        /** dataset */
        DataSet<T> data;
        /** distances */
        Vector<float> dist;
        /** pointer to training mask */
        Vector<unsigned char>* mask;
	
	Normalizer<T>* normalizer;
	bool norm;
    };

    template<typename T>
    KNNClassifier<T>::KNNClassifier(int k, bool norm)
    {
        this->k= k;
	this->norm= norm;
	
	if ( norm )
            normalizer= new Normalizer<T>(0, 1);
        else
            normalizer= NULL;
    }

    template<typename T>
    KNNClassifier<T>::KNNClassifier(const KNNClassifier& k)
    {
      this->k= k.k;
      this->norm= k.norm;
    }

    template<typename T>
    KNNClassifier<T>::~KNNClassifier()
    {
    }

    template<typename T>
    int KNNClassifier<T>::train(DataSet<T>& data, Vector<unsigned char>* mask)
    {
        this->classNames= data.classNames;
        this->featureNames= data.featureNames;
        this->preprocessingNames= data.preprocessingNames;

        this->data= data;
        this->data.init();
        probabilities.resize(this->data.numberOfClasses);
        this->mask= mask;

        return 0;
    }

    template<typename T>
    int KNNClassifier<T>::classify(Vector<T>& c, Vector<float>* probabilities)
    {
        //data.init();

        dist.resize(data.size());

        dist= 0;
        if ( probabilities == NULL )
            probabilities= &(this->probabilities);
        *probabilities= 0;

        float minDist;
        int minIdx;

	#pragma omp parallel for
        for ( unsigned int i= 0; i < data.size(); ++i )
        {
            if ( !mask || (*mask)(i) )
                dist(i)= data(i).dist2(c);
        }

        for ( int i= 0; i < k; ++i )
        {
            minIdx= 0;
            minDist= FLT_MAX;
            for ( unsigned int j= 0; j < dist.size(); ++j )
            {
                if ( !mask || (*mask)(j) )
                {
                    if ( dist(j) < minDist )
                    {
                        minDist= dist(j);
                        minIdx= j;
                    }
                }
            }
            (*probabilities)(data(minIdx).classIndex)= (*probabilities)(data(minIdx).classIndex) + 1;
            dist(minIdx)= FLT_MAX;
        }

        int maxIdx= 0;
        for ( unsigned int i= 0; i < probabilities->size(); ++i )
        {
            (*probabilities)(i)= (*probabilities)(i)/k;
            if ( (*probabilities)(maxIdx) < (*probabilities)(i) )
                maxIdx= i;
        }
        
        return maxIdx;
    }

    template<typename T>
    int KNNClassifier<T>::saveModelToFile(ofstream& file)
    {
        file << "KNNClassifier" << std::endl;
        
        this->Classifier<T>::saveModelToFile(file);

        file << this->k << endl;
        file << data.size() << endl;
        for ( unsigned int i= 0; i < data.size(); ++i )
        {
            file << data(i) << endl;
        }

        return 0;
    }

    template<typename T>
    int KNNClassifier<T>::openModelFromFile(ifstream& file)
    {
        std::string tmp;
        int k;
        FeatureVector<T> tmpfv;

        getline(file, tmp);

        tmpfv.resize(this->featureNames.size());
        if ( tmp.find("KNNClassifier") == 0 )
        {
            this->Classifier<T>::openModelFromFile(file);

            file >> this->k;
            file >> k;

            for ( int i= 0; i < k; ++i )
            {
                file >> tmpfv;
                data.push_back(tmpfv);
            }

            data.init();
            probabilities.resize(this->data.numberOfClasses);
            this->mask= NULL;

            return 0;
        }

        return 1;
    }
}

#endif	/* KNNClassifier_H */

