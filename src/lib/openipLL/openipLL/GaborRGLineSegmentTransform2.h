#ifndef GABORRGLINESEGMENTTRANSFORM2_H
#define GABORRGLINESEGMENTTRANSFORM2_H

#include <float.h>

#include <openipDS/Image.h>

#include <openipLL/morphology.h>
#include <openipLL/imageIO.h>
#include <openipLL/ROIDetector.h>
#include <openipLL/ComponentLabeling.h>
#include <openipML/statisticalFunctions.h>
#include <openipML/StatisticalTests.h>
#include <openipML/Noises.h>
#include <openipDS/Stopper.h>
#include <openipDS/GaborFilter2.h>
#include <openipDS/MatchedGaborFilter2.h>
#include <openipDS/drawing.h>
#include <openipDS/OrientationEstimators.h>
#include <openipLL/NeighborhoodRegionGrowing.h>

namespace openip
{
   template<typename INPUT, typename OUTPUT>
    class GaborRGLineSegmentTransform2: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::apply;

        /**
         * default constructor
         */
        GaborRGLineSegmentTransform2(float th1, float th2, float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma);

        /**
         * copy constructor
         * @param a instance to copy
         */
        GaborRGLineSegmentTransform2(const GaborRGLineSegmentTransform2& a);

        /**
         * destructor
         */
        ~GaborRGLineSegmentTransform2();

