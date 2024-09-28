# Proyecto BD2

- [Introducción](#introducción)
  - [Objetivos](#objetivos)
    - [Principal](#principal)
    - [Secundarios](#secundario)
  - [Dataset](#dataset)
    - [Generación de registros en binario](#generación-de-registros-en-binario)
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




# Autores

|                     **Esteban Vasquez Grados**                   |                                 **Darío Ricardo Nuñes Villacorta**                                 |                       **Yamileth Yarel Rincón Tejada**                     |  **Flavía Ailen Mañuico Quequejana** |   **Maria Fernanda Surco Vergara**  |
|:----------------------------------------------------------------------------------:|:-----------------------------------------------------------------------------------:|:-----------------------------------------------------------------------------------:|:-----------------------------------------------------------------------------------:|:----:|



