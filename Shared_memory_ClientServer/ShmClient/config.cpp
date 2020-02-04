#include "config.h"
#include <QFile>
#include <QXmlStreamReader>

config::config()
{
    id = 0;
    threads = 0;
}

config::~config()
{

}

int config::load(const char *xml_file)
{
    QFile file(xml_file);
       if(!file.open(QFile::ReadOnly | QFile::Text)){
           std::cerr<<"Error in load file\nTry another directory/"<<std::endl;
           file.close();
           return -1;
       }

       QXmlStreamReader xmlDoc(&file);

       while(!xmlDoc.atEnd() && !xmlDoc.hasError()){
           QXmlStreamReader::TokenType token = xmlDoc.readNext();
           /* If token is just StartDocument, we'll go to next.*/
           if(token == QXmlStreamReader::StartDocument) {
               continue;
           }
           if(token == QXmlStreamReader::StartElement) {
               if(xmlDoc.name() == "client"){
                  QXmlStreamAttributes attributes = xmlDoc.attributes();
                  id = attributes.value("id").toString().toInt();
                  threads = attributes.value("threads_count").toString().toInt();
               } else if(xmlDoc.name() == "file"){
                    QXmlStreamAttributes attributes = xmlDoc.attributes();
                    files.push_back(attributes.value("name").toString().toStdString());
               }
           }
       }
       file.close();
    return 0;
}

uint32_t config::client_id()
{
    return id;
}

uint32_t config::thread_count()
{
    return threads;
}

uint32_t config::files_count()
{
    return files.size();
}

const char* config::file(uint32_t index)
{
    return files.at(index).c_str();
}


