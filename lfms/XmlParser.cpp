#include <string>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include "XmlParser.h"

bool XmlParser::init(const string& buffer)
{
    buf = buffer.c_str();

    doc = xmlParseMemory(buf, buffer.size());

    return true;
}

string XmlParser::xpath(const string& xpath)
{
    xmlXPathContextPtr ctx;
    xmlXPathObjectPtr result;
    xmlChar* str;
    string value;

    ctx = xmlXPathNewContext(doc);
    result = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), ctx);

    if (!result ||
        xmlXPathNodeSetIsEmpty(result->nodesetval) ||
        (result->nodesetval->nodeNr > 1))
    {
        return "";
    }

    str = xmlNodeListGetString(doc, result->nodesetval->nodeTab[0]->xmlChildrenNode, 1);
    value = (char *)str;

    xmlFree(str);

    xmlXPathFreeObject(result);

    return value;
}
