/**
 * @file EpsilonSVRegression.h
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
 * EpsilonSVRegression represents the epsilon - Support Vector Regression method,
 * based on libsvm.
 */

#ifndef ESVREGRESSION_H
#define	ESVREGRESSION_H

#include <openipML/svm.h>
#include <openipDS/RegressionDataSet.h>
#include <openipML/Regression.h>

using namespace svm;

namespace openip
{
    /**
     * EpsilonSVRegression represents the epsilon - Support Vector Regression method,
     * based on libsvm.
     */
    template <typename T>
    class EpsilonSVRegression: public Regression<T>
    {
    public:
        /**
         * constructor
         * @param kernelType kernel type (according to libsvm)
         * @param degree degree of polinomial and RBF kernel
         * @param gamma exponent of RBF kernel
         * @param C C constant
         * @param coef0 0th coefficient of polinomial kernel
         * @param cacheSize cache size
         * @param eps iteration tolerance
         * @param nu nu (in case of nu-SVR)
         * @param p target function insensitivity
         * @param shrinking 0/non-0: shrinking off/on
         * @param probability 0/non-0: probability estimation off/on
         */
        EpsilonSVRegression(int kernelType= RBF, int degree= 2, double gamma= 1.0/3, double C= 1, double coef0= 1, double cacheSize= 100, double eps= 0.0001, double nu= 0.1, double p= 0.001, int shrinking= 0, int probability= 0);

        /**
         * copy constructor
         * @param e instance to copy
         */
        EpsilonSVRegression(const EpsilonSVRegression& e);

        /**
         * destructor
         */
        ~EpsilonSVRegression();

        /**
         * training the regression model
         * @param r training dataset
         * @param mask only those vectors are used to train, which have non-zero corresponding element in the mask
         */
        virtual int train(RegressionDataSet<T>& r, Vector<unsigned char>* mask= NULL);

        /**
         * evaluates the regression model at v
         * @param v parameter vector
         * @return target function value
         */
        virtual float regress(Vector<T>& v);

        /** sv parameters */
        svm_parameter svparam;
        /** sv problem */
        svm_problem svproblem;
        /** sv model */
        svm_model* svmodel;

        /** input vector when evaluating the regression model */
        svm_node* input;
    };
    
    template<typename T>
    EpsilonSVRegression<T>::EpsilonSVRegression(int kernelType, int degree, double gamma, double C, double coef0, double cacheSize, double eps, double nu, double p, int shrinking, int probability)
    {
        svparam.svm_type= EPSILON_SVR;
        svparam.kernel_type= kernelType;
        svparam.degree= degree;
        svparam.gamma= gamma;
        svparam.coef0= coef0;
        svparam.cache_size= cacheSize;
        svparam.eps= eps;
        svparam.C= C;
        svparam.nr_weight= 0;
        svparam.nu= nu;
        svparam.p= p;
        svparam.shrinking= shrinking;
        svparam.probability= probability;

        printf("kernel: %d, degree: %d, gamma: %f, C: %f, coef0: %f, eps: %f, nu: %f, p: %f, shrinking: %d, probability: %d\n", kernelType, degree, gamma, C, coef0, eps, nu, p, shrinking, probability);
    }

    template<typename T>
    EpsilonSVRegression<T>::EpsilonSVRegression(const EpsilonSVRegression& e)
    {
        svparam.svm_type= e.svparam.svm_type;
        svparam.kernel_type= e.svparam.kernel_type;
        svparam.degree= e.svparam.degree;
        svparam.gamma= e.svparam.gamma;
        svparam.coef0= e.svparam.coef0;
        svparam.cache_size= e.svparam.cache_size;
        svparam.eps= e.svparam.eps;
        svparam.nu= e.svparam.nu;
        svparam.p= e.svparam.p;
        svparam.shrinking= e.svparam.shrinking;
        svparam.probability= e.svparam.probability;
    }

    template<typename T>
    EpsilonSVRegression<T>::~EpsilonSVRegression()
    {
    }

    template<typename T>
    int EpsilonSVRegression<T>::train(RegressionDataSet<T>& r, Vector<unsigned char>* mask)
    {
        int trainingSetSize= 0;
        if ( mask )
        {
            for ( unsigned int i= 0; i < mask->size(); ++i )
                if ( (*mask)(i) > 0 )
                    ++trainingSetSize;
        }
        else
            trainingSetSize= r.size();

        svproblem.l= trainingSetSize;
        svproblem.x= (svm_node**)malloc(sizeof(svm_node*)*trainingSetSize);
        int k= 0;
        for ( unsigned int i= 0; i < r.size(); ++i )
        {
            if ( !mask || (*mask)(i) > 0 )
            {
                svproblem.x[k]= (svm_node*)malloc(sizeof(svm_node)*(r(i).size()+1));
                for ( unsigned int j= 0; j < r(i).size(); ++j )
                {
                    svproblem.x[k][j].index= j;
                    svproblem.x[k][j].value= r(i)(j);
                }
                svproblem.x[k][r(i).size()].index= -1;
                ++k;
            }
        }
        svproblem.y= (double*)(malloc(sizeof(double)*r.size()));
        for ( unsigned int i= 0; i < r.size(); ++i )
            svproblem.y[i]= r(i).targetValue;

        printf("start training\n");
        svmodel= svm_train(&svproblem, &svparam);
        printf("training finished\n");

        input= (svm_node*)malloc(sizeof(svm_node)*(r(0).size()+1));
	
	printf("validate\n");
	for ( unsigned int i= 0; i < r.size(); ++i )
	  printf("%f,%f ", r(i).targetValue, regress(r(i)));
	
	return 0;
    }

    template<typename T>
    float EpsilonSVRegression<T>::regress(Vector<T>& v)
    {
        for ( unsigned int i= 0; i < v.size(); ++i )
        {
            input[i].index= i;
            input[i].value= v(i);
        }
        input[v.size()].index= -1;
        
        return (float)(svm_predict(svmodel, input));
    }
}

#endif	/* ESVREGRESSION_H */

