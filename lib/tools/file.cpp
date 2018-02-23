#include "file.h"
#include <iostream>

T::File::File(const W::String& p_path):
    path(p_path)
{
}

T::File::~File()
{
}

const W::String & T::File::getPath() const
{
    return path;
}

W::String T::File::suffix() const
{
    uint64_t dotPos = path.findLastOf(W::String(u"."));
    if (dotPos > path.findLastOf(W::String(u"/"))) {
        return path.substr(dotPos + 1);
    } else {
        return W::String(u"");
    }
}

bool T::File::readDirectoryRecursive(const W::String& path, std::list<T::File>* result)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(path.toString().c_str());
    bool success = false;
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
                continue;
            }
            W::String d_path = path + W::String(u"/") + W::String(std::string(dir->d_name));
            
            struct stat st;
            int err = lstat(d_path.toString().c_str(), &st);
            if (err == 0) {
                success = true;
                switch (st.st_mode & S_IFMT) {
                    case S_IFDIR:
                        success = File::readDirectoryRecursive(d_path, result);
                        break;
                    case S_IFREG:
                        result->emplace_back(d_path);
                        break;
                }
            } else {
                std::cout << "unable read description of file " << d_path.toString() << ". ";
                switch (errno) {
                    case EACCES:
                        std::cout << "Search permission is denied for one of the directories in the path prefix of path";
                        break;
                    case EFAULT:
                        std::cout << "Bad address";
                        break;
                    case ELOOP:
                        std::cout << "Too many symbolic links encountered while traversing the path";
                        break;
                    case ENAMETOOLONG:
                        std::cout << "path is too long";
                        break;
                    case ENOENT:
                        std::cout << "A component of path does not exist, or path is an empty string";
                        break;
                    case ENOMEM:
                        std::cout << "Out of memory";
                        break;
                    case ENOTDIR:
                        std::cout << "A component of the path prefix of path is not a directory";
                        break;
                    case EOVERFLOW:
                        std::cout << "EOVERFLOW error";
                        break;
                    default:
                       std::cout << "undefined error"; 
                }
                std::cout << std::endl;
            }
        }

        closedir(d);
    } else {
        std::cout << "unable to open a directory " << path.toString() << std::endl;
    }
    
    return success;
}

W::String T::File::parentDirectory() const
{
    uint64_t lastSlashPos = path.findLastOf(W::String(u"/"));
    W::String fPath = path.substr(0, lastSlashPos);
    uint64_t pSpashPos = fPath.findLastOf(W::String(u"/"));
    return fPath.substr(pSpashPos + 1);
}

W::String T::File::name() const
{
    uint64_t slashPos = path.findLastOf(W::String(u"/"));
    return path.substr(slashPos + 1);
    
}

W::String T::File::nameWithoutSuffix() const
{
    W::String nws = name();
    uint64_t dotPos = path.findLastOf(W::String(u"."));
    return nws.substr(0, dotPos);
}
