#include "xmsdiskclientgui.h"
#include <QMouseEvent>
#include <QMenu>
#include <QHBoxLayout>
#include <sstream>
#include <list>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QLineEdit>
#include "filepassword.h"
//#include "file_manager.h"
#include "task_list_gui.h"
#include "xtools.h"
using namespace std;
using namespace xdisk;

static list<XFileInfo> file_list;
static list<QCheckBox *> check_list;
static TaskListGUI *task_gui = 0;

#define FILE_ICON_PATH ":/XMSDiskClientGui/Resources/img/FileType/Small/"
XMSDiskClientGui::~XMSDiskClientGui()
{
    
}
void XMSDiskClientGui::ErrorSlot(std::string err)
{
    QMessageBox::information(this, "XMS ERROR", QString::fromLocal8Bit(err.c_str()));
}
XMSDiskClientGui::XMSDiskClientGui(XFileManager *xfm, QWidget *parent)
    : QWidget(parent)
{    
    ui.setupUi(this);
    set_xfm(xfm);

    ////ȥ��ԭ���ڱ߿�
    setWindowFlags(Qt::FramelessWindowHint);

    ////���ر���������Բ��
    setAttribute(Qt::WA_TranslucentBackground);

    setMouseTracking(true);
    auto head = ui.filetableWidget->horizontalHeader();
    head->setDefaultAlignment(Qt::AlignLeft);
    auto tab = ui.filetableWidget;
    tab->setColumnWidth(0, 40);     //checkall
    tab->setColumnWidth(1, 500);    //filename
    tab->setColumnWidth(2, 150);    //time
    tab->setColumnWidth(3, 100);    //size
    auto hitem = tab->horizontalHeaderItem(0);
    qRegisterMetaType<std::list<XFileInfo>>("std::list<FileInfo>");
    qRegisterMetaType< std::string>("std::string");
    qRegisterMetaType<xdisk::XFileInfoList>("xdisk::XFileInfoList");
    

    
    //xfm_ = XFileManager::Instance();
    //connect(FileManager::Get(), SIGNAL(RefreshData(xdisk::XFileInfoList)), this, SLOT(RefreshData(xdisk::XFileInfoList)));
    connect(this->xfm_, SIGNAL(RefreshData(xdisk::XFileInfoList, std::string)), this, SLOT(RefreshData(xdisk::XFileInfoList, std::string)));
    //this->xfm_->GetDir("/");
    while (tab->rowCount() > 0)
    {
        tab->removeRow(0);
    }

    task_gui = new TaskListGUI(this);
    task_gui->hide();

    qRegisterMetaType<std::list<xdisk::XFileTask>>("std::list<xdisk::XFileTask>");
    connect(this->xfm_, SIGNAL(RefreshUploadTask(std::list<xdisk::XFileTask>)), task_gui, SLOT(RefreshUploadTask(std::list<xdisk::XFileTask>)));
    connect(this->xfm_, SIGNAL(RefreshDownloadTask(std::list<xdisk::XFileTask>)), task_gui, SLOT(RefreshDownloadTask(std::list<xdisk::XFileTask>)));

    qRegisterMetaType<xdisk::XDiskInfo>("xdisk::XDiskInfo");
    connect(this->xfm_, SIGNAL(RefreshDiskInfo(xdisk::XDiskInfo)), this, SLOT(RefreshDiskInfo(xdisk::XDiskInfo)));
    
    connect(this->xfm_, SIGNAL(ErrorSig(std::string)), this, SLOT(ErrorSlot(std::string)));

    
    //ֻ�ܻ�ȡ�û�Ŀ¼��
    //FileManager::Get()->GetDir("/");

    Refresh();

    //��ʾ�û���
    ui.username_label->setText(xfm_->login().username().c_str());
    
    //ui.username_label->set_text(xfm_->login()->username().c_str());
    
    //TaskTab();
    return;

}

