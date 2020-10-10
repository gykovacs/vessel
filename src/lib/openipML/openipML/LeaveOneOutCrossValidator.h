/**
 * @file LeaveOneOutCrossValidator.h
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
 * LeaveOneOutCrossValidator represents the leave one out cross validation method.
 */

#ifndef LEAVEONEOUTCROSSVALIDATION_H
#define	LEAVEONEOUTCROSSVALIDATION_H

#include <stdlib.h>
#include <time.h>

#include <openipML/Validator.h>

namespace openip
{
    /**
     * LeaveOneOutCrossValidator represents the leave one out cross validation method.
     */
    template<typename T>
    class LeaveOneOutCrossValidator: public Validator<T>
    {
    public:
        using Validator<T>::m;
        using Validator<T>::error;
        using Validator<T>::dumpResults;
        using Validator<T>::measure;
        using Validator<T>::evaluate;
	using Validator<T>::meanSquaredError;
	using Validator<T>::r2;
	using Validator<T>::sstot;
	using Validator<T>::ssreg;
	using Validator<T>::ssres;

        /**
         * constructor
         */
        LeaveOneOutCrossValidator(typename Validator<T>::validator_measure measure= Validator<T>::VALIDATOR_ACC);

        /**
         * copy constructor
         * @param r instance to copy
         */
        LeaveOneOutCrossValidator(const LeaveOneOutCrossValidator& r);

        /**
         * destructor
         */
        ~LeaveOneOutCrossValidator();

        /**
         * do the validation
         * @param c classifier to validate
         * @param d dataset to validate with
         * @return accuracy of classification
         */
        virtual float validate(Classifier<T>* c, DataSet<T>& d);
	
	virtual float validate(Regression<T>* r, RegressionDataSet<T>& data);
    };

    template<typename T>
    LeaveOneOutCrossValidator<T>::LeaveOneOutCrossValidator(typename Validator<T>::validator_measure measure)
    : Validator<T>(measure)
    {
    }

    template<typename T>
    LeaveOneOutCrossValidator<T>::LeaveOneOutCrossValidator(const LeaveOneOutCrossValidator& r)
    : Validator<T>(r)
    {
    }

    template<typename T>
    LeaveOneOutCrossValidator<T>::~LeaveOneOutCrossValidator()
    {
    }

    template<typename T>
    float LeaveOneOutCrossValidator<T>::validate(Classifier<T>* c, DataSet<T>& data)
    {
        //tprintf("data.init()\n");
        data.init();
        //tprintf("data.init() finished\n");
        m.resize(data.numberOfClasses, data.numberOfClasses);
        m= 0;

        if ( data(0).size() == 0 )
            return 1.0f;

        Vector<unsigned char> mask(data.size());
        srand(time(NULL));

        int tmp;
        error= 0.0f;

        mask= 1;
        for ( unsigned int i= 0; i < data.size(); ++i )
        {
            mask(i)= 0;
            //tprintf("training...\n");
            c->train(data, &mask);
            //tprintf("training finished...\n");

            //tprintf("classify...\n");
            tmp= c->classify(data(i));
            //tprintf("classify fininshed\n");
            if ( data(i).classIndex != tmp )
                error+= 1.0f;
            m(data(i).classIndex, tmp)+= 1.0;

            mask(i)= 1;
        }

        error/= data.size();
        //tprintf("evaluation\n");
        return evaluate();
    }
    
    template<typename T>
    float LeaveOneOutCrossValidator<T>::validate(Regression<T>* r, RegressionDataSet<T>& data)
    {
	meanSquaredError= 0;
	sstot= 0;
	ssreg= 0;
	ssres= 0;
	
	if ( data(0).size() == 0 )
            return 1.0f;

        data.init();

        Vector<unsigned char> mask(data.size());
        srand(time(NULL));

        mask= 1;

        float tmp;

        int nn= 0;
	float ymean= 0;
	for ( unsigned int i= 0; i < data.size(); ++i )
	  ymean+= data(i).targetValue;
	ymean/= data.size();
	
        for ( unsigned int i= 0; i < data.size(); ++i )
        {
	  tprintf("LOOCV: %d/%d %f\n", i, data.size(), meanSquaredError/(i+0.0001));
	    mask(i)= 0;
	    
	    ymean= 0;
	    for ( unsigned int j= 0; j < data.size(); ++j )
	      if ( i != j )
		ymean+= data(j).targetValue;
	    ymean/= (data.size()-1);
	    //printf("ymean: %f\n", ymean);
	    
            r->train(data, &mask);

	    tmp= r->regress(data(i), NULL);
	    meanSquaredError+= (tmp - data(i).targetValue)*(tmp - data(i).targetValue);
	    sstot+= (data(i).targetValue - ymean)*(data(i).targetValue - ymean);
	    ssreg+= (tmp - ymean)*(tmp - ymean);
	    ssres+= (tmp - data(i).targetValue)*(tmp - data(i).targetValue);
	    
	    printf("%f,%f ", tmp, data(i).targetValue); fflush(stdout);
	    ++nn;

            mask(i)= 1;
        }

        meanSquaredError/= nn;
	r2= 1 - ssres/sstot;
	
        return evaluate();
    }
}

#endif	/* LEAVEONEOUTCROSSVALIDATION_H */

