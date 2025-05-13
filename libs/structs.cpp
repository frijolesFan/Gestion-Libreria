/* ESTRUCTURAS Y LIBRERIAS

* Contiene todas las estructuras y las librerias usadas en el proyecto. 

* Realizado por: Juan Camilo Guevara
* Fecha: 13/11/2023
* Compilado Versión: g++ (Rev7, Built by MSYS2 project) 13.1.0 [C/C++ visual studio code]
* structs.cpp
*/
#pragma once

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <Windows.h>
#include <cstdlib>
#include <math.h>
#include <vector>
#include <chrono>
using namespace std;

//tipo de variable de fecha
struct date{
    int year, month, day;
};

/*Tipo de variable para los libros*/
struct book{
    int bookID;         //identificador del libro
    char name[50];      //nombre del libro
    char auth[50];      //autor del libro
    char desc[150];     //descripcion del libro
    date release;       //fecha de publicación
    int disp;           //disponibilidad (1 libre, 0 no disponible, -1 dado de baja)
};

/*EXISTEN 3 TIPOS DE USUARIO 
1. Usuario: Puede buscar libros, pedir y devolver libros (si esta paz y salvo),
   consultar su estado de prestamo actual
2. Trabajador: Puede prestar y recibir libros
3. Administrador: Puede agregar libros, dar libros de baja y modificar libros
            --las clases superiores heredan los permisos de la anterior--*/
struct user{
    int usertype;       //tipo de usuario (1 usuario, 2 trabajador, 3 admin)

    //Inicio de sesion
    char username[17];  //nombre de usuario
    char password[17];   //contraseña

    //Informacion
    char fullname[100];    //nombre completo                    /*/*/
    int doctype;        //tipo de documento (1. CC, 2. TI)
    char DNI[20];         //n. documento                        /*/*/
    char email[100];       //correo electronico                 /*/*/
    char direct[100];      //direccion de la casa               /*/*/
    char phone[11];     //numero de telefono

    //Prestamo
    int books[3];        //libros prestados (max. 3), garda la id
    date entrega[3];     //fecha de entrega de los libros
    bool ban[3];             //si puede pedir libros prestados (0 si, -1 no)
};