void XMSDiskClientGui::RefreshDiskInfo(xdisk::XDiskInfo info)
{
    
    string size_str = XGetSizeString(info.dir_size());
    size_str += "/";
    size_str += XGetSizeString(info.total());
    ui.disk_info_text->setText(size_str.c_str());
    ui.disk_info_bar->setMaximum(info.total());
    ui.disk_info_bar->setValue(info.dir_size());
}
//�ļ��Ƿ�����ϴ�
void XMSDiskClientGui::FileEnc()
{
    if (ui.file_enc->isChecked())
    {
        FilePassword pass_dia;
        if (pass_dia.exec() == QDialog::Accepted)
        {
            this->xfm_->set_password(pass_dia.password);
        }
    }
    else
    {
        this->xfm_->set_password("");
    }
}
void XMSDiskClientGui::MyTab()
{
    if (!task_gui)return;
    ui.filelistwidget->show();
    task_gui->hide();
}

void XMSDiskClientGui::TaskTab()
{
    task_gui->move(ui.filelistwidget->pos().x(), ui.filelistwidget->pos().y());
    task_gui->resize(size());
    //task_gui->
    ui.filelistwidget->hide();

    task_gui->Show();
}
void XMSDiskClientGui::DoubleClicked(int row, int col)
{
    //˫��������Ҫ����Ԥ��ͼƬ����Ƶ
    auto item = ui.filetableWidget->item(row, 1);
    QString dir = item->text();
    string filename = dir.toLocal8Bit();
    //remote_dir_ += filename;
    this->xfm_->GetDir(remote_dir_+"/"+filename);
    qDebug() << item;
}

void XMSDiskClientGui::NewDir()
{
    QDialog dialog;

    ////ȥ��ԭ���ڱ߿�
    dialog.setWindowFlags(Qt::FramelessWindowHint);

    ////���ر���������Բ��
    dialog.setAttribute(Qt::WA_TranslucentBackground);
    dialog.resize(400, 50);
    QLineEdit edit(&dialog);
    edit.resize(300, 40);
    QPushButton ok(&dialog);
    ok.move(305, 0);
    ok.setText(QString::fromLocal8Bit("ȷ��"));
    QPushButton cancel(&dialog);
    cancel.move(305, 22);
    cancel.setText(QString::fromLocal8Bit("ȡ��"));
    connect(&cancel, SIGNAL(clicked()), &dialog, SLOT(reject()));
    connect(&ok, SIGNAL(clicked()), &dialog, SLOT(accept()));
    auto re = dialog.exec();
    if (re == QDialog::Rejected)
    {
        return;
    }
    if (edit.text().isEmpty())return;

    string dir = edit.text().toLocal8Bit();
    //���ʹ���Ŀ¼������
    xfm_->NewDir(remote_dir_+"/"+dir);


}
void XMSDiskClientGui::Root()
{
    xfm_->GetDir("/");
}
//Ŀ¼����
void XMSDiskClientGui::Back()
{
    if (remote_dir_.empty() || remote_dir_ == "/")
        return;
    string tmp = remote_dir_;
    if (tmp[tmp.size() - 1] == '/')
    {
        tmp = tmp.substr(0, tmp.size() - 1);
    }
    int index = tmp.find_last_of('/');
    remote_dir_ = tmp.substr(0, index);
    xfm_->GetDir(remote_dir_);
}

void XMSDiskClientGui::Delete()
{
    auto tab = ui.filetableWidget;
    int row = -1;
    for (int i = 0; i < tab->rowCount(); i++)
    {
        auto w = tab->cellWidget(i, 0);
        if (!w)continue;
        auto check = (QCheckBox*)w->layout()->itemAt(0)->widget();
        //auto check = (QCheckBox*)tab->cellWidget(i, 0);
        if (!check)continue;
        if (check->isChecked())
        {
            row = i;
            break;
        }
    }
    if (row < 0)
    {
        QMessageBox::information(this, "", QString::fromLocal8Bit("��ѡ��ɾ���ļ�"));
        return;
    }
    auto re = QMessageBox::information(this, "", QString::fromLocal8Bit("��ȷ��ɾ���ļ���"), QMessageBox::Ok | QMessageBox::Cancel);
    if (re & QMessageBox::Cancel)
        return;

    auto item = ui.filetableWidget->item(row, 1);
    string filename = item->text().toLocal8Bit();
    XFileInfo file;
    file.set_filename(filename);
    //file.set_filedir()
    this->xfm_->DeleteFile(file);
}

