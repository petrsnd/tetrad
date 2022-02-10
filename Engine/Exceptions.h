#pragma once
#include "Definitions.h"

#include <exception>
#include <string>
#include <sstream>


namespace Implbits {

class GameBoardOutOfBoundsException : public std::exception
{
public:
    GameBoardOutOfBoundsException()
    {}
    virtual ~GameBoardOutOfBoundsException()
    {}
    virtual const char* what() const
    {
        return "Cannot set status of game board position out of bounds";
    }
};

class InvalidPositionIterationException : public std::exception
{
public:
    InvalidPositionIterationException()
    {}
    virtual ~InvalidPositionIterationException()
    {}
    virtual const char* what() const
    {
        return "Cannot iterate in an invalid direction";
    }
};

class ShapeConstructionException : public std::exception
{
public:
    ShapeConstructionException( const TetradShape& s )
    {
        std::stringstream msgstream;
        msgstream << "Unable to construct shape of type " << s;
        m_msg = msgstream.str();
    }
    virtual ~ShapeConstructionException()
    {}
    virtual const char* what() const
    {
        return m_msg.c_str();
    }
protected:
    std::string m_msg;
};

class LinearDistanceException : public std::exception
{
public:
    LinearDistanceException( const Position& from, const Position& to, const BoardDirection& dir )
    {
        std::stringstream msgstream;
        msgstream << "Unable to calculate linear distance, [" 
            << to.x << "," << to.y << "] is not in not in direction " << dir << " from [" 
            << from.x << "," << from.y << "]";
        m_msg = msgstream.str();
    }
    virtual ~LinearDistanceException()
    {}
    virtual const char* what() const
    {
        return m_msg.c_str();
    }
protected:
    std::string m_msg;
};

class UnexpectedException : public std::exception
{
public:
    UnexpectedException( const std::string& msg ) :
        m_msg( "Unexpected error: " + msg )
    {}
    virtual ~UnexpectedException()
    {}
    virtual const char* what() const
    {
        return m_msg.c_str();
    }
protected:
    std::string m_msg;
};

} // Implbits
