#include "Rawdata_Organizer.h"

Rawdata_Organizer::Rawdata_Organizer(const bool &apenasValidas, QObject *parent)
    :QObject(parent)
{
    this->mApenasValidas = apenasValidas;
}

void Rawdata_Organizer::processar(const QDir &dirIn, const QDir &dirOut)
{

    QStringList arqs{dirIn.entryList(QStringList{"*.csv"})};

    if(arqs.isEmpty())
    {
        this->mLog.add(Log::codigo::PASTA_VAZIA);
        return;
    }
    processarLista(arqs, dirIn, dirOut);

    if(this->mLog.logs().empty())
        this->mLog.add(Log::codigo::SEM_ERRO);


}

void Rawdata_Organizer::processarLista(QStringList &arqs, const QDir& dirIn, const QDir &dirOut)
{

    size_t contador{0};
    for(auto& fileName:arqs)
    {
        emit progressoFile(contador);
        emit progresso(++contador*100/arqs.size());
        processarArquivo(fileName, dirIn, dirOut);
        arqs.pop_front();

    }
}

void Rawdata_Organizer::processarArquivo(const QString &fileName, const QDir &dirIn, const QDir &dirOut)
{
    QStringList strCsv;
    std::string str;
    std::ifstream arq;
    QStringList cabecalho;

    std::unique_ptr<Rawdata> layout;

    arq.open(dirIn.absoluteFilePath(fileName).toStdString());
    if(arq.is_open())
    {
        qDebug() << "Processando..." << fileName;
        for(size_t i = 0; std::getline(arq,str); ++i)
        {
            strCsv = QString::fromStdString(str).split(";");

            if(i==0)
            {
                layout = tipoArquivo(str);
                if(layout == nullptr)
                {
                    qDebug() << "Arquivo ignorado (fora do layout):" << fileName;
                    this->mLog.add(Log::codigo::LAYOUT_INVALIDO, fileName);
                    break;
                }
                cabecalho = strCsv;
                continue;
            }

            addMedicao(layout, strCsv, dirOut, fileName, cabecalho);

        }
        arq.close();
    }
    else
    {
        qDebug() << "Erro ao ler:" << fileName;
        this->mLog.add(Log::codigo::ERRO_LEITURA, fileName);
    }

    if(!mMapUfMedicoes.isEmpty())
        descarregar(mMapUfMedicoes, dirOut, fileName, cabecalho);


}

void Rawdata_Organizer::processarMultThread(const QDir &dirIn, const QDir &dirOut)
{

    QStringList arqs{dirIn.entryList(QStringList{"*.csv"})};

    for(auto& fileName:arqs)
    {
       QtConcurrent::run(this, &processarArquivo, QString(fileName), QDir(dirIn), QDir(dirOut));

        if(arqs.isEmpty())
            break;
    }

}

QDir Rawdata_Organizer::dirIn() const
{
    return mDirIn;
}

void Rawdata_Organizer::setDirIn(const QDir &dirIn)
{
    mDirIn = dirIn;
}

QDir Rawdata_Organizer::dirOut() const
{
    return mDirOut;
}

void Rawdata_Organizer::setDirOut(const QDir &dirOut)
{
    mDirOut = dirOut;
}

void Rawdata_Organizer::descarregar(QHash<QString, std::vector<QStringList> > &mapUfMedicoes, const QDir &dirOut, const QString &fileName, const QStringList& cabecalho)
{
    for(auto itMap = mapUfMedicoes.begin(); itMap != mapUfMedicoes.end(); ++itMap)
    {
        descarregar(itMap.value(), dirOut, itMap.key() + "_" + fileName, cabecalho);
    }
    mapUfMedicoes.clear();
}

void Rawdata_Organizer::descarregar(std::vector<QStringList> &vecMedicoes, const QDir &dirOut, const QString &fileName, const QStringList& cabecalho)
{
    std::ofstream arq;
    static std::set<QString> arqsSalvos;
    std::string strCabecalho{""};
    if(arqsSalvos.insert(fileName).second)
    {
        arq.open(dirOut.absoluteFilePath(fileName).toStdString());
        strCabecalho = cabecalho.join(";").toStdString();
    }
    else
    {
        arq.open(dirOut.absoluteFilePath(fileName).toStdString(), std::ios_base::app);
    }

    if(arq.is_open())
    {
        if(!strCabecalho.empty())
            arq << strCabecalho << std::endl;
        for(auto& medicao:vecMedicoes)
        {
            arq << medicao.join(";").toStdString() << std::endl;
        }
        arq.close();
    }
    else
    {
        qDebug() << "Erro ao tentar salvar:" << fileName;
        this->mLog.add(Log::codigo::ERRO_ESCRITA, fileName);
        arqsSalvos.erase(arqsSalvos.find(fileName));
    }


    vecMedicoes.clear();
}

