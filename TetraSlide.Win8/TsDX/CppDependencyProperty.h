#pragma once

#define DEFINE_DEPENDENCY_PROPERTY( type, name ) \
    private: \
        static Windows::UI::Xaml::DependencyProperty^ m_dp##name; \
    public: \
        static property Windows::UI::Xaml::DependencyProperty^ name##Property \
        { \
             Windows::UI::Xaml::DependencyProperty^ get() { return m_dp##name ; } \
        } \
        property type name \
        { \
            type get() { return ( type )GetValue( name##Property ); } \
            void set( type value ) { SetValue( name##Property, value ); } \
        }
#define REGISTER_DEPENDENCY_PROPERTY( type, cls, name ) \
    Windows::UI::Xaml::DependencyProperty^ cls##::m_dp##name = \
    Windows::UI::Xaml::DependencyProperty::Register( #name, type##::typeid, cls##::typeid, nullptr );
