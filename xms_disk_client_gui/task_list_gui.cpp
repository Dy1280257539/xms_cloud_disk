#include "task_list_gui.h"
#include "taskitemgui.h"
#include <QDebug>
#include <map>
#include <string>
#include <sstream>
using namespace std;
static map<int, TaskItemGUI*> task_items;
void TaskListGUI::RefreshUploadTask(std::list<xdisk::XFileTask> file_list)
{
    if (file_list.empty())return;
    stringstream ss;
    ss << "("<< file_list.size() << ")";
    ui.uplabel->setText(ss.str().c_str());

    upload_list = file_list;
    if (!ui.upButton->isChecked())
        return;
    RefreshTask(file_list);
}

void TaskListGUI::RefreshDownloadTask(std::list<xdisk::XFileTask> file_list)
{
    if (file_list.empty())return;
    download_list = file_list;

    stringstream ss;
    ss << "(" << file_list.size() << ")";
    ui.downlabel->setText(ss.str().c_str());

    if (!ui.downButton->isChecked())
        return;
    RefreshTask(file_list);
}

void TaskListGUI::RefreshTask(std::list<xdisk::XFileTask> file_list)
{
    //只修改 不清理
    auto tab = ui.taskableWidget;
    //while (tab->rowCount() > 0)
    //{
    //    tab->removeRow(0);
    //}
    for(auto task: file_list)
    {
        if (task_items.find(task.index()) == task_items.end())
        {
            tab->insertRow(0);
            auto item = new TaskItemGUI();
            item->SetTask(task);
            tab->setCellWidget(0, 0, item);
            
            //item->show();
            tab->setRowHeight(0,51);
            //tab->setRowHeight(0, item->height());
            task_items[task.index()] = item;
        }
        else
        {
            task_items[task.index()]->SetTask(task);
        }
    }
}


TaskListGUI::TaskListGUI(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    auto tab = ui.taskableWidget;
    while (tab->rowCount() > 0)
    {
        tab->removeRow(0);
    }
    tab->setIconSize(QSize(30, 30));
    tab->setColumnWidth(0, 100);     //checkall
    tab->setColumnWidth(1, 500);    //filename

    tab->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    tab->setSelectionMode(QAbstractItemView::SingleSelection);//设置选中单个
    ui.uplabel->setText("");
    ui.downlabel->setText("");
    ui.oklabel->setText("");
    

}

void TaskListGUI::OkTask()
{
    //RefreshTask(ok_list);
}
void TaskListGUI::UpTask()
{
    auto tab = ui.taskableWidget;
    task_items.clear();
    while (tab->rowCount() > 0)
    {
        tab->removeRow(0);
    }
    RefreshTask(upload_list);
}

void TaskListGUI::DownTask()
{
    task_items.clear();
    auto tab = ui.taskableWidget;
    while (tab->rowCount() > 0)
    {
        tab->removeRow(0);
    }
    RefreshTask(download_list);
}

void TaskListGUI::Select(QModelIndex index)
{

}

TaskListGUI::~TaskListGUI()
{
}

void TaskListGUI::Hide()
{
    this->hide();
}
void TaskListGUI::Show()
{
    this->show();
    QWidget *p = (QWidget *)this->parent();
    int w = p->width();
    auto tab_pos = ui.taskableWidget->pos();
    auto size = ui.taskableWidget->size();
    
    size.setHeight(p->height() - pos().y());
    size.setWidth(w- tab_pos.x());
    ui.taskableWidget->resize(size);
}