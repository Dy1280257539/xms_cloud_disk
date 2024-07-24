#pragma once
#include "xservice_client.h"
#include "xms_disk_client_gui.pb.h"
#include <fstream>
#include "xtools.h"
class XDownloadClient :public XServiceClient
{
public:
    XDownloadClient();
    ~XDownloadClient();
    virtual void ConnectedCB() override;
    void DownloadFileRes(xmsg::XMsgHead *head, XMsg *msg);
    void DownloadSliceReq(xmsg::XMsgHead *head, XMsg *msg);

    static void RegMsgCallback()
    {
        RegCB((xmsg::MsgType)xdisk::DOWNLOAD_FILE_RES, (MsgCBFunc)&XDownloadClient::DownloadFileRes);
        RegCB((xmsg::MsgType)xdisk::DOWNLOAD_SLICE_REQ, (MsgCBFunc)&XDownloadClient::DownloadSliceReq);
    }
    bool set_file(xdisk::XFileInfo file);

    void TimerCB() override;

    
    //void set_save_path(std::string path) { this->save_path_ = path; }
private:
    int task_id_ = 0;
    //std::string save_path_ = "";


    xdisk::XFileInfo file_;
    std::ofstream ofs_;

    //char *slice_buf_ = 0;
    //char *slice_buf_enc_ = 0;

    //��ʼ��������ʱ���Ѿ����͵�ֵ��Ҫȷ�������Ѿ������ͳɹ�
    long long begin_recv_data_size_ = -1;

    //std::list<std::string> md5_base64s_;
    std::string all_md5_base64_ = "";

    //�����ļ���
    XAES *aes_ = 0;

};

