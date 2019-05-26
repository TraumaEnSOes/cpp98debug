#include "cpp98debug.hpp"

Cpp98Debug *Cpp98Debug::m_singleton = NULL;

int main( ) {
  Cpp98Debug log( ::std::cout, "test.cfg" );

  log( "test" ) << "hola" << " " << "mundo !" << '\n';
  TRACE98( log, "trace" ) << "aqui con numero de linea" << '\n';
  log( "mimodulo" ) << 12 << ", " << 2.0f << ", " << 5.0 << '\n';
  log( "mimodulo2" ) << 12 << ", " << 2.0f << ", " << 5.0 << '\n';
}

