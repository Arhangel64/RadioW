#ifndef PLAYER_H
#define PLAYER_H

#include <map>
#include <deque>

#include <wModel/model.h>
#include <wModel/button.h>
#include <wModel/modelstring.h>
#include <wModel/list.h>

#include <wType/vocabulary.h>
#include <wType/vector.h>
#include <wType/address.h>

#include "proxysong.h"
/**
 * @todo write docs
 */
namespace M {
    class Player : public M::Model {
        Q_OBJECT
    public:
        Player(const W::Address& address, QObject* parent = 0);
        ~Player();
        
        void set(const W::Object & value) override;
        void set(W::Object * value) override;
        M::Model::ModelType getType() const override;
        
        enum ItemType {
            playPause,
            currentPlayback,
            queue
        };
        
    protected:
        void h_subscribe(const W::Event & ev) override;
        
        handler(get);
        handler(queue);
        
    private:
        typedef std::map<ItemType, const W::Address&> ItemMap;
        typedef std::deque<ProxySong*> Queue;
        
        ItemMap controls;
        ItemMap views;
        M::Button* playPauseBtn;
        M::List* _queueView;
        Queue _queue;
        ProxySong* current;
        
    private slots:
        void onPlayPauseBtn();
        
    };
}

#endif // PLAYER_H
