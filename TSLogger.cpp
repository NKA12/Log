#include "TSLogger.h"


eRetValue TSLogger::setLogLevel(eTSLogLevel _logLevel)
{
     mLogLevel = _logLevel;
}

//----------------------------------------------------------------------------------------
eRetValue TSLogger::setAssociatedFile(const std::string & _fileName)
{
    mLogFileName= _fileName;
}
//----------------------------------------------------------------------------------------
eRetValue TSLogger::init(const std::string & _fileName,eTSLogLevel _logLevel)
{
    boost::mutex::scoped_lock scopedLock(mMutex);
    setLogLevel(_logLevel);
    setAssociatedFile(_fileName);
    if( mFileStream.is_open() )
    {
        std::cout << "File: " << mLogFileName << " is already open " << std::endl;
    }
    mFileStream.open( mLogFileName.c_str(), std::ofstream::out |std::ofstream::trunc );
    if( !mFileStream )
    {
        std::cout << "Could not open File: " << mLogFileName << std::endl;
    }
    else
    {
        std::cout << "Correctly opened File " << mLogFileName << std::endl;
    }

    mLogLevelMap.insert(std::pair<eTSLogLevel , std::string>(eLOG_TRACE,"[TRACE]"));
    mLogLevelMap.insert(std::pair<eTSLogLevel , std::string>(eLOG_INFO,"[INFO]"));
    mLogLevelMap.insert(std::pair<eTSLogLevel , std::string>(eLOG_WARN,"[WARN]"));
    mLogLevelMap.insert(std::pair<eTSLogLevel , std::string>(eLOG_ERROR,"[ERROR]"));
    mLogLevelMap.insert(std::pair<eTSLogLevel , std::string>(eLOG_FATAL,"[FATAL]"));

}
//----------------------------------------------------------------------------------------
eTSLogLevel TSLogger::getLogLevel()
{
     boost::mutex::scoped_lock scopedLock(mMutex);
     return mLogLevel;
}

//----------------------------------------------------------------------------------------
eRetValue TSLogger::TSwrite( eTSLogLevel _level,const char * fmt,...)
{
    if(mLogLevel <= _level)
    {
        va_list argptr;
        va_start(argptr, fmt);
        char buf[500] ;
        vsprintf(buf, fmt, argptr);
        va_end(argptr);

        std::ostringstream _logMessage;
       _logMessage << __FILE__;
       _logMessage<< ":" ;
       _logMessage<< EXPAND(__LINE__) << " " ;
       #ifdef USETIMESTAMP
       _logMessage << getTimeStamp() << " " ;
       #endif // USETIMESTAMP
       //no need for a mutex (read action)
       std::map<eTSLogLevel , std::string >::const_iterator cIter =  mLogLevelMap.find(_level);
       if(cIter != mLogLevelMap.end())
            _logMessage << cIter->second << " " ;
       _logMessage << buf;
       std::cout <<_logMessage.str() << std::endl;
        boost::mutex::scoped_lock scopedLock(mMutex);
        if( mFileStream.is_open() )
        {
            mFileStream << _logMessage.str() << std::endl;
        }
        else
            std::cout << "No Log File is configured" << std::endl;
    }


}
//----------------------------------------------------------------------------------------
std::string TSLogger::getTimeStamp()
{

     char lctime[20];
     std::time_t lc = std::time(NULL);
     strftime(lctime,sizeof(lctime),"%Y-%m-%d %H:%M:%S",std::localtime(&lc));
     return lctime ;
}
