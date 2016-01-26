#ifndef HANDLER_H
#define HANDLER_H

#include "wType/address.h"
#include "wType/event.h"

namespace W 
{
    template<typename InstanceType, typename MethodType>
    class ImplHandle;
    
    class Handler
    {
    public:
        Handler(const Address& p_rel_addr);
        ~Handler();
        
        template<typename InstanceType, typename MethodType>
        static Handler* create(const Address& addr, InstanceType* inst, MethodType mth)
        {
            return new ImplHandle<InstanceType, MethodType>(addr, inst, mth);
        }
        
        const W::Address& getAddress() const;
        
        virtual void pass(const W::Event& ev) const = 0;
        
    private:
        W::Address address;
    };
    
    template<typename InstanceType, typename MethodType>
        class ImplHandle: public Handler
        {
        public:
            ImplHandle(const Address& p_rel_addr, InstanceType *p_inst, MethodType p_mth):
                Handler(p_rel_addr),
                inst(p_inst),
                mth(p_mth)
            {}
            
            ~ImplHandle() {}
            
            void pass(const W::Event& ev) const
            {
                ( ( *inst ).*mth )(ev);
            }
            
        private:
            InstanceType* inst;
            MethodType mth;
        };
    
}

#endif // HANDLER_H
