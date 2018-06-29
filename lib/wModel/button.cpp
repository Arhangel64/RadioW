#include "button.h"

M::Button::Button(const W::Address& address, QObject* parent):
    M::Model(address, parent),
    enabled(true),
    hasImage(false),
    hasLabel(false),
    imageName(0),
    label(0)
{
    W::Handler* get = W::Handler::create(address + W::Address({u"get"}), this, &M::Button::_h_get);
    W::Handler* activate = W::Handler::create(address + W::Address({u"activate"}), this, &M::Button::_h_activate);
    
    addHandler(get);
    addHandler(activate);
}

M::Button::~Button()
{
    if (hasImage) {
        delete imageName;
    }
}

void M::Button::setImage(const W::String& p_image)
{
    
    if (hasImage) {
        if (*imageName != p_image) {
            imageName = static_cast<W::String*>(p_image.copy());
            W::Vocabulary* vc = new W::Vocabulary();
            vc->insert(u"image", p_image);
            broadcast(vc, W::Address{u"changeImage"});
        }
    } else {
        imageName = static_cast<W::String*>(p_image.copy());
        hasImage = true;
        W::Vocabulary* vc = new W::Vocabulary();
        vc->insert(u"image", p_image);
        broadcast(vc, W::Address{u"setImage"});
    }
    hasImage = true;
}

void M::Button::setEnabled(bool p_enabled)
{
    if (enabled != p_enabled) {
        enabled = p_enabled;
        W::Vocabulary* vc = new W::Vocabulary();
        vc->insert(u"enable", new W::Boolean(enabled));
        broadcast(vc, W::Address{u"setEnabled"});
    }
}

void M::Button::setLabel(const W::String& p_label)
{
    if (hasLabel) {
        label->set(p_label);
    } else {
        label = new M::String(p_label, address + W::Address{u"label"});
        addModel(label);
        W::Vocabulary* vc = new W::Vocabulary();
        vc->insert(u"hasLabel", new W::Boolean(true));
        vc->insert(u"label", label->getAddress());
        broadcast(vc, W::Address{u"setLabel"});
        hasLabel = true;
    }
}

void M::Button::h_subscribe(const W::Event& ev)
{
    M::Model::h_subscribe(ev);
    
    h_get(ev);
}

void M::Button::h_get(const W::Event& ev)
{
    W::Vocabulary* vc = new W::Vocabulary();
    vc->insert(u"hasImage", new W::Boolean(hasImage));
    if (hasImage) {
        vc->insert(u"image", imageName->copy());
    }
    vc->insert(u"hasLabel", new W::Boolean(hasLabel));
    if (hasLabel) {
        vc->insert(u"label", label->getAddress());
    }
    vc->insert(u"enabled", new W::Boolean(enabled));
    
    response(vc, W::Address({u"get"}), ev);
}

void M::Button::h_activate(const W::Event& ev)
{
    if (enabled) {
        emit activated();
    }
}

M::Model::ModelType M::Button::getType() const
{
    return M::Model::button;
}

void M::Button::set(const W::Object& value)
{
    throw 14;   //what do you expect here? not implemented, and not sure it ever would be
}

void M::Button::set(W::Object* value)
{
    set(*value);
}
