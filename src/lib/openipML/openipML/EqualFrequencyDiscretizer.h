/**
 * @file EqualFrequencyDiscretizer.h
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
 * EqualFrequencyDiscretizer represents the equal frequency discretization method.
 */

#ifndef EQUALFREQUENCYDiscretizer_H
#define	EQUALFREQUENCYDiscretizer_H

#include <algorithm>

#include <openipDS/Histogram.h>

#include <openipML/Discretizer.h>

namespace openip
{
    /**
     * EqualFrequencyDiscretizer represents the equal frequency discretization method.
     */
    template<typename T>
    class EqualFrequencyDiscretizer: public Discretizer<T>
    {
    public:
        using Discretizer<T>::openDiscretizerFromFile;
        using Discretizer<T>::saveDiscretizerToFile;
        using Discretizer<T>::discretize;
        using Discretizer<T>::data;
        using Discretizer<T>::k;
	using Discretizer<T>::unknown;
	using Discretizer<T>::unknownSet;

        /**
         * constructor
         * @param k number of discrete objects
         */
        EqualFrequencyDiscretizer(int k= 10, int unknown= 28);

        /**
         * copy constructor
         * @param d instance to copy
         */
        EqualFrequencyDiscretizer(const EqualFrequencyDiscretizer& d);

        /**
         * destructor
         */
        ~EqualFrequencyDiscretizer();

        /**
         * initializes the discretizer
         * @param input this dataset is used to initialize the discretizer
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int init(DataSet<T>& input);

        /**
         * opens the discretizer from file
         * @param file input file stream
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int openDiscretizerFromFile(std::ifstream& file);

        /**
         * saves the discretizer to file
         * @param file output file stream
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int saveDiscretizerToFile(std::ofstream& file);

    };

    template<typename T>
    EqualFrequencyDiscretizer<T>::EqualFrequencyDiscretizer(int k, int unknown)
    : Discretizer<T>(unknown)
    {
        this->k= k;
        data.resize(k-1);
    }

    template<typename T>
    EqualFrequencyDiscretizer<T>::EqualFrequencyDiscretizer(const EqualFrequencyDiscretizer& d)
    : Discretizer<T>(d)
    {
        this->k= d.k;
        data= d.data;
    }

    template<typename T>
    EqualFrequencyDiscretizer<T>::~EqualFrequencyDiscretizer()
    {
    }

    template<typename T>
    int compare (const void* i,const void* j) { return ( *(T*)i< *(T*)j ); }

    template<typename T>
    int EqualFrequencyDiscretizer<T>::init(DataSet<T>& input)
    {
        //printf("init begin\n"); fflush(stdout);
        input.init();

        int totalNumber= input.size();

        vector<T> tmpData;
        tmpData.resize(totalNumber);

        //printf("%d %d\n", k, totalNumber); fflush(stdout);
        data.resize(k-1);
        for ( int i= 0; i < k-1; ++i )
            data(i).resize(input.dimension);//= Vector<T>(input.dimension);

	if ( !unknownSet )
	{
	  for ( int d= 0; d < input.dimension; ++d )
	  {
	      for ( unsigned int j= 0; j < input.size(); ++j )
		      tmpData[j]= input(j)(d);

	      //printf("sort\n");fflush(stdout);
	      std::stable_sort(tmpData.begin(), tmpData.end());
	      //qsort((T*)(&(tmpData[0])), tmpData.size(), sizeof(T), compare<T>);
	      //printf("end sort\n"); fflush(stdout);

	      for ( int kk= 0; kk < k-1; ++kk )
		  data(kk)(d)= tmpData[(kk+1)*double(totalNumber)/k/* < totalNumber ? (kk+1)*totalNumber/k : (kk+1)*totalNumber/k-1*/];
              
              printf("borders %d:\n", k);
              for ( unsigned int i= 0; i < data.size(); ++i )
                  printf("%f\n", data(i)(0));
              printf("borders finished\n");

	  }
	}
	else
	{
	  
	  for ( int d= 0; d < input.dimension; ++d )
	  {
	    int n= 0;
	    for ( unsigned int j= 0; j < input.size(); ++j )
	      if ( input(j)(d) != unknown )
		tmpData[n++]= input(j)(d);
	    std::stable_sort(tmpData.begin(), tmpData.begin() + n);
	    
	    int totalNumber= n;
	    for ( int kk= 0; kk < k-1; ++kk )
	      data(kk)(d)= tmpData[(kk+1)*totalNumber/k < totalNumber ? (kk+1)*totalNumber/k : (kk+1)*totalNumber/k-1];
	  }
	}
	
        //printf("init end\n"); fflush(stdout);

        return 0;
    }

    template<typename T>
    int EqualFrequencyDiscretizer<T>::openDiscretizerFromFile(std::ifstream& file)
    {
        std::string tmp;

        file >> tmp;


        if ( tmp.find("EqualFrequencyDiscretizer") == 0 )
        {
            this->Discretizer<T>::openDiscretizerFromFile(file);

            return 0;
        }
        return 1;
    }

    template<typename T>
    int EqualFrequencyDiscretizer<T>::saveDiscretizerToFile(std::ofstream& file)
    {
        file << "EqualFrequencyDiscretizer" << std::endl;
        this->Discretizer<T>::saveDiscretizerToFile(file);

        return 0;
    }
}

#endif	/* EQUALFREQUENCYDiscretizer_H */

