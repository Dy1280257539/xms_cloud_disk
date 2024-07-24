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
    //��Ҫͨ�����ػ�ȡ�ϴ���΢�����б�
    //���һ��΢�����ϴ������б��΢���� FileServerListService
    auto fm = FileManager::Get();
    XFileInfo file;
    string upload_ip = "127.0.0.1";
    int upload_port = UPLOAD_PORT;
    while (!is_exit_)
    {
        for (auto c = upload_clients.begin(); c != upload_clients.end();)
        {
            //һ��������쳣�رգ�һ������������ر�
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
        //��������
        auto client = new XUploadClient();

        //�����Զ�������ʧ�ܾ͹ر����¿�ʼ
        client->set_auto_connect(false);
        client->set_auto_delete(false);
        client->set_server_ip(upload_ip.c_str());
        client->set_server_port(upload_port);
        if (!client->LoadFile(file))
        {
            cout << "client->LoadFile failed!" << endl;
            //����һ��������Ϣ
            delete client;
            continue;
        }
        client->StartConnect();
        upload_clients.push_back(client);
    }

    for (auto c : upload_clients)
    {
        //һ��������쳣�رգ�һ������������ر�
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
