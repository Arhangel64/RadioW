#ifndef HANDLER_H
#define HANDLER_H

#include "node.h"

namespace W 
{
    template<typename InstanceType, typename MethodType>
    class ImplHandle;
    
    class Handler: public Node
    {
        Q_OBJECT
        
        
        
    public:
        Handler(const Address& p_rel_addr, QObject* parent = 0);
        ~Handler();
        
        template<typename InstanceType, typename MethodType>
        static Handler* create(const Address& addr, InstanceType* inst, MethodType mth, QObject* parent = 0)
        {
            return new ImplHandle<InstanceType, MethodType>(addr, inst, mth, parent);
        }
    };
    
    template<typename InstanceType, typename MethodType>
        class ImplHandle: public Handler
        {
        public:
            ImplHandle(const Address& p_rel_addr, InstanceType *p_inst, MethodType p_mth, QObject* parent = 0):
                Handler(p_rel_addr, parent),
                inst(p_inst),
                mth(p_mth)
            {}
            
            ~ImplHandle() {}
            
            void pass(const W::Event& ev) const
            {
                ( ( *inst ).*mth )( ev );
            }
            
        private:
            InstanceType* inst;
            MethodType mth;
        };
    
}

#endif // HANDLER_H
