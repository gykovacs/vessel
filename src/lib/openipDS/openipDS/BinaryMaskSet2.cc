#include <fstream>

#include <openipDS/BinaryMaskSet2.h>

openip::BinaryMaskSet2::BinaryMaskSet2(unsigned char matches, unsigned char notMatches, int mode)
{
    this->matches= matches;
    this->notMatches= notMatches;
    this->mode= mode;
    this->lookUpTableFileName= std::string("binaryMaskSet2.lut");
}

openip::BinaryMaskSet2::BinaryMaskSet2(const BinaryMaskSet2& bms)
: vector<BinaryMask2* >(bms)
{
    for ( BinaryMaskSet2::const_iterator bit= bms.begin(); bit != bms.end(); ++bit )
        this->push_back(*bit);
}

openip::BinaryMaskSet2::~BinaryMaskSet2()
{
}

void openip::BinaryMaskSet2::generateLookUpTable()
{
    unsigned int lutSize= 1;
    lutSize= lutSize << 25;
    lookUpTable.resize(lutSize);

    std::ifstream inFile(lookUpTableFileName.c_str(), ios::binary);
    if ( inFile != NULL )
        inFile.read((char*)&(lookUpTable[0]), lutSize);
    else
    {
        if ( this->mode == MATCH_ANY )
            for ( unsigned int i= 0; i < lookUpTable.size(); ++i )
                lookUpTable[i]= matchAny(i);
        else if ( this->mode == MATCH_NONE )
            for ( unsigned int i= 0; i < lookUpTable.size(); ++i )
                lookUpTable[i]= matchNone(i);
        else if ( this->mode == MATCH_ALL )
            for ( unsigned int i= 0; i < lookUpTable.size(); ++i )
                lookUpTable[i]= matchAll(i);
    }

    if ( inFile == NULL )
    {
        std::ofstream outFile(lookUpTableFileName.c_str(), ios::binary);
        if ( outFile != NULL )
            outFile.write((char*)&(lookUpTable[0]), lutSize);
        outFile.close();
    }
    inFile.close();
}

unsigned char openip::BinaryMaskSet2::matchAny(unsigned int env)
{
    for ( BinaryMaskSet2::iterator bit= this->begin(); bit != this->end(); ++bit )
        if ( (*bit)->match(env) == (*bit)->matches )
            return matches;
    return notMatches;
}

unsigned char openip::BinaryMaskSet2::matchAll(unsigned int env)
{
    for ( BinaryMaskSet2::iterator bit= this->begin(); bit != this->end(); ++bit )
        if ( (*bit)->match(env) == (*bit)->notMatches )
            return notMatches;
    return matches;
}

unsigned char openip::BinaryMaskSet2::matchNone(unsigned int env)
{
    for ( BinaryMaskSet2::iterator bit= this->begin(); bit != this->end(); ++bit )
        if ( (*bit)->match(env) == (*bit)->matches )
            return notMatches;
    return matches;
}

void openip::BinaryMaskSet2::matchAny(Image<unsigned char>& input, Image<unsigned char>& output, Image<unsigned char>* mask)
{
    this->updateStride(input.columns);

    int end= input.n - this->max;

    if ( mask == NULL )
    {
        if ( this->lookUpTable.size() == 0 )
            for ( int i= -this->min; i < end; ++i )
                output(i)= this->matchAny(extractEnvironment(input, i));
        else
            for ( int i= -this->min; i < end; ++i )
                output(i)= lookUpTable[extractEnvironment(input, i)];
    }
    else
    {
        if ( this->lookUpTable.size() == 0 )
        {
            for ( int i= -this->min; i < end; ++i )
                if ( (*mask)(i) > 0 )
                    output(i)= this->matchAny(extractEnvironment(input, i));
        }
        else
        {
            for ( int i= -this->min; i < end; ++i )
                if ( (*mask)(i) > 0 )
                    output(i)= lookUpTable[extractEnvironment(input, i)];
        }
    }
}

void openip::BinaryMaskSet2::matchAny(Image<unsigned char>& input, PixelSet1& output, Image<unsigned char>* mask)
{
    this->updateStride(input.columns);

    int end= input.n - this->max;

    if ( mask == NULL )
    {
        if ( this->lookUpTable.size() == 0 )
        {
            for ( int i= -this->min; i < end; ++i )
                if ( this->matchAny(extractEnvironment(input, i)) == matches )
                    output.push_back(i);
        }
        else
        {
            for ( int i= -this->min; i < end; ++i )
                if ( lookUpTable[extractEnvironment(input, i)] == matches )
                    output.push_back(i);
        }
    }
    else
    {
        if ( this->lookUpTable.size() == 0 )
        {
            for ( int i= -this->min; i < end; ++i )
                if ( (*mask)(i) && this->matchAny(extractEnvironment(input, i)) == matches )
                    output.push_back(i);
        }
        else
        {
            for ( int i= -this->min; i < end; ++i )
                if ( (*mask)(i) && lookUpTable[extractEnvironment(input, i)] == matches )
                    output.push_back(i);
        }
    }
}

