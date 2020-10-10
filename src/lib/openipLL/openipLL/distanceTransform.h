#ifndef _DISTANCE_TRANSFORM_
#define _DISTANCE_TRANSFORM_

#include <stdio.h>
#include <math.h>

#include <openipDS/Image.h>
#include <openipDS/constants.h>
#include <openipDS/Transform2.h>

#define DEBUG 1
#define PROGRESS 1

namespace openip
{
    void distanceLinearTime(Image<unsigned char>& input, Image<float>& output, Image<unsigned char>* roi= NULL);

    class DistanceTransform: public Transform2<unsigned char, float>
    {
    public:
        DistanceTransform();

        DistanceTransform(const DistanceTransform& dt);

        ~DistanceTransform();

        virtual void apply(Image<unsigned char>& input, Image<float>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
    };
}

#endif
