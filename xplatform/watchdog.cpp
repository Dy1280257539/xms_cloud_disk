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
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <string>
using namespace std;
int main(int argc,char *argv[])
{
    //3 程序退出后过多长时间重启
    int timeval = 3;
    if(argc<3)
    {
        cout<<"Useage: ./watchdog 3 ./register_server 20011"<<endl;
        return -1;
    }
    //启动子进程，主进程退出，交给1进程（守护进程后台 ）
    int pid = 0;
    pid = fork();
    if(pid > 0) exit(0);//父进程退出 交给1进程
    setsid();   //产生新的会话 与父进程脱离
    umask(0);   //掩码
    
    //进入守护进程

    //重启间隔
    timeval = atoi(argv[1]);

    //参数传递
    string cmd = "";
    for(int i = 2; i<argc; i++)
    {
        cmd +=" ";
        cmd +=argv[i];
    }
    cout<<cmd<<endl;
    for(;;)
    {
        //启动进程，并等待进程退出
        int ret = system(cmd.c_str());
        sleep(timeval);
    }

    return 0;
}