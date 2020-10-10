#include <openipDS/BinaryMask2.h>

openip::BinaryMask2::BinaryMask2(int stride, unsigned char matches, unsigned char notMatches)
: Mask2<unsigned char, unsigned char, unsigned char>(stride, matches, notMatches)
{
}

openip::BinaryMask2::BinaryMask2(const BinaryMask2& b)
: Mask2<unsigned char, unsigned char, unsigned char>(b)
{
    this->matches= b.matches;
    this->notMatches= b.notMatches;
    this->stride= b.stride;
    this->min= b.min;
    this->max= b.max;
    this->mask= b.mask;
    this->pattern= b.pattern;
}

openip::BinaryMask2::~BinaryMask2()
{
}

void openip::BinaryMask2::generateMaskAndPattern()
{
    mask= 0;
    pattern= 0;

    for ( BinaryMask2::iterator bit= this->begin(); bit != this->end(); ++bit )
    {
        if (bit->first == 0 )
        {
            mask|= POWER0;
            if ( bit->second != 0 )
                pattern|= POWER0;
        }
        else if (bit->first == +1)
        {
            mask|= POWER1;
            if ( bit->second != 0 )
                pattern|= POWER1;
        }
        else if (bit->first == -stride + 1)
        {
            mask|= POWER2;
            if ( bit->second != 0 )
                pattern|= POWER2;
        }
        else if (bit->first == -stride)
        {
            mask|= POWER3;
            if ( bit->second != 0 )
                pattern|= POWER3;
        }
        else if (bit->first == -stride-1)
        {
            mask|= POWER4;
            if ( bit->second != 0 )
                pattern|= POWER4;
        }
        else if (bit->first == -1)
        {
            mask|= POWER5;
            if ( bit->second != 0 )
                pattern|= POWER5;
        }
        else if (bit->first == stride-1)
        {
            mask|= POWER6;
            if ( bit->second != 0 )
                pattern|= POWER6;
        }
        else if (bit->first == stride)
        {
            mask|= POWER7;
            if ( bit->second != 0 )
                pattern|= POWER7;
        }
        else if (bit->first == stride+1)
        {
            mask|= POWER8;
            if ( bit->second != 0 )
                pattern|= POWER8;
        }
        else if (bit->first == +2)
        {
            mask|= POWER9;
            if ( bit->second != 0 )
                pattern|= POWER9;
        }
        else if (bit->first == -stride+2)
        {
            mask|= POWER10;
            if ( bit->second != 0 )
                pattern|= POWER10;
        }
        else if (bit->first == -2*stride+2)
        {
            mask|= POWER11;
            if ( bit->second != 0 )
                pattern|= POWER11;
        }
        else if (bit->first == -2*stride+1)
        {
            mask|= POWER12;
            if ( bit->second != 0 )
                pattern|= POWER12;
        }
        else if (bit->first == -2*stride)
        {
            mask|= POWER13;
            if ( bit->second != 0 )
                pattern|= POWER13;
        }
        else if (bit->first == -2*stride-1)
        {
            mask|= POWER14;
            if ( bit->second != 0 )
                pattern|= POWER14;
        }
        else if (bit->first == -2*stride-2)
        {
            mask|= POWER15;
            if ( bit->second != 0 )
                pattern|= POWER15;
        }
        else if (bit->first == -stride-2)
        {
            mask|= POWER16;
            if ( bit->second != 0 )
                pattern|= POWER16;
        }
        else if (bit->first == -2)
        {
            mask|= POWER17;
            if ( bit->second != 0 )
                pattern|= POWER17;
        }
        else if (bit->first == stride-2)
        {
            mask|= POWER18;
            if ( bit->second != 0 )
                pattern|= POWER18;
        }
        else if (bit->first == 2*stride-2)
        {
            mask|= POWER19;
            if ( bit->second != 0 )
                pattern|= POWER19;
        }
        else if (bit->first == 2*stride-1)
        {
            mask|= POWER20;
            if ( bit->second != 0 )
                pattern|= POWER20;
        }
        else if (bit->first == 2*stride)
        {
            mask|= POWER21;
            if ( bit->second != 0 )
                pattern|= POWER21;
        }
        else if (bit->first == 2*stride+1)
        {
            mask|= POWER22;
            if ( bit->second != 0 )
                pattern|= POWER22;
        }
        else if (bit->first == 2*stride+2)
        {
            mask|= POWER23;
            if ( bit->second != 0 )
                pattern|= POWER23;
        }
        else if (bit->first == stride+2)
        {
            mask|= POWER24;
            if ( bit->second != 0 )
                pattern|= POWER24;
        }
    }
}

