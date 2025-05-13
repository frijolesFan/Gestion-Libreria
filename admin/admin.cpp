/* FUNCIONES DEL ADMINISTRADOR

* Incluye todas las funciones del administrador, estas se toman desde el menu
adminMenu() y son:
    - Modificar permisos de usuario: Pide el nombre de un usuario para modifiar su
    estado de usuario a trabajador o viceversa
    -Modificar libros:
        -Información general de un libro existente:
            -Nombre
            -Autor
            -Descripcion
        -Dar libros de baja: Es decir, se muestra a los usuarios pero no se puede
        volver a solicitar
        -Eliminar libros: Elimina un libro existente disponible
        -Agregar libros: Permite agregar un libro

* Realizado por: Juan Camilo Guevara
* Fecha: 18/11/2023
* Compilado Versión: g++ (Rev7, Built by MSYS2 project) 13.1.0 [C/C++ visual studio code]
* admin.cpp
*/
#pragma once

#include "../libs/database.cpp"
#include "../libs/structs.cpp"
#include "../libs/formats.cpp"
#include "../data/data.cpp"

void adminMenu();
void booksMenu();
void modifyUsers(int error);
void gotoBook(int error, void (*funcion)(struct book &libro));
void modBookMenu(struct book &libro);

/// @brief Permite modificar los permisos de un usuario
/// @param usuario Se pasa el usuario a modificar
void showUser(struct user &usuario){
    char op;
    do {
        system("cls");
        color(cons, amarillo);
        cout << "\n\t~ " << usuario.username << endl; 
        color(cons, blanco);

        cout << "\n\tDesea convertir este usuario en ";
        (usuario.usertype == 1) ? (color(cons, rosa)) : (color(cons, azul));    //muestra el color correspondiente al tipo de usuario
        cout << ((usuario.usertype == 1) ? "trabajador" : "usuario");

        color(cons, blanco); cout << " ?";
        cout << "\n\t(y/n)";
        cout << "\n\n\t//"; cin >> op;
        fflush(stdin);
    } while (op != 'y' && op != 'n');

    switch(op){
        case 'y':
            usuario.usertype = ((usuario.usertype == 1) ? 2 : 1);
            saveUsers();
            return interstitial(amarillo, "Los permisos se han modificado", adminMenu);
        break;
        break;
        case 'n':
            return modifyUsers(0);
        break;
    }
}

/// @brief Menu para modificar permisos de usuario
/// @param error Codigo de error (-1: Usuario inexistente)
void modifyUsers(int error){
    string name;

    system("cls");
    color(cons, amarillo);
    cout << "\t  (introduzca 0 para regresar)" << endl;   //muestra instruccion para regresar
    color(cons, blanco);
    cout << "\n\t MODIFICAR PERMISOS DE USUARIO " << endl;
    switch(error){
        case -1:
            color(cons, rojo);
            cout << "\t   --El usuario no existe--    \n";
            color(cons, blanco);
        break;
    }
    cout << "\n\tNombre de usuario: //"; getline(cin, name);
    fflush(stdin);

    if(name[0] == '0' && name.length() == 1) return adminMenu();    //codigo de regreso

    int id = -1;

    for (int i = 0; i < users.size(); i++){
        if (strcmp(users[i].username, name.c_str()) == 0){
            id = i;
            break;
        }
    }

    if (id == -1){  //si no encuentra el usuario
        return modifyUsers(-1);
    } else {
        return showUser(users[id]);
    }
}

/// @brief Permite dar un libro de baja
/// @param libro Libro a dar de baja
void downBooks(struct book &libro){
    if (libro.disp == 0) return interstitial(rojo, "El libro no se puede dar de baja ahora mismo", booksMenu);  //no deja si el libro esta prestado

    for (int i = 0; i < solicitudesLib.size(); i++){   //o si el libro esta siendo solicitado
        if (solicitudesLib[i] == libro.bookID) {
            return interstitial(rojo, "El libro no se puede dar de baja ahora mismo", booksMenu);
        }
    }

    char op;
    do {
        system("cls");
        color(cons, amarillo);
        cout << "\n\t~ " << libro.name << endl; 
        color(cons, blanco);

        cout << "\n\tDesea dar este libro de baja?" << endl;
        cout << "\t  (no se puede deshacer)" << endl;
        color(cons, amarillo);

        color(cons, blanco);
        cout << "\n\t(y/n)";
        cout << "\n\n\t//"; cin >> op;
        fflush(stdin);
    } while (op != 'y' && op != 'n');

    switch(op){
        case 'y':
            libro.disp = -1;    //modifica la disponibilidad para indicar que se dio de baja
            saveBooks();
            return interstitial(amarillo, "El libro se ha dado de baja", booksMenu);
        break;
        case 'n':
            return gotoBook(0, downBooks);
        break;
    }
}

