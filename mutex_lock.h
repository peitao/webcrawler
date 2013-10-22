#ifndef __MUTEX_LOCK_H__
#define __MUTEX_LOCK_H__

#include <boost/utility.hpp>

class MutexLock : boost::noncopyable
{
public:
	MutexLock()
	{	
		pthread_mutex_init( &_mutex ,NULL );
	}
	~MutexLock()
	{
		pthread_mutex_destroy( &_mutex );
	}

	/* 由MutexLockGuard调用，严禁用户代码调用 */
	void lock()
	{
		pthread_mutex_lock( &_mutex );
	}

	/* 由MutexLockGuard调用，严禁用户代码调用 */
	void unlock()
	{
		pthread_mutex_unlock( &_mutex );
	}

private:
	pthread_mutex_t _mutex;
};

class MutexLockGuard : boost::noncopyable
{
public:
	explicit MutexLockGuard(MutexLock & mutex): _mutex(mutex)
	{
		_mutex.lock();
	}

	~MutexLockGuard()
	{
		_mutex.unlock();
	}
private:
	MutexLock & _mutex;
};

#define MutexLockGuard(x) static_assert( false , "miss mutex guard var name" )

#endif /* __MUTEX_LOCK_H__ */
