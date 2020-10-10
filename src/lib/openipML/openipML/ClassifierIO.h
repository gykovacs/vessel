/**
 * @file ClassifierIO.h
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

#ifndef CLASSIFIERIO_H
#define	CLASSIFIERIO_H

#include <iostream>
#include <fstream>
#include <string>

#include <openipML/Discretizer.h>
#include <openipML/EqualWidthDiscretizer.h>
#include <openipML/EqualFrequencyDiscretizer.h>
#include <openipML/Classifier.h>
#include <openipML/KNNClassifier.h>
#include <openipML/DiscreteNaiveBayesClassifier.h>
#include <openipML/ContinuousNaiveBayesClassifier.h>
#include <openipML/DiscreteMultivariateBayesClassifier.h>
#include <openipML/EpsilonSVMClassifier.h>
#include <openipML/NuSVMClassifier.h>
#include <openipML/AODEClassifier.h>
#include <openipML/KernelKNNClassifier.h>
#include <openipML/DecisionTreeClassifier.h>
#include <openipML/DiscreteMultivariateBayesClassifier.h>
#include <openipML/BaggedClassifier.h>
#include <openipML/EnsembleClassifier.h>
#include <openipML/NeuralNetworkClassifier.h>
#include <openipML/CascadedNeuralNetworkClassifier.h>
#include <openipML/RandomForestTreeClassifier.h>
#include <openipML/RandomForestClassifier.h>
#include <openipML/GeneralDecisionTreeClassifier.h>
#include <openipML/GeneralizedRandomForestTreeClassifier.h>
#include <openipML/BinaryMajorityClassifier.h>

namespace openip
{
    /**
     * ClassifierIO is used to open classifier model files.
     */
    template<typename T>
    class ClassifierIO
    {
    public:
        /**
         * default constructor
         */
        ClassifierIO();

        /**
         * copy constructor
         * @param d instance to copy
         */
        ClassifierIO(const ClassifierIO& d);

        /**
         * destructor
         */
        ~ClassifierIO();

        /**
         * opens a classifier from file
         * @param filename filename
         * @return classifier object
         */
        Classifier<T>* openClassifierFromFile(char* filename);

        Classifier<T>* openClassifierFromFile(std::string& filename);

        /**
         * opens a classifier from file stream
         * @param file input filestream
         * @return classifier object
         */
        Classifier<T>* openClassifierFromFile(std::ifstream& file);

        Classifier<T>* createClassifierFromString(std::string str);

        string filename;
    };

    template<typename T>
    ClassifierIO<T>::ClassifierIO()
    {
    }

    template<typename T>
    ClassifierIO<T>::ClassifierIO(const ClassifierIO& d)
    {
    }

    template<typename T>
    ClassifierIO<T>::~ClassifierIO()
    {
    }

    template<typename T>
    Classifier<T>* ClassifierIO<T>::openClassifierFromFile(char* filename)
    {
        this->filename= std::string(filename);

        std::ifstream file;
        file.open(filename);

        Classifier<T>* tmp;
        tmp= this->openClassifierFromFile(file);
        file.close();

        return tmp;
    }

    template<typename T>
    Classifier<T>* ClassifierIO<T>::openClassifierFromFile(std::string& filename)
    {
        this->filename= filename;
        std::ifstream file;
        file.open(filename.c_str());

        Classifier<T>* tmp;
        tmp= this->openClassifierFromFile(file);
        file.close();

        return tmp;
    }

    template<typename T>
    Classifier<T>* ClassifierIO<T>::openClassifierFromFile(std::ifstream& file)
    {
        std::string tmp;
        Classifier<T>* classifier= NULL;

        file >> tmp;
	cout << "magic string: " << tmp << endl;

        if ( tmp.find("KNNClassifier") == 0  )
            classifier= new KNNClassifier<T>(1);
        else if ( tmp.find("EpsilonSVMClassifier") == 0 )
            classifier= new EpsilonSVMClassifier<T>();
        else if ( tmp.find("NuSVMClassifier") == 0 )
            classifier= new NuSVMClassifier<T>();
        else if ( tmp.find("DiscreteNaiveBayesClassifier") == 0 )
            classifier= new DiscreteNaiveBayesClassifier<T>();
        else if ( tmp.find("ContinuousNaiveBayesClassifier") == 0 )
            classifier= new ContinuousNaiveBayesClassifier<T>();
        else if ( tmp.find("AODEClassifier") == 0 )
            classifier= new AODEClassifier<T>();
        else if ( tmp.find("DiscreteMultivariateBayesClassifier") == 0 )
            classifier= new DiscreteMultivariateBayesClassifier<T>();
        else if ( tmp.find("KernelKNNClassifier") == 0 )
            classifier= new KernelKNNClassifier<T>(1);
        else if ( tmp.find("DecisionTreeClassifier") == 0 )
            classifier= new DecisionTreeClassifier<T>();
        else if ( tmp.find("BaggedClassifier") == 0 )
            classifier= new BaggedClassifier<T>();
        else if ( tmp.find("EnsembleClassifier") == 0 )
            classifier= new EnsembleClassifier<T>(1, filename);
        else if ( tmp.find("NeuralNetworkClassifier") == 0 )
            classifier= new NeuralNetworkClassifier<T>();
        else if ( tmp.find("CascadedNeuralNetworkClassifier") == 0 )
            classifier= new CascadedNeuralNetworkClassifier<T>();
        else if ( tmp.find("RandomForestTreeClassifier") == 0 )
            classifier= new RandomForestTreeClassifier<T>();
        else if ( tmp.find("RandomForestClassifier") == 0 )
            classifier= new RandomForestClassifier<T>();
        else if ( tmp.find("GeneralDecisionTreeClassifier") == 0 )
            classifier= new GeneralDecisionTreeClassifier<T>();
        else if ( tmp.find("GeneralizedRandomForestTreeClassifier") == 0 )
            classifier= new GeneralizedRandomForestTreeClassifier<T>();
        else if ( tmp.find("BinaryMajorityClassifier") == 0 )
            classifier= new BinaryMajorityClassifier<T>();

        for ( unsigned int i= 0; i < tmp.length(); ++i )
            file.unget();

        classifier->openModelFromFile(file);

        return classifier;
    }

    template<typename T>
    Classifier<T>* ClassifierIO<T>::createClassifierFromString(std::string str)
    {
        KernelFunctionIO<T> kfio;
        DistanceFunctionIO<T> dfio;
        stringstream ss;
        ss.str(str);
        string name;
        ss >> name;

        if ( name.find("KernelKNNClassifier") == 0 )
        {
            int k;
            ss >> k;
            string distance;
            string kernel;
            ss >> distance;
            ss >> kernel;
            tprintf("create knn classifier: %s\nk: %d\ndistance: %s\nkernel: %s\n", name.c_str(), k, distance.c_str(), kernel.c_str());
            DistanceFunction<T>* df= dfio.createDistanceFunctionFromString(distance);
            KernelFunction<T>* kf= kfio.createKernelFunctionFromString(kernel);
            if ( distance.find("KernelEuclideanDistance") == 0 )
            {
                tprintf("setting kernel function\n");
                dynamic_cast<KernelEuclideanDistance<T>*>(df)->kernel= kf;
            }
            if ( distance.find("KernelNonCenteredCorrelationDistance") == 0 )
                dynamic_cast<KernelNonCenteredCorrelationDistance<T>*>(df)->kernel= kf;
            return new KernelKNNClassifier<T>(k, df);
        }
        if ( name.find("EpsilonSVMClassifier") == 0 )
        {
            string kernel;
            ss >> kernel;
            KernelFunctionIO<double> kfdio;
            tprintf("create esvm classifier: %s\nkernel: %s\n", name.c_str(), kernel.c_str());
            openipKernel= kfdio.createKernelFunctionFromString(kernel);
            return new EpsilonSVMClassifier<T>(OWN, 1, 1, 0.1, 1, 100, true, openipKernel);
        }
        if ( name.find("DiscreteNaiveBayesClassifier") == 0 )
        {
            string discretizer;
            ss >> discretizer;
            int n;
            ss >> n;
            tprintf("create dnbc classifier: %s\nn: %d\ndiscretizer: %s\n", name.c_str(), n, discretizer.c_str());
            if ( (discretizer.find("ewd")) != discretizer.size() )
                return new DiscreteNaiveBayesClassifier<T>(1.0, new EqualWidthDiscretizer<T>(n));
            if ( (discretizer.find("efd")) != discretizer.size() )
                return new DiscreteNaiveBayesClassifier<T>(1.0, new EqualFrequencyDiscretizer<T>(n));
            if ( (discretizer.find("kmcd")) != discretizer.size() )
                return new DiscreteNaiveBayesClassifier<T>(1.0, new KMeansClusteringDiscretizer<T>(n));
            if ( (discretizer.find("ebd")) != discretizer.size() )
                return new DiscreteNaiveBayesClassifier<T>(1.0, new EntropyBasedDiscretizer<T>(n));
        }
        if ( name.find("AODEClassifier") == 0 )
        {
            string discretizer;
            ss >> discretizer;
            int n;
            ss >> n;
            tprintf("create aode classifier: %s\nn: %d\ndiscretizer: %s\n", name.c_str(), n, discretizer.c_str());
            if ( discretizer.find("ewd") != discretizer.size() )
                return new AODEClassifier<T>(1.0, new EqualWidthDiscretizer<T>(n));
            if ( discretizer.find("efd") != discretizer.size() )
                return new AODEClassifier<T>(1.0, new EqualFrequencyDiscretizer<T>(n));
            if ( discretizer.find("kmcd") != discretizer.size() )
                return new AODEClassifier<T>(1.0, new KMeansClusteringDiscretizer<T>(n));
            if ( discretizer.find("ebd") != discretizer.size() )
                return new AODEClassifier<T>(1.0, new EntropyBasedDiscretizer<T>(n));
        }
        if ( name.find("ContinuousNaiveBayesClassifier") == 0 )
        {
            tprintf("create cnbc classifier\n");
            return new ContinuousNaiveBayesClassifier<T>();
        }
        if ( name.find("NeuralNetworkClassifier") == 0 )
        {
            string activation;
            string learning;
            ss >> activation;
            ss >> learning;
            int act;
            int learn;
            int n;
            ss >> n;
            if ( activation.find("sigmoid") == 0 )
                act= NN_ACTIVATION_SIGMOID;
            if ( activation.find("sigsymm") == 0 )
                act= NN_ACTIVATION_SIGMOID_SYMMETRIC;
            if ( activation.find("gauss") == 0 )
                act= NN_ACTIVATION_GAUSSIAN;
            if ( activation.find("sinsymm") == 0 )
                act= NN_ACTIVATION_SIN_SYMMETRIC;

            if ( learning.find("qprop") == 0 )
                learn= NN_TRAIN_QUICKPROP;
            if ( learning.find("rprop") == 0 )
                learn= NN_TRAIN_RPROP;

            tprintf("create nn classifier: %s\nn: %d\nactivation: %s\nlearning: %s\n", name.c_str(), n, activation.c_str(), learning.c_str());
            Vector<unsigned int> nl;
            return new NeuralNetworkClassifier<T>(nl, (openip::NNActivationFunction)act, (openip::NNTrainingAlgorithm)learn, n);
        }
        if ( name.find("BinaryMajorityClassifier") == 0 )
        {
            return new BinaryMajorityClassifier<T>();
        }
        return NULL;
    }
}

#endif	/* CLASSIFIERIO_H */

