#ifndef __Example__LogDelegate__
#define __Example__LogDelegate__

#include "FxGameDelegate.h"

class LogDelegate : public FxDelegateHandler
{
public:
    static void _Debug(const FxDelegateArgs& params);
    static void _Info(const FxDelegateArgs& params);
    static void _Warn(const FxDelegateArgs& params);
    static void _Error(const FxDelegateArgs& params);
    
    virtual void Accept(FxDelegateHandler::CallbackProcessor* cbreg);
};

#endif
