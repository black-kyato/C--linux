
#include <fun.h>

class MyLog
{
private:
    MyLog(bool b)
    {
        outToScreen = b;
    }
    ~MyLog(){}
    static MyLog * log;
    bool outToScreen;//是否输出日志信息到屏幕
    static std::string _screenInfo;//屏幕日志信息
    static std::string _logName;//文件日志名称
    static log4cpp::Category& logCat;
    static log4cpp::Category& coutCat;
    static log4cpp::FileAppender* logFile;//文件日志输入
    static log4cpp::OstreamAppender* logScreen;//屏幕日志输入
    static log4cpp::Priority::PriorityLevel logPri;//文件日志优先级
    static log4cpp::Priority::PriorityLevel coutPri;//屏幕日志优先级
    static log4cpp::PatternLayout* logLayout;//日志布局 
    static log4cpp::PatternLayout* screenLayout;//屏幕布局 
public:
    //获取日志函数,默认参数选择是否输出到屏幕
    static MyLog* getLog(bool toScreen = true,std::string coutName ="screenInfo",std::string logName = "log"){
        if(MyLog::log == NULL)
        {
            MyLog::log = new MyLog(toScreen);
            
            MyLog::_logName = logName;
            MyLog::_screenInfo = coutName;
            
            logScreen = new log4cpp::OstreamAppender("logScreen",&std::cout);
            logFile = new log4cpp::FileAppender("logFile",MyLog::_logName);
            
            //设置布局
            MyLog::logLayout = new log4cpp::PatternLayout();
            MyLog::screenLayout = new log4cpp::PatternLayout();
            logLayout->setConversionPattern("%d{%Y/%m/%d,%H:%M:%S} -- [%p] %c: %m%n");
            screenLayout->setConversionPattern("%d{%Y/%m/%d %H:%M:%S} -- [%p] %c: %m%n");
            MyLog::logScreen->setLayout(screenLayout);
            MyLog::logFile->setLayout(logLayout);

            //追加到目录
            MyLog::logCat.addAppender(MyLog::logFile);
            MyLog::coutCat.addAppender(MyLog::logScreen);
            //设置优先级
            MyLog::logCat.setPriority(MyLog::logPri);
            MyLog::coutCat.setPriority(MyLog::coutPri);
        }
        MyLog::log->outToScreen = toScreen;


        return MyLog::log;
    }
    //销毁日志对象
    static void destoryLog()
    {
        log4cpp::Category::shutdown();
        delete MyLog::log;
    }
    //设置日志记录优先级
    static void setPri(log4cpp::Priority::PriorityLevel coutLevel,log4cpp::Priority::PriorityLevel logLevel)
    {
        MyLog::logPri = logLevel;
        MyLog::coutPri = coutLevel;
        MyLog::logCat.setPriority(MyLog::logPri);
        MyLog::coutCat.setPriority(MyLog::coutPri);
    }
    //记录日志,调用参数__FILE__, __LINE__ ,__FUNCTION__
    void warn(const char * msg,const char *filename = __FILE__,int line = __LINE__,const char *function = "warn")
    {
        char info[4096] = {0};
        sprintf(info,"\nIn file %s,line %d,function %s:%s",filename,line,function,msg);
        if(this->outToScreen)
        {
            logCat.warn(info);
            coutCat.warn(info);
        }
        else
        {
            logCat.warn(info);
        }
    }
    void error(const char * msg,const char *filename = __FILE__,int line = __LINE__,const char *function = "error")
    {
        char info[4096] = {0};
        sprintf(info,"\nIn file %s,line %d,function %s:%s",filename,line,function,msg);
        if(this->outToScreen)
        {
            logCat.error(info);
            coutCat.error(info);
        }
        else
        {
            logCat.error(info);
        }
    }
    void debug(const char * msg,const char *filename = __FILE__,int line = __LINE__,const char *function = "debug")
    {
        char info[4096] = {0};
        sprintf(info,"\nIn file %s,line %d,function %s:%s",filename,line,function,msg);
        if(this->outToScreen)
        {
            logCat.debug(info);
            coutCat.debug(info);
        }
        else
        {
            logCat.debug(info);
        }
    }
    void info(const char * msg,const char *filename = __FILE__,int line = __LINE__,const char *function = "info")
    {
        char info[4096] = {0};
        sprintf(info,"\nIn file %s,line %d,function %s:%s",filename,line,function,msg);

        if(this->outToScreen)
        {
            logCat.info(info);
            coutCat.info(info);
        }
        else
        {
            logCat.info(info);
        }
    }
};
MyLog* MyLog::log = NULL;
std::string MyLog::_screenInfo = "screenInfo";
std::string MyLog::_logName = "log";

log4cpp::Category& root = log4cpp::Category::getRoot();
log4cpp::Category& MyLog::logCat = root.getInstance(MyLog::_logName);
log4cpp::Category& MyLog::coutCat = root.getInstance(MyLog::_screenInfo);

log4cpp::Priority::PriorityLevel MyLog::coutPri = log4cpp::Priority::INFO;
log4cpp::Priority::PriorityLevel MyLog::logPri = log4cpp::Priority::NOTSET;

log4cpp::PatternLayout* MyLog::logLayout  = NULL;
log4cpp::PatternLayout* MyLog::screenLayout  = NULL;

log4cpp::FileAppender* MyLog::logFile = NULL;//文件日志输入
log4cpp::OstreamAppender* MyLog::logScreen = NULL;//屏幕日志输入

#define  MyLogWARN(msg) MyLog::getLog()->warn(msg,__FILE__,__LINE__,__FUNCTION__);
#define  MyLogINFO(msg) MyLog::getLog()->info(msg,__FILE__,__LINE__,__FUNCTION__);
#define  MyLogERROR(msg) MyLog::getLog()->error(msg,__FILE__,__LINE__,__FUNCTION__);
#define  MyLogDEBUG(msg) MyLog::getLog()->debug(msg,__FILE__,__LINE__,__FUNCTION__);

