#ifndef PLAYER_H
#define PLAYER_H

#include <map>

#include <wModel/model.h>
#include <wModel/button.h>

#include <wType/vocabulary.h>
#include <wType/address.h>

/**
 * @todo write docs
 */
namespace M {
    class Player : public M::Model {
    public:
        Player(const W::Address& address, QObject* parent = 0);
        ~Player();
        
    private:
        typedef std::map<W::Address, W::Vocabulary*> ContentMap;
        
        ContentMap content;
        M::Button* playPause;
    };
}

#endif // PLAYER_H
