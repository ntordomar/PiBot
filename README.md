[![✗](https://img.shields.io/badge/PiBot_License-v0.2.0-blue)]



# Compilador Flex/Bison

Un compilador de SQL construido con Flex y Bison.

## Requerimientos

Para construir el compilador, se requieren las siguientes dependencias:

* [Bison v3.8.2](https://www.gnu.org/software/bison/)
* [CMake v3.24.1](https://cmake.org/)
* [Flex v2.6.4](https://github.com/westes/flex)
* [GCC v11.1.0](https://gcc.gnu.org/)
* [Make v4.3](https://www.gnu.org/software/make/)

Si en lugar de trabajar con un entorno _Linux_, se está construyendo el proyecto sobre un entorno _Microsoft Windows_, se debe instalar _Microsoft Visual Studio 2022_ con las extensiones para desarrollar aplicaciones en _C/C++_, así como también las herramientas requeridas, con excepción del compilador _GCC_ y la herramienta _Make_.

## Construcción


## Ejecución
- Dentro del archivo program ubicado en la root del proyecto escribir la consulta que se desee compilar
- Ingresar al directorio 'script', y ejecutar program.sh
- dentro de la carpeta output encontraras un archivo output.tex, el cual contendra el algebra relacional de la consulta


## Testing
Para correr los tests parado desde el directorio 'script' ejecutar ./piBot=test.sh
En caso de querer obtener los .tex de todos los tests aceptados, ejecutar tex-test.sh desde el directorio 'script'