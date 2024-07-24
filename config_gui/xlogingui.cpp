#include "xlogingui.h"
#include <QMessageBox>
#include <string>
#include "xauth_client.h"
using namespace std;
XLoginGUI::XLoginGUI(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
}

XLoginGUI::~XLoginGUI()
{

}

void XLoginGUI::Login()
{
    if (ui.usernameEdit->text().isEmpty() || ui.passwordEdit->text().isEmpty())
    {
        QMessageBox::information(this, "", QString::fromLocal8Bit("用户名或者密码不能为空"));
        return;
    }
    string username = ui.usernameEdit->text().toStdString();
    string password = ui.passwordEdit->text().toStdString();

    XAuthClient::Get()->LoginReq(username, password);
    xmsg::XLoginRes login;
    bool re = XAuthClient::Get()->GetLoginInfo(username, &login,1000);
    if (!re)
    {
        QMessageBox::information(this, "", QString::fromLocal8Bit("用户名或密码错误"));
        return;
    }
    cout << "Login success!" << endl;
    accept();
}