#pragma once

#ifdef _WIN32
#ifdef XCOM_EXPORTS
#define XCOM_API __declspec(dllexport)
#else
#define XCOM_API __declspec(dllimport)
#endif
#else
#define XCOM_API
#endif

class XCOM_API XSSL
{
public:
    XSSL();
    ~XSSL();

    //�ն���
    bool IsEmpty() { return ssl_ == 0; }

    //����˽���ssl����
    bool Accept();

    //�ͻ��˴���ssl����
    bool Connect();

    //��ӡͨ��ʹ�õ��㷨
    void PrintCipher();

    //��ӡ�Է�֤����Ϣ
    void PrintCert();

    int Write(const void *data, int data_size);

    int Read(void *buf, int buf_size);

    void set_ssl(struct ssl_st *ssl) { this->ssl_ = ssl; }

    //�ͷ�SSL
    void Close();

    struct ssl_st *ssl() { return ssl_; }
private:
    struct ssl_st *ssl_ = 0;
};

