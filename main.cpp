#include "cpp98debug.hpp"

Cpp98DebugBase *Cpp98DebugBase::m_singleton = NULL;

int main( ) {
  Cpp98Debug log( ::std::cout, "test.cfg" );

  log( "test" ) << "hola" << " " << "mundo !" << '\n';
  TRACE98( log, "trace" ) << "aqui con numero de linea" << '\n';
  log( "mimodulo" ) << 12 << ", " << 2.0f << ", " << 5.0 << '\n';
}