void openip::BinaryMask2::match(Image<unsigned char>& input, Image<unsigned char>& output, Image<unsigned char>* mask)
{
    this->updateStride(input.columns);
    int end= input.n - this->max;

    if ( mask == NULL )
        for ( int i= -this->min; i < end; ++i )
            output(i)= Feature2<unsigned char, unsigned char>::apply(input, i);
    else
        for ( int i= -this->min; i < end; ++i )
            if ( (*mask)(i) > 0)
                output(i)= Feature2<unsigned char, unsigned char>::apply(input, i);
}

void openip::BinaryMask2::match(Image<unsigned char>& input, PixelSet1& output, Image<unsigned char>* mask)
{
    output.clear();

    this->updateStride(input.columns);

    int end= input.n - this->max;

    if ( mask == NULL )
    {
        for ( int i= -this->min; i < end; ++i )
            if ( Mask2<unsigned char, unsigned char, unsigned char>::apply(input, i) == matches )
                output.push_back(i);
    }
    else
    {
        for ( int i= -this->min; i < end; ++i )
            if ( (*mask)(i) > 0 && Mask2<unsigned char, unsigned char, unsigned char>::apply(input, i) == matches )
                output.push_back(i);
    }
}

unsigned char openip::BinaryMask2::match(unsigned int env)
{
    if ( (env & mask) == pattern )
        return matches;
    return notMatches;
}

BinaryMask2& openip::BinaryMask2::rotate90()
{
    int x, y;
    for ( fIt fit= this->begin(); fit != this->end(); ++fit )
    {
        x= fit->first/this->stride;
        if ( abs(fit->first%this->stride) > this->stride/2 )
            x= (fit->first > 0) ? x+1 : x-1;

        y= fit->first%this->stride;
        if ( abs(y) > this->stride/2 )
            y+= y > 0? -this->stride : this->stride;

        fit->first= -y*this->stride + x;
    }

    this->computeMinMax();
    this->generateMaskAndPattern();
    return *this;
}

BinaryMask2& openip::BinaryMask2::rotate180()
{
    int x, y;
    for ( fIt fit= this->begin(); fit != this->end(); ++fit )
    {
        x= fit->first/this->stride;
        if ( abs(fit->first%this->stride) > this->stride/2 )
            x= (fit->first > 0) ? x+1 : x-1;

        y= fit->first%this->stride;
        if ( abs(y) > this->stride/2 )
            y+= y > 0? -this->stride : this->stride;

        fit->first= -x*this->stride - y;
    }

    this->computeMinMax();
    this->generateMaskAndPattern();
    return *this;
}

BinaryMask2& openip::BinaryMask2::rotate270()
{
    int x, y;
    for ( fIt fit= this->begin(); fit != this->end(); ++fit )
    {
        x= fit->first/this->stride;
        if ( abs(fit->first%this->stride) > this->stride/2 )
            x= (fit->first > 0) ? x+1 : x-1;

        y= fit->first%this->stride;
        if ( abs(y) > this->stride/2 )
            y+= y > 0? -this->stride : this->stride;

        fit->first= y*this->stride - x;
    }

    this->computeMinMax();
    this->generateMaskAndPattern();
    return *this;
}

std::ostream& openip::operator<<(std::ostream& o, const openip::BinaryMask2& b)
{
    o << "[Mask2 - size: ";

    std::stringstream pss, mss;

    unsigned int tmp= 1;
    for ( int i= 0; i < 32; ++i )
    {
        if ( ( b.mask & tmp) > 0 )
            mss << "1";
        else
            mss << "0";
        if ( ( b.pattern & tmp) > 0 )
            pss << "1";
        else
            pss << "0";
        tmp= tmp << 1;
    }

    o << b.size();
    o << " elements: ";
    o << mss.str();
    o << " ";
    o << pss.str();
    o << "]";

    return o;
}