void Rawdata_Organizer::addMedicao(const std::unique_ptr<Rawdata>& layout, const QStringList &strCsv, const QDir& dirOut, const QString& fileName, const QStringList& cabecalho)
{
    if(this->mApenasValidas)
    {
        if(!layout.get()->descartado(strCsv))
        {
            QString ufTemp{layout.get()->uf(strCsv)};
            this->mMapUfMedicoes[ufTemp].push_back(strCsv);

            if(this->mMapUfMedicoes[ufTemp].size() > this->MAX_MED)
                descarregar(this->mMapUfMedicoes, dirOut, fileName, cabecalho);
        }
    }
    else
    {
        QString ufTemp{layout.get()->uf(strCsv)};
        this->mMapUfMedicoes[ufTemp].push_back(strCsv);

        if(this->mMapUfMedicoes[ufTemp].size() > this->MAX_MED)
            descarregar(this->mMapUfMedicoes, dirOut, fileName, cabecalho);
    }

}

Log Rawdata_Organizer::log() const
{
    return mLog;
}

void Rawdata_Organizer::setApenasValidas(bool apenasValidas)
{
    mApenasValidas = apenasValidas;
}

template<class T>
std::unique_ptr<Rawdata> Rawdata_Organizer::tipoArquivo(const T &cabecalho)
{
    if(cabecalho == "deviceID;IP;location;serialNumber;macAddress;manufacturer;modelName;softwareVersion;bwUp;bwDown;CEP;dateTime;speedTestInterval;speedTestCounter;testPoint;speedTestTimeUp;speedTestTimeDown;bytesUp;bytesDown;concurrentTrafficUp;concurrentTrafficDown;warmTime;warmBytes;warmIntervals;speedUp;speedDown;Tipo de Descarte")
        return std::unique_ptr<Rawdata>(new Scm_4_5);
    else if(cabecalho == "deviceID;IP;location;serialNumber;macAddress;manufacturer;modelName;softwareVersion;bwUp;bwDown;CEP;dateTime;testPoint;listRTT;avgRTT;minRTT;maxRTT;latencyPacketSize;Tipo de Descarte")
        return std::unique_ptr<Rawdata>(new Scm_6);
    else if(cabecalho == "deviceID;IP;location;serialNumber;macAddress;manufacturer;modelName;softwareVersion;bwUp;bwDown;CEP;dateTime;testPoint;Jitter;Tipo de Descarte")
        return std::unique_ptr<Rawdata>(new Scm_7);
    else if(cabecalho == "deviceID;IP;location;serialNumber;macAddress;manufacturer;modelName;softwareVersion;bwUp;bwDown;CEP;dateTime;testPoint;Packet Loss Failure;Packet Loss Successes;Tipo de Descarte")
        return std::unique_ptr<Rawdata>(new Scm_8);
    else if(cabecalho == "deviceID;IP;location;serialNumber;macAddress;manufacturer;modelName;softwareVersion;bwUp;bwDown;CEP;dateTime;testPoint;availSuccesses;availFails;availInterval;Tipo de Descarte")
        return std::unique_ptr<Rawdata>(new Scm_9);
    else if(cabecalho == "UF;VelocidadeDOWN;VelocidadeUP;deviceID;sourceIPv4;sourceIPv6;location;serialNumber;macAddress;softwareVersion;manufacturer;modelName;IMSI;IMEI;ICCID;osVersion;signature;dateTime;cellIdChanges;cellId;coordinates;minRSSI;wanMode;roaming;cpuLevel;ramLevel;batteryLevel;speedTestInterval;speedTestCounter;testPoint;speedTestTimeUp;speedTestTimeDown;bytesUp;bytesDown;concurrentTrafficUp;concurrentTrafficDown;warmTime;warmBytes;warmIntervals;speedUp;speedDown;availSuccesses;availFails;availInterval;listRTT;avgRTT;minRTT;maxRTT;Jitter;latencyPacketSize;serviceStatus;snr;cqi;Network Type;Teste Autonomo;CGI;TechChanges;Tipo de Descarte")
        return std::unique_ptr<Rawdata>(new Smp_10_11);
    else
        return nullptr;

}
