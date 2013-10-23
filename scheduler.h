#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <string>
#include <vector>
#include <map>
#include <queue>
#include "mutex_lock.h"
#include "bloom_filter.h"
#include "urls.h"
using std::string;
using std::vector;
using std::map;
using std::queue;

class Scheduler
{
public:

	void add_urls( vector<string>& urls );

	string get_url();

	Scheduler():_c_index(0),_total_urls(0),_mBloom(bloom_filter_size){}
private:

	void inc_index();
	/* 保存当前抓取的位置 */	
	size_t _c_index;	
	size_t _total_urls;

	size_t find_or_add( const string & host);	

	/* 以主机为维度组织的URL 队列列表 */
	vector < queue<string> > _urldb;
	vector < string > _hostdb;

	/* 建立主机url到_urldb的索引，便于插入url时查找主机所在位置 */
	map < string, size_t >   _hostmap;
	
	BloomFilter _mBloom;

	/* 互斥锁 */
	MutexLock _mutex;

};


#endif /* __SCHEDULER_H__ */
