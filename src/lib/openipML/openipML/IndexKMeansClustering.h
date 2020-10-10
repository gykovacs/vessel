#ifndef INDEXKMEANSCLUSTERING_H
#define INDEXKMEANSCLUSTERING_H

#include <openipML/KMeansClustering.h>

namespace openip
{
    /**
    * KMeansClustering represents the k-means clustering as an optimization problem
    */
    template<typename T>
    class IndexKMeansClustering: public KMeansClustering<T>
    {
    public:
        using KMeansClustering<T>::iteration;
        using KMeansClustering<T>::maxIteration;
        using KMeansClustering<T>::eps;
        using KMeansClustering<T>::k;
        using KMeansClustering<T>::diff;
        using KMeansClustering<T>::means;
        using KMeansClustering<T>::newMeans;
        using KMeansClustering<T>::sizes;
        using KMeansClustering<T>::data;

        /**
        * constructor
        */
        IndexKMeansClustering();

        /**
        * copy constructor
        * @param kmc instance to copy
        */
        IndexKMeansClustering(const IndexKMeansClustering& kmc);

        /**
        * destructor
        */
        ~IndexKMeansClustering();

        /**
        * setup the optimization problem
        * @param input input vectors
        * @param k number of clusters
        * @param eps convergence limit
        * @param maxIteration maximum number of iterations
        */
        void setup(Vector<T>* input, int k= 2, float eps= 0.0000001, int maxIteration= 20000);

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
        * do data labeling based on the clusters
        * @param data data vector to label
        * @param mask output labeling
        */
        void labelize(Vector<T>* data, Vector<unsigned char>* mask);

        /**
        * checks wether the stopping condition holds
        * @return true if stopping condition holds, otherwise false
        */
        int stopCondition();

        Vector<float> weights;
    };

    template<typename T>
    IndexKMeansClustering<T>::IndexKMeansClustering()
    : KMeansClustering<T>()
    {
    }

    template<typename T>
    IndexKMeansClustering<T>::IndexKMeansClustering(const IndexKMeansClustering<T>& kmc)
    : KMeansClustering<T>(kmc)
    {
    }

    template<typename T>
    IndexKMeansClustering<T>::~IndexKMeansClustering()
    {
    }

    template<typename T>
    void IndexKMeansClustering<T>::setup(Vector<T>* input, int k, float eps, int maxIteration)
    {
        this->eps= eps;
        this->maxIteration= maxIteration;
        this->data= input;
        this->means.resize(k);
        this->newMeans.resize(k);
        this->weights.resize(k);
        this->sizes.resize(k);
        this->k= k;
        this->diff= FLT_MAX;
    }

    template<typename T>
    void IndexKMeansClustering<T>::init()
    {
        for ( unsigned int i= 0; i < means.size(); ++i )
        {
            int idx= float(data->size())/float(means.size())*i + float(data->size())/float(2*means.size());
            means[i]= idx;
        }
    }

    template<typename T>
    void IndexKMeansClustering<T>::expect()
    {
       // printf("kexpect\n"); fflush(stdout);
        int minIdx;

        for ( int i= 0; i < k; ++i )
        {
            sizes[i]= 0;
            newMeans[i]= 0;
            weights[i]= 0;
        }

        for ( unsigned int i= 0; i < data->size(); ++i )
        {
            minIdx= 0;
            for ( unsigned int j= 0; j < means.size(); ++j )
                if ( abs(means[j] - i) < abs(means[minIdx] - i))
                    minIdx= j;

            newMeans[minIdx]+= (*data)(i) * i;
            weights[minIdx]+= (*data)(i);
            sizes[minIdx]++;
        }
    }

    template<typename T>
    void IndexKMeansClustering<T>::maximize()
    {
        //printf("kmax\n"); fflush(stdout);
        diff= 0;
        for ( int i= 0; i < k; ++i )
        {
            if ( weights[i] != 0 )
                newMeans[i]/= weights[i];
            else
                newMeans[i]= means[i] / 2;

            //cout << i << ": " << newMeans[i] << " " << sizes[i] << endl;
            diff+= openip::distance(means[i],newMeans[i]);
            means[i]= newMeans[i];
        }
    }

    template<typename T>
    int IndexKMeansClustering<T>::stopCondition()
    {
         //printf("condition\n"); fflush(stdout);
        if ( (iteration > maxIteration) || (diff < eps) )
            return 1;
        return 0;
    }

    template<typename T>
    void IndexKMeansClustering<T>::labelize(Vector<T>* data, Vector<unsigned char>* mask)
    {
        typename Vector<T>::iterator vit= data->begin();

        for ( typename Vector<unsigned char>::iterator mit= mask->begin(); mit != mask->end(); ++mit, ++vit )
            *mit= means.getIndexOfClosestElement(*vit);
    }
}


#endif // INDEXKMEANSCLUSTERING_H
