#ifndef M_ATTRIBUTES_H
#define M_ATTRIBUTES_H

#include "vocabulary.h"

#include <wType/string.h>

#include <map>

namespace M {
    class Attributes : public M::Vocabulary
    {
    public:
        Attributes(const W::Address p_address, QObject* parent = 0);
        ~Attributes();
        
        void addAttribute(const W::String& key, M::Model* model);
        void removeAttribute(const W::String& key);
        void setAttribute(const W::String& key, const W::Object& value);
        void setAttribute(const W::String& key, W::Object* value);
        
        M::Model::ModelType getType() const override;
        static const M::Model::ModelType type = attributes;
        
    private:
        typedef std::map<W::String, M::Model*> Map;
        
        Map* attributes;
    };
}

#endif // ATTRIBUTES_H
