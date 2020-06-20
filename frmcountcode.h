#pragma once

#include <QWidget>

namespace Ui {
class frmCountCode;
}

class frmCountCode : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(frmCountCode)

public:
    explicit frmCountCode(QWidget *parent = nullptr);
    ~frmCountCode() override;

private:
    Ui::frmCountCode *ui = nullptr;
    QStringList listFile = {};

private:
    void initForm();
    bool checkFile(const QString &fileName);
    void countCode(const QString &filePath);
    void countCode(const QStringList &files);
    void countCode(const QString &fileName, int &lineCode, int &lineEmpty, int &lineComments);

private Q_SLOTS:
    void on_btnOpenFile_clicked();
    void on_btnOpenPath_clicked();
    void on_btnClear_clicked();
};
