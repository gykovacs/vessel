#include <openipLL/colorSpaces.h>

#include <openipDS/Image.h>

#include <omp.h>
#include <iostream>
#include <math.h>
#include <complex>

using namespace std;

#define LUV_U 0.950456
#define LUV_V 1.088754

namespace openip
{
    void luv2rgb(float l, float u, float v, unsigned char& r, unsigned char& g, unsigned char& b)
    {
        float x, y, z;
        luv2xyz(l, u, v, x, y, z);
        xyz2rgb(x, y, z, r, g, b);
    }

    void luv2rgb(Image<float>& l, Image<float>& u, Image<float>& v, Image<unsigned char>& r, Image<unsigned char>& g, Image<unsigned char>& b)
    {
        Image<unsigned char>::iIt itR= r.begin();
        Image<unsigned char>::iIt itG= g.begin();
        Image<unsigned char>::iIt itB= b.begin();

        Image<float>::iIt itL= l.begin();
        Image<float>::iIt itU= u.begin();
        Image<float>::iIt itV= v.begin();

        while ( itR != r.end() )
        {
            luv2rgb(*itL, *itU, *itV, *itR, *itG, *itB);

            ++itL;
            ++itU;
            ++itV;
            ++itR;
            ++itG;
            ++itB;
        }
    }

    void luv2xyz(float l, float u, float v, float& x, float& y, float& z)
    {
        float u_, v_;
        u_= u / (13 * l) + LUV_U;
        v_= v / (13 * l) + LUV_V;

        if ( l <= 8 )
            y= l * pow((3.0/29), 3);
        else
            y= pow(((l + 16)/116.0), 3);

        x= -(9*y*u_)/((u_ - 4)*v_ - u_ *v_);
        z= (9 * y - (15*v_*y) - v_*x)/(3*v_);
     }

    void luv2xyz(Image<float>& l, Image<float>& u, Image<float>& v, Image<float>& x, Image<float>& y, Image<float>& z)
    {
        Image<float>::iIt itL= l.begin();
        Image<float>::iIt itU= u.begin();
        Image<float>::iIt itV= v.begin();
        Image<float>::iIt itX= x.begin();
        Image<float>::iIt itY= y.begin();
        Image<float>::iIt itZ= z.begin();

        while ( itX != x.end() )
        {
            luv2xyz(*itL, *itU, *itV, *itX, *itY, *itZ);

            ++itL;
            ++itU;
            ++itV;
            ++itX;
            ++itY;
            ++itZ;
        }
    }

    void rgb2luv(unsigned char r, unsigned char g, unsigned char b, float& l, float& u, float& v)
    {
        float x, y, z;
        rgb2xyz(r, g, b, x, y, z);
        xyz2luv(x, y, z, l, u, v);
    }

    void rgb2luv(Image<unsigned char>& r, Image<unsigned char>& g, Image<unsigned char>& b, Image<float>& l, Image<float>& u, Image<float>& v)
    {
        Image<unsigned char>::iIt itR= r.begin();
        Image<unsigned char>::iIt itG= g.begin();
        Image<unsigned char>::iIt itB= b.begin();
        Image<float>::iIt itL= l.begin();
        Image<float>::iIt itU= u.begin();
        Image<float>::iIt itV= v.begin();

        while ( itR != r.end() )
        {
            rgb2luv(*itR, *itG, *itB, *itL, *itU, *itV);

            ++itR;
            ++itG;
            ++itB;
            ++itL;
            ++itU;
            ++itV;
        }
    }

    void rgb2luv(unsigned char r, unsigned char g, unsigned char b, unsigned char& l, unsigned char& u, unsigned char& v)
    {
        float x, y, z;
        float lt, ut, vt;

        rgb2xyz(r, g, b, x, y, z);
        xyz2luv(x, y, z, lt, ut, vt);

        l= (unsigned char)(lt*10);
        u= (unsigned char)((ut+100) / 200 * 255);
        v= (unsigned char)((vt+100) / 200 * 255);
    }

    void rgb2luv(Image<unsigned char>& r, Image<unsigned char>& g, Image<unsigned char>& b, Image<unsigned char>& l, Image<unsigned char>& u, Image<unsigned char>& v)
    {
        Image<unsigned char>::iIt itR= r.begin();
        Image<unsigned char>::iIt itG= g.begin();
        Image<unsigned char>::iIt itB= b.begin();
        Image<unsigned char>::iIt itL= l.begin();
        Image<unsigned char>::iIt itU= u.begin();
        Image<unsigned char>::iIt itV= v.begin();

        while ( itR != r.end() )
        {
            rgb2luv(*itR, *itG, *itB, *itL, *itU, *itV);

            ++itR;
            ++itG;
            ++itB;
            ++itL;
            ++itU;
            ++itV;
        }
    }

