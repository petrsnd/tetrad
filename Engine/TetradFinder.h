#pragma once
#include "GameBoard.h"
#include "Move.h"

#include <memory>


namespace Implbits {

class IShape
{
public:
    typedef std::shared_ptr< IShape > Ptr;
    virtual ShapeSet GetRotation0() const = 0;
    virtual ShapeSet GetRotation90() const = 0;
    virtual ShapeSet GetRotation180() const = 0;
    virtual ShapeSet GetRotation270() const = 0;
};

class TetradFinder
{
public:
    typedef std::shared_ptr< TetradFinder > Ptr;
    static TetradFinder::Ptr Instance();
    ~TetradFinder();
    TetradResult FindTetrads( const GameBoard::ConstPtr& board ) const;
private:
    TetradFinder();
    void SearchForTetrad( const GameBoard::ConstPtr& board, const TetradShape& s, TetradResult& result ) const;
    void CheckRotations( const GameBoard::ConstPtr& board, const TetradShape& s, const IShape::Ptr& shape, TetradResult& result ) const;
    bool CheckRotationX( const GameBoard::ConstPtr& board, const TetradShape& s, const ShapeSet& shapeSet, TetradResult& result ) const;
    PositionState IsTetradShape( const GameBoard::ConstPtr& board, const ShapeSet& ss ) const;
    PositionState GetTetradShapeOwner( const GameBoard::ConstPtr& board, const ShapeSet& ss ) const;
    bool IsTetradShapeUnencumbered( const GameBoard::ConstPtr& board, const ShapeSet& ss, const PositionState& owner ) const;
};

} // Implbits
