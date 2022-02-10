#pragma once

#include "TsDefinitions.h"

#include <Engine/Interfaces.h>

namespace TsWinRT
{

public interface class ITsPlayer
{
public:
    virtual property TsPlayerType Type;
    virtual property Platform::String^ Identity;
    virtual property Platform::String^ Name;
    virtual property TsPlayerTeam Team;
    virtual property int Color;
    virtual bool HasTetrad( TsTetradShape shape );
    virtual int GetNumberOfMoves();
};

public ref class TsPlayerSpec sealed : public ITsPlayer
{
public:
    TsPlayerSpec();
    virtual ~TsPlayerSpec();
    virtual property TsPlayerType Type;
    virtual property Platform::String^ Identity;
    virtual property Platform::String^ Name;
    virtual property TsPlayerTeam Team;
    virtual property int Color;
    virtual bool HasTetrad( TsTetradShape shape );
    virtual int GetNumberOfMoves();
};

public ref class TsPlayer sealed : public ITsPlayer 
{
internal:
    explicit TsPlayer( const Implbits::IPlayer::Ptr& player );
public:
    virtual ~TsPlayer();
    virtual property TsPlayerType Type
    {
        TsPlayerType get();
        void set( TsPlayerType type );
    }
    virtual property Platform::String^ Identity
    {
        Platform::String^ get();
        void set( Platform::String^ id );
    }
    virtual property Platform::String^ Name
    {
        Platform::String^ get();
        void set( Platform::String^ name );
    }
    virtual property TsPlayerTeam Team
    {
        TsPlayerTeam get();
        void set( TsPlayerTeam team );
    }
    virtual property int Color
    {
        int get();
        void set( int color );
    }
    virtual bool HasTetrad( TsTetradShape shape );
    virtual int GetNumberOfMoves();
private:
    Implbits::IPlayer::Ptr m_player;
};

} // TsWinRT