    void xyz2luv(float x, float y, float z, float& l, float& u, float& v)
    {
        float u_, v_;
        u_= LUV_U;
        v_= LUV_V;

        float uu, vv;
        if ( x + 15*y + 3*z == 0 )
        {
            uu= 0;
            vv= 0;
        }
        else
        {
            uu= 4*x/(x + 15*y + 3*z);
            vv= 9*y/(x + 15*y + 3*z);
        }

        float yy;
        yy= y;

        complex<float> fyc(yy, 0);
        complex<float> pfy= pow(fyc, (float)(1.0/3.0));

        float fy= real(pfy);

        if ( yy < 0.008856 )
            fy= yy*(29*29*29/(3*3*3));
        else
            fy= fy*116 - 16;

        l= fy;
        u= 13 * l * (uu - u_);
        v= 13 * l * (vv - v_);
    }

    void rgb2luv(float r, float g, float b, float& l, float& u, float& v)
    {
        float x, y, z;
        rgb2xyz(r, g, b, x, y, z);
        xyz2luv(x, y, z, l, u, v);
    }

    void xyz2luv(Image<float>& x, Image<float>& y, Image<float>& z, Image<float>& l, Image<float>& u, Image<float>& v)
    {
        Image<float>::iIt itX= x.begin();
        Image<float>::iIt itY= y.begin();
        Image<float>::iIt itZ= z.begin();
        Image<float>::iIt itL= l.begin();
        Image<float>::iIt itU= u.begin();
        Image<float>::iIt itV= v.begin();

        while ( itX != x.end() )
        {
            xyz2luv(*itX, *itY, *itZ, *itL, *itU, *itV);

            ++itL;
            ++itU;
            ++itV;
            ++itX;
            ++itY;
            ++itZ;
        }
    }

    void rgb2luv(Image<float>& r, Image<float>& g, Image<float>& b, Image<float>& l, Image<float>& u, Image<float>& v)
    {
        Image<float>::iIt itR= r.begin();
        Image<float>::iIt itG= g.begin();
        Image<float>::iIt itB= b.begin();
        Image<float>::iIt itL= l.begin();
        Image<float>::iIt itU= u.begin();
        Image<float>::iIt itV= v.begin();

        while ( itR != r.end() )
        {
            rgb2luv(*itR, *itG, *itB, *itL, *itU, *itV);

            ++itR;
            ++itG;
            ++itB;
            ++itL;
            ++itU;
            ++itV;
        }
    }

    void xyz2rgb(Image<float>& x, Image<float>& y, Image<float>& z, Image<float>& r, Image<float>& g, Image<float>& b)
    {
        Image<float>::iIt itX= x.begin();
        Image<float>::iIt itY= y.begin();
        Image<float>::iIt itZ= z.begin();
        Image<float>::iIt itR= r.begin();
        Image<float>::iIt itG= g.begin();
        Image<float>::iIt itB= b.begin();

        while ( itR != r.end() )
        {
            xyz2rgb(*itX, *itY, *itZ, *itR, *itG, *itB);

            ++itR;
            ++itG;
            ++itB;
            ++itX;
            ++itY;
            ++itZ;
        }
    }

    void xyz2rgb(Image<float>& x, Image<float>& y, Image<float>& z, Image<unsigned char>& r, Image<unsigned char>& g, Image<unsigned char>& b)
    {
        Image<float>::iIt itX= x.begin();
        Image<float>::iIt itY= y.begin();
        Image<float>::iIt itZ= z.begin();
        Image<unsigned char>::iIt itR= r.begin();
        Image<unsigned char>::iIt itG= g.begin();
        Image<unsigned char>::iIt itB= b.begin();

        while ( itR != r.end() )
        {
            xyz2rgb(*itX, *itY, *itZ, *itR, *itG, *itB);

            ++itR;
            ++itG;
            ++itB;
            ++itX;
            ++itY;
            ++itZ;
        }
    }

