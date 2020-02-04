#ifndef CONFIG_H
#define CONFIG_H
#include <QXmlStreamReader>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <string>

class config
{
public:
    config();
    ~config();
    int load(const char *xml_file);
    uint32_t client_id();
    uint32_t thread_count();
    uint32_t files_count();
    const char *file(uint32_t index);

private:
    uint32_t id;
    uint32_t threads;
    std::vector<std::string> files;

};

#endif // CONFIG_H
