#ifndef DEFINES_UTILS_H
#define DEFINES_UTILS_H


#define handler(HANDLER)    \
    void _h_##HANDLER(const W::Event& ev) {h_##HANDLER(ev);}\
    virtual void h_##HANDLER(const W::Event& ev);\
    
#endif