/// @brief Permite eliminar un libro especifico
/// @param libro Libro a eliminar
void deleteBook(struct book &libro){
    if (libro.disp == 0) return interstitial(rojo, "El libro no se puede eliminar ahora mismo", booksMenu);  //no deja si el libro esta prestado

    for (int i = 0; i < solicitudesLib.size(); i++){   //o si el libro esta siendo solicitado
        if (solicitudesLib[i] == libro.bookID) {
            return interstitial(rojo, "El libro no se puede eliminar ahora mismo", booksMenu);
        }
    }

    char op;
    int id;
    do {
        system("cls");
        color(cons, amarillo);
        cout << "\n\t~ " << libro.name << endl; 
        color(cons, blanco);

        cout << "\n\tDesea ELIMINAR este libro?" << endl;
        cout << "\t  (no se puede deshacer)" << endl;
        color(cons, amarillo);

        color(cons, blanco);
        cout << "\n\t(y/n)";
        cout << "\n\n\t//"; cin >> op;
        fflush(stdin);
    } while (op != 'y' && op != 'n');

    switch(op){
        case 'y':
            for (int i = 0; i < books.size(); i++){     //busca la posición del libro
                if (books[i].bookID == libro.bookID){
                    id = i;
                    break;
                }
            }
            books.erase(books.begin() + id);    //elimina el libro en cuestión
            saveBooks();
            return interstitial(amarillo, "El libro se ha eliminado", booksMenu);
        break;
        case 'n':
            return gotoBook(0, downBooks);
        break;
    }
}

