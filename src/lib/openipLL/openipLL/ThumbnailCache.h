/* 
 * File:   ThumbnailCache.h
 * Author: gykovacs
 *
 * Created on August 9, 2011, 8:48 PM
 */

#ifndef THUMBNAILCACHE_H
#define	THUMBNAILCACHE_H

#include <openipLL/libmd5/md5.h>
#include <openipLL/Scaling.h>

#include <openipDS/FileSystem.h>
#include <openipDS/Image.h>

#include <openipLL/libmd5/md5.h>

#include <string>
#include <vector>

namespace openip
{
    class ThumbnailCache
    {
    public:
        ThumbnailCache(char* directory, int diameter= 100);
        
        ThumbnailCache(const ThumbnailCache& t);
        
        ~ThumbnailCache();
        
        void put(std::string& s, std::string& modification, std::string& filename);
        
        md5_t md5Key;
        char signature[33];
        char sigtmp[16];
        char tmp[1000];
        char tmp2[1000];
        
        Image<unsigned char> r;
        Image<unsigned char> g;
        Image<unsigned char> b;

        std::vector<File> files;
        
        int diameter;
        
        std::string dir;
    };
}

#endif	/* THUMBNAILCACHE_H */

