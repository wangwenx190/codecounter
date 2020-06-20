#include "frmcountcode.h"
#include "ui_frmcountcode.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QTextStream>

frmCountCode::frmCountCode(QWidget *parent) : QWidget(parent), ui(new Ui::frmCountCode)
{
    ui->setupUi(this);
    initForm();
    on_btnClear_clicked();
}

frmCountCode::~frmCountCode()
{
    delete ui;
}

void frmCountCode::initForm()
{
    const QStringList headText = {QString::fromUtf8("File name"),
                                  QString::fromUtf8("File type"),
                                  QString::fromUtf8("File size"),
                                  QString::fromUtf8("Total lines"),
                                  QString::fromUtf8("Code lines"),
                                  QString::fromUtf8("Comment lines"),
                                  QString::fromUtf8("Empty lines"),
                                  QString::fromUtf8("File path")};

    const int columnCount = headText.count();
    ui->tableWidget->setColumnCount(columnCount);
    ui->tableWidget->setHorizontalHeaderLabels(headText);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setHighlightSections(false);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(20);
    ui->tableWidget->verticalHeader()->setHighlightSections(false);

    ui->txtCount->setStyleSheet(QString::fromUtf8("color:#17A086;"));
    ui->txtSize->setStyleSheet(QString::fromUtf8("color:#CA5AA6;"));
    ui->txtRow->setStyleSheet(QString::fromUtf8("color:#CD1B19;"));
    ui->txtCode->setStyleSheet(QString::fromUtf8("color:#22A3A9;"));
    ui->txtComment->setStyleSheet(QString::fromUtf8("color:#D64D54;"));
    ui->txtEmpty->setStyleSheet(QString::fromUtf8("color:#A279C5;"));

    QFont font = QApplication::font();
    font.setBold(true);
    if (font.pointSize() > 0) {
        font.setPointSize(font.pointSize() + 1);
    } else {
        font.setPixelSize(font.pixelSize() + 2);
    }

    ui->txtCount->setFont(font);
    ui->txtSize->setFont(font);
    ui->txtRow->setFont(font);
    ui->txtCode->setFont(font);
    ui->txtComment->setFont(font);
    ui->txtEmpty->setFont(font);

    ui->txtFilter->setPlaceholderText(
        QString::fromUtf8("Use space to separate, for example: *.h *.cpp *.c"));
}

bool frmCountCode::checkFile(const QString &fileName)
{
    if (fileName.startsWith(QString::fromUtf8("moc_"), Qt::CaseInsensitive)
        || fileName.startsWith(QString::fromUtf8("ui_"), Qt::CaseInsensitive)
        || fileName.startsWith(QString::fromUtf8("qrc_"), Qt::CaseInsensitive)) {
        return false;
    }

    const QFileInfo file(fileName);
    const QString suffix = QString::fromUtf8("*.") + file.suffix();
    const QString filter = ui->txtFilter->text().trimmed();
    const QStringList filters = filter.split(QChar::fromLatin1(' '));
    return filters.contains(suffix);
}

void frmCountCode::countCode(const QString &filePath)
{
    const QDir dir(filePath);
    const auto fil = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries | QDir::Readable,
                                       QDir::Name | QDir::DirsFirst);
    for (auto &&fileInfo : qAsConst(fil)) {
        if (fileInfo.isFile()) {
            if (checkFile(fileInfo.fileName())) {
                listFile << fileInfo.canonicalFilePath();
            }
        } else {
            countCode(fileInfo.canonicalFilePath());
        }
    }
}