void openip::BinaryMaskSet2::matchAll(Image<unsigned char>& input, Image<unsigned char>& output, Image<unsigned char>* mask)
{
    this->updateStride(input.columns);

    int end= input.n - this->max;

    if ( mask == NULL )
    {
        if ( this->lookUpTable.size() == 0 )
            for ( int i= -this->min; i < end; ++i )
                output(i)= this->matchAll(extractEnvironment(input, i));
        else
            for ( int i= -this->min; i < end; ++i )
                output(i)= lookUpTable[extractEnvironment(input, i)];
    }
    else
    {
        if ( this->lookUpTable.size() == 0 )
        {
            for ( int i= -this->min; i < end; ++i )
                if ( (*mask)(i) > 0 )
                    output(i)= this->matchAll(extractEnvironment(input, i));
        }
        else
        {
            for ( int i= -this->min; i < end; ++i )
                if ( (*mask)(i) > 0 )
                    output(i)= lookUpTable[extractEnvironment(input, i)];
        }
    }
}

void openip::BinaryMaskSet2::matchAll(Image<unsigned char>& input, PixelSet1& output, Image<unsigned char>* mask)
{
    this->updateStride(input.columns);

    int end= input.n - this->max;

    if ( mask == NULL )
    {
        if ( this->lookUpTable.size() == 0 )
        {
            for ( int i= -this->min; i < end; ++i )
                if ( this->matchAll(extractEnvironment(input, i)) == matches )
                    output.push_back(i);
        }
        else
        {
            for ( int i= -this->min; i < end; ++i )
                if ( lookUpTable[extractEnvironment(input, i)] == matches )
                    output.push_back(i);
        }
    }
    else
    {
        if ( this->lookUpTable.size() == 0 )
        {
            for ( int i= -this->min; i < end; ++i )
                if ( (*mask)(i) > 0 && this->matchAll(extractEnvironment(input, i)) == matches )
                    output.push_back(i);
        }
        else
        {
            for ( int i= -this->min; i < end; ++i )
                if ( (*mask)(i) > 0 && lookUpTable[extractEnvironment(input, i)] == matches )
                    output.push_back(i);
        }
    }
}

void openip::BinaryMaskSet2::matchNone(Image<unsigned char>& input, Image<unsigned char>& output, Image<unsigned char>* mask)
{
    this->updateStride(input.columns);
    
    int end= input.n - this->max;

    if ( mask == NULL )
    {
        if ( this->lookUpTable.size() == 0 )
            for ( int i= -this->min; i < end; ++i )
                output(i)= this->matchNone(extractEnvironment(input, i));
        else
            for ( int i= -this->min; i < end; ++i )
                output(i)= lookUpTable[extractEnvironment(input, i)];
    }
    else
    {
        if ( this->lookUpTable.size() == 0 )
        {
            for ( int i= -this->min; i < end; ++i )
                if ( (*mask)(i) > 0 )
                    output(i)= this->matchNone(extractEnvironment(input, i));
        }
        else
        {
            for ( int i= -this->min; i < end; ++i )
                if ( (*mask)(i) > 0 )
                    output(i)= lookUpTable[extractEnvironment(input, i)];
        }
    }
}

void openip::BinaryMaskSet2::matchNone(Image<unsigned char>& input, PixelSet1& output, Image<unsigned char>* mask)
{
    this->updateStride(input.columns);

    int end= input.n - this->max;

    if ( mask == NULL )
    {
        if ( this->lookUpTable.size() == 0 )
        {
            for ( int i= -this->min; i < end; ++i )
                if ( this->matchNone(extractEnvironment(input, i)) == matches )
                    output.push_back(i);
        }
        else
        {
            for ( int i= -this->min; i < end; ++i )
                if ( lookUpTable[extractEnvironment(input, i)] == matches )
                    output.push_back(i);
        }
    }
    else
    {
        if ( this->lookUpTable.size() == 0 )
        {
            for ( int i= -this->min; i < end; ++i )
                if ( (*mask)(i) > 0 && this->matchNone(extractEnvironment(input, i)) == matches )
                    output.push_back(i);
        }
        else
        {
            for ( int i= -this->min; i < end; ++i )
                if ( (*mask)(i) > 0 && lookUpTable[extractEnvironment(input, i)] == matches )
                    output.push_back(i);
        }
    }
}

