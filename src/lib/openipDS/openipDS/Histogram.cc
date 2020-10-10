#include <openipDS/Histogram.h>

namespace openip
{
    float Histogram::indexToValue(int i)
    {
        return ((float)i)/(this->size()-1)*(this->max - this->min) + this->min;
    }

    int Histogram::valueToIndex(float v)
    {
        return int((v - min)/d);
    }

    Histogram::Histogram()
    : Vector<float>()
    {
        normalized= false;
    }

    Histogram::Histogram(const Histogram& h)
    : Vector<float>(h)
    {
        normalized= h.normalized;
    }

    Histogram::Histogram(int bins)
    : Vector<float>()
    {
        normalized= false;
        this->resize(bins);
    }

    Histogram::~Histogram()
    {
    }

    void Histogram::normalize()
    {
        this->Vector<float>::normalize();
        normalized= true;
    }

    void Histogram::normalize(Histogram& hN, unsigned int t, unsigned int min, unsigned int max)
    {
        float sum= 0;

        for ( unsigned int i= min; i <= t; i++ )
            sum+= operator[](i);

        for ( unsigned int i= min; i <= t; i++ )
            hN[i]= operator[](i) / sum;

        sum= 0;
        for ( unsigned int i= t + 1; i <= max; i++ )
            sum+= operator[](i);

        for ( unsigned int i= t + 1; i <= max; i++ )
            hN[i]= operator[](i) / sum;

        normalized= true;
    }

    float Histogram::mfIndex(int index)
    {
        if ( !normalized )
            normalize();

        float mf= 0;
        float nf= 0;

        for ( unsigned int i= index; i < this->size(); ++i )
        {
            mf+= (*this)(i)*(this->indexToValue(i));
            nf+= (*this)(i);
        }
        return mf/nf;
    }

    float Histogram::mfIndex255(int index)
    {
        if ( !normalized )
            normalize();

        float mf= 0;
        float nf= 0;

        for ( unsigned int i= index; i < this->size(); ++i )
        {
            mf+= (*this)(i)*(this->indexToValue(i));
            nf+= (*this)(i);
        }
        return (mf/nf - min)/(max - min)*255.0;
    }

    float Histogram::mbIndex255(int index)
    {
        if ( !normalized )
            normalize();

        float mb= 0;
        float nb= 0;

        for ( int i= 0; i < index; ++i )
        {
            mb+= (*this)(i)*(this->indexToValue(i));
            nb+= (*this)(i);
        }
        return (mb/nb - min)/(max - min)*255.0;
    }

    float Histogram::mfValue(float value)
    {
        int index= this->valueToIndex(value);
        return mfIndex(index);
    }

    float Histogram::mbIndex(int index)
    {
        if ( !normalized )
            normalize();

        float mb= 0;
        float nb= 0;

        for ( int i= 0; i < index; ++i )
        {
            mb+= (*this)(i)*(this->indexToValue(i));
            nb+= (*this)(i);
        }
        return mb/nb;
    }

    float Histogram::mbValue(float value)
    {
        int index= this->valueToIndex(value);
        return mbIndex(index);
    }

    float Histogram::vfIndex(int index)
    {
        if ( !normalized )
            normalize();

        float sf= 0;
        float nf= 0;
        float mf= mfIndex(index);

        for ( unsigned int i= index; i < this->size(); ++i )
        {
            sf+= (*this)(i)*( (this->indexToValue(i) - mf)*(this->indexToValue(i) - mf) );
            nf+= (*this)(i);
        }
        return sf/nf;
    }

    float Histogram::vfValue(float value)
    {
        int index= this->valueToIndex(value);
        return sfIndex(index);
    }

    float Histogram::vbIndex(int index)
    {
        if ( !normalized )
            normalize();

        float sb= 0;
        float nb= 0;
        float mb= mbIndex(index);

        for ( int i= 0; i < index; ++i )
        {
            sb+= (*this)(i)*( (this->indexToValue(i) - mb)*(this->indexToValue(i) - mb) );
            nb+= (*this)(i);
        }
        return sb/nb;
    }

    float Histogram::vbValue(float value)
    {
        int index= this->valueToIndex(value);
        return sbIndex(index);
    }