        /**
         * estimates the orientation of the image content in the foreground (non-zero) region of the mask
         * @param input input image
         * @param output output image
         * @param mask the operation is performed in the foreground (non-zero) region of the mask
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual Border2 getProposedBorder();

        MatchedCCorrelationGaborFilterR2<INPUT, OUTPUT>* mgf;
        NeighborhoodRegionGrowing<OUTPUT>* rg;

        float th1;
        float th2;
        float sigma;
        float theta0;
        float step;
        float theta1;
        float lambda;
        float psi;
        float gamma;
    };

    template<typename INPUT, typename OUTPUT>
    GaborRGLineSegmentTransform2<INPUT, OUTPUT>::GaborRGLineSegmentTransform2(float th1_, float th2_, float sigma_, float theta0_, float step_, float theta1_, float lambda_, float psi_, float gamma_)
        : Transform2<INPUT, OUTPUT>(), th1(th1_), th2(th2_), sigma(sigma_), theta0(theta0_), step(step_), theta1(theta1_), lambda(lambda_), psi(psi_), gamma(gamma_)
    {
        mgf= new MatchedCCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, theta0, step, theta1, lambda, psi, gamma);
        rg= new NeighborhoodRegionGrowing<OUTPUT>(REGION_GROWING_HARD_THRESHOLD, th2);

        std::stringstream ss;
        ss << "GaborRGLineSegmentTransform2 " << th1 << " " << th2 << " " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GaborRGLineSegmentTransform2<INPUT, OUTPUT>::GaborRGLineSegmentTransform2(const GaborRGLineSegmentTransform2& a)
    : Transform2<INPUT, OUTPUT>(a)
    {
    }

    template<typename INPUT, typename OUTPUT>
    GaborRGLineSegmentTransform2<INPUT, OUTPUT>::~GaborRGLineSegmentTransform2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void GaborRGLineSegmentTransform2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>*)
    {
        if ( th2 > th1 )
        {
            output= 0;
            return;
        }

        mgf->updateStride(input.columns);

        Image<OUTPUT> filtered;
        filtered.resizeImage(input);
        mgf->apply(input, filtered, roi);

        Image<unsigned char> seed;
        seed.resizeImage(input);
        Image<unsigned char> rgimage;
        rgimage.resizeImage(input);

        for ( unsigned int i= 0; i < filtered.n; ++i )
            if ( filtered(i) > th1 && (roi == NULL || (*roi)(i)))
                seed(i)= 255;
            else
                seed(i)= 0;

        rg->doRG(filtered, rgimage, seed, roi);

        /*for ( int i= 0; i < filtered.n; ++i )
            output(i)= rgimage(i);*/
        output= rgimage;
    }

    template<typename INPUT, typename OUTPUT>
    Border2 GaborRGLineSegmentTransform2<INPUT, OUTPUT>::getProposedBorder()
    {
        return mgf->getProposedBorder();
    }




     template<typename INPUT, typename OUTPUT>
     class PowerGaborRGLineSegmentTransform2: public Transform2<INPUT, OUTPUT>
     {
     public:
         using Transform2<INPUT, OUTPUT>::descriptor;
         using Transform2<INPUT, OUTPUT>::apply;

         /**
          * default constructor
          */
         PowerGaborRGLineSegmentTransform2(float th1, float th2, float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma, float powerFactor, float scale= 1);

         /**
          * copy constructor
          * @param a instance to copy
          */
         PowerGaborRGLineSegmentTransform2(const PowerGaborRGLineSegmentTransform2& a);

         /**
          * destructor
          */
         ~PowerGaborRGLineSegmentTransform2();
	 
	 void regenerate();
	 
	 void regenerate2();

         /**
          * estimates the orientation of the image content in the foreground (non-zero) region of the mask
          * @param input input image
          * @param output output image
          * @param mask the operation is performed in the foreground (non-zero) region of the mask
          */
         virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
	 
	 virtual void applyGetFiltered(Image<INPUT>& input, Image<OUTPUT>& output, Image<OUTPUT>& filtered, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
	 
	 virtual void apply2(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

         virtual Border2 getProposedBorder();

         MatchedCCorrelationPowerGaborFilterR2<INPUT, OUTPUT>* mgf;
         NeighborhoodRegionGrowing<OUTPUT>* rg;

         float th1;
         float th2;
         float sigma;
         float theta0;
         float step;
         float theta1;
         float lambda;
         float psi;
         float gamma;
         float powerFactor;
	 float scale;
	 
	 float meanCorr1;
	 float meanCorr2;
	 float meanCorr3;
     };

     template<typename INPUT, typename OUTPUT>
     PowerGaborRGLineSegmentTransform2<INPUT, OUTPUT>::PowerGaborRGLineSegmentTransform2(float th1_, float th2_, float sigma_, float theta0_, float step_, float theta1_, float lambda_, float psi_, float gamma_, float powerFactor_, float scale_)
         : Transform2<INPUT, OUTPUT>(), th1(th1_), th2(th2_), sigma(sigma_), theta0(theta0_), step(step_), theta1(theta1_), lambda(lambda_), psi(psi_), gamma(gamma_), powerFactor(powerFactor_), scale(scale_)
     {
         mgf= new MatchedCCorrelationPowerGaborFilterR2<INPUT, OUTPUT>(sigma, theta0, step, theta1, lambda, psi, gamma, powerFactor, scale);
         rg= new NeighborhoodRegionGrowing<OUTPUT>(REGION_GROWING_HARD_THRESHOLD, th2);

         std::stringstream ss;
         ss << "PowerGaborRGLineSegmentTransform2 " << th1 << " " << th2 << " " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma << " " << powerFactor;
         descriptor= ss.str();
     }
     
     template<typename INPUT, typename OUTPUT>
     void PowerGaborRGLineSegmentTransform2<INPUT, OUTPUT>::regenerate()
     {
       delete mgf;
       mgf= new MatchedCCorrelationPowerGaborFilterR2<INPUT, OUTPUT>(sigma, theta0, step, theta1, lambda, psi, gamma, powerFactor, scale);
       delete rg;
       rg= new NeighborhoodRegionGrowing<OUTPUT>(REGION_GROWING_HARD_THRESHOLD, th2);
     }
     
     template<typename INPUT, typename OUTPUT>
     void PowerGaborRGLineSegmentTransform2<INPUT, OUTPUT>::regenerate2()
     {
       delete rg;
       rg= new NeighborhoodRegionGrowing<OUTPUT>(REGION_GROWING_HARD_THRESHOLD, th2);
     }

     template<typename INPUT, typename OUTPUT>
     PowerGaborRGLineSegmentTransform2<INPUT, OUTPUT>::PowerGaborRGLineSegmentTransform2(const PowerGaborRGLineSegmentTransform2& a)
     : Transform2<INPUT, OUTPUT>(a)
     {
     }

     template<typename INPUT, typename OUTPUT>
     PowerGaborRGLineSegmentTransform2<INPUT, OUTPUT>::~PowerGaborRGLineSegmentTransform2()
     {
     }

     template<typename INPUT, typename OUTPUT>
     void PowerGaborRGLineSegmentTransform2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>*)
     {
         if ( th2 > th1 )
         {
             output= 0;
             return;
         }

         mgf->updateStride(input.columns);

         Image<OUTPUT> filtered;
         filtered.resizeImage(input);
         mgf->apply(input, filtered, roi);

         Image<unsigned char> seed;
         seed.resizeImage(input);
         Image<unsigned char> rgimage;
         rgimage.resizeImage(input);

         for ( unsigned int i= 0; i < filtered.n; ++i )
             if ( filtered(i) > th1 && (roi == NULL || (*roi)(i)))
                 seed(i)= 255;
             else
                 seed(i)= 0;

         rg->doRG(filtered, rgimage, seed, roi);
	 
	 meanCorr1= meanCorr2= meanCorr3= 0;
	 int n1, n2, n3;
	 n1= n2= n3= 0;	 
	 for ( unsigned int i= 0; i < seed.n; ++i )
	 {
	   if ( roi == NULL || (*roi)(i) )
	   {
	    if ( seed(i) )
	    {
	      meanCorr1+= filtered(i);
	      n1++;
	    }
	    if ( !seed(i) && rgimage(i) )
	    {
	      meanCorr2+= filtered(i);
	      n2++;
	    }
	    if ( rgimage(i) )
	    {
	      meanCorr3+= filtered(i);
	      n3++;
	    }
	   }
	 }    
	 
	 if ( n1 > 0 )
	  meanCorr1/= n1;
	 else
	   meanCorr1= 0;
	 if ( n2 > 0 )
	  meanCorr2/= n2;
	 else
	   meanCorr2= 0;
	 if ( n3 > 0 )
	  meanCorr3/= n3;
	 else
	   meanCorr3= 0;
	 
	 //printf("%f %f %f\n", meanCorr1, meanCorr2, meanCorr3);
         /*for ( int i= 0; i < filtered.n; ++i )
             output(i)= rgimage(i);*/
         output= rgimage;
     }
     
     template<typename INPUT, typename OUTPUT>
     void PowerGaborRGLineSegmentTransform2<INPUT, OUTPUT>::applyGetFiltered(Image<INPUT>& input, Image<OUTPUT>& output, Image<OUTPUT>& filtered, Image<unsigned char>* roi, Image<unsigned char>*)
     {
         if ( th2 > th1 )
         {
             output= 0;
             return;
         }

         mgf->updateStride(input.columns);

         filtered.resizeImage(input);
         mgf->apply(input, filtered, roi);

         Image<unsigned char> seed;
         seed.resizeImage(input);
         Image<unsigned char> rgimage;
         rgimage.resizeImage(input);

         for ( unsigned int i= 0; i < filtered.n; ++i )
             if ( filtered(i) > th1 && (roi == NULL || (*roi)(i)))
                 seed(i)= 255;
             else
                 seed(i)= 0;

         rg->doRG(filtered, rgimage, seed, roi);

	 meanCorr1= meanCorr2= meanCorr3= 0;
	 int n1, n2, n3;
	 n1= n2= n3= 0;	 
	 for ( unsigned int i= 0; i < seed.n; ++i )
	 {
	   if ( roi == NULL || (*roi)(i) )
	   {
	    if ( seed(i) )
	    {
	      meanCorr1+= filtered(i);
	      n1++;
	    }
	    if ( !seed(i) && rgimage(i) )
	    {
	      meanCorr2+= filtered(i);
	      n2++;
	    }
	    if ( rgimage(i) )
	    {
	      meanCorr3+= filtered(i);
	      n3++;
	    }
	   }
	 }     
	 
	 if ( n1 > 0 )
	  meanCorr1/= n1;
	 else
	   meanCorr1= 0;
	 if ( n2 > 0 )
	  meanCorr2/= n2;
	 else
	   meanCorr2= 0;
	 if ( n3 > 0 )
	  meanCorr3/= n3;
	 else
	   meanCorr3= 0;
	 
	 //printf("%f %f %f\n", meanCorr1, meanCorr2, meanCorr3);
         /*for ( int i= 0; i < filtered.n; ++i )
             output(i)= rgimage(i);*/
         output= rgimage;
     }
     
     template<typename INPUT, typename OUTPUT>
     void PowerGaborRGLineSegmentTransform2<INPUT, OUTPUT>::apply2(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>*)
     {
         if ( th2 > th1 )
         {
             output= 0;
             return;
         }

         mgf->updateStride(input.columns);

         Image<OUTPUT> filtered;
         filtered.resizeImage(input);
         mgf->apply(input, filtered, roi);

         Image<unsigned char> seed;
         seed.resizeImage(input);
         Image<unsigned char> rgimage;
         rgimage.resizeImage(input);

         for ( unsigned int i= 0; i < filtered.n; ++i )
             if ( filtered(i) > th1 && (roi == NULL || (*roi)(i)))
                 seed(i)= 255;
             else
                 seed(i)= 0;

         rg->doRG(filtered, rgimage, seed, roi);

	 meanCorr1= meanCorr2= meanCorr3= 0;
	 int n1, n2, n3;
	 n1= n2= n3= 0;	 
	 for ( unsigned int i= 0; i < seed.n; ++i )
	 {
	   if ( roi == NULL || (*roi)(i) )
	   {
	    if ( seed(i) )
	    {
	      meanCorr1+= filtered(i);
	      n1++;
	    }
	    if ( !seed(i) && rgimage(i) )
	    {
	      meanCorr2+= filtered(i);
	      n2++;
	    }
	    if ( rgimage(i) )
	    {
	      meanCorr3+= filtered(i);
	      n3++;
	    }
	   }
	 }      
	 
	 if ( n1 > 0 )
	  meanCorr1/= n1;
	 else
	   meanCorr1= 0;
	 if ( n2 > 0 )
	  meanCorr2/= n2;
	 else
	   meanCorr2= 0;
	 if ( n3 > 0 )
	  meanCorr3/= n3;
	 else
	   meanCorr3= 0;
	 //printf("%f %f %f\n", meanCorr1, meanCorr2, meanCorr3);
         for ( unsigned int i= 0; i < filtered.n; ++i )
             output(i)= rgimage(i)*filtered(i);
         //output= rgimage;
     }

     template<typename INPUT, typename OUTPUT>
     Border2 PowerGaborRGLineSegmentTransform2<INPUT, OUTPUT>::getProposedBorder()
     {
         return mgf->getProposedBorder();
     }

     template<typename INPUT, typename OUTPUT>
     class PowerGaborSimpleRGLineSegmentTransform2: public Transform2<INPUT, OUTPUT>
     {
     public:
         using Transform2<INPUT, OUTPUT>::descriptor;
         using Transform2<INPUT, OUTPUT>::apply;

         /**
          * default constructor
          */
         PowerGaborSimpleRGLineSegmentTransform2(float th1, float th2, float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma, float powerFactor, float scale= 1);

         /**
          * copy constructor
          * @param a instance to copy
          */
         PowerGaborSimpleRGLineSegmentTransform2(const PowerGaborSimpleRGLineSegmentTransform2& a);

         /**
          * destructor
          */
         ~PowerGaborSimpleRGLineSegmentTransform2();

	 void regenerate();
	 
	 void regenerate2();
	 
         /**
          * estimates the orientation of the image content in the foreground (non-zero) region of the mask
          * @param input input image
          * @param output output image
          * @param mask the operation is performed in the foreground (non-zero) region of the mask
          */
         virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
	 
	 virtual void applyOnlyRG(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
	 
	 virtual void applyGetFiltered(Image<INPUT>& input, Image<OUTPUT>& output, Image<OUTPUT>& filtered, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
	 
	 virtual void apply2(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

         virtual Border2 getProposedBorder();

         MatchedCCorrelationPowerGaborFilterSimpleR2<INPUT, OUTPUT>* mgf;
         NeighborhoodRegionGrowing<OUTPUT>* rg;

         float th1;
         float th2;
         float sigma;
         float theta0;
         float step;
         float theta1;
         float lambda;
         float psi;
         float gamma;
         float powerFactor;
	 float scale;
	 
	 float meanCorr1;
	 float meanCorr2;
	 float meanCorr3;
     };

     template<typename INPUT, typename OUTPUT>
     PowerGaborSimpleRGLineSegmentTransform2<INPUT, OUTPUT>::PowerGaborSimpleRGLineSegmentTransform2(float th1_, float th2_, float sigma_, float theta0_, float step_, float theta1_, float lambda_, float psi_, float gamma_, float powerFactor_, float scale_)
         : Transform2<INPUT, OUTPUT>(), th1(th1_), th2(th2_), sigma(sigma_), theta0(theta0_), step(step_), theta1(theta1_), lambda(lambda_), psi(psi_), gamma(gamma_), powerFactor(powerFactor_), scale(scale_)
     {
         mgf= new MatchedCCorrelationPowerGaborFilterSimpleR2<INPUT, OUTPUT>(sigma, theta0, step, theta1, lambda, psi, gamma, powerFactor, scale);
         rg= new NeighborhoodRegionGrowing<OUTPUT>(REGION_GROWING_HARD_THRESHOLD, th2);

         std::stringstream ss;
         ss << "PowerGaborSimpleRGLineSegmentTransform2 " << th1 << " " << th2 << " " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma << " " << powerFactor;
         descriptor= ss.str();
     }

     template<typename INPUT, typename OUTPUT>
     void PowerGaborSimpleRGLineSegmentTransform2<INPUT, OUTPUT>::regenerate()
     {
       //tprintf("regenerate, scale: %d %f %d\n", mgf->size(), scale, mgf->operator[](0)->size());
       delete mgf;
       mgf= new MatchedCCorrelationPowerGaborFilterSimpleR2<INPUT, OUTPUT>(sigma, theta0, step, theta1, lambda, psi, gamma, powerFactor, scale);
       //tprintf("regenerate, size: %d\n", mgf->operator[](0)->size());
       delete rg;
       rg= new NeighborhoodRegionGrowing<OUTPUT>(REGION_GROWING_HARD_THRESHOLD, th2);
     }
     
     template<typename INPUT, typename OUTPUT>
     void PowerGaborSimpleRGLineSegmentTransform2<INPUT, OUTPUT>::regenerate2()
     {
       delete rg;
       rg= new NeighborhoodRegionGrowing<OUTPUT>(REGION_GROWING_HARD_THRESHOLD, th2);
     }
     
     template<typename INPUT, typename OUTPUT>
     PowerGaborSimpleRGLineSegmentTransform2<INPUT, OUTPUT>::PowerGaborSimpleRGLineSegmentTransform2(const PowerGaborSimpleRGLineSegmentTransform2& a)
     : Transform2<INPUT, OUTPUT>(a)
     {
     }

     template<typename INPUT, typename OUTPUT>
     PowerGaborSimpleRGLineSegmentTransform2<INPUT, OUTPUT>::~PowerGaborSimpleRGLineSegmentTransform2()
     {
     }

     template<typename INPUT, typename OUTPUT>
     void PowerGaborSimpleRGLineSegmentTransform2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>*)
     {
         if ( th2 > th1 )
         {
             output= 0;
             return;
         }

         mgf->updateStride(input.columns);

         Image<OUTPUT> filtered;
         filtered.resizeImage(input);
         mgf->apply(input, filtered, roi);

         Image<unsigned char> seed;
         seed.resizeImage(input);
         Image<unsigned char> rgimage;
         rgimage.resizeImage(input);

         for ( unsigned int i= 0; i < filtered.n; ++i )
             if ( filtered(i) > th1 && (roi == NULL || (*roi)(i)))
                 seed(i)= 255;
             else
                 seed(i)= 0;

         rg->doRG(filtered, rgimage, seed, roi);
	 
	 meanCorr1= meanCorr2= meanCorr3= 0;
	 int n1, n2, n3;
	 n1= n2= n3= 0;	 
	 for ( unsigned int i= 0; i < seed.n; ++i )
	 {
	   if ( roi == NULL || (*roi)(i) )
	   {
	    if ( seed(i) )
	    {
	      meanCorr1+= filtered(i);
	      n1++;
	    }
	    if ( !seed(i) && rgimage(i) )
	    {
	      meanCorr2+= filtered(i);
	      n2++;
	    }
	    if ( rgimage(i) )
	    {
	      meanCorr3+= filtered(i);
	      n3++;
	    }
	   }
	 }      
	 
	 if ( n1 > 0 )
	  meanCorr1/= n1;
	 else
	   meanCorr1= 0;
	 if ( n2 > 0 )
	  meanCorr2/= n2;
	 else
	   meanCorr2= 0;
	 if ( n3 > 0 )
	  meanCorr3/= n3;
	 else
	   meanCorr3= 0;
	 //printf("%f %f %f\n", meanCorr1, meanCorr2, meanCorr3);
	 //seed.normalize(0, 255);
	 //char tmp[100];
	 //sprintf(tmp, "%f", th1);
	 //writeImage((input.filename + "-seed" + string(tmp) + ".bmp").c_str(), seed);

         /*for ( int i= 0; i < filtered.n; ++i )
             output(i)= rgimage(i);*/
         output= rgimage;
     }
     
     template<typename INPUT, typename OUTPUT>
     void PowerGaborSimpleRGLineSegmentTransform2<INPUT, OUTPUT>::applyOnlyRG(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>*)
     {
       //printf("applyOnlyRG\n"); fflush(stdout);
         if ( th2 > th1 )
         {
             output= 0;
             return;
         }

         //mgf->updateStride(input.columns);

         Image<OUTPUT> filtered;
         filtered.resizeImage(input);
         //mgf->apply(input, filtered, roi);
	 filtered= input;

         Image<unsigned char> seed;
         seed.resizeImage(input);
         Image<unsigned char> rgimage;
         rgimage.resizeImage(input);

         for (unsigned int i= 0; i < filtered.n; ++i )
             if ( filtered(i) > th1 && (roi == NULL || (*roi)(i)))
                 seed(i)= 255;
             else
                 seed(i)= 0;

         rg->doRG(filtered, rgimage, seed, roi);
	 
	 meanCorr1= meanCorr2= meanCorr3= 0;
	 int n1, n2, n3;
	 n1= n2= n3= 0;	 
	 for ( unsigned int i= 0; i < seed.n; ++i )
	 {
	   if ( roi == NULL || (*roi)(i) )
	   {
	    if ( seed(i) )
	    {
	      meanCorr1+= filtered(i);
	      n1++;
	    }
	    if ( !seed(i) && rgimage(i) )
	    {
	      meanCorr2+= filtered(i);
	      n2++;
	    }
	    if ( rgimage(i) )
	    {
	      meanCorr3+= filtered(i);
	      n3++;
	    }
	   }
	 }      
	 
	 if ( n1 > 0 )
	  meanCorr1/= n1;
	 else
	   meanCorr1= 0;
	 if ( n2 > 0 )
	  meanCorr2/= n2;
	 else
	   meanCorr2= 0;
	 if ( n3 > 0 )
	  meanCorr3/= n3;
	 else
	   meanCorr3= 0;
	 //printf("%f %f %f\n", meanCorr1, meanCorr2, meanCorr3);fflush(stdout);
	 //seed.normalize(0, 255);
	 //char tmp[100];
	 //sprintf(tmp, "%f", th1);
	 //writeImage((input.filename + "-seed" + string(tmp) + ".bmp").c_str(), seed);

         /*for ( int i= 0; i < filtered.n; ++i )
             output(i)= rgimage(i);*/
         output= rgimage;
     }
     
     template<typename INPUT, typename OUTPUT>
     void PowerGaborSimpleRGLineSegmentTransform2<INPUT, OUTPUT>::applyGetFiltered(Image<INPUT>& input, Image<OUTPUT>& output, Image<OUTPUT>& filtered, Image<unsigned char>* roi, Image<unsigned char>*)
     {
         if ( th2 > th1 )
         {
             output= 0;
             return;
         }

         mgf->updateStride(input.columns);

         filtered.resizeImage(input);
         mgf->apply(input, filtered, roi);

         Image<unsigned char> seed;
         seed.resizeImage(input);
         Image<unsigned char> rgimage;
         rgimage.resizeImage(input);

         for ( unsigned int i= 0; i < filtered.n; ++i )
             if ( filtered(i) > th1 && (roi == NULL || (*roi)(i)))
                 seed(i)= 255;
             else
                 seed(i)= 0;

         rg->doRG(filtered, rgimage, seed, roi);
	 
	 meanCorr1= meanCorr2= meanCorr3= 0;
	 int n1, n2, n3;
	 n1= n2= n3= 0;	 
	 for ( unsigned int i= 0; i < seed.n; ++i )
	 {
	   if ( roi == NULL || (*roi)(i) )
	   {
	    if ( seed(i) )
	    {
	      meanCorr1+= filtered(i);
	      n1++;
	    }
	    if ( !seed(i) && rgimage(i) )
	    {
	      meanCorr2+= filtered(i);
	      n2++;
	    }
	    if ( rgimage(i) )
	    {
	      meanCorr3+= filtered(i);
	      n3++;
	    }
	   }
	 }      
	 
	 if ( n1 > 0 )
	  meanCorr1/= n1;
	 else
	   meanCorr1= 0;
	 if ( n2 > 0 )
	  meanCorr2/= n2;
	 else
	   meanCorr2= 0;
	 if ( n3 > 0 )
	  meanCorr3/= n3;
	 else
	   meanCorr3= 0;
	 //printf("%f %f %f\n", meanCorr1, meanCorr2, meanCorr3);
	 //seed.normalize(0, 255);
	 //char tmp[100];
	 //sprintf(tmp, "%f", th1);
	 //writeImage((input.filename + "-seed" + string(tmp) + ".bmp").c_str(), seed);

         /*for ( int i= 0; i < filtered.n; ++i )
             output(i)= rgimage(i);*/
         output= rgimage;
     }

     template<typename INPUT, typename OUTPUT>
     void PowerGaborSimpleRGLineSegmentTransform2<INPUT, OUTPUT>::apply2(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>*)
     {
         if ( th2 > th1 )
         {
             output= 0;
             return;
         }

         mgf->updateStride(input.columns);

         Image<OUTPUT> filtered;
         filtered.resizeImage(input);
	 filtered= 0;
         mgf->apply(input, filtered, roi);

         Image<unsigned char> seed;
         seed.resizeImage(input);
         Image<unsigned char> rgimage;
         rgimage.resizeImage(input);
	 rgimage= 0;

         for ( unsigned int i= 0; i < filtered.n; ++i )
             if ( filtered(i) > th1 && (roi == NULL || (*roi)(i)))
                 seed(i)= 255;
             else
                 seed(i)= 0;

         rg->doRG(filtered, rgimage, seed, roi);
	 
	 meanCorr1= meanCorr2= meanCorr3= 0;
	 int n1, n2, n3;
	 n1= n2= n3= 0;	 
	 for ( unsigned int i= 0; i < seed.n; ++i )
	 {
	   if ( roi == NULL || (*roi)(i) )
	   {
	    if ( seed(i) )
	    {
	      meanCorr1+= filtered(i);
	      n1++;
	    }
	    if ( !seed(i) && rgimage(i) )
	    {
	      meanCorr2+= filtered(i);
	      n2++;
	    }
	    if ( rgimage(i) )
	    {
	      meanCorr3+= filtered(i);
	      n3++;
	    }
	   }
	 }      
	 
	 if ( n1 > 0 )
	  meanCorr1/= n1;
	 else
	   meanCorr1= 0;
	 if ( n2 > 0 )
	  meanCorr2/= n2;
	 else
	   meanCorr2= 0;
	 if ( n3 > 0 )
	  meanCorr3/= n3;
	 else
	   meanCorr3= 0;
	 //printf("%f %f %f\n", meanCorr1, meanCorr2, meanCorr3);
	 //seed.normalize(0, 255);
	 //char tmp[100];
	 //sprintf(tmp, "%f", th1);
	 //writeImage((input.filename + "-seed" + string(tmp) + ".bmp").c_str(), seed);

         for ( unsigned int i= 0; i < filtered.n; ++i )
             output(i)= rgimage(i)*filtered(i);
         //output= rgimage;
     }
     
     template<typename INPUT, typename OUTPUT>
     Border2 PowerGaborSimpleRGLineSegmentTransform2<INPUT, OUTPUT>::getProposedBorder()
     {
         return mgf->getProposedBorder();
     }
}

#endif // GABORRGLINESEGMENTFEATURE2_H
