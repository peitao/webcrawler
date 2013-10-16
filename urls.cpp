#include "urls.h"

typedef map < string, size_t > URLDB;
static URLDB mDB;

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
