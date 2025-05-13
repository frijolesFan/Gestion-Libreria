/* MANEJO DE DATOS

* Funcion para el manejo de archivos, es decir, guarda y lee los datos del programa
por separado, como lo son los usuarios, los libros y las solicitudes.
Además incluye fuciones para verificar la existencia de un archivo para su lectura
o creación o una funcion abortar que funciona para meterse en un interstitial en
caso de errores en archivos

* Realizado por: Juan Camilo Guevara
* Fecha: 17/11/2023
* Compilado Versión: g++ (Rev7, Built by MSYS2 project) 13.1.0 [C/C++ visual studio code]
* main.cpp
*/
#pragma once

#include "../libs/database.cpp"
#include "../libs/structs.cpp"
#include "../libs/formats.cpp"
#include <fstream>

void abortar(){
    exit(0);
}

/// @brief Verifica si un archivo existe, de ser asi lo lee, sino, lo crea
/// @param fileName Direccion del archivo a verificar
/// @param save Funcion para guardar
/// @param load Funcion para leer
void verifyFile(string fileName, void (*save)(void), void (*load)(void)){
    ifstream test(fileName);
    if (test.is_open()){
        test.close();
        load();
    } else {
        save();
    }
}

/// @brief Funcion para guardar los usuarios
void saveUsers(){
    ofstream file("data/users.dat", ios::binary);

    if (!file.is_open()) {
        return interstitial(rojo, "Error al guardar los usuarios", abortar);
    }

    for (const auto& u : users) {                   //itera sobre el vector y escribe los datos en el archivo
        file.write(reinterpret_cast<const char*>(&u), sizeof(user));
    }

    file.close();
}

/// @brief Funcion para cargar los usuarios
void loadUsers(){
    ifstream file("data/users.dat", ios::binary);

    if (!file.is_open()) {
        return interstitial(rojo, "Error al cargar los usuarios", abortar);
    }

    users.clear();  //limpia el vector actual para ingresar los nuevos datos

    user tempUser;  //usuario temporal donde se almacenara la informacion

    while (file.read(reinterpret_cast<char*>(&tempUser), sizeof(user))) {
        users.push_back(tempUser);
    }

    file.close();
}

/// @brief Permite guardar los libros
void saveBooks(){
    ofstream file("data/books.dat", ios::binary);

    if (!file.is_open()) {
        return interstitial(rojo, "Error al guardar los libros", abortar);
    }

    for (const auto& b : books) {                   //itera sobre el vector y escribe los datos en el archivo
        file.write(reinterpret_cast<const char*>(&b), sizeof(book));
    }

    file.close();
}

/// @brief Permite leer los libros
void loadBooks(){
    ifstream file("data/books.dat", ios::binary);

    if (!file.is_open()) {
        return interstitial(rojo, "Error al cargar los libros", abortar);
    }

    books.clear();  //limpia el vector actual para ingresar los nuevos datos

    book tempBook;  //usuario temporal donde se almacenara la informacion

    while (file.read(reinterpret_cast<char*>(&tempBook), sizeof(book))) {
        books.push_back(tempBook);
    }

    file.close();
}

/// @brief Guarda toda la información de solicitudes
void saveReqs(){
    ofstream file1("data/req/rusers.dat");              //crea los tres archivos a manejar, los nombres se manejan en archivo de texto ya que
    ofstream file2("data/req/rdates.dat", ios::binary); //se almacenan std::string y no char
    ofstream file3("data/req/rbooks.dat", ios::binary);
    
    /*/---------- Guarda los nombres ----------/*/

    if (file1.is_open()) {
        for (const string& solicitud : solicitudesUser) {
            file1 << solicitud << endl;
        }
        file1.close();
    } else {
        return interstitial(rojo, "Error al guardar las solicitudes (1)", abortar);
    }

    /*/---------- Guarda las fechas ----------/*/

    if (!file2.is_open()) {
        return interstitial(rojo, "Error al guardar las solicitudes (2)", abortar);
    }

    for (const auto& d : solicitudesDate) {                   //itera sobre el vector y escribe los datos en el archivo
        file2.write(reinterpret_cast<const char*>(&d), sizeof(date));
    }

    /*/---------- Guarda los libros ----------/*/

    if (!file3.is_open()) {
        return interstitial(rojo, "Error al guardar las solicitudes (3)", abortar);
    }

    for (const auto& b :solicitudesLib) {                   //itera sobre el vector y escribe los datos en el archivo
        file3.write(reinterpret_cast<const char*>(&b), sizeof(int));
    }
}

/// @brief Lee todas las solicitudes y las escribe en el vector correspondiente
void loadReqs(){
    ifstream file1("data/req/rusers.dat");              //crea los tres archivos a manejar, los nombres se manejan en archivo de texto ya que
    ifstream file2("data/req/rdates.dat", ios::binary); //se almacenan std::string y no char
    ifstream file3("data/req/rbooks.dat", ios::binary);

    /*/---------- Carga los nombres ----------/*/

    if (file1.is_open()) {
        string linea;
        solicitudesUser.clear();
        while (getline(file1, linea)) {
            solicitudesUser.push_back(linea);
        }
        file1.close();
    } else {
        return interstitial(rojo, "Error al cargar las solicitudes (1)", abortar);
    }

    /*/---------- Carga las fechas ----------/*/

    if (!file2.is_open()) {
        return interstitial(rojo, "Error al cargar las solicitudes (2)", abortar);
    }

    solicitudesDate.clear();  //limpia el vector actual para ingresar los nuevos datos

    date tempDate;  //usuario temporal donde se almacenara la informacion

    while (file2.read(reinterpret_cast<char*>(&tempDate), sizeof(date))) {
        solicitudesDate.push_back(tempDate);
    }

    file2.close();

    /*/---------- Carga los libros ----------/*/

    if (!file3.is_open()) {
        return interstitial(rojo, "Error al cargar las solicitudes (3)", abortar);
    }

    solicitudesLib.clear();  //limpia el vector actual para ingresar los nuevos datos

    int tempLib;  //usuario temporal donde se almacenara la informacion

    while (file3.read(reinterpret_cast<char*>(&tempLib), sizeof(int))) {
        solicitudesLib.push_back(tempLib);
    }

    file3.close();
}