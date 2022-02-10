#pragma once

#include "TsPosition.h"
#include "TsTile.h"

#include <Engine/Interfaces.h>

namespace TsWinRT
{

public ref class TsTurnCallback sealed
{
internal:
    explicit TsTurnCallback( const Implbits::ITurnCallback::Ptr& callback );
public:
    virtual ~TsTurnCallback();
    void LocalTurnComplete( TsTile^ tile, TsPosition^ target );
    void AnimationComplete();
    void GetRemoteGameComplete( Platform::String^ payload );
    void PutRemoteGameComplete();
    void DeleteRemoteGameComplete();
private:
    Implbits::ITurnCallback::Ptr m_callback;
};

} // TsWinRT
