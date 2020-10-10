/**
 * @file DiscreteClassifier.h
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
 * ContinuousNaiveBayesClassifier represents the continuous naive Bayes
 * classification method.
 */

#ifndef DISCRETECLASSIFIER_H
#define	DISCRETECLASSIFIER_H

#include <iostream>
#include <fstream>
#include <string>

#include <openipML/Classifier.h>
#include <openipML/DiscretizerIO.h>

namespace openip
{
    /**
     * DiscreteClassifier is the base class of all histogram based classification methods.
     */
    template<typename T>
    class DiscreteClassifier: public Classifier<T>
    {
    public:
        using Classifier<T>::train;
        using Classifier<T>::classify;
        using Classifier<T>::openModelFromFile;
        using Classifier<T>::saveModelToFile;
        using Classifier<T>::probabilities;
        using Classifier<T>::classNames;
        using Classifier<T>::featureNames;
	using Classifier<T>::unknown;
	using Classifier<T>::unknownSet;

        /**
         * default constructor
         */
        DiscreteClassifier(T unknown);

        /**
         * copy constructor
         * @param d instance to copy
         */
        DiscreteClassifier(const DiscreteClassifier& d);

        /**
         * destructor
         */
        ~DiscreteClassifier();

        /**
         * saves the model to file stream
         * @param file output file stream
         * @return non-zero in case of errors, zero otherwise
         */
        int saveModelToFile(std::ofstream& file);

        /**
         * opens the model from file stream
         * @param file input file stream
         * @return non-zero in case of errors, zero otherwise
         */
        int openModelFromFile(std::ifstream& file);

        /**
         * saves the model to file
         * @param filename output file name
         * @return non-zero in case of errors, zero otherwise
         */
        int saveModelToFile(char* filename);

        /**
         * opens the model from file
         * @param filename input file name
         * @return non-zero in case of errors, zero otherwise
         */
        int openModelFromFile(char* filename);

        /** discrete dataset */
        DataSet<int> data;

    public:
        /**
         * transforms the attributes to one range
         */
        void transformAttributesToOneRange();

        /**
         * transforms the parameter vector to one range
         * @param v vector to transform into one attribute range
         */
        void transformAttributesToOneRange(Vector<int>& v);

        /** cumulative range */
        Vector<int> cumulativeRange;
        /** size of transformed attribute range */
        int transformedAttributeRange;
        /** discretizer object pointer */
        Discretizer<T>* discretizer;
    };

    template<typename T>
    DiscreteClassifier<T>::DiscreteClassifier(T unknown)
    : Classifier<T>(unknown)
    {
    }

    template<typename T>
    DiscreteClassifier<T>::DiscreteClassifier(const DiscreteClassifier& d)
    : Classifier<T>(d)
    {
    }

    template<typename T>
    DiscreteClassifier<T>::~DiscreteClassifier()
    {
    }

    template<typename T>
    void DiscreteClassifier<T>::transformAttributesToOneRange()
    {
        data.init();
        cumulativeRange= data.cumulativeRange;
        transformedAttributeRange= data.transformedAttributeRange;
        for ( DataSet<int>::iterator dit= data.begin(); dit != data.end(); ++dit )
        {
            transformAttributesToOneRange(*dit);
        }
    }

    template<typename T>
    void DiscreteClassifier<T>::transformAttributesToOneRange(Vector<int>& v)
    {
      if ( !unknownSet )
      {
        v+= cumulativeRange;

        for ( unsigned int i= 0; i < v.size() - 1; ++i )
        {
            if ( v(i) < cumulativeRange(i) )
                v(i)= cumulativeRange(i);
            if ( v(i) >= cumulativeRange(i+1) )
                v(i)= cumulativeRange(i+1) - 1;
        }
        if ( v(v.size() - 1) >= transformedAttributeRange )
            v(v.size() - 1) = transformedAttributeRange - 1;
      }
      else
      {
	for ( unsigned int i= 0; i < v.size(); ++i )
	  if ( v(i) != unknown )
	    v(i)+= cumulativeRange(i);

        for ( unsigned int i= 0; i < v.size() - 1; ++i )
        {
	  if ( v(i) != unknown )
	  {
            if ( v(i) < cumulativeRange(i) )
                v(i)= cumulativeRange(i);
            if ( v(i) >= cumulativeRange(i+1) )
                v(i)= cumulativeRange(i+1) - 1;
	  }
        }
        if ( v(v.size() - 1) != unknown && v(v.size() - 1) >= transformedAttributeRange )
            v(v.size() - 1) = transformedAttributeRange - 1;
      }
    }

    template<typename T>
    int DiscreteClassifier<T>::saveModelToFile(std::ofstream& file)
    {
        this->Classifier<T>::saveModelToFile(file);

        file << cumulativeRange << std::endl;
        file << transformedAttributeRange << std::endl;

        discretizer->saveDiscretizerToFile(file);

        return 0;
    }

    template<typename T>
    int DiscreteClassifier<T>::openModelFromFile(std::ifstream& file)
    {
        DiscretizerIO<T> dio;
        this->Classifier<T>::openModelFromFile(file);

	cout << "aaa" << endl;
        file >> cumulativeRange;
	cout << "bbb" << endl;
        file >> transformedAttributeRange;

	cout << "ddd" << endl;
        discretizer= dio.openDiscretizerFromFile(file);
	cout << "eee" << endl;

        return 0;
    }

    template<typename T>
    int DiscreteClassifier<T>::saveModelToFile(char* filename)
    {
        std::ofstream file;
        file.open(filename);
        this->saveModelToFile(file);
        file.close();

        return 0;
    }

    template<typename T>
    int DiscreteClassifier<T>::openModelFromFile(char* filename)
    {
        std::ifstream file;
        file.open(filename);
        this->openModelFromFile(file);
        file.close();

        return 0;
    }
}

#endif	/* DISCRETECLASSIFIER_H */

