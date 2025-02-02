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

    //空对象
    bool IsEmpty() { return ssl_ == 0; }

    //服务端接收ssl连接
    bool Accept();

    //客户端处理ssl握手
    bool Connect();

    //打印通信使用的算法
    void PrintCipher();

    //打印对方证书信息
    void PrintCert();

    int Write(const void *data, int data_size);

    int Read(void *buf, int buf_size);

    void set_ssl(struct ssl_st *ssl) { this->ssl_ = ssl; }

    //释放SSL
    void Close();

    struct ssl_st *ssl() { return ssl_; }
private:
    struct ssl_st *ssl_ = 0;
};

