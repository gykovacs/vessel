#ifndef THRESHOLD2_H
#define THRESHOLD2_H

namespace openip
{
   template<typename INPUT, typename OUTPUT>
   class Threshold2
   {
   public:
       Threshold2();

       Threshold2(const Threshold2& th);

       virtual ~Threshold2();

       virtual void threshold(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL, INPUT* th1= NULL, Image<INPUT>* th2= NULL);
   };

   template<typename INPUT, typename OUTPUT>
   Threshold2<INPUT, OUTPUT>::Threshold2()
   {
   }

   template<typename INPUT, typename OUTPUT>
   Threshold2<INPUT, OUTPUT>::Threshold2(const Threshold2& /*th*/)
   {
   }

   template<typename INPUT, typename OUTPUT>
   Threshold2<INPUT, OUTPUT>::~Threshold2()
   {
   }

   template<typename INPUT, typename OUTPUT>
   void Threshold2<INPUT, OUTPUT>::threshold(Image<INPUT>& /*input*/, Image<OUTPUT>& /*output*/, Image<unsigned char>* /*roi*/, Image<unsigned char>* /*support*/, INPUT* /*th1*/, Image<INPUT>* /*th2*/)
   {
   }
}

#endif // THRESHOLD2_H
