#include <cstdio>
#include <bitset>
#include <iostream>
#include <cstdlib>
#include "bloom_filter.h"
#include "util.h"
using namespace std;

static const int K_HASH = 10;
static const size_t K_HASH_SEED [] = {31,131,1313,13131,131313,1313131,13131313,131313131,1313131313,2};
/* 构造一个BloomFilter */
BloomFilter::BloomFilter( size_t size )
{
	if ( size < 1024 * 1024 )
		cout <<	"Warning: bloom filter size is too small <" << size << ">" <<endl;

	__p_vec = (char *) malloc (size);

	if ( ! __p_vec )
		cout << "Err: malloc in bloom filter" << endl;

	__size = size * 8;

}

void BloomFilter::insert ( const string &  str )
{
	size_t index[K_HASH];
	hash_bloom_str ( str, index );
	for ( size_t i = 0; i < K_HASH; i++ )
	{
		set_bit_index( index[i] );
	}
}

void BloomFilter::hash_bloom_str (const string & str, size_t * index)
{
	for ( size_t i = 0; i < K_HASH ; i++ )
	{
		* index ++ = str_hash( str.c_str(), K_HASH_SEED[i] ) % __size ;
	}
}
void BloomFilter::set_bit_index( size_t index )
{
	__p_vec[index / 8] |=  1 << ( 7 - index % 8 ) ;
}
bool BloomFilter::test_bit_index ( size_t index )
{
	char tmp = __p_vec[index / 8] | ( 1 << ( 7 - index % 8 ) ) ;
	return __p_vec[index / 8] == tmp;
}
bool BloomFilter::isExist ( const string & str)
{
	for (size_t i = 0; i < K_HASH; i++ )
	{
		size_t index = str_hash ( str.c_str(), K_HASH_SEED[i] ) % __size;
		if ( test_bit_index ( index ) == false )
			return false;
	}
	return true;
}

void test_main_bloom()
{
	cout << str_hash("http://baidu.com",131)<<endl;
	cout << str_hash("http://baidu.com/haha",1313)<<endl;
	BloomFilter bloom(bloom_filter_size);
	bloom.insert("http://baidu.com");
	bloom.insert("http://163.com");
	bloom.insert("http://63.com");
	cout << bloom.isExist("http://baidu.com") <<endl;
	cout << bloom.isExist("http://163.com") <<endl;
	cout << bloom.isExist("http://63.com") <<endl;
	cout << bloom.isExist("http:63.com") <<endl;
	cout << bloom.isExist("htt//63.com") <<endl;
	cout << bloom.isExist("htt//63.com") <<endl;
	cout << bloom.isExist("htt//63.com") <<endl;
	cout << bloom.isExist("htt63.com") <<endl;
}
