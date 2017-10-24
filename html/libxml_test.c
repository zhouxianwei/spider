#include <stdio.h>
#include <string.h>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>

int main(){
    htmlDocPtr doc;
    doc = htmlReadFile("baidu.html", NULL, HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING | HTML_PARSE_NONET);

    xmlNode *rootElement;
    rootElement = xmlDocGetRootElement(doc);
    
    xmlXPathContextPtr context;
    context = xmlXPathNewContext(doc);


    xmlXPathObjectPtr result = xmlXPathEvalExpression((unsigned char *)"/html/body//a", context);
    xmlXPathFreeContext(context);
    xmlNodeSetPtr nodeSet = result->nodesetval;

    int size = nodeSet->nodeNr;
    for(int i = 0; i < size; i++){
	xmlNode *node = nodeSet->nodeTab[i];
	xmlAttr * attr = node->properties;
	while(attr){
	    if(strcmp((char *)attr->name, "href") == 0){
		printf("%s\n",attr->children->content);
	    }
	    attr = attr->next;
	}
//	printf("%s\n", (char*)xmlNodeGetContent(nodeSet->nodeTab[i]));
    }
    return 0;
}
