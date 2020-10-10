/**
 * @file VisualWordSet.h
 * @author Janos Istvan Ivan <dzsekob@gmail.com>
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
 * The VisualWordSet is a set of VisualWords represented as a filter set.
 */

#ifndef VISUALWORDSET_H
#define	VISUALWORDSET_H

#include <openipDS/VisualWord.h>
#include <openipDS/FilterSet2.h>

namespace openip
{
    /**
     * VisualWordSet is a set of VisualWords represented as a filter set.
     */
    template<typename INPUT, typename OUTPUT>
    class VisualWordSet: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::apply;
        using FilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using FilterSet2<INPUT, OUTPUT, float>::getMin;
        using FilterSet2<INPUT, OUTPUT, float>::getMax;
        using FilterSet2<INPUT, OUTPUT, float>::updateStride;
        using FilterSet2<INPUT, OUTPUT, float>::getProposedBorder;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::push_back;
        using FilterSet2<INPUT, OUTPUT, float>::resize;

        /**
         * default constructor
         */
        VisualWordSet();

        /**
         * constructor
         * @param s descriptor string (filename)
         */
        VisualWordSet(std::string s);

        /**
         * copy constructor
         * @param v instance to copy
         */
        VisualWordSet(const VisualWordSet& v);

        /**
         * destructor
         */
        ~VisualWordSet();

        /**
         * returns the descriptor string
         * @return descriptor string
         */
        virtual std::string getDescriptor(char separator= ' ', int i= 0);
    };

    template<typename INPUT, typename OUTPUT>
    VisualWordSet<INPUT, OUTPUT>::VisualWordSet()
    : FilterSet2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    VisualWordSet<INPUT, OUTPUT>::VisualWordSet(const VisualWordSet& v)
    : FilterSet2<INPUT, OUTPUT, float>(v)
    {
    }

    template<typename INPUT, typename OUTPUT>
    VisualWordSet<INPUT, OUTPUT>::VisualWordSet(std::string s)
    {
        stringstream ss(s);
        std::string tmp;
        while ( !ss.eof() )
        {
            getline(ss, tmp);
            if ( tmp.size() > 0 )
                this->push_back(new CorrelationFilter2<INPUT, OUTPUT, float>(VisualWordSquare<INPUT, OUTPUT>(tmp)));
        }
        this->computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    VisualWordSet<INPUT, OUTPUT>::~VisualWordSet()
    {
    }

    template<typename INPUT, typename OUTPUT>
    std::string VisualWordSet<INPUT, OUTPUT>::getDescriptor(char, int)
    {
        stringstream ss;
        for ( unsigned int i= 0; i < this->size(); ++i )
        {
            ss << (*this)[i]->descriptor;
            ss << endl;
        }

        return ss.str();
    }
}

#endif	/* VISUALWORDSET_H */

