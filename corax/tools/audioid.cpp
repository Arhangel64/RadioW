#include "audioid.h"
AudioId::AudioId(const W::String& p_artist, const W::String& p_album, const W::String& p_name):
    artist(p_artist),
    album(p_album),
    name(p_name)
{
}

AudioId::AudioId(const AudioId& other):
    artist(other.artist),
    album(other.album),
    name(other.name)
{
}

bool AudioId::operator==(const AudioId& other) const
{
    return name == other.name && album == other.album && artist == other.artist;
}

bool AudioId::operator!=(const AudioId& other) const
{
    return operator==(other);
}

bool AudioId::operator>(const AudioId& other) const
{
    if (name == other.name) {
        if (album == other.album) {
            return name > other.name;
        } else {
            return album > other.album;
        }
    } else {
        return name > other.name;
    }
}

bool AudioId::operator<(const AudioId& other) const
{
    if (name == other.name) {
        if (album == other.album) {
            return name < other.name;
        } else {
            return album < other.album;
        }
    } else {
        return name < other.name;
    }
}


bool AudioId::operator>=(const AudioId& other) const
{
    return !operator<(other);
}

bool AudioId::operator<=(const AudioId& other) const
{
    return !operator>(other);
}
