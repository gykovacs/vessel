/**
 * @file RegressionDataSet.h
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
 * The RegressionDataSet class represents a set of feature vectors.
 */

#ifndef REGRESSION_DATASET_H
#define	REGRESSION_DATASET_H

#include <fstream>
#include <iostream>
#include <time.h>
#include <string.h>
#include <set>

#include <openipDS/Vector.h>
#include <openipDS/RegressionFeatureVector.h>
#include <openipDS/minilzo/minilzo.h>
#include <openipDS/DataSet.h>


namespace openip
{

    /**
     * RegressionDataSet class represents a vector of feature vector objects.
     */
    template<typename T>
    class RegressionDataSet: public Vector<RegressionFeatureVector<T> >
    {
    public:
        using Vector<RegressionFeatureVector<T> >::push_back;
        using Vector<RegressionFeatureVector<T> >::operator();
        using Vector<RegressionFeatureVector<T> >::erase;
        using Vector<RegressionFeatureVector<T> >::clear;
        
        /**
         * default constructor
         */
        RegressionDataSet();

        /**
         * copy constructor
         * @param d instance to copy
         */
        RegressionDataSet(const RegressionDataSet& d);

        /**
         * destructor
         */
        virtual ~RegressionDataSet();

        /**
         * initializes the dataset
         * @return non-zero in case of errors
         */
        virtual int init();

        /**
         * equalizes the dataset, after equalization each class will have the
         * same number of feature vectors
         * @return non-zero in case of errors
         */
        virtual int equalize();

        /**
         * subsamples the dataset to the sth size
         * @param s ratio of the size of the original and the new dataset
         * @return non-zero in case of errors
         */
        virtual int sample(float s);

        /**
         * adds a feature vector to the dataset
         * @param c the feature vector object to add
         */
        virtual void add(RegressionFeatureVector<T>& c);

        /**
         * adds a feature vector to the dataset
         * @param targetValue class index of the new vector
         * @param v the elements of the new vector
         */
        virtual void add(float targetValue, Vector<T>& v);

        /**
         * returns true is the dataset is discrete
         * @return true if the dataset is discrete, false otherwise
         */
        virtual bool isDiscrete();

        /**
         * removes features from the dataset, indexed by 1 in the mask
         * @param mask contains 1 in the position of features to remove, 0 otherwise
         * @param d the resulting dataset
         * @return non-zero in case of errors
         */
        virtual int filterFeatureVectors(Vector<int>& mask, RegressionDataSet<T>& d);

        /**
         * returns a string containing the feature descriptors, where the mask contains 1
         * @param mask contains 1 for those features that's descriptors must be returned
         * @return string containing feature descriptors
         */
        virtual string dumpFeatures(Vector<int>& mask);
	
	virtual void sortByCoordinate(int coord);
	
	virtual void getCoordinateVector(int coord, Vector<T>& v);
	
	virtual void setCoordinateVector(int coord, Vector<T>& v);

        /**
         * save to file
         * @param filename filename to save in
         * @param type storage mode
         * @return non-zero in case of errors
         */
        virtual int saveToFile(const char* filename, DataSetIOType type= DATASET_IO_TEXT);

        /**
         * open dataset from file
         * @param filename filename to open from
         * @param type storage mode
         * @return non-zero in case of errors
         */
        virtual int openFromFile(const char* filename, DataSetIOType type= DATASET_IO_TEXT);

        /**
         * save to binary file
         * @param filename filename to save in
         * @return non-zero in case of errors
         */
        virtual int saveToBinaryFile(const char* filename);

        /**
         * open from binary file
         * @param filename filename to open from
         * @return non-zero in case of errors
         */
        virtual int openFromBinaryFile(const char* filename);

        /**
         * save to text file
         * @param filename filename to save in
         * @return non-zero in case of errors
         */
        virtual int saveToTextFile(const char* filename);

        /**
         * open from text file
         * @param filename filename to open from
         * @return non-zero in case of errors
         */
        virtual int openFromTextFile(const char* filename);

        /**
         * save to lzo compressed file
         * @param filename filename to save in
         * @return non-zero in case of errors
         */
        virtual int saveToCompressedFile(const char* filename);

        /**
         * open from lzo compressed file
         * @param filename filename to open from
         * @return non-zero in case of errors
         */
        virtual int openFromCompressedFile(const char* filename);

        /**
         * the ith element contains the name of the ith feature
         */
        Vector<std::string> featureNames;

        /**
         * the ith element contains the name of the ith preprocessing step
         */
        Vector<std::string> preprocessingNames;

        /**
         * after initialization min contains the minimum values of coordinates
         */
        RegressionFeatureVector<T> min;

