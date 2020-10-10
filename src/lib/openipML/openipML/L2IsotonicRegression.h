#ifndef _L2ISOTONIC_REGRESSION_H_
#define _L2ISOTONIC_REGRESSION_H_

#include <limits.h>
#include <omp.h>

#include <openipML/Regression.h>
#include <openipML/Normalizer.h>
#include <openipML/PrincipalComponentAnalysis.h>
#include <openipML/PoolAdjacentViolatorsAlgorithm.h>

namespace openip
{
    template<typename T>
    class L2IsotonicRegression: public Regression<T>
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
        L2IsotonicRegression(int decreasing= 0, int fromright= 0, int coord= 0, bool norm= true, int pca= 0);

        /**
         * copy constructor
         * @param k instance to copy
         */
        L2IsotonicRegression(const L2IsotonicRegression& k);

        /**
         * destructor
         */
        ~L2IsotonicRegression();

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

        Normalizer<T>* normalizer;
	
	int pca;
	
	int decreasing;
	int fromright;
	int coord;
    };

    template<typename T>
    L2IsotonicRegression<T>::L2IsotonicRegression(int decreasing, int fromright, int coord, bool /*norm*/, int pca)
        : Regression<T>()
    {
        /*if ( norm )
            normalizer= new Normalizer<T>(0, 1, unknown);
        else
            normalizer= NULL;*/
	
	this->pca= pca;
	this->decreasing= decreasing;
	this->fromright= fromright;
	this->coord= coord;
    }

    template<typename T>
    L2IsotonicRegression<T>::L2IsotonicRegression(const L2IsotonicRegression& k)
        : Regression<T>(k)
    {
        this->mask= k.mask;
        this->dist= k.dist;
        this->normalizer= new Normalizer<T>(*(k.normalizer));
	this->pca= k.pca;
	this->decreasing= k.decreasing;
	this->fromright= k.fromright;
	this->coord= k.coord;
    }

    template<typename T>
    L2IsotonicRegression<T>::~L2IsotonicRegression()
    {
    }

    template<typename T>
    int L2IsotonicRegression<T>::train(RegressionDataSet<T>& data, Vector<unsigned char>* mask)
    {
      tprintf("start training\n");
        this->featureNames= data.featureNames;
        this->preprocessingNames= data.preprocessingNames;
        this->data.clear();

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
        
        tprintf("initializing dataset\n");
        this->data.init();
	tprintf("sorting by coordinate\n");
	for ( unsigned int i= 0; i < this->data.size(); ++i )
	  printf("%f ", this->data(i)(coord));
	printf("\n");
	this->data.sortByCoordinate(coord);
	for ( unsigned int i= 0; i < this->data.size(); ++i )
	  printf("%f ", this->data(i)(coord));
	printf("\n");
	
	Vector<T> values;
	this->data.getCoordinateVector(-1, values);
	
	tprintf("applying PAVA\n");
	PoolAdjacentViolatorsAlgorithm<T, T> pava(decreasing, fromright);
	Vector<T> output;
	output.resize(values.size());
	for ( unsigned int i= 0; i < values.size(); ++i )
	  printf("%f ", values(i));
	printf("\n");
	pava.apply(values, output);
	for ( unsigned int i= 0; i < output.size(); ++i )
	  printf("%f ", output(i));
	printf("\n");
	
	this->data.setCoordinateVector(-1, values);
        
        this->mask= mask;

	tprintf("training finished\n");
        return 0;
    }

    template<typename T>
    float L2IsotonicRegression<T>::regress(Vector<T>& c, Vector<float>* probabilities)
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
	    dist(i)= fabs(c(coord) - data(i)(coord));
	}

	Vector<int> indices;
	for ( int i= 0; i < 1; ++i )
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

	/*float mean= 0;
	float sum= 0;
	for ( int i= 0; i < indices.size(); ++i )
	{
	  float d= distance->evaluate(data(indices(i)), c);
	  mean+= data(indices(i)).targetValue * d;
	  sum+= d;
	}
	mean/= sum;*/
	
	return data(minIdx)(coord);
    }

    template<typename T>
    int L2IsotonicRegression<T>::saveModelToFile(ofstream& file)
    {
        file << "L2IsotonicRegression" << std::endl;

        this->Regression<T>::saveModelToFile(file);
        if ( normalizer )
        {
            file << "1" << std::endl;
            normalizer->saveToFile(file);
        }
        else
            file << "0" << std::endl;

	file << decreasing << endl;
	file << fromright << endl;
	file << coord << endl;
        file << data.size() << endl;
        for ( unsigned int i= 0; i < data.size(); ++i )
        {
            file << data(i) << endl;
        }

        return 0;
    }

    template<typename T>
    int L2IsotonicRegression<T>::openModelFromFile(ifstream& file)
    {
        std::string tmp;
        int n;
        RegressionFeatureVector<T> tmpfv;

        getline(file, tmp);
        if ( tmp.find("L2IsotonicRegression") == 0 )
        {
            this->Regression<T>::openModelFromFile(file);

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
	    
	    file >> decreasing;
	    file >> fromright;
	    file >> coord;
            file >> n;

            file.get();

            tmpfv.resize(featureNames.size());
            data.resize(n);
            for ( int i= 0; i < n; ++i )
            {
                file >> tmpfv;
                data(i)= tmpfv;
            }

            data.featureNames= this->featureNames;
            data.preprocessingNames= this->preprocessingNames;
            printf("isotonic regression read %lu %lu %d %lu %lu\n", data.featureNames.size(), data.preprocessingNames.size(), n, data.size(), data(0).size()); fflush(stdout);
            data.init();
            printf("initialized\n"); fflush(stdout);
            this->mask= NULL;

            return 0;
        }

        return 1;
    }
}

#endif