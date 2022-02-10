#include "pch.h"
#include "TsTurnCallback.h"
#include "TsConvert.h"
#include "WinRTUtil.h"

namespace TsWinRT
{

TsTurnCallback::TsTurnCallback( const Implbits::ITurnCallback::Ptr& callback ) :
    m_callback( callback )
{}
TsTurnCallback::~TsTurnCallback()
{}
void TsTurnCallback::LocalTurnComplete( TsTile^ tile, TsPosition^ target )
{
    m_callback->LocalTurnComplete( tile->GetPtr(), Implbits::Position( target->X, target->Y ) );
}
void TsTurnCallback::AnimationComplete()
{
    m_callback->AnimationComplete();
}
void TsTurnCallback::GetRemoteGameComplete( Platform::String^ payload )
{
    m_callback->GetRemoteGameComplete( ConvertPlatformStringToStdString( payload ) );
}
void TsTurnCallback::PutRemoteGameComplete()
{
    m_callback->AnimationComplete();
}
void TsTurnCallback::DeleteRemoteGameComplete()
{
    m_callback->AnimationComplete();
}

} // TsWinRT
