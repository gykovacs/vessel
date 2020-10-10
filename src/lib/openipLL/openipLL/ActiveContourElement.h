/* 
 * File:   SnakeElement.h
 * Author: gykovacs
 *
 * Created on 2011. január 4., 22:02
 */

#ifndef ACTIVECONTOURELEMENT_H
#define	ACTIVECONTOURELEMENT_H

#include <openipDS/Pixel.h>

namespace openip
{
    class ActiveContourElement: public Pixel2
    {
    public:
        ActiveContourElement();

        ActiveContourElement(const ActiveContourElement& ace);

        ActiveContourElement(int r, int c, float alpha= 1, float beta= 1, float gamma= 1);

        ~ActiveContourElement();

        float alpha;
        float beta;
        float gamma;
        float curv;
    };
}

#endif	/* SNAKEELEMENT_H */

