/**
 * @file KMeansClustering.h
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
 * The KMeansClustering represents the k-means clustering optimization problem.
 */

#ifndef _KMEAN_CLUSTERING_H_
#define _KMEAN_CLUSTERING_H_

#include <stdlib.h>
#include <openipDS/Vector.h>
#include <openipDS/FeatureVector.h>
#include <openipDS/Stopper.h>

#include <openipML/ExpectationMaximization.h>

namespace openip
{
    /**
    * KMeansClustering represents the k-means clustering as an optimization problem
    */
    template<typename T>
    class KMeansClustering: public ExpectationMaximization
    {
    public:
	/**
	* constructor
	*/
        KMeansClustering();

	/**
	* copy constructor
	* @param kmc instance to copy
	*/
        KMeansClustering(const KMeansClustering& kmc);

	/**
	* destructor
	*/
        ~KMeansClustering();

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
	
	float getObjective();

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

	/**
	* size of clusters
	*/
        Vector<int> sizes;

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
	
	Vector<T> lastMeans;
	int iter;
    };

    template<typename T>
    KMeansClustering<T>::KMeansClustering()
    : ExpectationMaximization(), data(NULL), diff(100000000.0f), maxIteration(0), eps(0.0f), k(0)
    {
      iter= 0;
    }

    template<typename T>
    KMeansClustering<T>::KMeansClustering(const KMeansClustering<T>& kmc)
    : ExpectationMaximization(kmc), data(kmc.data), diff(kmc.diff), maxIteration(kmc.maxIteration), eps(kmc.eps), k(kmc.k), means(kmc.means), sizes(kmc.sizes)
    {
    }

    template<typename T>
    KMeansClustering<T>::~KMeansClustering()
    {
    }

    template<typename T>
    void KMeansClustering<T>::setup(Vector<T>* input, int k, float eps, int maxIteration)
    {
        this->eps= eps;
        this->maxIteration= maxIteration;
        this->data= input;
        this->means.resize(k);
        this->newMeans.resize(k);
        this->sizes.resize(k);
        this->k= k;
        this->diff= FLT_MAX;
	this->lastMeans.resize(k);
	lastMeans= 0;

        /*if ( dynamic_cast<Vector<Vector<float> >*>(input) != NULL )
        {
            tprintf("works\n");
            Vector<Vector<float> >* tinput= dynamic_cast<Vector<Vector<float> >*>(input);
            Vector<Vector<float> >* tmeans= dynamic_cast<Vector<Vector<float> >*>(&means);
            Vector<Vector<float> >* tnmeans= dynamic_cast<Vector<Vector<float> >*>(&newMeans);
            int m= (*tinput)(0).size();
            for ( int i= 0; i < tmeans->size(); ++i )
            {
                (*tmeans)(i).resize(m);
                (*tnmeans)(i).resize(m);
            }
        }*/
    }

    template<typename T>
    void KMeansClustering<T>::init()
    {
        for ( unsigned int i= 0; i < means.size(); ++i )
        {
            int idx= rand()%(data->size());
            means[i]= (*data)(idx);
            newMeans[i]= (*data)(idx);
        }
    }

    template<typename T>
    void KMeansClustering<T>::expect()
    {
        //tprintf("kexpect %d\n", k);
        int minIdx= 0;

        for ( int i= 0; i < k; ++i )
        {
            sizes[i]= 0;
            newMeans[i]= 0;
        }

        /*for ( typename Vector<T>::iterator vit= data->begin(); vit != data->end(); ++vit )
        {
            minIdx= means.getIndexOfClosestElement(*vit);

            newMeans[minIdx]+= *vit;
            sizes[minIdx]++;
        }*/
        //#pragma omp parallel for
        for ( unsigned int i= 0; i < data->size(); ++i )
        {
            /*minIdx= means.getIndexOfClosestElement((*data)(i));*/
	    for ( unsigned int j= 0; j < means.size(); ++j )
	      if ( fabs(means(j) - (*data)(i) ) < fabs(means(minIdx) - (*data)(i)) )
		minIdx= j;

            //#pragma omp critical
            {
                newMeans[minIdx]+= (*data)(i);
                sizes[minIdx]++;
            }
        }
        //tprintf("kexpect finished %d\n", k);
    }

    template<typename T>
    void KMeansClustering<T>::maximize()
    {
        //tprintf("kmax %d\n", k);
        diff= 0;
	//cout << endl;
        stringstream ss;
        for ( int i= 0; i < k; ++i )
        {
            if ( sizes[i] != 0 )
                newMeans[i]/= sizes[i];
            else
                newMeans[i]= means[i] / 2;

            //cout << i << ": " << newMeans[i] << " " << sizes[i] << "; ";
            
            diff+= openip::distance(means[i],newMeans[i]);
            /*if ( i == k-1 )
                tprintf("diff means[0]: %f %f %d\n", means[0], newMeans[0], sizes[0]);*/
            //ss << means[i] << " " << newMeans[i] << " ";
        }
        //tprintf("diff: %s\n", ss.str().c_str());
        for ( int i= 0; i < k; ++i )
        {
            means[i]= newMeans[i];
        }
        //tprintf("kmax finished %d %f\n", k, diff);
    }

    template<typename T>
    int KMeansClustering<T>::stopCondition()
    {
        //tprintf("condition %d/%d %f\n", iteration, maxIteration, eps);
      
        if ( (iteration > maxIteration) || (diff < eps) )
	{
            return 1;
	}
        return 0;
    }

    template<typename T>
    void KMeansClustering<T>::labelize(Vector<T>* data, Vector<unsigned char>* mask)
    {
      
      
        typename Vector<T>::iterator vit= data->begin();

        for ( typename Vector<unsigned char>::iterator mit= mask->begin(); mit != mask->end(); ++mit, ++vit )
            *mit= means.getIndexOfClosestElement(*vit);
    }
    
    template<typename T>
    float KMeansClustering<T>::getObjective()
    {
        //tprintf("getObjective %d\n", k);
      Vector<float> variances;
      variances.resize(means.size());
      variances= 0;
      for ( unsigned int i= 0; i < (*data).size(); ++i )
      {
	int j= means.getIndexOfClosestElement((*data)(i));
	  variances(j)+= (means(j) - (*data)(i))*(means(j) - (*data)(i));
      }
      float a= 0;
      for ( unsigned int i= 0; i < variances.size(); ++i )
	a+= variances(i);
      return a;
    }
}

#endif
