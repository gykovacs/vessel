#include <openipDS/Cache1.h>
#include <openipDS/Vector.h>

namespace openip
{
   /* Cache1::Cache1()
    {
        this->fuc= NULL;
        this->fs= NULL;
        this->fi= NULL;
        this->ff= NULL;
        this->fd= NULL;
        this->fcf= NULL;
        this->fcd= NULL;
    }

    Cache1::Cache1(const Cache1 &c)
    {
        this->fuc= c.fuc;
        this->fs= c.fs;
        this->fi= c.fi;
        this->ff= c.ff;
        this->fd= c.fd;
        this->fcf= c.fcf;
        this->fcd= c.fcd;
        this->n= c.n;
    }

    Cache1::~Cache1()
    {
        delete(this->fuc);
        delete(this->fs);
        delete(this->fi);
        delete(this->ff);
        delete(this->fd);
        delete(this->fcf);
        delete(this->fcd);
    }

    Cache1::Cache1(unsigned long long int n)
    {
        this->n= n;

        this->fuc= NULL;
        this->fs= NULL;
        this->fi= NULL;
        this->ff= NULL;
        this->fd= NULL;
        this->fcf= NULL;
        this->fcd= NULL;
    }

    Cache1::Cache1(unsigned long long int n, unsigned int length)
    {
        this->create(n, length);
    }

    void Cache1::create(unsigned int length)
    {
        this->create(this->n, length);
    }

    void Cache1::create(unsigned long long int n, unsigned int length)
    {
        this->fuc= new VectorFactory<unsigned char>( length, (n & CACHE_UCHAR_MASK) );
        this->fs= new VectorFactory<short>( length, (n & CACHE_SHORT_MASK) >> 8 );
        this->fi= new VectorFactory<int>( length, (n & CACHE_INT_MASK) >> 16 );
        this->ff= new VectorFactory<float>( length, (n & CACHE_FLOAT_MASK) >> 24 );
        this->fd= new VectorFactory<double>( length, (n & CACHE_DOUBLE_MASK) >> 32 );
        this->fcf= new VectorFactory<complex<float> >( length, (n & CACHE_CFLOAT_MASK) >> 40 );
        this->fcd= new VectorFactory<complex<double> >( length, (n & CACHE_CDOUBLE_MASK) >> 48 );
        this->n= n;
    }

    std::ostream& operator<<(std::ostream& o, const Cache1& c)
    {
        o << "[Cache1 ";

        o << "uc: ";
        o << (c.n & CACHE_UCHAR_MASK);
        o << ", s: ";
        o << (c.n & CACHE_SHORT_MASK);
        o << ", i: ";
        o << (c.n & CACHE_INT_MASK);
        o << ", f: ";
        o << (c.n & CACHE_FLOAT_MASK);
        o << ", d: ";
        o << (c.n & CACHE_DOUBLE_MASK);
        o << ", cf: ";
        o << (c.n & CACHE_CFLOAT_MASK);
        o << ", cd: ";
        o << (c.n & CACHE_CDOUBLE_MASK);
        o << "]";
        o << std::endl;

        return o;
    }*/
}
