#include "player.h"

M::Player::Player(const W::Address& address, QObject* parent):
    M::Model(address, parent),
    controls(),
    views(),
    playPauseBtn(new M::Button(address + W::Address{u"play"})),
    _queueView(new M::List(address + W::Address{u"queueView"})),
    _queue(),
    current(0)
{
    W::Handler* get = W::Handler::create(address + W::Address({u"get"}), this, &M::Player::_h_get);
    W::Handler* hqueue = W::Handler::create(address + W::Address({u"queue"}), this, &M::Player::_h_queue);
    addHandler(get);
    addHandler(hqueue);
    
    playPauseBtn->setLabel(W::String(u"play"));
    playPauseBtn->setEnabled(false);
    connect(playPauseBtn, SIGNAL(activated()), this, SLOT(onPlayPauseBtn()));
    
    addModel(playPauseBtn);
    addModel(_queueView);
    
    controls.insert(std::make_pair(playPause, playPauseBtn->getAddress()));
    views.insert(std::make_pair(queue, _queueView->getAddress()));
}

M::Player::~Player()
{

}

void M::Player::set(const W::Object& value)
{
    throw 14;   //what do you expect here? not implemented, and not sure it ever would be
}

void M::Player::set(W::Object* value)
{
    set(*value);
}

M::Model::ModelType M::Player::getType() const
{
    return M::Model::player;
}

void M::Player::h_subscribe(const W::Event& ev)
{
    M::Model::h_subscribe(ev);
    
    h_get(ev);
}


void M::Player::h_get(const W::Event& ev)
{
    W::Vector* ctrls = new W::Vector();
    ItemMap::const_iterator citr = controls.begin();
    ItemMap::const_iterator cend = controls.end();
    
    for (; citr != cend; ++citr) {
        W::Vocabulary* cvc = new W::Vocabulary();
        cvc->insert(u"type", new W::Uint64(cend->first));
        cvc->insert(u"address", cend->second);
        
        ctrls->push(cvc);
    }
    
    W::Vector* vws = new W::Vector();
    ItemMap::const_iterator vitr = views.begin();
    ItemMap::const_iterator vend = views.end();
    
    for (; vitr != vend; ++vitr) {
        W::Vocabulary* vvc = new W::Vocabulary();
        vvc->insert(u"type", new W::Uint64(vend->first));
        vvc->insert(u"address", vend->second);
        
        vws->push(vvc);
    }
    
    W::Vocabulary* res = new W::Vocabulary();
    
    res->insert(u"controls", ctrls);
    res->insert(u"views", vws);
    
    response(res, W::Address({u"get"}), ev);
}

void M::Player::onPlayPauseBtn()
{
    
}

void M::Player::h_queue(const W::Event& ev)
{
    const W::Vocabulary& data = static_cast<const W::Vocabulary&>(ev.getData());
    
    const W::Uint64& id = static_cast<const W::Uint64&>(data.at(u"id"));
    ProxySong* song = new ProxySong(id, address + W::Address{u"currentPlayback"});
    addModel(song);
    if (current == 0) {
        current = song;
        views.insert(std::make_pair(currentPlayback, song->getAddress()));
        W::Vocabulary* avc = new W::Vocabulary();
        avc->insert(u"type", new W::Uint64(currentPlayback));
        avc->insert(u"address", song->getAddress());
        
        W::Vector* add = new W::Vector();
        add->push(avc);
        
        W::Vocabulary* res = new W::Vocabulary();
        res->insert(u"add", add);
        res->insert(u"remove", new W::Vector());
        
        broadcast(res, W::Address{u"viewsChange"});
    } else {
        _queue.push_back(song);
        _queueView->push(song->getAddress());
    }
}
