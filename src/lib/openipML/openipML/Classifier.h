/**
 * @file Classifier.h
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
 * Classifier is the base class of all classifier methods.
 */


#ifndef Classifier_H
#define	Classifier_H

#include <openipDS/VectorN.h>
#include <openipDS/Vector.h>

#include <openipDS/DataSet.h>
#include <openipDS/ContinuousDataSet.h>
#include <openipDS/DiscreteDataSet.h>

#include <openipDS/ContinuousDataSet.h>
#include <openipDS/DiscreteDataSet.h>
#include <openipDS/Stopper.h>

#include <openipML/Discretizer.h>
#include <openipML/EqualFrequencyDiscretizer.h>

#include <fstream>
#include <iostream>
#include <string>

/**
 * namespace of all openip objects and functions
 */
namespace openip
{
    template<typename T>
    class Validator;

    /**
     * Classifier is the base class for all classifier methods.
     */
    template<typename T>
    class Classifier
    {
    public:
        /**
         * default constructor
         */
        Classifier(T unknown= 28);

        /**
         * copy constructor
         * @param c instance to copy
         */
        Classifier(const Classifier& c);

        /**
         * destructor
         */
        virtual ~Classifier();

        /**
         * training method
         * @param data dataset to train
         * @param mask only the elements having non-zero corresponding element in the mask are trained
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int train(DataSet<T>& data, Vector<unsigned char>* mask= NULL);

        /**
         * classifying method
         * @param v vector to classify
         * @param probabilities probabilities estimates
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int classify(Vector<T>& v, Vector<float>* probabilities= NULL);

        /**
         * evaluates the classifier on the parameter dataset
         * @param data dataset to evaluate the classifier on
         * @return accuracy
         */
        virtual float evaluate(DataSet<T>& data, int measure= 6);

	virtual void init();
	
        /**
         * save model to the parameter file stream
         * @param o output file stream
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int saveModelToFile(std::ofstream& o);

        /**
         * open model from the parameter file stream
         * @param i input file stream
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int openModelFromFile(std::ifstream& i);

        /**
         * save model to the parameter file
         * @param filename output file name
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int saveModelToFile(const char* filename);

        /**
         * open model from the parameter file
         * @param filename input file name
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int openModelFromFile(const char* filename);

        /** vector for probability estimates */
        Vector<float> probabilities;
        /** class names */
        Vector<std::string> classNames;
        /** feature descriptors */
        Vector<std::string> featureNames;
        /** preprocessing steps */
        Vector<std::string> preprocessingNames;
	
	int unknown;
	int unknownSet;
    };

    template<typename T>
    Classifier<T>::Classifier(T unknown)
    {
      this->unknown= unknown;
      if ( unknown == 28 )
	unknownSet= 0;
      else
	unknownSet= 1;
    }

    template<typename T>
    Classifier<T>::Classifier(const Classifier& c)
    {
      unknown= c.unknown;
      unknownSet= c.unknownSet;
    }

    template<typename T>
    Classifier<T>::~Classifier()
    {
    }

    template<typename T>
    int Classifier<T>::train(DataSet<T>&, Vector<unsigned char>*)
    {
        return 0;
    }

    template<typename T>
    int Classifier<T>::classify(Vector<T>&, Vector<float>*)
    {
        return 0;
    }
    
    template<typename T>
    void Classifier<T>::init()
    {
    }

    template<typename T>
    float Classifier<T>::evaluate(DataSet<T>& data, int measure)
    {
        tprintf("starting evaluation in classifier\n");
        Validator<T> v(measure);
        tprintf("init dataset\n");
	
        data.init();
	
	DataSet<T> filtered;
	data.filterFeatureVectorsByFeatureNames(featureNames, filtered);
	filtered.init();
	
        tprintf("initialization finished\n");
        v.m.resize(filtered.numberOfClasses, filtered.numberOfClasses);

        tprintf("%d %d\n", filtered.numberOfClasses, (int)(filtered.size()));
        v.m= 0;
        tprintf("classification...\n");
        int label;
        for ( unsigned int i= 0; i < filtered.size(); ++i )
        {
            label= this->classify(filtered(i));
            v.m(filtered(i).classIndex, label)= v.m(filtered(i).classIndex, label ) + 1;
        }
        tprintf("classification finished\n");

        v.dumpResults();

        return v.evaluate();
    }

    template<typename T>
    int Classifier<T>::saveModelToFile(const char* filename)
    {
        std::ofstream file;
        file.open(filename);

        saveModelToFile(file);

        return 0;
    }

    template<typename T>
    int Classifier<T>::openModelFromFile(const char* filename)
    {
        std::ifstream file;
        file.open(filename);

        openModelFromFile(file);

        return 0;
    }

    template<typename T>
    int Classifier<T>::saveModelToFile(std::ofstream& file)
    {
        file << classNames.size() << std::endl;
        for ( unsigned int i= 0; i < classNames.size(); ++i )
            file << classNames(i) << std::endl;
        file << featureNames.size() << std::endl;
        for ( unsigned int i= 0; i < featureNames.size(); ++i )
            file << featureNames(i) << std::endl;
        file << preprocessingNames.size() << std::endl;
        for ( unsigned int i= 0; i < preprocessingNames.size(); ++i )
            file << preprocessingNames(i) << std::endl;
	file << unknown << std::endl;
	file << unknownSet << std::endl;

        return 0;
    }

    template<typename T>
    int Classifier<T>::openModelFromFile(std::ifstream& file)
    {
        std::string tmp;
        int k;
        
        getline(file, tmp);
        sscanf(tmp.c_str(), "%d", &k);
        for ( int i= 0; i < k; ++i )
        {
            getline(file, tmp);
            classNames.push_back(tmp);
        }
        
        getline(file, tmp);
        sscanf(tmp.c_str(), "%d", &k);
        for ( int i= 0; i < k; ++i )
        {
            getline(file, tmp);
            featureNames.push_back(tmp);
        }

        getline(file, tmp);
        sscanf(tmp.c_str(), "%d", &k);
        for ( int i= 0; i < k; ++i )
        {
            getline(file, tmp);
            preprocessingNames.push_back(tmp);
        }

        getline(file, tmp);
	stringstream ss0(tmp);
        ss0 >> unknown;
	getline(file, tmp);
	stringstream ss1(tmp);
	ss1 >> unknownSet;
        
        return 0;
    }

}

#endif	/* Classifier_H */

