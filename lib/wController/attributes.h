#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include "vocabulary.h"

#include <map>

#include <wType/uint64.h>

namespace C {
    class Attributes : public C::Vocabulary
    {
        Q_OBJECT
    public:
        Attributes(const W::Address& p_address, QObject* parent = 0);
        ~Attributes();
        
        void unsubscribe();
        
    signals:
        void attributeChange(const W::String& atteName, const W::Object& value);
        
    protected:
        void _newElement(const W::String & key, const W::Object & element) override;
        void _removeElement(const W::String & key) override;
        void _clear() override;
        
    protected slots:
        void onAttrModification(const W::Object& data);
        void onSocketDisconnected() override;
        
    private:
        typedef std::map<W::String, C::Controller*> Map;
        typedef std::map<C::Controller*, W::String> RMap;
        
        static uint64_t counter;
    
        Map* attributes;
        RMap* reversed;
    };
}

#endif // ATTRIBUTES_H
