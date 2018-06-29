#ifndef BUTTON_H
#define BUTTON_H

#include <wModel/model.h>
#include <wModel/modelstring.h>

#include <wDispatcher/handler.h>

#include <wType/address.h>
#include <wType/event.h>
#include <wType/vocabulary.h>
#include <wType/boolean.h>

/**
 * @todo write docs
 */
namespace M {
    class Button : public Model {
        Q_OBJECT
    public:
        Button(const W::Address& address, QObject* parent = 0);
        ~Button();
        
        void setImage(const W::String& p_image);
        void setLabel(const W::String& p_label);
        void setEnabled(bool p_enabled);
        
        M::Model::ModelType getType() const override;
        void set(const W::Object & value) override;
        void set(W::Object * value) override;
        
    signals:
        void activated() const;
        
    protected:
        void h_subscribe(const W::Event & ev) override;
        
        handler(get);
        handler(activate);
        
    protected:
        bool enabled;
        bool hasImage;
        bool hasLabel;
        
        W::String* imageName;
        M::String* label;
    };
}


#endif // BUTTON_H
