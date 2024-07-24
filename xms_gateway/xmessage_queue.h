/*/*******************************************************************************
**                                                                            **
**                     Jiedi(China nanjing)Ltd.                               **
**	               ������������ �Ĳܿ����˴��������Ϊѧϰ�ο�                **
*******************************************************************************/

/*****************************FILE INFOMATION***********************************
**
** Project       : C++΢����ܹ�����ȫ������Ŀʵѵ
** Contact       : xiacaojun@qq.com
**  ����   : http://blog.csdn.net/jiedichina
**	��Ƶ�γ� : ��Ѷ����		https://jiedi.ke.qq.com/				
			   
** 			   ���Ŀ���		http://www.laoxiaketang.com 
**                 
**   C++΢����ܹ�����ȫ������Ŀʵѵ �γ�Ⱥ 774313419�� ����Ⱥ���ش���ͽ���
**   ΢�Ź��ں�  : jiedi2007
**		ͷ����	 : �Ĳܿ�
**
*****************************************************************************
//������������������ C++΢����ܹ�����ȫ������Ŀʵѵ �γ�  QQȺ��774313419 ���ش���ͽ���*/

#ifndef XMESSAGE_QUEUE_H
#define XMESSAGE_QUEUE_H
#include <map>
#include <list>
enum XMessageType
{
	XMESSAGE_TYPE_NONE = 0,
	XMESSAGE_AUTH,				//��Ȩ��Ϣ
	XMESSAGE_AUTH_ERROR,		//��Ȩ�쳣
	XMESSAGE_RELAY ,			//ת����Ϣ
	XMESSAGE_REPLY,				//�ظ���Ϣ
};


//�������Ϣ�̳�
struct XMessage
{
	//��Ϣ����
	XMessageType message_type = XMESSAGE_TYPE_NONE;
	bool is_ok = false;		//�Ƿ���ɹ�
	std::string msg = "";
	//����ռ�
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