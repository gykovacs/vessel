#include <openipDS/Cache2.h>

/*namespace openip
{
    Cache2::Cache2()
    : fuc(0), fs(0), fi(0), ff(0), fd(0), fcf(0), fcd(0),
            topBorder(0), bottomBorder(0), leftBorder(0), rightBorder(0), borderMode(1), n(0)
    {
    }

    Cache2::Cache2(const Cache2 &c)
    : fuc(c.fuc), fs(c.fs), fi(c.fi), ff(c.ff), fd(c.fd), fcf(c.fcf), fcd(c.fcd), topBorder(c.topBorder),
            bottomBorder(c.bottomBorder), leftBorder(c.leftBorder), rightBorder(c.rightBorder), borderMode(c.borderMode), n(c.n)
    {
    }

    Cache2::~Cache2()
    {
        delete(this->fuc);
        delete(this->fs);
        delete(this->fi);
        delete(this->ff);
        delete(this->fd);
        delete(this->fcf);
        delete(this->fcd);
    }

    Cache2::Cache2(unsigned long long int n)
    : fuc(0), fs(0), fi(0), ff(0), fd(0), fcf(0), fcd(0), topBorder(0), bottomBorder(0),
            leftBorder(0), rightBorder(0), borderMode(1)
    {
        this->n= n;
    }

    Cache2::Cache2(unsigned long long int n, unsigned int rows, unsigned int columns)
    {
        this->create(n, rows + topBorder + bottomBorder, columns + leftBorder + rightBorder);
    }

    void Cache2::create(unsigned int rows, unsigned int columns)
    {
        this->create(this->n, rows + topBorder + bottomBorder, columns + leftBorder + rightBorder);
    }

    void Cache2::create(unsigned long long int n, unsigned int rows, unsigned int columns)
    {
        this->fuc= new ImageFactory<unsigned char>( rows, columns, (n & CACHE_UCHAR_MASK), topBorder, bottomBorder, leftBorder, rightBorder, borderMode );
        this->fs= new ImageFactory<short>( rows, columns, (n & CACHE_SHORT_MASK) >> 8, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
        this->fi= new ImageFactory<int>( rows, columns, (n & CACHE_INT_MASK) >> 16, topBorder, bottomBorder, leftBorder, rightBorder, borderMode );
        this->ff= new ImageFactory<float>( rows, columns, (n & CACHE_FLOAT_MASK) >> 24, topBorder, bottomBorder, leftBorder, rightBorder, borderMode );
        this->fd= new ImageFactory<double>( rows, columns, (n & CACHE_DOUBLE_MASK) >> 32, topBorder, bottomBorder, leftBorder, rightBorder, borderMode );
        this->fcf= new ImageFactory<complex<float> >( rows, columns, (n & CACHE_CFLOAT_MASK) >> 40, topBorder, bottomBorder, leftBorder, rightBorder, borderMode );
        this->fcd= new ImageFactory<complex<double> >( rows, columns, (n & CACHE_CDOUBLE_MASK) >> 48, topBorder, bottomBorder, leftBorder, rightBorder, borderMode );
        this->n= n;
        this->columns= columns;
        this->rows= rows;
    }

    void Cache2::setBorder(int borderM, int topB, int bottomB, int leftB, int rightB)
    {
        topBorder= topB;
        bottomBorder= bottomB;
        leftBorder= leftB;
        rightBorder= rightB;
        borderMode= borderM;
    }

    void Cache2::computeFourierOptimalBorders(int rows, int columns)
    {
        int c= columns;
        int r= rows;
        int tmpCol= -1;
        int tmpRow= -1;

        while( c != 1 )
        {
            ++tmpCol;
            c= columns + (tmpCol);
            while( c % 2 == 0 && c > 1 )
                c= c/2;
            while( c % 3 == 0 && c > 2)
                c= c/3;
            while( c % 5 == 0 && c > 4)
                c= c/5;
        }
        r= rows;
        while( r != 1 )
        {
            ++tmpRow;
            r= rows + (tmpRow);
            while( r % 2 == 0 && r > 1)
                r= r/2;
            while( r % 3 == 0 && r > 2)
                r= r/3;
            while( r % 5 == 0 && r > 4)
                r= r/5;
        }

        if ( tmpCol % 2 == 1 )
            leftBorder= tmpCol/2 + 1;
        else
            leftBorder= tmpCol/2;
        rightBorder= tmpCol/2;

        if ( tmpRow % 2 == 1 )
            topBorder= tmpRow/2 + 1;
        else
            topBorder= tmpRow/2;
        bottomBorder= tmpRow/2;
    }

    std::ostream& operator<<(std::ostream& o, const Cache2& c)
    {
        o << "[Cache2 ";
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
        o << ", topBorder: ";
        o << c.topBorder;
        o << ", bottomBorder: ";
        o << c.bottomBorder;
        o << ", leftBorder: ";
        o << c.leftBorder;
        o << ", rightBorder: ";
        o << c.rightBorder;
        o << ", borderMode: ";
        o << c.borderMode;
        o << "]";

        return o;
    }
}
*/
