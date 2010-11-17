#include <string>
#include <libxml/parser.h>

using namespace std;

class XmlParser {
    const char* buf;
    xmlDocPtr doc;

 public:
    bool init(const string&);
    string xpath(const string&);
};
