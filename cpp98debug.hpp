#ifndef CPP98DEBUG_HPP
#define CPP98DEBUG_HPP

#include <cstdio>
#include <cstring>

#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>

class Cpp98Debug {
private:
  struct StringComparator {
    bool operator( )( const char *str1, const char *str2 ) const { return strcmp( str1, str2 ) < 0; }
  };

  bool m_default;
  std::basic_ostream< char > *m_os;
  bool m_freeOs;
  std::map< const char *, bool, StringComparator > m_map;

  static Cpp98Debug *m_singleton;

protected:
  void init( std::basic_istream< char > &cfgFile ) {
    m_singleton = this;
  
    int lineNumber = 0;
    std::string line;

    while( true ) {
      ++lineNumber;
      
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
  }
  void init( const char *cfg ) {
    std::ifstream cfgFile( cfg );
  
    if( !cfgFile.is_open( ) ) {
      std::cerr << "CPP98DEBUG: Error al abrir el archivo de configuracion " << cfg << '\n';
    } else {
      init( cfgFile );
    }
  }

public:
  void flush( const char *str ) {
    ( *m_os ) << str << std::endl;
  }

  static Cpp98Debug &singleton( ) { return *m_singleton; }

  class output {
    bool m_do;
    char m_final[1024];
    char m_buff[128];

  public:
    ~output( ) {
      if( m_do ) {
        Cpp98Debug::singleton( ).flush( m_final );
      }
    }
    output( const char *module, bool d ) : m_do( d ) {
      if( d ) {
        strcpy( m_final, module );
        strcat( m_final, ": " );
      }
    }

    output &flush( ) {
      if( m_do ) {
        Cpp98Debug::singleton( ).flush( m_final );
        m_final[0] = 0;
      }
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
  // 'assertion' opcional, para depurar solo ciertos casos.
  output operator( )( const char *module, bool assertion = true ) {
    std::map< const char *, bool, StringComparator >::const_iterator iter = m_map.find( module );

    return output( module, ( ( iter == m_map.end( ) ) ? m_default : iter->second ) && assertion );
  }
  // Para ignorar lo indicado en el archivo de configuración.
  output operator( )( bool forced, const char *module, bool assertion = true ) {
    return forced ? output( module, assertion ) : output( module, false );
  }

  ~Cpp98Debug( ) {
    if( m_freeOs && m_os ) {
      ( (std::ofstream *)m_os )->close( );
      delete m_os;
    }
  }
  template< typename OT, typename CT > Cpp98Debug( std::basic_ostream< char, OT > &os, std::basic_istream< char, CT > &cfg ) :
    m_default( true ),
    m_os( &os ),
    m_freeOs( false ),
    m_map( )
  {
    init( cfg );
  }
  template< typename OT > Cpp98Debug( std::basic_ostream< char, OT > &os, const char *cfg ) :
    m_default( true ),
    m_os( &os ),
    m_freeOs( false ),
    m_map( )
  {
    init( cfg );
  }
  template< typename CT > Cpp98Debug( const char *os, std::basic_istream< char, CT > &cfg ) :
    m_default( true ),
    m_os( NULL ),
    m_freeOs( false ),
    m_map( )
  {
    std::ofstream *pos = new std::ofstream( os );
    if( !( pos->is_open( ) ) ) {
      throw std::runtime_error( "CPP98DEBUG: Error al abrir el archivo de trazas.\n" );
    }
    m_os = pos;
    m_freeOs = true;
    init( cfg );
  }
  template< typename OT > Cpp98Debug( std::basic_ostream< char, OT > &os, bool def = 1 ) :
    m_default( def ),
    m_os( NULL ),
    m_freeOs( false ),
    m_map( )
  {
    Cpp98Debug::m_singleton = this;
  }
};

#define TRACE98( LOG, MOD ) ( LOG( MOD ) << __LINE__ << ": " )
#define CTRACE98( LOG, MOD, ASSERTION ) ( LOG( MOD, ( ASSERTION ) ) << __LINE__ << ": " )

#endif
