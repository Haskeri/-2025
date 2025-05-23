# README.md

## Текстовый редактор с функционалом работы с файлами на QT

### Описание проекта
Разработанное приложение представляет собой полнофункциональный текстовый редактор с расширенными возможностями работы с файловой системой. Реализовано на фреймворке QT с использованием C++. Основные функции включают просмотр и редактирование текстовых файлов, управление файлами и папками, а также отображение детальной информации о файлах.

### Ключевые возможности
- Просмотр и редактирование содержимого текстовых файлов
- Навигация по файловой системе через древовидное представление
- Создание, переименование и удаление файлов/папок
- Отображение метаинформации:
  - Дата создания и последнего изменения
  - Размер файла
  - Атрибуты (только для чтения, скрытый и др.)
- Валидация операций с файловой системой
- Поддержка двойного клика для открытия файлов/папок

### Техническая реализация
Проект реализован с использованием следующих компонентов QT:
- `QFileSystemModel` - для работы с файловой системой
- `QFile` и `QTextStream` - для операций с файлами
- `QTreeView` - для отображения структуры каталогов
- `QTextEdit` - для редактирования содержимого файлов
- Различные диалоговые окна (`QMessageBox`, `QInputDialog`)

### Установка и запуск
1. **Требования**:
   - QT 5.15 или новее
   - C++ компилятор (GCC, MinGW, MSVC)
   - CMake (версия 3.5+)

2. **Сборка**:
```bash
mkdir build
cd build
cmake ..
make
```

3. **Запуск**:
```bash
./FileExplorer
```

### Руководство пользователя
1. **Навигация**:
   - Используйте дерево файлов слева для навигации
   - Двойной клик по файлу открывает его для редактирования
   - Двойной клик по папке открывает её содержимое

2. **Работа с файлами**:
   - Для создания нового файла введите путь и нажмите "Создать файл"
   - Для сохранения изменений нажмите "Сохранить"
   - Для переименования выберите файл и нажмите "Переименовать"

3. **Работа с папками**:
   - Создание новой папки - введите путь и нажмите "Создать папку"
   - Удаление - выберите элемент и нажмите "Удалить"

### Примеры использования кода
Основные функции реализованы в следующих методах:

1. **Открытие файла**:
```cpp
void MainWindow::openFile(const QModelIndex &index)
{
    QFileInfo fileInfo = model->fileInfo(index);
    if(fileInfo.isFile()) {
        QFile file(fileInfo.filePath());
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            ui->textEdit->setPlainText(file.readAll());
            updateFileInfo(fileInfo);
            file.close();
        }
    }
}
```

2. **Сохранение файла**:
```cpp
void MainWindow::saveFile()
{
    if(currentFile.isEmpty()) return;
    
    QFile file(currentFile);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << ui->textEdit->toPlainText();
        file.close();
    }
}
```

### Дальнейшее развитие
Планируемые улучшения:
- Добавление поддержки вкладок
- Реализация поиска по содержимому файлов
- Поддержка синтаксической подсветки
- Интеграция с системой контроля версий
