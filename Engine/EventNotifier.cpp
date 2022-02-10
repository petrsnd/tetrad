#ifdef _MSC_VER
#include "stdafx.h"
#include "GuidCreator.h"
#endif
#include "EventNotifier.h"


namespace Implbits {

EventNotifier::EventNotifier()
{
#ifdef _MSC_VER
    std::string name = "Local\\" + GuidCreator::Instance()->NewGuid();
    m_hEvent = CreateEvent( NULL, TRUE, FALSE, name.c_str() );
#elif __APPLE__
    m_EventMutex = PTHREAD_MUTEX_INITIALIZER;
    m_EventCondition = PTHREAD_COND_INITIALIZER;
#else
    // TODO: other platforms
#endif
}
EventNotifier::~EventNotifier()
{
#ifdef _MSC_VER
    CloseHandle( m_hEvent );
#elif __APPLE__
#else
    // TODO: other platforms
#endif
}
void EventNotifier::WaitForView()
{
#ifdef _MSC_VER
    ResetEvent( m_hEvent );
    WaitForSingleObject( m_hEvent, INFINITE );
#elif __APPLE__
    pthread_mutex_lock( &m_EventMutex );
    pthread_cond_wait( &m_EventCondition, &m_EventMutex );
    pthread_mutex_unlock( &m_EventMutex );
#else
    // TODO: other platforms
#endif
}
void EventNotifier::SetViewFinished()
{
#ifdef _MSC_VER
    SetEvent( m_hEvent );
#elif __APPLE__
    pthread_mutex_lock( &m_EventMutex );
    pthread_cond_signal( &m_EventCondition );
    pthread_mutex_unlock( &m_EventMutex );
#else
    // TODO: other platforms
#endif
}

} // Implbits