void XMSDiskClientGui::Download()
{
   // int row = ui.filetableWidget->currentRow();

    auto tab = ui.filetableWidget;
    int row = -1;
    for (int i = 0; i < tab->rowCount(); i++)
    {
        auto w = tab->cellWidget(i, 0);
        if (!w)continue;
        auto check = (QCheckBox*)w->layout()->itemAt(0)->widget();
        //auto check = (QCheckBox*)tab->cellWidget(i, 0);
        if (!check)continue;
        if (check->isChecked())
        {
            row = i;
            break;
        }
    }
    
    if (row < 0)
    {
        QMessageBox::information(this, "", QString::fromLocal8Bit("��ѡ�������ļ�"));
        return;
    }
    //��ȡѡ����ļ���
    auto item = ui.filetableWidget->item(row, 1);
    string filename = item->text().toLocal8Bit();
    //��ȡ����·��
    QString localpath = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("��ѡ������·��"));
    if (localpath.isEmpty())
        return;
    XFileInfo task;
    task.set_filename(filename);
    task.set_filedir(remote_dir_);
    if (localpath[localpath.size() - 1] != "/" && localpath[localpath.size() - 1] != "\\")
        localpath += "/";
    task.set_local_path(localpath.toStdString() + filename);
    xfm_->DownloadFile(task);

}
void XMSDiskClientGui::Checkall()
{
    static int count = 0;
    count++;
    qDebug() << count << "Checkall()" << ui.checkallBox->isChecked() << endl;
    auto tab = ui.filetableWidget;
    //for (auto check : check_list)
    //{
    //    check->setChecked(true);
    //}
    for (int i = 0; i < tab->rowCount(); i++)
    {
        auto w = tab->cellWidget(i, 0);
        if (!w)continue;
        auto check = (QCheckBox*)w->layout()->itemAt(0)->widget();
       //auto check = (QCheckBox*)tab->cellWidget(i, 0);
        if (!check)continue;
        check->setChecked(ui.checkallBox->isChecked());
    }
    
}
void XMSDiskClientGui::SelectFile(QModelIndex index)
{
    auto tab = ui.filetableWidget;
    for (int i = 0; i < tab->rowCount(); i++)
    {
        auto w = tab->cellWidget(i, 0);
        if (!w)continue;
        auto check = (QCheckBox*)w->layout()->itemAt(0)->widget();
        if (!check)continue;
        check->setChecked(false);
    }
    auto w = tab->cellWidget(index.row(), 0);
    if (!w)return;
    auto check = (QCheckBox*)w->layout()->itemAt(0)->widget();
    //auto check = (QCheckBox*)tab->cellWidget(i, 0);
    if (!check)return;
    check->setChecked(true);
}

