/**
 * @file EqualWidthDiscretizer.h
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
 * EqualWidthDiscretizer represents the equal width discretization method.
 */

#ifndef EQUALWIDTHDiscretizer_H
#define	EQUALWIDTHDiscretizer_H

#include <openipDS/DiscreteDataSet.h>
#include <openipDS/ContinuousDataSet.h>

#include <openipML/Discretizer.h>

namespace openip
{
    /**
     * EqualWidthDiscretizer represents the equal width discretization method.
     */
    template<typename T>
    class EqualWidthDiscretizer: public Discretizer<T>
    {
    public:
        using Discretizer<T>::openDiscretizerFromFile;
        using Discretizer<T>::saveDiscretizerToFile;
        using Discretizer<T>::discretize;
        using Discretizer<T>::data;
        using Discretizer<T>::k;
        using Discretizer<T>::error;

        /**
         * constructor
         * @param k number of discrete objects
         */
        EqualWidthDiscretizer(int k= 10, T unknown= 28);

        /**
         * copy constructor
         * @param d instance to copy
         */
        EqualWidthDiscretizer(const EqualWidthDiscretizer& d);

        /**
         * destructor
         */
        ~EqualWidthDiscretizer();

        /**
         * initializes the discretizer
         * @param input the discretizer is initialized using this dataset
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int init(DataSet<T>& input);

        /**
         * opens the discretizer from file stream
         * @param file input file stream
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int openDiscretizerFromFile(std::ifstream& file);

        /**
         * saves the discretizer to file stream
         * @param file output file stream
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int saveDiscretizerToFile(std::ofstream& file);
    };

    template<typename T>
    EqualWidthDiscretizer<T>::EqualWidthDiscretizer(int k, T unknown)
    : Discretizer<T>(unknown)
    {
        this->k= k;
        this->data.resize(k);
    }

    template<typename T>
    EqualWidthDiscretizer<T>::EqualWidthDiscretizer(const EqualWidthDiscretizer& d)
    : Discretizer<T>(d)
    {
        this->k= d.k;
        this->data= d.data;
    }

    template<typename T>
    EqualWidthDiscretizer<T>::~EqualWidthDiscretizer()
    {
    }

    template<typename T>
    int EqualWidthDiscretizer<T>::init(DataSet<T>& input)
    {
        error= 0;
        input.init();

        Vector<T> min;
        Vector<T> max;

        min.resize(input.dimension);
        max.resize(input.dimension);
        data.resize(k-1);
	
        for ( int i= 0; i < k; ++i )
            data(i).resize(input.dimension);

        min= input(0);
        max= input(0);

        for ( typename DataSet<T>::iterator it= input.begin(); it != input.end(); ++it )
        {
	  if ( !input.unknownSet )
	  {
            for ( int i= 0; i < input.dimension; ++i )
            {
                if ( (*it)(i) < min(i) )
                    min(i)= (*it)(i);
                if ( (*it)(i) > max(i) )
                    max(i)= (*it)(i);
            }
	  }
	  else
	  {
	    this->unknown= input.unknown;
	    this->unknownSet= input.unknownSet;
	    for ( int i= 0; i < input.dimension; ++i )
            {
                if ( (*it)(i) != input.unknown && (*it)(i) < min(i) )
                    min(i)= (*it)(i);
                if ( (*it)(i) != input.unknown && (*it)(i) > max(i) )
                    max(i)= (*it)(i);
            }
	  }
        }
        
        //printf("min/max: %f,%f\n", float(min.getMin()), float(max.getMax()));
        
        for ( int i= 0; i < input.dimension; ++i )
            for ( int j= 0; j < k-1; ++j )
                data(j)(i)= min(i) + (j+1) * (max(i) - min(i)) / k;

        return 0;
    }

    template<typename T>
    int EqualWidthDiscretizer<T>::openDiscretizerFromFile(std::ifstream& file)
    {
        std::string tmp;

        file >> tmp;

        if ( tmp.compare("EqualWidthDiscretizer") == 0 || tmp.find("EqualWidthDiscretizer") == 0 )
        {
            this->Discretizer<T>::openDiscretizerFromFile(file);

            return 0;
        }
        return 1;
    }

    template<typename T>
    int EqualWidthDiscretizer<T>::saveDiscretizerToFile(std::ofstream& file)
    {
        file << "EqualWidthDiscretizer" << std::endl;
        this->Discretizer<T>::saveDiscretizerToFile(file);

        return 0;
    }
}

#endif	/* EQUALWIDTHDiscretizer_H */

