#include <string>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xmlerror.h>
#include "XmlParser.h"

void DummyXmlErrorHandler(void * ctx, const char * msg, ...)
{
    return;
}

XmlParser::XmlParser()
{
    isXmlParsed = false;
    xmlGenericErrorFunc handler = (xmlGenericErrorFunc)DummyXmlErrorHandler;
    initGenericErrorDefaultFunc(&handler);
}

bool XmlParser::isReady()
{
    return isXmlParsed;
}

bool XmlParser::init(const string& buffer)
{
    buf = buffer.c_str();

    doc = xmlParseMemory(buf, buffer.size());

    isXmlParsed = (doc != NULL ? true: false);
    return isXmlParsed;
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
