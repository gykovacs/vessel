/**
 * @file Feature2Set.h
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
 * The Feature2Set is used to represent an ordered set of Feature2 objects.
 */

#ifndef FEATURE2SET_H
#define	FEATURE2SET_H

#include <openipDS/Feature2Generator.h>
#include <openipDS/VectorFeature2.h>
#include <openipDS/StructuringElement2s.h>

namespace openip
{
    /**
     * Feature2Set represents an ordered set of Feature2 objects
     */
    template<typename INPUT, typename OUTPUT>
    class Feature2Set: public VectorFeature2<INPUT, OUTPUT>, public std::vector<Feature2<INPUT, OUTPUT>* >
    {
    public:
        using VectorFeature2<INPUT, OUTPUT>::apply;
        using vector<Feature2<INPUT, OUTPUT>* >::push_back;

        /**
         * default constructor
         */
        Feature2Set();

        /**
         * copy constructor
         * @param f instance to copy
         */
        Feature2Set(const Feature2Set& f);

        /**
         * constructor
         * @param fdf feature descriptor file name
         */
        Feature2Set(char* fdf);

        /**
         * destructor
         */
        ~Feature2Set();

        /**
         * updates the stride of the features
         * @param stride new stride
         * @return non-zero if the stride is changed, zero otherwise
         */
        int updateStride(int stride);

        /**
         * computes the mininum and maximum positions in row-major representation, in which
         * the each feature can be computed
         */
        void computeMinMax();

        /**
         * computes the features in a given position
         * @param input input image
         * @param n the position to compute the features in
         * @param output the output vector
         */
        virtual void apply(Image<INPUT>& input, int n, Vector<OUTPUT>& output);

        /**
         * adds a feature to the feature set
         * @param f feature to add to the set
         */
        void add(Feature2<INPUT, OUTPUT>* f);

        /**
         * adds a feature to the feature set
         * @param s descriptor string
         */
        void add(std::string& s);

        /**
         * minimum position in row-major representation, in which each feature can be computed
         */
        int min;

        /**
         * maximum position in row-major representation, in which each feature can be computed
         */
        int max;

        /**
         * Feature2Generator object
         */
        Feature2Generator<INPUT, OUTPUT> f2g;
    };

    template<typename INPUT, typename OUTPUT>
    Feature2Set<INPUT, OUTPUT>::Feature2Set()
    : VectorFeature2<INPUT, OUTPUT>(), vector<Feature2<INPUT, OUTPUT>* >()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Feature2Set<INPUT, OUTPUT>::Feature2Set(const Feature2Set& f)
    : VectorFeature2<INPUT, OUTPUT>(f), vector<Feature2<INPUT, OUTPUT>* >(f)
    {
        this->min= f.min;
        this->max= f.max;
        this->f2g= f.f2g;
    }

    template<typename INPUT, typename OUTPUT>
    Feature2Set<INPUT, OUTPUT>::Feature2Set(char* fdf)
    : VectorFeature2<INPUT, OUTPUT>(), vector<Feature2<INPUT, OUTPUT>* >()
    {
        std::ifstream file;
        file.open(fdf);

        std::string feature;
        Feature2<unsigned char, float>* tmp;

        if ( file.is_open() )
        {
            while ( ! file.eof() )
            {
                getline(file, feature);
                tmp= f2g.generate(feature);
                if ( tmp != NULL )
                    this->push_back(tmp);
            }
        }

        file.close();
    }

    template<typename INPUT, typename OUTPUT>
    Feature2Set<INPUT, OUTPUT>::~Feature2Set()
    {
    }

    template<typename INPUT, typename OUTPUT>
    int Feature2Set<INPUT, OUTPUT>::updateStride(int stride)
    {
        int tmp= 0;
        for ( int i= 0; i < this->size(); ++i )
            tmp|= (*this)[i]->updateStride(stride);

        return tmp;
    }

    template<typename INPUT, typename OUTPUT>
    void Feature2Set<INPUT, OUTPUT>::computeMinMax()
    {
        min= max= 0;
        for ( int i= 0; i < this->size(); ++i )
        {
            (*this)[i]->computeMinMax();
            if ( (*this)[i]->getMin() < min )
                min= (*this)[i]->getMin();
            if ( (*this)[i]->getMax() > max )
                max= (*this)[i]->getMax();
        }
    }

    template<typename INPUT, typename OUTPUT>
    void Feature2Set<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Vector<OUTPUT>& output)
    {
        if ( n >= -min && n < input.n - max )
        {
            output.resize(this->size());
            for ( int i= 0; i < this->size(); ++i )
                output(i)= (*this)[i]->apply(input, n);
        }
    }

    template<typename INPUT, typename OUTPUT>
    void Feature2Set<INPUT, OUTPUT>::add(Feature2<INPUT, OUTPUT>* f)
    {
        this->push_back(f);
    }

    template<typename INPUT, typename OUTPUT>
    void Feature2Set<INPUT, OUTPUT>::add(std::string& s)
    {
        this->push_back(f2g.generate(s));
    }
}

#endif	/* FEATURE2SET_H */

