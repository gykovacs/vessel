#include <openipDS/Progress.h>

namespace openip
{
    Progress::Progress(float min_, float cur_, float step_, float max_, ProgressType type_)
    {
        this->min= min_;
        this->current= cur_;
        this->step= step_;
        this->max= max_;
        this->type= type_;
    }

    void Progress::increase()
    {
        this->current+= this->step;
    }

    void Progress::increase(float step_)
    {
        this->current+= step_;
    }

    void Progress::set(float cur_)
    {
        this->current= cur_;
    }

    void Progress::print()
    {
        float perc= this->current / (this->max - this->min) * 100;

        if ( this->type == PERCENTAGE )
            printf("\b\b\b\b\b\b\b\b\b%3.2f %c\n", perc, '%');
        else if ( this->type == PROGRESSBAR )
        {
            printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");

            int percI= (int)(perc/2);

            printf("||");

            for ( int i= 0; i < percI; ++i )
                printf("O");

            for ( int i= percI; i < 50; ++i )
                printf("_");

            printf("||");

            printf(" %3.2f %c", perc, '%');
        }

        fflush(stdout);
    }

    std::ostream& operator<<(std::ostream& o, const Progress& p)
    {
        o << "Progress [";
        o << p.current;
        o << "]";
        
        return o;
    }
}
