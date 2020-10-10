/**
 * @file Mask2s.h
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
 * Some common 2 dimensional masks are defined in the file.
 */

#ifndef _MASK2S_H
#define	_MASK2S_H

#include <openipDS/Image.h>
#include <openipDS/Mask.h>

namespace openip
{
    /**
     * simple test mask
     */
    template<typename INPUT, typename OUTPUT>
    class TestMask: public Mask<INPUT, OUTPUT, unsigned char>
    {

    public:

        using Mask<INPUT, OUTPUT, unsigned char>::apply;
        using Mask<INPUT, OUTPUT, unsigned char>::match;

        /**
         * test mask constructor
         */
        TestMask();
    };

    template<typename INPUT, typename OUTPUT>
    TestMask<INPUT, OUTPUT>::TestMask()
    : Mask<INPUT, OUTPUT, unsigned char>()
    {
        this->insert(PositionWeightPair<unsigned char>(-2, 255));
        this->insert(PositionWeightPair<unsigned char>(-1, 255));
        this->insert(PositionWeightPair<unsigned char>(1, 255));
        this->insert(PositionWeightPair<unsigned char>(2, 255));
        this->insert(PositionWeightPair<unsigned char>(0, 0));
        this->computeMinMax();
    }
}

#endif	/* _MASK2S_H */

