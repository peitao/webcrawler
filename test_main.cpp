#include <boost/utility.hpp>
#include <curl/curl.h>
#include <iostream>
#include <iomanip>
#include <sys/syscall.h>
#include <sys/types.h>
#include <curl/curl.h>
#include <queue>
#include <string>
#include <fstream>
#include <iostream>
#include <string.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include "util.h"
#include "urls.h"
#include "bloom_filter.h"
#include "mutex_lock.h"
using namespace std;
/* libcurl的回调函数，向数据缓冲区中写入读取到的page */
struct buffer_internel
{
	size_t size;
	void * data_buffer;
};

static size_t write_data( void *buffer, size_t size, size_t nmemb, void *pbuf )
{
	buffer_internel * pbuf_in = (buffer_internel *)pbuf;
	
	/* 本次读取到的数据大小 */
	size_t total_size = size * nmemb;
	
	/* 复制数据到缓冲区 */
	memcpy( pbuf_in->data_buffer + pbuf_in->size, buffer, total_size );
	
	/* 更新缓冲区中数据的大小 */
	pbuf_in->size += total_size;
	
	return total_size;
}
char data_buffer[1024*1024*100];
int main (int argc, char const* argv[])
{
	//test_main_util(argc,argv);
	//test_main_urls();
	//test_main_bloom();
	curl_global_init(CURL_GLOBAL_ALL);

	CURL * curl = curl_easy_init();

	/* 数据buffer和读取到的数据大小 */
	buffer_internel buf_in = {0,data_buffer};
	
	curl_easy_setopt( curl, CURLOPT_URL, "http://www.12ddd.cn" );
	curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, write_data );
	curl_easy_setopt( curl, CURLOPT_WRITEDATA, &buf_in );
	if ( curl_easy_perform( curl ) )
	 	cout << "curl_easy_perform err" << endl;
	return 0;
}