    float Histogram::sfIndex(int index)
    {
        return sqrt(vfIndex(index));
    }

    float Histogram::sfValue(float value)
    {
        return sqrt(vfValue(value));
    }

    float Histogram::sbIndex(int index)
    {
        return sqrt(vbIndex(index));
    }

    float Histogram::sbValue(float value)
    {
        return sqrt(vbValue(value));
    }

    float Histogram::pIndex(int index)
    {
        if ( !normalized )
            normalize();

        float sum= 0;
        for ( int i= 0; i < index; ++i )
            sum+= (*this)(i);

        return sum;
    }

    float Histogram::pValue(float value)
    {
        int index= this->valueToIndex(value);
        return pIndex(index);
    }

    /*float Histogram::mean(unsigned int t, int mode, unsigned int limit)
    {
        float tmp= 0;
        switch(mode)
        {
            case HISTOGRAM_LESS_OR_EQUAL:
                for ( unsigned int i= limit; i <= t; ++i )
                    tmp+= (*this)[i]*i;
                break;
            case HISTOGRAM_GREATER:
                if (limit == 0)
                    limit= this->size();
                for ( unsigned int i= t+1; i < limit; ++i )
                    tmp+= (*this)[i]*i;
                break;
        }
        return tmp;
    }

    void Histogram::findTwoPeak(float& mB, float& mF)
    {
        unsigned int first, last;
        float threshold= 0.0;
        unsigned int i;

        for ( i= 0; i < 256; i++ )
                threshold+= (*this)[i];
        threshold /= 255.0;

        for ( i= 0; i < 256; i++ )
        {
                first= i;
                if ( (*this)[i] > threshold )
                        break;
        }

        for ( int i= 255; i >= 0; i-- )
        {
                last= i;
                if ( (*this)[i] > threshold )
                        break;
        }

        float value= (*this)[first];
        mB= first;

        for ( i= first; i <= (first + last) / 2; i++)
                if ( (*this)[i] > value )
                {
                        value= (*this)[i];
                        mB= i;
                }

        mF= last;
        value= (*this)[last];
        for ( i= last; i > (first + last) / 2; i-- )
                if ( (*this)[i] > value )
                {
                        value= (*this)[i];
                        mF= i;
                }
    }

    float Histogram::variance(unsigned int t, int mode)
    {
        float result= 0.0;
        float mean;

        mean= this->mean( t, mode );

        switch(mode)
        {
            case HISTOGRAM_LESS_OR_EQUAL:
                for ( unsigned int i= 0; i <= t; i++ )
                    result+= (( i - mean ) * ( i - mean )) * (*this)[i];
                break;
            case HISTOGRAM_GREATER:
                for ( unsigned int i= t+1; i <= this->size(); i++ )
                    result+= (( i - mean ) * ( i - mean )) * (*this)[i];
                break;
        }
        return result;
    }

    float Histogram::probabilityMassFunction(unsigned int start, unsigned int end)
    {
        float sum= 0.0;

        for (unsigned int i= start; i<= end; i++)
            sum+= (*this)[i];

        return sum;
    }

    float Histogram::probabilityMassFunction(unsigned int g)
    {
        float sum= 0.0;

        for (unsigned int i= 0; i<= g; i++)
            sum+= (*this)[i];

        return sum;
    }

    void Histogram::minMaxNonZeroElements( unsigned int& min, unsigned int& max )
    {
        unsigned int i;
        for ( i= 0; i < this->size(); ++i )
            if ( (*this)[i] > 0 )
            {
                min= i;
                break;
            }

        for ( i= this->size() - 1; i > 0; --i )
            if ( (*this)[i] > 0 )
            {
                max= i;
                break;
            }
    }*/

    float Histogram::getMin(int begin, int end, Vector<unsigned char>* mask)
    {
        float min= (*this)[begin];
        int i;

        if ( mask == NULL )
        {
            for ( i= begin + 1; i <= end; i++ )
                if ( (*this)[i] < min )
                    min= (*this)[i];
        }
        else
        {
            for ( i= begin + 1; i <= end; i++ )
                if ( (*mask)(i) > 0 )
                    if ( (*this)[i] < min )
                        min= (*this)[i];
        }
        return min;
    }

