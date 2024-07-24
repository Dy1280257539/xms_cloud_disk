#include <iostream>
#include "xauth_client.h"
#include <thread>
using namespace std;
int main()
{
    std::cout << "XAuth Client!\n";
    XAuthClient::RegMsgCallback();
    XAuthClient client;
    client.set_server_ip("127.0.0.1");
    client.set_server_port(AUTH_PORT);
    client.StartConnect();
    while (!client.is_connected())
    {
        this_thread::sleep_for(100ms);
    }
    xmsg::XAddUserReq adduser;
    adduser.set_username("root");
    adduser.set_password("123456");
    adduser.set_rolename("root");
    client.AddUserReq(&adduser);
    client.LoginReq("root", "123456");
    XThreadPool::Wait();
}
