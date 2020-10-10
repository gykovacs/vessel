/**
 * @file KMeansClusteringDiscretizer.h
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
 * KMeansClusteringDiscretizer represents the discretization method based on
 * k-means clustering.
 */

#ifndef KMEANSCLUSTERINGDISCRETIZER_H
#define	KMEANSCLUSTERINGDISCRETIZER_H

#include <openipDS/DiscreteDataSet.h>
#include <openipDS/ContinuousDataSet.h>

#include <openipML/Discretizer.h>
#include <openipML/KMeansClustering.h>

namespace openip
{
    /**
     * KMeansClusteringDiscretizer represents the discretization method based on
     * k-means clustering.
     */
    template<typename T>
    class KMeansClusteringDiscretizer: public Discretizer<T>
    {
    public:
        using Discretizer<T>::openDiscretizerFromFile;
        using Discretizer<T>::saveDiscretizerToFile;
        using Discretizer<T>::discretize;
        using Discretizer<T>::data;
        using Discretizer<T>::k;
	using Discretizer<T>::unknown;
	using Discretizer<T>::unknownSet;
        using Discretizer<T>::error;

        /**
         * constructor
         * @param k number of clusters (discrete objects)
         */
        KMeansClusteringDiscretizer(int k= 10, int unknown= 28, int multi= 0);

        /**
         * copy constructor
         * @param d instance to copy
         */
        KMeansClusteringDiscretizer(const KMeansClusteringDiscretizer& d);

        /**
         * destructor
         */
        ~KMeansClusteringDiscretizer();

        /**
         * initializes the discretizer
         * @param input the discretizer is initialized with this dataset
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int init(DataSet<T>& input);

        /**
         * opens the discretizer from input file stream
         * @param file input file stream
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int openDiscretizerFromFile(std::ifstream& file);

        /**
         * saves the discretizer to output file stream
         * @param file output file stream
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int saveDiscretizerToFile(std::ofstream& file);
	
	int multi;
    };

    template<typename T>
    KMeansClusteringDiscretizer<T>::KMeansClusteringDiscretizer(int k, int unknown, int multi)
    : Discretizer<T>(unknown)
    {
        this->k= k;
        this->data.resize(k);
	this->multi= multi;
    }

    template<typename T>
    KMeansClusteringDiscretizer<T>::KMeansClusteringDiscretizer(const KMeansClusteringDiscretizer& d)
    : Discretizer<T>(d)
    {
        this->k= d.k;
        this->data= d.data;
	this->multi= d.multi;
    }

    template<typename T>
    KMeansClusteringDiscretizer<T>::~KMeansClusteringDiscretizer()
    {
    }

    template<typename T>
    int KMeansClusteringDiscretizer<T>::init(DataSet<T>& input)
    {
        error= 0;
        input.init();

        data.resize(k-1);
        for ( unsigned int i= 0; i < data.size(); ++i )
            data(i).resize(input.dimension);

        Vector<T> tmp;
        tmp.resize(input.size());

	if ( !unknownSet )
	{
	  for ( int d= 0; d < input.dimension; ++d )
	  {
	      for ( unsigned int i= 0; i < input.size(); ++i )
		  tmp(i)= input(i)(d);

	      float minObj= FLT_MAX;
              
              #pragma omp parallel for
	      for ( int j= -1; j < multi; ++j )
	      {
		KMeansClustering<T> kmc;
		
		kmc.setup(&tmp, k);
		kmc.doEM();
		
                #pragma omp critical
                {
                    if ( kmc.getObjective() < minObj )
                    {
                        sort(kmc.means.begin(), kmc.means.end());

                        for ( unsigned int i= 0; i < kmc.means.size() - 1; ++i )
                        {
                            if ( kmc.means(i) == kmc.means(i+1) )
                                error= 1;
                            data(i)(d)= (kmc.means(i) + kmc.means(i+1))/2;
                        }
                        
                        minObj= kmc.getObjective();
                    }
                }
	      }
	  }
	}
	else
	{
	  for ( int d= 0; d < input.dimension; ++d )
	  {
	      int n= 0;
	      for ( unsigned int i= 0; i < input.size(); ++i )
		if ( input(i)(d) != unknown )
		  ++n;
	      tmp.resize(n);
	      n= 0;
	      for ( unsigned int i= 0; i < input.size(); ++i )
		  tmp(n++)= input(i)(d);

	      float minObj= FLT_MAX;
              
              #pragma omp parallel for
	      for ( int j= 0; j < multi; ++j )
	      {
		KMeansClustering<T> kmc;
		
		kmc.setup(&tmp, k);
		kmc.doEM();

                #pragma omp critical
                {
                    if ( kmc.getObjective() < minObj )
                    {
                        sort(kmc.means.begin(), kmc.means.end());

                        for ( unsigned int i= 0; i < kmc.means.size() - 1; ++i )
                            data(i)(d)= (kmc.means(i) + kmc.means(i+1))/2;
                        
                        minObj= kmc.getObjective();
                    }
                }
	      }
	  }
	}

        return 0;
    }

    template<typename T>
    int KMeansClusteringDiscretizer<T>::openDiscretizerFromFile(std::ifstream& file)
    {
        std::string tmp;

        file >> tmp;

        if ( tmp.compare("KMeansClusteringDiscretizer") == 0 || tmp.find("KMeansClusteringDiscretizer") == 0 )
        {
            this->Discretizer<T>::openDiscretizerFromFile(file);

            return 0;
        }
        return 1;
    }

    template<typename T>
    int KMeansClusteringDiscretizer<T>::saveDiscretizerToFile(std::ofstream& file)
    {
        file << "KMeansClusteringDiscretizer" << std::endl;
        this->Discretizer<T>::saveDiscretizerToFile(file);

        return 0;
    }
}

#endif	/* KMEANSCLUSTERINGDISCRETIZER_H */