    float Histogram::getMax(int begin, int end, Vector<unsigned char>* mask)
    {
        float max= (*this)[begin];
        int i;

        if ( mask == NULL )
        {
            for ( i= begin + 1; i <= end; i++ )
                if ( (*this)[i] > max )
                    max= (*this)[i];
        }
        else
        {
            for ( i= begin + 1; i <= end; i++ )
                if ( (*mask)(i) > 0 )
                    if ( (*this)[i] > max )
                        max= (*this)[i];
        }
        return max;
    }

    int Histogram::getMinIndex(int begin, int end, Vector<unsigned char>* mask)
    {
        float min= (*this)[begin];
        int i;
        int index= begin;

        if ( mask == NULL )
        {
            for ( i= begin + 1; i <= end; i++ )
                if ( (*this)[i] < min )
                {
                    min= (*this)[i];
                    index= i;
                }
        }
        else
        {
            for ( i= begin + 1; i <= end; i++ )
                if ( (*mask)(i) > 0 )
                    if ( (*this)[i] < min )
                    {
                        min= (*this)[i];
                        index= i;
                    }
        }
        return index;
    }

    int Histogram::getMaxIndex(int begin, int end, Vector<unsigned char>* mask)
    {
        float max= (*this)[begin];
        int i;
        int index= begin;

        if ( mask == NULL )
        {
            for ( i= begin + 1; i <= end; i++ )
                if ( (*this)[i] > max )
                {
                    max= (*this)[i];
                    index= i;
                }
        }
        else
        {
            for ( i= begin + 1; i <= end; i++ )
                if ( (*mask)(i) > 0 )
                    if ( (*this)[i] > max )
                    {
                        max= (*this)[i];
                        index= i;
                    }
        }
        return index;
    }

    /*Histogram Histogram::histogramConvexHull()
    {
        int size= 256;
        int i;
        

        Histogram output;
        output.resize( size );

        Vector<float> histogram;
        histogram.resize( size );

        int minX= 0, maxX= size - 1;
        for ( i= 0; (*this)[i] == 0.0 ; i++ )
            minX++;

        for ( i= size - 1; (*this)[i] == 0 ; i-- )
            maxX--;

        float maxLocalIndex, maxH;
        maxH= this->getMaxIndex();

        float maxLocal, maxM;
        maxM= this->getMax();

        maxLocalIndex= maxH;
        maxLocal= maxM;

        output[maxH]= maxM;

        float minLocalIndex, minLocal;
        float opposite, rate;
        int maxmin, range;

        do
        {
            for ( i= maxLocalIndex + 1; i <= maxX; i++ )
                histogram[i]= (maxLocal - (*this)[i]) / (i - maxLocalIndex);	//tangens meghatározása

            if ( maxLocalIndex == size - 1 )
                minLocalIndex= maxLocalIndex;
            else
                minLocalIndex= this->getMinIndex(maxLocalIndex + 1, maxX);

            minLocal= (*this)[minLocalIndex];

            opposite= maxLocal - minLocal;
            maxmin= minLocalIndex - maxLocalIndex;
            rate= opposite / maxmin;
            range= 1;
            for ( i= maxLocalIndex+1; i <= minLocalIndex; i++ )
            {
                output[i]=  rate * range++;
                output[i]= maxLocal - output[i];
            }

            maxLocalIndex= minLocalIndex;
            maxLocal= minLocal;
        } while ( maxLocalIndex != maxX);

        maxLocalIndex= maxH;	maxLocal= maxM;

        do
        {
            for ( i= maxLocalIndex - 1; i >= minX; i-- )
                histogram[i]= (maxLocal - (*this)[i]) / (maxLocalIndex - i);

            if ( maxLocalIndex == 0 )
                minLocalIndex= maxLocalIndex;
            else
                minLocalIndex= this->getMinIndex(minX, maxLocalIndex - 1);

            minLocal= (*this)[minLocalIndex];

            opposite= maxLocal - minLocal;
            maxmin= maxLocalIndex - minLocalIndex;
            rate= opposite / maxmin;
            range= 1;
            for ( i= maxLocalIndex - 1; i >= minLocalIndex; i-- )
            {
                output[i]=  rate * range++;
                output[i]= maxLocal - output[i];
            }

            maxLocalIndex= minLocalIndex;
            maxLocal= minLocal;
        } while ( maxLocalIndex != minX );

        return output;
    }

    void Histogram::peakDetectionSignal(int kernel, Vector<openip::Peak>& peaks)
    {
        int size= this->size();
        int i, k, l, n;

        Vector<float> rN;
        rN.resize(size);
        float tmp= 0.0;

        for ( i= 0; i < size; i++ )
            rN[i]= 0.0;

        float wN= 1.0 / (double)kernel;

        for ( n= ((kernel - 1) / 2) - 1; n < size - ((kernel - 1) / 2); n++ )
        {

            for ( k= 1; k <= (kernel - 1) / 2.0; k++ )
            {
                tmp= 0.0;

                for ( l= n - k + 1; l <= n; l++ )
                    tmp+= (*this)[l];

                for ( l= n + 1; l <= n + k; l++ )
                    tmp-= (*this)[l];

                rN[n]+= wN * tmp;
            }
        }

        int start, end, maximum= 0;
        double localMax;
        k= 1;
        i= 0;

        do
        {
            for ( ; i < size; i++ )
                if ( rN[i] >= 0.0 )
                {
                    start= i;
                    break;
                }

            end= 0;
            localMax= 0.0;
            for ( ; i < size; i++ )
            {
                if ( rN[i] >= localMax )
                {
                    localMax= rN[i];
                    maximum= i;
                    continue;
                }
                
                if ( rN[i] <= 0.0 )
                {
                    end= i;
                    break;
                }
            }

            if ( end != 0 )
            {
                peaks.resize(k);
                peaks[k - 1]= Peak(start, end, maximum);
                k++;
            }
            
        } while ( end != 0 );
    }

    void Histogram::separate(Histogram& lower, Histogram& upper, int t)
    {
        lower.resize(t);
        upper.resize(this->size() - t);
        for ( int i= 0; i < t; ++i )
            lower[i]= (*this)[i];
        for ( unsigned int i= t; i < this->size(); ++i )
            upper[i]= (*this)[i];
    }*/

