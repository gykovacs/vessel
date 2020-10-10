#include <openipDS/Voxel.h>

namespace openip
{
    Voxel3::Voxel3()
    {
        s= r= c= 0;
    }

    Voxel3::Voxel3(const Voxel3& v)
    {
        this->s= v.s;
        this->r= v.r;
        this->c= v.c;
    }

    Voxel3::Voxel3(int s, int r, int c)
    {
        this->s= s;
        this->r= r;
        this->c= c;
    }

    Voxel3::~Voxel3()
    {
    }

    int Voxel3::getVoxel1(int rows, int columns)
    {
        return s*rows*columns + r*columns + c;
    }
}
