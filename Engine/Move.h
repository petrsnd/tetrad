#pragma once
#include "Exceptions.h"
#include "Definitions.h"
#include "JsonSerializable.h"

#include <memory>
#include <vector>


namespace Implbits {

class IMoveResult : public IJsonSerializable
{
public:
    typedef std::shared_ptr< IMoveResult > Ptr;
    virtual ~IMoveResult() {}
    virtual void Flip() = 0;
    virtual BoardDirection GetDirection() const = 0;
    virtual int GetDistance() const = 0;
    virtual std::vector< Position > GetPositions() const = 0;
    virtual bool HasTetradForTeam( const PlayerTeam team ) const = 0;
    virtual TetradShape GetTetradForTeam( const PlayerTeam team ) const = 0;
    virtual ShapeSet GetTetradShapeSetForTeam( const PlayerTeam team ) const = 0;
    // IJsonSerializable
    virtual json_spirit::mObject JsonEncode() const = 0;
    virtual void JsonDecode( json_spirit::mObject& json ) = 0;
};
class IMove : public IJsonSerializable
{
public:
    typedef std::shared_ptr< IMove > Ptr;
    virtual ~IMove() {}
    virtual void Flip() = 0;
    virtual PositionState GetWho() const = 0;
    virtual Position GetFrom() const = 0;
    virtual Position GetTo() const = 0;
    virtual IMoveResult::Ptr GetResult() const = 0;
    // IJsonSerializable
    virtual json_spirit::mObject JsonEncode() const = 0;
    virtual void JsonDecode( json_spirit::mObject& json ) = 0;
};

IMove::Ptr JsonDecodeMove( json_spirit::mObject& json );
IMove::Ptr FlipMove( const IMove::Ptr& move );

} // Implbits
