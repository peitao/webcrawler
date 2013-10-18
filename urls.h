#ifndef __URLS_H__
#define __URLS_H__
#include <string>
#include <map>
#include <vector>
#include "config.h"
using std::string;
using std::map;
using std::vector;
bool url_exist(string url);

bool url_add(string url);

string url_get_url();

void url_put_urls( vector< string > & urls);

void test_main_urls();
#endif /* __URLS_H__ */
