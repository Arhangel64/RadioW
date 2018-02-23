#ifndef FILE_H
#define FILE_H

/**
 * @todo write docs
 */

#include <wModel/model.h>

#include <wType/blob.h>

namespace M {
    
    class File: public Model {
    public:
        File(W::Blob* p_file, const W::Address& addr, QObject* parent = 0);
        ~File();
        
        void set(const W::Object & value) override;
        void set(W::Object * value) override;
        
        M::Model::ModelType getType() const override;
        static const M::Model::ModelType type = file;
        
    protected:
        virtual void initAdditional();
        void h_subscribe(const W::Event & ev) override;
        
        handler(get);
        handler(getAdditional);
        
    protected:
        W::Vocabulary additional;
        W::Blob* file;
    };
}

#endif // FILE_H