/// @brief Permite modificar la informacion de un libro
/// @param libro Libro a modificar
/// @param parameter Parametro a modificar (1 nombre, 2 autor, 3 descripcion, 4 f. publicacion)
/// @param length Largo del texto a ingresar
void modBook(struct book &libro, int parameter, int length = 50){
    char op;
    if (parameter == 4){    //si va a modificar la fecha
        struct date temporal;
        do {
            color(cons, amarillo);
            cout << "\t  (introduzca 0 para regresar)" << endl;   //muestra instruccion para regresar
            color(cons, blanco);
            cout << "\n\t        MODIFICAR FECHA        " << endl;
            
            color(cons, amarillo); cout << "\n\tAno"; color(cons, blanco); cout << ": //"; cin >> temporal.year;
            fflush(stdin);
            if (temporal.year == 0) return modBookMenu(libro);

            color(cons, amarillo); cout << "\n\tMes"; color(cons, blanco); cout << ": //"; cin >> temporal.month;
            fflush(stdin);
            if (temporal.month == 0) return modBookMenu(libro);

            color(cons, amarillo); cout << "\n\tDia"; color(cons, blanco); cout << ": //"; cin >> temporal.day;
            fflush(stdin);
            if (temporal.day == 0) return modBookMenu(libro);

            while (temporal.month < 1 || temporal.month > 12 || temporal.day < 1 || temporal.day > 31){
                cout << "\n\t        MODIFICAR FECHA        " << endl;

                color(cons, rojo);
                cout << "\t      --Fecha invalida--       " << endl;
                color(cons, blanco);

                color(cons, amarillo); cout << "\n\tAno"; color(cons, blanco); cout << ": //"; cin >> temporal.year;
                fflush(stdin);
                if (temporal.year == 0) return modBookMenu(libro);

                color(cons, amarillo); cout << "\n\tMes"; color(cons, blanco); cout << ": //"; cin >> temporal.month;
                fflush(stdin);
                if (temporal.month == 0) return modBookMenu(libro);

                color(cons, amarillo); cout << "\n\tDia"; color(cons, blanco); cout << ": //"; cin >> temporal.day;
                fflush(stdin);
                if (temporal.day == 0) return modBookMenu(libro);
            }

            cout << "\n\tDesea continuar? (y/n)";
            cout << "\n\t//"; cin >> op;
            fflush(stdin);

        } while (op != 'y' && op != 'n');

        if (op == 'n') return modBook(libro, parameter, length);    //si ingresa no, se reinicia el formulario

        libro.release.day = temporal.day;
        libro.release.month = temporal.month;
        libro.release.year = temporal.year;
    } else {
        string texto;
        do {
            system("cls");
           
            color(cons, amarillo);
            cout << "\t  (introduzca 0 para regresar)" << endl;   //muestra instruccion para regresar
            color(cons, blanco);

            cout << "\n\t" << ((parameter == 1) ? "       " : ((parameter == 2) ? "        " : "     "));    //centra el texto
            cout << "\n\t MODIFICAR " <<  ((parameter == 1) ? "NOMBRE" : ((parameter == 2) ? "AUTOR" : "DESCRIPCION")) << endl;
            cout << "\n\t//"; getline(cin, texto);
            fflush(stdin);

            if (texto[0] == '0' && texto.length() == 1) return gotoBook(0, modBookMenu);

            while (texto.length() > length){    //si el texto es mas largo de lo permitido
                system("cls");
                
                color(cons, amarillo);
                cout << "\t  (introduzca 0 para regresar)" << endl;   //muestra instruccion para regresar
                color(cons, blanco);

                cout << "\n\t" << ((parameter == 1) ? "       " : ((parameter == 2) ? "        " : "     "));    //centra el texto
                cout << "\n\t MODIFICAR " <<  ((parameter == 1) ? "NOMBRE" : ((parameter == 2) ? "AUTOR" : "DESCRIPCION")) << endl;
                color(cons, rojo); cout << "     --TAMANO MAXIMO: " << length << "--     "; color(cons, blanco);
                cout << "\n\t//"; getline(cin, texto);
                fflush(stdin);

                if (texto[0] == '0' && texto.length() == 1) return gotoBook(0, modBookMenu);
            }

            cout << "\n\tDesea continuar? (y/n)";
            cout << "\n\t//"; cin >> op;
            fflush(stdin);

        } while (op != 'y' && op != 'n');

        if (op == 'n') return modBook(libro, parameter, length);    //si ingresa no, se reinicia el formulario

        switch (parameter){
            case 1: //modifica el nombre
                strcpy(libro.name, texto.c_str());
            break;
            case 2: //modifica el autor
                strcpy(libro.auth, texto.c_str());
            break;
            case 3: //modifica la descripcion
                strcpy(libro.desc, texto.c_str());
            break;
        }
    }
    //al final se guarda la modificacion realizada
    saveBooks();
    interstitial(verde, "Libro modificado correctamente", booksMenu);
}

/// @brief Menu con opciones de modificacion de un libro especifico
/// @param libro Libro a modificar, se obitiene con gotoBook
void modBookMenu(struct book &libro){
    int op;
    do {
        system("cls");
        cout << "\n\t~ "; color(cons, amarillo); cout << libro.name << endl;
        color(cons, blanco);
        cout << "\n\t1. Modificar nombre";
        cout << "\n\t2. Modificar autor";
        cout << "\n\t3. Modificar descripción";
        cout << "\n\t4. Modificar fecha de publicacion";
        cout << "\n\t5. Regresar";
        cout << "\n\n\t//"; cin >> op;
        fflush(stdin);
    } while (op < 1 || op > 5);

    switch (op){
        case 1:
            return modBook(libro, 1, 50);
        break;
        case 2:
            return modBook(libro, 2, 50);
        break;
        case 3:
            return modBook(libro, 3, 150);
        break;
        case 4:
            return modBook(libro, 4);
        break;
        case 5:
            return gotoBook(0, modBookMenu);
        break;
    }
}

