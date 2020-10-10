/* 
 * File:   Noise.h
 * Author: gykovacs
 *
 * Created on October 14, 2011, 3:43 PM
 */

#ifndef NOISE_H
#define	NOISE_H

#include <openipDS/Vector.h>
#include <openipDS/DataSet.h>

namespace openip
{
    template<typename T>
    class Noise
    {
    public:

        Noise();

        Noise(const Noise&);

        virtual ~Noise();

        virtual void apply(T& input);

        virtual void apply(Vector<T>& input);
        
        virtual void apply(DataSet<T>& input);
    };

    template<typename T>
    Noise<T>::Noise()
    {
    }

    template<typename T>
    Noise<T>::Noise(const Noise& n)
    {
    }

    template<typename T>
    Noise<T>::~Noise()
    {
    }

    template<typename T>
    void Noise<T>::apply(T& )
    {
    }

    template<typename T>
    void Noise<T>::apply(Vector<T>& input)
    {
        for ( unsigned int i= 0; i < input.size(); ++i )
            apply(input(i));
    }
    
    template<typename T>
    void Noise<T>::apply(DataSet<T>& input)
    {
        for ( unsigned int i= 0; i < input.size(); ++i )
            apply(input(i));
    }
}

#endif	/* NOISE_H */

