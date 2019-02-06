#ifndef RAWDATA_ORGANIZER_H
#define RAWDATA_ORGANIZER_H

#include "Scm_4_5.h"
#include "Scm_6.h"
#include "Scm_7.h"
#include "Scm_8.h"
#include "Scm_9.h"
#include "Smp_10_11.h"
#include "Thread_Guard.h"
#include "Log.h"

#include <memory>
#include <cstdint>
#include <vector>
#include <fstream>
#include <QDir>
#include <QDebug>
#include <QHash>
#include <set>
#include <thread>

class Rawdata_Organizer
{
public:
    Rawdata_Organizer(const bool& apenasValidas=false, const std::uint8_t& numThreads=4);
    virtual ~Rawdata_Organizer(){mMapUfMedicoes.clear();}

    void processar(const QDir& dirIn, const QDir& dirOut);

    void processarMultThread(const QDir& dirIn, const QDir& dirOut);

    QDir dirIn() const;
    void setDirIn(const QDir &dirIn);

    QDir dirOut() const;
    void setDirOut(const QDir &dirOut);

    void setApenasValidas(bool apenasValidas);

    Log log() const;

private:
    template <class T>
    std::unique_ptr<Rawdata> tipoArquivo(const T& cabecalho);
    void descarregar(QHash<QString, std::vector<QStringList>>& mapUfMedicoes, const QDir& dirOut, const QString& fileName, const QStringList& cabecalho);
    void descarregar(std::vector<QStringList> &mapMedicoes, const QDir& dirOut, const QString& fileName, const QStringList &cabecalho);

    void addMedicao(const std::unique_ptr<Rawdata> &layout, const QStringList& strCsv, const QDir& dirOut, const QString &fileName, const QStringList &cabecalho);

    void processarLista(const QStringList& arqs, const QDir &dirIn, const QDir& dirOut);

private:

    QDir mDirIn, mDirOut;

    bool mApenasValidas{false};

    std::uint8_t mNumThreads;

    QHash<QString, std::vector<QStringList>> mMapUfMedicoes;

    const std::uint16_t MAX_MED = static_cast<std::uint16_t>(1E5);

    QStringList mFilaProcessamento;

    std::uint8_t currentThreads{0};

    Log mLog;

};

#endif // RAWDATA_ORGANIZER_H
