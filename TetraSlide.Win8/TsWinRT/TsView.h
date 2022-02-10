#pragma once

#include "TsDefinitions.h"
#include "TsTile.h"
#include "TsPlayer.h"
#include "TsGameDescriptor.h"
#include "TsTurnCallback.h"

#include <collection.h>

namespace TsWinRT
{

public interface class ITsView
{
public:
    virtual void GameResumed( TsGameDescriptor^ game, TsPlayerTeam turn );
    virtual void GameSuspended( TsGameDescriptor^ game );
    virtual void GameOver( ITsPlayer^ winner );
    virtual void StateChanged();
    virtual void TetradFound( TsTurnCallback^ callback, ITsPlayer^ player, TsTetradShape shape, Windows::Foundation::Collections::IVector< TsTile^ >^ tiles );
    virtual void DoLocalTurn( TsTurnCallback^ callback );
    virtual void AnimateTurn( TsTurnCallback^ callback, Windows::Foundation::Collections::IVector< TsTile^ >^ tiles, TsBoardDirection dir, int dist );
    virtual void Log( Platform::String^ msg, Platform::String^ file, int line );
    virtual Windows::Foundation::Collections::IVector< Platform::String^ >^ GetRemoteGames();
    virtual Platform::String^ NewRemoteGame( Platform::String^ localId, Platform::String^ remoteId );
    virtual void GetRemoteGame( TsTurnCallback^ callback, Platform::String^ gameId );
    virtual void PutRemoteGame( TsTurnCallback^ callback, Platform::String^ gameId, Platform::String^ payload );
    virtual void DeleteRemoteGame( TsTurnCallback^ callback, Platform::String^ gameId );
};

} // TsWinRT
