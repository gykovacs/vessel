/* 
 * File:   BivariateFunction.h
 * Author: gykovacs
 *
 * Created on 2011. február 21., 18:12
 */

#ifndef BIVARIATEFUNCTION_H
#define	BIVARIATEFUNCTION_H

#include <string>
#include <iostream>

#include <openipDS/Vector.h>

namespace openip
{
    template<typename T>
    class BivariateFunction
    {
    public:
        BivariateFunction();

        BivariateFunction(const BivariateFunction& b);

        ~BivariateFunction();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);

        std::string descriptor;
    };

    template<typename T>
    BivariateFunction<T>::BivariateFunction()
    {
    }

    template<typename T>
    BivariateFunction<T>::BivariateFunction(const BivariateFunction& b)
    {
        descriptor= b.descriptor;
    }

    template<typename T>
    BivariateFunction<T>::~BivariateFunction()
    {
    }

    template<typename T>
    double BivariateFunction<T>::evaluate(Vector<T>&, Vector<T>&)
    {
        return 0.0;
    }
}

#endif	/* BIVARIATEFUNCTION_H */

