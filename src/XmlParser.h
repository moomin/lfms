#include <string>
#include <libxml/parser.h>

#ifndef XML_PARSER_H
#define XML_PARSER_H

using namespace std;

class XmlParser {
    const char* buf;
    bool isXmlParsed;
    xmlDocPtr doc;

 public:
    XmlParser();
    bool isReady();
    bool init(const string&);
    string xpath(const string&);
};

#endif
