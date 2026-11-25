# BOF-Examples
Estos son ejemplos de BOF (Beacon Object File) creados para la conferencia de BUGCON 2026, dentro de la conferencia para la villa de APT. 

## Template de BOF
Para la compilación y desarrollo de BOF, se utiliza el template oficial de Cobalt Strike, este podemos obtenerlo en desde su [repositorio](https://github.com/Cobalt-Strike/bof-vs). 

Si tienes Visual Studio, puedes descargar el [template comprimido](https://github.com/Cobalt-Strike/bof-vs/releases) (.zip) y copiarlo y pegarlo en la siguiente ubicación de windows ```C:\Users\User\Documents\Visual Studio 2022\Templates\ProjectTemplates```, no es necesario que lo descomprimas.

## Para realizar pruebas
No necesitas tener el agente de Cobalt Strike para realizar las pruebas, gracias a TrustedSec, nos otorga un [programa](https://github.com/trustedsec/coffloader) para ejecutar archivos COFF (Common Object File Format). 

Para usarlo es necesario que lo compiles en linux con ```make```. 

Cuando pases el ejecutable a tu windows, asegurate de que sea en una carpeta que tenga excepción de Windows defender ya que lo tomará como un virus.

## Reglas Para que los BOF funcionen
Al ser archivos COFF, los BOF siguen ciertas reglas, ya que no se pueden programar como un programa ```.exe``` convencional.

1. No existen variables Globales. Esto es debido a que ciertas secciones de memoria del objeto no deben se usan o no deben existir, a lo que generaran problemas al momento de cargar el ejecutable.

Incorrecto
```C
char variableB = 1;

void go()
{
    ...
}
```

Correcto
```C
void go()
{
    char variableB = 1;
}
```

### <b>Tampoco puedes usar variables staticas ``` static char ```</b> 

2. No puedes usar literales de cadenas. No puedes definir una variable de cadena por medio de un puntero, ejemplo:

Incorrecto
```C
char* palabra = "Hola";
```

Correcto
```C
char palabra[5] = {'H', 'o', 'l', 'a', 0}; 
```

3. No puedes hacer llamada de funciones a WinAPI a través de inclusiones estandar. Estas se declaran por una macro que generan en el template, pero no hace la llamada directamente a la API.


Incorrecto
```c
int var = GetLastError();
```

Correcto
```c
DFR(KERNEL32, GetLastError); // Dependiendo donde lo uses puede ser DFR_LOCAL
#define GetLastError KERNEL32$GetLastError
...

int var = GetLastError(); // Puede ser directamente con KERNEL32$GetLastError()

```

## Más ejemplos
Aqui estan multiples repositorios que tienen ejemplos de BOF, seguramente algunos los tienes que adaptar para que funcionen con el template de Windows. Aunque también si te las ingenias puedes compilar los ejemplos directamente:

- https://github.com/Ap3x/Beacon-Object-File-Library
- https://github.com/trustedsec/CS-Situational-Awareness-BOF
- https://github.com/ajpc500/BOFs
- https://github.com/chryzsh/redteam-bofs