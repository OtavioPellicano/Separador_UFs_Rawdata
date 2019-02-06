#include "Log.h"

Log::Log()
{

}

void Log::add(const Log::codigo &cod, const QString &obs)
{
    if(mProcessamentoOK)
        if(cod != SEM_ERRO)
            this->mProcessamentoOK = false;

    this->mVecLog.push_back(QStringList{QString::number(cod), descricao(cod), obs});
}

QString Log::descricao(const Log::codigo &cod)
{
    switch (cod) {
    case codigo::SEM_ERRO:
        return QString{"Processamento concluído com sucesso"};

    case codigo::PASTA_VAZIA:
        return QString{"Pasta de origem vazia"};

    case codigo::LAYOUT_INVALIDO:
        return QString{"Arquivo ignorado (Layout inválido)"};

    case codigo::ERRO_LEITURA:
        return QString{"Arquivo ignorado (Leitura impossível)"};

    case codigo::ERRO_ESCRITA:
        return QString{"Salvamento impossível"};

    default:
        return QString{"ERRO DESCONHECIDO"};

    }
}

std::vector<QStringList> Log::logs()
{
    return this->mVecLog;
}

QStringList Log::cabecalho()
{
    return this->mCabecalho;
}

bool Log::processamentoOK() const
{
    return mProcessamentoOK;
}

void Log::print()
{
    QString separador{";"};

    std::cout << cabecalho().join(separador).toStdString() << std::endl;
    for(auto& item:this->mVecLog)
    {
        std::cout << item.join(separador).toStdString() << std::endl;
    }

}
