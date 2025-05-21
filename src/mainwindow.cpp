#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileSystemModel>
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>
using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), model(new QFileSystemModel)
{
    ui->setupUi(this);
    model.setRootPath(QDir::homePath());
    ui->treeView->setModel(&model);
    ui->treeView->setRootIndex(model.index(QDir::homePath())); // домашнюю папка пользователя как корень
    ui->filenameEdit->setText(QDir::homePath());
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::inputPathExists() { // Проверка, существует ли элемент, введенный в lineEdit
    QFileInfo fi(ui->filenameEdit->text());
    if(fi.exists()) {
        QMessageBox::critical(this, "Ошибка", "Элемент с таким названием уже существует.");
        return true;
    }
    return false;
}

void MainWindow::on_createButton_clicked() //Создание файла
{
    if(inputPathExists()) {
        return;
    }
    QString fileName = ui->filenameEdit->text();
    QFile file(fileName);
    if(!file.open(QIODevice::Text | QIODevice::NewOnly)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл для записи.");
        return;
    }
    file.close(); // Сохраняем пустой файл
    QModelIndex index = model.index(fileName);
    updateIndex(index); // Обновляем выбранный файл
}

void MainWindow::on_createDirButton_clicked() // Создание папки
{
    if(inputPathExists()) {
        return;
    }
    QString dirName = ui->filenameEdit->text();
    QFileInfo info(dirName);
    if(!info.absoluteDir().mkdir(info.fileName())) { // получаем родительскую директорию и создаем в ней новую папку
        QMessageBox::critical(this, "Ошибка", "Не удалось создать папку.");
        return;
    }
    QModelIndex index = model.index(dirName);
    updateIndex(index);
}


void MainWindow::on_chooseFileButton_clicked() // Открытие файла / Папки
{
    QModelIndex index = this->model.index(ui->filenameEdit->text()); // получаем индекс модели из введенных данных пользователем
    if(!index.isValid()) { // Если !isValid, файл не существует
        QMessageBox::critical(this, "Ошибка", "Такой папки/файла не существует.");
        return;
    }
    updateIndex(index);
}


void MainWindow::on_saveButton_clicked() // Сохранение файла
{
    if(selectedPath.isEmpty()) { // Не сохраняем файл, если путь не выбран.
        QMessageBox::critical(this, "Ошибка", "Не выбран файл для сохранения.");
        return;
    }
    QFile file(this->selectedPath);
    if(!file.open(QIODevice::Text | QIODevice::WriteOnly)) { // Открываем файл для записи текста
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл для записи.");
        return;
    }
    QTextStream out(&file);
    out << ui->contentEdit->toPlainText(); // записываем содержимое файла с помощью QTextStream
    file.close();
    QFileInfo info(file);
    this->updateInfo(info); // Обновляем информацию в окне
}


void MainWindow::on_renameButton_clicked() // Переименование элемента
{
    bool ok;
    QString newName = QInputDialog::getText(this, tr("Переименование файла"),
                          tr("Новое название:"), QLineEdit::Normal,
                          QDir::home().dirName(), &ok); // Получаем новое название из диалога
    if(!ok || newName.isEmpty()) return; // Если пользователь отменил, return
    QFileInfo info(selectedPath);

    qDebug() << info.absoluteFilePath();
    qDebug() << info.absoluteDir();
    QString newPath = QStringLiteral("%1%2%3").arg(info.absoluteDir().path()).arg(QDir::separator()).arg(newName); // Новый путь:
    // (родительская_директория)/(название)
    bool done = QFile::rename(info.absoluteFilePath(), newPath);
    if(!done) {
        QMessageBox::critical(this, "Ошибка", "Не удалось переименовать элемент.");
        return;
    }
    QModelIndex index = model.index(newPath);
    updateIndex(index);
}


void MainWindow::on_deleteButton_clicked() // Удаление элемента
{
    QFileInfo info(selectedPath);
    QModelIndex index = model.index(info.absoluteDir().path()); // В TreeView выбираем родительскую папку
    if(info.isFile()) {
        QFile::remove(selectedPath); // Если файл, удаляем через QFile::remove
    } else {
        info.absoluteDir().rmdir(info.fileName()); // Если папка, то удаляем через rmdir
    }
    updateIndex(index);
}


void MainWindow::updateIndex(QModelIndex &index) { // Обновление treeView
    ui->treeView->setCurrentIndex(index); // Выбираем элемент програматично
    this->on_treeView_doubleClicked(index);
}


void MainWindow::on_treeView_doubleClicked(const QModelIndex &index) // Сигнал при двойной нажатии по файлу в treeView
{
    QFileInfo info = model.fileInfo(index); // Получаем информацию о файле
    this->selectedPath = model.filePath(index); // Устанавливаем selectedPath
    this->updateInfo(info); // Обновляем информацию о файле в интерфейсе
    if(info.isFile()) {
        ui->contentEdit->setEnabled(true); // Если файл, разрешаем редактировать содержимое и сохранять его.
        ui->saveButton->setEnabled(true);
        QFile file = QFile(selectedPath);
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream stream(&file);
            ui->contentEdit->setPlainText( stream.readAll() ); // Загруажем содержимое в редактор
        } else
            QMessageBox::warning(this, "title", "file open error:" + file.errorString() );
    } else if(info.isDir()) { // Если выбранный элемент папка, отключаем сохранение и редактирование.
        ui->fileSizeEdit->setText("Папка");
        ui->contentEdit->setPlainText("");
        ui->contentEdit->setEnabled(false);
        ui->saveButton->setEnabled(false);
    }
    ui->renameButton->setEnabled(true); // Если элемент выбран, включаем rename и delete
    ui->deleteButton->setEnabled(true);
}

void MainWindow::updateInfo(QFileInfo &info) { // Обновление информации о дате создания/изменения, размера, только ли для чтения файл
    ui->filenameEdit->setText(info.filePath());
    ui->creationDateEdit->setDateTime(info.birthTime());
    ui->modifiedDateEdit->setDateTime(info.lastModified());
    ui->fileSizeEdit->setText(QString::number(info.size()));
    ui->readOnlyLabel->setText(info.isWritable() ? "Нет" : "Да");
}

