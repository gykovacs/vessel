#ifndef COLORTRANSFORM2S_H
#define COLORTRANSFORM2S_H

#include <openipDS/Transform2.h>
#include <openipLL/colorSpaces.h>

namespace openip
{
     template<typename INPUT, typename OUTPUT>
     class ChannelTransform2: public Transform2<INPUT, OUTPUT>
     {
     public:
         using Transform2<INPUT, OUTPUT>::descriptor;
         using Transform2<INPUT, OUTPUT>::apply;

         /**
          * default constructor
          */
         ChannelTransform2(int channel);

         /**
          * copy constructor
          * @param a instance to copy
          */
         ChannelTransform2(const ChannelTransform2& a);

         /**
          * destructor
          */
         ~ChannelTransform2();

         /**
          * estimates the orientation of the image content in the foreground (non-zero) region of the mask
          * @param input input image
          * @param output output image
          * @param mask the operation is performed in the foreground (non-zero) region of the mask
          */
         virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

         int channel;
     };

     template<typename INPUT, typename OUTPUT>
     ChannelTransform2<INPUT, OUTPUT>::ChannelTransform2(int channel)
     : Transform2<INPUT, OUTPUT>()
     {
         std::stringstream ss;
         ss << "ChannelTransform2 " << channel;
         descriptor= ss.str();
         this->channel= channel;
     }

     template<typename INPUT, typename OUTPUT>
     ChannelTransform2<INPUT, OUTPUT>::ChannelTransform2(const ChannelTransform2& a)
     : Transform2<INPUT, OUTPUT>(a)
     {
         this->channel= a.channel;
     }

     template<typename INPUT, typename OUTPUT>
     ChannelTransform2<INPUT, OUTPUT>::~ChannelTransform2()
     {
     }

