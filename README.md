# Proyecto BD2

- [Introducción](#introducción)
  - [Objetivos](#objetivos)
    - [Principal](#principal)
    - [Secundarios](#secundario)
  - [Dataset](#dataset)
- [Técnicas de indexación](#tecnicas-de-indexación)
  - [Sequential File](#sequential-file)
    - [Elementos de ayuda](#elementos-de-ayuda)
      - [Variables globales](#variables-globales)
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
struct Record {
    char key[40];
    char id[200];
    char name[100];
    char popularity[10];
    char duration_ms[15];
    char explicit_[5];
    char artists[200];
    char id_artists[100];
    char release_date[15];
    char danceability[10];
    char energy[10];
    char loudness[10];
    char mode[5];
    char speechiness[10];
    char acousticness[10];
    char instrumentalness[10];
    char liveness[10];
    char valence[10];
    char tempo[10];
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

**[COLOCAR IMAGEN]**

>**Implementación**: Para la implementación de este algoritmo, se uso la técnica de un espacio auxiliar alfinal de cada registro el cual apunta al siguiente puntero que sea su consecuente de modo que los punteros siempre estaran ordenados apuntando a uno mayor que este el cual se calculca en base al campo principal (PK).



# Autores

|                     **Esteban Vasquez Grados**                   |                                 **Darío Ricardo Nuñes Villacorta**                                 |                       **Yamileth Yarel Rincón Tejada**                     |  **Flavia Ailen Mañuico Quequejana** |   **Maria Fernanda Surco Vergara**  |
|:----------------------------------------------------------------------------------:|:-----------------------------------------------------------------------------------:|:-----------------------------------------------------------------------------------:|:-----------------------------------------------------------------------------------:|:----:|



