/* 
 * File:   DistanceFunctionIO.h
 * Author: gykovacs
 *
 * Created on 2011. február 22., 23:13
 */

#ifndef DISTANCEFUNCTIONIO_H
#define	DISTANCEFUNCTIONIO_H

#include <iostream>

#include <openipML/SimilarityFunction.h>
#include <openipML/KernelFunctions.h>
#include <openipML/CorrelationFunctions.h>
#include <openipML/DistanceFunctions.h>
#include <openipML/KernelFunctionIO.h>

#include "KernelFunctionIO.h"

namespace openip
{
    template<typename T>
    class DistanceFunctionIO
    {
    public:
        DistanceFunctionIO();

        DistanceFunctionIO(const DistanceFunctionIO& b);

        ~DistanceFunctionIO();

        int saveDistanceFunctionToFile(DistanceFunction<T>* bf, std::ofstream& file);

        DistanceFunction<T>* openDistanceFunctionFromFile(std::ifstream& file);

        DistanceFunction<T>* createDistanceFunctionFromString(std::string str);
    };

    template<typename T>
    DistanceFunctionIO<T>::DistanceFunctionIO()
    {
    }

    template<typename T>
    DistanceFunctionIO<T>::DistanceFunctionIO(const DistanceFunctionIO& b)
    {
    }

    template<typename T>
    DistanceFunctionIO<T>::~DistanceFunctionIO()
    {
    }

    template<typename T>
    int DistanceFunctionIO<T>::saveDistanceFunctionToFile(DistanceFunction<T>* df, std::ofstream& file)
    {
        if ( dynamic_cast<EuclideanDistance<T>*>(df) != NULL )
            file << "EuclideanDistance" << endl;
        else if (dynamic_cast<HausdorffDistance<T>*>(df) != NULL )
            file << "HausdorffDistance" << endl;
        else if (dynamic_cast<SquaredEuclideanDistance<T>*>(df) != NULL )
            file << "SquaredEuclideanDistance" << endl;
        else if (dynamic_cast<KernelEuclideanDistance<T>*>(df) != NULL )
        {
            file << "KernelEuclideanDistance" << endl;
            KernelEuclideanDistance<T>* ked= dynamic_cast<KernelEuclideanDistance<T>*>(df);
            KernelFunctionIO<T> kfio;
            kfio.saveKernelFunctionToFile(ked->kernel, file);
        }
        else if (dynamic_cast<KernelEuclidean2Distance<T>*>(df) != NULL )
        {
            file << "KernelEuclidean2Distance" << endl;
            KernelEuclidean2Distance<T>* ked= dynamic_cast<KernelEuclidean2Distance<T>*>(df);
            KernelFunctionIO<T> kfio;
            kfio.saveKernelFunctionToFile(ked->kernel, file);
        }
        else if ( dynamic_cast<KernelNonCenteredCorrelationDistance<T>*>(df) != NULL )
        {
            file << "KernelNonCenteredCorrelationDistance" << endl;
            KernelNonCenteredCorrelationDistance<T>* kccd= dynamic_cast<KernelNonCenteredCorrelationDistance<T>*>(df);
            KernelFunctionIO<T> kfio;
            kfio.saveKernelFunctionToFile(kccd->kernel, file);
        }
        return 0;
    }

    template<typename T>
    DistanceFunction<T>* DistanceFunctionIO<T>::openDistanceFunctionFromFile(std::ifstream& file)
    {
        std::string tmp;
        getline(file, tmp);
        cout << "dist " << tmp << std::endl;
        char distanceName[30];
        std::stringstream ss(tmp);
        ss >> distanceName;

        if ( strcmp(distanceName, "EuclideanDistance") == 0 )
            return new EuclideanDistance<T>();
        else if ( strcmp(distanceName, "HausdorffDistance") == 0 )
            return new HausdorffDistance<T>();
        else if ( strcmp(distanceName, "SquaredEuclideanDistance") == 0 )
            return new SquaredEuclideanDistance<T>();
        else if ( strcmp(distanceName, "KernelEuclideanDistance") == 0 )
        {
            KernelFunctionIO<T> kfio;
            return new KernelEuclideanDistance<T>(kfio.openKernelFunctionFromFile(file));
        }
        else if ( strcmp(distanceName, "KernelEuclidean2Distance") == 0 )
        {
            KernelFunctionIO<T> kfio;
            return new KernelEuclidean2Distance<T>(kfio.openKernelFunctionFromFile(file));
        }
        else if ( strcmp(distanceName, "KernelNonCenteredCorrelationDistance") == 0 )
        {
            KernelFunctionIO<T> kfio;
            return new KernelNonCenteredCorrelationDistance<T>(kfio.openKernelFunctionFromFile(file));
        }
        return NULL;
    }

    template<typename T>
    DistanceFunction<T>* DistanceFunctionIO<T>::createDistanceFunctionFromString(std::string str)
    {
        if ( str.find("EuclideanDistance") == 0 )
            return new EuclideanDistance<T>();
        if ( str.find("HausdorffDistance") == 0 )
            return new HausdorffDistance<T>();
        if ( str.find("SquaredEuclideanDistance") == 0 )
            return new SquaredEuclideanDistance<T>();
        if ( str.find("KernelEuclideanDistance") == 0 )
            return new KernelEuclideanDistance<T>(NULL);
	if ( str.find("KernelEuclidean2Distance") == 0 )
            return new KernelEuclidean2Distance<T>(NULL);
        if ( str.find("KernelNonCenteredCorrelationDistance") == 0 )
            return new KernelNonCenteredCorrelationDistance<T>(NULL);
        return NULL;
    }
}

#endif	/* BIVARIATEFUNCTIONIO_H */

