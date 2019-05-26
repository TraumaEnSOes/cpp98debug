# cpp98debug

Mínimo sistema de trazas mediante archivo de configuración, para C++98

Por temas de trabajo, necesita un sistema en el que poder realizar ciertas trazas en un archivo propio. Además, necesitaba que
fuera configurable mediante un archivo externo, para no tener que estar recompilando continuamente. Y, a ser posible, que no fuera
necesario tocar los archivos de dependencias del proyecto.

El resultado es este. Sistema mínimo (solo un .hpp), que lee su configuración desde el archivo indicado.

La configuración es muy sencilla, simple, y extremadamente burda:

1|0 TEXTO

El TEXTO puede ser cualquier cadena; '1' indica que las trazas asociadas a esa cadena SI se volcarán al archivo, y '0' indica
que dichas trazas NO se volcarán al archivo.

NO PUEDE EXISTIR NINGUNA SEPARACIÓN entre el '0' o '1' y el TEXTO. Se ve mejor en este ejemplo de configuración:

1*
0mimodulo

En este ejemplo, las trazas asociadas a 'mimodulo' no se vuelcan.

El texto especial '*' (asterisco) indica 'cualquier texto no indicado'. Es un valor por defecto para cualquier elemento no indicado
directamente en el archivo de configuración.
