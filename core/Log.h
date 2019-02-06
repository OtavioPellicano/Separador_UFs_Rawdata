#ifndef LOG_H
#define LOG_H

#include <vector>
#include <QStringList>
#include <iostream>

class Log
{
public:
    enum codigo{
        SEM_ERRO,
        PASTA_VAZIA,
        LAYOUT_INVALIDO,
        ERRO_LEITURA,
        ERRO_ESCRITA
    };
    Log();
    virtual ~Log(){mVecLog.clear();}

    void add(const codigo& cod, const QString& obs="");

    QString descricao(const codigo& cod);

    std::vector<QStringList> logs();

    QStringList cabecalho();

    bool processamentoOK() const;

    void print();

private:
    std::vector<QStringList> mVecLog;

    bool mProcessamentoOK{true};

    QStringList mCabecalho{"Codigo", "Descricao", "Observacao"};

};

#endif // LOG_H