        /**
         * after initialization max contains the maximum values of coordinates
         */
        RegressionFeatureVector<T> max;

        /**
         * after initialization range contains the range of the feature coordinates
         */
        RegressionFeatureVector<T> range;

        /**
         * after initialization cumulativeRange contains the cumulative range of features coordinates
         */
        RegressionFeatureVector<T> cumulativeRange;

        /**
         * after initialization maxRange contains the maximum range of the coordinates
         */
        int maxRange;

        /**
         * after initialization contains the dimension of feature vectors, basicalle equals with (*this)(0).size()
         */
        int dimension;

        /**
         * after transformation to one attribute range transformedAttributeRange contains the maximum value of the range
         */
        int transformedAttributeRange;
    };

    template<typename T>
    RegressionDataSet<T>::RegressionDataSet()
    : Vector<RegressionFeatureVector<T> >()
    {
        transformedAttributeRange= -1;
    }

    template<typename T>
    RegressionDataSet<T>::RegressionDataSet(const RegressionDataSet& d)
    : Vector<RegressionFeatureVector<T> >(d)
    {
        transformedAttributeRange= d.transformedAttributeRange;
        min= d.min;
        max= d.max;
        range= d.range;
        cumulativeRange= d.cumulativeRange;
        maxRange= d.maxRange;
        dimension= d.dimension;
        featureNames= d.featureNames;
        preprocessingNames= d.preprocessingNames;
    }

    template<typename T>
    RegressionDataSet<T>::~RegressionDataSet()
    {
    }

    template<typename T>
    int RegressionDataSet<T>::init()
    {
        dimension= (*this)(0).size();

        min.resize(dimension);
        max.resize(dimension);
        range.resize(dimension);
        cumulativeRange.resize(dimension);

        min= max= (*this)(0);

        for ( typename RegressionDataSet<T>::iterator it= this->begin(); it != this->end(); ++it )
        {
            for ( int i= 0; i < dimension; ++i )
            {
                if ( (*it)(i) < min(i) )
                    min(i)= (*it)(i);
                if ( (*it)(i) > max(i) )
                    max(i)= (*it)(i);
            }
        }

        for ( int i= 0; i < dimension; ++i )
        {
            range(i)= max(i) - min(i);

            if ( i == 0 )
                cumulativeRange(0)= 0;
            else
                cumulativeRange(i)= cumulativeRange(i-1) + range(i-1) + 1;
        }

        maxRange= 0;
        for ( int i= 0; i < dimension; ++i )
            if ( maxRange < range(i) )
                maxRange= range(i);

        transformedAttributeRange= cumulativeRange(dimension-1) + range(dimension - 1) + 1;

        return 0;
    }

    template<typename T>
    int RegressionDataSet<T>::equalize()
    {   
        return 0;
    }

    template<typename T>
    int RegressionDataSet<T>::filterFeatureVectors(Vector<int>& mask, RegressionDataSet<T>& output)
    {
        output.clear();
        output.featureNames.clear();
        output.preprocessingNames.clear();
        output.preprocessingNames= this->preprocessingNames;

        int size= 0;
        for ( unsigned int i= 0; i < mask.size(); ++i )
            if ( mask(i) != 0 )
                size++;

        vector<int> hash;
        hash.resize(size);

        int j= 0;
        for ( unsigned int i= 0; i < mask.size(); ++i )
            if ( mask(i) != 0 )
                hash[j++]= i;

        for ( int i= 0; i < size; ++i )
            output.featureNames.push_back(this->featureNames[hash[i]]);

        RegressionFeatureVector<T> f(size);
        for ( unsigned int i= 0; i < this->size(); ++i )
        {
            f.targetValue= (*this)(i).targetValue;
            for ( int j= 0; j < size; ++j )
                f(j)= (*this)(i)(hash[j]);

            output.push_back(f);
        }

        output.init();

        return 0;
    }
    
    template<typename T>
    int RegressionDataSet<T>::sample(float s)
    {
        int newSize= this->size()*s;
        printf("sample dataset to size %d\n", newSize);

        RegressionDataSet<T> sampled;
        Vector<unsigned char> mask;
        mask.resize(this->size());
        mask= 0;

        int tmp;
        while ( newSize > 0 )
        {
            tmp= rand()%mask.size();
            if ( mask(tmp) == 0 )
            {
                --newSize;
                mask(tmp)= 1;
            }
        }

        for ( unsigned int i= 0; i < mask.size(); ++i )
            if ( mask(i) > (unsigned char)0 )
                sampled.push_back((*this)(i));

        sampled.featureNames= this->featureNames;
        sampled.preprocessingNames= this->preprocessingNames;

        this->clear();
        (*this)= sampled;
        this->init();

        return 0;
    }

