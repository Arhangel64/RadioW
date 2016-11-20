#ifndef CONTROLLER_STRING_H
#define CONTROLLER_STRING_H

#include "controller.h"

#include <wType/string.h>
#include <wType/vocabulary.h>
#include <wType/event.h>

#include <QtCore/QObject>

namespace C {
    class String : public C::Controller
    {
        Q_OBJECT
    public:
        String(const W::Address p_address, QObject* parent = 0);
        ~String();
        
    signals:
        void change(const QString& str);
        
    protected:
        W::String data;
        
        virtual void h_get(const W::Event& ev);
        
    private:
        static uint64_t counter;
    };
}

#endif // CONTROLLER_STRING_H
