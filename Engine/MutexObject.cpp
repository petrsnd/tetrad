#ifdef _MSC_VER
#include "stdafx.h"
#endif
#include "MutexObject.h"


namespace Implbits {

MutexObject::MutexObject()
{
#ifdef _MSC_VER
    m_hMutex = CreateMutex( NULL, FALSE, NULL );
#elif __APPLE__
    m_Mutex = PTHREAD_MUTEX_INITIALIZER;
#else
    // TODO: other platforms
#endif
}
MutexObject::~MutexObject()
{
#ifdef _MSC_VER
    CloseHandle( m_hMutex );
#elif __APPLE__
#else
    // TODO: other platforms
#endif
}
void MutexObject::Lock()
{
#ifdef _MSC_VER
    WaitForSingleObject( m_hMutex, INFINITE );
#elif __APPLE__
    pthread_mutex_lock( &m_Mutex );
#else
    // TODO: other platforms
#endif
}
void MutexObject::Unlock()
{
#ifdef _MSC_VER
    ReleaseMutex( m_hMutex );
#elif __APPLE__
    pthread_mutex_unlock( &m_Mutex );
#else
    // TODO: other platforms
#endif
}

} // Implbits