    template<typename T>
    void RegressionDataSet<T>::add(RegressionFeatureVector<T>& c)
    {
        this->push_back(c);
    }

    template<typename T>
    void RegressionDataSet<T>::add(float targetValue, Vector<T>& v)
    {
        this->push_back(RegressionFeatureVector<T>(targetValue, v));
    }

    template<typename T>
    bool RegressionDataSet<T>::isDiscrete()
    {
        if ( typeid(this) == typeid(RegressionDataSet<unsigned int>*) )
            return true;
        if ( typeid(this) == typeid(RegressionDataSet<short>*) )
            return true;
        if ( typeid(this) == typeid(RegressionDataSet<int>*) )
            return true;
        if ( typeid(this) == typeid(RegressionDataSet<long int>*) )
            return true;
        if ( typeid(this) == typeid(RegressionDataSet<unsigned short>*) )
            return true;
        if ( typeid(this) == typeid(RegressionDataSet<long long int>*) )
            return true;
        if ( typeid(this) == typeid(RegressionDataSet<char>*) )
            return true;
        if ( typeid(this) == typeid(RegressionDataSet<unsigned char>*) )
            return true;
        return false;
    }

    template<typename T>
    int RegressionDataSet<T>::saveToFile(const char* filename, DataSetIOType type)
    {
        switch(type)
        {
            case DATASET_IO_TEXT:
                return saveToTextFile(filename);
            case DATASET_IO_BINARY:
                return saveToBinaryFile(filename);
            case DATASET_IO_COMPRESSED:
                return saveToCompressedFile(filename);
        }
        return 0;
    }

    template<typename T>
    int RegressionDataSet<T>::openFromFile(const char* filename, DataSetIOType type)
    {
        switch(type)
        {
            case DATASET_IO_TEXT:
                return openFromTextFile(filename);
            case DATASET_IO_BINARY:
                return openFromBinaryFile(filename);
            case DATASET_IO_COMPRESSED:
                return openFromCompressedFile(filename);
        }
        return 0;
    }

    template<typename T>
    int RegressionDataSet<T>::saveToTextFile(const char* filename)
    {
        std::ofstream file;
        file.open(filename);

        file << this->preprocessingNames.size() << endl;
        for ( unsigned int i= 0; i < (unsigned int)(this->preprocessingNames.size()); ++i)
            file << preprocessingNames[i] << endl;

        file << this->featureNames.size() << endl;
        for ( unsigned int i= 0; i < (unsigned int)(this->dimension); ++i )
            if ( i < featureNames.size() )
                file << featureNames[i] << endl;

        file << this->size() << endl;
        for ( unsigned int i= 0; i < this->size(); ++i )
            file << (*this)(i) << endl;
        file.close();
        return 0;
    }

    template<typename T>
    int RegressionDataSet<T>::openFromTextFile(const char* filename)
    {
        std::ifstream file;
        file.open(filename, ifstream::in);

        if ( !file.is_open() )
            return 1;

        int size;
        int dimension;
        int preprocessingSteps;

        featureNames.clear();
        preprocessingNames.clear();

        std::string stmp;

        file >> preprocessingSteps;
        file.get();
        for ( int i= 0; i < preprocessingSteps; ++i )
        {
            getline(file, stmp);
            this->preprocessingNames.push_back(stmp);
        }

        file >> dimension;
	printf("open: dimension - %d\n", dimension); fflush(stdout);
        file.get();
        for ( int i= 0; i < dimension; ++i )
        {
            getline(file, stmp);
            this->featureNames.push_back(stmp);
        }

        file >> size;
	printf("open: size - %d\n", size); fflush(stdout);
        file.get();
        this->resize(size);
        RegressionFeatureVector<T> tmp(dimension);
        for ( int i= 0; i < size; ++i )
        {
            file >> tmp;
            (*this)(i)= tmp;
        }
        
        file.close();

        return 0;
    }

