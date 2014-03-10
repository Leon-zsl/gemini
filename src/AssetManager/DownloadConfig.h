#ifndef __Example_DownloadConfig__
#define __Example_DownloadConfig__

#include <string>

namespace base
{
struct DownloadConfig
{
public:
    //root dir of remote asset
    std::string serverAddr;

    //root dir of local asset
    std::string localDir;
    
    //max download resource count in parallel
    int maxDownloadTask;
    
    DownloadConfig()
    :maxDownloadTask(8)
    {}
    
    DownloadConfig(const std::string& servaddr,
                   const std::string& locdir,
                   int maxtask)
    :serverAddr(servaddr),
    localDir(locdir),
    maxDownloadTask(maxtask)
    {}
};
}
#endif
