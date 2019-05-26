# cpp98debug

Mínimo sistema de trazas mediante archivo de configuración, para C++98

Por temas de trabajo, necesita un sistema en el que poder realizar ciertas trazas en un archivo propio. Además, necesitaba que
fuera configurable mediante un archivo externo, para no tener que estar recompilando continuamente. Y, a ser posible, que no fuera
necesario tocar los archivos de dependencias del proyecto.

El resultado es este. Sistema mínimo (solo un .hpp), que lee su configuración desde el archivo indicado.

La configuración es muy sencilla, simple, y extremadamente burda:

TEXTO 1|0

El TEXTO puede ser cualquier cadena; '1' indica que las trazas asociadas a esa cadena SI se volcarán al archivo, y '0' indica
que dichas trazas NO se volcarán al archivo.

NO PUEDE EXISTIR NINGUNA SEPARACIÓN entre el TEXTO y el '0' o '1'. Se ve mejor en este ejemplo de configuración:

Entrada/UDP/MONI1
Entrada/UDP/CFGP0
*0

En este ejemplo, las trazas asociadas a 'Entrada/UDP/MONI' si se vuelcan.

El texto especial '*' (asterisco) indica 'cualquier texto no indicado'. Es un valor por defecto para cualquier elemento no indicado
directamente en el archivo de configuración.