    void xyz2rgb(Image<unsigned char>& x, Image<unsigned char>& y, Image<unsigned char>& z, Image<unsigned char>& r, Image<unsigned char>& g, Image<unsigned char>& b)
    {
        Image<unsigned char>::iIt itX= x.begin();
        Image<unsigned char>::iIt itY= y.begin();
        Image<unsigned char>::iIt itZ= z.begin();
        Image<unsigned char>::iIt itR= r.begin();
        Image<unsigned char>::iIt itG= g.begin();
        Image<unsigned char>::iIt itB= b.begin();

        while ( itR != r.end() )
        {
            xyz2rgb(*itX, *itY, *itZ, *itR, *itG, *itB);

            ++itR;
            ++itG;
            ++itB;
            ++itX;
            ++itY;
            ++itZ;
        }
    }

    void xyz2rgb(float x, float y, float z, float& r, float& g, float& b)
    {
        //with D65 white point

        r= 3.2404542 * x + -1.5371385 * y + -0.4985314 * z;
        g= -0.9692660 * x + 1.8760108 * y + 0.0415560 * z;
        b= 0.0556434 * x + -0.2040259 * y + 1.0572252 * z;
    }

    void xyz2rgb(float x, float y, float z, unsigned char& r, unsigned char& g, unsigned char& b)
    {
        //with D65 white point

        float rt, gt, bt;

        xyz2rgb(x, y, z, rt, gt, bt);

        rt= rt / 1.204794* 255;
        gt= gt / 0.948292 * 255;
        bt= bt / 0.908916 * 255;

        if ( rt > 255 )
            rt= 255;
        if ( rt < 0 )
            rt= 0;

        if ( gt > 255 )
            gt= 255;
        if ( gt < 0 )
            gt= 0;

        if ( bt > 255 )
            bt= 255;
        if ( bt < 0 )
            bt= 0;

        r= (unsigned char)(rt);
        g= (unsigned char)(gt);
        b= (unsigned char)(bt);
    }

    void xyz2rgb(unsigned char x, unsigned char y, unsigned char z, unsigned char& r, unsigned char& g, unsigned char& b)
    {
        //with D65 white point

        float xt, yt, zt, rt, gt, bt;
        xt= x/255.0f * 0.950456;
        yt= y/255.0f;
        zt= z/255.0f * 1.088754;

        rt= (3.240479 * xt + -1.537150 * yt + -0.498535 * zt) / 1.2047843* 255;
        gt= (-0.969256 * xt + 1.875992 * yt + 0.041556 * zt) / 0.9483008 * 255;
        bt= (0.055648 * xt + -0.204043 * yt + 1.057311 * zt) / 0.9088427 * 255;

        r= (unsigned char)(rt < 0 ? 255 - rt : rt);
        g= (unsigned char)(gt < 0 ? 255 - rt : gt);
        b= (unsigned char)(bt < 0 ? 255 - rt : bt);
    }

    void rgb2xyz(Image<float>& r, Image<float>& g, Image<float>& b, Image<float>& x, Image<float>& y, Image<float>& z)
    {
        Image<float>::iIt itR= r.begin();
        Image<float>::iIt itG= g.begin();
        Image<float>::iIt itB= b.begin();
        Image<float>::iIt itX= x.begin();
        Image<float>::iIt itY= y.begin();
        Image<float>::iIt itZ= z.begin();

        while ( itR != r.end() )
        {
            rgb2xyz(*itR, *itG, *itB, *itX, *itY, *itZ);

            ++itR;
            ++itG;
            ++itB;
            ++itX;
            ++itY;
            ++itZ;
        }
    }

    void rgb2xyz(Image<unsigned char>& r, Image<unsigned char>& g, Image<unsigned char>& b, Image<float>& x, Image<float>& y, Image<float>& z)
    {
        Image<unsigned char>::iIt itR= r.begin();
        Image<unsigned char>::iIt itG= g.begin();
        Image<unsigned char>::iIt itB= b.begin();
        Image<float>::iIt itX= x.begin();
        Image<float>::iIt itY= y.begin();
        Image<float>::iIt itZ= z.begin();

        while ( itR != r.end() )
        {
            rgb2xyz(*itR, *itG, *itB, *itX, *itY, *itZ);

            ++itR;
            ++itG;
            ++itB;
            ++itX;
            ++itY;
            ++itZ;
        }
    }

