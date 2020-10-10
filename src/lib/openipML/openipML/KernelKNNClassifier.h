/*
 * File:   KernelKNNClassifier.h
 * Author: gykovacs
 *
 * Created on 2011. február 22., 14:00
 */

#ifndef KERNELKNNCLASSIFIER_H
#define	KERNELKNNCLASSIFIER_H

#include <limits.h>
#include <omp.h>

#include <openipML/Classifier.h>
#include <openipML/KernelFunctions.h>
#include <openipML/DistanceFunctions.h>
#include <openipML/DistanceFunctionIO.h>
#include <openipML/Normalizer.h>
#include <openipML/PrincipalComponentAnalysis.h>

namespace openip
{
    /**
     * KernelKNNClassifier represents the k-Nearest Neighbour classification method using kernels.
     */
    template<typename T>
    class KernelKNNClassifier: public Classifier<T>
    {
    public:
        using Classifier<T>::probabilities;
        using Classifier<T>::classNames;
        using Classifier<T>::featureNames;
        using Classifier<T>::saveModelToFile;
        using Classifier<T>::openModelFromFile;
	using Classifier<T>::unknown;
	using Classifier<T>::unknownSet;

        /**
         * constructor
         * @param k number of neighbours
         */
        KernelKNNClassifier(int k, DistanceFunction<T> * distance= new KernelEuclideanDistance<T>(new DotProductKernel<T>()), bool norm= true, int pca= 0, int unknown= 28);

        /**
         * copy constructor
         * @param k instance to copy
         */
        KernelKNNClassifier(const KernelKNNClassifier& k);

        /**
         * destructor
         */
        ~KernelKNNClassifier();

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

        DistanceFunction<T>* distance;

        Normalizer<T>* normalizer;
	
	Vector<T> c1;
	Vector<float> smallestDistances;
	
	int pca;
    };

    template<typename T>
    KernelKNNClassifier<T>::KernelKNNClassifier(int k, DistanceFunction<T>* distance, bool norm, int pca, int unknown)
        : Classifier<T>(unknown)
    {
        this->k= k;
        this->distance= distance;

        if ( norm )
            normalizer= new Normalizer<T>(0, 1, unknown);
        else
            normalizer= NULL;
	
	this->pca= pca;
    }

    template<typename T>
    KernelKNNClassifier<T>::KernelKNNClassifier(const KernelKNNClassifier& k)
        : Classifier<T>(k)
    {
        this->k= k.k;
        this->distance= k.distance;
        this->mask= k.mask;
        this->dist= k.dist;
        this->normalizer= new Normalizer<T>(*(k.normalizer));
	this->pca= k.pca;
    }

    template<typename T>
    KernelKNNClassifier<T>::~KernelKNNClassifier()
    {
    }

    template<typename T>
    int KernelKNNClassifier<T>::train(DataSet<T>& data, Vector<unsigned char>* mask)
    {
        this->classNames= data.classNames;
        this->featureNames= data.featureNames;
        this->preprocessingNames= data.preprocessingNames;

        this->data.clear();

        if ( normalizer )
            normalizer->train(data);
        
        FeatureVector<T> fv;
        fv.resize(featureNames.size());
        for ( unsigned int i= 0; i < data.size(); ++i )
            if ( !mask || (*mask)(i) )
            {
                if ( normalizer )
                {
                    normalizer->normalize(data(i), fv);
                    fv.weightForeground= data(i).weightForeground;
                    fv.weightBackground= data(i).weightBackground;
                    fv.classIndex= data(i).classIndex;
                    this->data.push_back(fv);
                }
                else
                    this->data.push_back(data(i));
            }
        
        this->data.init();
        
        probabilities.resize(this->data.numberOfClasses);
        this->mask= mask;

        return 0;
    }

