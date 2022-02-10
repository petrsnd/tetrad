#pragma once
#include "GameMechanics.h"

#include <boost/algorithm/string.hpp>

#include <functional>
#include <memory>
#include <map>
#include <set>


namespace Implbits {

class IMoveGenerator
{
public:
    typedef std::shared_ptr< IMoveGenerator > Ptr;
    virtual IMove::Ptr GetNextMove( const GameBoard::ConstPtr& board, const IMove::Ptr lastmove, const std::set< TetradShape >& myTetrads, const std::set< TetradShape >& opponentTetrads ) = 0;
};

struct LessNoCase : std::binary_function< std::string, std::string, bool >
{
    bool operator()( const std::string & s1, const std::string & s2 ) const
    {
        return boost::algorithm::lexicographical_compare( s1, s2, boost::is_iless() );
    }
};

class MoveGeneratorFactory
{
public:
    typedef std::shared_ptr< MoveGeneratorFactory > Ptr;
    typedef std::function< IMoveGenerator::Ptr ( const PlayerTeam ) > MoveGeneratorCreator;
    typedef std::map< std::string, MoveGeneratorCreator, LessNoCase > Registry;
    static MoveGeneratorFactory::Ptr Instance();
    ~MoveGeneratorFactory();
    bool Register( const std::string& key, const MoveGeneratorCreator& generator );
    IMoveGenerator::Ptr Create( const std::string& key, const PlayerTeam team ) const;
private:
    MoveGeneratorFactory();
    Registry m_reg;
};

} // Implbits
