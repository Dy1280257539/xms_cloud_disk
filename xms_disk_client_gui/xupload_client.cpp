#include "xupload_client.h"
#include"xmsg_com.pb.h"
#include "xms_disk_client_gui.pb.h"
#include "xfile_manager.h"
#include "xtools.h"
using namespace std;
using namespace xmsg;
using namespace xdisk;
//�ӽ�100Mÿ�����񻺴� 100m �������ʱ��������
// ÿ��100����Ϣ�ȴ���Ӧ����������100ms
// ��ʱ������ �ٶȣ�����Ҫ������һ�����ݿ��б�
#define FILE_SLICE_BYTE 10000000 
//#define FILE_SLICE_BYTE 10000 
bool XUploadClient::set_file(xdisk::XFileInfo file)
{
    this->file_ = file;
    ifs_.open(file_.local_path(), ios::binary);
    if (!ifs_.is_open())
    {
        return false;
    }

    int filesize = 0;
    // ��ȡmd5 ÿ��FILE_SLICE_BYTE ����һ��Ƭmd5
    // ���е�Ƭmd5��������һ���ļ�md5
    // һ��ʼ������md5��Ŀ����Ϊ�˺�����봫

    char md5_base64[17] = { 0 };
    string all_md5_base64 = "";
    while (!ifs_.eof())
    {
        ifs_.read(slice_buf_, FILE_SLICE_BYTE);
        int size = ifs_.gcount();
        filesize += size;

        string md5_base64 = XMD5_base64((unsigned char*)slice_buf_, size);
        md5_base64s_.push_back(md5_base64);
        all_md5_base64 += md5_base64;
        //cout << "[" << md5_base64 << "]" << flush;
    }
    if (filesize == 0)
    {
        ifs_.close();
        return false;
    }
    //�����ļ���md5 ���ܺ�У�飬��ͨ�ļ�һ��ʼ�����ɣ������ļ��������봫
    string file_md5 = XMD5_base64((unsigned char*)all_md5_base64.c_str(), all_md5_base64.size());
    file_.set_md5(file_md5);

    file_.set_filesize(filesize);

    //����Ǽ����ļ� �ļ���С����16�ı���
    if (file_.is_enc())
    {

        int dec_size = 0;
        if (filesize % 16 != 0)
        {
            dec_size = filesize  + (16 - filesize % 16);
        }
        file_.set_filesize(dec_size);
        file_.set_ori_size(filesize);
    }
    ifs_.clear();
    ifs_.seekg(0, ios_base::beg);
    cout << file_.DebugString();

    //int size = ifs_.tellg ();

    if (file_.is_enc())
    {
        auto pass = file_.password();
        if (pass.empty())
        {
            LOGERROR("please set  password");
            return false;
        }
        if (!aes_)
        {
            aes_ = XAES::Create();
        }
        aes_->SetKey(pass.c_str(), pass.size(), true);
    }
    return true;
}

void XUploadClient::ConnectedCB()
{
    auto file = file_;
    file.clear_password(); //��Ҫ��������
    SendMsg((MsgType)UPLOAD_FILE_REQ, &file);
    cout << "XUploadClient::Connect()" << endl;
}

void XUploadClient::SendSlice()
{
    if (ifs_.eof())
    {
        SendMsg((MsgType)UPLOAD_FILE_END_REQ, &file_);
        //�ļ�����β�����ͽ���
        return;
    }

    int size = FILE_SLICE_BYTE;
    if (size > file_.filesize())
        size = file_.filesize();

    //��ǰ�ļ���ƫ��λ��
    long long offset = ifs_.tellg();
    XMsgHead head;
    head.set_msg_type((MsgType)SEND_SLICE_REQ);
    head.set_offset(offset);


    ifs_.read(slice_buf_, FILE_SLICE_BYTE);
    size = ifs_.gcount();


    XMsg data;
    data.data = slice_buf_;
    data.size = size;

    //�����Ҫ����
    if (file_.is_enc())
    {
        long long enc_size = 0;

        if (!aes_)
        {
            LOGERROR("aes not init!");
            return;
        }
        enc_size = aes_->Encrypt((unsigned char*)slice_buf_, size, (unsigned char*)slice_buf_enc_);
        data.data = slice_buf_enc_;
        data.size = enc_size;
        string md5_base64 = XMD5_base64((unsigned char*)slice_buf_enc_, enc_size);
        head.set_md5(md5_base64);
    }
    else
    {
        //δ����
        if (!md5_base64s_.empty())
        {
            head.set_md5(md5_base64s_.front());
            md5_base64s_.pop_front();
        }
    }

    XFileInfo *info = new XFileInfo();
    info->CopyFrom(file_);
    SendMsg(&head, &data);
}

void XUploadClient::UploadFileRes(xmsg::XMsgHead *head, XMsg *msg)
{
    cout << "UploadFileRes 1 " << endl;
    //��ʼ��������ʱ���Ѿ����͵�ֵ��Ҫȷ�������Ѿ������ͳɹ�
    //����Э�飬���յ��������ķ���������϶��ѷ������
    begin_send_data_size_ = send_data_size();
    //��ʼ�����ļ�
    SendSlice();
}

void XUploadClient::SendSliceRes(xmsg::XMsgHead *head, XMsg *msg)
{
    cout << "SendSliceRes 2 " << endl;
    SendSlice();
}

void XUploadClient::UploadFileEndRes(xmsg::XMsgHead *head, XMsg *msg)
{
    cout << "UploadFileEndRes 3" << endl;
    XFileManager::Instance()->UploadEnd(task_id);
    XFileManager::Instance()->RefreshDir();
    
    //�������ˢ�½���
    ClearTimer();
    Close();
    DropInMsg();
}

//ͨ����ʱ�����ٽ���
void XUploadClient::TimerCB()
{
    if (begin_send_data_size_ < 0)
        return;
    auto size = BufferSize();
    
    
    //�ѷ��͵����� ����ȫ׼ȷ������ȷ�ϵ����ݰ�����
    long long sended = send_data_size() - begin_send_data_size_ - BufferSize();

    cout << sended <<":"<< file_.filesize() << endl;

    //������ݹ�������С
    XFileManager::Instance()->UploadProcess(task_id, sended);
}

XUploadClient::XUploadClient()
{
    slice_buf_ = new char[FILE_SLICE_BYTE];

    //����16�ı���Ҫ��ȫ������Ҫ��Ԥ���ռ�
    slice_buf_enc_ = new char[FILE_SLICE_BYTE+16];
    //ͨ����ʱ�����ٽ���
    set_timer_ms(100);
}


XUploadClient::~XUploadClient()
{
    delete slice_buf_;
    slice_buf_ = NULL;

    delete slice_buf_enc_;
    slice_buf_enc_ = NULL;
    if (aes_)
    {
        aes_->Drop();
        aes_ = NULL;
    }
}
