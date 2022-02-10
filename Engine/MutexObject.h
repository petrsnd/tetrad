#pragma once
#ifdef _MSC_VER
#include <Windows.h>
#elif __APPLE__
#include <pthread.h>
#else
// TODO: other platforms
#endif


namespace Implbits {

class MutexObject
{
public:
    MutexObject();
    ~MutexObject();
    void Lock();
    void Unlock();
private:
#ifdef _MSC_VER
    HANDLE m_hMutex;
#elif __APPLE__
    pthread_mutex_t m_Mutex;
#else
    // TODO: other platforms
#endif
};

} // Implbits