void frmCountCode::countCode(const QStringList &files)
{
    int lineCode = 0;
    int lineEmpty = 0;
    int lineComments = 0;
    const int count = files.count();
    on_btnClear_clicked();
    ui->tableWidget->setRowCount(count);

    quint32 totalLines = 0;
    quint32 totalBytes = 0;
    quint32 totalCodes = 0;
    quint32 totalComments = 0;
    quint32 totalEmptys = 0;

    for (int i = 0; i != count; ++i) {
        const QFileInfo fileInfo(files.at(i));
        countCode(fileInfo.canonicalFilePath(), lineCode, lineEmpty, lineComments);
        const int lineAll = lineCode + lineEmpty + lineComments;

        auto *itemName = new QTableWidgetItem;
        itemName->setText(fileInfo.fileName());

        auto *itemSuffix = new QTableWidgetItem;
        itemSuffix->setText(fileInfo.suffix());

        auto *itemSize = new QTableWidgetItem;
        itemSize->setText(QString::number(fileInfo.size()));

        auto *itemLine = new QTableWidgetItem;
        itemLine->setText(QString::number(lineAll));

        auto *itemCode = new QTableWidgetItem;
        itemCode->setText(QString::number(lineCode));

        auto *itemComment = new QTableWidgetItem;
        itemComment->setText(QString::number(lineComments));

        auto *itemEmpty = new QTableWidgetItem;
        itemEmpty->setText(QString::number(lineEmpty));

        auto *itemPath = new QTableWidgetItem;
        itemPath->setText(QDir::toNativeSeparators(fileInfo.canonicalFilePath()));

        itemSuffix->setTextAlignment(Qt::AlignCenter);
        itemSize->setTextAlignment(Qt::AlignCenter);
        itemLine->setTextAlignment(Qt::AlignCenter);
        itemCode->setTextAlignment(Qt::AlignCenter);
        itemComment->setTextAlignment(Qt::AlignCenter);
        itemEmpty->setTextAlignment(Qt::AlignCenter);

        ui->tableWidget->setItem(i, 0, itemName);
        ui->tableWidget->setItem(i, 1, itemSuffix);
        ui->tableWidget->setItem(i, 2, itemSize);
        ui->tableWidget->setItem(i, 3, itemLine);
        ui->tableWidget->setItem(i, 4, itemCode);
        ui->tableWidget->setItem(i, 5, itemComment);
        ui->tableWidget->setItem(i, 6, itemEmpty);
        ui->tableWidget->setItem(i, 7, itemPath);

        totalBytes += fileInfo.size();
        totalLines += lineAll;
        totalCodes += lineCode;
        totalComments += lineComments;
        totalEmptys += lineEmpty;

        if (i % 100 == 0) {
            qApp->processEvents();
        }
    }

    listFile.clear();
    ui->txtCount->setText(QString::number(count));
    ui->txtSize->setText(QString::number(totalBytes));
    ui->txtRow->setText(QString::number(totalLines));
    ui->txtCode->setText(QString::number(totalCodes));
    ui->txtComment->setText(QString::number(totalComments));
    ui->txtEmpty->setText(QString::number(totalEmptys));

    qreal percent = 0.0;
    percent = (static_cast<qreal>(totalCodes) / totalLines) * 100;
    ui->labPercentCode->setText(
        QString::fromUtf8("%1%").arg(percent, 5, 'f', 2, QChar::fromLatin1(' ')));
    percent = (static_cast<qreal>(totalComments) / totalLines) * 100;
    ui->labPercentComment->setText(
        QString::fromUtf8("%1%").arg(percent, 5, 'f', 2, QChar::fromLatin1(' ')));
    percent = (static_cast<qreal>(totalEmptys) / totalLines) * 100;
    ui->labPercentEmpty->setText(
        QString::fromUtf8("%1%").arg(percent, 5, 'f', 2, QChar::fromLatin1(' ')));
}

void frmCountCode::countCode(const QString &fileName,
                             int &lineCode,
                             int &lineEmpty,
                             int &lineComments)
{
    lineCode = lineEmpty = lineComments = 0;
    QFile file(fileName);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream out(&file);
        QString line = QString();
        bool isComment = false;
        while (!out.atEnd()) {
            line = out.readLine();

            if (line.startsWith(QChar::fromLatin1(' '))) {
                line.remove(QChar::fromLatin1(' '));
            }

            if (line.startsWith(QString::fromUtf8("/*"))) {
                isComment = true;
            }

            if (isComment) {
                lineComments++;
            } else {
                if (line.startsWith(QString::fromUtf8("//"))) {
                    lineComments++;
                } else if (line.isEmpty()) {
                    lineEmpty++;
                } else {
                    lineCode++;
                }
            }

            if (line.endsWith(QString::fromUtf8("*/"))) {
                isComment = false;
            }
        }
    }
}

void frmCountCode::on_btnOpenFile_clicked()
{
    const QString filter = QString::fromUtf8("Code files (%1)").arg(ui->txtFilter->text().trimmed());
    const QStringList files = QFileDialog::getOpenFileNames(nullptr,
                                                            QString::fromUtf8("Select files"),
                                                            QCoreApplication::applicationDirPath(),
                                                            filter);
    if (files.size() > 0) {
        ui->txtFile->setText(files.join(QChar::fromLatin1('|')));
        countCode(files);
    }
}

void frmCountCode::on_btnOpenPath_clicked()
{
    const QString path = QFileDialog::getExistingDirectory(nullptr,
                                                           QString::fromUtf8("Select folder"),
                                                           QCoreApplication::applicationDirPath(),
                                                           QFileDialog::ShowDirsOnly
                                                               | QFileDialog::DontResolveSymlinks);
    if (!path.isEmpty()) {
        ui->txtPath->setText(QDir::toNativeSeparators(path));
        listFile.clear();
        countCode(path);
        countCode(listFile);
    }
}

void frmCountCode::on_btnClear_clicked()
{
    ui->txtCount->setText(QString::fromUtf8("0"));
    ui->txtSize->setText(QString::fromUtf8("0"));
    ui->txtRow->setText(QString::fromUtf8("0"));

    ui->txtCode->setText(QString::fromUtf8("0"));
    ui->txtComment->setText(QString::fromUtf8("0"));
    ui->txtEmpty->setText(QString::fromUtf8("0"));

    ui->labPercentCode->setText(QString::fromUtf8("0%"));
    ui->labPercentComment->setText(QString::fromUtf8("0%"));
    ui->labPercentEmpty->setText(QString::fromUtf8("0%"));

    ui->tableWidget->setRowCount(0);
}
