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
static const size_t thread_numbers = 10;

/* 高级配置 */
/* 32位最大512MB */
static const size_t bloom_filter_size = 100 * 1024 * 1024;

static const size_t PAGE_BUFF_SIZE = 1024*1024*5;

static const string forbidden[] = 
{
".tar",".gz",".tgz",".zip",".Z",".rpm",".deb"
,".ps",".dvi",".pdf"
," .png",".jpg",".jpeg",".bmp",".smi",".tiff",".gif"
,".mov",".avi"," .mpeg", ".mpg" ,".mp3" ,".qt" ,".wav" ,".ram" ,".rm"
,".jar", ".java", ".class", ".diff"
,".doc", ".xls", ".ppt" ,".mdb" ,".rtf", ".exe", ".pps", ".so", ".psd"
};
#endif
