#include "xfile_manager.h"
#include "xtools.h"
using namespace std;
using namespace xdisk;
XFileManager *XFileManager::instance_ = 0;


xmsg::XServiceList XFileManager::upload_servers()
{
    XMUTEX(&servers_mutex_);
    return upload_servers_;
}
xmsg::XServiceList XFileManager::download_servers()
{
    XMUTEX(&servers_mutex_);
    return download_servers_;
}

void XFileManager::set_upload_servers(xmsg::XServiceList servers)
{
    XMUTEX(&servers_mutex_);
    this->upload_servers_ = servers;

}
void XFileManager::set_download_servers(xmsg::XServiceList servers)
{
    XMUTEX(&servers_mutex_);
    this->download_servers_ = servers;
}
int XFileManager::AddUploadTask(xdisk::XFileInfo file)
{
    XFileTask task;
    auto filetask = new XFileInfo();
    filetask->CopyFrom(file);
    //ֻ���ö�̬����Ŀռ䣬����task ���ü���Ϊ0����ʱdelete
    task.set_allocated_file(filetask);
    static int task_id = 0;
    task_id++;
    task.set_index(task_id);
    task.set_tasktime(XGetTime());

    {
        XMutex mutex(&uploads_mutex_);
        uploads_.push_back(task);
    }
    UploadProcess(task_id, 0);
    return task_id;
}
int XFileManager::AddDownloadTask(xdisk::XFileInfo file)
{
    XFileTask task;
    auto filetask = new XFileInfo();
    filetask->CopyFrom(file);
    //ֻ���ö�̬����Ŀռ䣬����task ���ü���Ϊ0����ʱdelete
    task.set_allocated_file(filetask);
    static int task_id = 0;
    task_id++;
    task.set_index(task_id);
    task.set_tasktime(XGetTime());

    {
        XMutex mutex(&downloads_mutex_);
        downloads_.push_back(task);
    }
    DownloadProcess(task_id, 0);
    return task_id;
}
void XFileManager::UploadEnd(int task_id)
{
    //uploads_mutex_.lock();
    XMutex mutex(&uploads_mutex_);
    for (auto up = uploads_.begin(); up != uploads_.end(); up++)
    {
        if (task_id == up->index())
        {
            up->set_is_complete(true);
            up->mutable_file()->set_net_size(up->file().filesize());
        }
    }
    //uploads_mutex_.unlock();
    RefreshUploadTask(uploads_);
}

//���ȴ�
void XFileManager::UploadProcess(int task_id, int sended)
{
    XMutex mutex(&uploads_mutex_);
    //uploads_mutex_.lock();
    for (auto up = uploads_.begin(); up != uploads_.end(); up++)
    {
        if (task_id == up->index())
        {
            up->mutable_file()->set_net_size(sended);
        }
    }
    //uploads_mutex_.unlock();
    RefreshUploadTask(uploads_);
}

void XFileManager::DownloadEnd(int task_id)
{
    XMutex mutex(&downloads_mutex_);
    for (auto up = downloads_.begin(); up != downloads_.end(); up++)
    {
        if (task_id == up->index())
        {
            up->set_is_complete(true);
            up->mutable_file()->set_net_size(up->file().filesize());
        }
    }
    //uploads_mutex_.unlock();
    RefreshDownloadTask(downloads_);
}


void XFileManager::DownloadProcess(int task_id, int recved)
{
    XMutex mutex(&downloads_mutex_);
    //uploads_mutex_.lock();
    for (auto down = downloads_.begin(); down != downloads_.end(); down++)
    {
        if (task_id == down->index())
        {
            down->mutable_file()->set_net_size(recved);
        }
    }
    //uploads_mutex_.unlock();
    RefreshDownloadTask(downloads_);
}

