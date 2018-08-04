#include "proxysong.h"

ProxySong::ProxySong(const W::Uint64& p_id, const W::Address& p_address, QObject* parent):
    M::Vocabulary(p_address, parent),
    songCtrl(new C::Vocabulary(W::Address{u"songs", W::String(p_id.toString())})),
    albumCtrl(0),
    artistCtrl(0),
    fileId(0),
    _ready(false)
{
    addController(songCtrl, W::String(u"Perturabo"));
    connect(songCtrl, SIGNAL(newElement(const W::String&, const W::Object&)), SLOT(onSongNewElement(const W::String&, const W::Object&)));
    connect(songCtrl, SIGNAL(removeElement(const W::String&)), SLOT(onSongRemoveElement(const W::String&)));
    
    insert(W::String(u"id"), p_id);
    insert(W::String(u"artist"), new W::String(u"undefined"));
    insert(W::String(u"album"), new W::String(u"undefined"));
    insert(W::String(u"name"), new W::String(u"undefined"));
    insert(W::String(u"image"), new W::Uint64(0));
}

bool ProxySong::isReady() const
{
    return _ready;
}

ProxySong::~ProxySong()
{
}

void ProxySong::onSongNewElement(const W::String& key, const W::Object& element)
{
    if (key == u"name") {
        insert(key, element);
    } else if (key == u"audio") {
        if (_ready) {
            _ready = false;
            emit notReady();
        }
        
        fileId = static_cast<const W::Uint64&>(element);
        _ready = true;
        emit ready();
    } else if (key == u"artist") {
        if (artistCtrl != 0) {
            removeController(artistCtrl);
            disconnect(artistCtrl, SIGNAL(newElement(const W::String&, const W::Object&)), this, SLOT(onArtistNewElement(const W::String&, const W::Object&)));
            disconnect(artistCtrl, SIGNAL(removeElement(const W::String&)), this, SLOT(onAtristRemoveElement(const W::String&)));
            artistCtrl->deleteLater();
        }
        const W::Uint64& aid = static_cast<const W::Uint64&>(element);
        artistCtrl = new C::Vocabulary(W::Address{u"artists", W::String(aid.toString())});
        addController(artistCtrl, W::String(u"Perturabo"));
        connect(artistCtrl, SIGNAL(newElement(const W::String&, const W::Object&)), SLOT(onArtistNewElement(const W::String&, const W::Object&)));
        connect(artistCtrl, SIGNAL(removeElement(const W::String&)), SLOT(onAtristRemoveElement(const W::String&)));
    } else if (key == u"album") {
        if (albumCtrl != 0) {
            removeController(albumCtrl);
            disconnect(albumCtrl, SIGNAL(newElement(const W::String&, const W::Object&)), this, SLOT(onAlbumNewElement(const W::String&, const W::Object&)));
            disconnect(albumCtrl, SIGNAL(removeElement(const W::String&)), this, SLOT(onAlbumRemoveElement(const W::String&)));
            albumCtrl->deleteLater();
        }
        const W::Uint64& aid = static_cast<const W::Uint64&>(element);
        albumCtrl = new C::Vocabulary(W::Address{u"albums", W::String(aid.toString())});
        addController(albumCtrl, W::String(u"Perturabo"));
        connect(albumCtrl, SIGNAL(newElement(const W::String&, const W::Object&)), SLOT(onAlbumNewElement(const W::String&, const W::Object&)));
        connect(albumCtrl, SIGNAL(removeElement(const W::String&)), SLOT(onAlbumRemoveElement(const W::String&)));
    }
}

void ProxySong::onSongRemoveElement(const W::String& key)
{
    if (key == u"name") {
        insert(key, new W::String(u"undefined"));
    } else if (key == u"audio") {
        if (_ready) {
            _ready = false;
            fileId = W::Uint64(0);
            emit notReady();
        }
    } else if (key == u"artist") {
        if (artistCtrl != 0) {
            removeController(artistCtrl);
            disconnect(artistCtrl, SIGNAL(newElement(const W::String&, const W::Object&)), this, SLOT(onArtistNewElement(const W::String&, const W::Object&)));
            disconnect(artistCtrl, SIGNAL(removeElement(const W::String&)), this, SLOT(onAtristRemoveElement(const W::String&)));
            artistCtrl->deleteLater();
            artistCtrl = 0;
        }
    } else if (key == u"album") {
        if (albumCtrl != 0) {
            removeController(albumCtrl);
            disconnect(albumCtrl, SIGNAL(newElement(const W::String&, const W::Object&)), this, SLOT(onAlbumNewElement(const W::String&, const W::Object&)));
            disconnect(albumCtrl, SIGNAL(removeElement(const W::String&)), this, SLOT(onAlbumRemoveElement(const W::String&)));
            albumCtrl->deleteLater();
            albumCtrl = 0;
        }
    }
}

void ProxySong::onAlbumNewElement(const W::String& key, const W::Object& element)
{
    if (key == u"name") {
        insert(W::String(u"album"), element);
    } else if (key == u"image") {
        insert(key, element);
    }
}

void ProxySong::onAlbumRemoveElement(const W::String& key)
{
    if (key == u"name") {
        insert(W::String(u"album"), new W::String(u"undefined"));
    } else if (key == u"image") {
        insert(key, new W::Uint64(0));
    }
}

void ProxySong::onArtistNewElement(const W::String& key, const W::Object& element)
{
    if (key == u"name") {
        insert(W::String(u"artist"), element);
    }
}

void ProxySong::onArtistRemoveElement(const W::String& key)
{
    if (key == u"name") {
        insert(W::String(u"artist"), new W::String(u"undefined"));
    }
}

const W::Uint64 & ProxySong::getFileId() const
{
    return fileId;
}
