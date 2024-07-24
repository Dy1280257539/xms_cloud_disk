#pragma once
#include "xservice_client.h"
#include <string>
#include <map>
#include <mutex>
#include <thread>
class XAuthClient:public XServiceClient
{
public:
    static XAuthClient*Get()
    {
        static XAuthClient xc;
        return &xc;
    }
    XAuthClient();
    ~XAuthClient();

    /////////////////////////////////////////////////////////////
    /// 登录
    /// @para username 用户名
    /// @para password 密码（明文),在函数中会经过md5_base64编码后发送
    void LoginReq(std::string username, std::string password);

    
    void AddUserReq(xmsg::XAddUserReq *user);

    static void RegMsgCallback()
    {
        RegCB(xmsg::MSG_LOGIN_RES, (MsgCBFunc)&XAuthClient::LoginRes);
        RegCB(xmsg::MSG_ADD_USER_RES, (MsgCBFunc)&XAuthClient::AddUserRes);
    }

    bool GetLoginInfo(std::string username, xmsg::XLoginRes *out_info, int timeout_ms);

private:
    void AddUserRes(xmsg::XMsgHead *head, XMsg*msg);
    void LoginRes(xmsg::XMsgHead *head, XMsg*msg);
    
    std::map<std::string, xmsg::XLoginRes> login_map_;
    std::mutex logins_mutex_;
};

