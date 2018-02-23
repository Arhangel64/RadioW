#include "audiotag.h"

AudioTag::AudioTag(const AudioTag& other):
    fileRef(other.fileRef)
{
}

AudioTag::AudioTag(const T::File& file):
    fileRef(file.getPath().toString().c_str())
{
}

AudioTag::~AudioTag()
{
}

W::String AudioTag::getTitle() const
{
    return W::String(fileRef.tag()->title().to8Bit(true));
}

W::String AudioTag::getAlbum() const
{
    return W::String(fileRef.tag()->album().to8Bit(true));
}

W::String AudioTag::getArtist() const
{
    return W::String(fileRef.tag()->artist().to8Bit(true));
}

W::Uint64 AudioTag::getYear() const
{
    return W::Uint64(fileRef.tag()->year());
}
