#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pushButton_salvar->setEnabled(false);

    ui->tableWidget_listaArquivos->setSelectionBehavior(QAbstractItemView::SelectRows);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::carregarListaArquivos(const QDir &dirIn)
{

    ui->tableWidget_listaArquivos->setColumnCount(1);
    ui->tableWidget_listaArquivos->setHorizontalHeaderLabels(QStringList{"Arquivos .csv no diretório"});
    ui->tableWidget_listaArquivos->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_listaArquivos->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QStringList arqs{dirIn.entryList(QStringList{"*.csv"})};

    ui->tableWidget_listaArquivos->clearContents();

    size_t i{0};
    for(auto it = arqs.begin(); it != arqs.end();++it, ++i)
    {
        ui->tableWidget_listaArquivos->setRowCount(i+1);
        if(i < mMAXIMO_ITENS_LISTA)
            ui->tableWidget_listaArquivos->setItem(i, 0, new QTableWidgetItem(it[0]));
        else
        {
            ui->tableWidget_listaArquivos->setItem(i, 0, new QTableWidgetItem("..."));
            break;
        }
     }

    updateProgressBar(0);
    ui->statusBar->clearMessage();
}

void MainWindow::carregarTabelaLog(const QStringList &cabecalho, const std::vector<QStringList> &vecLog)
{

    ui->tableWidget_listaArquivos->clearContents();

    ui->tableWidget_listaArquivos->setColumnCount(cabecalho.size());
    ui->tableWidget_listaArquivos->setRowCount(vecLog.size());
    ui->tableWidget_listaArquivos->setHorizontalHeaderLabels(cabecalho);
    ui->tableWidget_listaArquivos->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_listaArquivos->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableWidget_listaArquivos->setColumnWidth(0, 80);
    ui->tableWidget_listaArquivos->setColumnWidth(1, 250);


    std::size_t i{0};
    for(auto itVec = vecLog.begin(); itVec != vecLog.end(); ++itVec, ++i)
    {
        ui->tableWidget_listaArquivos->setItem(i, 0, new QTableWidgetItem((*itVec)[0]));
        ui->tableWidget_listaArquivos->setItem(i, 1, new QTableWidgetItem((*itVec)[1]));
        ui->tableWidget_listaArquivos->setItem(i, 2, new QTableWidgetItem((*itVec)[2]));
    }
}

void MainWindow::on_pushButton_buscar_clicked()
{
    //Windows file dialog does not support displaying files in the directory chooser.
    QString path{QFileDialog::getExistingDirectory(this, tr("Diretório de Rawdatas"))};
    if(path.isEmpty())
    {
        return;
    }

    ui->lineEdit_dirIn->setText(path);
    ui->pushButton_salvar->setEnabled(true);

    carregarListaArquivos(QDir{path});

}

void MainWindow::on_pushButton_salvar_clicked()
{

    QDir dirIn;
    dirIn.cd(ui->lineEdit_dirIn->text());
    if(!dirIn.exists())
    {
        QMessageBox::information(this, tr("Processamento Rawdata"), tr("Escolha um diretório válido!"), QMessageBox::Ok);
        return;
    }

    carregarListaArquivos(dirIn);

    QMessageBox::warning(this, tr("Processamento Rawdata"), tr("Após iniciado, o processamento não pode ser cancelado!"));

    QString pathOut{QFileDialog::getExistingDirectory(this, tr("Diretório de Saída"))};
    if(pathOut.isEmpty())
        return;

    QDir dirOut{pathOut};

    mRawdata = new Rawdata_Organizer(ui->checkBox_medValidas->isChecked(), this);
    mRawdata->deleteLater();

    mThread = new QThread(this);
    mThread->deleteLater();
    mRawdata->moveToThread(mThread);

    connect(mRawdata, SIGNAL(progresso(int)), this, SLOT(updateProgressBar(int)));
    connect(mRawdata, SIGNAL(progressoFile(int)), this, SLOT(updateProgressoLista(int)));

    ui->statusBar->showMessage("Processando...");
    mRawdata->processar(dirIn, dirOut);

    carregarTabelaLog(mRawdata->log().cabecalho(), mRawdata->log().logs());

    updateProgressBar(100);

    ui->statusBar->showMessage("Processamento Concluído!");

}

void MainWindow::updateProgressBar(int value)
{
    QCoreApplication::processEvents();
    ui->progressBar->setValue(value);
}

void MainWindow::updateProgressoLista(int value)
{
    if(value >= ui->tableWidget_listaArquivos->rowCount())
        return;

    if(value != 0)
        ui->tableWidget_listaArquivos->item(value - 1, 0)->setSelected(false);

    ui->tableWidget_listaArquivos->item(value, 0)->setSelected(true);
    ui->tableWidget_listaArquivos->verticalScrollBar()->setValue(value);
}

