#include <fstream>

#include <openipDS/BinaryMaskSystem2.h>

openip::BinaryMaskSystem2::BinaryMaskSystem2(unsigned char matches, unsigned char notMatches)
{
    this->matches= matches;
    this->notMatches= notMatches;
    this->lookUpTableFileName= std::string("binaryMaskSystem2.lut");
}

openip::BinaryMaskSystem2::BinaryMaskSystem2(const BinaryMaskSystem2& bms)
: vector<BinaryMaskSet2* >(bms)
{
    for ( BinaryMaskSystem2::const_iterator bit= bms.begin(); bit != bms.end(); ++bit )
        this->push_back(*bit);
}

openip::BinaryMaskSystem2::~BinaryMaskSystem2()
{
}

void openip::BinaryMaskSystem2::match(Image<unsigned char>& in, Image<unsigned char>& out, Image<unsigned char>* mask)
{
    this->updateStride(in.columns);

    int end= in.n - this->max;

    if ( mask == NULL )
    {
        if ( lookUpTable.size() == 0 )
            for ( int i= - this->min; i < end; ++i )
                out(i)= this->match(extractEnvironment(in, i));
        else
            for ( int i= - this->min; i < end; ++i )
                out(i)= this->lookUpTable[extractEnvironment(in, i)];
    }
    else
    {
        if ( lookUpTable.size() == 0 )
        {
            for ( int i= - this->min; i < end; ++i )
                if ( (*mask)(i) > 0 )
                    out(i)= this->match(extractEnvironment(in, i));
        }
        else
        {
            for ( int i= - this->min; i < end; ++i )
                if ( (*mask)(i) > 0 )
                    out(i)= this->lookUpTable[extractEnvironment(in, i)];
        }
    }
}

/*void openip::BinaryMaskSystem2::match(Image<unsigned char>& in, Cache2* c, Image<unsigned char>* mask)
{
    Image<unsigned char>* tmp;
    c->get(&tmp);
    this->match(in, *tmp, mask);
    in= *tmp;
    c->put(tmp);
}*/

void openip::BinaryMaskSystem2::match(Image<unsigned char>& in, PixelSet1& out, Image<unsigned char>* mask)
{
    out.clear();

    this->updateStride(in.columns);

    int end= in.n - this->max;

    //printf("match: %p %d %d\n", mask, this->min, end);
    if ( mask == NULL )
    {
        if ( lookUpTable.size() == 0 )
        {
            for ( int i= - this->min; i < end; ++i )
                if ( this->match(extractEnvironment(in, i)) == this->matches )
                    out.push_back(i);
        }
        else
        {
            for ( int i= - this->min; i < end; ++i )
                if ( this->lookUpTable[extractEnvironment(in, i)] == this->matches )
                    out.push_back(i);
        }
    }
    else
    {
        if ( lookUpTable.size() == 0 )
        {
            for ( int i= - this->min; i < end; ++i )
                if ( (*mask)(i) > 0 && this->match(extractEnvironment(in, i)) == this->matches )
                    out.push_back(i);
        }
        else
        {
            //printf("."); 
            for ( int i= - this->min; i < end; ++i )
            {
                //printf("%d ", this->lookUpTable[extractEnvironment(in, i)]);
                if ( (*mask)(i) > 0 && this->lookUpTable[extractEnvironment(in, i)] == this->matches )
                {
                    //printf(",");
                    out.push_back(i);
                }
            }
        }
    }
}

void openip::BinaryMaskSystem2::generateLookUpTable()
{
    unsigned int lutSize= 1;
    lutSize= lutSize << 25;

    this->lookUpTable.resize(lutSize);

    std::ifstream inFile(lookUpTableFileName.c_str(), ios::binary);
    
    if ( inFile != NULL )
        inFile.read((char*)&(lookUpTable[0]), lutSize);
    else
    {
        BinaryMaskSystem2::iterator bit= this->begin();

        for ( unsigned int i= 0; i < lutSize; ++i )
            lookUpTable[i]= this->match(i);
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

unsigned char openip::BinaryMaskSystem2::match(unsigned int env)
{
    for ( BinaryMaskSystem2::iterator bit= this->begin(); bit != this->end(); ++bit )
        if ( (*bit)->match(env) != (*bit)->matches )
            return notMatches;
    return matches;
}

int openip::BinaryMaskSystem2::stride()
{
    BinaryMaskSystem2::iterator bit= this->begin();
    if ( this->begin() == this->end() )
        return -1;

    int stride= (*bit)->stride();
    for (; bit != this->end(); ++bit )
        if ( stride != (*bit)->stride() )
            return -1;
    return stride;
}

void openip::BinaryMaskSystem2::updateStride(int stride)
{
    if ( this->stride() != stride )
    {
        for( BinaryMaskSystem2::iterator bit= this->begin(); bit != this->end(); ++bit )
            (*bit)->updateStride(stride);
    }
    this->computeMinMax();
}

void openip::BinaryMaskSystem2::computeMinMax()
{
    if ( this->lookUpTable.size() == 0 )
    {
        BinaryMaskSystem2::iterator bit= this->begin();

        this->min= -2*(*bit)->stride() - 2;
        this->max= 2*(*bit)->stride() + 2;

        for ( ; bit != this->end(); ++bit )
        {
            if ( this->min > (*bit)->min )
                this->min= (*bit)->min;
            if ( this->max < (*bit)->max )
                this->max= (*bit)->max;
        }
    }
    else
    {
        int stride= this->stride();
        min= -2*stride - 2;
        max= 2*stride + 2;
    }
}

void openip::BinaryMaskSystem2::init(int stride)
{
    if ( stride != -1 )
        updateStride(stride);
    computeMinMax();
}

unsigned int openip::BinaryMaskSystem2::extractEnvironment(Image<unsigned char>& in, int n)
{
    unsigned int env= 0;
    if ( in(n) > 0 )
        env|= POWER0;
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

    //printf("%d ", env);
    return env;
}

std::ostream& openip::operator<<(std::ostream& o, const openip::BinaryMaskSystem2& b)
{
    o << "[BinaryMaskSystem2 - elements: ";
    for ( BinaryMaskSystem2::const_iterator bit= b.begin(); bit != b.end(); ++bit )
    {
        o << **bit;
    }
    o << "]";

    return o;
}
