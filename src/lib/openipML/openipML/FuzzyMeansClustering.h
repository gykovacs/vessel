#ifndef FUZZYMEANSCLUSTERING_H
#define FUZZYMEANSCLUSTERING_H

#include <stdlib.h>
#include <openipDS/Vector.h>
#include <openipDS/FeatureVector.h>
#include <openipDS/Matrix.h>

#include <openipML/ExpectationMaximization.h>

namespace openip
{
    /**
    * FuzzyMeansClustering represents the fuzzy-means clustering as an optimization problem
    */
    template<typename T>
    class FuzzyMeansClustering: public ExpectationMaximization
    {
    public:
        /**
        * constructor
        */
        FuzzyMeansClustering();

        /**
        * copy constructor
        * @param kmc instance to copy
        */
        FuzzyMeansClustering(const FuzzyMeansClustering& kmc);

        /**
        * destructor
        */
        ~FuzzyMeansClustering();

        /**
        * setup the optimization problem
        * @param input input vectors
        * @param k number of clusters
        * @param eps convergence limit
        * @param maxIteration maximum number of iterations
        */
        void setup(Vector<T>* input, int k= 2, float eps= 0.0000001, int maxIteration= 20000, float fuzzyness= 2.0);

        /**
        * initialize iterations
        */
        void init();

        /**
        * do expectation
        */
        void expect();

        /**
        * do maximization
        */
        void maximize();

       /**
        * checks wether the stopping condition holds
        * @return true if stopping condition holds, otherwise false
        */
        int stopCondition();

        /**
        * input data pointer
        */
        Vector<T>* data;

        /**
        * vector of current cluster means
        */
        Vector<T> means;

        /**
        * vector of new cluster means
        */
        Vector<T> newMeans;

        Matrix<float> membership;

        /**
        * difference between the previous and current model
        */
        float diff;

        /**
        * number of maximum iterations
        */
        int maxIteration;

        /**
        * convergence limit
        */
        float eps;

        /**
        * number of desired clusters
        */
        int k;

        float fuzzyness;
    };

    template<typename T>
    FuzzyMeansClustering<T>::FuzzyMeansClustering()
        : ExpectationMaximization(), data(NULL), diff(100000000.0f), maxIteration(0), eps(0.0f), k(0), fuzzyness(2.0)
    {
    }

    template<typename T>
    FuzzyMeansClustering<T>::FuzzyMeansClustering(const FuzzyMeansClustering<T>& kmc)
        : ExpectationMaximization(kmc), data(kmc.data), diff(kmc.diff), maxIteration(kmc.maxIteration), eps(kmc.eps), k(kmc.k), means(kmc.means), fuzzyness(kmc.fuzzyness)
    {
    }

    template<typename T>
    FuzzyMeansClustering<T>::~FuzzyMeansClustering()
    {
    }

    template<typename T>
    void FuzzyMeansClustering<T>::setup(Vector<T>* input, int k, float eps, int maxIteration, float fuzzyness)
    {
        this->eps= eps;
        this->maxIteration= maxIteration;
        this->data= input;
        this->means.resize(k);
        this->newMeans.resize(k);
        this->k= k;
        this->diff= FLT_MAX;
        this->fuzzyness= fuzzyness;
    }

    template<typename T>
    void FuzzyMeansClustering<T>::init()
    {
        membership.resize(data->size(), k);

        T min, max;
        data->getMinMax(min, max);

        for ( unsigned int i= 0; i < means.size(); ++i )
        {
            means[i]= min + i*(max - min)/float(k);
        }
    }

