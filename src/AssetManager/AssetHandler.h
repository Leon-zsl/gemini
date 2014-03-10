#ifndef Gemini_AssetHandler_h
#define Gemini_AssetHandler_h

#include <string>

namespace base
{
#define ASSET_OK                        0
#define ASSET_ERR_UNKNOWNASSET         -1
#define ASSET_ERR_LOADFAILED           -2
#define ASSET_ERR_DOWNLOADFAILED       -3

struct AssetLoadStatus
{
    int             errcode;
    int             internal_errcode;
    char*           data;
    int             len;
    
    AssetLoadStatus()
    :errcode(0),
    internal_errcode(0),
    data(NULL),
    len(0)
    {}
};

class AssetHandler
{
public:
    virtual void Handle(const std::string& asset, AssetLoadStatus status) = 0;
};
  
#define AF_USE_LOCAL                    0
#define AF_USE_REMOTE                   1
#define AF_UNKNOWNASSET                -1
#define AF_DOWNLOADFAILED              -2

struct AssetFileStatus
{
    int             status;
    int             internal_errorcode;
    std::string     full_path;
};
    
class AssetFileHandler
{
public:
    virtual void Handle(const std::string& asset, AssetFileStatus status) = 0;
};
}
#endif
