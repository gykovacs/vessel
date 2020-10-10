#ifndef CUT_H
#define CUT_H

#include <openipDS/Transform3.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class VolumeCut: public Transform3<INPUT, OUTPUT>
    {
    public:
        VolumeCut(int direction, int from, int to);

        VolumeCut(VolumeCut<INPUT, OUTPUT>& vc);

        ~VolumeCut();

        void apply(Volume<INPUT> &input, Volume<OUTPUT> &output, Volume<unsigned char> *roi= NULL, Volume<unsigned char> *support= NULL);

        int direction;
        int from;
        int to;
    };

    template<typename INPUT, typename OUTPUT>
    VolumeCut<INPUT, OUTPUT>::VolumeCut(int direction, int from, int to)
        : Transform3<INPUT, OUTPUT>()
    {
        this->direction= direction;
        this->from= from;
        this->to= to;
    }

    template<typename INPUT, typename OUTPUT>
    VolumeCut<INPUT, OUTPUT>::VolumeCut(VolumeCut<INPUT, OUTPUT> &vc)
        : Transform3<INPUT, OUTPUT>(vc)
    {
        this->direction= vc.direction;
        this->from= vc.from;
        this->to= vc.to;
    }

    template<typename INPUT, typename OUTPUT>
    VolumeCut<INPUT, OUTPUT>::~VolumeCut()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void VolumeCut<INPUT, OUTPUT>::apply(Volume<INPUT> &input, Volume<OUTPUT> &output, Volume<unsigned char> */*roi*/, Volume<unsigned char> */*support*/)
    {
        if ( from == -1 )
            from= 0;

        if ( direction == 0 )
        {
            if ( to == -1 )
                to= input.slices;
            printf("%d %d %d\n", direction, from, to);
            if ( input.rows == output.rows && input.columns == output.columns && int(output.slices) == to - from )
            {
                for ( int i= from; i < to; ++i )
                    for ( unsigned int j= 0; j < output.rows; ++j )
                        for ( unsigned int k= 0; k < output.columns; ++k )
                        {
                            output(i - from, j, k)= input(i, j, k);
                        }
            }
            else
            {
                Border3 b= input.getBorder();
                printf("%d %d %d %d %d %d %d\n", b.frontBorder, b.backBorder, b.topBorder, b.bottomBorder, b.leftBorder, b.rightBorder, b.borderMode);
                output.resizeVolume(to - from, input.rows, input.columns, input.getBorder());
                printf("%d %d %d %d %d %d %d %d %d %d\n", output.slices, output.rows, output.columns, output.frontBorder, output.backBorder, output.topBorder, output.bottomBorder, output.leftBorder, output.rightBorder, output.borderMode);
                this->apply(input, output);
                return;
            }
        }
        if ( direction == 1 )
        {
            if ( to == -1 )
                to= input.rows;
            printf("%d %d %d\n", direction, from, to);
            if ( input.slices == output.slices && input.columns == output.columns && int(output.rows) == to - from )
            {
                for ( int i= from; i < to; ++i )
                    for ( unsigned int j= 0; j < output.slices; ++j )
                        for ( unsigned int k= 0; k < output.columns; ++k )
                        {
                            output(j, i - from, k)= input(j, i, k);
                        }
            }
            else
            {
                Border3 b= input.getBorder();
                printf("%d %d %d %d %d %d %d\n", b.frontBorder, b.backBorder, b.topBorder, b.bottomBorder, b.leftBorder, b.rightBorder, b.borderMode);
                output.resizeVolume(input.slices, to - from, input.columns, input.getBorder());
                printf("%d %d %d %d %d %d %d %d %d %d\n", output.slices, output.rows, output.columns, output.frontBorder, output.backBorder, output.topBorder, output.bottomBorder, output.leftBorder, output.rightBorder, output.borderMode);
                this->apply(input, output);
                return;
            }
        }
        if ( direction == 2 )
        {
            if ( to == -1 )
                to= input.columns;
            if ( input.rows == output.rows && input.slices == output.slices && int(output.columns) == to - from )
            {
                for ( int i= from; i < to; ++i )
                    for ( unsigned int j= 0; j < output.rows; ++j )
                        for ( unsigned int k= 0; k < output.slices; ++k )
                        {
                            output(k, j, i - from)= input(k, j, i);
                        }
            }
            else
            {
                output.resizeVolume(input.slices, input.rows, to - from, input.getBorder());
                this->apply(input, output);
                return;
            }
        }
    }
}

#endif // CUT_H
