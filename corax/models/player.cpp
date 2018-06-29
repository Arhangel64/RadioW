#include "player.h"

M::Player::Player(const W::Address& address, QObject* parent):
    M::Model(address, parent),
    content(),
    playPause(new M::Button(address + W::Address{u"play"}))
{
    addModel(playPause);
}

M::Player::~Player()
{
    ContentMap::const_iterator itr = content.begin();
    ContentMap::const_iterator end = content.end();
    
    for (; itr != end; ++itr) {
        delete itr->second;
    }
}
