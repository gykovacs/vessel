/* 
 * File:   Scaling.h
 * Author: gykovacs
 *
 * Created on August 1, 2011, 10:25 AM
 */

#ifndef SCALING_H
#define	SCALING_H

#include <openipLL/Interpolation.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    void bicubicScaling(Image<INPUT>& input, Image<OUTPUT>& output)
    {
        float dr, dc;
        dc= float(input.columns - input.leftBorder - input.rightBorder)/(output.columns - output.leftBorder - output.rightBorder);
        dr= float(input.rows - input.topBorder - input.bottomBorder)/(output.rows - output.topBorder - output.bottomBorder);
        INPUT min, max;
        input.getMinMax(min, max);
        OUTPUT tmp;

        int ca, cb, ra, rb;

        float tdr= (output.topBorder) * dr;
        float tdc= (output.leftBorder) * dc;
        float p[16]= {0};

        //#pragma omp parallel for private(p, ca, cb, ra, rb, tdr, tdc), firstprivate(oldca)
        for ( int r= output.topBorder; r < output.rows - output.topBorder; ++r )
        {
            tdr= (r - output.topBorder) * dr;
            ra= floor(tdr);
            rb= ra + 1;

            tdc= 0;
            for ( int c= output.leftBorder; c < output.columns - output.rightBorder; ++c )
            {
                ca= floor(tdc);
                cb= ca + 1;

                //if ( oldca != ca )
		if ( ra > 0 && ra < input.rows-1 && ca > 0 && ca < input.columns-1 )
                {
                    p[5]= input(input.topBorder + ra, input.leftBorder + ca);
                    p[9]= input(input.topBorder + rb, input.leftBorder + ca);
                    p[6]= input(input.topBorder + ra, input.leftBorder + cb);
                    p[10]= input(input.topBorder + rb, input.leftBorder + cb);
                    p[0]= input(input.topBorder + ra-1, input.leftBorder + ca-1);
                    p[1]= input(input.topBorder + ra-1, input.leftBorder + ca);
                    p[2]= input(input.topBorder + ra-1, input.leftBorder + cb);
                    p[3]= input(input.topBorder + ra-1, input.leftBorder + cb+1);
                    p[4]= input(input.topBorder + ra, input.leftBorder + ca-1);
                    p[7]= input(input.topBorder + ra, input.leftBorder + cb+1);
                    p[8]= input(input.topBorder + rb, input.leftBorder + ca-1);
                    p[11]= input(input.topBorder + rb, input.leftBorder + cb+1);
                    p[12]= input(input.topBorder + rb+1, input.leftBorder + ca-1);
                    p[13]= input(input.topBorder + rb+1, input.leftBorder + ca);
                    p[14]= input(input.topBorder + rb+1, input.leftBorder + cb);
                    p[15]= input(input.topBorder + rb+1, input.leftBorder + cb+1);
                }
                else
		{
		  for ( int i= 0; i < 16; ++i )
		    p[i]= 0;
		}
                tmp= bicubicInterpolate(p, tdr - floor(tdr), tdc - floor(tdc));
                if ( tmp < min )
                    output(r, c)= min;
                else if ( tmp > max )
                    output(r, c)= max;
                else
                    output(r, c)= tmp;

                tdc+= dc;
                //oldca= ca;
            }
            //tdr+= dr;
            //oldca= ca;
        }

        output.fillBorder();
    }
    
    template<typename INPUT, typename OUTPUT>
    void multiBicubicScaling(Image<INPUT>& input, Image<OUTPUT>& output)
    {
        Image<INPUT> tmp0;
	Image<INPUT> tmp1;
	tmp0.resizeImage(input);
	tmp1.resizeImage(output);
	tmp0= input;
	tmp1= output;
	
	while ( float(tmp0.columns)/float(tmp1.columns) > 1.5 )
	{
	  tmp1.removeBorder();
	  tmp0.removeBorder();
	  tmp1.resizeImage(tmp0.rows/1.5, tmp0.columns/1.5);
	  tmp0.setBorder(input.getBorder2());
	  tmp1.setBorder(input.getBorder2());
	  
	  bicubicScaling(tmp0, tmp1);
	  
	  tmp0= tmp1;
	}
        bicubicScaling(tmp0, output);
    }

    template<typename INPUT, typename OUTPUT>
    void bilinearScaling(Image<INPUT>& input, Image<OUTPUT>& output)
    {
        float dr, dc;
        dc= float(input.columns - input.leftBorder - input.rightBorder)/(output.columns - output.leftBorder - output.rightBorder);
        dr= float(input.rows - input.topBorder - input.bottomBorder)/(output.rows - output.topBorder - output.bottomBorder);

        int ca, cb, ra, rb/*, oldca= -1*/;
        //float p[4];
        float tdr= (output.topBorder) * dr;
        float tdc= (output.leftBorder) * dc;

        //#pragma omp parallel for private(ca, cb, ra, rb, tdr, tdc)
        for ( int r= output.topBorder; r < output.rows - output.topBorder; ++r )
        {
            tdr= (r - output.topBorder) * dr;
            ra= floor(tdr);
            rb= ra + 1;

            tdc= 0;
            for ( int c= output.leftBorder; c < output.columns - output.rightBorder; ++c )
            {
                ca= floor(tdc);
                cb= ca + 1;

                //if ( oldca != ca )
/*                {
                    p[0]= input(input.topBorder + ra, input.leftBorder + ca);
                    p[1]= input(input.topBorder + ra, input.leftBorder + cb);
                    p[2]= input(input.topBorder + rb, input.leftBorder + ca);
                    p[3]= input(input.topBorder + rb, input.leftBorder + cb);
                }*/
                output(r, c)= bilinearInterpolate2(input(input.topBorder + ra, input.leftBorder + ca), 
                        input(input.topBorder + ra, input.leftBorder + cb), 
                        input(input.topBorder + rb, input.leftBorder + ca), 
                        input(input.topBorder + rb, input.leftBorder + cb), 
                        tdr - floor(tdr), tdc - floor(tdc));

                tdc+= dc;
                //oldca= ca;
            }
        }

        output.fillBorder();
    }

    template<typename INPUT, typename OUTPUT>
    void binearestNeighborScaling(Image<INPUT>& input, Image<OUTPUT>& output)
    {
        float dr, dc;
        dc= float(input.columns - input.leftBorder - input.rightBorder)/(output.columns - output.leftBorder - output.rightBorder);
        dr= float(input.rows - input.topBorder - input.bottomBorder)/(output.rows - output.topBorder - output.bottomBorder);

        int ca, cb, ra, rb/*, oldca= -1*/;
        float p[4]={0};
        float tdr= (output.topBorder) * dr;
        float tdc= (output.leftBorder) * dc;

        #pragma omp parallel for private(p, ca, cb, ra, rb, tdr, tdc)
        for ( int r= output.topBorder; r < output.rows - output.topBorder; ++r )
        {
            tdr= (r - output.topBorder) * dr;
            ra= floor(tdr);
            rb= ra + 1;

            //tdc= 0;
            for ( int c= output.leftBorder; c < output.columns - output.rightBorder; ++c )
            {
		tdc= (c - output.leftBorder) * dc;
                ca= floor(tdc);
                cb= ca + 1;

                //if ( oldca != ca )
                if ( rb >= input.rows - input.bottomBorder && cb >= input.columns - input.rightBorder )
                {
                    p[0]= input(input.topBorder + ra, input.leftBorder + ca);
                    p[1]= input(input.topBorder + ra, input.leftBorder + ca);
                    p[2]= input(input.topBorder + ra, input.leftBorder + ca);
                    p[3]= input(input.topBorder + ra, input.leftBorder + ca);
                }
                else if ( rb >= input.rows - input.bottomBorder )
                {
                    p[0]= input(input.topBorder + ra, input.leftBorder + ca);
                    p[1]= input(input.topBorder + ra, input.leftBorder + cb);
                    p[2]= input(input.topBorder + ra, input.leftBorder + ca);
                    p[3]= input(input.topBorder + ra, input.leftBorder + cb);
                }
                else if ( cb >= input.columns - input.rightBorder )
                {
                    p[0]= input(input.topBorder + ra, input.leftBorder + ca);
                    p[1]= input(input.topBorder + ra, input.leftBorder + ca);
                    p[2]= input(input.topBorder + rb, input.leftBorder + ca);
                    p[3]= input(input.topBorder + rb, input.leftBorder + ca);
                }
                else if ( rb < input.rows - input.bottomBorder && cb < input.columns - input.rightBorder )
                {
                    p[0]= input(input.topBorder + ra, input.leftBorder + ca);
                    p[1]= input(input.topBorder + ra, input.leftBorder + cb);
                    p[2]= input(input.topBorder + rb, input.leftBorder + ca);
                    p[3]= input(input.topBorder + rb, input.leftBorder + cb);
                }
                output(r, c)= binearestNeighborInterpolate(p, tdr - floor(tdr), tdc - floor(tdc));

                //tdc+= dc;
                //oldca= ca;
            }
        }

        output.fillBorder();
    }
    
    template<typename INPUT, typename OUTPUT>
    class BicubicScaling: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::getProposedBorder;

        BicubicScaling();

        BicubicScaling(const BicubicScaling& b);

        ~BicubicScaling();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual Border2 getProposedBorder();
    };

    template<typename INPUT, typename OUTPUT>
    BicubicScaling<INPUT, OUTPUT>::BicubicScaling()
    : Transform2<INPUT, OUTPUT>()
    {
        descriptor= std::string("BicubicScaling ");
    }

    template<typename INPUT, typename OUTPUT>
    BicubicScaling<INPUT, OUTPUT>::BicubicScaling(const BicubicScaling& b)
    : Transform2<INPUT, OUTPUT>(b)
    {
    }

    template<typename INPUT, typename OUTPUT>
    BicubicScaling<INPUT, OUTPUT>::~BicubicScaling()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void BicubicScaling<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        bicubicScaling(input, output);
    }

    template<typename INPUT, typename OUTPUT>
    Border2 BicubicScaling<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(1, 1, 1, 1);
    }

    template<typename INPUT, typename OUTPUT>
    class NearestNeighborScaling: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::getProposedBorder;

        NearestNeighborScaling();

        NearestNeighborScaling(const NearestNeighborScaling& b);

        ~NearestNeighborScaling();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual Border2 getProposedBorder();
    };

    template<typename INPUT, typename OUTPUT>
    NearestNeighborScaling<INPUT, OUTPUT>::NearestNeighborScaling()
    : Transform2<INPUT, OUTPUT>()
    {
        descriptor= std::string("NearestNeighborScaling ");
    }

    template<typename INPUT, typename OUTPUT>
    NearestNeighborScaling<INPUT, OUTPUT>::NearestNeighborScaling(const NearestNeighborScaling& b)
    : Transform2<INPUT, OUTPUT>(b)
    {
    }

    template<typename INPUT, typename OUTPUT>
    NearestNeighborScaling<INPUT, OUTPUT>::~NearestNeighborScaling()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void NearestNeighborScaling<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        binearestNeighborScaling(input, output);
    }

    template<typename INPUT, typename OUTPUT>
    Border2 NearestNeighborScaling<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(1, 1, 1, 1);
    }

    template<typename INPUT, typename OUTPUT>
    class BilinearScaling: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::getProposedBorder;

        BilinearScaling();

        BilinearScaling(const BilinearScaling& b);

        ~BilinearScaling();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual Border2 getProposedBorder();
    };

    template<typename INPUT, typename OUTPUT>
    BilinearScaling<INPUT, OUTPUT>::BilinearScaling()
    : Transform2<INPUT, OUTPUT>()
    {
        descriptor= std::string("BilinearScaling ");
    }

    template<typename INPUT, typename OUTPUT>
    BilinearScaling<INPUT, OUTPUT>::BilinearScaling(const BilinearScaling& b)
    : Transform2<INPUT, OUTPUT>(b)
    {
    }

    template<typename INPUT, typename OUTPUT>
    BilinearScaling<INPUT, OUTPUT>::~BilinearScaling()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void BilinearScaling<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        bilinearScaling(input, output);
    }

    template<typename INPUT, typename OUTPUT>
    Border2 BilinearScaling<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(1, 1, 1, 1);
    }
    
    template<typename INPUT>
    void bicubicResize(Image<INPUT>& input, int rows, int columns, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED)
    {
        if ( input.rows != rows || input.columns != columns || input.topBorder != topBorder || input.bottomBorder != bottomBorder || input.leftBorder != leftBorder || input.rightBorder != rightBorder )
        {
            Image<INPUT> tmp(input);
            input.resizeImage(rows, columns, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
            bicubicScaling(tmp, input);
        }
    }
    
    template<typename INPUT, typename T>
    void bicubicResize(Image<INPUT>& input, Image<T>& pattern)
    {
        if ( input.rows != pattern.rows || input.columns != pattern.columns || input.topBorder != pattern.topBorder || input.bottomBorder != pattern.bottomBorder || input.leftBorder != pattern.leftBorder || input.rightBorder != pattern.rightBorder )
        {
            Image<INPUT> tmp(input);
            input.resizeImage(pattern.rows - pattern.topBorder - pattern.bottomBorder, pattern.columns - pattern.leftBorder - pattern.rightBorder, pattern.topBorder, pattern.bottomBorder, pattern.leftBorder, pattern.rightBorder, pattern.borderMode);
            bicubicScaling(tmp, input);
        }
    }

    template<typename INPUT>
    void bilinearResize(Image<INPUT>& input, int rows, int columns, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED)
    {
        if ( input.rows != rows || input.columns != columns || input.topBorder != topBorder || input.bottomBorder != bottomBorder || input.leftBorder != leftBorder || input.rightBorder != rightBorder )
        {
            Image<INPUT> tmp(input);
            input.resizeImage(rows - topBorder - bottomBorder, columns - leftBorder - rightBorder, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
            bilinearScaling(tmp, input);
        }
    }
    
    template<typename INPUT, typename T>
    void bilinearResize(Image<INPUT>& input, Image<T>& pattern)
    {
        if ( input.rows != pattern.rows || input.columns != pattern.columns || input.topBorder != pattern.topBorder || input.bottomBorder != pattern.bottomBorder || input.leftBorder != pattern.leftBorder || input.rightBorder != pattern.rightBorder )
        {
            Image<INPUT> tmp(input);
            input.resizeImage(pattern.rows - pattern.topBorder - pattern.bottomBorder, pattern.columns - pattern.leftBorder - pattern.rightBorder, pattern.topBorder, pattern.bottomBorder, pattern.leftBorder, pattern.rightBorder, pattern.borderMode);
            bilinearScaling(tmp, input);
        }
    }

    template<typename INPUT>
    void binearestNeighborResize(Image<INPUT>& input, int rows, int columns, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED)
    {
        if ( input.rows != rows || input.columns != columns || input.topBorder != topBorder || input.bottomBorder != bottomBorder || input.leftBorder != leftBorder || input.rightBorder != rightBorder )
        {
            Image<INPUT> tmp(input);
            input.resizeImage(rows, columns, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
            binearestNeighborScaling(tmp, input);
        }
    }

    template<typename INPUT, typename T>
    void binearestNeighborResize(Image<INPUT>& input, Image<T>& pattern)
    {
        if ( input.rows != pattern.rows || input.columns != pattern.columns || input.topBorder != pattern.topBorder || input.bottomBorder != pattern.bottomBorder || input.leftBorder != pattern.leftBorder || input.rightBorder != pattern.rightBorder )
        {
            Image<INPUT> tmp(input);
            input.resizeImage(pattern.rows - pattern.topBorder - pattern.bottomBorder, pattern.columns - pattern.leftBorder - pattern.rightBorder, pattern.topBorder, pattern.bottomBorder, pattern.leftBorder, pattern.rightBorder, pattern.borderMode);
            binearestNeighborScaling(tmp, input);
        }
    }

    
    template<typename INPUT, typename OUTPUT>
    class ScalingToRoiDiameter: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::getProposedBorder;

        ScalingToRoiDiameter(int diam);

        ScalingToRoiDiameter(const ScalingToRoiDiameter& b);

        ~ScalingToRoiDiameter();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual Border2 getProposedBorder();
        
        int diam;
    };

    template<typename INPUT, typename OUTPUT>
    Border2 ScalingToRoiDiameter<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(1,1,1,1,BORDER_MODE_MIRRORED);
    }
    
    template<typename INPUT, typename OUTPUT>
    ScalingToRoiDiameter<INPUT, OUTPUT>::ScalingToRoiDiameter(int diam)
    : Transform2<INPUT, OUTPUT>()
    {
        this->diam= diam;
        
        std::stringstream ss;
        ss << "ScalingToRoiDiameter " << diam;
        descriptor= ss.str();
    }
    
    template<typename INPUT, typename OUTPUT>
    ScalingToRoiDiameter<INPUT, OUTPUT>::ScalingToRoiDiameter(const ScalingToRoiDiameter<INPUT, OUTPUT>& s)
    : Transform2<INPUT, OUTPUT>(s)
    {
        this->diam= s.diam;
    }
    
    template<typename INPUT, typename OUTPUT>
    ScalingToRoiDiameter<INPUT, OUTPUT>::~ScalingToRoiDiameter()
    {
    }
    
    template<typename INPUT, typename OUTPUT>
    void ScalingToRoiDiameter<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        if ( roi )
        {
            int minr= INT_MAX, maxr= -INT_MAX, minc= INT_MAX, maxc= -INT_MAX;
            for ( int r= roi->topBorder; r < roi->rows - roi->bottomBorder; ++r )
                for ( int c= roi->leftBorder; c < roi->columns - roi->bottomBorder; ++c )
                {
                    if ( (*roi)(r, c) )
                    {
                        if ( r < minr )
                            minr= r;
                        if ( r > maxr )
                            maxr= r;
                        if ( c < minc )
                            minc= c;
                        if ( c > maxc )
                            maxc= c;
                    }
                }
            int diameter= (maxr - minr) > (maxc - minc) ? (maxr - minr) : (maxc - minc);

            int nrows= float(roi->rows - roi->topBorder - roi->bottomBorder) / diameter * diam;
            int ncolumns= float(roi->columns - roi->leftBorder - roi->rightBorder) / diameter * diam;

            bicubicResize(input, nrows, ncolumns, input.topBorder, input.bottomBorder, input.leftBorder, input.rightBorder);

            output.resizeImage(input.rows - input.topBorder - input.bottomBorder, input.columns - input.leftBorder - input.rightBorder, input.topBorder, input.bottomBorder, input.leftBorder, input.rightBorder);

            binearestNeighborResize(*roi, nrows, ncolumns, input.topBorder, input.bottomBorder, input.leftBorder, input.rightBorder, BORDER_MODE_ZERO);

            if ( support )
            {
                binearestNeighborResize(*support, nrows, ncolumns, input.topBorder, input.bottomBorder, input.leftBorder, input.rightBorder, BORDER_MODE_ZERO);
                printf("resize support\n");
            }
            output= input;
        }
        else
            output= input;
    }
}


#endif	/* SCALING_H */

