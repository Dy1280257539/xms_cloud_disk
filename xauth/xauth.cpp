/*/*******************************************************************************
**                                                                            **
**                     Jiedi(China nanjing)Ltd.                               **
**	               创建：丁宋涛 夏曹俊，此代码可用作为学习参考                **
*******************************************************************************/

/*****************************FILE INFOMATION***********************************
**
** Project       : C++微服务架构及安全云盘项目实训
** Contact       : xiacaojun@qq.com
**  博客   : http://blog.csdn.net/jiedichina
**	视频课程 : 腾讯课堂		https://jiedi.ke.qq.com/				
			   
** 			   老夏课堂		http://www.laoxiaketang.com 
**                 
**   C++微服务架构及安全云盘项目实训 课程群 774313419： 加入群下载代码和交流
**   微信公众号  : jiedi2007
**		头条号	 : 夏曹俊
**
*****************************************************************************
//！！！！！！！！！ C++微服务架构及安全云盘项目实训 课程  QQ群：774313419 下载代码和交流*/
#include <iostream>
#include "xservice.h"
#include "xauth_handle.h"
#include "xauth_dao.h"
#include "xregister_client.h"
#include "xtools.h"
using namespace std;
class XAuthService :public XService
{
public:
    XServiceHandle *CreateServiceHandle() override
    {
        return new XAuthHandle();
    }
};
int main(int argc,char *argv[])
{
    int server_port = AUTH_PORT;
    auto ip = XGetHostByName("xms_register_server");
    XRegisterClient::Get()->set_server_ip(ip.c_str());
    XRegisterClient::Get()->set_server_port(REGISTER_PORT);
    XRegisterClient::Get()->RegisterServer(AUTH_NAME, server_port, 0);
    XAuthDao::Get()->Init();
    XAuthDao::Get()->Install();

    
    XAuthHandle::RegMsgCallback();
    XAuthService service;
    service.set_server_port(server_port);
    service.Start();
    XThreadPool::Wait();
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

