#ifndef NORMALIZER_H
#define NORMALIZER_H

#include <openipDS/DataSet.h>
#include <iostream>

namespace openip
{
    template<typename INPUT>
    class Normalizer
    {
    public:
        Normalizer(float min= 0, float max= 0, int unknown= 28);

        Normalizer(const Normalizer& n);

        ~Normalizer();

        void normalize(FeatureVector<INPUT>& input, FeatureVector<INPUT>& output);

        void normalize(Vector<INPUT>& input, Vector<INPUT>& output);

        void normalize(INPUT input, int idx, INPUT& output);

        void normalize(DataSet<INPUT>& d, Vector<unsigned char>* mask= NULL);

        void normalize(DataSet<INPUT>& input, DataSet<INPUT>& output);

        void train(DataSet<INPUT>& d, Vector<unsigned char>* mask= NULL);

        void saveToFile(std::ofstream& file);

        void openFromFile(std::ifstream& file);

        Vector<float> mins;
        Vector<float> maxs;
        Vector<float> diffs;
        float min;
        float max;
	
	int unknown;
	int unknownSet;
    };

    template<typename INPUT>
    Normalizer<INPUT>::Normalizer(float min, float max, int unknown)
    {
        this->min= min;
        this->max= max;
	
	this->unknown= unknown;
	if ( unknown != 28 )
	  this->unknownSet= 1;
	else
	  this->unknownSet= 0;
    }

    template<typename INPUT>
    Normalizer<INPUT>::Normalizer(const Normalizer<INPUT>& n)
    {
        this->mins= n.mins;
        this->maxs= n.maxs;
        this->min= n.min;
        this->max= n.max;
	
	this->unknown= n.unknown;
	this->unknownSet= n.unknownSet;
    }

    template<typename INPUT>
    Normalizer<INPUT>::~Normalizer()
    {
    }

    template<typename INPUT>
    void Normalizer<INPUT>::normalize(FeatureVector<INPUT>& input, FeatureVector<INPUT>& output)
    {
        if ( mins.size() > 0 )
            for ( unsigned int j= 0; j < input.size(); ++j )
                normalize(input(j), j, output(j));
        else
            output= input;
    }

    template<typename INPUT>
    void Normalizer<INPUT>::normalize(Vector<INPUT>& input, Vector<INPUT>& output)
    {
        if ( mins.size() > 0 )
            for ( unsigned int j= 0; j < input.size(); ++j )
                normalize(input(j), j, output(j));
        else
            output= input;
    }

    template<typename INPUT>
    void Normalizer<INPUT>::normalize(INPUT input, int idx, INPUT &output)
    {
        if ( mins.size() > 0 )
        {
	  if ( !unknownSet || (unknownSet && input != unknown) )
	  {
            if ( input >= mins(idx) && input <= maxs(idx) && diffs(idx) > 0 )
                output= (input - mins(idx))/diffs(idx)*(max - min) + min;
            else if ( input < mins(idx) )
                output= min;
            else if ( input > maxs(idx) )
                output= max;
            else
                output= min;
	  }
	  else
	    output= input;
        }
        else
            output= input;
    }

    template<typename INPUT>
    void Normalizer<INPUT>::normalize(DataSet<INPUT>& d, Vector<unsigned char>* mask)
    {
        train(d, mask);

	if ( !unknownSet )
	{
	  int dim= d(0).size();
	  for ( int i= 0; i < d.size(); ++i )
	      if ( !mask || (*mask)(i) )
	      {
		  for ( int j= 0; j < dim; ++j )
		  {
		      d(i)(j)= (d(i)(j) - mins(j))/diffs(j)*(max - min) + min;
		  }
	      }
	}
	else
	{
	  int dim= d(0).size();
	  for ( int i= 0; i < d.size(); ++i )
	      if ( !mask || (*mask)(i) )
	      {
		  for ( int j= 0; j < dim; ++j )
		  {
		    if ( d(i)(j) != unknown ) 
		      d(i)(j)= (d(i)(j) - mins(j))/diffs(j)*(max - min) + min;
		  }
	      }
	}
    }

    template<typename INPUT>
    void Normalizer<INPUT>::normalize(DataSet<INPUT>& input, DataSet<INPUT>& output)
    {
        output= input;
        normalize(output);
    }

