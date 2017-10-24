#include <stdio.h>

#include <curl/curl.h>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <mysql.h>

#define MAX_BUF 65536
 char wr_buf[MAX_BUF + 1];
 int wr_index;

int writer_data(void *, size_t, size_t, void *);

int main(){
    CURL *curl;
    CURLcode res;
    char *content;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl){
	curl_easy_setopt(curl, CURLOPT_URL, "localhost");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer_data);
	res = curl_easy_perform(curl);
	//if(res == 0) printf("%s\n", wr_buf);
	htmlDocPtr doc;
	xmlNode *root_element;
	doc = htmlReadMemory(wr_buf, strlen(wr_buf), NULL, NULL, HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING | HTML_PARSE_NONET);
	root_element = xmlDocGetRootElement(doc);
	

//	printf("%s-----%s\n", root_element->name, (char *)xmlNodeGetContent(root_element));


	xmlXPathContextPtr context;
	context = xmlXPathNewContext(doc);
	

	xmlXPathObjectPtr result;
	result = xmlXPathEvalExpression((unsigned char*)"/html//a", context);
	xmlXPathFreeContext(context);

	xmlNodeSetPtr nodeSet = result->nodesetval;
	
	MYSQL mysql, *sock;
	mysql_init(&mysql);
	sock = mysql_real_connect(&mysql, "localhost", "root", "root", "spider", 3306, NULL, 0);

	mysql_query(sock, "INSERT INTO `host` SET host='127.0.0.1', hostname='localhost';");

	int size = nodeSet->nodeNr;
	for(int i = 0; i < size; i++){
	    xmlNode *cur = nodeSet->nodeTab[i];
	    //printf("%s\n", (char *)xmlNodeGetContent(cur));
	    xmlAttr *attr = cur->properties;
	    while(attr){
		if(strcmp((char *)attr->name, "href") == 0){
		    printf("%s\n", attr->children->content);
		}
		attr = attr->next;
	    }
	}

	curl_easy_cleanup(curl);
    }
    return 0;
}

int writer_data(void *buffer, size_t size, size_t nmemb, void *writerData){
    int sizes = size * nmemb;
    if(wr_index + sizes > MAX_BUF){
    	*(int *)writerData = 1;
    	return 0;
    }
    memcpy((void *)&wr_buf[wr_index], buffer, (size_t)sizes);
 
    /* Update the write index */
    wr_index += sizes;
 
    /* Null terminate the buffer */
    wr_buf[wr_index] = 0;
 
    /* Return the number of bytes received, indicating to curl that all is okay */
    return sizes;
}
