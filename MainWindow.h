#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QThread>
#include "core/Rawdata_Organizer.h"
#include <QScrollBar>

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

    void updateProgressBar(int value);

    void updateProgressoLista(int value);


private:
    Ui::MainWindow *ui;

    QThread* mThread;

    Rawdata_Organizer* mRawdata;

    const std::uint8_t mMAXIMO_ITENS_LISTA{100};
};

#endif // MAINWINDOW_H