    template<typename T>
    int RegressionDataSet<T>::saveToBinaryFile(const char* filename)
    {
        std::ofstream file;
        file.open(filename, ios::binary);

        if ( !file.is_open() )
            return 1;

        int size= this->size();
        int features= this->featureNames.size();
        int preprocessingSteps= this->preprocessingNames.size();

        file.write((char*)&size, sizeof(size));
        file.write((char*)&features, sizeof(features));
        file.write((char*)&preprocessingSteps, sizeof(preprocessingSteps));

        int tmp;
        for ( int i= 0; i < features; ++i )
        {
            tmp= featureNames(i).size();
            file.write((char*)&(tmp), sizeof(tmp));
            file.write((char*)&(featureNames(i)[0]), featureNames(i).size());
        }
        for ( int i= 0; i < preprocessingSteps; ++i )
        {
            tmp= preprocessingNames(i).size();
            file.write((char*)&(tmp), sizeof(tmp));
            file.write((char*)&(preprocessingNames(i)[0]), preprocessingNames(i).size());
        }

        for ( int i= 0; i < size; ++i )
        {
            file.write((char*)&(*this)(i).targetValue, sizeof((*this)(i).targetValue));
            file.write((char*)&(*this)(i)(0), sizeof(T)*features);
        }

        file.close();
        return 0;
    }

    template<typename T>
    int RegressionDataSet<T>::openFromBinaryFile(const char* filename)
    {
        std::ifstream file;
        file.open(filename, ifstream::in | ios::binary);

        if ( !file.is_open() )
            return 1;

        featureNames.clear();
        preprocessingNames.clear();

        int size;
        int features;
        int preprocessingSteps;

        file.read((char*)&size, sizeof(size));
        file.read((char*)&features, sizeof(features));
        file.read((char*)&preprocessingSteps, sizeof(preprocessingSteps));

        int tmp;
        std::string stmp;

        for ( int i= 0; i < features; ++i )
        {
            file.read((char*)&tmp, sizeof(int));
            stmp.resize(tmp);
            file.read(&(stmp[0]), tmp);
            featureNames.push_back(stmp);
        }

        for ( int i= 0; i < preprocessingSteps; ++i )
        {
            file.read((char*)&tmp, sizeof(int));
            stmp.resize(tmp);
            file.read(&(stmp[0]), tmp);
            preprocessingNames.push_back(stmp);
        }

        this->resize(size);

        for ( int i= 0; i < size; ++i )
        {
            (*this)(i).resize(features);
            file.read((char*)&(*this)(i).targetValue, sizeof((*this)(i).targetValue));
            file.read((char*)&(*this)(i)(0), sizeof(T)*features);
        }

        return 0;
    }

    template<typename T>
    int RegressionDataSet<T>::saveToCompressedFile(const char* filename)
    {
        int size= this->size();
        int features= this->featureNames.size();
        int preprocessingSteps= this->preprocessingNames.size();

        lzo_uint bytes= 0;
        bytes+= sizeof(size);
        bytes+= sizeof(features);
        bytes+= sizeof(preprocessingSteps);

        for ( unsigned int i= 0; i < featureNames.size(); ++i )
            bytes+= sizeof(int) + featureNames(i).size();

        for ( unsigned int i= 0; i < preprocessingNames.size(); ++i )
            bytes+= sizeof(int) + preprocessingNames(i).size();

        if ( this->size() > 0 )
            bytes+= (sizeof(int) * (*this)(0).size()*sizeof(T)) * size;

        unsigned char* __LZO_MMODEL src= (unsigned char*)(malloc(bytes));
        lzo_bytep ptr= src;

        memcpy((char*)ptr, (char*)&size, sizeof(size));
        ptr+= sizeof(size);
        memcpy((char*)ptr, (char*)&features, sizeof(features));
        ptr+= sizeof(features);
        memcpy((char*)ptr, (char*)&preprocessingSteps, sizeof(preprocessingSteps));
        ptr+= sizeof(preprocessingSteps);

        int tmp;

        for ( unsigned int i= 0; i < featureNames.size(); ++i )
        {
            tmp= featureNames(i).size();
            memcpy((void*)ptr, (void*)&tmp, sizeof(tmp));
            ptr+= sizeof(tmp);
            memcpy((void*)ptr, (void*)&(featureNames(i)[0]), tmp);
            ptr+= tmp;
        }

        for ( unsigned int i= 0; i < preprocessingNames.size(); ++i )
        {
            tmp= preprocessingNames(i).size();
            memcpy((void*)ptr, (void*)&tmp, sizeof(tmp));
            ptr+= sizeof(tmp);
            memcpy((void*)ptr, (void*)&(preprocessingNames(i)[0]), tmp);
            ptr+= tmp;
        }

        for ( unsigned int i= 0; i < this->size(); ++i )
        {
            memcpy((void*)ptr, (void*)&(*this)(i).targetValue, sizeof((*this)(i).targetValue));
            ptr+= sizeof((*this)(i).targetValue);
            memcpy((void*)ptr, (void*)&(*this)(i)(0), sizeof(T)*features);
            ptr+= sizeof(T)*features;
        }

        unsigned char* __LZO_MMODEL dst= (unsigned char*)(malloc(bytes + bytes/16 + 64 + 3));
        lzo_uint length;
        HEAP_ALLOC(wrkmem, LZO1X_1_MEM_COMPRESS);

        lzo_init();
        lzo1x_1_compress( src, bytes, dst, &length, wrkmem );

        std::ofstream file;
        file.open(filename, ios::binary);
        file.write((char*)&length, sizeof(length));
        file.write((char*)&bytes, sizeof(bytes));
        file.write((char*)dst, length);
        file.close();
        free(src);
        free(dst);

        return 0;
    }

