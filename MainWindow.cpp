#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pushButton_salvar->setEnabled(false);

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
    ui->tableWidget_listaArquivos->setRowCount(arqs.size());
    ui->tableWidget_listaArquivos->clearContents();

    size_t i{0};
    for(auto it = arqs.begin(); it != arqs.end();++ it, ++i)
    {
        ui->tableWidget_listaArquivos->setItem(i, 0, new QTableWidgetItem(it[0]));
    }
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
    }

    QString pathOut{QFileDialog::getExistingDirectory(this, tr("Diretório de Saída"))};
    if(pathOut.isEmpty())
        return;

    QDir dirOut{pathOut};

    QMessageBox msg(this);
    msg.setWindowTitle(tr("Processamento Rawdata"));
    msg.setText("Separando UFs...");
    msg.setStandardButtons(0);  //removendo todos os botões
    msg.show();
    QTest::qWait(5);    //Esperando a janela abrir

    Rawdata_Organizer rawdata(ui->checkBox_medValidas->isChecked());
    rawdata.processar(dirIn, dirOut);

    msg.close();
    msg.setText("Processamento Concluído!");
    msg.setStandardButtons(QMessageBox::Ok);

    carregarTabelaLog(rawdata.log().cabecalho(), rawdata.log().logs());

    msg.exec();

//    ResumoProcessamento resPros(rawdata.log().cabecalho(), rawdata.log().logs(), this);

//    resPros.exec();


    qDebug() << "Salvo";

}
