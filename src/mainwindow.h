#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_createButton_clicked();

    void on_chooseFileButton_clicked();

    void on_saveButton_clicked();

    void on_renameButton_clicked();

    void on_deleteButton_clicked();

    void on_treeView_doubleClicked(const QModelIndex &index);

    void updateInfo(QFileInfo &info);

    void on_createDirButton_clicked();

    bool inputPathExists();

    void updateIndex(QModelIndex &index);


private:
    Ui::MainWindow *ui;
    QFileSystemModel model; // Модель файловой системы для TreeView
    QString selectedPath; // Открытый пользователем путь


};
#endif // MAINWINDOW_H
