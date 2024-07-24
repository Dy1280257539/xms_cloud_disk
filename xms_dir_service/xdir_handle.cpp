#include "xdir_handle.h"
#include "xtools.h"
#include "xlog_client.h"
#ifdef _WIN32
#define DIR_ROOT "./server_root/"
#else
#define DIR_ROOT "/mnt/xms/"
#endif
#define FILE_INFO_NAME_PRE ".info_"
//10G
#define USER_SPACE 1073741824
using namespace xdisk;
using namespace std;
using namespace xmsg;

static string GetUserPath(const xmsg::XMsgHead *head)
{
    if (!head)
        return "";
    string path = DIR_ROOT;
    path += head->username();
    path += "/";
    return path;
}

void XDirHandle::GetDiskInfoReq(xmsg::XMsgHead *head, XMsg *msg)
{
    string path = GetUserPath(head);
    XDiskInfo res;
    unsigned long long avail = 0;
    unsigned long long total = 0;
    unsigned long long free = 0;

    //�˲���������Դ������Ҫ�Ż�
    long long dir_size = GetDirSize(path.c_str());
    res.set_dir_size(dir_size);

    //�����root�û�������ȫ���ռ�
    if (head->username() == "root")
    {
        GetDiskSize(path.c_str(), &avail, &total, &free);
        res.set_avail(avail);
        res.set_free(free);
        res.set_total(total);
    }
    else
    {
        //�û��Ŀռ䣬�������ļ��ж�ȡ�����ߴ��û�����Ϣ�ж�ȡ��Ĭ��10G
        long long user_size = USER_SPACE;
        res.set_avail(user_size - dir_size);
        res.set_free(user_size - dir_size);
        res.set_total(user_size);
    }

    head->set_msg_type((xmsg::MsgType)xdisk::GET_DISK_INFO_RES);
    SendMsg(head, &res);
    //XG req;
    //if (!req.ParseFromArray(msg->data, msg->size))
    //{
    //    LOGDEBUG("XDirHandle::GetDiskInfoReq failed!");
    //    return;
    //}
    //cout<<req.DebugString();
}

void XDirHandle::NewDirReq(xmsg::XMsgHead *head, XMsg *msg)
{
    XGetDirReq req;
    if (!req.ParseFromArray(msg->data, msg->size))
    {
        LOGDEBUG("XDirHandle::NewDirReq failed!");
        return;
    }

    string path = GetUserPath(head);
    path += req.root();
    path += "/";

    //����Ŀ¼
    XNewDir(path);
    XMessageRes res;
    res.set_return_(XMessageRes::OK);
    res.set_msg("OK");
    head->set_msg_type((MsgType)NEW_DIR_RES);
    SendMsg(head, &res);
}
void XDirHandle::GetDirReq(xmsg::XMsgHead *head, XMsg *msg)
{
    // ��Ŀ¼ + �û��� + ���Ŀ¼
    XGetDirReq req;
    if (!req.ParseFromArray(msg->data, msg->size))
    {
        LOGDEBUG("XDirHandle::GetDirReq failed!");
        return;
    }
    cout << req.DebugString();
    string path = GetUserPath(head);
    path += req.root();
    cout << "GetDirReq path = " << path << endl;
    auto files = GetDirList(path);
    XFileInfoList file_list;

    for (auto file : files)
    {
        if (file.filename == "." || file.filename == "..")
            continue;
        //�ļ���Ϣ�ļ�
        string pre = file.filename.substr(0, strlen(FILE_INFO_NAME_PRE));
        if (pre == FILE_INFO_NAME_PRE)
            continue;
        auto info = file_list.add_files();
        info->set_filename(file.filename);
        info->set_filesize(file.filesize);
        info->set_filetime(file.time_str);
        info->set_is_dir(file.is_dir);
    }
    head->set_msg_type((xmsg::MsgType)xdisk::GET_DIR_RES);
    SendMsg(head, &file_list);
}
void XDirHandle::DeleteFileReq(xmsg::XMsgHead *head, XMsg *msg)
{
    //DELETE_FILE_RES = 1015;			//XMessageRes
    XFileInfo req;
    if (!req.ParseFromArray(msg->data, msg->size))
    {
        LOGDEBUG("DeleteFileReq ParseFromArray failed!");
        return;
    }
    string path = GetUserPath(head);
    path += req.filedir();
    path += "/";

    string info_path = path;

    path += req.filename();
    info_path += FILE_INFO_NAME_PRE;
    info_path += req.filename();
    //ɾ���ļ� ������봫�ļ���Ҫɾ�����ô���
    XDelFile(path);

    //ɾ����Ϣ�ļ�
    XDelFile(info_path);
    //�ж��Ƿ�ɾ���ɹ��������Ƿ��д��ļ�
    head->set_msg_type((xmsg::MsgType)xdisk::DELETE_FILE_RES);
    XMessageRes res;
    res.set_return_(XMessageRes::OK);
    res.set_msg("OK");
    SendMsg(head, &res);
}

//void XDirHandle::ChangeDirReq(xmsg::XMsgHead *head, XMsg *msg)
//{
//
//}

XDirHandle::XDirHandle()
{
}

XDirHandle::~XDirHandle()
{
}
