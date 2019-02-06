#ifndef RAWDATA_H
#define RAWDATA_H

#include <QStringList>
#include <cstdint>
#include <map>

class Rawdata
{
public:
    enum class enIndicador{SCM, SMP};

    Rawdata();
    virtual ~Rawdata(){}

    QString uf(const QStringList& strCsv);

    bool descartado(const QStringList& strCsv);

private:
    QString uf(const QString& cep);

protected:
    std::int8_t mColUf{0};
    std::int8_t mColCep{10};
    std::int8_t mColTipoDescarte{0};

    enIndicador mTipoIndicador;

};

#endif // RAWDATA_H
