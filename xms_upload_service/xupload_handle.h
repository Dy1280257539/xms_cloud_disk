#pragma once
#include "xservice_handle.h"
#include "xms_disk_client_gui.pb.h"
#include <list>
#include <fstream>
#include "xtools.h"
//ÿ������ֻ����һ���ļ�
class XUploadHandle:public XServiceHandle
{
public:
    XUploadHandle() 
    {
        //�趨��ʱ�����ڻ�ȡ���ͽ���
        set_timer_ms(100);
    };
    virtual ~XUploadHandle()
    {
        if (aes_)
            delete aes_;
        aes_ = 0;
    }

    void UploadFileReq(xmsg::XMsgHead *head, XMsg *msg);
    void SendSliceReq(xmsg::XMsgHead *head, XMsg *msg);
    void UploadFileEndReq(xmsg::XMsgHead *head, XMsg *msg);

    static void RegMsgCallback()
    {
        RegCB((xmsg::MsgType)xdisk::UPLOAD_FILE_REQ, (MsgCBFunc)&XUploadHandle::UploadFileReq);
        RegCB((xmsg::MsgType)xdisk::SEND_SLICE_REQ, (MsgCBFunc)&XUploadHandle::SendSliceReq);
        
        RegCB((xmsg::MsgType)xdisk::UPLOAD_FILE_END_REQ, (MsgCBFunc)&XUploadHandle::UploadFileEndReq);
    }
private:
    std::string save_dir_ = "";
    xdisk::XFileInfo cur_file_;         //��ǰ���յ��ļ�
    xdisk::XFileSlice cur_slice_;       //��ǰ���յ��ļ�Ƭ��Ϣ
    std::list<XMsg> cur_data_;          //��ǰ���յ��ļ�Ƭ����
    std::list<XMsg> caches_;            //�����ļ��Ļ���
    std::ofstream ofs_;                 //д�뱾���ļ�
    int filesize_ = 0;                  //�ļ���С
    int sendsize_ = 0;                  //�Ѿ����͵��ļ���С
    XAES *aes_ = 0; //�����ļ�
};

