## Descripción
El cliente funciona tanto para IPv4 como para IPv6, al conectarse se da la opción de usar ssl.
Por defecto, los parámetros son los siguientes:
``` c++
ssl = false
ipv6 = false
direccion = "163.178.104.62"
figure = "whale-1.txt"
```
## Uso
Se debe compilar el programa, para esto se utiliza el comando ```make```, el ejecutable es ```client.out```.  

Los argumentos pueden ser dados en cualquier orden y pueden ser omitidos, si se omiten se utiliza el valor por defecto, a continuación algunos casos de uso y su respectiva salida:
``` c++
root@user: client.out whale-1.txt ssl ose ipv4

Se utiliza la siguiente direccion: 163.178.104.62
IPv6: No
SSL: Si

GET /aArt/index.php?disk=Disk-01&fig=whale-1.txt HTTP/1.1
Host: os.ecci.ucr.ac.cr
Connection: close


178 bytes leidos
255 bytes leidos
160 bytes leidos 

              __ \ / __
             /  \ | /  \
                 \|/
            _,.---v---._
   /\__/\  /            \
   \_  _/ /              \
     \ \_|           @ __|
      \                \_
       \     ,__/ 2025  /
     ~~~`~~~~~~~~~~~~~~/~~~~
```
``` c++
root@user: client.out ssl ose ipv4 rabbit.txt

Se utiliza la siguiente direccion: 163.178.104.62
IPv6: No
SSL: Si

GET /aArt/index.php?disk=Disk-01&fig=rabbit.txt HTTP/1.1
Host: os.ecci.ucr.ac.cr
Connection: close


178 bytes leidos
185 bytes leidos

(\_╨_/)
(='.'=)
(")_(")
```
``` c++
root@user: client.out ipv4 osi  seahorse.txt ssl

Se utiliza la siguiente direccion: 10.84.166.62
IPv6: No
SSL: Si

GET /aArt/index.php?disk=Disk-01&fig=seahorse.txt HTTP/1.1
Host: os.ecci.ucr.ac.cr
Connection: close


179 bytes leidos
255 bytes leidos
255 bytes leidos
255 bytes leidos
255 bytes leidos
225 bytes leidos

                   ,   /^\     ___
                  /^\_/   `...'  /`
               ,__\    ,'     ~ (
            ,___\ ,,    .,       \
             \___ \\\ .'.'   .-.  )
               .'.-\\\`.`.  '.-. (
              / (==== ."".  ( o ) \
            ,/u  `~~~'|  /   `-'   )
           "")^u ^u^|~| `""".  ~_ /
             /^u ^u ^\~\     ".  \\
     _      /u^  u ^u  ~\      ". \\
    ( \     )^ ^U ^U ^U\~\      ". \\
   (_ (\   /^U ^ ^U ^U  ~|       ". `\
  (_  _ \  )U ^ U^ ^U ^|~|        ". `\.
 (_  = _(\ \^ U ^U ^ U^ ~|          ".`.; 
(_ -(    _\_)U ^ ^ U^ ^|~|            ""
(_    =   ( ^ U^ U^ ^ U ~|
(_ -  ( ~  = ^ U ^U U ^|~/
 (_  =     (_^U^ ^ U^ U /
  (_-   ~_(/ \^ U^ ^U^,"
   (_ =  _/   |^ u^u."
    (_  (/    |u^ u.(
     (__/     )^u^ u/
             /u^ u^(
            |^ u^ u/
            |u^ u^(       ____
            |^u^ u(    .-'    `-,
             \^u ^ \  / ' .---.  \
              \^ u^u\ |  '  `  ;  |
               \u^u^u:` . `-'  ;  |
                `-.^ u`._   _.'^'./
                   "-.^.-```_=~._/
                      `"------"'
```
``` c++
root@user: client.out ipv4 osi  seahorse.txt ssl fifafo

Argumento no reconocido: fifafo
```
Para utilizar y probar IPv6, sin ssl, puede hacer uso del servidor de python (en la carpeta Server Script) provisto en el curso, se debe cambiar el puerto en el cliente de ```80``` a ```8080``` y la dirección ```os6``` en el diccionario de ```"fe80::8f5a:e2e1:7256:ffe3%enp0s31f6"``` a ```::1```, estos cambios se dan en ```client.cc``` en las lineas ***23*** y ***77***, se da una salida como la siguiente, donde se conecta pero no encuentra el elemento: 
``` c++
root@user: client.out ipv6
Se utiliza la siguiente direccion: ::1
IPv6: Si
SSL: No

GET /aArt/index.php?disk=Disk-01&fig=whale-1.txt HTTP/1.1
Host: os.ecci.ucr.ac.cr
Connection: close


Elemento no disponible.
```