#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QFile>
#include <QString>

class ConfigManager {
public:
    ConfigManager() {}
    ~ConfigManager() {}

    QString readConfig(const QString& fileName, const QString& key) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return "";
        }

        QXmlStreamReader reader(&file);
        while (!reader.atEnd()) {
            if (reader.isStartElement() && reader.name() == key) {
                return reader.readElementText();
            }
            reader.readNext();
        }

        return "";
    }

    void writeConfig(const QString& fileName, const QString& key, const QString& value) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            return;
        }

        QXmlStreamReader reader(&file);
        QXmlStreamWriter writer(&file);
        writer.setAutoFormatting(true);
        writer.writeStartDocument();

        bool foundKey = false;
        while (!reader.atEnd()) {
            if (reader.isStartElement() && reader.name() == key) {
                writer.writeStartElement(key);
                writer.writeCharacters(value);
                writer.writeEndElement();
                foundKey = true;
            } else {
                writer.writeCurrentToken(reader);
            }
            reader.readNext();
        }

        if (!foundKey) {
            writer.writeStartElement(key);
            writer.writeCharacters(value);
            writer.writeEndElement();
        }

        writer.writeEndDocument();
    }
};

#endif // CONFIGMANAGER_H

