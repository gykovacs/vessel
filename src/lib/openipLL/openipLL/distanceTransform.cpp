#include <openipLL/distanceTransform.h>


namespace openip
{
    int round(double d)
    {
            return int(floor(d + 0.5));
    }

    double f(long int i, long int x, long int y, Image<float>& g)
    {
            double r= (x-i)*(x-i)+g(y, i)*g(y, i);
            return r;
    }

    bool C(int i, int h, int u, int y, Image<float>& g)
    {
            return f(i,u,y,g) < f(h,u,y,g);
    }

    int UL(int i, int h, int y, Image<float>& g)
    {
            return round(0.5 + (i*i-h*h + g(y, i)*g(y, i) - g(y, h)*g(y, h)+1)/(2*(i-h)));
    }

    int UR(int i, int h, int y, Image<float>& g)
    {
            return round( ( h*h - i*i + g(y, h)*g(y, h) - g(y, i)*g(y, i) - 1 ) / ( 2 * (h-i)) - 0.5);
    }

    bool contains(int i, std::vector<int>* v)
    {
            std::vector<int>::iterator it= v->begin();
            while( it != v->end() )
            {
                    if ( *it == i )
                            return true;
                    ++it;
            }
            return false;
    }

    void distanceLinearTime(Image<unsigned char>& input, Image<float>& output, Image<unsigned char>* roi)
    {
        int i,j,x,y,width= input.columns, height= input.rows;

        Image<float> g;
        g.resizeImage(input.rows, input.columns);

        vector<vector<int>*>* B= new vector<vector<int>*>();
        for ( i= 0; i < width; ++i )
                B->push_back(new vector<int>());

        for ( i= 0; i < width; ++i )
                for ( j= 0; j < height; ++j )
                        if ( input(j, i) > 0 && (!roi || (*roi)(j,i) > 0 ) )
                                B->at(i)->push_back(j);
        for ( x= 0; x < width; ++x )
        {
            g(0, x)= contains(0,B->at(x)) ? 0 : (width*width+height*height);

            y= 1;

            while( y < height )
            {
                    g(y,x)= contains(y,B->at(x)) ? 0 : ( 1 + g(y-1, x) );
                    y= y+1;
            }

            y= height-2;

            while( y >= 0 )
            {
                    g(y, x)= (g(y, x)>(1 + g(y+1, x)))?(1 + g(y+1, x)):g(y, x);
                    y= y-1;
            }
        }

        int p,q;
        int* s= (int*)malloc(sizeof(int)*width);
        int* t= (int*)malloc(sizeof(int)*width);

        for ( y= 0; y < height; ++y )
        {
            x= 1;
            output(y, 0)= f(0,0,y,g);
            p= 0;
            q= 0;
            s[0]= 0;
            t[0]= 0;

            while ( x < width )
            {
                if ( p < q && t[p+1] == x )
                        p= p+1;
                else
                        t[p]= x;

                if ( C(x,s[q],width-1,y,g) )
                {
                        while( p <= q && C(x,s[q],t[q],y,g) )
                                q= q-1;
                        q= q+1;
                        s[q]= x;
                        if ( p < q )
                                t[q]= UL(x,s[q-1],y,g);
                }
                output(y, x)= f(s[p],x,y,g);

                x= x+1;
            }

            x= width-2;
            p= 0;
            q= 0;
            s[0]= width-1;
            t[0]= width-1;

            while( x >= 0 )
            {
                if ( p < q && t[p+1]==x )
                        p= p+1;
                else
                        t[p]=x;
                if ( C(x,s[q],0,y,g) )
                {
                        while( p <= q && C(x,s[q],t[q],y,g) )
                                q= q-1;
                        q= q+1;
                        s[q]= x;
                        if ( p < q )
                                t[q]= UR(x,s[q-1],y,g);
                }
                output(y, x)= (output(y, x) < f(s[p],x,y,g)) ? output(y, x): f(s[p],x,y,g);

                x= x-1;
            }
        }

        for ( i= 0; i < height; ++i )
            for ( j= 0; j < width; ++j )
            {
                output(i, j)= sqrt(output(i, j));
            }
    }

    DistanceTransform::DistanceTransform()
    : Transform2<unsigned char, float>()
    {
        std::stringstream ss;
        ss << "DistanceTransform ";
        this->descriptor= ss.str();
    }

    DistanceTransform::DistanceTransform(const DistanceTransform& dt)
    : Transform2<unsigned char, float>(dt)
    {
    }

    DistanceTransform::~DistanceTransform()
    {
    }

    void DistanceTransform::apply(Image<unsigned char>& input, Image<float>& output, Image<unsigned char>* roi, Image<unsigned char>* )
    {
        distanceLinearTime(input, output, roi);
    }

}