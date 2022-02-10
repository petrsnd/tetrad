#pragma once
#include "Interfaces.h"
#include <memory>
#include <sstream>
#include <iostream>


namespace Implbits {


class LogProxy
{
private:
    std::stringstream m_ss;
    IView::Ptr m_view;
    std::string m_file;
    int m_line;
    bool m_isCopy;

public:
    LogProxy( const IView::Ptr view, const std::string& file, int line ) : 
       m_view( view ), m_file( file ), m_line( line ), m_isCopy( false )
    {}
    
    LogProxy( const LogProxy& rhs ) : 
        m_view( rhs.m_view ),
        m_file( rhs.m_file ), 
        m_line( rhs.m_line), m_isCopy(true)
    {}

    LogProxy& operator<<( const std::string& t )
    { 
        m_ss << t.c_str();
        return *this;
    }

    template< typename T >
    LogProxy& operator<<( T& t )
    {
        m_ss << t;
        return *this;
    }

    ~LogProxy()
    {
        if( m_isCopy )
        {
            if( m_view )
                m_view->Log( m_ss.str(), m_file, m_line );
            else
            {
                std::cerr << "Attempting to log to an uninitialized logger! From:" 
                << m_file << ":" << m_line << std::endl;
            }
        }
    }

    
private:
    LogProxy operator=(const LogProxy& );
    std::vector< std::string > m_args;
};



class Logger
{
    friend class LogProxy;
public:
    typedef std::shared_ptr< Logger > Ptr;
    static LogProxy Instance(const std::string& file, int line)
    {
        return LogProxy(GetInstance()->m_view, file, line );
    }

    static void Init( const IView::Ptr& view )
    {
        GetInstance()->m_view = view;
    }
private:
    static Logger::Ptr GetInstance()
    {
        static Logger::Ptr instance = std::make_shared<Logger>();
        return instance;
    }
    IView::Ptr m_view;
};

}


#define LOG Logger::Instance(__FILE__, __LINE__)