    template<typename INPUT>
    void Normalizer<INPUT>::train(DataSet<INPUT>& d, Vector<unsigned char>* mask)
    {
        int dim= d(0).size();
        mins.resize(dim);
        maxs.resize(dim);
        diffs.resize(dim);
        mins= FLT_MAX;
        maxs= -FLT_MAX;
	if ( !unknownSet )
	{
	  for ( unsigned int i= 0; i < d.size(); ++i )
	      if ( !mask || (*mask)(i) )
	      {
		  for ( int j= 0; j < dim; ++j )
		  {
		      if ( d(i)(j) < mins(j) )
			  mins(j)= d(i)(j);
		      if ( d(i)(j) > maxs(j) )
			  maxs(j)= d(i)(j);
		  }
	      }
	}
	else
	{
	  for ( unsigned int i= 0; i < d.size(); ++i )
	      if ( !mask || (*mask)(i) )
	      {
		  for ( int j= 0; j < dim; ++j )
		  {
		      if ( d(i)(j) != unknown && d(i)(j) < mins(j) )
			  mins(j)= d(i)(j);
		      if ( d(i)(j) != unknown && d(i)(j) > maxs(j) )
			  maxs(j)= d(i)(j);
		  }
	      }
	}
        diffs= maxs;
        diffs-= mins;
        //cout << mins << maxs << diffs << std::endl;
    }

    template<typename INPUT>
    void Normalizer<INPUT>::saveToFile(std::ofstream& file)
    {
        file << min << std::endl;
        file << max << std::endl;
	file << unknown << std::endl;
	file << unknownSet << std::endl;
        file << mins.size() << std::endl;
        for ( unsigned int i= 0; i < mins.size(); ++i )
            file << mins(i) << " " << maxs(i) << std::endl;
    }

    template<typename INPUT>
    void Normalizer<INPUT>::openFromFile(std::ifstream& file)
    {
        file >> min;
        file >> max;
	file >> unknown;
	file >> unknownSet;
        unsigned int s;
        file >> s;
        mins.resize(s);
        maxs.resize(s);
        diffs.resize(s);
        float tmp;
        for ( unsigned int i= 0; i < s; ++i )
        {
            file >> tmp;
            mins(i)= tmp;
            file >> tmp;
            maxs(i)= tmp;
            diffs(i)= maxs(i) - mins(i);
        }
    }

    template<typename INPUT>
    class StatisticalNormalizer
    {
    public:
        StatisticalNormalizer(float min= 0, float max= 0);

        StatisticalNormalizer(const StatisticalNormalizer& n);

        ~StatisticalNormalizer();

        void normalize(FeatureVector<INPUT>& input, FeatureVector<INPUT>& output);

        void normalize(Vector<INPUT>& input, Vector<INPUT>& output);

        void normalize(INPUT input, int idx, INPUT& output);

        void normalize(DataSet<INPUT>& d, Vector<unsigned char>* mask= NULL);

        void normalize(DataSet<INPUT>& input, DataSet<INPUT>& output);

        void train(DataSet<INPUT>& d, Vector<unsigned char>* mask= NULL);

        void saveToFile(std::ofstream& file);

        void openFromFile(std::ifstream& file);

        Vector<float> mins;
        Vector<float> maxs;
        Vector<float> diffs;
        float min;
        float max;
    };

    template<typename INPUT>
    StatisticalNormalizer<INPUT>::StatisticalNormalizer(float min, float max)
    {
        this->min= min;
        this->max= max;
    }

    template<typename INPUT>
    StatisticalNormalizer<INPUT>::StatisticalNormalizer(const StatisticalNormalizer<INPUT>& n)
    {
        this->mins= n.mins;
        this->maxs= n.maxs;
        this->min= n.min;
        this->max= n.max;
    }

    template<typename INPUT>
    StatisticalNormalizer<INPUT>::~StatisticalNormalizer()
    {
    }

