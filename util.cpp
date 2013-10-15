#include <iostream>
#include <string.h>
#include <curl/curl.h>
#include "util.h"

using namespace std;
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

size_t fetch_url( void * data_buffer, const char * url )
{	
	/* 数据buffer和读取到的数据大小 */
	buffer_internel buf_in = {0,data_buffer};
	
	/* 初始化curl库 */
	CURL * curl = NULL;
	if ( (curl = curl_easy_init()) == NULL )
		cout << "curl_easy_init err" << endl;
	
	/* 设置curl的参数，url和回调函数 */
	curl_easy_setopt( curl, CURLOPT_URL, url );
	curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, write_data );
	curl_easy_setopt( curl, CURLOPT_WRITEDATA, &buf_in );
	
	/* 从网络读取url对应资源 */
	if ( curl_easy_perform( curl ) )
	 	cout << "curl_easy_perform err" << endl;
	
	curl_easy_cleanup( curl );

	return buf_in.size;
}

bool is_root_path(const char * url)
{
	return ! (strstr( url, "http" ) == NULL);
}

string get_host_name(const char * url)
{
	if ( strlen(url) < 2)  return string();
	
	const char * start = url;
	const char * to = url;
	if ( strstr( start, "http://" ) )
		start += 7;
	else 
	{
		if ( (* start) == '/' )
			start += 1;
	}
	
	to = start;
	
	const char * end = strstr( to, "/" );
	if (end) to = end;
	else to = url + strlen(url);
	

	
	return string( start, to );
}

string pick_url(const char * start)
{
	const char * from = start;
	const char * to = start;
	for (  ; ; )
	{
		if ( (*to) == '\0' ) break;
		if ( (*to) == '>' ) break;
		if ( (*to) == '\"') break;
		if ( (*to) == ' ') break;
		if ( (*to) == '#') break;
		if ( (*to) == '}') break;
		if ( (*to) == '{') break;
		if ( (*to) == '\'') break;
		to++;
	}
	
 	string ret = string(from,to);
 		
	size_t size = ret.size();
	
	if ( size == 0 ) return string();
	
	if ( ret[ size - 1 ] == '/' ) 
		return ret.substr( 0, size - 1 );
	else return ret;
}

void parse_page(const char * url, const char * buffer, size_t page_size, vector<string> & new_urls )
{
	/* 提取主机名字，已便于将相对url转换为绝对url*/
	string host_name = get_host_name( url );
	
	/* 游标 */
	const char * current = buffer;
	
	while ( current = strstr( current, "<a href=\"" ) )
	{
		/* 截取当前url */
		
		string url = pick_url( current + 9 );
		
		//cout<<url<<endl;
		
		current = current + 9 + url.size();
		
		if ( is_root_path(url.c_str()) )  new_urls.push_back( url );
		else
		{
			new_urls.push_back( "http://" + host_name + url);
		}
	}

}

void test_main_util(int argc, char const* argv[])
{
	/*测试fetch_url
	cout << "Test: test_main_util" << endl;
	
	char haha[1024*1024*3];
	
	haha[fetch_url(haha,argv[1])] = '\0';
	
	cout << haha << endl;*/
	
	cout<<get_host_name("http://haha.com/")<<endl;
	cout<<get_host_name("http://haha.com")<<endl;
	cout<<get_host_name("haha.com/")<<endl;
	cout<<get_host_name("/haha.com/cdad")<<endl;
	cout<<get_host_name("haha.com")<<endl;
	cout<<get_host_name("haha.com/")<<endl;
	
	cout<<is_root_path("http://haha.com/")<<endl;
	cout<<is_root_path("/haha.com/")<<endl;
	cout<<is_root_path("haha.com/")<<endl;
	cout<<is_root_path("haha.com/")<<endl;
	
}





























