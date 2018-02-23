#include "parser.h"

Parser::Parser(const W::Socket* p_socket, W::Dispatcher* p_dp, ResourceCache* p_audio, ResourceCache* p_images):
    QObject(),
    socket(p_socket),
    dp(p_dp),
    songs(W::Address({u"songs"})),
    albums(W::Address({u"albums"})),
    artists(W::Address({u"artists"})),
    audio(p_audio),
    images(p_images),
    path(),
    songsReady(false),
    albumsReady(false),
    artistsReady(false),
    state(idle),
    foundImages(),
    foundAudios()
{
    connect(&songs, SIGNAL(ready()), this, SLOT(onSongsReady()));
    connect(&songs, SIGNAL(serviceMessage(const QString&)), this, SIGNAL(serviceMessage(const QString&)));
    connect(&albums, SIGNAL(ready()), this, SLOT(onAlbumsReady()));
    connect(&albums, SIGNAL(serviceMessage(const QString&)), this, SIGNAL(serviceMessage(const QString&)));
    connect(&artists, SIGNAL(ready()), this, SLOT(onArtistsReady()));
    connect(&artists, SIGNAL(serviceMessage(const QString&)), this, SIGNAL(serviceMessage(const QString&)));
    
    songs.registerController(dp, socket);
    albums.registerController(dp, socket);
    artists.registerController(dp, socket);
}

Parser::~Parser()
{
}


void Parser::onSongsReady()
{
    songsReady = true;
    emit serviceMessage("Songs are ready");
    checkState();
}

void Parser::onAlbumsReady()
{
    albumsReady = true;
    emit serviceMessage("Albums are ready");
    checkState();
}

void Parser::onArtistsReady()
{
    artistsReady = true;
    emit serviceMessage("Artists are ready");
    checkState();
}

void Parser::checkState()
{
    switch (state) {
        case idle:
            break;
        case waitingForCollections:
            if (songsReady && albumsReady && artistsReady) {
                state = parsingDirectory;
                parseDirectory();
            }
            break;
        case parsingDirectory:
            parseDirectory();
            break;
        case updatingMusicDataBase:
            if (songsReady && albumsReady && artistsReady) {
                updateMusicDataBase();
            }
            break;
        case updatingImageDataBase:
            if (songsReady && albumsReady && artistsReady) {
                updateImageDataBase();
            }
            break;
            
    }
    
    
}

void Parser::parse(const W::String& p_path)
{
    if (state != idle) {
        emit serviceMessage("An attempt to make parsing while another isn't finished, quitting");
        throw 15;
    }
    
    path = p_path;
    
    if (!songs.isSubscribed()) {
        songs.subscribe();
    }
    
    if (!albums.isSubscribed()) {
        albums.subscribe();
    }
    
    if (!artists.isSubscribed()) {
        artists.subscribe();
    }
    
    if (!songsReady || !albumsReady || !artistsReady) {
        state = waitingForCollections;
    } else {
        state = parsingDirectory;
    }
    
    checkState();
}

void Parser::parseDirectory()
{
        emit serviceMessage(QString("Starting to parse directory ") + path.toString().c_str());
        
        std::list<T::File> *list = new std::list<T::File>();
        bool success = T::File::readDirectoryRecursive(path, list);
        
            if (success) {
                emit serviceMessage("Successully recursively red the directory");
                std::set<uint64_t> presentMusicId = audio->getAllIdentificators();
                std::set<W::String> presentAudio;
                std::set<uint64_t>::const_iterator pai(presentMusicId.begin()), pae(presentMusicId.end());
                for (; pai != pae; ++pai) {
                    presentAudio.insert(audio->getPath(*pai));
                }
                
                std::set<uint64_t> presentImageId = images->getAllIdentificators();
                std::set<W::String> presentImages;
                std::set<uint64_t>::const_iterator pii(presentImageId.begin()), pie(presentImageId.end());
                for (; pii != pie; ++pii) {
                    presentImages.insert(images->getPath(*pii));
                }
                
                std::list<T::File>::const_iterator itr = list->begin();
                std::list<T::File>::const_iterator end = list->end();
                for (; itr != end; ++itr) {
                    W::String path = itr->getPath();
                    emit serviceMessage(QString("Analysing ") + path.toString().c_str());
                    
                    if (itr->suffix() == u"mp3") {
                        if (presentAudio.find(itr->getPath()) == presentAudio.end()) {
                            AudioTag tag(*itr);
                            uint64_t id = audio->addResource(itr->getPath());
                            AudioId aid(tag.getArtist(), tag.getAlbum(), tag.getTitle());
                            foundAudios.insert(std::make_pair(aid, id));
                        }
                        
                    } else if (itr->suffix() == u"jpg") {
                        if (presentImages.find(itr->getPath()) == presentImages.end()) {
                            uint64_t id = images->addResource(itr->getPath());
                            foundImages.insert(std::make_pair(itr->parentDirectory(), id));
                        }
                    }
                }
                
                emit serviceMessage(QString("Found ") + std::to_string(foundAudios.size()).c_str() + " audio files");
                emit serviceMessage(QString("Found ") + std::to_string(foundImages.size()).c_str() + " images");
                
                state = updatingMusicDataBase;
                
                
                
                updateMusicDataBase();
            } else {
                emit serviceMessage("Error parsing the directory");
            }
            
            delete list;
}