    template<typename INPUT>
    void StatisticalNormalizer<INPUT>::normalize(FeatureVector<INPUT>& input, FeatureVector<INPUT>& output)
    {
        if ( mins.size() > 0 )
        {
            for ( unsigned int j= 0; j < input.size(); ++j )
            {
                /*if ( input(j) >= mins(j) && input(j) <= maxs(j) && diffs(j) > 0 )
                    output(j)= (input(j) - mins(j))/diffs(j)*(max - min) + min;
                else if ( input(j) < mins(j) )
                    output(j)= min;
                else if ( input(j) > maxs(j) )
                    output(j)= max;
                else
                    output(j)= min;*/
                normalize(input(j), j, output(j));
            }
        }
        else
            output= input;
    }

    template<typename INPUT>
    void StatisticalNormalizer<INPUT>::normalize(Vector<INPUT>& input, Vector<INPUT>& output)
    {
        if ( mins.size() > 0 )
        {
            for ( unsigned int j= 0; j < input.size(); ++j )
            {
                /*if ( input(j) >= mins(j) && input(j) <= maxs(j) && diffs(j) > 0 )
                    output(j)= (input(j) - mins(j))/diffs(j)*(max - min) + min;
                else if ( input(j) < mins(j) )
                    output(j)= min;
                else if ( input(j) > maxs(j) )
                    output(j)= max;
                else
                    output(j)= min;*/
                normalize(input(j), j, output(j));
            }
        }
        else
            output= input;
    }

    template<typename INPUT>
    void StatisticalNormalizer<INPUT>::normalize(INPUT input, int idx, INPUT &output)
    {
        if ( mins.size() > 0 )
        {
            if ( input >= mins(idx) && input <= maxs(idx) && diffs(idx) > 0 )
                output= (input - mins(idx))/diffs(idx)*(max - min) + min;
            else if ( input < mins(idx) )
                output= min;
            else if ( input > maxs(idx) )
                output= max;
            else
                output= min;
        }
        else
            output= input;
    }

    template<typename INPUT>
    void StatisticalNormalizer<INPUT>::normalize(DataSet<INPUT>& d, Vector<unsigned char>* mask)
    {
        train(d, mask);

        int dim= d(0).size();
        for ( int i= 0; i < d.size(); ++i )
            if ( !mask || (*mask)(i) )
            {
                for ( int j= 0; j < dim; ++j )
                {
                    d(i)(j)= (d(i)(j) - mins(j))/diffs(j)*(max - min) + min;
                }
            }
    }

    template<typename INPUT>
    void StatisticalNormalizer<INPUT>::normalize(DataSet<INPUT>& input, DataSet<INPUT>& output)
    {
        output= input;
        normalize(output);
    }

    template<typename INPUT>
    void StatisticalNormalizer<INPUT>::train(DataSet<INPUT>& d, Vector<unsigned char>* mask)
    {
        int dim= d(0).size();
        mins.resize(dim);
        maxs.resize(dim);
        diffs.resize(dim);
        mins= FLT_MAX;
        maxs= -FLT_MAX;
        for ( unsigned int i= 0; i < d.size(); ++i )
            if ( !mask || (*mask)(i) )
            {
                for ( int j= 0; j < dim; ++j )
                {
                    if ( d(i)(j) < mins(j) )
                        mins(j)= d(i)(j);
                    if ( d(i)(j) > maxs(j) )
                        maxs(j)= d(i)(j);
                }
            }
        diffs= maxs;
        diffs-= mins;
        //cout << mins << maxs << diffs << std::endl;
    }

    template<typename INPUT>
    void StatisticalNormalizer<INPUT>::saveToFile(std::ofstream& file)
    {
        file << min << std::endl;
        file << max << std::endl;
        file << mins.size() << std::endl;
        for ( unsigned int i= 0; i < mins.size(); ++i )
            file << mins(i) << " " << maxs(i) << std::endl;
    }

    template<typename INPUT>
    void StatisticalNormalizer<INPUT>::openFromFile(std::ifstream& file)
    {
        file >> min;
        file >> max;
        unsigned int s;
        file >> s;
        mins.resize(s);
        maxs.resize(s);
        diffs.resize(s);
        float tmp;
        for ( unsigned int i= 0; i < s; ++i )
        {
            file >> tmp;
            mins(i)= tmp;
            file >> tmp;
            maxs(i)= tmp;
            diffs(i)= maxs(i) - mins(i);
        }
    }
}

#endif // NORMALIZER_H
