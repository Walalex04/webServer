# Servidor web desde 0

El proposito de este pryecto en la programacion de un servidor web programando directamente con la api del sistema operativo, para este caso se utiliza la programacion de redes del sistema operativo Linux, la idea es ver la configuracion inicial de los struct para realizar los enlaces entre los sockets y poner a la eescuha en el puerto 8080.

Este readme sera simplemente un blog donde se ira documentando la teoria que voy estudiando y como lo implementare, luego corregire todo para documentar como se debe

Por otra parte como proposito generar es crear un proxy con el lenguaje de programacion de rust, sin embargo, es indespensable el manejo del 100% o lo mas cercano de este acerca de programacion a bajo nivel (Se esta programando en la red TCP/IP); toda la programacion se hara puramente en C para el manejo de memoria usando punteros y luego se realizara una analogia en el lenguaje de programacion rust que es el objetivo final



## Teoria Basica acerca del desarrollo de web server


### Modelo OSI

Como referencia para la teoria se considero [Modelo OSI IBM](https://www.ibm.com/es-es/think/topics/osi-model)

En sintesis, el modelo OSI es un forma de organizacion que permite gestionar el flujo de trasmision de datos mediante diferentes tipos de redes (alambricas, como inalambricas); la idea general es dividir todo el proceso en 7 capas lo que permite a los desarrolladores centrarse en cada una abstrayendo las demas, esto mejora la efinciencia del desarrollo de aplicacacion asi como tambien permite la regulacion de esta tecnologia. Como se menciona en la ultima parte, exiten muchos protocolos ya estandarizados por los desarrroladores de sotware no tiene la necesidad de gestionar el tema de encriptacion o enrutamiento de datos, si no que en cambio utilizan lo que se proporciona en la capa para la cual desarrolla; las capas previamente mencionadas seran mencionadas a continuacion con una breve descripcion, si se puede revisar el anterior enlace para mas detalle o buscar en internet.

**Capa 7 (Aplicacion):** Es la capa mas alta de abstraccion del proceso, Esta capa es la que establece el protocolo que se se va usar (para este caso http), esta capa ofrece servicios de red a aplicacion de los usuarios, mas no bien se centra en la gestion de los datos; Asimismo se asegura de otras cuestiones como por ejemplo de que el receptor pueda capturar los datos, o el sistema de auntenticacion. Los datos que se quiere transmitir, como un archivo, son enviados a la capa de presentacion.


**Capa 6 (Presentacion):** En sintesis, lo que realiza esta capa es la transformacion de los datos de tal forma que la aplicacion pueda entender (o visto desde otra perpectica que la red puesta trasmitir), esto depende del protocolo que se use en la etapa de aplicacion. Aqui tambien es donde se encripta los datos o se comprimen.

**Capra 5 (Sesion):** Por su nombre, esta es la capa que se encarga de iniciar , finalizar y administrar las sesiones entre dispositivos, manteniendo esta abierta para que lleguen los datos sin corrumpirse.

**Capa 4 (Transporte):** Esta capa utiliza 2 protocolos estandarizadas, esto no es mas como se manejan los errores en caso de perdida de datos, en esta los datos provenientes de de la anterior capa los segmenta para enviarlos por TCP o UDP, la diferencia entre estos dos es que la primera en caso de perdida se reenvia el bloque de datos de nuevo y el segundo no. Por ejemplo, cuando se descarga un archivo todos los segmentos de datos son importantes, por lo que es necesario que en caso de algun error, el servidor reenvie de nuevo los datos; por otra parte en un stremear (como live en ig) en caso de perdida de algun segmento no es necesario que este lo envie, sencillamente se pasa de el ya que no proporciona algun error critico. Ademas en esta etapa se le agrega un encabeza. Notese que asi como se segmenta los datos para enviarlos, esta misma capa reconstruye el mensaje en caso de recibirlos (remitente)

**Capa 3 (Red):**  Esta capa facilira la transferencia de los datos dentro de una red, se encarga de la mejor ruta de trasnmision entre nodos de una red; en caso de que los segmentos sean muy grandes para la transmion en esta etapa se lo divide, en esta capa es donde se utiliza el Internet Protocol v4 o v6 IPv4 o IPv6

**Capa 2 (Enlace de datos): ** Esta capa es la que gestiona la transferencia de los datos sin errores, en esta etapa es donde se utiliza la capa MAC para la trasmision, en esta se generan las tramas dependiendo de la tarjeta de interfaz de red (NIC), lease mas sobre esta capa en la referencia antes mencionada. En esta etapa es donde se suelen realizar reconocimiento de dispositivos para hacking enviando paquetes ARP 

**Capa 1 (Capa Fisica):** Esta se encarga de la conversion de datos a pulsos electricos u ondas electromagneticas, para la trasmision mediante algun medio fisico. aqui se controla los baudios, los modos de transmision (duplex, semiduples, full duplex) entre otras cosas


Por lo general existe otro modelo TCP/IP que es muy parecido al OSI con la diferencia que agrupa capas para generar un modelo con menos de esta.

Para nuestro proyecto, el sistema operativo ofrece una api para establecer conexiones entre dispositvos conectados en una red mediante IPv4 o IPv6, asi como el propia hardware de la comunicacion de la red fisica, por lo que unicamente nuestro objetivo sera trabajar en la capa de sesion y presentacion, posiblemente se ocupe ciertas caracteristicas de la etapa de transporte, pero no se podra ir a un nivel inferior debido a las restriccion del sistema operativo; Como futuro proyecto se puede utilizar un sistema embebido para la programacion de sistemas de transmision de datos desde la capa 1, obviamente, con sus restriccion por la propia definicion de un sistema embebido (la idea seria ocupar una esp32 que contiene el modulo wifi incluido pero esta ofrece ya una capa de abstraccion para este modulo) o (una raspberry programando en bare metal)


### API Network Linux

Como referencia se tomara [Beej's Guide to Network Programing](https://beej.us/guide/bgnet/pdf/bgnet_usl_c_1.pdf)

Como principal cuestion a tratar es **Que es un socket?**; En sintesis, y sin ser muy tecnicos, un sockets es un elemento del sistema operativo que inicia una comunicacion entre 2 programas por medio de una red, en si el socket es un "file" o "psudo-file" en linux que permite esta conexcion, sin embargo, no es lo unico que se requiere para establecer la conexion

Existen algunos "sockets" en linux que determinan propiedades diferentes dependiendo el uso que se le quiera dar, en la pagina [Socket Types](https://www.ibm.com/docs/en/aix/7.1.0?topic=protocols-socket-types) se puede revisar a detalle, sin embargo nosotros hablaremos sobre dos especificamente. **SOCK_STREAM**, la principal caracterista de este tipo es que permite la transmision de datos secuencialas y bidireccionales, en otras palabras esta estrechamente ligado con el protocolo TCP/IP lo que es fundaemental para la creacion deeste tipo de servidores, por ejemplo servidores que utilizan el protocolo http. Por otro lado se tiene el **SOCK_DGRAM** que es el User Datagram Program o UDP

Ahora hablaremos del tema mas importa que son los **strucs**; estos permiten la configuracion principal para establecer la comunicacion entre 2 programas. Empecemos a hablar del primer **struct addrinfo**, este usualmente se utiliza para inicializar o preparar ciertos parametros importante, ya que aveces se quiere resolver un dominio.

```
struct addrinfo {
    int ai_flags; // AI_PASSIVE, AI_CANONNAME, etc.
    int ai_family; // AF_INET, AF_INET6, AF_UNSPEC
    int ai_socktype; // SOCK_STREAM, SOCK_DGRAM
    int ai_protocol; // use 0 for "any"
    size_t ai_addrlen; // size of ai_addr in bytes
    struct sockaddr *ai_addr; // struct sockaddr_in or _in6
    char *ai_canonname; // full canonical hostname
    struct addrinfo *ai_next; // linked list, next node
};

```

Lo que se suleve hacer con esta estructura es colocar ciertos valores para luego llamar a la funcion **getaddrinfo()** y obtener la estructura completa, notese que se la estrcutura presenta un puntero hacia la siguiente direccion, esto es debido a que actualemnente un dominio suele estar vinculado con IPv4 e IPv6. Otra estructura es el **struct sockaddr** que almacena informacion del tipo de ip y, la adress y el puerto como un array de 14 bytes

```
struct sockaddr {
    unsigned short sa_family; // address family, AF_xxx
    char sa_data[14]; // 14 bytes of protocol address
};

```

no obstante, algunos programadores han desarrollado estrcutaras para ipv4 y ipv6 por separado, estos estan estechamente ligados con el anterior lo que permite realizar un casting sencillo.

**Para IPv4**

```
struct sockaddr_in {
    short int sin_family; // Address family, AF_INET
    unsigned short int sin_port; // Port number
    struct in_addr sin_addr; // Internet address
    unsigned char sin_zero[8]; // Same size as struct sockaddr
};

```

```
struct in_addr {
    uint32_t s_addr; // that's a 32-bit int (4 bytes)
};
```

**Para IPv6**


```
struct sockaddr_in6 {
    u_int16_t sin6_family; // address family, AF_INET6
    u_int16_t sin6_port; // port, Network Byte Order
    u_int32_t sin6_flowinfo; // IPv6 flow information
    struct in6_addr sin6_addr; // IPv6 address
    u_int32_t sin6_scope_id; // Scope ID
};

```
```
struct in6_addr {
    unsigned char s6_addr[16]; // IPv6 address
};
```

Por otra parte existe una estructura que permite almacenar la ip sin saber con anticipacion el tipo de ip que se esta trabajando


```
struct sockaddr_storage {
    sa_family_t ss_family; // address family
    // all this is padding, implementation specific, ignore it:
    char __ss_pad1[_SS_PAD1SIZE];
    int64_t __ss_align;
    char __ss_pad2[_SS_PAD2SIZE];
};
```


