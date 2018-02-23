#ifndef TOOLS_FILE_H
#define TOOLS_FILE_H

#include <wType/string.h> 

#include <list>
#include <dirent.h> 
#include <string.h>
#include <sys/stat.h>

namespace T {
    
class File
{
public:
    File(const W::String& p_path);
    ~File();
    
    const W::String& getPath() const;
    W::String suffix() const;
    W::String nameWithoutSuffix() const;
    W::String name() const;
    W::String parentDirectory() const;
    
    static bool readDirectoryRecursive(const W::String& path, std::list<File>* result);
    
private:
    W::String path;
};

}

#endif // TOOLS_FILE_H
