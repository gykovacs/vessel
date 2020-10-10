#include <openipDS/OpenIPObject.h>

namespace openip
{
    std::string openip::OpenIPObject::toString() const
    {
        return string("");
    }

    std::ostream& operator<<(std::ostream& o, const OpenIPObject& v)
    {
        return o << v.toString();
    }
}
