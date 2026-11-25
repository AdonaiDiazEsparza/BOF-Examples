# BOF-Examples
Estos son ejemplos de BOF (Beacon Object File) creados para la conferencia de BUGCON 2026, dentro de la conferencia para la villa de APT. 

## Template de BOF
Para la compilación y desarrollo de BOF, se utiliza el template oficial de Cobalt Strike, este podemos obtenerlo en desde su [repositorio](https://github.com/Cobalt-Strike/bof-vs). 

Si tienes Visual Studio, puedes descargar el [template comprimido](https://github.com/Cobalt-Strike/bof-vs/releases) (.zip) y copiarlo y pegarlo en la siguiente ubicación de windows ```C:\Users\User\Documents\Visual Studio 2022\Templates\ProjectTemplates```, no es necesario que lo descomprimas.

## Para realizar pruebas
No necesitas tener el agente de Cobalt Strike para realizar las pruebas, gracias a TrustedSec, nos otorga un [programa](https://github.com/trustedsec/coffloader) para ejecutar archivos COFF (Common Object File Format). 

Para usarlo es necesario que lo compiles en linux con ```make```. 

Cuando pases el ejecutable a tu windows, asegurate de que sea en una carpeta que tenga excepción de Windows defender ya que lo tomará como un virus.

