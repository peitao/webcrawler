#ifndef __BLOOM_FILTER_H__
#define __BLOOM_FILTER_H__

#include <string>
#include <list>
using std::list;
using std::string;
/* 主要用于URL的判重复（近似算法） */

class BloomFilter
{
public:
	BloomFilter ( size_t size );

	/* 判断一个url是否存在  */
	bool isExist (const string & str );

	void insert (const string & str  );
private:

        void hash_bloom_str (const string & str, size_t * index);
	inline void set_bit_index( size_t index );
	inline bool test_bit_index ( size_t index );
	/* 不允许复制和拷贝 */
	BloomFilter( const BloomFilter & );
	BloomFilter & operator = ( const BloomFilter & );

	/* bloom filter的内存 */
	char * __p_vec;

	/* __p_vec的bit大小 */
	size_t __size;

};


void test_main_bloom();
#endif /* __BLOOM_FILTER_H__ */
