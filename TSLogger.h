#ifndef __TSLOGGER_H__
#define __TSLOGGER_H__

//--------------EXTERNAL INCLUDES ------------------
#include <fstream> //for fstream
#include <ctime> //for timestamp
#include <cstdarg>//variadic
//--------------INTERNAL INCLUDES ------------------
#include "TSSingleton.h"


enum eTSLogLevel
{
    eLOG_TRACE,
    eLOG_INFO,
    eLOG_WARN,
    eLOG_ERROR,
    eLOG_FATAL
};

enum eRetValue
{
    eSUCCESS,
    eERROR
};



#define EXPAND(x) EXPAND_SIMPLE(x)
#define EXPAND_SIMPLE(x) #x


#define TSLOG_TRACE(msg,...)        \
do {                            \
    TSLogger::getInstance().TSTSwrite(eLOG_TRACE,msg,##__VA_ARGS__);      \
}while(0)

#define TSLOG_INFO(msg,...)         \
do {                            \
    TSLogger::getInstance().TSwrite(eLOG_INFO,msg,##__VA_ARGS__);       \
}while(0)

#define TSLOG_WARN(msg,...)         \
do {                            \
    TSLogger::getInstance().TSwrite(eLOG_WARN,msg,##__VA_ARGS__);       \
}while(0)
#define TSLOG_ERROR(msg,...)         \
do {                            \
    TSLogger::getInstance().TSwrite(eLOG_ERROR,msg,##__VA_ARGS__);       \
}while(0)
#define TSLOG_FATAL(fmt,...)         \
do {                            \
    TSLogger::getInstance().TSwrite(eLOG_FATAL,msg,##__VA_ARGS__);       \
}while(0)




class TSLogger : public TSSingleton<TSLogger>
{
     TSLogger(){}
     eRetValue setAssociatedFile(const std::string & ) ;
     eRetValue setLogLevel(eTSLogLevel );
     std::string getTimeStamp();


public:

    eRetValue init(const std::string & ,eTSLogLevel ) ;
    eRetValue TSwrite(eTSLogLevel,const char *,...);
    eTSLogLevel getLogLevel();
    friend class TSSingleton<TSLogger> ;
protected:

private:
    std::string     mLogFileName;
    eTSLogLevel     mLogLevel;
    std::ofstream   mFileStream ;
    boost::mutex    mMutex;
    std::map<eTSLogLevel , std::string > mLogLevelMap;


};



#endif // TSLOGGER_H_INCLUDED
