#include "xauth_client.h"
#include "xtools.h"
#include <iostream>
using namespace std;
using namespace xmsg;
void XAuthClient::LoginReq(std::string username, std::string password)
{
    XLoginReq req;
    req.set_username(username);
    auto md5_pass = XMD5_base64((unsigned char*)password.data(), password.size());
    req.set_password(md5_pass);
    cout << req.DebugString();
    {
        XMutex mux(&logins_mutex_);
        login_map_.erase(username);
    }
    XMsgHead head;
    head.set_service_name(AUTH_NAME);
    head.set_msg_type(MSG_LOGIN_REQ);
    XMsgEvent::SendMsg(&head, &req);
}

void XAuthClient::AddUserReq(xmsg::XAddUserReq *user)
{
    auto md5_pass = XMD5_base64((unsigned char*)user->password().data(), user->password().size());
    user->set_password(md5_pass);
    SendMsg(MSG_ADD_USER_REQ, user);
}

void XAuthClient::AddUserRes(xmsg::XMsgHead *head, XMsg*msg)
{
    XMessageRes res;
    if (!res.ParseFromArray(msg->data, msg->size))
    {
        return;
    }
    if (res.return_() == XMessageRes::ERROR)
    {
        cout << res.msg() << endl;
        return;
    }
    cout << "Adduser success!" << endl;
}

void XAuthClient::LoginRes(xmsg::XMsgHead *head, XMsg*msg)
{
    XLoginRes res;
    if (!res.ParseFromArray(msg->data, msg->size))
    {
        return;
    }
    cout << res.DebugString();
    {
        XMutex mux(&logins_mutex_);
        if (res.username().empty())
            return;
        login_map_[res.username()] = res;
    }
}
bool XAuthClient::GetLoginInfo(std::string username, xmsg::XLoginRes *out_info, int timeout_ms)
{
    if (!out_info)return false;
    int count = timeout_ms / 10;
    if (count <= 0)count = 1;
    int i = 0;
    for (; i < count; i++)
    {
        logins_mutex_.lock();
        auto login_ptr = login_map_.find(username);
        if (login_ptr == login_map_.end())
        {
            logins_mutex_.unlock();
            this_thread::sleep_for(10ms);
            continue;
        }
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        auto login = login_ptr->second;
        logins_mutex_.unlock();
        if (login.res() != XLoginRes::OK)
        {
            return false;
        }
        out_info->CopyFrom(login);
        return true;
    }
    return false;
}


XAuthClient::XAuthClient()
{
}


XAuthClient::~XAuthClient()
{
}
