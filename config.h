#ifndef __CONFIG_H
#define __CONFIG_H

#include <string>
using std::string;
/* config */

/* 开始抓取到种子 */
static const string  seed_url = "hao.360.cn";

/* 保存抓取到文件的 */
static const char * save_file_prefix = "../save/page";

/* 启动线程的个数 */
static const size_t thread_numbers = 200;

/* 高级配置 */
static const size_t PAGE_BUFF_SIZE = 1024*1024*5;

#endif