unsigned int openip::BinaryMaskSet2::extractEnvironment(Image<unsigned char>& in, int n)
{
    unsigned int env= 0;

    if ( in(n + 1) > 0 )
        env|= POWER1;
    if ( in(n - in.columns + 1) > 0 )
        env|= POWER2;
    if ( in(n - in.columns) > 0 )
        env|= POWER3;
    if ( in(n - in.columns - 1) > 0 )
        env|= POWER4;
    if ( in(n - 1 ) > 0 )
        env|= POWER5;
    if ( in(n + in.columns - 1) > 0 )
        env|= POWER6;
    if ( in(n + in.columns) > 0 )
        env|= POWER7;
    if ( in(n + in.columns + 1) > 0 )
        env|= POWER8;
    if ( in(n + 2) > 0 )
        env|= POWER9;
    if ( in(n - in.columns + 2) > 0 )
        env|= POWER10;
    if ( in(n - 2*in.columns + 2) > 0 )
        env|= POWER11;
    if ( in(n - 2*in.columns + 1) > 0 )
        env|= POWER12;
    if ( in(n - 2*in.columns) > 0 )
        env|= POWER13;
    if ( in(n - 2*in.columns - 1) > 0 )
        env|= POWER14;
    if ( in(n - 2*in.columns - 2) > 0 )
        env|= POWER15;
    if ( in(n - in.columns - 2 ) > 0 )
        env|= POWER16;
    if ( in(n - 2) > 0 )
        env|= POWER17;
    if ( in(n + in.columns - 2 ) > 0 )
        env|= POWER18;
    if ( in(n + 2*in.columns - 2) > 0 )
        env|= POWER19;
    if ( in(n + 2*in.columns - 1) > 0 )
        env|= POWER20;
    if ( in(n + 2*in.columns) > 0 )
        env|= POWER21;
    if ( in(n + 2*in.columns + 1) > 0 )
        env|= POWER22;
    if ( in(n + 2*in.columns + 2) > 0 )
        env|= POWER23;
    if ( in(n + in.columns + 2) > 0 )
        env|= POWER24;

    return env;
}

void openip::BinaryMaskSet2::match(Image<unsigned char>& input, Image<unsigned char>& output, Image<unsigned char>* mask)
{
    if ( this->mode == MATCH_ANY )
        this->matchAny(input, output, mask);
    else if ( this->mode == MATCH_ALL )
        this->matchAll(input, output, mask);
    else if ( this->mode == MATCH_NONE )
        this->matchNone(input, output, mask);
}

void openip::BinaryMaskSet2::match(Image<unsigned char>& input, PixelSet1& output, Image<unsigned char>* mask)
{
    if ( this->mode == MATCH_ANY )
        this->matchAny(input, output, mask);
    else if (this->mode == MATCH_ALL )
        this->matchAll(input, output, mask);
    else if ( this->mode == MATCH_NONE )
        this->matchNone(input, output, mask);
}

unsigned char openip::BinaryMaskSet2::match(unsigned int env)
{
    switch(this->mode)
    {
        case MATCH_ANY:
            return this->matchAny(env);
        case MATCH_ALL:
            return this->matchAll(env);
        case MATCH_NONE:
            return this->matchNone(env);
    }
    return 0;
}

unsigned char openip::BinaryMaskSet2::match(Image<unsigned char>& input, int n)
{
    return this->match(this->extractEnvironment(input, n));
}

int openip::BinaryMaskSet2::stride()
{
    BinaryMaskSet2::iterator bit= this->begin();

    if ( this->end() == this->begin() )
        return -1;

    int stride= (*bit)->stride;

    for (; bit != this->end(); ++bit )
        if ( (*bit)->stride != stride)
            return -1;
    return stride;
}

void openip::BinaryMaskSet2::init(int stride)
{
    if ( stride != -1 )
        updateStride(stride);
    computeMinMax();
}

void openip::BinaryMaskSet2::updateStride(int stride)
{
    if ( this->stride() != stride )
    {
        BinaryMaskSet2::iterator bit= this->begin();

        for (; bit != this->end(); ++bit )
            (*bit)->updateStride(stride);
    }
    this->computeMinMax();
}

void openip::BinaryMaskSet2::computeMinMax()
{
    if ( lookUpTable.size() == 0 )
    {
        BinaryMaskSet2::iterator bit= this->begin();
        this->min= (*bit)->min;
        this->max= (*bit)->max;

        for (; bit != this->end(); ++bit )
            if ( (*bit)->min < this->min )
                this->min= (*bit)->min;
            else if ( (*bit)->max > this->max )
                this->max= (*bit)->max;
    }
    else
    {
        int stride= this->stride();
        this->min= -2*stride - 2;
        this->max= 2*stride + 2;
    }
}

std::ostream& openip::operator<<(std::ostream& o, const openip::BinaryMaskSet2& b)
{
    o << "[BinaryMaskSet2 - elements: ";
    for ( BinaryMaskSet2::const_iterator bit= b.begin(); bit != b.end(); ++bit )
    {
        o << **bit;
    }
    o << "]";

    return o;
}