    void Histogram::accumulate()
    {
        for ( unsigned int i= 1; i < this->size(); ++i )
            (*this)[i]= (*this)[i] + (*this)[i-1];
    }

    std::ostream& operator<<(std::ostream& o, const Histogram& h)
    {
        o << "[Histogram - size: ";

        o << h.size();
        o << " elements: ";
        for ( unsigned int i= 0; i < h.size(); ++i )
        {
            if ( (h)(i) > 0  )
            {
                o << i;
                o << ":";
                o << (h)(i);
                o << " ";
            }
        }
        o << "]";

        return o;
    }


    Peak::Peak()
    {
    }

    Peak::Peak(const Peak& p)
    {
        this->start= p.start;
        this->end= p.end;
        this->maximum= p.maximum;
    }

    Peak::Peak(int start, int end, int maximum)
    {
        this->start= start;
        this->end= end;
        this->maximum= maximum;
    }

    Peak::~Peak()
    {
    }

    bool Peak::operator == (const Peak& p)
    {
        if ( maximum != p.maximum )
            return false;

        return true;
    }

    bool Peak::operator != (const Peak& p)
    {
        if ( maximum == p.maximum )
            return false;

        return true;
    }

    bool Peak::operator <= (const Peak& p)
    {
        if ( maximum <= p.maximum )
            return false;

        return true;
    }

    bool Peak::operator >= (const Peak& p)
    {
        if ( maximum >= p.maximum )
            return false;

        return true;
    }

    int Peak::getStart() const
    {
        return this->start;
    }

    int Peak::getEnd() const
    {
        return this->end;
    }

    int Peak::getMaximum() const
    {
        return this->maximum;
    }

    std::ostream& operator<<(std::ostream& o, const Peak& p)
    {
        char s[10];

        o << "Start: ";
        sprintf(s, "%d" , p.getStart());
        o << s;
        o << " End: ";
        sprintf(s, "%d" , p.getEnd());
        o << s;
        o << " Maximum: ";
        sprintf(s, "%d" , p.getMaximum());
        o << s;

        return o;
    }
}