    template<typename T>
    int RegressionDataSet<T>::openFromCompressedFile(const char* filename)
    {
        std::ifstream file;
        file.open(filename, ios::binary);

        lzo_uint length;
        lzo_uint finalSize;

        file.read((char*)&length, sizeof(length));
        file.read((char*)&finalSize, sizeof(finalSize));

        unsigned char* __LZO_MMODEL buf= (unsigned char*)malloc(sizeof(unsigned char)*length);
        unsigned char* __LZO_MMODEL dec= (unsigned char*)malloc(sizeof(unsigned char)*(finalSize + finalSize/16 + 64 + 3));

        file.read((char*)buf, length);

        file.close();

        lzo_uint decompressedSize;
        lzo_init();
        lzo1x_decompress(buf, length, dec, &decompressedSize, NULL);

        int size;
        int features;
        int preprocessingSteps;

        unsigned char* ptr= dec;

        memcpy(&size, (char*)ptr, sizeof(size));
        ptr+= sizeof(size);
        memcpy(&features, (char*)ptr, sizeof(features));
        ptr+= sizeof(features);
        memcpy(&preprocessingSteps, (char*)ptr, sizeof(preprocessingSteps));
        ptr+= sizeof(preprocessingSteps);

        featureNames.resize(features);
        preprocessingNames.resize(preprocessingSteps);
        int tmp;

        for ( int i= 0; i < features; ++i )
        {
            memcpy(&tmp, (char*)ptr, sizeof(int));
            ptr+= sizeof(int);
            this->featureNames(i).resize(tmp);
            memcpy(&((featureNames(i)[0])), (char*)ptr, tmp);
            ptr+= tmp;
        }

        for ( int i= 0; i < preprocessingSteps; ++i )
        {
            memcpy(&tmp, (char*)ptr, sizeof(int));
            ptr+= sizeof(int);
            this->preprocessingNames(i).resize(tmp);
            memcpy(&((preprocessingNames(i)[0])), (char*)ptr, tmp);
            ptr+= tmp;
        }

        this->resize(size);
        for ( int i= 0; i < size; ++i )
        {
            (*this)(i).resize(features);
            memcpy(&(*this)(i).targetValue, (char*)ptr, sizeof((*this)(i).targetValue));
            ptr+= sizeof((*this)(i).targetValue);
            memcpy(&(*this)(i)(0), (char*)ptr, sizeof(T)*features);
            ptr+= sizeof(T)*features;
        }

        free(buf);
        free(dec);

        return 0;
    }

    template<typename T>
    string RegressionDataSet<T>::dumpFeatures(Vector<int>& mask)
    {
        stringstream ss;
        for ( unsigned int i= 0; i < preprocessingNames.size(); ++i )
            ss << "preprocessing " << preprocessingNames[i] << endl;
        
        for ( unsigned int i= 0; i < mask.size(); ++i )
        {
            if ( mask(i) > 0 )
                ss << "feature " << featureNames[i] << endl;
        }
        return ss.str();
    }
    
    template<typename T>
    void RegressionDataSet<T>::sortByCoordinate(int coord)
    {
      sort(this->begin(), this->end(), RegressionFeatureVectorComparator<T>(coord));
    }
    
    template<typename T>
    void RegressionDataSet<T>::getCoordinateVector(int coord, Vector<T>& v)
    {
      v.resize(this->size());
      if ( coord >= 0 )
	for ( unsigned int i= 0; i < v.size(); ++i )
	  v(i)= (*this)(i)(coord);
      else
	for ( unsigned int i= 0; i < v.size(); ++i )
	  v(i)= (*this)(i).targetValue;
    }
    
    template<typename T>
    void RegressionDataSet<T>::setCoordinateVector(int coord, Vector<T>& v)
    {
      v.resize(this->size());
      if ( coord >= 0 )
	for ( unsigned int i= 0; i < v.size(); ++i )
	  (*this)(i)(coord)= v(i);
      else
	for ( unsigned int i= 0; i < v.size(); ++i )
	  (*this)(i).targetValue= v(i);
    }
}

#endif	/* DATASET_H */

