#include <openipLL/ThumbnailCache.h>
#include <openipLL/imageIO.h>

namespace openip
{
    ThumbnailCache::ThumbnailCache(char* directory, int diameter)
    {
        Directory d= Directory(directory);
        this->diameter= diameter;
        md5_init(&md5Key);
        
        d.open();
        while ( d.hasNextFile() )
            files.push_back(d.nextFile());
        
        d.close();
        
        for ( unsigned int i= 0; i < files.size(); ++i )
            std::cout << files[i] << std::endl;
        
        dir= std::string(directory);
    }
        
    ThumbnailCache::ThumbnailCache(const ThumbnailCache& /*t*/)
    {
        
    }

    ThumbnailCache::~ThumbnailCache()
    {
        
    }

    void ThumbnailCache::put(std::string& filename, std::string& modification, std::string& output)
    {
        
        tmp[0]= 0;
        strcat(tmp, filename.c_str());
        strcat(tmp, modification.c_str());
        
        File f(filename);
        
        md5_buffer(tmp, strlen(tmp), sigtmp);
        md5_sig_to_string(sigtmp, signature, 33);
        //printf("%s %s %s\n", filename.c_str(), modification.c_str(), signature);fflush(stdout);
        
        sprintf(tmp, "%s.jpg", signature);
        
        tmp2[0]= 0;
        sprintf(tmp2, "%s/%s", dir.c_str(), tmp);
        
        output= std::string(tmp2);
        
        printf("%s\n", tmp);fflush(stdout);
        
        if ( std::find(files.begin(), files.end(), File(tmp)) == files.end() )
        {
            printf("not present in cache %s\n", filename.c_str());fflush(stdout);
            char fil[1000];
            strcpy(fil, filename.c_str());
            readImage(fil, r, g, b);
            printf("a"); fflush(stdout);
            int rows= r.rows;
            printf("b"); fflush(stdout);
            int columns= r.columns;
            printf("c"); fflush(stdout);
            int newRows= sqrt((float(diameter)*diameter*rows*rows)/(float(rows)*rows + columns*columns));
            printf("d"); fflush(stdout);
            int newColumns= sqrt(diameter*diameter - newRows*newRows);

            printf("%d %d %d %d\n", rows, columns, newRows, newColumns); fflush(stdout);
            
            binearestNeighborResize(r, newRows, newColumns);
            binearestNeighborResize(g, newRows, newColumns);
            binearestNeighborResize(b, newRows, newColumns);

            
            printf("%s\n", tmp2);
            writeImage(tmp2, r, g, b);
            
            files.push_back(File(tmp2));
        }
        else
        {
            printf("present in cache");fflush(stdout);
        }
    }


}
