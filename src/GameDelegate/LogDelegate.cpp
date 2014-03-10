#include "LogDelegate.h"

#include <stdio.h>

void LogDelegate::Accept(FxDelegateHandler::CallbackProcessor* cbreg)
{
    cbreg->Process("Log_Debug", LogDelegate::_Debug);
    cbreg->Process("Log_Info", LogDelegate::_Info);
    cbreg->Process("Log_Warn", LogDelegate::_Warn);
    cbreg->Process("Log_Error", LogDelegate::_Error);
}

void LogDelegate::_Debug(const FxDelegateArgs &params)
{
    const char* log = params[0].GetString();
    printf("debug: %s\n", log);
}

void LogDelegate::_Info(const FxDelegateArgs &params)
{
    const char* log = params[0].GetString();
    printf("info: %s\n", log);
}

void LogDelegate::_Warn(const FxDelegateArgs &params)
{
    const char* log = params[0].GetString();
    printf("warn: %s\n", log);
}

void LogDelegate::_Error(const FxDelegateArgs &params)
{
    const char* log = params[0].GetString();
    printf("error: %s\n", log);
}