void Parser::updateMusicDataBase()
{
    while (foundAudios.size() > 0) {
        std::map<AudioId, uint64_t>::const_iterator itr = foundAudios.begin();
        
        std::set<uint64_t> aids = artists.find(W::String(u"name"), itr->first.artist);
        if (aids.size() == 0) {
            W::Vocabulary art;
            art.insert(u"name", itr->first.artist);
            artists.addRemoteElement(art);
            artistsReady = false;
            emit serviceMessage(QString("Creating artist: ") + itr->first.artist.toString().c_str());
            
            return;
        }
        uint64_t artistId = *(aids.begin());
        
        uint64_t thumbId = 0;       //TODO make some default picture for the case of not found images
        std::set<uint64_t> alids = albums.find(W::String(u"name"), itr->first.album);
        std::map<W::String, uint64_t>::const_iterator albImageItr = foundImages.find(itr->first.album);
        if (albImageItr != foundImages.end()) {
            thumbId = albImageItr->second;
        }
        uint64_t albumId = 0;
        bool albumFound = false;
        const C::Vocabulary* albCtrl = 0;
        while (alids.size() > 0 && !albumFound) {
            std::set<uint64_t>::const_iterator litr = alids.begin();
            albumId = *litr;
            alids.erase(litr);
            albCtrl = &albums.get(albumId);
            if (static_cast<const W::Uint64&>(albCtrl->at(u"artist")) == artistId) {
                albumFound = true;
            }
        }
        if (!albumFound) {
            W::Vocabulary alb;
            alb.insert(u"name", itr->first.album);
            alb.insert(u"artist", W::Uint64(artistId));
            if (thumbId != 0) {
                alb.insert(u"image", W::Uint64(thumbId));
                emit serviceMessage(QString("Found a cover for album: ") + itr->first.album.toString().c_str());
            }
            albums.addRemoteElement(alb);
            albumsReady = false;
            emit serviceMessage(QString("Creating album: ") + itr->first.album.toString().c_str());
            
            return;
        }
        if (thumbId != 0 && (!albCtrl->has(u"image") || static_cast<const W::Uint64&>(albCtrl->at(u"image")) != thumbId)) {
            W::Vocabulary alb;
            alb.insert(u"image", W::Uint64(thumbId));
            albums.updateRemoteElement(W::Uint64(albumId), alb);
            emit serviceMessage(QString("Found a cover for album: ") + itr->first.album.toString().c_str());
            foundImages.erase(albImageItr);
        }
        
        std::set<uint64_t> sids = songs.find(W::String(u"name"), itr->first.name);
        uint64_t songId = 0;
        bool songFound = false;
        const C::Vocabulary* songCtrl = 0;
        while (sids.size() > 0 && !songFound) {
            std::set<uint64_t>::const_iterator sitr = sids.begin();
            songId = *sitr;
            sids.erase(sitr);
            songCtrl = &songs.get(songId);
            if (static_cast<const W::Uint64&>(songCtrl->at(u"album")) == albumId && static_cast<const W::Uint64&>(songCtrl->at(u"artist")) == artistId) {
                songFound = true;
            }
        }
        
        W::Vocabulary sng;
        sng.insert(u"audio", W::Uint64(itr->second));
        
        if (!songFound) {
            sng.insert(u"name", itr->first.name);
            sng.insert(u"album", W::Uint64(albumId));
            sng.insert(u"artist", W::Uint64(artistId));
            songs.addRemoteElement(sng);
            songsReady = false;
            emit serviceMessage(QString("Creating a song: ") + itr->first.name.toString().c_str());
        } else if (!songCtrl->has(u"audio") || static_cast<const W::Uint64&>(songCtrl->at(u"audio")) != itr->second) {
            emit serviceMessage(QString("Found missing media for a song: ") + itr->first.name.toString().c_str());
            songs.updateRemoteElement(W::Uint64(songId), sng);
        }
        
        foundAudios.erase(itr);
        if (!songFound) {
            return;
        }
    }
    
    emit serviceMessage("Audio parsing is complete");
    
    state = updatingImageDataBase;
    emit serviceMessage("Parsing images");
    
    updateImageDataBase();
}

void Parser::updateImageDataBase()
{
    while (foundImages.size() > 0) {
        std::map<W::String, uint64_t>::const_iterator itr = foundImages.begin();
        
        std::set<uint64_t> alids = albums.find(W::String(u"name"), itr->first);
        if (alids.size() == 0) {
            emit serviceMessage(QString("Image in the folder ") + itr->first.toString().c_str() + " doesn't belong to any albumm, skipping");
        } else if (alids.size() > 1) {
            emit serviceMessage(QString("Image in the folder ") + itr->first.toString().c_str() + " belongs to " + std::to_string(alids.size()).c_str() + " albums, skipping");
        } else {
            uint64_t albumId = *alids.begin();
            const C::Vocabulary& ctrl = albums.get(albumId);
            
            if (!ctrl.has(u"image") || static_cast<const W::Uint64&>(ctrl.at(u"image")) != itr->second) {
                W::Vocabulary vc;
                vc.insert(u"image", W::Uint64(itr->second));
                emit serviceMessage(QString("Found missing cover for album: ") + itr->first.toString().c_str());
                
                albums.updateRemoteElement(W::Uint64(albumId), vc);
            }
        }
        
        foundImages.erase(itr);
    }
    
    emit serviceMessage("Parsing is complete");
    
    state = idle;
    emit done(path);
}

