#include "AssetSettingUtil.h"

#include <stdio.h>

namespace base
{
void AssetSettingUtil::SaveSettings(const std::string& file_name, std::map<std::string, FileInfo>& info_map)
{
    if(info_map.empty()) return;
    
    FILE* file = fopen(file_name.c_str(), "wb");
    if(file == NULL) {
        printf("save settings file failed!");
        return;
    }
    
    for(std::map<std::string, FileInfo>::iterator itr = info_map.begin(); itr != info_map.end(); ++itr) {
        std::string val = itr->first + ":";
        if(itr->second.expired) {
            val += itr->second.local_md5 + "\n";
        } else {
            val += itr->second.remote_md5 + "\n";
        }
        size_t len = fwrite(val.c_str(), 1, val.size(), file);
        if(len != val.size()) {
            printf("save settings failed");
            break;
        }
    }
    
    fflush(file);
    fclose(file);
}

void AssetSettingUtil::ParseLocalSettings(const char *data, int len, std::map<std::string, FileInfo> &info_map)
{
    ParseSettings(data, len, info_map, HandleLocalSettings);
}

void AssetSettingUtil::ParseRemoteSettings(const char *data, int len, std::map<std::string, FileInfo> &info_map)
{
    ParseSettings(data, len, info_map, HandleRemoteSettings);
}

void AssetSettingUtil::ParseSettings(const char* data, int len, std::map<std::string, FileInfo>& info_map, Handler h)
{
    if(data == NULL || len == 0) return;
    
    int index = 0;
    std::string line;
    while(index < len) {
        char val = data[index];
        
        if(val == ' ' || val == '\t' || val == '\r') {
            line = "";
            index++;
            continue;
        }
        
        if(val == '\n' || index == len - 1) {
            size_t pos = line.find(':');
            if(pos <=0 || pos >= line.size() - 1) {
                line = "";
                index++;
                continue;
            }
            
            std::string file = line.substr(0, pos);
            std::string md5 = line.substr(pos + 1, line.size() - (pos + 1));
            h(file, md5, info_map);
            
            line = "";
            index++;
            continue;
        }
        
        line.push_back(val);
        index++;
    }
}

void AssetSettingUtil::HandleLocalSettings(const std::string& file, const std::string& md5, std::map<std::string, FileInfo>& info_map)
{
    std::map<std::string, FileInfo>::iterator itr = info_map.find(file);
    if(itr != info_map.end()) {
        printf("duplicated file found: %s", file.c_str());
    }
    
    FileInfo info;
    info.name = file;
    info.expired = false;
    info.local_md5 = md5;
    info_map[file] = info;
}

void AssetSettingUtil::HandleRemoteSettings(const std::string& file, const std::string& md5, std::map<std::string, FileInfo>& info_map)
{
    std::map<std::string, FileInfo>::iterator itr = info_map.find(file);
    if(itr == info_map.end()) {
        FileInfo info;
        info.name = file;
        info.expired = true;
        info.remote_md5 = md5;
        info_map[file] = info;
    } else {
        FileInfo& info = itr->second;
        info.remote_md5 = md5;
        if(md5 == info.local_md5)
            info.expired = false;
        else
            info.expired = true;
        info_map[file] = info;
    }
}
}