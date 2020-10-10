#include <openipDS/FileSystem.h>
#include <string>

namespace openip
{
    File::File()
    : std::string()
    {
    }

    File::File(const char* filename)
    : std::string(filename)
    {
    }

    File::File(std::string s)
    : std::string(s)
    {
    }

    File::File(const File& f)
    : std::string(f)
    {
    }

    File::~File()
    {
    }

    bool File::operator==(const File& f)
    {
        return !(this->compare(f));
    }
    
    Directory File::getParentDirectory()
    {
        int n= this->find_last_of("/\\");
        return Directory(this->substr(0, n));
    }

    std::string File::getAbsoluteFilename(std::string s)
    {
        return s + std::string("/") + *this;
    }

    File File::getFilename()
    {
        int n= this->find_last_of("/\\");
        return File(this->substr(n+1));
    }

    std::string File::getExtension()
    {
        int n= this->find_last_of(".");
        if ( n == -1 )
            return std::string("");
        else
            return this->substr(n+1);
    }

    Directory::Directory(const char* dirname)
    : std::string(dirname)
    {
    }

    Directory::Directory(std::string s)
    : std::string(s)
    {
    }

    Directory::Directory(const Directory& d)
    : std::string(d)
    {
    }

    Directory::~Directory()
    {
    }

    Directory Directory::getParentDirectory()
    {
        int n= this->find_last_of("/\\");
        return Directory(this->substr(0, n));
    }

    int Directory::open()
    {
        dir= opendir(this->c_str());

        if ( dir == NULL )
            return 1;
        return 0;
    }

    int Directory::close()
    {
        return closedir(dir);
    }

    int Directory::hasNextFile()
    {
        dirp = readdir(dir);
        if ( dirp == NULL )
            return 0;
        else
            return 1;
    }

    File Directory::nextFile()
    {
        return File(dirp->d_name);
    }
}
