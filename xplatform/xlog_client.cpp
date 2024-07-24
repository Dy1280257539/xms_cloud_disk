#include "xlog_client.h"
#include "xtools.h"
#include <string>
#define LOG_LIST_MAX 100
using namespace std;
using namespace xmsg;

namespace xms
{
    void XLog(xmsg::XLogLevel level, std::string msg, const char *filename, int line)
    {
        XAddLogReq req;
        req.set_log_level(level);
        req.set_log_txt(msg);
        req.set_filename(filename);
        req.set_line(line);
        XLogClient::Get()->AddLog(&req);
    }
}
void XLogClient::AddLog(const xmsg::XAddLogReq *req)
{
    if (!req)return;
    if (req->log_level() < log_level_)
    {
        return;
    }
    string level_str = "Debug";
    switch (req->log_level())
    {
    case XLOG_DEBUG:
        level_str = "DEBUG";
        break;
    case XLOG_INFO:
        level_str = "INFO";
        break;
    case XLOG_ERROR:
        level_str = "ERROR";
        break;
    case XLOG_FATAL:
        level_str = "FATAL";
        break;
    default:
        break;
    }

    string log_time = XGetTime(0, "%F %T");
    stringstream log_text;
    log_text << "=========================================================\n";
    log_text << log_time<<" "<<level_str<<"|"<<req->filename()<<":"<<req->line()<<"\n";
    log_text << req->log_txt()<<"\n";
    cout << log_text.str() << endl;
    if (log_ofs_)
    {
        log_ofs_.write(log_text.str().c_str(), log_text.str().size());
    }
    XAddLogReq tmp = *req;
    if (tmp.log_time() <= 0)
    {
        tmp.set_log_time(time(0));
    }
    tmp.set_service_port(service_port_);
    tmp.set_service_name(service_name_);

    logs_mutex_.lock();
    if (logs_.size() > LOG_LIST_MAX)
    {
        logs_.pop_front();
        cout << "-";
    }
    logs_.push_back(tmp);
    logs_mutex_.unlock();
}

void XLogClient::TimerCB()
{
    for (;;)
    {
        XAddLogReq log;
        {
            XMutex mutex(&logs_mutex_);
            if (logs_.empty())
                return;
            log = logs_.front();
            logs_.pop_front();
        }
        SendMsg(MSG_ADD_LOG_REQ, &log);
    }
}
XLogClient::XLogClient()
{
}


XLogClient::~XLogClient()
{
}
bool XLogClient::StartLog()
{
    if (strlen(server_ip()) == 0)
        set_server_ip("127.0.0.1");
    if (server_port() <= 0)
        set_service_port(XLOG_PORT);

    set_auto_connect(true);
    set_timer_ms(100);

    StartConnect();
    return true;

}