#include "xmlio.h"
#include <QFile>
#include <QSaveFile>
#include <QMessageBox>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

XmlIO::XmlIO(const QString& nomefile1, const QString &nomefile2)
    : ServerFile(nomefile1), NodesFile(nomefile2)
{ }


Plist<DeepPtr<Server>> XmlIO::readServer() const
{
    // Crea una lista vuota
    Plist<DeepPtr<Server>> qontainer;

    // Apre un file (QFile)
    QFile file(ServerFile);

    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox box(QMessageBox::Warning,
                        "Errore apertura file",
                        "Errore nell'apertura del file. Impossibile leggere il file.",
                        QMessageBox::Ok);
        box.exec();
        throw 1;
        return qontainer;
    }

    // Lettura da file XML
    QXmlStreamReader reader(&file);

    // Inizio a leggere il file XML
    if(reader.readNextStartElement() && reader.name() == "serverList")
    {
        while(reader.readNextStartElement())
        {
            try
            {
                qontainer.pushBack(Server::dataUnserialize(reader));
            }
            catch(std::string s)
            {
                QMessageBox box(QMessageBox::Warning,
                                "Errore caricamento file",
                                QString("Non è stato possibile caricare il file a causa di un errore nella lettura. "
                                "<br><u>Motivo:</u> %1").arg(QString::fromStdString(s)),
                                QMessageBox::Ok);
                box.exec();
                throw 1;
            }
        }
    }
    else
    {
        QMessageBox box(QMessageBox::Warning,
                        "Errore caricamento file",
                        "Non è stato possibile leggere il file dal momento che "
                        "il contenuto non è stato riconosciuto.",
                        QMessageBox::Ok);
        box.exec();
        throw 1;
    }

    file.close();
    return qontainer;
}


Plist<DeepPtr<Node>> XmlIO::readNodes() const
{
    // Crea una lista
    Plist<DeepPtr<Node>> qontainer;

    // Apre un file (QFile)
    QFile file(NodesFile);

    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox box(QMessageBox::Warning,
                        "Errore apertura file",
                        "Errore nell'apertura del file. Impossibile leggere il file.",
                        QMessageBox::Ok);
        box.exec();
        throw 1;
        return qontainer;
    }

    // Lettura da file XML
    QXmlStreamReader reader(&file);

    // Inizio a leggere il file XML
    if(reader.readNextStartElement() && reader.name() == "nodesList")
        {
            while(reader.readNextStartElement() && reader.name() == "node")
            {
                try
                {
                    qontainer.pushBack(Node::dataUnserialize(reader));
                }
                catch(std::string s)
                {
                    QMessageBox box(QMessageBox::Warning,
                                    "Errore caricamento file",
                                    QString("Non è stato possibile caricare il file a causa di un errore nella lettura. "
                                    "<br><u>Motivo:</u> %1").arg(QString::fromStdString(s)),
                                    QMessageBox::Ok);
                    box.exec();
                    throw 1;
                }
            }
    }

    file.close();
    return qontainer;
}



bool XmlIO::writeServer(const Plist<DeepPtr<Server>>& qontainer) const
{

    QSaveFile file(ServerFile);

    if(!file.open(QIODevice::WriteOnly))
    {
        return false;
    }

    QXmlStreamWriter writer(&file);

    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeComment("QCloud Manager - File di salvataggio auto-generato");

    writer.writeStartElement("serverList");

    try
    {
        for(auto cit = qontainer.iBegin(); !cit.hasEnded(); ++cit)
            (*cit)->dataSerialize(writer);
    }
    catch(std::string s)
    {
        return false;
    }

    writer.writeEndElement();
    writer.writeEndDocument();

    file.commit();
    return true;
}
