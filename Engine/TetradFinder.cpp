#ifdef _MSC_VER
#include "stdafx.h"
#endif
#include "TetradFinder.h"
#include "GameMechanicsUtil.h"

#include <functional>
#include <map>


namespace Implbits {

struct RelativeSet
{
    RelativeSet()
    {}
    RelativeSet( const Position& _p1, const Position& _p2, const Position& _p3 ) :
        p1( _p1 ), p2( _p2 ), p3( _p3 )
    {}
    Position p1;
    Position p2;
    Position p3;
};
class ShapeImpl : public IShape
{
public:
    static IShape::Ptr Create( const TetradShape s, const Position& o, const RelativeSet& rs )
    {
        return IShape::Ptr( new ShapeImpl( s, o, rs ) );
    }
    // Rotation matrix for rotating points through euclidean space about the origin of cartesian coordinate system
    //       --             --
    //   R = | cos a  -sin a |
    //       | sin a   cos a |
    //       --             --   where a is the angle of rotation.
    //
    //   The rotated position, r, is given by: r = Rv
    //      where v is the initial position
    //
    //   In our case we can reduce this to just three matrices: 90, 180, 270.
    //
    //       --    --         --     --         --     --
    // R90 = | 0 -1 |  R180 = | -1  0 |  R270 = |  0  1 |
    //       | 1  0 |         |  0 -1 |         | -1  0 |
    //       --    --         --     --         --     --
    //
    // This can be reduced to simple equations:
    //
    //   r90  = [ -v(2),  v(1) ]
    //   r180 = [ -v(1), -v(2) ]
    //   r270 = [  v(2), -v(1) ]
    //
    // So, for [ 1, 1 ]:  r90 = [ -1, 1 ],  r180 = [ -1, -1 ],  r270 = [ 1, -1 ]
    virtual ShapeSet GetRotation0() const
    {
        return GetShapeSet( m_relativeSet );
    }
    virtual ShapeSet GetRotation90() const
    {
        RelativeSet rs;
        rs.p1 = Rotate90( m_relativeSet.p1 );
        rs.p2 = Rotate90( m_relativeSet.p2 );
        rs.p3 = Rotate90( m_relativeSet.p3 );
        return GetShapeSet( rs );
    }
    virtual ShapeSet GetRotation180() const
    {
        RelativeSet rs;
        rs.p1 = Rotate180( m_relativeSet.p1 );
        rs.p2 = Rotate180( m_relativeSet.p2 );
        rs.p3 = Rotate180( m_relativeSet.p3 );
        return GetShapeSet( rs );
    }
    virtual ShapeSet GetRotation270() const
    {
        RelativeSet rs;
        rs.p1 = Rotate270( m_relativeSet.p1 );
        rs.p2 = Rotate270( m_relativeSet.p2 );
        rs.p3 = Rotate270( m_relativeSet.p3 );
        return GetShapeSet( rs );
    }
private:
    ShapeImpl( const TetradShape& s, const Position& o, const RelativeSet& rs ) :
         m_type( s ), m_origin( o ), m_relativeSet( rs )
    {}
    ShapeSet GetShapeSet( const RelativeSet& rs ) const
    {
        ShapeSet shape;
        shape[0] = m_origin;
        shape[1] = GetAbsolute( rs.p1 );
        shape[2] = GetAbsolute( rs.p2 );
        shape[3] = GetAbsolute( rs.p3 );
        return shape;
    }
    Position GetAbsolute( const Position& relative ) const
    {
        Position ret = m_origin;
        ret.x += relative.x;
        ret.y += relative.y;
        return ret;
    }
    Position Rotate90( const Position& p ) const
    {
        // [ -v(2),  v(1) ]
        return Position( 0 - p.y, p.x );
    }
    Position Rotate180( const Position& p ) const
    {
        // [ -v(1), -v(2) ]
        return Position( 0 - p.x, 0 - p.y );
    }
    Position Rotate270( const Position& p ) const
    {
        // [  v(2), -v(1) ]
        return Position( p.y, 0 - p.x );
    }
    TetradShape m_type;
    Position m_origin;
    RelativeSet m_relativeSet;
};
class ShapeFactory
{
public:
    typedef std::shared_ptr< ShapeFactory > Ptr;
    typedef std::function< IShape::Ptr ( const Position& ) > ShapeCreator;
    typedef std::map< TetradShape, ShapeCreator > Registry;
    static ShapeFactory::Ptr Instance()
    {
        static auto inst = ShapeFactory::Ptr( new ShapeFactory() );
        return inst;
    }
    bool Register( const TetradShape& s, const ShapeCreator& f )
    {
        return m_reg.insert( Registry::value_type( s, f ) ).second;
    }
    IShape::Ptr Create( const TetradShape& s, const Position& o ) const
    {
        auto it = m_reg.find( s );
        if ( it != std::end( m_reg ) )
            return it->second( o );
        throw ShapeConstructionException( s );
    }
private:
    ShapeFactory()
    {}
    Registry m_reg;
};

TetradFinder::Ptr TetradFinder::Instance()
{
    static auto inst = TetradFinder::Ptr( new TetradFinder() );
    return inst;
}
TetradFinder::TetradFinder()
{}
TetradFinder::~TetradFinder()
{}
TetradResult TetradFinder::FindTetrads( const GameBoard::ConstPtr& board ) const
{
    TetradResult result;
    std::for_each( TETRAD_BEGIN, TETRAD_END, [&]( TetradShape s )
        {
            SearchForTetrad( board, s, result );
            if ( result.home != TETRAD_END && result.away != TETRAD_END )
                return;
        } );
    return result;
}
void TetradFinder::SearchForTetrad( const GameBoard::ConstPtr& board, const TetradShape& s, TetradResult& result ) const
{
    ShapeFactory::Ptr factory = ShapeFactory::Instance();
    std::for_each( board->begin(), board->end(), [&]( Position o )
        {
            IShape::Ptr shape = factory->Create( s, o );
            CheckRotations( board, s, shape, result );
            if ( result.home != TETRAD_END && result.away != TETRAD_END )
                return;
        } );
}
void TetradFinder::CheckRotations( const GameBoard::ConstPtr& board, const TetradShape& s, const IShape::Ptr& shape, TetradResult& result ) const
{
    CheckRotationX( board, s, shape->GetRotation0(), result )
        || CheckRotationX( board, s, shape->GetRotation90(), result )
        || CheckRotationX( board, s, shape->GetRotation180(), result )
        || CheckRotationX( board, s, shape->GetRotation270(), result );
}
bool TetradFinder::CheckRotationX( const GameBoard::ConstPtr& board, const TetradShape& s, const ShapeSet& shapeSet, TetradResult& result ) const
{
    PositionState owner = IsTetradShape( board, shapeSet );
    if ( owner != STATE_INVALID )
    {
        if ( owner == STATE_HOME )
        {
            result.home = s;
            result.homeSet = shapeSet;
        }
        else if ( owner == STATE_AWAY )
        {
            result.away = s;
            result.awaySet = shapeSet;
        }
        return true;
    }
    return false;
}
PositionState TetradFinder::IsTetradShape( const GameBoard::ConstPtr& board, const ShapeSet& ss ) const
{
    PositionState owner = GetTetradShapeOwner( board, ss );
    if ( owner == STATE_HOME || owner == STATE_AWAY )
    {
        if ( IsTetradShapeUnencumbered( board, ss, owner ) )
            return owner;
    }
    return STATE_INVALID;
}
PositionState TetradFinder::GetTetradShapeOwner( const GameBoard::ConstPtr& board, const ShapeSet& ss ) const
{
    auto p = std::begin( ss );
    PositionState ps = board->GetPositionState( *p );
    switch ( ps )
    {
    case STATE_HOME:
    case STATE_AWAY:
        for ( p++; p != std::end( ss ); p++ )
        {
            PositionState ps_next = board->GetPositionState( *p );
            if ( ps_next != ps )
            {
                ps = STATE_INVALID;
                break;
            }
        }
        break;
    case STATE_EMPTY:
    case STATE_INVALID:
    default:
        ps = STATE_INVALID;
    }
    return ps;
}
bool TetradFinder::IsTetradShapeUnencumbered( const GameBoard::ConstPtr& board, const ShapeSet& ss, const PositionState& owner ) const
{
    for ( auto p = std::begin( ss ); p != std::end( ss ); p++ )
    {
        for ( BoardDirection d = DIR_BEGIN; d != DIR_END; d++ )
        {
            Position adjacent = IterateInDirection( *p, d );
            if ( ! std::any_of( std::begin( ss ), std::end( ss ), [&adjacent]( Position in_set ) -> bool { return adjacent == in_set; } )
                    && board->GetPositionState( adjacent ) == owner )
                return false;
        }
    }
    return true;
}

// shape factory registrations
namespace {
    using namespace Implbits;
    bool s_square = ShapeFactory::Instance()->Register( TETRAD_SQUARE, []( const Position& o ) -> IShape::Ptr
        {
            return ShapeImpl::Create( TETRAD_SQUARE, o, RelativeSet( Position( 1, 0 ), Position( 1, 1 ), Position( 0, 1 ) ) );
        } );
    bool s_line = ShapeFactory::Instance()->Register( TETRAD_LINE, []( const Position& o ) -> IShape::Ptr
        {
            return ShapeImpl::Create( TETRAD_LINE, o, RelativeSet( Position( 1, 0 ), Position( 2, 0 ), Position( 3, 0 ) ) );
        } );
    bool s_backwardsl = ShapeFactory::Instance()->Register( TETRAD_BACKWARDSL, []( const Position& o ) -> IShape::Ptr
        {
            return ShapeImpl::Create( TETRAD_BACKWARDSL, o, RelativeSet( Position( 0, 1 ), Position( 0, 2 ), Position( -1, 2 ) ) );
        } );
    bool s_lshape = ShapeFactory::Instance()->Register( TETRAD_LSHAPE, []( const Position& o ) -> IShape::Ptr
        {
            return ShapeImpl::Create( TETRAD_LSHAPE, o, RelativeSet( Position( 0, 1 ), Position( 0, 2 ), Position( 1, 2 ) ) );
        } );
    bool s_downstep = ShapeFactory::Instance()->Register( TETRAD_DOWNSTEP, []( const Position& o ) -> IShape::Ptr
        {
            return ShapeImpl::Create( TETRAD_DOWNSTEP, o, RelativeSet( Position( 1, 0 ), Position( 1, 1 ), Position( 2, 1 ) ) );
        } );
    bool s_upstep = ShapeFactory::Instance()->Register( TETRAD_UPSTEP, []( const Position& o ) -> IShape::Ptr
        {
            return ShapeImpl::Create( TETRAD_UPSTEP, o, RelativeSet( Position( 1, 0 ), Position( 1, -1 ), Position( 2, -1 ) ) );
        } );
    bool s_podium = ShapeFactory::Instance()->Register( TETRAD_PODIUM, []( const Position& o ) -> IShape::Ptr
        {
            return ShapeImpl::Create( TETRAD_PODIUM, o, RelativeSet( Position( 1, 0 ), Position( 2, 0 ), Position( 1, -1 ) ) );
        } );
}


} // Implbits

