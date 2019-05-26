#ifndef CPP98DEBUG_HPP
#define CPP98DEBUG_HPP

#include <cstdio>
#include <cstring>

#include <map>
#include <string>
#include <fstream>
#include <iostream>

class Cpp98DebugBase {
private:
  struct StringComparator {
    bool operator( )( const char *str1, const char *str2 ) const { return strcmp( str1, str2 ) < 0; }
  };

  bool m_default;
  std::map< const char *, bool, StringComparator > m_map;

  static Cpp98DebugBase *m_singleton;

protected:
  void init( const char *cfg ) {
    std::ifstream cfgFile( cfg );
    int lineNumber = 0;

    while( true ) {
      ++lineNumber;
      std::string line;
      std::getline( cfgFile, line );

      if( !cfgFile.good( ) ) break; // Cualquier error, terminamos.
      if( line.size( ) < 2 ) continue; // Ignorar líneas en blanco o con solo un '\n'.
      if( line[0] == '#' ) continue; // Los comentarios empiezan con '#'.
      if( ( line[0] != '0' ) && ( line[0] != '1' ) ) { // Linea mal formada. Aviso y continuamos.
        std::cerr << "CPP98DEBUG: Linea " << lineNumber << " incorrecta.\n";
        continue;
      }

      bool process = line[0] == '1' ? true : false;

      if( line[1] == '*' ) {
        m_default = process;
      } else {
        m_map[line.c_str( ) + 1] = process;
      }
    }

    std::cout << "defecto: " << m_default << '\n';
    std::map< const char *, bool, StringComparator >::const_iterator iter( m_map.begin( ) );

    while( iter != m_map.end( ) ) {
      std::cout << "Identificador " << iter->first << ": " << iter->second << '\n';
      ++iter;
    }
    std::cout << std::endl;
  }

public:
  virtual void flush( const char *str ) = 0;

  static Cpp98DebugBase &singleton( ) { return *m_singleton; }

  class output {
    bool m_do;
    char m_final[4096];
    char m_buff[1024];

  public:
    ~output( ) {
      if( m_do ) {
        Cpp98DebugBase::singleton( ).flush( m_final );
      }
    }
    output( const char *module, bool d ) : m_do( d ) {
      if( d ) {
        strcpy( m_final, module );
        strcat( m_final, ": " );
      }
    }

    output &flush( ) {
      if( m_do ) { Cpp98DebugBase::singleton( ).flush( m_final ); }
      return *this;
    }
    output &operator<<( bool v ) {
      if( m_do ) {
        strcat( m_final, v ? "TRUE" : "FALSE" );
      }
      return *this;
    }
    output &operator<<( const void *ptr ) {
      if( m_do ) {
        sprintf( m_buff, "%p", ptr );
        strcat( m_final, m_buff );
      }
      return *this;
    }
    output &operator<<( const char *str ) {
      if( m_do ) {
        strcat( m_final, str );
      }
      return *this;
    }
    output &operator<<( const std::string &str ) { return m_do ? operator<<( str.c_str( ) ) : *this; }
    output &operator<<( char c ) {
      if( m_do ) {
        signed char tmp[2] = { c, 0 };
        strcat( m_final, reinterpret_cast< char * >( tmp ) );
      }
      return *this;
    }
    output &operator<<( signed char c ) {
      if( m_do ) {
        signed char tmp[2] = { c, 0 };
        strcat( m_final, reinterpret_cast< char * >( tmp ) );
      }
      return *this;
    }
    output &operator<<( unsigned char c ) {
      if( m_do ) {
        unsigned char tmp[2] = { c, 0 };
        strcat( m_final, reinterpret_cast< char * >( tmp ) );
      }
      return *this;
    }
    output &operator<<( signed i ) {
      if( m_do ) {
        sprintf( m_buff, "%d", i );
        strcat( m_final, m_buff );
      }
      return *this;
    }
    output &operator<<( unsigned i ) {
      if( m_do ) {
        sprintf( m_buff, "%u", i );
        strcat( m_final, m_buff );
      }
      return *this;
    }
    output &operator<<( signed long l ) {
      if( m_do ) {
        sprintf( m_buff, "%ld", l );
        strcat( m_final, m_buff );
      }
      return *this;
    }
    output &operator<<( unsigned long l ) {
      if( m_do ) {
        sprintf( m_buff, "%lu", l );
        strcat( m_final, m_buff );
      }
      return *this;
    }
    output &operator<<( float f ) {
      if( m_do ) {
        sprintf( m_buff, "%f", f );
        strcat( m_final, m_buff );
      }
      return *this;
    }
    output &operator<<( double d ) {
      if( m_do ) {
        sprintf( m_buff, "%f", d );
        strcat( m_final, m_buff );
      }
      return *this;
    }
    output &operator<<( long double ld ) {
      if( m_do ) {
        sprintf( m_buff, "%Lf", ld );
        strcat( m_final, m_buff );
      }
      return *this;
    }
  };

  output operator( )( const char *module, bool assertion = true ) {
    std::map< const char *, bool, StringComparator >::const_iterator iter = m_map.find( module );

    return output( module, ( ( iter == m_map.end( ) ) ? m_default : iter->second ) && assertion );
  }
  output operator( )( bool, const char *module, bool assertion = true ) {
    return output( module, assertion );
  }

  Cpp98DebugBase( ) : m_default( true ), m_map( ) {
    m_singleton = this;
  }
};

template< typename T, typename TRAITS = std::char_traits< T > > class Cpp98DebugGeneric : public Cpp98DebugBase {
  std::basic_ostream< T, TRAITS > &m_os;

protected:
  virtual void flush( const char *str ) {
    m_os << str << std::endl;
  }

public:
  Cpp98DebugGeneric( std::basic_ostream< T, TRAITS > &os, const char *cfg ) : m_os( os ) { Cpp98DebugBase::init( cfg ); }
};

typedef Cpp98DebugGeneric< char, std::char_traits< char > > Cpp98Debug;

#define TRACE98( LOG, MOD ) ( LOG( MOD ) << __LINE__ << ": " )
#define CTRACE98( LOG, MOD, ASSERTION ) ( LOG( MOD, ( ASSERTION ) ) )

#endif
