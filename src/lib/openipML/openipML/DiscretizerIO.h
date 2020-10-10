/**
 * @file DiscretizerIO.h
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
 * DiscretizerIO is used to open discretizer objects from files.
 */

#ifndef DISCRETIZERIO_H
#define	DISCRETIZERIO_H

#include <iostream>
#include <fstream>
#include <string>

#include <openipML/Discretizer.h>
#include <openipML/EqualWidthDiscretizer.h>
#include <openipML/EqualFrequencyDiscretizer.h>
#include <openipML/KMeansClusteringDiscretizer.h>
#include <openipML/EntropyBasedDiscretizer.h>

namespace openip
{
    /**
     * DiscretizerIO is used to open discretizer object from file.
     */
    template<typename T>
    class DiscretizerIO
    {
    public:
        /**
         * constructor
         */
        DiscretizerIO();

        /**
         * copy constructor
         * @param d instance to copy
         */
        DiscretizerIO(const DiscretizerIO& d);

        /**
         * destructor
         */
        ~DiscretizerIO();

        /**
         * opens the discretizer from file
         * @param filename filename
         * @return discretizer object
         */
        Discretizer<T>* openDiscretizerFromFile(char* filename);

        /**
         * opens the discretizer from file
         * @param file filename
         * @return discretizer object
         */
        Discretizer<T>* openDiscretizerFromFile(std::ifstream& file);
    };

    template<typename T>
    DiscretizerIO<T>::DiscretizerIO()
    {
    }

    template<typename T>
    DiscretizerIO<T>::DiscretizerIO(const DiscretizerIO& d)
    {
    }

    template<typename T>
    DiscretizerIO<T>::~DiscretizerIO()
    {
    }

    template<typename T>
    Discretizer<T>* DiscretizerIO<T>::openDiscretizerFromFile(char* filename)
    {
        std::ifstream file;
        file.open(filename);

        Discretizer<T>* tmp;
        tmp= tmp->openDiscretizerFromFile(file);
        file.close();

        return tmp;
    }

    template<typename T>
    Discretizer<T>* DiscretizerIO<T>::openDiscretizerFromFile(std::ifstream& file)
    {
        std::string tmp;
        Discretizer<T>* discretizer= NULL;

        file >> tmp;

        if ( tmp.compare("EqualWidthDiscretizer") == 0  )
            discretizer= new EqualWidthDiscretizer<T>();
        else if ( tmp.compare("EqualFrequencyDiscretizer") == 0 )
            discretizer= new EqualFrequencyDiscretizer<T>();
        else if ( tmp.compare("KMeansClusteringDiscretizer") == 0 )
            discretizer= new KMeansClusteringDiscretizer<T>();
        else if ( tmp.compare("EntropyBasedDiscretizer") == 0 )
            discretizer= new EntropyBasedDiscretizer<T>();

        for ( unsigned int i= 0; i < tmp.length() + 1; ++i )
            file.unget();

        discretizer->openDiscretizerFromFile(file);

        return discretizer;
    }
}

#endif	/* DISCRETIZERIO_H */

