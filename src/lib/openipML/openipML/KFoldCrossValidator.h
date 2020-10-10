/**
 * @file KFoldCrossValidator.h
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
 * KFoldCrossValidator represents the k-fold cross validation method.
 */

#ifndef KFOLDCROSSValidator_H
#define	KFOLDCROSSValidator_H

#include <stdlib.h>
#include <time.h>
#include <omp.h>

#include <openipML/Validator.h>
#include <openipDS/Progress.h>

namespace openip
{
    /**
     * KFoldCrossValidator represents the k-fold cross validation method.
     */
    template<typename T>
    class KFoldCrossValidator: public Validator<T>
    {
    public:
        using Validator<T>::m;
        using Validator<T>::error;
        using Validator<T>::dumpResults;
        using Validator<T>::measure;
        using Validator<T>::evaluate;
        using Validator<T>::validator_measure;
	using Validator<T>::meanSquaredError;
	using Validator<T>::sstot;
	using Validator<T>::ssreg;
	using Validator<T>::ssres;
	using Validator<T>::r2;

        /**
         * constructor
         * @param k number of folds
         */
        KFoldCrossValidator(int k= 4, typename Validator<T>::validator_measure measure= Validator<T>::VALIDATOR_F1);

        /**
         * copy constructor
         * @param r instance to copy
         */
        KFoldCrossValidator(const KFoldCrossValidator& r);

        /**
         * destructor
         */
        ~KFoldCrossValidator();

        /**
         * do the validation
         * @param c classifier to validate
         * @param d dataset to validate with
         * @return accuracy of classification
         */
        virtual float validate(Classifier<T>* c, DataSet<T>& d);
	
	virtual float validate(Regression<T>* r, RegressionDataSet<T>& data);

        /** number of folds */
        int k;
    };

    template<typename T>
    KFoldCrossValidator<T>::KFoldCrossValidator(int k, typename Validator<T>::validator_measure measure)
    : Validator<T>(measure)
    {
        this->k= k;
	srand(time(NULL));
    }

    template<typename T>
    KFoldCrossValidator<T>::KFoldCrossValidator(const KFoldCrossValidator& r)
    : Validator<T>(r)
    {
        this->k= r.k;
    }

    template<typename T>
    KFoldCrossValidator<T>::~KFoldCrossValidator()
    {
    }

    template<typename T>
    float KFoldCrossValidator<T>::validate(Classifier<T>* c, DataSet<T>& data)
    {
        if ( data(0).size() == 0 )
            return 1.0f;

        data.init();

        m.resize(data.numberOfClasses, data.numberOfClasses);
        m= 0;

        Vector<unsigned char> mask(data.size());
        srand(time(NULL));

        int foldSize= data.size()/k;
        mask= 1;

        int j;
        int tmp;

        for ( int i= 2; i <= k; ++i )
        {
            j= foldSize;
            while( j > 0 )
            {
               tmp= rand() * rand() * rand() % mask.size();
               if ( mask(tmp) == 1 )
               {
                   mask(tmp)= i;
                   --j;
               }
            }
        }

        Vector<Vector<int> > s;
        s.resize(k+1);
        for ( int i= 0; i <= k; ++i )
            s[i].resize(data.numberOfClasses);

        for ( int i= 0; i <= k; ++i )
            s[i]= 0;
        for ( unsigned int i= 0; i < data.size(); ++i )
            if ( mask(i) > 0 )
            {
                s[mask(i)][data(i).classIndex]++;
            }

        int nn= 0;
        for ( int i= 1; i <= k; ++i )
        {
            if ( k != 1 )
                for ( unsigned int j= 0; j < mask.size(); ++j )
                    if ( mask(j) == i )
                        mask(j)= 0;

            c->train(data, &mask);

            Vector<float> probabilities(data.classNames.size());

            if ( data(0).weightForeground != 0 || data(0).weightBackground != 0 )
            {
                for ( unsigned int j= 0; j < mask.size(); ++j )
                {
                    if ( mask(j) == 0 || k == 1 )
                    {
                        tmp= c->classify(data(j), &probabilities);

                        if ( data(j).classIndex != tmp )
                        {
                            if ( data(j).classIndex > 0 )
                            {
                                m(data(j).classIndex, tmp)+= 1/*data(j).weightForeground*/;
                                ++nn;
                            }
                            else
                                m(data(j).classIndex, tmp)+= 1/*data(j).weightBackground*/;
                        }
                        else
                        {
                            if ( data(j).classIndex > 0 )
                            {
                                m(data(j).classIndex, tmp)+= 1/*data(j).weightForeground*/;
                                ++nn;
                            }
                            else
                                m(data(j).classIndex, tmp)+= 1/*data(j).weightBackground*/;
                        }
                    }
                }
            }
            else
            {
                for ( unsigned int j= 0; j < mask.size(); ++j )
                {
                    if ( mask(j) == 0 || k == 1 )
                    {
                        tmp= c->classify(data(j), &probabilities);
                        m(data(j).classIndex, tmp)+= 1;
                    }
                }
            }

            for ( unsigned int j= 0; j < mask.size(); ++j )
                if ( mask(j) == 0 )
                    mask(j)= i;
        }

        return evaluate();
    }
    
    template<typename T>
    float KFoldCrossValidator<T>::validate(Regression<T>* r, RegressionDataSet<T>& data)
    {
	meanSquaredError= 0;
	sstot= 0;
	ssreg= 0;
	ssres= 0;
	
	if ( data(0).size() == 0 )
            return 1.0f;

        data.init();

        Vector<unsigned char> mask(data.size());
        

        int foldSize= data.size()/k;
        mask= 1;

        int j;
        float tmp;

        for ( int i= 2; i <= k; ++i )
        {
            j= foldSize;
            while( j > 0 )
            {
               tmp= rand() * rand() * rand() % mask.size();
               if ( mask(tmp) == 1 )
               {
                   mask(tmp)= i;
                   --j;
               }
            }
        }

        int nn= 0;
	float ymean= 0;
        for ( int i= 1; i <= k; ++i )
        {
	    ymean= 0;
            if ( k != 1 )
                for ( unsigned int j= 0; j < mask.size(); ++j )
                    if ( mask(j) == i )
                        mask(j)= 0;
	    /*for ( int k= 0; k < mask.size(); ++k )
	      printf("%d ", mask(k));
	    printf("\n");*/

            r->train(data, &mask);

	    int nm= 0;
	    for ( unsigned int j= 0; j < mask.size(); ++j )
	      if ( mask(j) == 0 || k == 1 )
	      {
		ymean+= data(i).targetValue;
		++nm;
	      }
	    ymean/= nm;
	      
	    
	    for ( unsigned int j= 0; j < mask.size(); ++j )
	    {
		if ( mask(j) == 0 || k == 1 )
		{
		    tmp= r->regress(data(j), NULL);
		    
		    meanSquaredError+= (tmp - data(j).targetValue)*(tmp - data(j).targetValue);
		    sstot+= (data(j).targetValue - ymean)*(data(j).targetValue - ymean);
		    ssreg+= (tmp - ymean)*(tmp - ymean);
		    ssres+= (tmp - data(j).targetValue)*(tmp - data(j).targetValue);
		    
		    printf("%f,%f ", tmp, data(j).targetValue); fflush(stdout);
		    ++nn;
		}
	    }

            for ( unsigned int j= 0; j < mask.size(); ++j )
                if ( mask(j) == 0 )
                    mask(j)= i;
        }

        meanSquaredError/= nn;
	r2= 1 - ssres/sstot;
        
        return evaluate();
    }
}

#endif	/* KFOLDCROSSValidator_H */

