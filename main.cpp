#include <curl/curl.h>
#include <stdlib.h>
#include <queue>
#include <string>
#include <fstream>
#include <iostream>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include "urls.h"
#include "util.h"
#include "config.h"
using namespace std;

/* 子线程的ID数组 */
pthread_t child_threads[thread_numbers];

/* 程序需要使用所有互斥锁 */
pthread_mutex_t total_pages_lock;
pthread_mutex_t url_queue_lock;
pthread_mutex_t mDB_lock;

/* 初始化所有互斥锁 */
void init_all_lock()
{
	if ( pthread_mutex_init( &total_pages_lock, NULL ) )
		cout << "Init lock fail.." << endl;

	if ( pthread_mutex_init( &url_queue_lock, NULL ) )
		cout << "Init lock fail.." << endl;

	if ( pthread_mutex_init( &mDB_lock, NULL ) )
		cout << "Init lock fail.." << endl;
}

/* 统计信息 */
size_t total_pages = 0;
/* 获得目前抓取到总页面数（也作为保存页面的文件名字）*/
size_t get_and_inc()
{
	pthread_mutex_lock( &total_pages_lock );

	size_t ret = total_pages++;

	pthread_mutex_unlock( &total_pages_lock );

	return ret;
}

void * worker ( void * p )
{
	MYDEBUG("start a child thread");

	char * buffer = (char*)malloc( PAGE_BUFF_SIZE );
	
	if ( buffer == NULL )
		MYDEBUG("malloc ERROR!");
		
	CURL * hander = curl_easy_init();

	while ( 1 )
	{
		/* 获得一个待抓取的url */
		pthread_mutex_lock( &url_queue_lock );
		string curl = url_get_url();
		pthread_mutex_unlock( &url_queue_lock );

		if ( curl.size() == 0 ) continue;
		
		sleep(1);

		/* 抓取当前url */
		size_t page_size = fetch_url( buffer, curl.c_str(), hander );

		if ( page_size == 0 )
		{	
			continue;
		}

		buffer[page_size] = '\0';

		/* 保存页面 */
		size_t number = get_and_inc();

		save_page( curl, buffer, number );

		/* 分析页面 */
		vector<string> new_urls; 
		
		parse_page( curl.c_str(), buffer, page_size, new_urls );

		/* 将urls加入到全局队列，这里使用url_queue_lock。 */
		pthread_mutex_lock( &url_queue_lock );
		url_put_urls ( new_urls );
		pthread_mutex_unlock( &url_queue_lock );
	}

	free( buffer );
}

int main (int argc, char const* argv[])
{
	
	/* 初始化锁 */
	init_all_lock();

	MYDEBUG("init lock done");

	vector< string > seeds;
	seeds.push_back( seed_url );
	url_put_urls ( seeds );

	/* 启动worker线程们 */	
	for ( size_t i = 0; i < thread_numbers; i++ )
	{
		int err = pthread_create ( child_threads + i, NULL, worker, NULL );
		if ( err )
		{
			cout << "Can not create thread: " << strerror( err ) 
			<< endl;
			return 1;
		}
	}

	MYDEBUG("create threads done");

	/* 等待所有子进程结束 */
	void * tret;
	for ( size_t i = 0; i < thread_numbers; i++ )
	{
		pthread_join( child_threads[i], &tret );
	}

	return 0;
}



































	
