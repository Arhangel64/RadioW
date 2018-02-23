#ifndef PARSER_H
#define PARSER_H

/**
 * @todo write docs
 */

#include <map>
#include <QtCore/QObject>

#include <wType/string.h>
#include <wType/address.h>
#include <wSocket/socket.h>
#include <wDispatcher/dispatcher.h>
#include <wController/collection.h>
#include <wDatabase/resourcecache.h>
#include <tools/file.h>

#include "audiotag.h"
#include "audioid.h"

class Parser: public QObject
{
    Q_OBJECT
public:
    Parser(const W::Socket* p_socket, W::Dispatcher* p_dp, ResourceCache* p_audio, ResourceCache* p_images);
    ~Parser();
    
    void parse(const W::String& p_path);
    
signals:
    void serviceMessage(const QString& msg);
    void done(const W::String& path);
    
private:
    enum State {
        idle,
        waitingForCollections,
        parsingDirectory,
        updatingMusicDataBase,
        updatingImageDataBase
    };
    
    const W::Socket* socket;
    W::Dispatcher* dp;
    C::Collection songs;
    C::Collection albums;
    C::Collection artists;
    ResourceCache* audio;
    ResourceCache* images;
    W::String path;
    
    bool songsReady;
    bool albumsReady;
    bool artistsReady;
    State state;
    std::map<W::String, uint64_t> foundImages;
    std::map<AudioId, uint64_t> foundAudios;
    
    void checkState();
    void parseDirectory();
    void updateMusicDataBase();
    void updateImageDataBase();
    
private slots:
    void onSongsReady();
    void onAlbumsReady();
    void onArtistsReady();
};

#endif // PARSER_H
