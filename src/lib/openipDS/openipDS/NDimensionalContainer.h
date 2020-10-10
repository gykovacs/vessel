/**
 * @file NDimensionalContainer.h
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
 * The NDimensionalContainer represents a container with arbitrary dimensions.
 */

#ifndef NDIMENSIONALCONTAINER_H
#define	NDIMENSIONALCONTAINER_H

#include <openipDS/Vector.h>

namespace openip
{
    /**
     * NDimensionalContainer represents a container with arbitrary dimensions.
     */
    template<typename T>
    class NDimensionalContainer: public Vector<T>
    {
    public:
        using Vector<T>::operator();
        using Vector<T>::resize;

        /**
         * default constructor
         */
        NDimensionalContainer();

        /**
         * constructor
         * @param dimensions this vector contains the size of dimensions of the container
         */
        NDimensionalContainer(Vector<int>& dimensions);

        /**
         * copy constructor
         * @param n instance to copy
         */
        NDimensionalContainer(const NDimensionalContainer& n);

        /**
         * destructor
         */
        ~NDimensionalContainer();

        /**
         * access operator
         * @param index index of the element to access
         * @return the element
         */
        T& operator()(Vector<int>& index);

        /**
         * access operator
         * @param index index of the element to access
         * @return the element
         */
        T operator()(Vector<int>& index) const;

        /**
         * resizes the container
         * @param dimensions new sizes of dimensions
         */
        void resize(Vector<int>& dimensions);

        /**
         * dimensions of the container
         */
        Vector<int> dimensions;

        /**
         * sizes of the dimensions
         */
        Vector<int> sizes;
    };

    /**
     * prints the description of the parameter NDimensionalContainer object to the parameter output stream
     * @param o parameter output stream
     * @param n NDimensionalContainer object to describe
     * @return reference to the parameter output stream
     */
    template<typename T>
    std::ostream& operator<<(std::ostream& o, const NDimensionalContainer<T>& n);

    /**
     * prints the description of the parameter NDimensionalContainer object to the parameter output file stream
     * @param o parameter output file stream
     * @param v NDimensionalContainer object to describe
     * @return reference to the parameter output stream
     */
    template<typename T>
    std::ifstream& operator>>(std::ifstream& file, NDimensionalContainer<T>& n);

    /**
     * fills the parameter NDimensionalContainer from the parameter input file stream
     * @param i parameter input file stream
     * @param v NDimensionalContainer object to fill from the parameter input file stream
     * @return reference to the parameter input file stream
     */
    template<typename T>
    std::ofstream& operator<<(std::ofstream& file, NDimensionalContainer<T>& n);

    template<typename T>
    NDimensionalContainer<T>::NDimensionalContainer()
    : Vector<T>()
    {
    }

    template<typename T>
    NDimensionalContainer<T>::NDimensionalContainer(Vector<int>& dimensions)
    : Vector<T>()
    {
        resize(dimensions);
    }

    template<typename T>
    NDimensionalContainer<T>::NDimensionalContainer(const NDimensionalContainer& n)
    : Vector<T>(n)
    {
        this->dimensions= n.dimensions;
    }

    template<typename T>
    NDimensionalContainer<T>::~NDimensionalContainer()
    {

    }

    template<typename T>
    T& NDimensionalContainer<T>::operator()(Vector<int>& index)
    {
        int idx= 0;

        for ( unsigned int i= 0; i < index.size(); ++i )
            idx+= index(i)*sizes(i);

        return (*this)(idx);
    }

    template<typename T>
    T NDimensionalContainer<T>::operator()(Vector<int>& index) const
    {
        int idx= 0;

        for ( int i= 0; i < index.size(); ++i )
            idx+= index(i)*sizes(i);

        return (*this)(idx);
    }

    template<typename T>
    void NDimensionalContainer<T>::resize(Vector<int>& dimensions)
    {
        int size= 1;

        for ( Vector<int>::iterator it= dimensions.begin(); it != dimensions.end(); ++it )
            size*= *it;

        this->Vector<T>::resize(size);
        this->dimensions= dimensions;
        sizes.resize(dimensions.size());

        sizes(0)= 1;

        for ( unsigned int i= 1; i < dimensions.size(); ++i )
        {
            sizes(i)= sizes(i-1) * dimensions(i);
        }
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& o, const NDimensionalContainer<T>& v)
    {
        o << "[NDimensionalContainer - size: ";
        o << v.size();
        o << " :";

        if ( v.size() < 100 )
        {
            for ( typename NDimensionalContainer<T>::const_iterator it= v.begin(); it != v.end(); ++it )
            {
                o << " ";
                o << *it;
            }
        }

        o << "]";

        return o;
    }

    template<typename T>
    std::ofstream& operator<<(std::ofstream& file, NDimensionalContainer<T>& n)
    {
        file << n.dimensions << std::endl;
        file << n.sizes << std::endl;
        file << n.size() << std::endl;
        for ( unsigned int i= 0; i < n.size(); ++i )
            file << n(i) << " ";
        file << std::endl;

        return file;
    }

    template<typename T>
    std::ifstream& operator>>(std::ifstream& file, NDimensionalContainer<T>& n)
    {
        Vector<int> dimensions;
        Vector<int> sizes;
        file >> dimensions;
        file >> sizes;

        n.dimensions= dimensions;
        n.sizes= sizes;
        int size;
        file >> size;
        n.resize(size);

        for ( unsigned int i= 0; i < n.size(); ++i )
            file >> n(i);

        return file;
    }
}

#endif	/* NDIMENSIONALCONTAINER_H */

