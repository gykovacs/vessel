/**
 * @file ContinuousNaiveBayesClassifier.h
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

#ifndef CONTINUOUSNAIVEBAYESClassifier_H
#define	CONTINUOUSNAIVEBAYESClassifier_H

#include "Classifier.h"


#include <iostream>
#include <fstream>

#include <openipDS/Vector.h>
#include <openipDS/DataSet.h>

#include <openipML/Classifier.h>
//#include <openipML/GaussianDistributionVector.h>
#include <openipML/JointDistribution.h>
#include <openipML/GaussianDistribution.h>
//#include <openipML/UnivariateGaussianDistributionSet.h>
#include <openipML/PrincipalComponentAnalysis.h>

namespace openip
{
    /**
     * ContinuousNaiveBayesClassifier represents the continuous naive Bayes
     * classification method.
     */
    template<typename T>
    class ContinuousNaiveBayesClassifier: public Classifier<T>
    {
    public:
        using Classifier<T>::probabilities;
        using Classifier<T>::saveModelToFile;
        using Classifier<T>::openModelFromFile;
	using Classifier<T>::unknown;
	using Classifier<T>::unknownSet;

        /**
         * default constructor
         */
        ContinuousNaiveBayesClassifier(int pca= 0, int unknown= 28);

        /**
         * copy constructor
         * @param c instance to copy
         */
        ContinuousNaiveBayesClassifier(const ContinuousNaiveBayesClassifier& c);

        /**
         * destructor
         */
        ~ContinuousNaiveBayesClassifier();

        /**
         * training method of the classifier
         * @param data training dataset
         * @param mask only those elements of the dataset are trained, which have a corresponding non-zero element in the mask
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int train(DataSet<T>& data, Vector<unsigned char>* mask= NULL);

        /**
         * classifies the parameter vector
         * @param c vector to classify
         * @param probabilities probability estimates
         * @return predicted class index of the vector
         */
        virtual int classify(Vector<T>& c, Vector<float>* probabilities= NULL);

        /**
         * saves the model to filestream
         * @param file output filestream
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int saveModelToFile(std::ofstream& file);

        /**
         * opens the model from filestream
         * @param file input filestream
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int openModelFromFile(std::ifstream& file);

        /** model of the classifier */
        Vector<JointDistribution<T>* > model;
	
	int pca;
	PrincipalComponentAnalysis<T> principalComponentAnalysis;
    };

    template<typename T>
    ContinuousNaiveBayesClassifier<T>::ContinuousNaiveBayesClassifier(int pca, int unknown)
    : Classifier<T>(unknown)
    {
      this->pca= pca;
    }

    template<typename T>
    ContinuousNaiveBayesClassifier<T>::ContinuousNaiveBayesClassifier(const ContinuousNaiveBayesClassifier& c)
    : Classifier<T>(c)
    {
      this->pca= c.pca;
    }

    template<typename T>
    ContinuousNaiveBayesClassifier<T>::~ContinuousNaiveBayesClassifier()
    {
    }

    template<typename T>
    int ContinuousNaiveBayesClassifier<T>::train(DataSet<T>& dataInput, Vector<unsigned char>* mask)
    {
        this->classNames= dataInput.classNames;
        this->featureNames= dataInput.featureNames;
        this->preprocessingNames= dataInput.preprocessingNames;

	DataSet<T> data;

	dataInput.unknown= unknown;
	dataInput.unknownSet= unknownSet;
	
        if ( pca )
        {
            //tprintf("applying PCA\n");
	    principalComponentAnalysis.unknown= unknown;
	    principalComponentAnalysis.unknownSet= unknownSet;
            principalComponentAnalysis.train(dataInput, pca);
            principalComponentAnalysis.transform(dataInput, data);
        }
        else
	{
	  data= dataInput;
	  data.unknown= unknown;
	  data.unknownSet= unknownSet;
	}
	
        data.init();
        probabilities.resize(data.numberOfClasses);
        model.clear();

        Vector<Vector<T> > means;
        Vector<Vector<T> > vars;
        Vector<int> number(data.numberOfClasses);
        Vector<T> tmp;
	Vector<Vector<T> > numberKnown;

        for ( int i= 0; i < data.numberOfClasses; ++i )
        {
            means.push_back(Vector<T>(data(0).size()));
            vars.push_back(Vector<T>(data(0).size()));
	    numberKnown.push_back(Vector<T>(data(0).size()));
            means(i)= 0;
            vars(i)= 0;
            number(i)= 0;
	    numberKnown(i)= 0;
        }

        if ( !unknownSet )
	{
	  for ( unsigned int i= 0; i < data.size(); ++i )
	  {
	      if ( !mask || (*mask)(i) )
	      {
		  means(data(i).classIndex)+= data(i);
		  number(data(i).classIndex)+= 1;
	      }
	  }
	  
	  for ( int i= 0; i < data.numberOfClasses; ++i )
            means(i)/= number(i);
	}
	else
	{
	  for ( unsigned int i= 0; i < data.size(); ++i )
	  {
	      if ( !mask || (*mask)(i) )
	      {
		for ( int j= 0; j < data.dimension; ++j )
		{
		  if ( data(i)(j) != unknown )
		  {
		    means(data(i).classIndex)(j)+= data(i)(j);
		    numberKnown(data(i).classIndex)(j)+= 1;
		  }
		}
	      }
	  }
	  
	  for ( int i= 0; i < data.numberOfClasses; ++i )
	    for ( int j= 0; j < data.dimension; ++j )
	      means(i)(j)/=numberKnown(i)(j);
	}

	if ( !unknownSet )
	{
	  for ( unsigned int i= 0; i < data.size(); ++i )
	  {
	      if ( !mask || (*mask)(i) )
	      {
		  tmp= means(data(i).classIndex) - data(i);
		  vars(data(i).classIndex)+= tmp*tmp;
	      }
	  }

	  for ( int i= 0; i < data.numberOfClasses; ++i )
	  {
	      vars(i)/= number(i);
	      for ( unsigned int j= 0; j < vars(i).size(); ++j )
		  vars(i)(j)= sqrt(vars(i)(j));
	  }
	}
	else
	{
	  float tmpf;
	  for ( unsigned int i= 0; i < data.size(); ++i )
	  {
	      if ( !mask || (*mask)(i) )
	      {
		for ( int j= 0; j < data.dimension; ++j )
		{
		  if ( data(i)(j) != unknown )
		  {
		    tmpf= means(data(i).classIndex)(j) - data(i)(j);
		    vars(data(i).classIndex)(j)+= tmpf*tmpf;
		  }
		}
	      }
	  }

	  for ( int i= 0; i < data.numberOfClasses; ++i )
	  {
	    for ( int k= 0; k < data.dimension; ++k )
	    {
	      vars(i)(k)/= numberKnown(i)(k);
	      vars(i)(k)= sqrt(vars(i)(k));
	    }
	  }
	}

        for ( int i= 0; i < data.numberOfClasses; ++i )
        {
            //printf("%f %f\n", means(i)(0), vars(i)(0));
            model.push_back(new UnivariateGaussianJointDistribution<T>(means(i), vars(i)));
        }

        return 0;
    }

    template<typename T>
    int ContinuousNaiveBayesClassifier<T>::classify(Vector<T>& c, Vector<float>* probabilities)
    {
        float sum= 0;
        if ( probabilities == NULL )
            probabilities= &(this->probabilities);
	if ( !unknownSet )
	{
	  for ( unsigned int i= 0; i < model.size(); ++i )
	  {
	      (*probabilities)(i)= (model)(i)->probability(c);
	      sum+= (*probabilities)(i);
	  }
	}
	else
	{
	  for ( unsigned int i= 0; i < model.size(); ++i )
	  {
	    (*probabilities)(i)= 1;
	    for ( unsigned int j= 0; j < model(i)->size(); ++j )
	    {
	      if ( c(j) != unknown )
	      {
		(*probabilities)(i)*= (model)(i)->operator()(j)->probability(c(j));
	      }
	    }
	    sum+= (*probabilities)(i);
	  }
	}

        for ( unsigned int i= 0; i < probabilities->size(); ++i )
        {
            (*probabilities)(i)/= sum;
        }
        
        int maxIdx= 0;
        for ( unsigned int i= 1; i < probabilities->size(); ++i )
            if ( (*probabilities)(i) > (*probabilities)(maxIdx) )
                maxIdx= i;
        return maxIdx;
    }

    template<typename T>
    int ContinuousNaiveBayesClassifier<T>::saveModelToFile(std::ofstream& file)
    {
        file << "ContinuousNaiveBayesClassifier" << std::endl;
        this->Classifier<T>::saveModelToFile(file);

	this->principalComponentAnalysis.saveToFile(file);
        file << pca << std::endl;
	
        file << model.size() << std::endl;
        printf("model size: %d\n", (int)(model.size())); fflush(stdout);
        for ( unsigned int i= 0; i < model.size(); ++i )
        {
            //printf("save model to file\n"); fflush(stdout);
            model(i)->saveDistributionToFile(file);
        }

        return 0;
    }

    template<typename T>
    int ContinuousNaiveBayesClassifier<T>::openModelFromFile(std::ifstream& file)
    {
        std::string tmp;

        getline(file, tmp);

        if ( tmp.compare("ContinuousNaiveBayesClassifier") == 0 )
        {
            this->Classifier<T>::openModelFromFile(file);
	    
	    this->principalComponentAnalysis.openFromFile(file);
	    file >> pca;
	    
            int k;
            file >> k;

            for ( int i= 0; i < k; ++i )
            {
                UnivariateGaussianJointDistribution<T>* ugjd= new UnivariateGaussianJointDistribution<T>();
                ugjd->openDistributionFromFile(file);
                model.push_back(ugjd);
            }

            probabilities.resize(model.size());

            return 0;
        }

        return 1;
    }
}

#endif	/* CONTINUOUSNAIVEBAYESClassifier_H */

