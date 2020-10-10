/**
 * @file Kx2CrossValidator.h
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
 * Kx2CrossValidator represents the kx2 cross validation method.
 */

#ifndef KX2CROSSVALIDATION_H
#define	KX2CROSSVALIDATION_H

#include <stdlib.h>
#include <time.h>

#include <openipML/Validator.h>

namespace openip
{
    /**
     * Kx2CrossValidator represents the kx2 cross validation method.
     */
    template<typename T>
    class Kx2CrossValidator: public Validator<T>
    {
    public:
        using Validator<T>::m;
        using Validator<T>::error;
        using Validator<T>::dumpResults;
        using Validator<T>::measure;
        using Validator<T>::evaluate;

        /**
         * constructor
         * @param k number of folds
         */
        Kx2CrossValidator(int k= 8, typename Validator<T>::validator_measure measure= Validator<T>::VALIDATOR_ACC);

        /**
         * copy constructor
         * @param r instance to copy
         */
        Kx2CrossValidator(const Kx2CrossValidator& r);

        /**
         * destructor
         */
        ~Kx2CrossValidator();

        /**
         * do the validation
         * @param c classifier to validate
         * @param d dataset to validate with
         * @return accuracy of classification
         */
        virtual float validate(Classifier<T>* c, DataSet<T>& d);

        /** number of folds */
        int k;
    };

    template<typename T>
    Kx2CrossValidator<T>::Kx2CrossValidator(int k, typename Validator<T>::validator_measure measure)
    : Validator<T>(measure)
    {
        this->k= k;
    }

    template<typename T>
    Kx2CrossValidator<T>::Kx2CrossValidator(const Kx2CrossValidator& r)
    : Validator<T>(r)
    {
        this->k= r.k;
    }

    template<typename T>
    Kx2CrossValidator<T>::~Kx2CrossValidator()
    {
    }

    template<typename T>
    float Kx2CrossValidator<T>::validate(Classifier<T>* c, DataSet<T>& data)
    {
        Vector<unsigned char> mask(data.size());
        srand(time(NULL));
        data.init();

        m.resize(data.numberOfClasses, data.numberOfClasses);
        m= 0;

        int j;
        int tmp;
        float error= 0.0f;
        for ( int e= 0; e < k; ++e )
        {
            mask= 0;
            j= data.size()/2;
            while ( j > 0 )
            {
                tmp= rand() % data.size();
                if ( mask(tmp) == 0 )
                {
                    mask(tmp)= 1;
                    --j;
                }
            }

            c->train(data, &mask);

            for ( unsigned int i= 0; i < mask.size(); ++i )
                if ( mask(i) == 0 )
                    if ( data(i).classIndex != c->classify(data(i)) )
                        error+= 1.0f;

            for ( unsigned int i= 0; i < mask.size(); ++i )
                mask(i)= 1 - mask(i);

            c->train(data, &mask);

            int tmp;
            for ( unsigned int i= 0; i < mask.size(); ++i )
                if ( mask(i) == 0 )
                {
                    tmp= c->classify(data(i));
                    if ( data(i).classIndex != tmp )
                        error+= 1.0f;
                    m(data(i).classIndex, tmp)+= 1.0f;
                }
        }

        error/= (k*data.size());
        return evaluate();
    }
}

#endif	/* KX2CROSSVALIDATION_H */

