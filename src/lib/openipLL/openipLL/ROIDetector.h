/**
 * @file ROIDetector.h
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
 * Some ROI detection methods.
 */

#ifndef _ROIDETECTOR_H
#define	_ROIDETECTOR_H

#include <openipDS/Image.h>

namespace openip
{
    /**
     * simple roi detection for fundus images
     * @param input input image
     * @param output roi
     */
    void roiDetectionSimple(Image<unsigned char>* input, Image<unsigned char>* output);
}

#endif	/* _ROIDETECTOR_H */

