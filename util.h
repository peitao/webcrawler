#ifndef __UTIL_H__
#define __UTIL_H__

#include <vector>
#include <string>

using std::vector;
using std::string;

/* 读取一个URL到data_buffer，返回读取的字节数 */
size_t fetch_url( void * data_buffer, const char * url );

/* 分析一个页面中的url */
void parse_page(const char * url, const char * buffer, size_t page_size, vector<string> & new_urls );

void test_main_util(int argc, char const* argv[]);


#endif /* __UTIL_H__ */

