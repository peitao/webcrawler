#include "scheduler.h"
#include "util.h"

/* 添加URL */
void Scheduler::add_urls( vector < string > & urls )
{
	MutexLockGuard lock(_mutex);
	
	TRACE("add_url");

	for ( size_t i = 0; i < urls.size(); i++ ) 
	{
		
		string host = get_host_name ( urls[i].c_str() );
		
		size_t index = find_or_add( host );

		string path = get_relative_name ( urls[i].c_str() );
	
		_urldb[index].push( path );

		_total_urls ++;

	}

}

void Scheduler::inc_index()
{
	if ( ++ _c_index > _hostdb.size() )
		_c_index = 0;
}

/* 返回下一个待抓取的url，这里有可能死循环。不过目前没关系 - - */
string Scheduler::get_url()
{
	MutexLockGuard lock(_mutex);

	if ( _total_urls == 0 ) return string("");

	TRACE("get_url");

	while ( _urldb[_c_index].empty() )  inc_index();

	string url = "http://" + _hostdb[_c_index] + _urldb[_c_index].front();

	MYDEBUG(url);

	_urldb[_c_index].pop();

	inc_index();

	_total_urls --;
	return url;

}
/* 返回一个主机所在的index，如果没有则新建一个主机，返回其index */
size_t Scheduler::find_or_add( const string & host)
{
	map < string,size_t >::iterator i = _hostmap.find ( host );

	if ( i != _hostmap.end() )
		return i->second;
	else
	{
		size_t index = _urldb.size();

		_urldb.push_back( queue<string>() );

		_hostdb.push_back( host );
		
		_hostmap.insert( map < string, size_t >::value_type( host, index ) );

		return index;
	}
}
