#include "file.h"
#include <iostream>

QMimeDatabase M::File::mimeDB;

M::File::File(W::Blob* p_file, const W::Address& addr, QObject* parent):
    M::Model(addr, parent),
    additional(),
    file(p_file)
{
    W::Handler* get = W::Handler::create(address + W::Address({u"get"}), this, &M::File::_h_get);
    W::Handler* getAdditional = W::Handler::create(address + W::Address({u"getAdditional"}), this, &M::File::_h_getAdditional);
    
    addHandler(get);
    addHandler(getAdditional);
}

M::File::~File()
{
    delete file;
}

M::Model::ModelType M::File::getType() const
{
    return type;
}

void M::File::initAdditional(const W::String& p_mime)
{
    additional.clear();
    
    additional.insert(u"size", new W::Uint64(file->size()));
    additional.insert(u"mimeType", p_mime);
}

void M::File::set(const W::Object& value)
{
    set(value.copy());
}

void M::File::set(W::Object* value)
{
    delete file;
    file = static_cast<W::Blob*>(value);
    
    QMimeType mt = mimeDB.mimeTypeForData(file->byteArray());
    initAdditional(W::String(mt.name().toStdString()));
    
    W::Vocabulary* vc = static_cast<W::Vocabulary*>(additional.copy());
    
    broadcast(vc, W::Address({u"getAdditional"}));
}

void M::File::h_getAdditional(const W::Event& ev)
{
    W::Vocabulary* vc = static_cast<W::Vocabulary*>(additional.copy());
    
    response(vc, W::Address({u"getAdditional"}), ev);
}

void M::File::h_subscribe(const W::Event& ev)
{
    M::Model::h_subscribe(ev);
    
    h_getAdditional(ev);
}

void M::File::h_get(const W::Event& ev)
{
    W::Vocabulary* vc = new W::Vocabulary();
    vc->insert(u"additional", additional.copy());
    vc->insert(u"data", file->copy());
    
    response(vc, W::Address({u"get"}), ev);
}

M::File * M::File::create(W::Blob* blob, const W::Address& addr, QObject* parent)
{
    M::File* out;
    
    QMimeType mt = mimeDB.mimeTypeForData(blob->byteArray());
    out = new File(blob, addr, parent);
    
    out->initAdditional(W::String(mt.name().toStdString()));
    return out;
}
