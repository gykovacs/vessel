/* 
 * File:   ActiveContour.h
 * Author: gykovacs
 *
 * Created on January 4, 2011, 12:50 PM
 */

#ifndef ACTIVECONTOUR_H
#define	ACTIVECONTOUR_H

#include <openipLL/ActiveContourElement.h>
#include <vector>

namespace openip
{
    class ActiveContour: public std::vector<ActiveContourElement>
    {
    public:
        ActiveContour();

        ActiveContour(const ActiveContour& ac);

        ~ActiveContour();

        ActiveContourElement& operator()(int i);

        ActiveContourElement operator()(int i) const;

        void add(int row, int column, float alpha= 1, float beta= 1, float gamma= 1);

        float energyInternal();
    };
}

#endif	/* ACTIVECONTOUR_H */