    void rgb2xyz(Image<unsigned char>& r, Image<unsigned char>& g, Image<unsigned char>& b, Image<unsigned char>& x, Image<unsigned char>& y, Image<unsigned char>& z)
    {
        Image<unsigned char>::iIt itR= r.begin();
        Image<unsigned char>::iIt itG= g.begin();
        Image<unsigned char>::iIt itB= b.begin();
        Image<unsigned char>::iIt itX= x.begin();
        Image<unsigned char>::iIt itY= y.begin();
        Image<unsigned char>::iIt itZ= z.begin();

        while ( itR != r.end() )
        {
            rgb2xyz(*itR, *itG, *itB, *itX, *itY, *itZ);

            ++itR;
            ++itG;
            ++itB;
            ++itX;
            ++itY;
            ++itZ;
        }
    }

    void rgb2xyz(float r, float g, float b, float& x, float& y, float& z)
    {
        //with D65 white point

        x= 0.4124564 * r + 0.3575761 * g + 0.1916313 * b;
        y= 0.2126729 * r + 0.7151522 * g + 0.0865432 * b;
        z= 0.0193339 * r + 0.1119313 * g + 0.9518563 * b;
    }

    void rgb2xyz(unsigned char r, unsigned char g, unsigned char b, float& x, float& y, float& z)
    {
        //with D65 white point

        float rt, gt, bt;
        rt= r/255.0f;
        gt= g/255.0f;
        bt= b/255.0f;

        rgb2xyz(rt, gt, bt, x, y, z);
    }

    void rgb2xyz(unsigned char r, unsigned char g, unsigned char b, unsigned char& x, unsigned char& y, unsigned char& z)
    {
        //with D65 white point

        float rt, gt, bt;
        rt= r/255.0f;
        gt= g/255.0f;
        bt= b/255.0f;

        x= (unsigned char)((0.412453 * rt + 0.357580 * gt + 0.180423 * bt) / 0.950456 * 255);
        y= (unsigned char)((0.212671 * rt + 0.715160 * gt + 0.072169 * bt) * 255);
        z= (unsigned char)((0.019334 * rt + 0.119193 * gt + 0.950227 * bt) / 1.088754 * 255);
    }

    

    

    void rgb2ycc(unsigned char r, unsigned char g, unsigned char b, unsigned char& y, unsigned char& cb, unsigned char& cr)
    {
        y= (unsigned char)(16 + 1.0/256 * (65.738 * r + 129.057 * g + 25.064 * b));
        cb= (unsigned char)(128 + 1.0/256 * (-37.945 * r - 74.494 * g + 112.439 * b));
        cr= (unsigned char)(128 + 1.0/256 * (112.439 * r - 94.154 * g - 18.285 * b));
    }

    void ycc2rgb(unsigned char y, unsigned char cb, unsigned char cr, unsigned char& r, unsigned char& g, unsigned char& b)
    {
        r= (unsigned char)((298.082 * y + 408.583 * cr)/256.0 - 222.921);
        g= (unsigned char)((298.082 * y - 100.291 * cb - 208.120 * cr)/256.0 + 135.576);
        b= (unsigned char)((298.082 * y + 516.412 * cb )/256.0 - 276.836);
    }

    void rgb2ycc(Image<unsigned char>& r, Image<unsigned char>& g, Image<unsigned char>& b, Image<unsigned char>& y, Image<unsigned char>& cb, Image<unsigned char>& cr)
    {
        Image<unsigned char>::iIt itR= r.begin();
        Image<unsigned char>::iIt itG= g.begin();
        Image<unsigned char>::iIt itB= b.begin();
        Image<unsigned char>::iIt itY= y.begin();
        Image<unsigned char>::iIt itCb= cb.begin();
        Image<unsigned char>::iIt itCr= cr.begin();

        while ( itR != r.end() )
        {
            rgb2ycc(*itR, *itG, *itB, *itY, *itCb, *itCr);

            ++itR;
            ++itG;
            ++itB;
            ++itY;
            ++itCb;
            ++itCr;
        }
    }

    void ycc2rgb(Image<unsigned char>& y, Image<unsigned char>& cb, Image<unsigned char>& cr, Image<unsigned char>& r, Image<unsigned char>& g, Image<unsigned char>& b)
    {
        Image<unsigned char>::iIt itY= y.begin();
        Image<unsigned char>::iIt itCb= cb.begin();
        Image<unsigned char>::iIt itCr= cr.begin();
        Image<unsigned char>::iIt itR= r.begin();
        Image<unsigned char>::iIt itG= g.begin();
        Image<unsigned char>::iIt itB= b.begin();

        while ( itR != r.end() )
        {
            ycc2rgb(*itY, *itCb, *itCr, *itR, *itG, *itB);

            ++itR;
            ++itG;
            ++itB;
            ++itY;
            ++itCb;
            ++itCr;
        }
    }

