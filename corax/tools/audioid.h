#ifndef AUDIOID_H
#define AUDIOID_H

/**
 * @todo write docs
 */

#include <wType/string.h>

class AudioId
{
public:
    AudioId(const W::String& p_artist, const W::String& p_album, const W::String& p_name);
    AudioId(const AudioId& other);
    
    bool operator==(const AudioId& other) const;
    bool operator!=(const AudioId& other) const;
    bool operator<(const AudioId& other) const;
    bool operator>(const AudioId& other) const;
    bool operator<=(const AudioId& other) const;
    bool operator>=(const AudioId& other) const;
    
    const W::String artist;
    const W::String album;
    const W::String name;
};

#endif // AUDIOID_H
