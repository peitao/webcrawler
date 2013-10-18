#ifndef __TESTTIME_H__
#define __TESTTIME_H__

#include <sys/time.h>
#include <iostream>
#include <string>
using std::string;
class testtime
{
	struct timeval startTime,endTime;
	float nTime;
	string name;
	public:
	testtime(const string & pname)
	{
		gettimeofday(&startTime,NULL); 
		name = pname;
	}
	~testtime()
	{
		gettimeofday(&endTime,NULL);

		nTime = 1000000*(endTime.tv_sec-startTime.tv_sec)+

		endTime.tv_usec-startTime.tv_usec;

		nTime/=1000;	

		std::cerr<<"\n\t"<<name<<" use time:"<<nTime<<"ms"<<std::endl;
	}
};

#endif /* __TESTTIME_H__ */



