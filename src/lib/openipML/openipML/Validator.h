/**
 * @file Validator.h
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
 * Validator is the base class of all validation methods.
 */

#ifndef Validator_H
#define	Validator_H

#include <openipDS/DataSet.h>
#include <openipML/Classifier.h>
#include <openipDS/Stopper.h>
#include <openipDS/RegressionDataSet.h>
#include <openipML/Regression.h>

namespace openip
{
    /**
     * Validator is the base class of all validation methods.
     */
    template<typename T>
    class Validator
    {
    public:
        typedef enum validator_measure { VALIDATOR_ACC= 1, VALIDATOR_PREC, VALIDATOR_SENS, VALIDATOR_SPEC, VALIDATOR_BALACC, VALIDATOR_F1, VALIDATOR_PPV, VALIDATOR_NPV, VALIDATOR_MSE, VALIDATOR_AMS } validator_measure;
        /**
         * constructor
         */
        Validator(int measure= VALIDATOR_F1);

        /**
         * copy constructor
         * @param v instance to copy
         */
        Validator(const Validator& v);

        /**
         * destructor
         */
        virtual ~Validator();

        /**
         * do the validation
         * @param c classifier to validate
         * @param d dataset to validate with
         * @return accuracy of classification
         */
        virtual float validate(Classifier<T>* c, DataSet<T>& d);
	
	virtual float validate(Regression<T>* c, RegressionDataSet<T>& d);

        virtual float evaluate();

        /**
         * dumps the detailed results to the standard output
         */
        virtual void dumpResults();
        
        virtual void dumpResultsInLine();
	
	virtual void dumpResultsRegression();
        
        virtual void dumpResultsInLineRegression();

        /** matrix of misclassifications */
        Matrix<double> m;
        
        virtual double accuracy();
        virtual double precision();
        virtual double sensitivity();
        virtual double specificity();
        virtual double balancedAccuracy();
        virtual double fScore(float beta);
        virtual double truePositive();
        virtual double trueNegative();
        virtual double trueClassified();
        virtual double falseClassified();
        virtual double falsePositive();
        virtual double falseNegative();
        virtual double positivePredictiveValue();
        virtual double negativePredictiveValue();
        virtual double f1Score();
        virtual double falsePositiveRate();
        virtual double falseDiscoveryRate();
	virtual double approximateMedianSignificance();
	
	virtual double mse();
        
        /** error */
        double error;
	double meanSquaredError;
	double sstot;
	double ssreg;
	double ssres;
	double r2;

        int measure;
    };

    template<typename T>
    Validator<T>::Validator(int measure)
    {
        this->measure= measure;
    }

    template<typename T>
    Validator<T>::Validator(const Validator& v)
    {
        this->measure= v.measure;
    }

    template<typename T>
    Validator<T>::~Validator()
    {
    }

    template<typename T>
    float Validator<T>::validate(Classifier<T>*, DataSet<T>&)
    {
        return 0.0f;
    }
    
    template<typename T>
    float Validator<T>::validate(Regression<T>*, RegressionDataSet<T>&)
    {
      return 0.0f;
    }

    template<typename T>
    float Validator<T>::evaluate()
    {
        switch( measure )
        {
            case VALIDATOR_ACC:
                return accuracy();
            case VALIDATOR_PREC:
                return precision();
            case VALIDATOR_SPEC:
                return specificity();
            case VALIDATOR_SENS:
                return sensitivity();
            case VALIDATOR_BALACC:
                return balancedAccuracy();
            case VALIDATOR_PPV:
                return positivePredictiveValue();
            case VALIDATOR_NPV:
                return negativePredictiveValue();
            case VALIDATOR_F1:
                return f1Score();
	    case VALIDATOR_MSE:
		return mse();
	    case VALIDATOR_AMS:
	        return approximateMedianSignificance();
        }
        return 0;
    }

    template<typename T>
    void Validator<T>::dumpResults()
    {
        Vector<float> v(m.rows);

        v= 0;

        for ( int i= 0; i < m.rows; ++i )
            for ( int j= 0; j < m.columns; ++j )
            {
                v(i)+= m(i,j);
            }

        cout << "overall accuracy: " << accuracy() << endl;

        cout << "labels g\\p\t";
        for ( int i= 0; i < m.rows; ++i )
            printf("%12d", i);
        printf("\n");
        for ( int i= 0; i < m.rows; ++i )
        {
            printf("%d\t", i);
            for ( int j= 0; j < m.columns; ++j )
            {
                if ( v(i) > 0 )
                    printf("%.0f (%.2f%%)\t", m(i,j), m(i,j)/v(i)*100);
                else
                    printf("0 (0.00%%)\t");
            }
            printf("\n");
        }
        
        if ( m.rows == 2 && m.columns == 2 )
        {
            std::cout << "accuracy: " << accuracy() << std::endl;
            std::cout << "precision: " << precision() << std::endl;
            std::cout << "sensitivity: " << sensitivity() << std::endl;
            std::cout << "specificity: " << specificity() << std::endl;
            std::cout << "positivePredictiveValue: " << positivePredictiveValue() << std::endl;
            std::cout << "negativePredictiveValue: " << negativePredictiveValue() << std::endl;
            std::cout << "balancedAccuracy: " << balancedAccuracy() << std::endl;
            std::cout << "f1Score: " << f1Score() << std::endl;
	    std::cout << "approximateMedianSignificance: " << approximateMedianSignificance() << std::endl;
        }
    }
    
