# Proyecto BD2

- [Introducción](#introducción)
  - [Objetivos](#objetivos)
    - [Principal](#principal)
    - [Secundarios](#secundario)
  - [Dataset](#dataset)
- [Técnicas de indexación](#tecnicas-de-indexación)
  - [Sequential File](#sequential-file)
    - [Elementos de ayuda](#elementos-de-ayuda)
    - [`Class Sequential_File`: Constructor](#class-sequential_file-constructor)
    - [`Class Sequential_File`: Funciones](#class-sequential_file-funciones)
    - [Código de prueba](#código-de-prueba)
    - [Análisis de complejidad](#análisis-de-complejidad)
    

- [Pruebas de uso y presentación](#Pruebas-de-uso-y-presentación)
- [Autores](#autores)


# Introducción

- Objetivo General
Desarrollar un proyecto integral de base de datos que soporte tanto el modelo relacional
basado en tablas como técnicas avanzadas de recuperación de información basadas en el
contenido de documentos textuales y objetos multimedia.

- Objetivo especifico
Comprender y aplicar de manera efectiva las técnicas de organización e indexación de
archivos físicos con el fin de desarrollar un mini gestor de bases de datos, que optimice la
gestión, almacenamiento y recuperación de datos estructurados dentro de un modelo
relacional basado en tablas.

## Dataset

Se usaron 2 dataset publicados en Kagle, uno contiene canciones de spotify y el otro son de datos de bicicletas.

Eligimos estos dataset ya que contienten alrededor de 500000 registros y 12 atributos, los cuales unicamente se 
filtraron los datos vacíos para evitar problemas con los algoritmos implementados.

> Esto nos ayudara para testear la capacidad de nuestros algoritmos.

### Dataset 1 (Spotify Dataset)

Spotify Dataset 1921-2020, 600k+ Tracks.

 ```c++
struct Record1 {
  char id[23];
  char name[529];
  char popularity[3];
  char duration_ms[7];
  char explicit_[1];
  char artists[934];
  char id_artists[1508];
  char release_date[10];
  char danceability[6];
  char energy[8];
  char key_str[2];
  char loudness[7];
  char mode[1];
  char speechiness[6];
  char acousticness[8];
  char instrumentalness[8];
  char liveness[7];
  char valence[8];
  char tempo[7];
  char time_signature[1];

  long left = -1;
  long right = -1;
  int height = 1;
  Record1(){};

};
 ```

|    **Campo**    |                            **Descripción**                                          | 
|:---------------:|:-----------------------------------------------------------------------------------:|
| ```id```        | Id unique.             |  
| ```name```  | Name of track | 
| ```popularity``` | Popularity of track in range 0 to 100. |
| ```duration_ms``` | Duration of songs in ms.|
| ```explicit``` | Artists who created the track. |
| ```id_artists``` | Id of artists who created the track. |
| ```...``` | ... |
| ```tempo``` | The overall tempo of track in BPM. |

### Dataset 2 (Bike Sales)

El conjunto de datos trata sobre la compra de bicicletas por parte de los usuarios. Contiene 100.000 registros. Este conjunto de datos recoge transacciones de venta detalladas de una cadena de tiendas de bicicletas, con 100.000 registros únicos de ventas de bicicletas. Cada fila representa una única transacción de venta e incluye información sobre el cliente, el modelo de bicicleta, los detalles de la venta y la ubicación de la tienda.

 ```c++
struct Record2 {
    char id[7];
    char date[10];
    char customer[4];
    char model[13];
    char price[7];
    char quantity[2];
    char storelocation[12];
    char salespersonid[3];
    char paymentmethod[11];
    char customerage[3];
    char customergender[6];
};
 ```

|    **Campo**    |
|:---------------:|
| ```id```        | 
| ```Date```  |
| ```Customer_ID``` | 
| ```Bike_Model``` |
| ```Price``` | 
| ```Quantity``` | 
| ```...``` | ... |
| ```Customer_Age``` |


# Técnicas de indexación
## Sequential File
Esta técnica de organización se caracteriza, principalmente, por su forma de organizar y almacenar los registros en un archivo de forma secuencial, es decir, uno detrás de otro. Cada record tiene un puntero que apunta al siguiente registro (***posición lógica***), y el último registro apunta a un valor nulo.

### Elementos de ayuda

- Se utilizaron 2 espacios adicionales en cada registro el cual nos ayudara a buscar el siguiente registro mayor que le sigue (ordenados por una key), estos se almacenan
en una variable ```siguientePos``` de tipo int y un ``esAux`` de tipo bool el cual nos indica si el siguiente elemento esta en el espacio auxiliar o en el principal.

**[COLOCAR IMAGEN]**

>**Implementación**: Para la implementación de este algoritmo, se uso la técnica de un espacio auxiliar alfinal de cada registro el cual apunta al siguiente puntero que sea su consecuente de modo que los punteros siempre estaran ordenados apuntando a uno mayor que este el cual se calculca en base al campo principal (PK).

1. Función add:

    **Descripción:** Esta función inserta un nuevo registro en el archivo auxiliar, luego actualiza los punteros del registro anterior en la secuencia, y finalmente, si el tamaño del archivo auxiliar excede ```log(MainSize/2)```, reconstruye el indice (llama a la función rebuild).
  
    **Complejidad:** La parte inicial de la función implica recorrer los registros del archivo principal(en caso exista un espacio liberado con -2) o auxiliar hasta encontrar la posición adecuada. Esto tiene una complejidad de ``O(n+m)``, donde n es el número de registros en total y m el número de registros en el espacio auxiliar.
  
    La operación de escritura en el archivo auxiliar toma un tiempo constante ``O(1)``, ya que el nuevo registro se agrega al final del archivo auxiliar. 
  
    **Complejidad total:** ``O(n+m)``, ya que en el peor de los casos se debe recorrer todo el archivo principal o auxiliar antes de insertar el nuevo registro.

2. Función search:

    **Descripción:** Esta función busca un registro con una clave específica. Empieza en el archivo principal y sigue los punteros hasta encontrar el registro o llegar al final de la secuencia.

    **Complejidad:** La búsqueda en secuencia recorre los registros del archivo principal y, si es necesario, del archivo auxiliar. En el peor de los casos, puede ser necesario recorrer todos los registros. Dado que la búsqueda es lineal, la complejidad es ``O(n)`` en el peor caso.

    Complejidad total: ``O(n)``.

3.   Función insertAll:

     **Descripción:** Esta función inserta todos los registros iniciales desde un vector en el archivo principal. Los registros se ordenan antes de ser escritos en el archivo.
  
     **Complejidad:** Primero, el vector de registros se ordena, lo cual tiene una complejidad de ``O(m log m)``, donde m es el número de registros en el vector. Luego, los registros se escriben en el archivo principal. Esto tiene una complejidad de ``O(m)``.
  
     **Complejidad total:** ```O(m log m)```, donde m es el número de registros iniciales.

4. Función rangeSearch:

    **Descripción:** Busca todos los registros cuyas claves estén en un rango dado. Recorre secuencialmente los registros del archivo principal y auxiliar.

    **Complejidad:**

   Al igual que en la función search, los registros se recorren de manera secuencial. En el peor caso, puede ser necesario recorrer todos los registros. Dado que no se hace un salto directo al inicio del rango, la complejidad es ``O(n)``.
   **Complejidad total:** ``O(n)``.

6.   Función rebuild:
     **Descripción:** Esta función reconstruye el archivo principal cuando el archivo auxiliar alcanza un tamaño mayor a ```log(MainSize/2)```. Reordena todos los registros y elimina el archivo auxiliar(se vacía), se crea un archivo temporal donde se colocan para luego renombrarlo por el archivo inicial.
  
     **Complejidad:** Se recorre todo el archivo principal y el archivo auxiliar para leer todos los registros, lo que tiene una complejidad de ``O(n)``, Luego, se escriben todos los registros de nuevo en el archivo principal, lo que tiene una complejidad de ``O(n)``.
  
     **Complejidad total:** ``O(n)``.


## AVL
La clase implementa un árbol balanceado donde la diferencia entre raama izquierda y derecha no supera el 1 0 -1.
**Estructura principal**
*Avl File: Contiene un long que piene ser la raiz, y tambien se le agregó 3 atributos a los records, left, right y heigh, que vienen a ser 2 refencias a los hijos del nodo y su altura

### Funciones: 
1. Add (Inserción)
     Se inserta un nuevo registro de datos, si la raiz es -1, se inserta al final del archivo, en caso contrario, se va recorriendo el árbol de manera recursiva comparando el key para poder ir bajando por el árbol. Despues de cada inserción, se realiza el balanceo, donde se realizan las rotaciones respectivas para que el factorr de balanceo no exceda a la unidad.
   
   **Complejidad:** En el mejor de los casos tiene complejidad O(1) es decir que esta vació y en el pero O(log(n))

2. Find (Búsqueda)
     Para la búsqueda se hace un proceso similar al del insert, donde se va comparando recursivamente hasta encontrar la posición respectiva, en caso que sea -1, devuelve falso, ya que, no existe el nodo a buscar.
   
   **Complejidad:** En el mejor de los casos es complejidad 0(1) (la raíz) y en el peor de los casos es complejidad 0(log(n))

3. Remove (Eliminación)
   Se busca el registro, comparando respectivamente los keys hasta encontrarlos. Una vez encontrado pregunta si alguno de sus hijos es nulo y si es asi, se intercambio por ese, caso contrario, se busca al minimo del subarbol izquierdo para que lo reemplace. Una vez hecho esto se realiza la actualización de las alturas y los balanceos respectivos para que se cumpla el factor de balanceo.
   
   **Complejidad:** En el peor de los casos tiene complejidad 0(log(n)), ya que, tiene que ir recorriendo el árbol, y en el mejor 0(1) habiendo solo 1 nodo en el árbol.
   
4. rangeSearch (Búsqueda por rango)
   Primero se verifica que los keys de begin y end existan en el AVL, caso contario, se busca el succesor del begin y/o el predecessor del end, esto se hace con un stack y un queue. Luego, se va recorriendo al árbol de manera recursiva, añadiendo a un vector los       registros que cumplar estar entre el rango.
   
   **Complejidad:** Este algoritmop tiene una complejidad muy costosa, siendo esta O(log(n)) + 0(k) + O(m), siendo k los k nodos recorridos para encontrar el begin si no se encuentra y m los m nodos recorridos hasta encontrar el end si es que no se encontrara y en el mejor de los casos   tiene complejidad 0(log(n))

## ISAM - SPARSE INDEX
La clase ISAM implementa un método de acceso secuencial indexado en dos niveles para gestionar grandes volúmenes de datos. La clase divide los datos en páginas y utiliza índices jerárquicos para realizar operaciones eficientes sobre el almacenamiento de datos.

**Estructuras principales:**
* PageData: Contiene un arreglo de registros con tamaño fijo (N = 100), una cuenta de registros (count), y un puntero (nextPage) para manejar páginas desbordadas.
* PageIndex: Contiene las claves (keys) para identificar páginas y punteros (pages) a páginas en niveles inferiores. También tiene una cuenta de claves (count).

### Funciones principales:
1.  add (Insertar)

    **Descripción:** Inserta un nuevo registro en el archivo de datos, actualizando las páginas de índices si es necesario. Si una página de datos está llena, se maneja a través del enlace de desbordamiento sin dividir la página.

    **Complejidad:** En promedio es O(logn) para localizar la página correcta a través de los índices y O(1) para insertar en la página. En el peor caso, si hay desbordamiento y hay que enlazar nuevas páginas, se mantiene dentro de O(logn).

2.  Remove(Eliminar)

    **Descripción:** Elimina un registro de la página de datos correspondiente y actualiza los índices si es necesario. Si se eliminan suficientes registros de una página, no es necesario compactar inmediatamente, pero se puede optimizar el espacio en futuras operaciones.
    
    **Complejidad:** O(logn) para localizar la página correcta y O(1) para eliminar el registro, manteniéndose en el peor caso dentro de O(logn).

3. Search(Búsqueda)
    
    **Descripción:** Busca un registro en el archivo de datos usando las claves en las páginas de índices para localizar la página de datos correcta.

    **Complejidad:** O(logn), donde n es el número de páginas en el sistema. La búsqueda primero navega por los índices y luego localiza el registro en la página.

4. RangeSearch(Búsqueda por rango)
   
    **Descripción:** Devuelve todos los registros dentro de un rango de claves. Navega por los índices para encontrar el inicio del rango y luego recorre secuencialmente las páginas de datos hasta alcanzar el final del rango.

    **Complejidad:** O(logn+m), donde m es el número de registros que caen dentro del rango.


Cada una de estas operaciones aprovecha la estructura de índices para mantener un acceso eficiente y rápido, minimizando el número de accesos a disco, lo que es crucial para grandes volúmenes de datos.

# Parser

Implementamos un analizador (Parser) y un Scánner para procesar una versión simplificada de SQL (Structured Query Language).

## Componentes principales
1. **Token:**

   - Representa las diferentes unidades léxicas (tokens) que se pueden reconocer en el texto de entrada.
   - Cada token tiene un tipo (definido por un enumerado Type) y un lexema (una cadena de texto que representa el token en el código fuente).
2. **Scanner:**

   - La clase Scanner es responsable de leer el texto de entrada y dividirlo en tokens.
   - Contiene un mapa (reserved) que almacena palabras reservadas en SQL y sus tipos correspondientes.
   - Métodos importantes:
     - nextToken(): Lee caracteres del texto de entrada y genera el siguiente token.
     - isNumber(): Verifica si una cadena representa un número.
     - checkReserved(): Comprueba si un lexema es una palabra reservada.
3. **Parser:**

   - La clase Parser interpreta los tokens generados por el Scanner y verifica su sintaxis.
   - Métodos importantes:
   - parse(): Inicia el proceso de análisis sintáctico.
   - parseStatement(): Interpreta diferentes tipos de sentencias SQL como SELECT, CREATE, INSERT, y DELETE.
   - parse_v_s(), parse_v_l(), y parserVals(): Manejan la sintaxis para insertar valores en una tabla.

## Funcionalidades

1. Análisis Léxico:

 - El escáner convierte el texto SQL en tokens que el parser puede interpretar.
 - Ignora espacios en blanco y saltos de línea, y reconoce palabras reservadas y nombres de identificadores.

2. Análisis Sintáctico:
   
 - El parser utiliza los tokens generados por el escáner para construir una representación estructurada de las sentencias SQL.
 - Cada tipo de sentencia tiene su propio manejo, lo que permite construir un árbol de sintaxis abstracta o realizar acciones específicas en función de la sentencia.

3. Validación de Sintaxis:

 - El parser verifica que las sentencias SQL estén correctamente formadas, emitiendo errores de sintaxis cuando se encuentra un token inesperado o mal colocado.

4. Manejo de Tablas:

 - El parser tiene la capacidad de verificar la existencia de tablas mediante la función existeTabla() y de almacenar los valores que se insertan en tablas.

5. Almacenamiento de Valores:

 - Al insertar datos, los valores se almacenan en un vector de pares, donde cada par consiste en el valor y su tipo correspondiente.

## Errores y Mensajes
- El código incluye mensajes de error que se muestran al usuario cuando se encuentra un problema en el análisis sintáctico, como caracteres no reconocidos o errores en la formación de las sentencias SQL.


# Autores

|                     **Esteban Vasquez Grados**                   |                                 **Darío Ricardo Nuñes Villacorta**                                 |                       **Yamileth Yarel Rincón Tejada**                     |  **Flavia Ailen Mañuico Quequejana** |   **Maria Fernanda Surco Vergara**  |
|:----------------------------------------------------------------------------------:|:-----------------------------------------------------------------------------------:|:-----------------------------------------------------------------------------------:|:-----------------------------------------------------------------------------------:|:----:|



