#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QTest>
#include "core/Rawdata_Organizer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void carregarListaArquivos(const QDir& dirIn);

    void carregarTabelaLog(const QStringList& cabecalho, const std::vector<QStringList>& vecLog);

private slots:
    void on_pushButton_buscar_clicked();

    void on_pushButton_salvar_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
