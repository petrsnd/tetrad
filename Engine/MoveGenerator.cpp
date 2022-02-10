#ifdef _MSC_VER
#include "stdafx.h"
#include <Windows.h>
#include <Wincrypt.h>
#endif
#include "MoveGenerator.h"
#include "Definitions.h"

#include <map>
#include <vector>

#include <cassert>
#include <climits>
#include <cstdlib>
#include <ctime>


namespace Implbits {

#ifdef _MSC_VER
class WindowsCryptoProvider
{
public:
    typedef std::shared_ptr< WindowsCryptoProvider > Ptr;
    static int Rand()
    {
        auto inst = WindowsCryptoProvider::Instance();
        if ( ! inst->m_hCryptProv )
            return rand();
        int rnd = 0;
        if ( ! CryptGenRandom( inst->m_hCryptProv, sizeof( int ), ( BYTE* )&rnd ) )
        {
            // This should not happen because we already tested getting a random, but just in case...
            srand( ( int )time( NULL ) );
            return rand();
        }
        return rnd;
    }
    ~WindowsCryptoProvider()
    {
        if ( m_hCryptProv )
            CryptReleaseContext( m_hCryptProv, 0 );
    }
private:
    WindowsCryptoProvider() :
        m_hCryptProv( NULL )
    {
        if ( ! CryptAcquireContext( &m_hCryptProv, NULL, NULL, PROV_RSA_FULL, 0 ) )
        {
            int rnd = 0;
            if ( GetLastError() != NTE_BAD_KEYSET
                 || ! CryptAcquireContext( &m_hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET )
                 || ! CryptGenRandom( m_hCryptProv, sizeof( int ), ( BYTE* )&rnd ) )
            {
                m_hCryptProv = NULL;
                srand( ( int )time( NULL ) );
            }
        } 
    }
    static WindowsCryptoProvider::Ptr Instance()
    {
        static auto inst = WindowsCryptoProvider::Ptr( new WindowsCryptoProvider() );
        return inst;
    }
    HCRYPTPROV m_hCryptProv;
};
#endif
static int GenerateRandomNumber()
{
#ifdef _MSC_VER
    return WindowsCryptoProvider::Rand();
#elif __APPLE__
    // TODO: IOS
    #error NotImplemented
#else
    // TODO: other platforms
    #error NotImplemented
#endif
}


MoveGeneratorFactory::Ptr MoveGeneratorFactory::Instance()
{
    static auto inst = MoveGeneratorFactory::Ptr( new MoveGeneratorFactory() );
    return inst;
}
MoveGeneratorFactory::MoveGeneratorFactory()
{}
MoveGeneratorFactory::~MoveGeneratorFactory()
{}
bool MoveGeneratorFactory::Register( const std::string& key, const MoveGeneratorCreator& f )
{
    return m_reg.insert( Registry::value_type( key, f ) ).second;
}
IMoveGenerator::Ptr MoveGeneratorFactory::Create( const std::string& key, const PlayerTeam team ) const
{
    auto it = m_reg.find( key );
    if ( it != m_reg.end() )
        return it->second( team );
    // Random generator is the default
    it = m_reg.find( "random" );
    if ( it != m_reg.end() )
        return it->second( team );
    return IMoveGenerator::Ptr();
}


class RandomMoveGenerator : public IMoveGenerator
{
public:
    static IMoveGenerator::Ptr Create()
    {
        return IMoveGenerator::Ptr( new RandomMoveGenerator() );
    }
    ~RandomMoveGenerator()
    {}
    virtual IMove::Ptr GetNextMove( const GameBoard::ConstPtr& board, const IMove::Ptr lastmove, const std::set< TetradShape >& myTetrads, const std::set< TetradShape >& opponentTetrads )
    {
        auto moves = GameMechanics::Instance()->GetValidMoves( board, lastmove, STATE_AWAY );
        int idx = GenerateRandomNumber() % moves.size();
        return moves[idx];
    }
private:
    RandomMoveGenerator()
    {}
};



class MoveTree
{
private:
    class MoveNode
    {
    public:
        typedef std::shared_ptr< MoveNode > Ptr;
        typedef std::map< int, MoveNode::Ptr > ChildMap;
        typedef std::vector< int > ChildMapIndex;
        MoveNode( const PlayerTeam team, const IMove::Ptr& move, const GameBoard::Ptr& board,
                  const size_t depth, const int offense, const int defense ) :
            m_myTeam( team ),
            m_opponentTeam( GetOpponent( team ) ),
            m_move( move ),
            m_board( board ),
            m_depth( depth ),
            m_offense( offense ),
            m_defense( defense )
        {
            RegenerateTree( depth );
        }
        ~MoveNode()
        {}
        void RegenerateTree( size_t depth )
        {
            m_depth = depth;
            if ( m_depth > 0 )
            {
                if ( m_children.empty() )
                    GenerateChildren();
                else
                    UpdateChildren();
            }
        }
        int GetEncoding() const
        {
            return m_board->Encode();
        }
        IMove::Ptr GetMove() const
        {
            return m_move;
        }
        size_t ChildCount() const
        {
            return m_children.size();
        }
        MoveNode::Ptr GetChild( const size_t i ) const
        {
            assert( i < m_childrenIndex.size() );
            auto it = m_children.find( m_childrenIndex[i] );
            return it->second;
        }
        MoveNode::Ptr FindChildWithBoard( const GameBoard::Ptr& board ) const
        {
            MoveNode::Ptr ret;
            auto it = m_children.find( board->Encode() );
            if ( it != std::end( m_children ) )
                ret = it->second;
            else
            {
                for ( auto& childpair : m_children )
                {
                    ret = FindChildWithBoard( childpair.second->m_board );
                    if ( ret )
                        break;
                }
            }
            return ret;
        }
        int CalculateScore( const std::set< TetradShape >& myTetrads, const std::set< TetradShape >& opponentTetrads ) const
        {
            int score = 0;
            if ( m_move->GetResult()->HasTetradForTeam( m_myTeam ) && ! AlreadyHasTetrad( myTetrads, m_move->GetResult()->GetTetradForTeam( m_myTeam ) ) )
                score += ( m_offense ^ static_cast< int >( m_depth ) );
            if ( m_move->GetResult()->HasTetradForTeam( m_opponentTeam ) && ! AlreadyHasTetrad( opponentTetrads, m_move->GetResult()->GetTetradForTeam( m_opponentTeam ) ) )
                score -= ( m_defense ^ static_cast< int >( m_depth ) );
            std::for_each( std::begin( m_children ), std::end( m_children ), [&score, &myTetrads, &opponentTetrads]( const ChildMap::value_type& childpair )
                {
                    score += childpair.second->CalculateScore( myTetrads, opponentTetrads );
                } );
            return score;
        }
    private:
        void GenerateChildren()
        {
            PositionState turn = STATE_HOME; // Home always goes first
            if ( m_move )
                turn = GetNextPlayer( m_move->GetWho() );
            auto moves = GameMechanics::Instance()->GetValidMoves( m_board, m_move, turn  );
            std::for_each( std::begin( moves ), std::end( moves ), [&]( IMove::Ptr& move )
                {
                    auto boardcpy = m_board->Copy();
                    GameMechanics::Instance()->ApplyMove( boardcpy, move );
                    auto child( MoveNode::Ptr( new MoveNode( m_myTeam, move, boardcpy, m_depth - 1, m_offense, m_defense ) ) );
                    m_children.insert( ChildMap::value_type( child->GetEncoding(), child ) );
                } );
            std::transform( std::begin( m_children ), std::end( m_children ), std::back_inserter( m_childrenIndex ), []( ChildMap::value_type& childpair ) -> int
                {
                    return childpair.first;
                } );
            assert( m_children.size() == m_childrenIndex.size() );
        }
        void UpdateChildren()
        {
            for ( auto& childpair : m_children )
                childpair.second->RegenerateTree( m_depth - 1 );
        }
        bool AlreadyHasTetrad( const std::set< TetradShape >& tetrads, const TetradShape s ) const
        {
            return tetrads.find( s ) != std::end( tetrads );
        }
        PlayerTeam m_myTeam;
        PlayerTeam m_opponentTeam;
        IMove::Ptr m_move;
        GameBoard::Ptr m_board;
        size_t m_depth;
        int m_offense;
        int m_defense;
        ChildMap m_children;
        ChildMapIndex m_childrenIndex;
    };
public:
    typedef std::shared_ptr< MoveTree > Ptr;
    MoveTree( const PlayerTeam team, const IMove::Ptr& lastmove, const GameBoard::Ptr& board,
              const size_t depth, const int offense, const int defense ) :
        m_myTeam( team ),
        m_root( MoveNode::Ptr( new MoveNode( team, lastmove, board, depth, offense, defense ) ) ),
        m_myDepth( depth ),
        m_offense( offense ),
        m_defense( defense )
    {}
    ~MoveTree()
    {}
    bool UpdateTree( const GameBoard::Ptr& board )
    {
        if ( m_returnedChild && m_returnedChild->ChildCount() > 0 )
        {
            auto newRoot = m_returnedChild->FindChildWithBoard( board );
            if ( newRoot )
            {
                m_root = newRoot;
                m_root->RegenerateTree( m_myDepth );
                return true;
            }
        }
        return false;
    }
    IMove::Ptr GetHighestScoringChild( const std::set< TetradShape >& myTetrads, const std::set< TetradShape >& opponentTetrads )
    {
        m_returnedChild = nullptr;
        assert( m_root->ChildCount() > 0 );
        int highest = INT_MIN;
        for ( size_t i = 0; i < m_root->ChildCount(); i++ )
        {
            auto child = m_root->GetChild( i );
            int score = child->CalculateScore( myTetrads, opponentTetrads );
            if ( score == highest && GenerateRandomNumber() % 2 ) // coin flip on equal score
                m_returnedChild = child;
            else if ( score > highest )
            {
                highest = score;
                m_returnedChild = child;
            }
        }
        return m_returnedChild->GetMove();
    }
private:
    static PlayerTeam GetOpponent( const PlayerTeam team )
    {
        if ( team == TEAM_HOME )
            return TEAM_AWAY;
        return TEAM_HOME;
    }
    static PositionState GetNextPlayer( const PositionState who )
    {
        if ( who == STATE_HOME )
            return STATE_AWAY;
        return STATE_HOME;
    }
    PlayerTeam m_myTeam;
    MoveNode::Ptr m_root;
    MoveNode::Ptr m_returnedChild;
    size_t m_myDepth;
    int m_offense;
    int m_defense;
};
class IntelligentMoveGenerator : public IMoveGenerator
{
public:
    static IMoveGenerator::Ptr Create( const PlayerTeam team, const size_t depth, const int offense, const int defense )
    {
        return IMoveGenerator::Ptr( new IntelligentMoveGenerator( team, depth, offense, defense ) );
    }
    ~IntelligentMoveGenerator()
    {}
    virtual IMove::Ptr GetNextMove( const GameBoard::ConstPtr& board, const IMove::Ptr lastmove, const std::set< TetradShape >& myTetrads, const std::set< TetradShape >& opponentTetrads )
    {
        auto boardCopy = board->Copy();
        if ( m_tree && m_tree->UpdateTree( boardCopy ) ) // If we can reuse the same tree it will go much faster
            return m_tree->GetHighestScoringChild( myTetrads, opponentTetrads );
        m_tree = MoveTree::Ptr( new MoveTree( m_team, lastmove, boardCopy, m_depth, m_offense, m_defense ) );
        return m_tree->GetHighestScoringChild( myTetrads, opponentTetrads );
    }
private:
    IntelligentMoveGenerator( const PlayerTeam team, const size_t depth, const int offense, const int defense ) :
       m_team( team ),
       m_depth( depth ),
       m_offense( offense ),
       m_defense( defense )
    {}
    MoveTree::Ptr m_tree;
    PlayerTeam m_team;
    const size_t m_depth;
    const int m_offense;
    const int m_defense;
};

namespace {
    using namespace Implbits;
    bool s_random = MoveGeneratorFactory::Instance()->Register( "random", []( const PlayerTeam team ) -> IMoveGenerator::Ptr
        { return RandomMoveGenerator::Create(); } );
    bool s_beginner = MoveGeneratorFactory::Instance()->Register( "beginner", []( const PlayerTeam team ) -> IMoveGenerator::Ptr
        { return RandomMoveGenerator::Create(); } );
    bool s_easy = MoveGeneratorFactory::Instance()->Register( "easy", []( const PlayerTeam team ) -> IMoveGenerator::Ptr
        { return IntelligentMoveGenerator::Create( team, 1, 1, 1 ); } );
    bool s_intermediate = MoveGeneratorFactory::Instance()->Register( "intermediate", []( const PlayerTeam team ) -> IMoveGenerator::Ptr
        { return IntelligentMoveGenerator::Create( team, 2, 2, 1 ); } );
    bool s_hard = MoveGeneratorFactory::Instance()->Register( "hard", []( const PlayerTeam team ) -> IMoveGenerator::Ptr
        { return IntelligentMoveGenerator::Create( team, 4, 2, 2 ); } );
    bool s_expert = MoveGeneratorFactory::Instance()->Register( "expert", []( const PlayerTeam team ) -> IMoveGenerator::Ptr
        { return IntelligentMoveGenerator::Create( team, 4, 2, 3 ); } );
}

} // Implbits
