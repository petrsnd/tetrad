#pragma once
#ifdef _MSC_VER
#include <Windows.h>
#elif __APPLE__
#include <pthread.h>
#else
// TODO: other platforms
#endif


namespace Implbits {

class EventNotifier
{
public:
    EventNotifier();
    ~EventNotifier();
    void WaitForView();
    void SetViewFinished();
private:
#ifdef _MSC_VER
    HANDLE m_hEvent;
#elif
    pthread_mutex_t m_EventMutex;
    pthread_cond_t m_EventCondition;
#else
    // TODO: other platforms
#endif
};

} // Implbits
