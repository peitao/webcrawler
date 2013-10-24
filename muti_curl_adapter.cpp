#include <curl/curl.h>
#include <string.h>
#include <iostream>
#include "muti_curl_adapter.h"
using namespace std;
static size_t write_data_buff( void *buffer, size_t size, size_t nmemb, void *pbuf )
{
	BuffHandler * pbuf_in = (BuffHandler *)pbuf;
	
	cout << "writing "<< size*nmemb<<"B "<<pbuf_in->url<<endl;	
	
	/* 本次读取到的数据大小 */
	size_t total_size = size * nmemb;

	if ( total_size + pbuf_in->size < BUFSIZE )
	{
		/* 复制数据到缓冲区 */
		memcpy( pbuf_in->data_buffer + pbuf_in->size, buffer, total_size );

		/* 更新缓冲区中数据的大小 */
		pbuf_in->size += total_size;
						
	}
	return total_size;
}

CURL * new_a_handler( const string & url , BuffHandler * buff )
{
	CURL * ret = curl_easy_init();

	curl_easy_setopt( ret, CURLOPT_URL, url.c_str() );
	curl_easy_setopt( ret, CURLOPT_WRITEFUNCTION, write_data_buff );
	curl_easy_setopt( ret, CURLOPT_WRITEDATA, buff );
	curl_easy_setopt( ret, CURLOPT_PRIVATE, buff );

	return ret;
}
