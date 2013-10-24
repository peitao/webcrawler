#ifndef __MUTI_CURL_ADAPTER_H__
#define __MUTI_CURL_ADAPTER_H__
#include <string>
#include <curl/curl.h>
using std::string;
const size_t BUFSIZE = 2 * 1024 * 1024;
/* 保存一个handler的缓存和handler */
struct BuffHandler
{
	CURL * handler;
	string url;
	size_t size;
	char data_buffer[BUFSIZE];
};

CURL * new_a_handler( const string & url , BuffHandler * buff );
#endif /* __MUTI_CURL_ADAPTER_H__ */
