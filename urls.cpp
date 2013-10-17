#include <queue>
#include <string>
#include "urls.h"
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

/* 
    如果这个url没有抓取过，那么将它加入到待抓队列中，并将其至为已经抓取的状态。

    如果这个url已经抓取过了，那么只是简单的增加它的计数。
 */
void url_put_url( string url )
{
	if ( url_exist( url ) == false )
        {
                url_queue.push( url );
                url_add( url );
        }	
}

void url_put_urls( vector< string > & urls)
{
	for ( size_t i = 0; i < urls.size(); i++ )
	{
		url_put_url( urls[i] );
	}
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
