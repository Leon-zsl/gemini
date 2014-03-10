#ifndef __Gemini__AssetSettingUtil__
#define __Gemini__AssetSettingUtil__

#include <map>
#include <string>

namespace base
{
struct FileInfo
{
    std::string         name;
    std::string         local_md5;
    std::string         remote_md5;
    bool                expired;
};

class AssetSettingUtil
{
public:
    static void SaveSettings(const std::string& file_name, std::map<std::string, FileInfo>& info_map);
    static void ParseLocalSettings(const char* data, int len, std::map<std::string, FileInfo>& info_map);
    static void ParseRemoteSettings(const char* data, int len, std::map<std::string, FileInfo>& info_map);
    
private:
    typedef void (*Handler)(const std::string& file, const std::string& md5, std::map<std::string, FileInfo>& info_map);
    static void ParseSettings(const char* data, int len, std::map<std::string, FileInfo>& info_map, Handler h);
    static void HandleLocalSettings(const std::string& file, const std::string& md5, std::map<std::string, FileInfo>& info_map);
    static void HandleRemoteSettings(const std::string& file, const std::string& md5, std::map<std::string, FileInfo>& info_map);
};
}
#endif
