/**
 * @file EntropyBasedDiscretizer.h
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
 * EntropyBasedDiscretizer represents the discretization method based on the
 * entropy of the data and class variables.
 */

#ifndef ENTROPYBASEDDISCRETIZER_H
#define	ENTROPYBASEDDISCRETIZER_H

#include <openipDS/DiscreteDataSet.h>
#include <openipDS/ContinuousDataSet.h>

#include <openipML/Discretizer.h>
#include <openipML/KMeansClustering.h>

#include <queue>

namespace openip
{
    /**
     * EntropyBasedDiscretizer represents the discretization method based on the
     * entropy of the data and class variables.
     */
    template<typename T>
    class EntropyBasedDiscretizer: public Discretizer<T>
    {
    public:
        using Discretizer<T>::openDiscretizerFromFile;
        using Discretizer<T>::saveDiscretizerToFile;
        using Discretizer<T>::discretize;
        using Discretizer<T>::data;
        using Discretizer<T>::k;
	using Discretizer<T>::unknown;
	using Discretizer<T>::unknownSet;

        /**
         * constructor
         * @param k number of discrete objects
         */
        EntropyBasedDiscretizer(int k= 10, int unknown= 28);

        /**
         * copy constructor
         * @param d instance to copy
         */
        EntropyBasedDiscretizer(const EntropyBasedDiscretizer& d);

        /**
         * destructor
         */
        ~EntropyBasedDiscretizer();

        /**
         * initializes the discretizer object
         * @param input the discretizer object is initialized using this dataset
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int init(DataSet<T>& input);

        /**
         * opens the discretizer from file
         * @param file input file stream
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int openDiscretizerFromFile(std::ifstream& file);

        /**
         * saves the discretizer to file
         * @param file output file stream
         * @return non-zero in case of errors, zero otherwise
         */
        virtual int saveDiscretizerToFile(std::ofstream& file);

        /**
         * binary partitioning of the dataset
         * @param data dataset to partition
         * @param begin first index
         * @param end last index
         * @param classes number of classes
         * @return midpoint
         */
        int binaryPartitioning(Vector<FeatureVector<T> >& data, int begin, int end, int classes);
    };

    template<typename T>
    EntropyBasedDiscretizer<T>::EntropyBasedDiscretizer(int k, int unknown)
    : Discretizer<T>(unknown)
    {
        this->k= k;
        this->data.resize(k);
    }

    template<typename T>
    EntropyBasedDiscretizer<T>::EntropyBasedDiscretizer(const EntropyBasedDiscretizer& d)
    : Discretizer<T>(d)
    {
        this->k= d.k;
        this->data= d.data;
    }

    template<typename T>
    EntropyBasedDiscretizer<T>::~EntropyBasedDiscretizer()
    {
    }

    template<typename T>
    int EntropyBasedDiscretizer<T>::init(DataSet<T>& input)
    {
        input.init();

        data.resize(k-1);
        for ( unsigned int i= 0; i < data.size(); ++i )
            data(i).resize(input.dimension);

        Vector<FeatureVector<T> > tmp;
        tmp.resize(input.size());
        
        for ( unsigned int i= 0; i < tmp.size(); ++i )
            tmp(i).resize(1);

        std::queue<int> queueBegin;
        std::queue<int> queueEnd;
        for ( int d= 0; d < input.dimension; ++d )
        {
	    if ( !unknownSet )
	    {
	      for ( unsigned int i= 0; i < input.size(); ++i )
	      {
		  tmp(i).classIndex= input(i).classIndex;
		  tmp(i)(0)= input(i)(d);
	      }
	      sort(tmp.begin(), tmp.end());
	    }
	    else
	    {
	      int n= 0;
	      for ( unsigned int i= 0; i < input.size(); ++i )
	      {
		if ( input(i)(d) != unknown )
		{
		  tmp(n).classIndex= input(i).classIndex;
		  tmp(n++)(0)= input(i)(d);
		}
	      }
	      sort(tmp.begin(), tmp.begin() + n);
	    }
            

            Vector<float> splits;
            splits.resize(k-1);
            splits= 0;

            int noSplit= 0;
            int split;

            while ( !queueBegin.empty() )
                queueBegin.pop();
            while ( !queueEnd.empty() )
                queueEnd.pop();
            
            queueBegin.push(0);
            queueEnd.push(tmp.size());

            
            while ( noSplit < k-1 )
            {
                split= binaryPartitioning(tmp, queueBegin.front(), queueEnd.front(), input.numberOfClasses);
                queueBegin.push(queueBegin.front());
                queueEnd.push(split);
                queueBegin.push(split);
                queueEnd.push(queueEnd.front());
                queueBegin.pop();
                queueEnd.pop();

                splits(noSplit)= tmp(split)(0);
                ++noSplit;
            }
            //cout << splits << endl;
            sort(splits.begin(), splits.end());
            for ( unsigned int i= 0; i < splits.size(); ++i )
                data(i)(d)= splits(i);
        }
        
        return 0;
    }

    template<typename T>
    int EntropyBasedDiscretizer<T>::binaryPartitioning(Vector<FeatureVector<T> >& input, int begin, int end, int classes)
    {
        if ( end == begin )
            return end;
        Vector<float> freqA;
        Vector<float> freqB;
        float nA;
        float nB;

        freqA.resize(classes);
        freqA= 0;
        freqB.resize(classes);
        freqB= 0;

        for ( int i= begin; i < end; ++i )
            freqA(input(i).classIndex)+= 1;
        nA= end - begin;
        nB= 0;

        float minEntropy= FLT_MAX;
        int minIdx= 0;
        float entropyA;
        float entropyB;

        float tmp;

        for ( int i= begin; i < end; ++i )
        {
            entropyA= entropyB= 0;
            if ( nA > 0 )
            {
                for ( unsigned a= 0; a < freqA.size(); ++a )
                    if ( freqA(a) > 0 )
                        entropyA+= -freqA(a)/nA * log(freqA(a)/nA);
            }
            else
                entropyA= 0;

            if ( nB > 0 )
            {
                for ( unsigned b= 0; b < freqB.size(); ++b )
                    if ( freqB(b) > 0 )
                        entropyB+= -freqB(b)/nB * log(freqB(b)/nB);
            }
            else
                entropyB= 0;

            tmp= nA/(end - begin)*entropyA + nB/(end - begin)*entropyB;
            if ( tmp < minEntropy )
            {
                minEntropy= tmp;
                minIdx= i;
            }

            freqA(input(i).classIndex)-= 1;
            freqB(input(i).classIndex)+= 1;
            nA-= 1;
            nB+= 1;
        }

        return minIdx;
    }

    template<typename T>
    int EntropyBasedDiscretizer<T>::openDiscretizerFromFile(std::ifstream& file)
    {
        std::string tmp;

        file >> tmp;

	cout << tmp << endl;
        if ( tmp.compare("EntropyBasedDiscretizer") == 0 || tmp.find("EntropyBasedDiscretizer") == 0 )
        {
	  cout << "opening" << endl;
            this->Discretizer<T>::openDiscretizerFromFile(file);

            return 0;
        }
        return 1;
    }

    template<typename T>
    int EntropyBasedDiscretizer<T>::saveDiscretizerToFile(std::ofstream& file)
    {
        file << "EntropyBasedDiscretizer" << std::endl;
        this->Discretizer<T>::saveDiscretizerToFile(file);

        return 0;
    }
}

#endif	/* ENTROPYBASEDDISCRETIZER_H */

