#ifndef __BLOOM_FILTER_H__
#define __BLOOM_FILTER_H__

#include <string>
#include <list>
#include <boost/utility.hpp>

using std::list;
using std::string;

/* 主要用于URL的判重复（近似算法） */
class BloomFilter : boost::noncopyable
{

public:
	BloomFilter ( size_t size );

	/* 判断一个url是否存在  */
	bool isExist (const string & str );

	/* 向bloom filter中添加一个字符串 */
	void insert (const string & str  );
private:

	/* 计算一个字符串的K个hash值  */
        void hash_bloom_str (const string & str, size_t * index);

	/* 将内存中的第index位至1  */
	inline void set_bit_index( size_t index );

	/* 判断内存中的第index位至0 */
	inline bool test_bit_index ( size_t index );

	/* bloom filter的内存 */
	char * __p_vec;

	/* __p_vec的bit大小 */
	size_t __size;

};


void test_main_bloom();
#endif /* __BLOOM_FILTER_H__ */
