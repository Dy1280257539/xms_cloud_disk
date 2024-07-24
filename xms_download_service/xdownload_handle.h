#pragma once
#include "xservice_handle.h"
#include "xms_disk_client_gui.pb.h"
#include <list>
#include <fstream>
//ÿ������ֻ����һ���ļ�
class XDownloadHandle:public XServiceHandle
{
public:
    XDownloadHandle();
    ~XDownloadHandle();

    void DownloadFileReq(xmsg::XMsgHead *head, XMsg *msg);
    void DownloadFileBegin(xmsg::XMsgHead *head, XMsg *msg);
    void DownloadSliceRes(xmsg::XMsgHead *head, XMsg *msg);

    static void RegMsgCallback()
    {
        RegCB((xmsg::MsgType)xdisk::DOWNLOAD_FILE_REQ, (MsgCBFunc)&XDownloadHandle::DownloadFileReq);
        RegCB((xmsg::MsgType)xdisk::DOWNLOAD_FILE_BEGTIN, (MsgCBFunc)&XDownloadHandle::DownloadFileBegin);
        RegCB((xmsg::MsgType)xdisk::DOWNLOAD_SLICE_RES, (MsgCBFunc)&XDownloadHandle::DownloadSliceRes);
    }
private:
    void SendSlice();
    xdisk::XFileInfo file_;         //��ǰ���յ��ļ�
    xdisk::XFileSlice cur_slice_;       //��ǰ���յ��ļ�Ƭ��Ϣ
    std::list<XMsg> cur_data_;          //��ǰ���յ��ļ�Ƭ����
    std::list<XMsg> caches_;            //�����ļ��Ļ���
    std::ifstream ifs_;                 //д�뱾���ļ�
    int filesize_ = 0;                  //�ļ���С
    int sendsize_ = 0;                  //�Ѿ����͵��ļ���С
    char *slice_buf_ = 0;
};

