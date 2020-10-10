#include <openipML/DecisionTreeClassifier.h>

namespace openip
{
    SortingRecord::SortingRecord()
    {
        data= 0;
        classIndex= 0;
    }

    SortingRecord::SortingRecord(float d, int c)
    {
        data= d;
        classIndex= c;
    }

    bool sortByData(const SortingRecord& lhs, const SortingRecord& rhs)
    {
        return lhs.data < rhs.data;
    }
}