     template<typename INPUT, typename OUTPUT>
     void ChannelTransform2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* /*roi*/, Image<unsigned char>*)
     {
         readImage(input.filename.c_str(), output, input.getBorder2(), channel);
     }

     template<typename INPUT, typename OUTPUT>
     class HSVHTransform2: public Transform2<INPUT, OUTPUT>
     {
     public:
         using Transform2<INPUT, OUTPUT>::descriptor;
         using Transform2<INPUT, OUTPUT>::apply;

         /**
          * default constructor
          */
         HSVHTransform2();

         /**
          * copy constructor
          * @param a instance to copy
          */
         HSVHTransform2(const HSVHTransform2& a);

         /**
          * destructor
          */
         ~HSVHTransform2();

         /**
          * estimates the orientation of the image content in the foreground (non-zero) region of the mask
          * @param input input image
          * @param output output image
          * @param mask the operation is performed in the foreground (non-zero) region of the mask
          */
         virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
     };

     template<typename INPUT, typename OUTPUT>
     HSVHTransform2<INPUT, OUTPUT>::HSVHTransform2()
     : Transform2<INPUT, OUTPUT>()
     {
         std::stringstream ss;
         ss << "HSVHTransform2 ";
         descriptor= ss.str();
     }

     template<typename INPUT, typename OUTPUT>
     HSVHTransform2<INPUT, OUTPUT>::HSVHTransform2(const HSVHTransform2& a)
     : Transform2<INPUT, OUTPUT>(a)
     {
     }

     template<typename INPUT, typename OUTPUT>
     HSVHTransform2<INPUT, OUTPUT>::~HSVHTransform2()
     {
     }

     template<typename INPUT, typename OUTPUT>
     void HSVHTransform2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>*)
     {
         Image<unsigned char> r;
         Image<unsigned char> g;
         Image<unsigned char> b;

         short h, s, v;

         readImage(input.filename.c_str(), r, g, b, input.getBorder2());
         for ( unsigned int i= 0; i < output.n; ++i )
         {
             if ( !roi || (*roi)(i) )
             {
                rgb2hsv(r(i), g(i), b(i), h, s, v);
                output(i)= h;
             }
         }
     }

     template<typename INPUT, typename OUTPUT>
     class LUVLTransform2: public Transform2<INPUT, OUTPUT>
     {
     public:
         using Transform2<INPUT, OUTPUT>::descriptor;
         using Transform2<INPUT, OUTPUT>::apply;

         /**
          * default constructor
          */
         LUVLTransform2();

         /**
          * copy constructor
          * @param a instance to copy
          */
         LUVLTransform2(const LUVLTransform2& a);

         /**
          * destructor
          */
         ~LUVLTransform2();

         /**
          * estimates the orientation of the image content in the foreground (non-zero) region of the mask
          * @param input input image
          * @param output output image
          * @param mask the operation is performed in the foreground (non-zero) region of the mask
          */
         virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
     };

     template<typename INPUT, typename OUTPUT>
     LUVLTransform2<INPUT, OUTPUT>::LUVLTransform2()
     : Transform2<INPUT, OUTPUT>()
     {
         std::stringstream ss;
         ss << "LUVLTransform2 ";
         descriptor= ss.str();
     }

     template<typename INPUT, typename OUTPUT>
     LUVLTransform2<INPUT, OUTPUT>::LUVLTransform2(const LUVLTransform2& a)
     : Transform2<INPUT, OUTPUT>(a)
     {
     }

     template<typename INPUT, typename OUTPUT>
     LUVLTransform2<INPUT, OUTPUT>::~LUVLTransform2()
     {
     }

     template<typename INPUT, typename OUTPUT>
     void LUVLTransform2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>*)
     {
         Image<unsigned char> r;
         Image<unsigned char> g;
         Image<unsigned char> b;

         float l, u, v;

         readImage(input.filename.c_str(), r, g, b, input.getBorder2());
         for ( unsigned int i= 0; i < output.n; ++i )
         {
             if ( !roi || (*roi)(i) )
             {
                 rgb2luv(r(i), g(i), b(i), l, u, v);
                 output(i)= l;
             }
         }
     }

     template<typename INPUT, typename OUTPUT>
     class LUVUTransform2: public Transform2<INPUT, OUTPUT>
     {
     public:
         using Transform2<INPUT, OUTPUT>::descriptor;
         using Transform2<INPUT, OUTPUT>::apply;

         /**
          * default constructor
          */
         LUVUTransform2();

         /**
          * copy constructor
          * @param a instance to copy
          */
         LUVUTransform2(const LUVUTransform2& a);

         /**
          * destructor
          */
         ~LUVUTransform2();

         /**
          * estimates the orientation of the image content in the foreground (non-zero) region of the mask
          * @param input input image
          * @param output output image
          * @param mask the operation is performed in the foreground (non-zero) region of the mask
          */
         virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
     };

     template<typename INPUT, typename OUTPUT>
     LUVUTransform2<INPUT, OUTPUT>::LUVUTransform2()
     : Transform2<INPUT, OUTPUT>()
     {
         std::stringstream ss;
         ss << "LUVUTransform2 ";
         descriptor= ss.str();
     }

     template<typename INPUT, typename OUTPUT>
     LUVUTransform2<INPUT, OUTPUT>::LUVUTransform2(const LUVUTransform2& a)
     : Transform2<INPUT, OUTPUT>(a)
     {
     }

     template<typename INPUT, typename OUTPUT>
     LUVUTransform2<INPUT, OUTPUT>::~LUVUTransform2()
     {
     }

     template<typename INPUT, typename OUTPUT>
     void LUVUTransform2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>*)
     {
         Image<unsigned char> r;
         Image<unsigned char> g;
         Image<unsigned char> b;

         float l, u, v;

         readImage(input.filename.c_str(), r, g, b, input.getBorder2());
         for ( unsigned int i= 0; i < output.n; ++i )
         {
             if ( !roi || (*roi)(i) )
             {
                 rgb2luv(r(i), g(i), b(i), l, u, v);
                 output(i)= u;
             }
         }
     }

     template<typename INPUT, typename OUTPUT>
     class LUVVTransform2: public Transform2<INPUT, OUTPUT>
     {
     public:
         using Transform2<INPUT, OUTPUT>::descriptor;
         using Transform2<INPUT, OUTPUT>::apply;

         /**
          * default constructor
          */
         LUVVTransform2();

         /**
          * copy constructor
          * @param a instance to copy
          */
         LUVVTransform2(const LUVVTransform2& a);

         /**
          * destructor
          */
         ~LUVVTransform2();

         /**
          * estimates the orientation of the image content in the foreground (non-zero) region of the mask
          * @param input input image
          * @param output output image
          * @param mask the operation is performed in the foreground (non-zero) region of the mask
          */
         virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
     };

     template<typename INPUT, typename OUTPUT>
     LUVVTransform2<INPUT, OUTPUT>::LUVVTransform2()
     : Transform2<INPUT, OUTPUT>()
     {
         std::stringstream ss;
         ss << "LUVVTransform2 ";
         descriptor= ss.str();
     }

     template<typename INPUT, typename OUTPUT>
     LUVVTransform2<INPUT, OUTPUT>::LUVVTransform2(const LUVVTransform2& a)
     : Transform2<INPUT, OUTPUT>(a)
     {
     }

     template<typename INPUT, typename OUTPUT>
     LUVVTransform2<INPUT, OUTPUT>::~LUVVTransform2()
     {
     }

     template<typename INPUT, typename OUTPUT>
     void LUVVTransform2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>*)
     {
         Image<unsigned char> r;
         Image<unsigned char> g;
         Image<unsigned char> b;

         float l, u, v;

         readImage(input.filename.c_str(), r, g, b, input.getBorder2());
         for ( unsigned int i= 0; i < output.n; ++i )
         {
             if ( !roi || (*roi)(i) )
             {
                 rgb2luv(r(i), g(i), b(i), l, u, v);
                 output(i)= v;
             }
         }
     }
}

#endif // COLORTRANSFORM2S_H