    template<typename T>
    void FuzzyMeansClustering<T>::expect()
    {
        printf("kexpect\n"); fflush(stdout);

        float tmp, a, b;
        for ( unsigned int i= 0; i < k; ++i )
        {
            newMeans[i]= 0;
        }
        for ( unsigned int i= 0; i < data->size(); ++i )
        {
            for ( unsigned int j= 0; j < this->k; ++j )
            {
                tmp= 0;
                for ( unsigned int l= 0; l < this->k; ++l )
                {
                    a= fabs((*data)(i) - means(j));
                    a= a*a;
                    b= fabs((*data)(i) - means(l));
                    b= b*b;

                    if ( b == 0 )
                    {
                        for ( unsigned int n= 0; n < this->k; ++n )
                        {
                            if ( n != l )
                                membership(i,n)= 0;
                            else
                                membership(i,n)= 1;
                        }
                        goto singularity;
                    }

                    tmp+= pow(a/b, 2.0/(fuzzyness-1.0));
                }
                membership(i,j)= 1.0/tmp;
            }
            singularity:
            ;
        }

    }

    template<typename T>
    void FuzzyMeansClustering<T>::maximize()
    {
        printf("kmax\n"); fflush(stdout);
        float a, b, tmp;
        diff= 0;

        for ( int i= 0; i < k; ++i )
        {
            a= 0;
            b= 0;
            for ( int j= 0; j < data->size(); ++j )
            {
                tmp= pow(membership(j,i), fuzzyness);
                a+= tmp*(*data)(j);
                b+= tmp;
            }
            newMeans(i)= a/b;
        }

        for ( int i= 0; i < k; ++i )
        {
            cout << i << ": " << newMeans[i] << "\t";
            if ( fabs(means[i] - newMeans[i]) > diff )
                diff= fabs(means[i]-newMeans[i]);
            means[i]= newMeans[i];
        }
    }

    template<typename T>
    int FuzzyMeansClustering<T>::stopCondition()
    {
         //printf("condition\n"); fflush(stdout);
        if ( (iteration > maxIteration) || (diff < eps) )
            return 1;
        return 0;
    }

    /*template<typename T>
    void fuzzyMeansClustering(T* data, unsigned int n, T* init, unsigned int k, float f, float* membership, T* means, int maxIterations= 10, float epsilon= 1.0 )
    {
        //init

        float diff;
        int iteration= 0;
        T newMeans[k];

        T min, max;
        for ( int i= 0; i < n; ++i )
        {
            if ( data[i] < min )
                min= data[i];
            if ( data[i] > max )
                max= data[i];
        }

        for ( unsigned int i= 0; i < k; ++i )
        {
            means[i]= min + i*(max - min)/float(k);
        }

        while (1)
        {
            ++iteration;
            //expect
            float tmp, a, b;
            for ( unsigned int i= 0; i < k; ++i )
            {
                newMeans[i]= 0;
            }
            for ( unsigned int i= 0; i < n; ++i )
            {
                for ( unsigned int j= 0; j < k; ++j )
                {
                    tmp= 0;
                    for ( unsigned int l= 0; l < k; ++l )
                    {
                        a= fabs(data[i] - means[j]);
                        a= a*a;
                        b= fabs(data[i] - means[l]);
                        b= b*b;

                        if ( b == 0 )
                        {
                            for ( unsigned int m= 0; m < k; ++m )
                            {
                                if ( n != l )
                                    membership[i*k+m]= 0;
                                else
                                    membership[i*k+m]= 1;
                            }
                            goto singularity;
                        }

                        tmp+= pow(a/b, 2.0/(f-1.0));
                    }
                    membership[i*k+j]= 1.0/tmp;
                }
                singularity:
                ;
            }

            //maximize
            diff= 0;

            for ( int i= 0; i < k; ++i )
            {
                a= 0;
                b= 0;
                for ( int j= 0; j < n; ++j )
                {
                    tmp= pow(membership[j*k+i], f);
                    a+= tmp*(data[j]);
                    b+= tmp;
                }
                newMeans[i]= a/b;
            }

            for ( int i= 0; i < k; ++i )
            {
                cout << i << ": " << newMeans[i] << "\t";
                if ( fabs(means[i] - newMeans[i]) > diff )
                    diff= fabs(means[i]-newMeans[i]);
                means[i]= newMeans[i];
            }

            //stop condition
            if ( (iteration > maxIterations) || (diff < epsilon) )
                return;
        }
    }*/
}

#endif // FUZZYMEANSCLUSTERING_H