    template<typename T>
    void Validator<T>::dumpResultsRegression()
    {
      cout << "mse: " << meanSquaredError << endl;
      cout << "sstot: " << sstot << endl;
      cout << "ssreg: " << ssreg << endl;
      cout << "ssres: " << ssres << endl;
      cout << "r^2: " << r2 << endl;
    }
    
    template<typename T>
    void Validator<T>::dumpResultsInLineRegression()
    {
        tprintf("mse: %f sstot: %f ssreg: %f ssres: %f r^2: %f\n", meanSquaredError, sstot, ssreg, ssres, r2);
    }
    
    template<typename T>
    void Validator<T>::dumpResultsInLine()
    {
        tprintf("tp: %f tn: %f fp: %f fn: %f spec: %f sens: %f acc: %f prec: %f ppv: %f npv: %f fpr: %f fdr: %f f1: %f ams: %f\n", truePositive(), trueNegative(), falsePositive(), falseNegative(), specificity(), sensitivity(), accuracy(), precision(), positivePredictiveValue(), negativePredictiveValue(), falsePositiveRate(), falseDiscoveryRate(), f1Score(), approximateMedianSignificance());
    }
    
    template<typename T>
    double Validator<T>::accuracy()
    {
      float acc= (trueClassified())/(trueClassified() + falseClassified());
      return acc;
    }
    
    template<typename T>
    double Validator<T>::precision()
    {
        return truePositive() / (truePositive() + falsePositive());
    }
    
    template<typename T>
    double Validator<T>::sensitivity()
    {
        return truePositive() / (truePositive() + falseNegative());
    }
    
    template<typename T>
    double Validator<T>::specificity()
    {
        return trueNegative() / (trueNegative() + falsePositive());
    }
    
    template<typename T>
    double Validator<T>::positivePredictiveValue()
    {
        return truePositive() / (truePositive() + falsePositive());
    }
    
    template<typename T>
    double Validator<T>::negativePredictiveValue()
    {
        return trueNegative() / (trueNegative() + falseNegative());
    }
    
    template<typename T>
    double Validator<T>::balancedAccuracy()
    {
        return (specificity() + sensitivity()) / 2.0;
    }
    
    template<typename T>
    double Validator<T>::fScore(float beta)
    {
        return ((1 + beta*beta) * truePositive()) / ((1 + beta*beta) * truePositive() + beta*beta*falseNegative() + falsePositive());
    }

    template<typename T>
    double Validator<T>::falsePositiveRate()
    {
        return falsePositive()/(falsePositive() + trueNegative());
    }

    template<typename T>
    double Validator<T>::falseDiscoveryRate()
    {
        return falsePositive()/(falsePositive() + truePositive());
    }
    
    template<typename T>
    double Validator<T>::f1Score()
    {
        return fScore(1);
    }
    
    template<typename T>
    double Validator<T>::truePositive()
    {
        return m(1,1);
    }
    
    template<typename T>
    double Validator<T>::trueNegative()
    {
        return m(0,0);
    }
    
    template<typename T>
    double Validator<T>::falsePositive()
    {
        return m(0,1);
    }
    
    template<typename T>
    double Validator<T>::falseNegative()
    {
        return m(1,0);
    }
    
    template<typename T>
    double Validator<T>::trueClassified()
    {
        float tmp= 0;
        for ( int i= 0; i < m.rows; ++i )
            tmp+= m(i,i);
        return tmp;
    }
    
    template<typename T>
    double Validator<T>::falseClassified()
    {
        float tmp= 0;
        for ( int i= 0; i < m.rows; ++i )
            for ( int j= 0; j < m.columns; ++j )
                if ( i != j )
                    tmp+= m(i,j);
        return tmp;
    }
    
    template<typename T>
    double Validator<T>::mse()
    {
      return meanSquaredError;
    }
    
    template<typename T>
    double Validator<T>::approximateMedianSignificance()
    {
      float s= m(1,1);
      float b= m(0,0);
      float br= 10;
      
      float tmp= (s + b + br)*log(1 + s/(b + br))-s;
      return sqrt(2*tmp);
    }
}

#endif	/* Validator_H */