    void hsv2rgb(short H, short S, short V, unsigned char& R, unsigned char& G, unsigned char& B)
    {
        float s= S/255.0f;
        float v= V/255.0f;

        int hi= (int)floor(H/60.0f) % 6;
        float f= H/60.0f - (int)floor(H/60.0f);

        float p= v*(1-s);
        float q= v*(1-f*s);
        float t= v*(1-(1-f)*s);

        if ( hi == 0 ) { R= (unsigned char)(v*255); G= (unsigned char)(t*255); B= (unsigned char)(p*255); }
        if ( hi == 1 ) { R= (unsigned char)(q*255); G= (unsigned char)(v*255); B= (unsigned char)(p*255); }
        if ( hi == 2 ) { R= (unsigned char)(p*255); G= (unsigned char)(v*255); B= (unsigned char)(t*255); }
        if ( hi == 3 ) { R= (unsigned char)(p*255); G= (unsigned char)(q*255); B= (unsigned char)(v*255); }
        if ( hi == 4 ) { R= (unsigned char)(t*255); G= (unsigned char)(p*255); B= (unsigned char)(v*255); }
        if ( hi == 5 ) { R= (unsigned char)(v*255); G= (unsigned char)(p*255); B= (unsigned char)(q*255); }
    }

    void rgb2hsv(unsigned char R, unsigned char G, unsigned char B, short& H, short& S, short& V)
    {
        float r= R/255.0f, g= G/255.0f, b= B/255.0f;
        float h, s, v;

        float max= r;
        max= max > g ? max : g;
        max= max > b ? max : b;

        float min= r;
        min= min < g ? min : g;
        min= min < b ? min : b;

        if ( max == min )
            h= 0;
        else if ( fabs(max - r) < 0.00001 )
        {
            h= (float)((int)((60 * (g - b) / (max - min) +360)) % 360);
        }
        else if ( fabs(max - g) < 0.00001 )
        {
            h= (60 * (b - r) / (max - min) +120);
        }
        else
        {
            h= (60 * (r - g) / (max - min) +240);
        }

        v= max;

        if ( max == 0 )
            s= 0;
        else
            s= (max - min)/max;

        H= (short)h;
        S= (short)(s*255);
        V= (short)(v*255);
    }

    void rgb2hsv(Image<unsigned char>& r, Image<unsigned char>& g, Image<unsigned char>&b, Image<short>& h, Image<short>& s, Image<short>&v)
    {
        Image<unsigned char>::iIt itR= r.begin();
        Image<unsigned char>::iIt itG= g.begin();
        Image<unsigned char>::iIt itB= b.begin();
        Image<short>::iIt itH= h.begin();
        Image<short>::iIt itS= s.begin();
        Image<short>::iIt itV= v.begin();

        while( itR != r.end() )
        {
            rgb2hsv(*itR, *itG, *itB, *itH, *itS, *itV);
            ++itR;
            ++itG;
            ++itB;
            ++itH;
            ++itS;
            ++itV;
        }
    }

    void hsv2rgb(Image<short>& h, Image<short>& s, Image<short>&v, Image<unsigned char>& r, Image<unsigned char>& g, Image<unsigned char>&b)
    {
        Image<short>::iIt hIt= h.begin();
        Image<short>::iIt sIt= s.begin();
        Image<short>::iIt vIt= v.begin();
        Image<unsigned char>::iIt itR= r.begin();
        Image<unsigned char>::iIt itG= g.begin();
        Image<unsigned char>::iIt itB= b.begin();

        while( itR != r.end() )
        {
            hsv2rgb(*hIt, *sIt, *vIt, *itR, *itG, *itB);
            ++hIt;
            ++sIt;
            ++vIt;
            ++itR;
            ++itG;
            ++itB;
        }
    }

