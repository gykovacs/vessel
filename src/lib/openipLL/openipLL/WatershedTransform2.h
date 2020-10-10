/* 
 * File:   WatershedTransform.h
 * Author: gykovacs
 *
 * Created on February 24, 2011, 1:27 PM
 */

#ifndef WATERSHEDTRANSFORM_H
#define	WATERSHEDTRANSFORM_H

#include <set>

#include <openipDS/Transform2.h>
#include <openipDS/PriorityQueue.h>
#include <openipDS/StructuringElement2s.h>
#include <openipDS/Stopper.h>
#include <openipLL/LowLevelTransforms.h>
#include <openipLL/imageIO.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class InterPixelWatershedTransform2: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;

        InterPixelWatershedTransform2();

        InterPixelWatershedTransform2(const InterPixelWatershedTransform2& w);

        ~InterPixelWatershedTransform2();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        void shrinkLabelRange(Image<int>& label, Image<unsigned char>& gray);

        void shrinkLabelRange(Image<int>& label, Image<unsigned char>& r, Image<unsigned char>& g, Image<unsigned char>& b);

        virtual Border2 getProposedBorder();
    };

    template<typename INPUT, typename OUTPUT>
    class WatershedTransform2: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;

        WatershedTransform2();

        WatershedTransform2(const WatershedTransform2& w);

        ~WatershedTransform2();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual Border2 getProposedBorder();
    };

    template<typename INPUT, typename OUTPUT>
    class WatershedTransform2Feature1: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;

        WatershedTransform2Feature1();

        WatershedTransform2Feature1(const WatershedTransform2Feature1& w);

        ~WatershedTransform2Feature1();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual Border2 getProposedBorder();
    };

    template<typename INPUT, typename OUTPUT>
    WatershedTransform2<INPUT, OUTPUT>::WatershedTransform2()
    : Transform2<INPUT, OUTPUT>()
    {
        descriptor= std::string("WatershedTransform2");
    }

    template<typename INPUT, typename OUTPUT>
    WatershedTransform2<INPUT, OUTPUT>::WatershedTransform2(const WatershedTransform2& w)
    : Transform2<INPUT, OUTPUT>(w)
    {
    }

    template<typename INPUT, typename OUTPUT>
    WatershedTransform2<INPUT, OUTPUT>::~WatershedTransform2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void WatershedTransform2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* , Image<unsigned char>*)
    {
        Image<int> tmp;
        Image<unsigned char> tmp2;
        tmp.resizeImage(input);
        tmp2.resizeImage(input);

        tmp= 0;
        tmp2= 0;

        StructuringElementSquare ses(3, input.columns);
        StructuringElementSquare::iterator it;
        StructuringElementDisk sd(1, input.columns);
        StructuringElementDisk::iterator sdit;

        int idx= 1;

        tprintf("finding minimas...\n");
        for ( unsigned int i= input.columns + 1; i < input.n - input.columns - 1; ++i )
        {
            for ( it= ses.begin(); it != ses.end(); ++it )
                if ( input(i + *it) < input(i) )
                {
                    tmp(i)= 0;
                    break;
                }
            if ( it == ses.end() )
                tmp(i)= idx++;
        }

        tprintf("create binary minima image...\n"); fflush(stdout);
        for ( unsigned int i= 0; i < tmp.size(); ++i )
            if ( tmp(i) > 0 )
                tmp2(i)= 255;

        tmp= 0;

        tprintf("component labeling...\n"); fflush(stdout);
        ComponentLabeling cl;
        cl.apply(tmp2, tmp);

        PriorityQueue<int, int> pq;
        for ( unsigned int i= tmp.columns + 1; i < tmp.n - tmp.columns - 1; ++i )
        {
            if ( tmp(i) == 0 )
                for ( sdit= sd.begin(); it != sd.end(); ++it )
                {
                    if ( (i + *sdit) > (input.columns + 1) && (i + *sdit) < (input.n - input.columns - 1) && tmp(i + *sdit) > 0 )
                    {
                        pq.push(-int(input(i)), i);
                        break;
                    }
                }
        }

        int pix;
        int label;

        tprintf("starting iteration...\n");
        while ( pq.size() > 0 )
        {
            pix= pq.top();
            pq.pop();
            label= -1;

            for ( sdit= sd.begin(); sdit != sd.end(); ++sdit )
            {
                if ( label == -1 && tmp(pix + *sdit) > 0 )
                    label= tmp(pix + *sdit);
                else if ( label != -1 && (tmp(pix + *sdit) != 0 && label != tmp(pix + *sdit)) )
                    break;
            }
            if ( sdit == sd.end() )
            {
                tmp(pix)= label;
                for ( sdit= sd.begin(); sdit != sd.end(); ++sdit )
                    if ( (pix + *sdit) > (input.columns + 1) && (pix + *sdit) < (input.n - input.columns - 1) && tmp(pix + *sdit) == 0 )
                        pq.push(-int(input(pix + *sdit)), pix + *sdit);
            }
        }

        tprintf("postprocessing...\n");
        output= 0;



        for ( unsigned int i= 0; i < output.size(); ++i )
            if ( tmp(i) == 0 )
            {
                for ( it= ses.begin(); it != ses.end(); ++it )
                {
                    if ( (i + *it) > (input.columns + 1) && (i + *it) < (input.n - input.columns - 1) && tmp(i + *it) != 0 )
                        output(i)= tmp(i + *it);
                }
            }
            else
                output(i)= tmp(i);

        for ( unsigned int i= 0; i < output.size(); ++i )
            if ( output(i) == 0 )
            {
                for ( it= ses.begin(); it != ses.end(); ++it )
                {
                    if ( (i + *it) > 0 && (i + *it) < input.n && output(i + *it) != 0 )
                        output(i)= output(i + *it);
                }
            }
    }

    template<typename INPUT, typename OUTPUT>
    Border2 WatershedTransform2<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(1,1,1,1);
    }

    template<typename INPUT, typename OUTPUT>
    WatershedTransform2Feature1<INPUT, OUTPUT>::WatershedTransform2Feature1()
    : Transform2<INPUT, OUTPUT>()
    {
        descriptor= std::string("WatershedTransform2Feature1");
    }

    template<typename INPUT, typename OUTPUT>
    WatershedTransform2Feature1<INPUT, OUTPUT>::WatershedTransform2Feature1(const WatershedTransform2Feature1& w)
    : Transform2<INPUT, OUTPUT>(w)
    {
    }

    template<typename INPUT, typename OUTPUT>
    WatershedTransform2Feature1<INPUT, OUTPUT>::~WatershedTransform2Feature1()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void WatershedTransform2Feature1<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>*, Image<unsigned char>*)
    {
        Image<int> tmp;
        tmp.resizeImage(input);
        tmp= 0;

        tprintf("applying inter pixel watershed transform...\n");
        InterPixelWatershedTransform2<INPUT, int> wt2;
        wt2.apply(input, tmp);

        int max;
        max= tmp.getMax();

        Vector<int> sizes(max+1);
        sizes= 0;
        int n= 0;
        tprintf("computing size of components...\n");
        for ( unsigned int i= 0; i < tmp.size(); ++i )
            sizes(tmp(i))= sizes(tmp(i)) + 1;

        tprintf("computing number of non-zero components...\n");
        for ( unsigned int i= 0; i < sizes.size(); ++i )
            if ( sizes(i) != 0 )
                ++n;

        tprintf("computing average component size...\n");
        float avgsize= 0;
        for ( unsigned int i= 0; i < sizes.size(); ++i )
            avgsize+= sizes(i);
        avgsize/= n;

        int mins, maxs;
        sizes.getMinMax(mins, maxs);

        tprintf("min and max component sizes: %d %d\n", mins, maxs);
        tprintf("number of components: %d average size: %f\n", n, avgsize);

        for ( unsigned int i= 0; i < output.size(); ++i )
            output(i)= avgsize/float(sizes(tmp(i)));
    }

    template<typename INPUT, typename OUTPUT>
    Border2 WatershedTransform2Feature1<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(1,1,1,1);
    }

    template<typename INPUT, typename OUTPUT>
    InterPixelWatershedTransform2<INPUT, OUTPUT>::InterPixelWatershedTransform2()
    : Transform2<INPUT, OUTPUT>()
    {
        descriptor= std::string("InterPixelWatershedTransform2");
    }

    template<typename INPUT, typename OUTPUT>
    InterPixelWatershedTransform2<INPUT, OUTPUT>::InterPixelWatershedTransform2(const InterPixelWatershedTransform2& w)
    : Transform2<INPUT, OUTPUT>(w)
    {
    }

    template<typename INPUT, typename OUTPUT>
    InterPixelWatershedTransform2<INPUT, OUTPUT>::~InterPixelWatershedTransform2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void InterPixelWatershedTransform2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>*, Image<unsigned char>*)
    {
        input.resizeBorder(1, 1, 1, 1, BORDER_MODE_255);

        Image<int> tmp;
        Image<unsigned char> tmp2;
        tmp.resizeImage(input);
        tmp2.resizeImage(input);

        tmp= 0;
        tmp2= 0;

        StructuringElementSquare ses(3, input.columns);
        StructuringElementSquare::iterator it;
        StructuringElementSquare sd(3, input.columns);
        StructuringElementSquare::iterator sdit;

        tprintf("finding minimas...\n");
        for ( unsigned int i= 0; i < input.n; ++i )
        {
            for ( it= ses.begin(); it != ses.end(); ++it )
                if ( (int(i) + *it) >= 0 && (int(i) + *it) < int(input.n) && input(int(i) + *it) < input(i) )
                {
                    tmp2(i)= 0;
                    break;
                }
            if ( it == ses.end() )
                tmp2(i)= 255;
        }

        //writeImage("minimas.png", tmp2);

        tprintf("component labeling...\n"); fflush(stdout);
        ComponentLabeling cl;
        cl.apply(tmp2, tmp);

        //writeImage("labeled.png", tmp);

        int max;
        max= tmp.getMax();
        Vector<unsigned char> labelsToRemove(max+1);
        labelsToRemove= 0;

        for ( unsigned int i= 0; i < tmp.n; ++i )
            for ( it= ses.begin(); it != ses.end(); ++it )
                if ( (int(i) + *it) >= 0 && (int(i) + *it) < int(tmp.n) && tmp(i) > 0 && tmp(int(i) + *it) == 0 && input(i) >= input(int(i) + *it) )
                    labelsToRemove(tmp(i))= 1;

        for ( unsigned int i= 0; i < input.n; ++i )
            if ( labelsToRemove(tmp(i)) == 1 )
            {
                tmp(i)= 0;
                //tmp2(i)= 0;
            }
            /*else
                if ( tmp(i) > 0 )
                    tmp2(i)= 255;*/

        //writeImage("minimas3.png", tmp);
        //writeImage("minimas2.png", tmp2);
        PriorityQueue<int, int> pq;
        for ( unsigned int i= 0; i < tmp.n; ++i )
        {
            if ( tmp(i) > 0 )
                pq.push(-int(input(i)), i);
        }

        int pix;

        tprintf("starting iteration...\n");
        while ( pq.size() > 0 )
        {
            pix= pq.top();
            pq.pop();

            for ( sdit= sd.begin(); sdit != sd.end(); ++sdit )
            {
                if ( (pix + *sdit) >= 0 && (pix + *sdit) < int(input.n) && tmp(pix + *sdit) == 0 )
                {
                    tmp(pix + *sdit)= tmp(pix);
                    pq.push(-int(input(pix + *sdit)), pix + *sdit);
                }
            }
        }
        /*Image<unsigned char> tmp3;
        tmp3.resizeImage(tmp2);
        for ( int i= 0; i < tmp.n; ++i )
            if ( tmp(i) > 255 )
            {
                tmp3(i)= tmp(i)%256;
                tmp2(i)= 0;
            }
            else
            {
                tmp2(i)= tmp(i);
                tmp3(i)= 0;
            }
        writeImage("test.png", tmp2, tmp3, tmp3);*/


        tmp.resizeBorder(0, 0, 0, 0);

        Image<unsigned char> labels;
        Image<unsigned char> r;
        Image<unsigned char> g;
        Image<unsigned char> b;
        labels.resizeImage(tmp);
        r.resizeImage(tmp);
        g.resizeImage(tmp);
        b.resizeImage(tmp);
        shrinkLabelRange(tmp, labels);
        writeImage("labelsShrunk.png", labels);
        shrinkLabelRange(tmp, r, g, b);
        writeImage("color.png", r, g, b);
        output= tmp;
    }

    template<typename INPUT, typename OUTPUT>
    Border2 InterPixelWatershedTransform2<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(1,1,1,1);
    }

    template<typename INPUT, typename OUTPUT>
    void InterPixelWatershedTransform2<INPUT, OUTPUT>::shrinkLabelRange(Image<int>& label, Image<unsigned char>& gray)
    {
        int max;
        max= label.getMax();
        Vector<unsigned char> l(max+1);
        Vector<unsigned char> map(max+1);

        l= 0;

        int n= 0;
        for ( unsigned int i= 0; i < label.size(); ++i )
            l(label(i))= 1;


        for ( unsigned int i= 0; i < l.size(); ++i )
            if ( l(i) == 1 )
                ++n;

        int idx= 0;
        for ( unsigned int i= 0; i < l.size(); ++i )
        {
            if ( l(i) == 1 )
            {
                ++idx;
            }

            map(i)= i;
            if ( l(i) == 0 )
            {
                idx= i;
                break;
            }
        }

        for ( unsigned int i= idx; i < map.size(); ++i )
            if ( l(i) == 1 )
                map(i)= idx++;

        for ( unsigned int i= 0; i < label.n; ++i )
            gray(i)= map(label(i));
    }

    template<typename INPUT, typename OUTPUT>
    void InterPixelWatershedTransform2<INPUT, OUTPUT>::shrinkLabelRange(Image<int>& label, Image<unsigned char>& r, Image<unsigned char>& g, Image<unsigned char>& b)
    {
        const int aa= 256;
        const int bb= 256*256;

        for ( unsigned int i= 0; i < label.size(); ++i )
        {
            if ( label(i) < aa )
            {
                r(i)= label(i);
                b(i)= g(i)= 0;
            }
            else if ( label(i) >= aa && label(i) < bb )
            {
                r(i)= label(i) / aa;
                g(i)= label(i) % aa;
                b(i)= 0;
            }
            else if ( label(i) >= bb )
            {
                r(i)= label(i) / bb;
                g(i)= (label(i) - label(i)/bb*bb) % aa;
                b(i)= label(i) % aa;
            }
        }
    }
}

#endif	/* WATERSHEDTRANSFORM_H */

