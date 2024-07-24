#pragma once

#include <QObject>
#include "xms_disk_client_gui.pb.h"
#include "xmsg_com.pb.h"
//struct XFileData
//{
//    std::string filename;
//    long long filesize;
//    std::string filetime;
//};
class XFileManager:public QObject
{

    Q_OBJECT

public:

    static XFileManager * Instance() { return instance_; }

    //��ȡĿ¼
    virtual void GetDir(std::string root) = 0;

    ///��ʼ�ϴ��ļ�
    ///@ file_local_path �����ļ�����ȫ·��
   // void UploadFile(std::string filename, std::string file_local_path, std::string remote_dir);

    void RefreshDir() { GetDir(root_); }

    virtual ~XFileManager() {};

    virtual void InitFileManager(std::string server_ip, int server_port) = 0;

    ///��ʼ�ϴ��ļ�
    ///@ file_local_path �����ļ�����ȫ·��
    virtual void UploadFile(xdisk::XFileInfo file) = 0;

    //�����ļ����ܵ���Կ
    virtual void set_password(std::string pass) { password_ = pass; };
    virtual std::string  password() { return password_ ; };

    //���ȴ�0~1000
    //�����ϴ��б���� �̰߳�ȫ
    virtual void UploadProcess(int task_id,int sended);
    virtual void UploadEnd(int task_id);
    


    ///��ʼ�ϴ��ļ�
    ///@ file_local_path �����ļ�����ȫ·��
    virtual void DownloadFile(xdisk::XFileInfo file) = 0;
    virtual void DownloadProcess(int task_id, int recved);
    virtual void DownloadEnd(int task_id);

    virtual void DeleteFile(xdisk::XFileInfo file) = 0;

    virtual void NewDir(std::string path) = 0;
    //���ȴ�0~1000
    //�����ϴ��б���� �̰߳�ȫ
    //virtual void DownloadProcess(int task_id, int sended);

    //��ȡ�ϴ������б��̰߳�ȫ
    //std::list<xdisk::XFileTask> uploads();

    //��������ID
    int AddUploadTask(xdisk::XFileInfo file );
    int AddDownloadTask(xdisk::XFileInfo file);
    virtual void set_login(xmsg::XLoginRes login) { this->login_ = login; }
    xmsg::XLoginRes login() { return login_; }
    
    xmsg::XServiceList upload_servers();
    xmsg::XServiceList download_servers();

    void set_upload_servers(xmsg::XServiceList servers);
    void set_download_servers(xmsg::XServiceList servers);

signals:
    //ˢ��Ŀ¼��ʾ
    void RefreshData(xdisk::XFileInfoList file_list, std::string cur_dir);
    void RefreshUploadTask(std::list<xdisk::XFileTask> file_list);
    void RefreshDownloadTask(std::list<xdisk::XFileTask> file_list);
    void RefreshDiskInfo(xdisk::XDiskInfo info);
    void ErrorSig(std::string str);
protected:
    XFileManager() {};
    //virtual bool CreateDir() = 0;
    //virtual bool CreateUpload() = 0;
    static XFileManager *instance_;
    std::string root_ = "";

    std::list<xdisk::XFileTask> uploads_;
    std::mutex uploads_mutex_;
    std::list<xdisk::XFileTask> downloads_;
    std::mutex downloads_mutex_;
    xmsg::XLoginRes login_;

    //�ϴ��ķ������б�
    std::mutex servers_mutex_;
    xmsg::XServiceList upload_servers_;

    xmsg::XServiceList download_servers_;
    std::string password_ = "";


};

