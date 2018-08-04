#ifndef PROXYSONG_H
#define PROXYSONG_H

/**
 * @todo write docs
 */
#include <wType/uint64.h>
#include <wType/address.h>

#include <wModel/vocabulary.h>

#include <wController/vocabulary.h>

class ProxySong : public M::Vocabulary {
    Q_OBJECT
public:
    ProxySong(const W::Uint64& p_id, const W::Address& p_address, QObject* parent = 0);
    ~ProxySong();
    
    const W::Uint64& getFileId() const;
    bool isReady() const;
    
signals:
    void ready();
    void notReady();
    
private:
    C::Vocabulary* songCtrl;
    C::Vocabulary* albumCtrl;
    C::Vocabulary* artistCtrl;
    
    W::Uint64 fileId;
    bool _ready;
    
private slots:
    void onSongNewElement(const W::String& key, const W::Object& element);
    void onSongRemoveElement(const W::String& key);
    
    void onAlbumNewElement(const W::String& key, const W::Object& element);
    void onAlbumRemoveElement(const W::String& key);
    
    void onArtistNewElement(const W::String& key, const W::Object& element);
    void onArtistRemoveElement(const W::String& key);
    
};

#endif // PROXYSONG_H
