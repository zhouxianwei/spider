#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>


#include <curl/curl.h>

FILE *fp;

size_t show_data(void *ptr, size_t size, size_t nmemb, void *stream){
    return fwrite(ptr, size, nmemb, (FILE *)fp);
}

int main(){
    CURL  *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(curl){
	curl_easy_setopt(curl, CURLOPT_URL, "http://www.baidu.com");
//	res = curl_easy_perform(curl);
	fp = fopen("aa.html", "w");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, show_data);
	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
    }
    return 0;
}
