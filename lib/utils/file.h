#ifndef W_FILE_H
#define W_FILE_H

#include <wType/string.h> 

#include <list>
#include <dirent.h> 
#include <string.h>
#include <sys/stat.h>

namespace W {

    class File
    {
    public:
        File(const W::String& p_path);
        ~File();
        
        const W::String& getPath() const;
        W::String suffix() const;
        
        static bool readDirectoryRecursive(const W::String& path, std::list<File>* result);
        
    private:
        W::String path;
    };

}

#endif // W_FILE_H
