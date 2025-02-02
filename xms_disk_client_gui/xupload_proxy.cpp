#include "xupload_proxy.h"
#include "file_manager.h"
#include "xupload_client.h"
#include <thread>
#include <vector>
using namespace std;
using namespace xdisk;
static list<XUploadClient*> upload_clients;
void XUploadProxy::Main()
{
    //需要通过网关获取上传的微服务列表
    //添加一个微服务上传服务列表的微服务 FileServerListService
    auto fm = FileManager::Get();
    XFileInfo file;
    string upload_ip = "127.0.0.1";
    int upload_port = UPLOAD_PORT;
    while (!is_exit_)
    {
        for (auto c = upload_clients.begin(); c != upload_clients.end();)
        {
            //一种情况是异常关闭，一种情况是正常关闭
            if ((*c)->is_closed())
            {
                bool has_error = (*c)->has_error();
                string error = (*c)->error();
                auto tmp = c;
                c++;
                (*tmp)->set_auto_delete(true);
                (*tmp)->Close();
                upload_clients.erase(tmp);
            }
            else
            {
                c++;
            }
        }

        if (!fm->GetTask(UPLOAD_FILE_REQ, &file))
        {
            this_thread::sleep_for(10ms);
            continue;
        }
        //处理任务
        auto client = new XUploadClient();

        //不用自动重连，失败就关闭重新开始
        client->set_auto_connect(false);
        client->set_auto_delete(false);
        client->set_server_ip(upload_ip.c_str());
        client->set_server_port(upload_port);
        if (!client->LoadFile(file))
        {
            cout << "client->LoadFile failed!" << endl;
            //返回一个错误消息
            delete client;
            continue;
        }
        client->StartConnect();
        upload_clients.push_back(client);
    }

    for (auto c : upload_clients)
    {
        //一种情况是异常关闭，一种情况是正常关闭
        c->set_auto_delete(true);
        c->Close();
    }
    upload_clients.clear();
}
void XUploadProxy::Start()
{
    XUploadClient::RegMsgCallback();
    thread th(&XUploadProxy::Main, this);
    th.detach();
}

XUploadProxy::~XUploadProxy()
{
    is_exit_ = true;
    this_thread::sleep_for(100ms);
}
