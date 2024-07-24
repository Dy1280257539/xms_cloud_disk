#include "xupload_handle.h"
#include "xtools.h"
#include "xlog_client.h"
using namespace xmsg;
using namespace xdisk;
using namespace std;
#ifdef _WIN32
#define DIR_ROOT "./server_root/"
#else
#define DIR_ROOT "/mnt/xms/"
#endif
#define FILE_INFO_NAME_PRE ".info_"
void XUploadHandle::UploadFileReq(xmsg::XMsgHead *head, XMsg *msg)
{
    //��֤�û�Ȩ��
    //���յ��ļ�����
    if (!cur_file_.ParseFromArray(msg->data, msg->size))
    {
        LOGDEBUG("UploadFileReq ParseFromArray failed!");
        return;
    }

    XMessageRes res;
    string path = DIR_ROOT;
    path += head->username();
    path += "/";
    path += cur_file_.filedir();
    save_dir_ = path;
    path += "/";
    cout << "UploadFileReq path = " << path << endl;
    //����Ŀ¼
    XNewDir(path);

    path += cur_file_.filename();

    ofs_.open(path, ios::binary);

    //��ҪУ��Ȩ��
    res.set_return_(XMessageRes::OK);
    res.set_msg("OK");
    if (!ofs_.is_open())
    {
        stringstream ss;
        ss << "UploadFileReq open file failed!" << path;
        res.set_return_(XMessageRes::ERROR);

        res.set_msg(ss.str());
        LOGINFO(ss.str().c_str())
    }

    //Ŀ¼�޸�λʵ��Ŀ¼ ��Ҫ��ӹ������� �ϴ�Ŀ¼

    //res.set_return_(XMessageRes::ERROR);

    head->set_msg_type((MsgType)UPLOAD_FILE_RES);
    SendMsg(head, &res);
}

void XUploadHandle::SendSliceReq(xmsg::XMsgHead *head, XMsg *msg)
{
    head->set_msg_type((MsgType)SEND_SLICE_RES);
    XMessageRes res;
    if (head->md5().empty())
    {
        cout << "XUploadHandle::SendSliceReq failed! md5 is empty!";

        //��ҪУ��Ȩ��
        res.set_return_(XMessageRes::ERROR);
        res.set_msg("md5 is empty");
        SendMsg(head, &res);
        return;
    }
    //У��md5
    string md5 = XMD5_base64((unsigned char *)msg->data, msg->size);
    if (head->md5() != md5)
    {
        cout << "XUploadHandle::SendSliceReq failed! md5 is error!";
        res.set_return_(XMessageRes::ERROR);
        res.set_msg("md5 is error");
        return;
    }

    ofs_.write(msg->data, msg->size);

    //��ҪУ��Ȩ��
    res.set_return_(XMessageRes::OK);
    res.set_msg("OK");
    SendMsg(head, &res);
}

void XUploadHandle::UploadFileEndReq(xmsg::XMsgHead *head, XMsg *msg)
{
    ofs_.close();
    //�ļ���Ϣ�洢 .filename.info
    string info_path = save_dir_;
    info_path += FILE_INFO_NAME_PRE;
    info_path += cur_file_.filename();
    ofstream ofs;
    ofs.open(info_path, ios::binary);
    if (ofs)
    {
        cur_file_.SerializeToOstream(&ofs);
        ofs.close();
    }

    //��֤�ļ�md5 ��֤�Ƿ���ȷ
    head->set_msg_type((MsgType)UPLOAD_FILE_END_RES);
    XMessageRes res;
    //��ҪУ��Ȩ��
    res.set_return_(XMessageRes::OK);
    res.set_msg("OK");
    SendMsg(head, &res);
}