/// @brief Permite acceder a un libro para operarlo
/// @param error Si el libro no existe, se ingresa como -1
/// @param funcion Funcion para operar el libro
void gotoBook(int error, void (*funcion)(struct book &libro)){
    string name;

    system("cls");
    color(cons, amarillo);
    cout << "\t  (introduzca 0 para regresar)" << endl;   //muestra instruccion para regresar
    switch(error){
        case -1:
            color(cons, rojo);
            cout << "\t    --El libro no existe--     \n";
        break;
    }
    color(cons, blanco);
    cout << "\n\tNombre del libro: //"; getline(cin, name);
    fflush(stdin);

    if(name[0] == '0' && name.length() == 1) return adminMenu();    //codigo de regreso

    for (int i = 0; i < name.length(); i++) {   //transforma el texto a minúsculas
        name[i] = tolower(name[i]);
    }

    int id = -1;

    for (int i = 0; i < books.size(); i++){
        char temporal[50];
        strncpy(temporal, books[i].name, sizeof(temporal)); //transforma el titulo a minusculas
        for (int j = 0; j < sizeof(temporal); j++){
            temporal[j] = tolower(temporal[j]);
        }

        if (strcmp(temporal, name.c_str()) == 0){   //compara ambos en minusculas
            id = i;
            break;
        }
    }

    if (id == -1){  //si no encuentra el usuario
        return gotoBook(-1, funcion);
    } else {
        return funcion(books[id]);  //manda el libro a la funcion de operacion
    }
}

/// @brief Permite agregar un libro
/// @param error Para mostrar errores de entrada
/// @param fase Fase de registro del libro
/// @param temporal Libro temporal, se añade al vector al final
void addBook(int error, int fase,struct book temporal = {}){
    string texto;                   //variables que se van a modificar mas adelante
    struct date temporaldate;
    char op;

    system("cls");
    if (fase != 5){             //si no esta en la fase de confirmacion
        color(cons, amarillo);
        cout << "\t  (introduzca 0 para regresar)" << endl;   //muestra instruccion para regresar
        color(cons, blanco);
    }
    cout << "\n\t        REGISTRAR LIBRO        " << endl;
    switch(fase){
        case 1: //si esta ingresando el nombre
            switch (error){
                case -1:
                    color(cons, rojo);
                    cout << "\t      --Nombre invalido--      " << endl;
                    color(cons, blanco);
                break;
            }
            cout << "\n\tNombre: //"; getline(cin, texto);
            fflush(stdin);

            if (texto[0] == '0' && texto.length() == 1) return booksMenu(); //si ingresa el codigo para regresar
            if (texto.length() > 50) return addBook(-1, 1, temporal);       //si ingresa un nombre muy largo

            strcpy(temporal.name, texto.c_str());   //cambia el nombre
            return addBook(0, 2, temporal);        //siguiente fase
        break;
        case 2: //si esta ingresando el autor
            switch (error){
                case -1:
                    color(cons, rojo);
                    cout << "\t      --Autor invalido--      " << endl;
                    color(cons, blanco);
                break;
            }
            cout << "\n\tAutor: //"; getline(cin, texto);
            fflush(stdin);

            if (texto[0] == '0' && texto.length() == 1) return addBook(0, 1, temporal); //si ingresa el codigo para regresar
            if (texto.length() > 50) return addBook(-1, 2, temporal);                   //si ingresa un autor muy largo

            strcpy(temporal.auth, texto.c_str());   //cambia el nombre
            return addBook(0, 3, temporal);        //siguiente fase
        break;
        case 3: //si esta ingresando la descripción
            switch (error){
                case -1:
                    color(cons, rojo);
                    cout << "\t    --Descripcion invalida--   " << endl;
                    color(cons, blanco);
                break;
            }
            cout << "\n\tDescripcion: //"; getline(cin, texto);
            fflush(stdin);

            if (texto[0] == '0' && texto.length() == 1) return addBook(0, 2, temporal); //si ingresa el codigo para regresar
            if (texto.length() > 150) return addBook(-1, 3, temporal);                   //si ingresa un autor muy largo

            strcpy(temporal.desc, texto.c_str());   //cambia el nombre
            return addBook(0, 4, temporal);        //siguiente fase
        break;
        case 4: //si está ingresando la fecha de publicación
            switch(error){
                case -1:
                    color(cons, rojo);
                    cout << "\t        --Ano invalido--       " << endl;
                    color(cons, blanco);
                break; 
                case -2:
                    color(cons, rojo);
                    cout << "\t        --Mes invalido--       " << endl;
                    color(cons, blanco);
                break; 
                case -3:
                    color(cons, rojo);
                    cout << "\t        --Dia invalido--       " << endl;
                    color(cons, blanco);
                break; 
            }
            color(cons, amarillo); cout << "\n\tAno"; color(cons, blanco); cout << ": //"; cin >> temporaldate.year;
            fflush(stdin);
            if (temporaldate.year > currentDate.year) return addBook(-1, 4, temporal);  //si el año es mayor al año actual
            if (temporaldate.year == 0) return addBook(0, 3, temporal); //si ingresa el codigo para regresar

            color(cons, amarillo); cout << "\n\tMes"; color(cons, blanco); cout << ": //"; cin >> temporaldate.month;
            fflush(stdin);
            if (temporaldate.month < 1 || temporaldate.month > 12) return addBook(-2, 4, temporal);  //si el mes no existe
            if (temporaldate.month == 0) return addBook(0, 3, temporal); //si ingresa el codigo para regresar

            color(cons, amarillo); cout << "\n\tDia"; color(cons, blanco); cout << ": //"; cin >> temporaldate.day;
            fflush(stdin);
            if (temporaldate.day < 1 || temporaldate.day > 32) return addBook(-3, 4, temporal);  //si el mes no existe
            if (temporaldate.day == 0) return addBook(0, 3, temporal); //si ingresa el codigo para regresar

            temporal.release.day = temporaldate.day;
            temporal.release.month = temporaldate.month;
            temporal.release.year = temporaldate.year;

            return addBook(0, 5, temporal);
        break;
        case 5: //finaliza el registro
            do {
                system("cls");
                cout << "\n\tDesea registrar el libro:" << endl;
                color(cons, amarillo);
                cout << "\t~ " << temporal.name << endl; color(cons, blanco);
                cout << "\n\t(y/n)";
                cout << "\n\n\t//"; cin >> op;
                fflush(stdin);
            } while (op != 'y' && 'n');

            if (op == 'n') return addBook(0, 1);    //reinicia el formulario

            temporal.disp = 1;
            temporal.bookID = books[books.size() - 1].bookID + 1;   //pone el id como el ultimo

            books.push_back(temporal);  //lo añade al vector
            saveBooks();
            return interstitial(verde, "El libro se ha registrado correctamente", booksMenu);
        break;
    }
}

