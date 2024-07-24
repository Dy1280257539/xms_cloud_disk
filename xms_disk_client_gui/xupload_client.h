#pragma once
#include "xservice_client.h"
#include "xms_disk_client_gui.pb.h"
#include <fstream>
#include <list>
#include "xtools.h"
//ÿ���ļ�����һ��upload����
class XUploadClient :public XServiceClient
{
public:
    XUploadClient();
    ~XUploadClient();
    virtual void ConnectedCB() override;
    void UploadFileRes(xmsg::XMsgHead *head, XMsg *msg);
    void UploadFileEndRes(xmsg::XMsgHead *head, XMsg *msg);
    void SendSliceRes(xmsg::XMsgHead *head, XMsg *msg);
    
    static void RegMsgCallback()
    {
        RegCB((xmsg::MsgType)xdisk::UPLOAD_FILE_RES, (MsgCBFunc)&XUploadClient::UploadFileRes);
        RegCB((xmsg::MsgType)xdisk::UPLOAD_FILE_END_RES, (MsgCBFunc)&XUploadClient::UploadFileEndRes);
        RegCB((xmsg::MsgType)xdisk::SEND_SLICE_RES, (MsgCBFunc)&XUploadClient::SendSliceRes);
    }

    bool set_file(xdisk::XFileInfo file);

    //�趨�ļ��ļ��ܵ� �̰߳�ȫ
    //void set_password(std::string password);
    //std::string password();


    void TimerCB() override;

    int task_id = 0;
private:

    //��ȡ�ļ� �������ļ�Ƭ
    void SendSlice();

    //�ϴ��ļ���Ϣ
    xdisk::XFileInfo file_;

    //�ϴ����ļ�
    std::ifstream ifs_;

    //��ȡ�ļ�Ƭ���õĻ���
    char *slice_buf_ = 0;

    //��ȡ�ļ�Ƭ���õĻ���(���ܺ�)
    char *slice_buf_enc_ = 0;

    //��ʼ��������ʱ���Ѿ����͵�ֵ��Ҫȷ�������Ѿ������ͳɹ�
    long long begin_send_data_size_ = -1;

    std::list<std::string> md5_base64s_;

    std::string password_;
    std::mutex  password_mutex_;

    //�����ļ���
    XAES *aes_ = 0;

};

