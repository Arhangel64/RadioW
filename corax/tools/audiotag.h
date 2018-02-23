#ifndef AUDIOTAG_H
#define AUDIOTAG_H

#include <wType/string.h>
#include <wType/uint64.h>

#include <tools/file.h>

#include <taglib/fileref.h>
#include <taglib/tag.h>

class AudioTag
{
public:
    AudioTag(const T::File& file);
    AudioTag(const AudioTag& other);
    ~AudioTag();
    
    W::String getTitle() const;
    W::String getAlbum() const;
    W::String getArtist() const;
    W::Uint64 getYear() const;
    
    
private:
    TagLib::FileRef fileRef;
};

#endif // AUDIOTAG_H