/// @brief Menu con opciones de modificacion de libros
void booksMenu(){
    int op;
    do {
        system("cls");
        cout << "\n\t       MODIFICAR LIBROS        " << endl;
        cout << "\n\t1. Agregar libros";
        cout << "\n\t2. Dar libros de baja";
        cout << "\n\t3. Modificar informacion";
        color(cons, rojo);
        cout << "\n\t4. Eliminar libros";
        color(cons, blanco);
        cout << "\n\t5. Regresar";
        cout << "\n\n\t//"; cin >> op;
        fflush(stdin);
    } while (op < 1 || op > 5);

    switch (op){
        case 1: //añadir libro
            return addBook(0, 1);
        break;
        case 2: //dar libro de baja
            return gotoBook(0, downBooks);
        break;
        case 3: //modificar libro
            return gotoBook(0, modBookMenu); 
        break;
        case 4:
            //eliminar libro
            return gotoBook(0, deleteBook);
        break;
        case 5: //regresar
            return adminMenu();
        break;
    }
}

/// @brief Menu de opciones de administrador
void adminMenu(){
    int op;
    do {
        system("cls");
        cout << "\n\t     MENU DE ADMINISTRADOR     " << endl;
        cout << "\n\t1. Modificar permisos de usuario";
        cout << "\n\t2. Modificar libros";
        cout << "\n\t3. Salir";
        cout << "\n\n\t//"; cin >> op;
        fflush(stdin);
    } while (op < 1 || op > 3);

    switch (op){
        case 1:
            return modifyUsers(0);
        break;
        case 2:
            return booksMenu();
        break;
        case 3:
            return pinitmenu();
        break;
    }
}