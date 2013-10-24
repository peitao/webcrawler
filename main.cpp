#include <curl/curl.h>
#include <queue>
#include <string>
#include <fstream>
#include <iostream>

#include "muti_curl_adapter.h"
#include "urls.h"
#include "util.h"
using namespace std;

/* config */
const char * seed_url = "hao.360.cn";
char * save_file_prefix = "../save/page";

const size_t MAX_HANDLER = 200;
/* 最大并发数的缓冲池 */
BuffHandler mbuffer_in[MAX_HANDLER];

/* 待抓取的url队列 */
queue<string> url_queue;

/* 统计信息 */
size_t total_pages = 0;

void fill_and_add_handler(queue<string> & que, CURLM * muti)
{
	if ( que.empty() == true ) 
	{
		cout << "queue is empty"<<endl;
		return;
	}
	size_t count = 0;
	for (size_t i = 0; i < MAX_HANDLER; i++)
	{
		if ( mbuffer_in[i].handler == NULL ) 
		{
			string url = que.front();
			mbuffer_in[i].handler = new_a_handler( url, mbuffer_in + i );
			mbuffer_in[i].size = 0;
			mbuffer_in[i].url = url;
			curl_multi_add_handle( muti, mbuffer_in[i].handler);
			count++;
			que.pop();
			if ( que.empty() == true )
			{
			cout << "queue is empty"<<endl;
			return;
			}
		}
	}
	cout << "add "<<count<<" handler"<<endl;
}
void main_a_page(BuffHandler * pbuf)
{
	
	cout <<"saving[size="<<pbuf->size<<"] "<<pbuf->url <<endl;
	pbuf->data_buffer[pbuf->size]='\0';

	/* 保存page到文件中去 */
	char filename[100];
	sprintf( filename, "%s%u", save_file_prefix, total_pages++ );
	ofstream file( filename );
	file << pbuf->url << "\n" << pbuf->data_buffer;

	/* 分析页面，获得新的url */
	vector<string> new_urls;
	parse_page( pbuf->url.c_str(), pbuf->data_buffer, pbuf->size, new_urls );

	cout <<"parse done.. " <<pbuf->url <<endl;
	/* 将新的url加入到队列尾部 */
	for (size_t i = 0; i < new_urls.size(); i++ )
	{
		/* 判断url是否重复 */
		if ( url_exist( new_urls[i] ) == false )
		{
			url_queue.push( new_urls[i] );
			url_add( new_urls[i] );
		}
	}
	cout<<"-------queue size "<< url_queue.size()<<endl;

	curl_easy_cleanup ( pbuf->handler );
	pbuf->handler = NULL;
	pbuf->size = 0;

}
int main (int argc, char const* argv[])
{
	/* 将种子url放到队列中 */
	url_queue.push( seed_url );

	int still_running;

	curl_global_init(CURL_GLOBAL_DEFAULT);
	
 	CURLM * multi_handle = curl_multi_init();

	fill_and_add_handler( url_queue , multi_handle );

  	curl_multi_perform(multi_handle, &still_running);
	
	do {
	
    		struct timeval timeout;
    		int rc; /* select() return code */

    		fd_set fdread;
    		fd_set fdwrite;
    		fd_set fdexcep;
    		int maxfd = -1;

    		long curl_timeo = -1;

    		FD_ZERO(&fdread);
    		FD_ZERO(&fdwrite);
    		FD_ZERO(&fdexcep);

    		/* set a suitable timeout to play around with */
    		timeout.tv_sec = 1;
    		timeout.tv_usec = 0;

    		curl_multi_timeout(multi_handle, &curl_timeo);
    		if(curl_timeo >= 0) {
      			timeout.tv_sec = curl_timeo / 1000;
      			if(timeout.tv_sec > 1)
        			timeout.tv_sec = 1;
      			else
        			timeout.tv_usec = (curl_timeo % 1000) * 1000;
    		}

    		/* get file descriptors from the transfers */

    		curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);

    		rc = select(maxfd+1, &fdread, &fdwrite, &fdexcep, &timeout);
		
    		switch(rc) 
		{
    		case -1:
      			/* select error */
     			still_running = 0;
      			printf("select() returns error, this is badness\n");
      			return 0;
    		case 0:
    		default:
      			/* timeout or readable/writable sockets */
      			curl_multi_perform(multi_handle, &still_running);
      			break;
		}
		/* 这里开始处理完成的page，并移除完成的url，加入新的url */
		CURLMsg * msg = NULL;
		int msgs;
		while( (msg = curl_multi_info_read( multi_handle, &msgs) ) )
		{
			BuffHandler * pbuf;
			if ( msg->msg == CURLMSG_DONE )
			{
				curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &pbuf);
				curl_multi_remove_handle( multi_handle, msg->easy_handle );

				main_a_page( pbuf );
			}
			else 
			{
				printf("msg err\n");
			}
		}
		fill_and_add_handler(url_queue, multi_handle );

	} while( 1 );	
	
	return 0;//一会删了下面
}
