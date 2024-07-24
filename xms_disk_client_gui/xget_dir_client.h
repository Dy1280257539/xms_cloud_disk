#pragma once
#include "xservice_client.h"
#include "xms_disk_client_gui.pb.h"

//��ʱ�������б��ȡ����
// ��Ҫ���ǵ�¼���Ƿ����XServiceClient��
// Ҫ�����Ȩ������

class XGetDirClient:public XServiceClient
{
public:
    static XGetDirClient* Get()
    {
        static XGetDirClient xc;
        return &xc;
    }
    //void TimerCB();
    XGetDirClient();

    ~XGetDirClient();

    void GetDirReq(xdisk::XGetDirReq req);
    void DeleteFileReq(xdisk::XFileInfo file);



    void GetDirRes(xmsg::XMsgHead *head, XMsg *msg);
    void DeleteFileRes(xmsg::XMsgHead *head, XMsg *msg);

    void NewDirReq(std::string path);
    
    void NewDirRes(xmsg::XMsgHead *head, XMsg *msg);


    //��ȡ�ϴ������صķ������б�
    void GetService() ;

    void GetServiceRes(xmsg::XMsgHead *head, XMsg *msg) ;


    //��ȡ���̿ռ�ʹ�����
    void GetDiskInfoReq();

    //��ȡ���̿ռ�ʹ�����
    void GetDiskInfoRes(xmsg::XMsgHead *head, XMsg *msg);

    //��ʱ����ȡ�ϴ������ط������б�
    virtual void TimerCB() override;


    static void RegMsgCallback()
    {
        RegCB((xmsg::MsgType)xdisk::GET_DIR_RES, (MsgCBFunc)&XGetDirClient::GetDirRes);
        RegCB((xmsg::MsgType)xdisk::DELETE_FILE_RES, (MsgCBFunc)&XGetDirClient::DeleteFileRes);
        RegCB((xmsg::MsgType)xdisk::NEW_DIR_RES, (MsgCBFunc)&XGetDirClient::NewDirRes);
        RegCB((xmsg::MsgType)xdisk::GET_DISK_INFO_RES, (MsgCBFunc)&XGetDirClient::GetDiskInfoRes);
        RegCB(xmsg::MSG_GET_OUT_SERVICE_RES, (MsgCBFunc)&XGetDirClient::GetServiceRes);

        //XServiceList
    }
    std::string cur_dir() { return cur_dir_; }

    //void set_login(const xmsg::XLoginRes &login) { this->login_ = login; }

private:
    std::string cur_dir_ = "";

    //��¼��Ϣ
    //xmsg::XLoginRes login_;


};