void XMSDiskClientGui::RefreshData(xdisk::XFileInfoList file_list, std::string dir)
{
    remote_dir_ = dir;
    QString view_dir = "";
    QString dir_str = QString::fromLocal8Bit(dir.c_str());
    auto dir_list = dir_str.split("/");
    for (auto d : dir_list)
    {
        auto dir = d.trimmed();
        if (dir.isEmpty())
        {
            continue;
        }
        view_dir += dir;
        view_dir += "> ";
    }
    ui.dir_label->setText(view_dir);


    auto tab = ui.filetableWidget;
    while (tab->rowCount() > 0) tab->removeRow(0);
    for (auto file : file_list.files())
    {
        ///�ļ���
        string filename = file.filename();
        if (filename.empty())continue;
        

        ///�ļ�����
        //string filetype = "";
        //int pos = filename.find_last_of('.');
        //if (pos > 0)
        //{
        //    filetype = filename.substr(pos + 1);
        //}
        ////ת��ΪСд �����������������
        //transform(filetype.begin(), filetype.end(), filetype.begin(), ::tolower);


        ///�ļ����Ͷ�Ӧͼ��
        string iconpath = FILE_ICON_PATH;
        //map<string, string> icons;
        //icons["jpg"] = "Img";
        //icons["png"] = "Img";
        //icons["gif"] = "Img";

        iconpath += XGetIconFilename(filename, file.is_dir());
        iconpath += "Type.png";
        //�����б�
        tab->insertRow(0);
        
        ///��һ��ѡ��� ���ж���
        QCheckBox *ckb = new QCheckBox(tab);
        check_list.push_back(ckb);
        auto hLayout = new QHBoxLayout();
        auto widget = new QWidget(tab);
        hLayout->addWidget(ckb);
        hLayout->setMargin(0);                          // �������, ����CheckBox����������ʾ
        hLayout->setAlignment(ckb, Qt::AlignCenter);
        widget->setLayout(hLayout);
        tab->setCellWidget(0, 0, widget);

        //�趨�ļ�����ͼ��
        QString qfilename;
        qfilename = QString::fromLocal8Bit(filename.c_str());
        tab->setItem(0, 1, new QTableWidgetItem(QIcon(iconpath.c_str()), qfilename));
        
        //�ļ�ʱ��
        tab->setItem(0, 2, new QTableWidgetItem(file.filetime().c_str()));

        ///�ļ���С B KB MB GB
        string filesize_str = "";
        if (!file.is_dir())
        {
            filesize_str = XGetSizeString(file.filesize());
        }
        
        tab->setItem(0, 3, new QTableWidgetItem(filesize_str.c_str()));
    }

    //�ļ�����
    stringstream ss;
    ss << tab->rowCount();
    ui.file_count->setText(ss.str().c_str());
}
void XMSDiskClientGui::Upload()
{
    //�û�ѡ��һ���ļ�
    QString filepath = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("��ѡ���ϴ��ļ�"));
    if (filepath.isEmpty())
        return;
    qDebug() << "filepath:" << filepath << endl;

    QFileInfo fileinfo;
    fileinfo = QFileInfo(filepath);
    qDebug() << fileinfo.filePath() << endl;
    qDebug() << fileinfo.fileName() << endl;
    qDebug() << fileinfo.canonicalFilePath() << endl;
    qDebug() << fileinfo.absoluteFilePath() << endl;
    string file_real_path = filepath.toLocal8Bit();
    string filename = fileinfo.fileName().toLocal8Bit();/*
    string filedir = file_real_path.substr(0, file_real_path.size() - filename.size());*/
    XFileInfo task;
    task.set_filename(filename);
    task.set_filedir(remote_dir_);
    task.set_local_path(file_real_path);
    xfm_->UploadFile(task);
}

void XMSDiskClientGui::Refresh()
{
    //FileManager::Get()->GetDir(remote_dir_);
    xfm_->GetDir(remote_dir_);
}

void XMSDiskClientGui::contextMenuEvent(QContextMenuEvent *event)
{

    ///�Ҽ��˵�
    QMenu Context;
    Context.addAction(ui.action_new_dir);
    Context.addAction(ui.upaction);
    Context.addAction(ui.downaction);
    Context.addAction(ui.refreshaction);

    
    Context.exec(QCursor::pos());
}

static bool mouse_press = false;
static QPoint mouse_point;
void XMSDiskClientGui::mouseMoveEvent(QMouseEvent *ev)
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
void XMSDiskClientGui::mousePressEvent(QMouseEvent *ev)
{
    //���������¼�¼λ��
    if (ev->button() == Qt::LeftButton)
    {
        mouse_press = true;
        mouse_point = ev->pos();
    }

}
void XMSDiskClientGui::mouseReleaseEvent(QMouseEvent *ev)
{
    mouse_press = false;
}