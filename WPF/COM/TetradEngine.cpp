// TetradEngine.cpp : Implementation of CTetradEngine

#include "stdafx.h"
#include "TetradEngine.h"
#include "Position.h"
#include "MoveResult.h"
#include "Tetrad.h"
#include "ComHelper.h"


static BOARD_DIRECTION ConvertBoardDirection( const Implbits::BoardDirection dir )
{
    switch ( dir )
    {
    case Implbits::DIR_LEFT:
        return DIR_LEFT;
    case Implbits::DIR_RIGHT:
        return DIR_RIGHT;
    case Implbits::DIR_UP:
        return DIR_UP;
    case Implbits::DIR_DOWN:
        return DIR_DOWN;
    case Implbits::DIR_INVALID:
    default:
        return DIR_INVALID;
    }
}
static TETRAD_SHAPE ConvertTetradShape( const Implbits::TetradShape s )
{
    switch ( s )
    {
    case Implbits::TETRAD_SQUARE:
        return TETRAD_SQUARE;
    case Implbits::TETRAD_LINE:
        return TETRAD_LINE;
    case Implbits::TETRAD_BACKWARDSL:
        return TETRAD_BACKWARDSL;
    case Implbits::TETRAD_LSHAPE:
        return TETRAD_LSHAPE;
    case Implbits::TETRAD_DOWNSTEP:
        return TETRAD_DOWNSTEP;
    case Implbits::TETRAD_UPSTEP:
        return TETRAD_UPSTEP;
    case Implbits::TETRAD_PODIUM:
        return TETRAD_PODIUM;
    case Implbits::TETRAD_END:
    default:
        return TETRAD_END;
    }
}
static CComPtr< CMoveResult > ConvertEngineResult( const Implbits::IMoveResult::Ptr& engineresult )
{
    auto moveResult = CoCreatePtr< CMoveResult >();

    moveResult->SetDirection( ConvertBoardDirection( engineresult->GetDirection() ) );
    moveResult->SetDistance( static_cast< BYTE >( engineresult->GetDistance() ) );

    auto posvector = engineresult->GetPositions();
    std::vector< CComPtr< IPosition > > positions;
    std::for_each( std::begin( posvector ), std::end( posvector ), [&positions]( Implbits::Position& vpos )
        {
            auto pos = CoCreateIPtr< IPosition, CPosition >();
            pos->put_X( vpos.x - 1 );
            pos->put_Y( vpos.y - 1 );
            positions.push_back( pos );
        } );
    moveResult->SetPositions( positions );

    if ( engineresult->HasHomeTetrad() )
    {
        auto myTetrad = CoCreatePtr< CTetrad >();
        myTetrad->SetShape( ConvertTetradShape( engineresult->GetHomeTetrad() ) );
        moveResult->SetMyTetrad( myTetrad );
    }
    if ( engineresult->HasAwayTetrad() )
    {
        auto theirTetrad = CoCreatePtr< CTetrad >();
        theirTetrad->SetShape( ConvertTetradShape( engineresult->GetAwayTetrad() ) );
        moveResult->SetTheirTetrad( theirTetrad );
    }
    return moveResult;
}

// CTetradEngine

CTetradEngine::CTetradEngine() : m_engine( nullptr ), m_mutator( nullptr )
{}

STDMETHODIMP CTetradEngine::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_ITetradEngine
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CTetradEngine::Init()
{
	m_engine = Implbits::IGameEngine::Create();
	m_mutator = m_engine->GetMutator();
	m_engine->Start();
	return S_OK;
}

STDMETHODIMP CTetradEngine::GetBoard(PLAYER player, ULONG* encoded)
{
	// TODO: Add your implementation code here
	*encoded = m_engine->GetGameBoard()->Encode();
	return S_OK;
}


STDMETHODIMP CTetradEngine::GetTetrads(PLAYER mine, ULONG* tetrads)
{
	// TODO: Never called!

	return S_OK;
}


STDMETHODIMP CTetradEngine::GetTargets(IPosition* position, SAFEARRAY** targets)
{
	SHORT posX, posY;
	position->get_X(&posX);
	position->get_Y(&posY);
	Implbits::Position selectedPosition( posX + 1, posY + 1 );
	auto moves = m_mutator->GetValidMoves( selectedPosition );
	
	if( moves.size() == 0 ) 
		return S_OK;
	
	CComSafeArray<VARIANT> arr;
	std::for_each( begin(moves), end(moves), [&arr]( const Implbits::Position& validMove )
	{
		CComPtr<IPosition> comPos = CoCreateIPtr< IPosition, CPosition >();
		comPos->put_X( validMove.x - 1 );
		comPos->put_Y( validMove.y - 1 );
		AppendObject( arr, comPos );
	});
	
	*targets = arr.Detach();
	
	return S_OK;
}

STDMETHODIMP CTetradEngine::GetOpponentMove(IMoveResult** result)
{
	auto engineresult = m_mutator->GetOpponentMove();
    auto moveResult = ConvertEngineResult( engineresult );

	*result = moveResult;
    ( *result )->AddRef();
	return S_OK;
}

STDMETHODIMP CTetradEngine::Move(IPosition* from, IPosition* to, IMoveResult** result)
{
	if( from == nullptr || to == nullptr || result == nullptr  ) 
		return E_POINTER;

	SHORT fromX, fromY, toX, toY;
	
	from->get_X( &fromX );
	from->get_Y( &fromY );
	to->get_X( &toX );
	to->get_Y( &toY );

    auto engineresult = m_mutator->MoveTile( Implbits::Position( fromX + 1, fromY + 1 ), Implbits::Position( toX + 1, toY + 1 ) );
    if ( ! engineresult || engineresult->GetDirection() == Implbits::DIR_INVALID )
    {
        *result = nullptr;
        return S_FALSE;
    }

    auto moveResult = ConvertEngineResult( engineresult );

    *result = moveResult;
    ( *result )->AddRef();
    return S_OK;
}

