#include <openipDS/Transform2.h>

void openip::pfn_notify(const char *errinfo, const void *, size_t , void *)
{
       fprintf(stderr, "%s\n", errinfo); fflush(stdout);
}


#ifdef USE_OPENCL



#endif

/*std::string openip::type(const std::type_info& ti)
{
    if ( ti == typeid(float) )
        return string("float");
    if ( ti == typeid(double) )
        return string("double");
    if ( ti == typeid(unsigned char) )
        return string("unsigned char");
    else
        return string("NULL");
}*/

