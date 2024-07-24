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

#ifndef XMESSAGE_QUEUE_H
#define XMESSAGE_QUEUE_H
#include <map>
#include <list>
enum XMessageType
{
	XMESSAGE_TYPE_NONE = 0,
	XMESSAGE_AUTH,				//鉴权消息
	XMESSAGE_AUTH_ERROR,		//鉴权异常
	XMESSAGE_RELAY ,			//转发消息
	XMESSAGE_REPLY,				//回复消息
};


//具体的消息继承
struct XMessage
{
	//消息分类
	XMessageType message_type = XMESSAGE_TYPE_NONE;
	bool is_ok = false;		//是否处理成功
	std::string msg = "";
	//清理空间
    virtual void Clear() {};
};

class XMessageQueue
{
public:

	void Push(XMessage msg);
	XMessage Pull(XMessageType type);
	void Main();
	void Start();
	void Stop()
	{
		is_exit_ = true;
	}
private:
	bool is_exit_ = false;
	std::mutex messages_mutex_;
	std::map<XMessageType, std::list<XMessage> >messages_;
};
#endif