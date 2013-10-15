#include <curl/curl.h>
#include <queue>
#include <string>
#include <fstream>
#include <iostream>

#include "util.h"

using namespace std;

/* config */
const char * seed_url = "hao.360.cn";
char * save_file_prefix = "../save/page";

/* implement config */
char buffer[1024*1024*20];

int main (int argc, char const* argv[])
{
	/* 待抓取的url队列 */
	queue<string> url_queue;
	
	/* 将种子url放到队列中 */
	url_queue.push( seed_url );

	/* 统计信息 */
	size_t total_pages = 0;
	
	while ( !url_queue.empty() )
	{

		/* 当前抓取的url */
		string c_url_str = url_queue.front();
		const char * c_url = c_url_str.c_str();
		url_queue.pop();
		/* 抓取 */
		cout << "fetching... " << c_url <<"\n";
		size_t page_size = fetch_url(buffer,c_url);
		buffer[page_size] = '\0';
		
		/* 失败 */
		if( page_size == 0 ) 
		{ 
			cout << "fail.."<<endl;
			continue; 
		}
		else
		{
			cout << "succeed.."<<endl;
		}
		
		/* 保存page到文件中去 */
		char filename[100];
		sprintf( filename, "%s%u", save_file_prefix, total_pages++ );
		ofstream file( filename );
		file << c_url << "\n" << buffer;
		
		/* 分析页面，获得新的url */
		vector<string> new_urls;
		parse_page( c_url, buffer, page_size, new_urls );

		/* 将新的url加入到队列尾部 */
		for (size_t i = 0; i < new_urls.size(); i++ )
		{
			url_queue.push( new_urls[i] );
		}
		
	}
	
	return 0;
}




































	
