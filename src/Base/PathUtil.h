#ifndef Example_PathUtil_h
#define Example_PathUtil_h

#include <string>

namespace base
{
class PathUtil
{
public:
    static std::string Join(const std::string& p0, const std::string& p1) {
        if(p0.empty()) return p1;
        if(p1.empty()) return p0;
        if(p0[p0.size() -1] == '/' || p1[0] == '/') return p0 + p1;
        return p0 + '/' + p1;
    }
    
    static std::string GetPath(const std::string& p) {
        if(p.empty()) return p;
        if(p[p.size() - 1] == '/') return p.substr(0, p.size() - 1);
        size_t pos = p.rfind('/');
        if(pos <= 0) return "";
        return p.substr(0, pos);
    }
    
    static std::string GetFile(const std::string& p) {
        if(p.empty()) return p;
        if(p[p.size() - 1] == '/') return "";
        
        size_t pos = p.rfind('/');
        if(pos <= 0) return p;
        return p.substr(pos + 1, p.size() - (pos + 1));
        
    }
};
}

#endif
