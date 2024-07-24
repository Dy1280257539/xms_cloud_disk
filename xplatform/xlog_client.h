#pragma once

#ifdef _WIN32
#ifdef XCOM_EXPORTS
#define XCOM_API __declspec(dllexport)
#else
#define XCOM_API __declspec(dllimport)
#endif
#else
#define XCOM_API
#endif
#include "xservice_client.h"
#include <fstream>
#include <list>
namespace xms
{
    XCOM_API void XLog(xmsg::XLogLevel level, std::string msg, const char *filename, int line);
}
#define LOGDEBUG(msg) xms::XLog(xmsg::XLOG_DEBUG,msg,__FILE__,__LINE__);
#define LOGINFO(msg) xms::XLog(xmsg::XLOG_INFO,msg,__FILE__,__LINE__);
#define LOGERROR(msg) xms::XLog(xmsg::XLOG_ERROR,msg,__FILE__,__LINE__);
#define LOGFATAL(msg) xms::XLog(xmsg::XLOG_FATAL,msg,__FILE__,__LINE__);

class XCOM_API XLogClient :public XServiceClient
{
public:
    static XLogClient* Get()
    {
        static XLogClient xc;
        return &xc;
    }
    ~XLogClient();

    void AddLog(const xmsg::XAddLogReq *req);
    //bool StartLog();

    void set_log_level(xmsg::XLogLevel log_level) { log_level_ = log_level; }


    void set_local_file(std::string local_file)
    {
        log_ofs_.open(local_file);
    }
    void TimerCB() override;
    void set_service_port(int port) { service_port_ = port; }
    void set_service_name_(std::string name) { service_name_ = name; }
    bool StartLog();
private:
    xmsg::XLogLevel log_level_ = xmsg::XLOG_INFO;
    std::ofstream log_ofs_;
    std::list<xmsg::XAddLogReq> logs_;
    std::mutex logs_mutex_;
    int service_port_ = 0;
    std::string service_name_ ="";
    XLogClient();
};



