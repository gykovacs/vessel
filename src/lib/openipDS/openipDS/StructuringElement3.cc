#include <openipDS/StructuringElement3.h>
#include <limits.h>

namespace openip
{
    StructuringElement3::StructuringElement3(int stride, int sliceSize)
    : VoxelSet1()
    {
        this->stride= stride;
        this->slicePitch= sliceSize;
    }

    StructuringElement3::StructuringElement3(const StructuringElement3& s)
    : VoxelSet1(s)
    {
    }

    StructuringElement3::~StructuringElement3()
    {
    }
    
    VoxelSet1& StructuringElement3::operator=(const VoxelSet1& v)
    {
        this->VoxelSet1::operator=(v);
        return *this;
    }

    std::ostream& operator<<(std::ostream& o, const StructuringElement3& s)
    {
        o << "[StructuringElement3 - size: ";
        o << s.size();
        o << " elements: ";
        for ( StructuringElement3::const_iterator it = s.begin(); it != s.end(); ++it )
        {
            o << *it;
            o << " ";
        }
        o << "]";

        return o;
    }
}

