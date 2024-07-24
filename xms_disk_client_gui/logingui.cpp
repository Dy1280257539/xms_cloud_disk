#include "logingui.h"
#include <QMouseEvent>
#include "xauth_client.h"
#include <string>
using namespace std;
LoginGUI::LoginGUI(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    ////ȥ��ԭ���ڱ߿�
    setWindowFlags(Qt::FramelessWindowHint);

    ////���ر���������Բ��
    setAttribute(Qt::WA_TranslucentBackground);
    ui.err_frame->hide();


}

LoginGUI::~LoginGUI()
{
}

void LoginGUI::Login()
{
    ui.err_frame->show();
    string username = ui.usernameEdit->text().toLocal8Bit();
    string password = ui.passwordEdit->text().toLocal8Bit();
    if (username.empty() || password.empty())
    {
        ui.err_msg->setText(QString::fromLocal8Bit("�û��������벻��Ϊ�գ�"));
        return;
    }

    if (!XAuthClient::Get()->Login(username, password))
    {

        ui.err_msg->setText(QString::fromLocal8Bit("�û���������������"));
        return;
    }
    static int count = 0;
    count++;
    
    ui.err_msg->setText(QString::number(count)+QString::fromLocal8Bit("��¼�ɹ���"));
    QDialog::accept();
}


static bool mouse_press = false;
static QPoint mouse_point;
void LoginGUI::mouseMoveEvent(QMouseEvent *ev)
{
    //û�а��£�����ԭ�¼�
    if (!mouse_press)
    {
        QWidget::mouseMoveEvent(ev);
        return;
    }
    auto cur_pos = ev->globalPos();
    this->move(cur_pos - mouse_point);
}
void LoginGUI::mousePressEvent(QMouseEvent *ev)
{
    //���������¼�¼λ��
    if (ev->button() == Qt::LeftButton)
    {
        mouse_press = true;
        mouse_point = ev->pos();
    }

}
void LoginGUI::mouseReleaseEvent(QMouseEvent *ev)
{
    mouse_press = false;
}