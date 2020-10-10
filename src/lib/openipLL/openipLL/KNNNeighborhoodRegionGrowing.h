#ifndef KNNNEIGHBORHOODREGIONGROWING_H
#define KNNNEIGHBORHOODREGIONGROWING_H

#include <openipDS/PixelSet1.h>
#include <openipDS/StructuringElement2s.h>

#include <openipML/KNNClassifier.h>

#include <openipLL/RegionGrowing.h>
#include <openipLL/Transform2Generator.h>

template<typename INPUT>
class KNNNeighborhoodRegionGrowing: public RegionGrowing<INPUT>
{
public:
    KNNNeighborhoodRegionGrowing(char* features, int r);

    KNNNeighborhoodRegionGrowing(const KNNNeighborhoodRegionGrowing& nrg);

    ~KNNNeighborhoodRegionGrowing();

    virtual void init();

    virtual void doRG(Image<INPUT>& input, Image<unsigned char>& output, Image<unsigned char>& seed, Image<unsigned char>* roi= NULL);

    char* features;
    int r;
    Transform2Chain<INPUT, INPUT>* t2chain;
    Transform2Set<INPUT, INPUT>* t2set;
};

template<typename INPUT>
KNNNeighborhoodRegionGrowing<INPUT>::KNNNeighborhoodRegionGrowing(char* features, int r)
    : RegionGrowing<INPUT>()
{
    this->features= features;
    this->r= r;
}

template<typename INPUT>
KNNNeighborhoodRegionGrowing<INPUT>::KNNNeighborhoodRegionGrowing(const KNNNeighborhoodRegionGrowing& nrg)
    : RegionGrowing<INPUT>(nrg)
{
    this->features= nrg.features;
    this->r= nrg.r;
}

template<typename INPUT>
KNNNeighborhoodRegionGrowing<INPUT>::~KNNNeighborhoodRegionGrowing()
{
}

template<typename INPUT>
void KNNNeighborhoodRegionGrowing<INPUT>::init()
{
    t2chain= (generateTransform2Chain<INPUT, INPUT>(std::string(features), std::string("preprocessing")));

    for ( unsigned int i= 0; i < t2chain->tvoo.size(); ++i )
        tprintf("preprocessing transform: %s\n", ((t2chain->tvoo(i)))->getDescriptor().c_str());

    t2set= (generateTransform2Set<INPUT, INPUT>(std::string(features), std::string("feature")));

    for ( unsigned int i= 0; i < t2set->size(); ++i )
        tprintf("feature to extract: %s\n", (t2set)->operator()(i)->getDescriptor().c_str());
}

template<typename INPUT>
void KNNNeighborhoodRegionGrowing<INPUT>::doRG(Image<INPUT> &input, Image<unsigned char> &output, Image<unsigned char> &seed, Image<unsigned char> *roi)
{
    tprintf("starting doRG\n");
    init();
    tprintf("initialized\n");
    StructuringElementDisk sed(r);
    sed.updateStride(input.columns);

    PixelSet1 seeds;
    for ( unsigned int i= 0; i < seed.n; ++i )
        if ( seed[i] )
            seeds.push_back(i);

    PixelSet1 final;
    StructuringElementSquare ses(3, seed.columns);

    output= seed;

    Image<float> tmp;

    Border2 b1= t2chain->getProposedBorder();
    Border2 b2= t2set->getProposedBorder();
//    Border2 b= maximizeBorders(b1, b2);

    ImageVector<float> iv;
    iv.resize(t2set->dimensions());
    tprintf("t2set.dimensions(): %d\n", t2set->dimensions());

    int d= t2set->dimensions();

    tprintf("preprocessing step...\n"); fflush(stdout);
    t2chain->apply(input, tmp, roi, roi);

    iv.resizeImages(tmp);
    tprintf("feature extraction step...\n"); fflush(stdout);
    tprintf("image vector dimensions: %d %d %d %d %d %d\n", iv(0).rows, iv(0).columns, iv(0).leftBorder, iv(0).rightBorder, iv(0).topBorder, iv(0).bottomBorder);

    t2set->apply(tmp, iv, roi, roi);

    tprintf("starting iteration\n");
    ContinuousFeatureVector cfv(t2set->dimensions());
    cfv.weightForeground= 1;
    cfv.weightBackground= 1;
    DataSet<float> data;
    KNNClassifier<float> knn(5);
    int label;
    while ( 1 )
    {
        final.clear();

        for ( int i= 0; i < int(output.n); ++i )
        {
            if ( !output(i) && (!roi || (*roi)(i)) )
            {
                for ( unsigned int j= 0; j < ses.size(); ++j )
                    if ( i + ses(j) >= 0 && i + ses(j) < int(output.n) )
                    {
                        data.clear();
                        for ( unsigned int k= 0; k < sed.size(); ++k )
                        {
                            if ( i + sed(k) >= 0 && i + sed(k) < int(output.n) && sed(k) != 0 )
                            {
                                for ( int l= 0; l < d; ++l )
                                {
                                    cfv(l)= iv(l)(i + sed(k));
                                }
                                cfv.classIndex= output(i + sed(k)) ? 1 : 0;
                                data.push_back(cfv);
                            }
                        }
                        for ( int l= 0; l < d; ++l )
                            cfv(l)= iv(l)(0);

                        //printf("%d ", i); fflush(stdout);
                        knn.train(data);
                        label= knn.classify(cfv);
                        if ( label )
                            final.push_back(i);
                        break;
                    }
            }
        }

        printf("final.size()= %zd\n", final.size());

        if ( final.size() == 0 )
            break;

        for ( unsigned int j= 0; j < final.size(); ++j )
            output(final(j))= 255;
   }
}

#endif // KNNNEIGHBORHOODREGIONGROWING_H
