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

#include "xmessage_queue.h"
#include <thread>
#include "xtools.h"

using namespace std;
void XMessageQueue::Push(XMessage msg)
{
	XMutex mutex(&messages_mutex_);
	messages_[msg.message_type].push_back(msg);
}

XMessage XMessageQueue::Pull(XMessageType type)
{
	XMutex mutex(&messages_mutex_);
	if (messages_[type].empty())
		return XMessage();
	auto msg = messages_[type].front();
	messages_[type].pop_front();
	return msg;
}

void XMessageQueue::Main()
{
	LOGDEBUG("XMessageQueue::Start");
	while (!is_exit_)
	{
		//this_thread::sleep_for();
	}
}

void XMessageQueue::Start()
{
	thread th(&XMessageQueue::Main, this);
	th.detach();

}