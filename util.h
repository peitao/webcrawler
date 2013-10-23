#ifndef __UTIL_H__
#define __UTIL_H__
#include <iostream>
#include <curl/curl.h>
#include <vector>
#include <string>
#include "config.h"
using std::vector;
using std::string;

/* 读取一个URL到data_buffer，返回读取的字节数 */
size_t fetch_url( void * data_buffer, const char * url, CURL * curl );

/* 分析一个页面中的url */
void parse_page(const char * url, const char * buffer, size_t page_size, vector<string> & new_urls );

/* 把一个buffer保存到文件中 */
void save_page( string curl, char * buffer, size_t file_no );

/* hash函数 */
size_t str_hash(const char * str, size_t seed );

string get_host_name(const char * url);
string get_relative_name(const char * url);

void test_main_util(int argc, char const* argv[]);

/* Debug 函数 */
void debug(string str);
#ifdef DEBUG
#define TRACE(x)  TraceDebug _debug__(x);
#endif

#ifndef DEBUG
#define TRACE(x) 
#endif

class TraceDebug
{
public:
	TraceDebug( string name  ):_name(name){ std::cerr << "Begin " << _name << std::endl; }
	~TraceDebug() { std::cerr << "End " << _name << std::endl; }
	string _name;
};
#define MYDEBUG(c)  debug(c)

#endif /* __UTIL_H__ */

