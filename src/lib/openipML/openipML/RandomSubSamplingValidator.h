/**
 * @file RandomSubSamplingValidator.h
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
 * RandomSubSamplingValidator represents the random subsampling validation method.
 */

#ifndef ValidatorRANDOMSUBSAMPLING_H
#define	ValidatorRANDOMSUBSAMPLING_H

#include <stdlib.h>
#include <time.h>

#include <openipML/Validator.h>

namespace openip
{
    /**
     * RandomSubSamplingValidator represents the random subsampling validation method.
     */
    template<typename T>
    class RandomSubSamplingValidator: public Validator<T>
    {
    public:
        using Validator<T>::m;
        using Validator<T>::error;
        using Validator<T>::dumpResults;
        using Validator<T>::measure;
        using Validator<T>::evaluate;

        /**
         * constructor
         * @param numberOfExperiments number of experiments
         * @param ratioOfTestCases ratio of test cases
         */
        RandomSubSamplingValidator(int numberOfExperiments= 10, float ratioOfTestCases= 0.1, typename Validator<T>::validator_measure measure= Validator<T>::VALIDATOR_ACC);

        /**
         * copy constructor
         * @param r instance to copy
         */
        RandomSubSamplingValidator(const RandomSubSamplingValidator& r);

        /**
         * destructor
         */
        ~RandomSubSamplingValidator();

        /**
         * do the validation
         * @param c classifier to validate
         * @param d dataset to validate with
         * @return accuracy of classification
         */
        virtual float validate(Classifier<T>* c, DataSet<T>& d);

        /** number of experiments */
        int numberOfExperiments;
        /** ratio of test cases */
        float ratioOfTestCases;
    };

    template<typename T>
    RandomSubSamplingValidator<T>::RandomSubSamplingValidator(int numberOfExperiments, float ratioOfTestCases, typename Validator<T>::validator_measure measure)
    : Validator<T>(measure)
    {
        this->numberOfExperiments= numberOfExperiments;
        this->ratioOfTestCases= ratioOfTestCases;
    }

    template<typename T>
    RandomSubSamplingValidator<T>::RandomSubSamplingValidator(const RandomSubSamplingValidator& r)
    : Validator<T>(r)
    {
        this->numberOfExperiments= r.numberOfExperiments;
        this->ratioOfTestCases= r.ratioOfTestCases;
    }

    template<typename T>
    RandomSubSamplingValidator<T>::~RandomSubSamplingValidator()
    {
    }

    template<typename T>
    float RandomSubSamplingValidator<T>::validate(Classifier<T>* c, DataSet<T>& d)
    {
        d.init();
        m.resize(d.numberOfClasses, d.numberOfClasses);
        m= 0;

        int numberOfTestCases= d.size() * ratioOfTestCases;
        Vector<unsigned char> mask(d.size());

        srand(time(NULL));

        int j, k;
        float error= 0;

        for ( int i= 0; i < numberOfExperiments; ++i )
        {
            mask= 1;

            j= numberOfTestCases;
            while ( j > 0 )
            {
                k= rand() % d.size();
                if ( mask(k) != 0 )
                {
                    mask(k)= 0;
                    --j;
                }
            }

            c->train(d, &mask);

            int tmp;
            for ( unsigned int j= 0; j < mask.size(); ++j )
            {
                if ( mask(j) == 0 )
                {
                    tmp= c->classify(d(j));
                    if ( tmp != d(j).classIndex )
                        error= error + 1.0f;
                    m(d(j).classIndex, tmp)+= 1.0f;
                }
            }
        }

        error/= (numberOfTestCases * numberOfExperiments);
        return evaluate();
    }
}

#endif	/* ValidatorRANDOMSUBSAMPLING_H */

