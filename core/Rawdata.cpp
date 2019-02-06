#include "Rawdata.h"

Rawdata::Rawdata()
{

}

QString Rawdata::uf(const QStringList &strCsv)
{
    if(this->mTipoIndicador == enIndicador::SCM)
        return uf(strCsv[mColCep]);
    else if(this->mTipoIndicador == enIndicador::SMP)
        return strCsv[mColUf];
    else
        return QString{""};

}

bool Rawdata::descartado(const QStringList &strCsv)
{
    if(strCsv[mColTipoDescarte].isEmpty())
        return false;
    else
        return true;
}

QString Rawdata::uf(const QString &cep)
{
    if(cep.isEmpty())
        return QString{""};

    using std::pair;
    long cepL = cep.toLong();

    static std::multimap<QString, pair<long, long>> mapUfCepRange = {
            {"AC", pair<long, long>(69900000, 69999999)},
            {"AL", pair<long, long>(57000000, 57999999)},
            {"AM", pair<long, long>(69000000, 69299999)},
            {"AM", pair<long, long>(69400000, 69899999)},
            {"AP", pair<long, long>(68900000, 68999999)},
            {"BA", pair<long, long>(40000000, 48999999)},
            {"CE", pair<long, long>(60000000, 63999999)},
            {"DF", pair<long, long>(70000000, 72799999)},
            {"DF", pair<long, long>(73000000, 73699999)},
            {"ES", pair<long, long>(29000000, 29999999)},
            {"GO", pair<long, long>(72800000, 72999999)},
            {"GO", pair<long, long>(73700000, 76799999)},
            {"MA", pair<long, long>(65000000, 65999999)},
            {"MG", pair<long, long>(30000000, 39999999)},
            {"MS", pair<long, long>(79000000, 79999999)},
            {"MT", pair<long, long>(78000000, 78899999)},
            {"PA", pair<long, long>(66000000, 68899999)},
            {"PB", pair<long, long>(58000000, 58999999)},
            {"PE", pair<long, long>(50000000, 56999999)},
            {"PI", pair<long, long>(64000000, 64999999)},
            {"PR", pair<long, long>(80000000, 87999999)},
            {"RJ", pair<long, long>(20000000, 28999999)},
            {"RN", pair<long, long>(59000000, 59999999)},
            {"RO", pair<long, long>(76800000, 76999999)},
            {"RR", pair<long, long>(69300000, 69399999)},
            {"RS", pair<long, long>(90000000, 99999999)},
            {"SC", pair<long, long>(88000000, 89999999)},
            {"SE", pair<long, long>(49000000, 49999999)},
            {"SP", pair<long, long>(1000000, 19999999)},
            {"TO", pair<long, long>(77000000, 77999999)}
        };

    for(auto& item : mapUfCepRange)
    {
        if(cepL >= item.second.first && cepL <= item.second.second)
            return item.first;
    }

    return QString{""};
}

