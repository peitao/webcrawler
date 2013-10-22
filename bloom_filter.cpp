#include <cstdio>
#include <bitset>
#include <iostream>
#include <cstdlib>
#include "bloom_filter.h"
#include "util.h"
using namespace std;

/* bloom filter中hash函数的个数和hash的种子 */
static const int K_HASH = 10;
static const size_t K_HASH_SEED [] = {31,131,1313,13131,131313,1313131,13131313,131313131,1313131313,2};

/* 构造一个BloomFilter，其中size是bloom filter的内存大小 */
BloomFilter::BloomFilter( size_t size )
{
	/* 如果size过小，提示警告 */
	if ( size < 1024 * 1024 )
		cout <<	"Warning: bloom filter size is too small <" << size << ">" <<endl;
	
	/* 分配内存 */
	__p_vec = (char *) malloc (size);

	if ( ! __p_vec )
		cout << "Err: malloc in bloom filter" << endl;

	/*  保存内存中总的bit数量 */
	__size = size * 8;

}

/* 向bloom filter中添加一个字符串 */
void BloomFilter::insert ( const string &  str )
{
	/* 保存K个hash计算出来到结果  */
	size_t index[K_HASH];

	/* 计算K个hash值  */
	hash_bloom_str ( str, index );

	/* 每个hash值至位  */
	for ( size_t i = 0; i < K_HASH; i++ )
	{
		set_bit_index( index[i] );
	}
}

/* 计算一个字符串的K个hash值  */
void BloomFilter::hash_bloom_str (const string & str, size_t * index)
{
	for ( size_t i = 0; i < K_HASH ; i++ )
	{
		* index ++ = str_hash( str.c_str(), K_HASH_SEED[i] ) % __size ;
	}
}

/* 将内存中的第index位至1  */
void BloomFilter::set_bit_index( size_t index )
{
	__p_vec[index / 8] |=  1 << ( 7 - index % 8 ) ;
}

/* 判断内存中的第index位至0 */
bool BloomFilter::test_bit_index ( size_t index )
{
	char tmp = __p_vec[index / 8] | ( 1 << ( 7 - index % 8 ) ) ;

	return __p_vec[index / 8] == tmp;
}

/* 判断一个字符串是否在bloom filter中，这个函数由于bloom filter的特性，存在一定误判 */
bool BloomFilter::isExist ( const string & str)
{
	for (size_t i = 0; i < K_HASH; i++ )
	{
		/* 计算第i个hash值 */
		size_t index = str_hash ( str.c_str(), K_HASH_SEED[i] ) % __size;

		/* 如果某一位不是1，则一定不存在，返回false */
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
