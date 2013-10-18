#include <queue>
#include <string>
#include <iostream>
#include "urls.h"
#include "util.h"
using namespace std;
typedef map < string, size_t > URLDB;

/* 待抓取的url队列 */
queue<string> url_queue;

/* 保存目前抓取的所有url */
static URLDB mDB;

/*
    获得一个待抓取的url
 */
string url_get_url()
{
	if ( url_queue.empty() == true )
		return string();

	string url = url_queue.front();
	url_queue.pop();

	return url;
}

bool url_filter ( string url )
{
	size_t pos =  url.rfind( "." );
	
	if ( pos == url.npos )
		return false;
	
	string subfix = url.substr( pos, -1 );
	
	for ( size_t i = 0; i < sizeof(forbidden) / sizeof(forbidden[0]); i++ )
	{
		if ( subfix == forbidden[i] )
			return true;
	}

	return false;
}

/* 
    如果这个url没有抓取过，那么将它加入到待抓队列中，并将其至为已经抓取的状态。

    如果这个url已经抓取过了，那么只是简单的增加它的计数。
 */
void url_put_url( string url )
{
	/* 过滤掉一些特定后缀的url */
	if ( url_filter( url ) )
		return;

	if ( url_exist( url ) == false )
        {
                url_queue.push( url );
                url_add( url );
        }	
}

void url_put_urls( vector< string > & urls)
{
	MYDEBUG("begin put urls");
	for ( size_t i = 0; i < urls.size(); i++ )
	{
		url_put_url( urls[i] );
	}

	MYDEBUG("end put urls");
}

bool url_exist(string url)
{
	if ( mDB.find(url) == mDB.end() )
		return false;
	else
		return true;
}

bool url_add(string url)
{
	return mDB.insert( URLDB::value_type( url, 1) ).second;
}

void test_main_urls()
{
	cout << url_filter( "http://baidu.daa.mp3" ) <<endl;
	cout << url_filter( "http://baidu.daa.txt" ) <<endl;
	cout << url_filter( "http://baidu.daa.html" ) <<endl;
	cout << url_filter( "http://baidu.daa.jpg" ) <<endl;
	cout << url_filter( "http://baidu.daa.rar" ) <<endl;
	cout << url_filter( "http://baidu.daa.exee" ) <<endl;
	cout << url_filter( "http://baidu.daa.p3" ) <<endl;
}
