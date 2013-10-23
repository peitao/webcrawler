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
#include "scheduler.h"
#include "mutex_lock.h"
#include "util.h"
#include "config.h"
using namespace std;

/* 子线程的ID数组 */
pthread_t child_threads[thread_numbers];

/* 程序需要使用所有互斥锁 */
MutexLock total_pages_lock;

/* 统计信息 */
size_t total_pages = 0;

Scheduler scheduler;

/* 获得目前抓取到总页面数（也作为保存页面的文件名字）*/
size_t get_and_inc()
{
	MutexLockGuard lock( total_pages_lock );

	size_t ret = total_pages++;

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
		//pthread_mutex_lock( &url_queue_lock );
		//string curl = url_get_url();
		//pthread_mutex_unlock( &url_queue_lock );
		string curl = scheduler.get_url();
		
		if ( curl.size() == 0 ) 
		{
			sleep(2);
		}		

		if ( curl.size() == 0 ) continue;
		
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
		//pthread_mutex_lock( &url_queue_lock );
		//url_put_urls ( new_urls );
		//pthread_mutex_unlock( &url_queue_lock );
		scheduler.add_urls( new_urls );
	}

	free( buffer );
}

int main (int argc, char const* argv[])
{
	
	MYDEBUG("init lock done");

	vector< string > seeds;
	seeds.push_back( seed_url );
	//url_put_urls ( seeds );
	scheduler.add_urls( seeds );	
	curl_global_init(CURL_GLOBAL_ALL);

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



































	
