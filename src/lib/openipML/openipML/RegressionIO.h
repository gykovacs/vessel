/**
 * @file RegressionIO.h
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
 * ClassifierIO is used to open classifier model files.
 */

#ifndef REGRESSIONIO_H
#define	REGRESSIONIO_H

#include <iostream>
#include <fstream>
#include <string>

#include <openipML/Regression.h>
#include <openipML/SequentialSVRegression.h>
#include <openipML/KernelKNNRegression.h>
#include <openipML/UnivariateLinearRegression.h>
#include <openipML/MultivariateLinearRegression.h>
#include <openipML/PartialLeastSquaresRegression.h>
#include <openipML/EpsilonSVRegression.h>
#include <openipML/NeuralNetworkRegression.h>
#include <openipML/L2IsotonicRegression.h>

namespace openip
{
    /**
     * RegressionIO is used to open classifier model files.
     */
    template<typename T>
    class RegressionIO
    {
    public:
        /**
         * default constructor
         */
        RegressionIO();

        /**
         * copy constructor
         * @param d instance to copy
         */
        RegressionIO(const RegressionIO& d);

        /**
         * destructor
         */
        ~RegressionIO();

        /**
         * opens a classifier from file
         * @param filename filename
         * @return classifier object
         */
        Regression<T>* openRegressionFromFile(char* filename);

        Regression<T>* openRegressionFromFile(std::string& filename);

        /**
         * opens a classifier from file stream
         * @param file input filestream
         * @return classifier object
         */
        Regression<T>* openRegressionFromFile(std::ifstream& file);

        Regression<T>* createRegressionFromString(std::string str);

        string filename;
    };

    template<typename T>
    RegressionIO<T>::RegressionIO()
    {
    }

    template<typename T>
    RegressionIO<T>::RegressionIO(const RegressionIO& d)
    {
    }

    template<typename T>
    RegressionIO<T>::~RegressionIO()
    {
    }

    template<typename T>
    Regression<T>* RegressionIO<T>::openRegressionFromFile(char* filename)
    {
        this->filename= std::string(filename);

        std::ifstream file;
        file.open(filename);

        Regression<T>* tmp;
        tmp= this->openRegressionFromFile(file);
        file.close();

        return tmp;
    }

    template<typename T>
    Regression<T>* RegressionIO<T>::openRegressionFromFile(std::string& filename)
    {
      printf("9"); fflush(stdout);
        this->filename= filename;
        std::ifstream file;
        file.open(filename.c_str());

	printf("1"); fflush(stdout);
        Regression<T>* tmp;
        tmp= this->openRegressionFromFile(file);
        file.close();
	printf("2"); fflush(stdout);
        return tmp;
    }

    template<typename T>
    Regression<T>* RegressionIO<T>::openRegressionFromFile(std::ifstream& file)
    {
        std::string tmp;
        Regression<T>* regression= NULL;

        file >> tmp;

        if ( tmp.find("SequentialSVRegression") == 0  )
            regression= new SequentialSVRegression<T>(0, 0, 0, NULL);
	if ( tmp.find("EpsilonSVRegression") == 0  )
            regression= new EpsilonSVRegression<T>();
	if ( tmp.find("NeuralNetworkRegression") == 0  )
	{
	  Vector<unsigned int> nn;
          regression= new NeuralNetworkRegression<T>(nn, NN_ACTIVATION_SIGMOID, NN_TRAIN_QUICKPROP, 1000);
	}
	if ( tmp.find("KernelKNNRegression") == 0  )
            regression= new KernelKNNRegression<T>(1, NULL, false, 0);
	if ( tmp.find("UnivariateLinearRegression") == 0  )
            regression= new UnivariateLinearRegression<T>();
	if ( tmp.find("MultivariateLinearRegression") == 0  )
            regression= new MultivariateLinearRegression<T>();
	if ( tmp.find("PartialLeastSquaresRegression") == 0  )
            regression= new PartialLeastSquaresRegression<T>(1);
	if ( tmp.find("L2IsotonicRegression") == 0 )
	    regression= new L2IsotonicRegression<T>();
	
        for ( unsigned int i= 0; i < tmp.length(); ++i )
            file.unget();

        regression->openModelFromFile(file);

        return regression;
    }

    template<typename T>
    Regression<T>* RegressionIO<T>::createRegressionFromString(std::string str)
    {
        stringstream ss;
        ss.str(str);
        string name;
        ss >> name;

        if ( name.find("SequentialSVRegression") == 0 )
        {
	  return NULL;
        }
	return NULL;
    }
}

#endif	/* CLASSIFIERIO_H */

