#ifndef _KERNELKNN_REGRESSION_H_
#define _KERNELKNN_REGRESSION_H_

#include <limits.h>
#include <omp.h>

#include <openipML/Regression.h>
#include <openipML/DistanceFunctions.h>
#include <openipML/KernelFunctions.h>
#include <openipML/Normalizer.h>
#include <openipML/PrincipalComponentAnalysis.h>

namespace openip
{
    /**
     * KernelKNNClassifier represents the k-Nearest Neighbour classification method using kernels.
     */
    template<typename T>
    class KernelKNNRegression: public Regression<T>
    {
    public:
        using Regression<T>::probabilities;
        using Regression<T>::featureNames;
        using Regression<T>::saveModelToFile;
        using Regression<T>::openModelFromFile;

        /**
         * constructor
         * @param k number of neighbours
         */
        KernelKNNRegression(int k, DistanceFunction<T> * distance= new KernelEuclideanDistance<T>(new DotProductKernel<T>()), bool norm= true, int pca= 0);

        /**
         * copy constructor
         * @param k instance to copy
         */
        KernelKNNRegression(const KernelKNNRegression& k);

        /**
         * destructor
         */
        ~KernelKNNRegression();

        /**
         * classifier training method
         * @param data training dataset
         * @param mask only those elements are trained, which have a corresponding non-zero element in the mask
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int train(RegressionDataSet<T>& data, Vector<unsigned char>* mask= NULL);

        /**
         * classifiaction method
         * @param c vector to classify
         * @param probabilities probability estimates
         * @return predicted class index
         */
        virtual float regress(Vector<T>& c, Vector<float>* probabilities= NULL);

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
        RegressionDataSet<T> data;
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
    KernelKNNRegression<T>::KernelKNNRegression(int k, DistanceFunction<T>* distance, bool /*norm*/, int pca)
        : Regression<T>()
    {
        this->k= k;
        this->distance= distance;

        /*if ( norm )
            normalizer= new Normalizer<T>(0, 1, unknown);
        else
            normalizer= NULL;*/
	
	this->pca= pca;
    }

    template<typename T>
    KernelKNNRegression<T>::KernelKNNRegression(const KernelKNNRegression& k)
        : Regression<T>(k)
    {
        this->k= k.k;
        this->distance= k.distance;
        this->mask= k.mask;
        this->dist= k.dist;
        this->normalizer= new Normalizer<T>(*(k.normalizer));
	this->pca= k.pca;
    }

    template<typename T>
    KernelKNNRegression<T>::~KernelKNNRegression()
    {
    }

    template<typename T>
    int KernelKNNRegression<T>::train(RegressionDataSet<T>& data, Vector<unsigned char>* mask)
    {
        this->featureNames= data.featureNames;
        this->preprocessingNames= data.preprocessingNames;

        this->data.clear();

        /*if ( normalizer )
            normalizer->train(data);*/
        
        RegressionFeatureVector<T> fv;
        fv.resize(featureNames.size());
        for ( unsigned int i= 0; i < data.size(); ++i )
            if ( !mask || (*mask)(i) )
            {
                /*if ( normalizer )
                {
                    normalizer->normalize(data(i), fv);
                    fv.targetValue= data(i).targetValue;
                    this->data.push_back(fv);
                }
                else*/
                    this->data.push_back(data(i));
            }
        
        this->data.init();
        
        this->mask= mask;

        return 0;
    }

    template<typename T>
    float KernelKNNRegression<T>::regress(Vector<T>& c, Vector<float>* probabilities)
    {
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
		dist(i)= distance->evaluate(data(i),c);
	}

	Vector<int> indices;
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
	    indices.push_back(minIdx);
	    dist(minIdx)= FLT_MAX;
	}

	float mean= 0;
	float sum= 0;
	for ( unsigned int i= 0; i < indices.size(); ++i )
	{
	  float d= distance->evaluate(data(indices(i)), c);
	  mean+= data(indices(i)).targetValue * d;
	  sum+= d;
	}
	mean/= sum;
	
	return mean;
    }

    template<typename T>
    int KernelKNNRegression<T>::saveModelToFile(ofstream& /*file*/)
    {
        /*file << "KernelKNNClassifier" << std::endl;

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
        }*/

        return 0;
    }

    template<typename T>
    int KernelKNNRegression<T>::openModelFromFile(ifstream& /*file*/)
    {
        /*std::string tmp;
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
        }*/

        return 1;
    }
}

#endif