    template<typename T>
    int KernelKNNClassifier<T>::classify(Vector<T>& c, Vector<float>* probabilities)
    {
        dist.resize(data.size());

        dist= 0;
        if ( probabilities == NULL )
            probabilities= &(this->probabilities);
        *probabilities= 0;

        smallestDistances.resize(probabilities->size());
        smallestDistances= FLT_MAX;

        float minDist;
        int minIdx;

        c1.resize(c.size());

        if ( normalizer )
            normalizer->normalize(c, c1);
        else
            c1= c;

	//#pragma omp parallel for private(sum)
        for ( unsigned int i= 0; i < data.size(); ++i )
        {
            dist(i)= distance->evaluate(data(i), c1);
/*	    sum= 0;
	    for ( unsigned int j= 0; j < c1.size(); ++j )
	      sum+= (data(i)(j) - c1(j))*(data(i)(j) - c1(j));
	    dist(i)= sum;*/
        }

        for ( int i= 0; i < k; ++i )
        {
            minIdx= 0;
            minDist= FLT_MAX;
            for ( unsigned int j= 0; j < dist.size(); ++j )
            {
                if ( dist(j) < minDist )
                {
                    minDist= dist(j);
                    minIdx= j;
                }
            }

            if ( data(minIdx).weightForeground > 0 )
	      (*probabilities)(data(minIdx).classIndex)= (*probabilities)(data(minIdx).classIndex) + data(minIdx).weightForeground;
	    else
	      (*probabilities)(data(minIdx).classIndex)= (*probabilities)(data(minIdx).classIndex) + 1;
            dist(minIdx)= FLT_MAX;

            if ( smallestDistances(data(minIdx).classIndex) > minDist )
                smallestDistances(data(minIdx).classIndex)= minDist;
        }

        int maxIdx= 0;
	float sumP= probabilities->getSum();
        for ( unsigned int i= 0; i < probabilities->size(); ++i )
        {
            (*probabilities)(i)= (*probabilities)(i)/sumP;
            if ( (*probabilities)(maxIdx) < (*probabilities)(i) )
                maxIdx= i;
	    smallestDistances(i)= 1.0/(1.0 + smallestDistances(i));
            /*if ( smallestDistances(i) > 0 )
                smallestDistances(i)= 1.0/smallestDistances(i);
            else
                smallestDistances(i)= 10000000000000;*/
        }

        smallestDistances/= smallestDistances.getSum();

        *probabilities= smallestDistances;

        return maxIdx;
    }

    template<typename T>
    int KernelKNNClassifier<T>::saveModelToFile(ofstream& file)
    {
        file << "KernelKNNClassifier" << std::endl;

        this->Classifier<T>::saveModelToFile(file);
        if ( normalizer )
        {
            file << "1" << std::endl;
            normalizer->saveToFile(file);
        }
        else
            file << "0" << std::endl;
        DistanceFunctionIO<T> dfio;
        dfio.saveDistanceFunctionToFile(distance, file);

        file << this->k << endl;
        file << data.size() << endl;
        for ( unsigned int i= 0; i < data.size(); ++i )
        {
            file << data(i) << endl;
        }

        return 0;
    }

    template<typename T>
    int KernelKNNClassifier<T>::openModelFromFile(ifstream& file)
    {
        std::string tmp;
        int n;
        FeatureVector<T> tmpfv;

        getline(file, tmp);
        if ( tmp.find("KernelKNNClassifier") == 0 )
        {
            this->Classifier<T>::openModelFromFile(file);

            int flag;
            file >> flag;
            if ( flag )
            {
                normalizer= new Normalizer<T>(0,1);
                normalizer->openFromFile(file);
            }
            else
                normalizer= NULL;

            file.get();
            DistanceFunctionIO<T> dfio;
            distance= dfio.openDistanceFunctionFromFile(file);
            file >> this->k;
            file >> n;

            file.get();

            tmpfv.resize(featureNames.size());
            data.resize(n);
            for ( int i= 0; i < n; ++i )
            {
                file >> tmpfv;
                data(i)= tmpfv;
            }

            data.classNames= this->classNames;
            data.featureNames= this->featureNames;
            data.preprocessingNames= this->preprocessingNames;
            printf("kknn read %lu %lu %lu %d %d %lu %lu\n", data.classNames.size(), data.featureNames.size(), data.preprocessingNames.size(), k, n, data.size(), data(0).size()); fflush(stdout);
            data.init();
            printf("initialized\n"); fflush(stdout);
            probabilities.resize(this->data.numberOfClasses);
            this->mask= NULL;

            return 0;
        }

        return 1;
    }
}

#endif	/* KERNELKNNCLASSIFIER_H */