    void hsl2rgb(short H, short S, short L, unsigned char& R, unsigned char& G, unsigned char& B)
    {
        float s= S/255.0f;
        float l= L/255.0f;

        float r, g, b;

        float q, p, hk, tr, tg, tb;

        if ( l < 0.5 )
            q= l*(1 + s);
        else
            q= l+s - l*s;

        p= 2*l - q;

        hk= H/360.0f;

        tr= hk + 1/3.0f;
        tg= hk;
        tb= hk - 1/3.0f;

        if ( tr < 0 ) 
            tr+= 1.0; 
        if ( tr > 1 ) 
            tr-= 1.0;
        if ( tg < 0 ) 
            tg+= 1.0; 
        if ( tg > 1 ) 
            tg-= 1.0;
        if ( tb < 0 ) 
            tb+= 1.0; 
        if ( tb > 1 ) 
            tb-= 1.0;

        if ( tr < 1/6.0 )
            r= p + ((q-p)*6*tr);
        else if ( tr < 1/2.0 )
            r= q;
        else if ( tr < 2/3.0 )
            r= p + ((q-p)*6*(2/3.0f-tr));
        else
            r= p;

        if ( tg < 1/6.0 )
            g= p + ((q-p)*6*tg);
        else if ( tg < 1/2.0 )
            g= q;
        else if ( tg < 2/3.0 )
            g= p + ((q-p)*6*(2/3.0f-tg));
        else
            g= p;

        if ( tb < 1/6.0 )
            b= p + ((q-p)*6*tb);
        else if ( tb < 1/2.0 )
            b= q;
        else if ( tb < 2/3.0 )
            b= p + ((q-p)*6*(2/3.0f-tb));
        else
            b= p;

        R= (unsigned char)(r*255);
        G= (unsigned char)(g*255);
        B= (unsigned char)(b*255);
    }

    void rgb2hsl(unsigned char R, unsigned char G, unsigned char B, short& H, short& S, short& V)
    {
        float r= R/255.0f, g= G/255.0f, b= B/255.0f;
        float h, s, v;

        float max= r;
        max= max > g ? max : g;
        max= max > b ? max : b;

        float min= r;
        min= min < g ? min : g;
        min= min < b ? min : b;

        if ( max == min )
            h= 0;
        else if ( fabs(max - r) < 0.00001 )
        {
            h= (float)((int)((60 * (g - b) / (max - min) +360)) % 360);
        }
        else if ( fabs(max - g) < 0.00001 )
        {
            h= (60 * (b - r) / (max - min) +120);
        }
        else
        {
            h= (60 * (r - g) / (max - min) +240);
        }

        v= 0.5f*(max + min);

        if ( max == min )
            s= 0;
        else if ( v <= 0.5 )
        {
            s= (max - min)/(max + min);
        }
        else
            s= (max - min)/(2-(max + min));

        H= (short)h;
        S= (short)(s*255);
        V= (short)(v*255);
    }

    void rgb2hsl(Image<unsigned char>& r, Image<unsigned char>& g, Image<unsigned char>&b, Image<short>& h, Image<short>& s, Image<short>&v)
    {
        Image<unsigned char>::iIt itR= r.begin();
        Image<unsigned char>::iIt itG= g.begin();
        Image<unsigned char>::iIt itB= b.begin();
        Image<short>::iIt itH= h.begin();
        Image<short>::iIt itS= s.begin();
        Image<short>::iIt itV= v.begin();

        while( itR != r.end() )
        {
            rgb2hsl(*itR, *itG, *itB, *itH, *itS, *itV);
            ++itR;
            ++itG;
            ++itB;
            ++itH;
            ++itS;
            ++itV;
        }
    }

    void hsl2rgb(Image<short>& h, Image<short>& s, Image<short>&v, Image<unsigned char>& r, Image<unsigned char>& g, Image<unsigned char>&b)
    {
        Image<unsigned char>::iIt itR= r.begin();
        Image<unsigned char>::iIt itG= g.begin();
        Image<unsigned char>::iIt itB= b.begin();
        Image<short>::iIt itH= h.begin();
        Image<short>::iIt itS= s.begin();
        Image<short>::iIt itV= v.begin();

        for ( int i= 0; i < (int)(r.n); ++i )
        {
            hsl2rgb(*itH, *itS, *itV, *itR, *itG, *itB);
            ++itH;
            ++itS;
            ++itV;
            ++itR;
            ++itG;
            ++itB;
        }
    }

    void invert(Image<unsigned char>& b)
    {
        Image<unsigned char>::iIt itB= b.begin();
        while( itB != b.end() )
        {
            *itB= 255 - *itB;
            ++itB;
        }
    }
}
