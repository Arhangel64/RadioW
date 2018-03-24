#ifndef FILE_H
#define FILE_H

/**
 * @todo write docs
 */

#include <wModel/model.h>

#include <wType/blob.h>
#include <QtCore/QMimeDatabase>

namespace M {
    
    class File: public Model {
    protected:
        File(W::Blob* p_file, const W::Address& addr, QObject* parent = 0);
    public:
        ~File();
        
        void set(const W::Object & value) override;
        void set(W::Object * value) override;
        
        M::Model::ModelType getType() const override;
        static const M::Model::ModelType type = file;
        
        static File* create(W::Blob* blob, const W::Address& addr, QObject* parent = 0);
        
    protected:
        virtual void initAdditional(const W::String& p_mime);
        void h_subscribe(const W::Event & ev) override;
        
        handler(get);
        handler(getAdditional);
        
    protected:
        W::Vocabulary additional;
        W::Blob* file;
        
        static QMimeDatabase mimeDB;
    };
}

#endif // FILE_H
