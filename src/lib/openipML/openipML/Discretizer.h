/**
 * @file Discretizer.h
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
 * Discretizer is the base class of all discretizer methods.
 */

#ifndef Discretizer_H
#define	Discretizer_H

#include <openipDS/VectorN.h>
#include <openipDS/DataSet.h>
#include <openipDS/DiscreteDataSet.h>
#include <openipDS/ContinuousDataSet.h>

#include <fstream>
#include <omp.h>

namespace openip
{
    /**
     * Discretizer is the base class of all discretizer methods.
     */
    template<typename T>
    class Discretizer
    {
    public:
        /**
         * default constructor
         */
        Discretizer(T unknown= 28);

        /**
         * copy constructor
         * @param d instance to copy
         */
        Discretizer(const Discretizer& d);

        /**
         * destructor
         */
        virtual ~Discretizer();

        /**
         * discretizes the parameter vector
         * @param input vector to discretize
         * @param output discretized vector
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int discretize(Vector<T>& input, Vector<int>& output);

        /**
         * discretizes the parameter dataset
         * @param input dataset to discretize
         * @param output discretized dataset
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int discretize(DataSet<T>& input, DataSet<int>& output);

        /**
         * initializes the discretizer
         * @param input dataset to discretize
         * @return non-zero in case of errors
         */
        virtual int init(DataSet<T>& input);

        /**
         * opens the discretizer from file
         * @param filename filename
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int openDiscretizerFromFile(char* filename);

        /**
         * saves the discretizer to file
         * @param filename filename
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int saveDiscretizerToFile(char* filename);

        /**
         * opens the discretizer from file stream
         * @param ifs input file stream
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int openDiscretizerFromFile(std::ifstream& ifs);

        /**
         * saves the discretizer to file stream
         * @param ofs output file stream
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int saveDiscretizerToFile(std::ofstream& ofs);

        /** discretization model */
        Vector<Vector<T> > data;
        /** number of discrete elements */
        int k;
	int unknown;
	int unknownSet;
        int error;
    };

    template<typename T>
    Discretizer<T>::Discretizer(T unknown)
    {
      this->unknown= unknown;
      if ( unknown == 28 )
	unknownSet= 0;
      else
	unknownSet= 1;
    }

    template<typename T>
    Discretizer<T>::Discretizer(const Discretizer& d)
    {
      this->unknown= d.unknown;
      this->unknownSet= d.unknownSet;
    }

    template<typename T>
    Discretizer<T>::~Discretizer()
    {
    }

    template<typename T>
    int Discretizer<T>::discretize(Vector<T>& input, Vector<int>& output)
    {
        output.resize(input.size());
        for ( unsigned int d= 0; d < input.size(); ++d )
        {
	    if ( unknownSet && input(d) == unknown )
	    {
	      output(d)= input(d);
	      continue;
	    }
            for ( unsigned int k= 0; k < data.size(); ++k )
            {
                if ( data(k)(d) > input(d) )
                {
                    output(d)= k;
                    break;
                }
                if ( k == data.size() - (unsigned int)1 )
                    output(d)= k + 1;
            }
        }

        return 0;
    }

    template<typename T>
    int Discretizer<T>::discretize(DataSet<T>& input, DataSet<int>& output)
    {
        init(input);
        
        output.resize(input.size());

        typename DataSet<int>::iterator outIt= output.begin();
        
	#pragma omp parallel for
        for ( unsigned int i= 0; i < input.size(); ++i )
        {
            output(i).resize(input.dimension);
            output(i).classIndex= input(i).classIndex;
            discretize(input(i), output(i));
	    output(i).weightForeground= input(i).weightForeground;
	    output(i).weightBackground= input(i).weightBackground;
        }

        return 0;
    }

    template<typename T>
    int Discretizer<T>::init(DataSet<T>&)
    {
        return 0;
    }

    template<typename T>
    int Discretizer<T>::openDiscretizerFromFile(char* filename)
    {
        std::ifstream file;
        file.open(filename);

        openDiscretizerFromFile(file);
        file.close();

        return 0;
    }

    template<typename T>
    int Discretizer<T>::saveDiscretizerToFile(char* filename)
    {
        std::ofstream file;
        file.open(filename);

        saveDiscretizerToFile(file);
        file.close();

        return 0;
    }

    template<typename T>
    int Discretizer<T>::openDiscretizerFromFile(std::ifstream& file)
    {
        file >> k;
	file >> unknown;
	file >> unknownSet;
        file >> data;

        return 0;
    }

    template<typename T>
    int Discretizer<T>::saveDiscretizerToFile(std::ofstream& file)
    {
        file << k << std::endl;
	file << unknown << std::endl;
	file << unknownSet << std::endl;
        file << data << std::endl;

        return 0;
    }
}

#endif	/* Discretizer_H */

