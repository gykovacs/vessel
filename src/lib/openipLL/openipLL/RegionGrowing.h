#ifndef REGIONGROWING_H
#define REGIONGROWING_H

#include <openipDS/Image.h>

#include <openipML/Classifier.h>
#include <openipML/SimulatedAnnealing.h>

#include <openipLL/LowLevelTransforms.h>

template<typename INPUT>
class RegionGrowing
{
public:
    RegionGrowing();

    RegionGrowing(const RegionGrowing& r);

    virtual ~RegionGrowing();

    virtual void init();

    virtual void doRG(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>& seed, Image<unsigned char>* roi= NULL);
};

template<typename INPUT>
RegionGrowing<INPUT>::RegionGrowing()
{
}

template<typename INPUT>
RegionGrowing<INPUT>::RegionGrowing(const RegionGrowing& /*r*/)
{
}

template<typename INPUT>
RegionGrowing<INPUT>::~RegionGrowing()
{
}

template<typename INPUT>
void RegionGrowing<INPUT>::init()
{
}

template<typename INPUT>
void RegionGrowing<INPUT>::doRG(Image<INPUT> &/*input*/, Image<unsigned char> &/*output*/, Image<unsigned char> &/*seed*/, Image<unsigned char> */*roi*/)
{
}

#endif // REGIONGROWING_H
