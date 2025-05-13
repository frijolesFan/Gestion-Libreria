/* MANEJO DE LIBROS

* Incluye todas las funciones para el uso de los libros de los usuarios, estas son
    - Consultar libros
        I. Todos los libros
        II. Libros por nombre
        III. Libros por autor
    -Mostrar información de un libro: Muestra la información y la descripción organizada
    por parrafos
    -Solicitar libros: Si existe la posibilidad, solicita un libro seleccionado
    -Devolver libros solicitados: Devuelve un libro, o en su defecto, cancela la solicitud
    de un libro
    -Ordenar libros: Ordena los libros en orden alfabetico segun su nombre o autor

* Realizado por: Juan Camilo Guevara
* Fecha: 17/11/2023
* Compilado Versión: g++ (Rev7, Built by MSYS2 project) 13.1.0 [C/C++ visual studio code]
* booksManagement.cpp
*/
#pragma once

#include "../libs/database.cpp"
#include "../libs/formats.cpp"
#include "../data/data.cpp"

/// @brief Menu de busqueda de libros
void searchBooks();

/// @brief Muestra la informacion de un libro 
/// @param libob Contenedor del libro que se va a mostrar
/// @param info Si es modo de vista con el libro ya en posesion (true) o no (false)
/// @param rechPos Si es para confirmar que el libro ha sido rechazado (-1 si no)
void bookShowInfo(struct book libob, bool info, int rechPos);

/// @brief Muestra los libros del usuario actual
void myBooks();

/// @brief Menú de busqueda de libros
void lookforBooks();

/// @brief Muestra las opciones por defecto de bsqueda de libros
void bookOptions(){
    system("cls");
    cout << "\n\t            LIBROS             " << endl;
    cout << "\n\tA. Buscar";
    cout << "\n\tB. Ordenar por..." << endl;
}

/// @brief Permite regresar un libro
/// @param libob Contenedor del libro a regresar
void returnBook(struct book libob){
    char op;
    do {
        system("cls");
        if (libob.disp == 1){
            cout << "\n\tDeseas cancelar tu solicitud para:\n";
        } else {
            cout << "\n\tDeseas devolver:\n";
        }
        color(cons, amarillo); cout << "\t\t--" << libob.name; color(cons, blanco); cout << "?";
        cout << "\n\t(y/n)\n\n\t//"; cin >> op;
        fflush(stdin);
        op = tolower(op);
    } while (op != 'y' && op != 'n');
    switch (op){
        case 'y':   //si quiere devolver un libro o cancelar una soliicitud
            for (int i = 0; i < 3; i++){
                if (users[currentuser].books[i] == libob.bookID){   //reinicia toda la informacion del libro
                    users[currentuser].books[i] = -1;
                    users[currentuser].ban[i] = false;
                    users[currentuser].entrega[i].day = -1;
                    users[currentuser].entrega[i].month = -1;
                    users[currentuser].entrega[i].year = -1;
                    if (libob.disp == 1){
                        for (int j = 0; j < solicitudesLib.size(); j++){    //elimina la solicitud si es una solicitud
                            if (solicitudesLib[j] == libob.bookID){
                                solicitudesLib.erase(solicitudesLib.begin() + j);
                                solicitudesUser.erase(solicitudesUser.begin() + j);
                                solicitudesDate.erase(solicitudesDate.begin() + j);
                            }
                        }
                    }
                }
            }
            for (int i = 0; i < books.size(); i++){
                if (books[i].bookID == libob.bookID){
                    books[i].disp = 1;  //reinicia la disponibilidad de un libro
                }
            }
            saveUsers();
            saveBooks();
            if (libob.disp == 1) saveReqs();
            interstitial(verde, ((libob.disp == 1) ? "La solicitud se ha cancelado" : "El libro se ha devuelto"), myBooks);
        break;
        case 'n':
            return bookShowInfo(libob, true, 0);
        break;
    }
}

/// @brief Funcion para solicitar libros
/// @param libob Contenedor del libro que se va a solicitar
void bookBorrow(struct book libob){
    if (verifyBan()){   //si el usuario esta baneado
        return interstitial(rojo, "No puedes solicitar libros en este momento", searchBooks);
    } else if (currentuser == -69){ //si esta como invitado
        return interstitial(rojo, "Inicia sesion para pedir libros", searchBooks);
    } else if (libob.disp <= 0){    //si el libro no esta disponible
        return interstitial(rojo, "El libro se encuentra disponible en este momento", searchBooks);
    } else {
        char op = 'a';
        do {
            system("cls");
            cout << "\n\tDesea pedir el libro: ";
            cout << "\n\t\t--"; color(cons, amarillo); cout << libob.name; color(cons, blanco);
            cout << "\n\t(y/n)\n\n\t//"; cin >> op;
            op = tolower(op);
            fflush(stdin);
        } while (op != 'y' && op != 'n');
        bool disponible = false;    //para verificar si tiene espacios disponibles
        switch(op){
            case 'y':   //solicitar el libro
                for (int i = 0; i < 3; i++){
                    if (users[currentuser].books[i] == libob.bookID){
                        return interstitial(rojo, "Ya has solicitado este libro", searchBooks);    //si ya ha solicitado el libro
                    }
                    if (users[currentuser].books[i] == -1){             //si tiene huecos disponibles para pedir libross
                        users[currentuser].books[i] = libob.bookID;     //lo asigna en la solicitud disponible mas cercana
                        disponible = true;
                        break;
                    }
                }
                if (!disponible){
                    return interstitial(rojo, "No puedes solicitar mas libros", searchBooks);  //si no tiene solicitudes disponibles manda error
                }
                solicitudesLib.push_back(libob.bookID);                         //almacena la informacion de solicitud en los vectores de solicitudes
                solicitudesUser.push_back(string(users[currentuser].username));
                solicitudesDate.push_back(currentDate);
                saveReqs();
                saveUsers();
                saveBooks();
                return interstitial(amarillo, "Su solicitud esta en proceso de aprobacion", searchBooks);  //informa que la solicitud se hizo con exito
            break;
            case 'n': 
                return bookShowInfo(libob, false, -1);
            break;
        }
    }   
}

/// @brief Muestra la informacion de un libro
/// @param libob Libro a mostrar
/// @param info Si el libro es solicitado o no
/// @param rechPos Si es para informar que la solicitud se rechazo o no
void bookShowInfo(struct book libob, bool info, int rechPos){
    loadBooks();            //reestablece el orden de los libros para evitar errores
    int maxCentrado = (strlen(libob.name) > strlen(libob.auth)) ? 1 : 2;    //define qué va a centrar
    char op;
    if (rechPos >= 0){
        if (users[currentuser].entrega[rechPos].month == -2){
            users[currentuser].books[rechPos] = -1;
            users[currentuser].ban[rechPos] = false;
            users[currentuser].entrega[rechPos].day = -1;
            users[currentuser].entrega[rechPos].month = -1;
            users[currentuser].entrega[rechPos].year = -1;
            saveReqs();
            saveBooks();
            saveUsers();
            return interstitial(rojo, "Se ha rechazado tu solicitud", pmaindefMenu);
        }
    }
    do {
        system("cls");
        cout << "\n\t";
        if (strlen(libob.name) + 4 <= 36)
            for (int i = 0; i < (36 - (strlen(libob.name) + 4)) / 2; i++)   //32 + los 4 '-' = 36
                cout << " ";    //centra el titulo si es mas corto que el nombre del autor
        cout << "--"; color(cons, amarillo); cout << libob.name; color(cons, blanco); cout << "--" << endl << "\t";

        if (strlen(libob.auth) + 4 <= 36)
            for (int i = 0; i < (((strlen(libob.name) > 36) ? (strlen(libob.name) + 4) : 38) - (strlen(libob.auth) + 4)) / 2; i++)   //lo centra teniendo en cuenta los caracteres de adorno
                cout << " ";    //crentra el autor si es mas corto que el titulo
        cout << "~ "; color(cons, azul); cout << libob.auth << endl << endl;

        color(cons, blanco);    //muestra la descripcion del libro en renglones

        formatParraf(libob.desc, ((strlen(libob.name)) > 36) ? (strlen(libob.name) + 4) : 34);
        cout << "\n\n\tPublicado: "; formatDate(libob.release);
        cout << "\n\n\t";
        if (strlen(libob.name) > 32 || strlen(libob.auth) > 32){        //dibuja el separador masomenos centrado
            cout << "<<";
            for (int i = 0; i < ((maxCentrado == 1) ? strlen(libob.name): strlen(libob.auth)); i++) cout << "-";
            cout << ">>";
        } else cout << "<<------------------------------->>" << endl;   //segun si debe hacerlo mas largo o chico
        if (!info){
            cout << " \n\t A. Solicitar libro / B. Regresar";
        } else if (libob.disp == 1){
            cout << " \n\tA. Cancelar solicitud / B. Regresar";
        } else {
            cout << " \n\t  A. Devolver libro / B. Regresar";
        }
        cout << "\n\n\t//"; cin >> op;
        fflush(stdin);
        op = tolower(op);
    } while (op != 'a' && op != 'b');
    switch(op){
        case 'a':
            if (!info){
                if (currentuser == -69){
                    return interstitial(rojo, "Inicia sesion para poder solicitar libros", searchBooks);
                }
                return bookBorrow(libob);
            } else {
                return returnBook(libob);
            }
        break;
        case 'b':
            if (!info){
                return searchBooks();
            } else {
                return myBooks();
            }
        break;
    }
}

/// @brief Permite buscar libros por autor
void lookforAuth(){
    string name;
    system("cls");
    color(cons, amarillo);
    cout << "\t  (introduzca 0 para regresar)" << endl;   //muestra instruccion para regresar
    color(cons, blanco);
    cout << "\n\t         BUSCAR AUTOR" << endl;
    cout << "\n\tAutor a buscar: //"; getline(cin, name);
    fflush(stdin); 

    if (name[0] == '0' && name.length() == 1) return lookforBooks();

    for (int i = 0; i < name.length(); i++) {   //transforma el texto a minúsculas
        name[i] = tolower(name[i]);
    }
    
    bool exists;
    int conter = 0;             //cuenta las posiciones de los libros
    vector<int> container;      //almacena el ID de los libros
    int op;

    do {
        system("cls");

        color(cons, amarillo);
        cout << "\n\t   Libros de " << name << ": " << endl;
        color(cons, blanco);

        for (int i = 0; i < books.size(); i++){
            char temporal[50];
            strncpy(temporal, books[i].auth, sizeof(temporal));
            for (int j = 0; j < sizeof(temporal); j++){
                temporal[j] = tolower(temporal[j]);
            }

            if (strncmp(temporal, name.c_str(), sizeof(temporal)) == 0){    //si encuentra una coincidencia la muestra como lista
                exists = true;
                conter++;
                cout << "\n\t" << conter << ". " << books[i].name;
                container.push_back(books[i].bookID);
            }
        }

        if (!exists){       //si no se encontro ningun libro de ese autor
            return interstitial(rojo, "No existen libros de este autor", lookforAuth);  //muestra mensaje de error
        }
        cout << "\n\n\t<<--------------------------->>" << endl;
        cout << "\n\t   Ingresa 0 para regresar";
        cout << "\n\n\t//"; cin >> op;
        fflush(stdin);
    } while (op < 0 || op > container.size());

    if (op == 0) return lookforBooks(); //si quire regresar
    else {
        for (int i = 0; i < books.size(); i++){
            if (books[i].bookID == container[op - 1]){
                return bookShowInfo(books[i], false, -1);   //si escoje un libro valido
                break;
            }
        }
    }
}

/// @brief Permite buscar libros por nombre
void lookforName(){
    string name;
    system("cls");
    color(cons, amarillo);
    cout << "\t  (introduzca 0 para regresar)" << endl;   //muestra instruccion para regresar
    color(cons, blanco);
    cout << "\n\t         BUSCAR LIBRO" << endl;
    cout << "\n\tNombre a buscar: //"; getline(cin, name);
    fflush(stdin);

    if (name[0] == '0' && name.length() == 1) return lookforBooks();

    for (int i = 0; i < name.length(); i++) {   //transforma el texto a minúsculas
        name[i] = tolower(name[i]);
    }

    int id = -1;        //variable contenedora del id del libro

    for (int i = 0; i < books.size(); i++){
        char temporal[50];
        strncpy(temporal, books[i].name, sizeof(temporal));
        for (int j = 0; j < sizeof(temporal); j++){
            temporal[j] = tolower(temporal[j]);
        }

        if (strncmp(temporal, name.c_str(), sizeof(temporal)) == 0){
            id = i;
            break;
        }
    }

    if (id == -1) return interstitial(rojo, "No se ha encontrado el libro", lookforName);
    else return bookShowInfo(books[id], false, -1);
}

/// @brief Menu para busqueda de libros
void lookforBooks(){
    int op;
    do {
        system("cls");
        cout << "\n\t         BUSCAR LIBRO" << endl;
        cout << "\n\t1. Buscar por nombre";
        cout << "\n\t2. Buscar por autor";
        cout << "\n\t3. Regresar";
        
        cout << "\n\n\t//"; cin >> op;
        fflush(stdin);
    } while (op < 1 || op > 3);

    switch (op){
        case 1: //buscar por nombre
            return lookforName();
        break;
        case 2: //buscar por autor
            return lookforAuth();
        break;
        case 3: //regresar al menú de busqueda
            return searchBooks();
        break;
    }
}

/// @brief Ordena los libros según un parametro
/// @param parameter 1 = Nombre / 2 = Autor
void sortbooksBy(int parameter){
    for (int i = 0; i < books.size() - 1; ++i) {
        for (int j = 0; j <  books.size() - i - 1; ++j) {
            // Compara los nombres y realiza el intercambio si es necesario
            if (((parameter == 1) ? strcmp(books[j].name, books[j + 1].name) : strcmp(books[j].auth, books[j + 1].auth)) > 0) {
                book temp = books[j];
                books[j] = books[j + 1];
                books[j + 1] = temp;
            }
        }
    }
}

/// @brief Menu con opciones de ordenamiento de libros
void sortBooks(){
    int op;
    do {
        system ("cls");
        cout << "\n\t        ORDENAR LIBROS         " << endl;
        cout << "\n\t1. Alfabeticamente por nombre (A-Z)";
        cout << "\n\t2. Alfabeticamente por autor (A-Z)";
        cout << "\n\t3. Regresar";
        cout << "\n\n\t//"; cin >> op;
        fflush(stdin);
    } while (op < 1 || op > 3);

    switch (op){
        case 3:
            return searchBooks();
        break;
        default:
            sortbooksBy(op);
            return searchBooks();
        break;
    }
}

int pagina = 1;
/// @brief Muestra todos los libros, separados por paginas
void searchBooks(){
    string op;                                           //define el numero de paginas dividiendo el numero de libros
    int pages = ceil(static_cast<float>(books.size()) / 10);  //entre los libos mostrados redondeado al mayor entero
    int libib;                  //variable para almacenar el libro seleccionado en un entero para manjar facilmente
    do {
        bookOptions();
        color(cons, amarillo);
        cout << endl << "\t       Pagina " << pagina << " de " << pages;
        color(cons, blanco);
        cout << endl << "\t<<--------------------------->>" << endl;
        for (int i = 0 + (10 * (pagina - 1)); i < 10 + (10 * (pagina - 1)); i++){  //muestra los libros de 10 en 10
            if (i == books.size()){                                                               //o hasta que no haya mas
                break;
            }
            cout << endl << "\t" << (i + 1) << ". " << books[i].name;   //muestra el libro correspondiente a i
        }
        cout << endl << "\n\t<<--------------------------->>" << endl;
        cout << "\n\tC. Anterior / D. Siguiente / E. Volver";
        cout << "\n\n\t//"; getline(cin, op);
        fflush(stdin);
        op[0] = tolower(op[0]);
    } while (!(op[0] == '1' || op[0] == '2'|| op[0] == '3' || op[0] == '4' || op[0] == '5' || op[0] == '6' || op[0] == '7' || op[0] == '8' || op[0] == '9' ||
    op == "a" || op == "b" || op == "c" || op == "d" || op == "e") || op.length() > 2); //verifica si es una seleccion valida

    switch(op[0]){  //opciones en base al pimer caracter de op
        case 'a':   //buscar
            lookforBooks();
        break;
        case 'b':   //ordenar
            sortBooks();
        break;
        case 'c':   //paiga anterior
            if (pagina == 1){
                return searchBooks();               //si es la primera pagina no deja devovler
            }   
            else {
                pagina--;
                return searchBooks();               //sino si
            }
        break;
        case 'd':   //pagina siguiente
            if (pagina == pages) return searchBooks();   //si es la ultima pagina no deja seguir
            else {
                pagina++;
                return searchBooks();               //sino si
            }
        break;
        case 'e':   //volver al menu
            loadBooks();            //reestablece el orden de los libros
            return pmaindefMenu();
        break;
        default:
            libib = stoi(op);   //pasa el numero ingresado a una variable en entero
            if ((libib > 10 * pagina) || (libib < (10 * (pagina - 1)) + 1) || libib > books.size()){
                return searchBooks();
            }
            return bookShowInfo(books[libib - 1], false, -1);
        break;
    }
}

/// @brief Muestra los libros en posesion del usuario actual
void myBooks(){
    if (users[currentuser].books[0] == -1 && users[currentuser].books[1] == -1 && users[currentuser].books[2] == -1){   //si no tiene libros prestados
        return interstitial(amarillo, "No posees libros en este momento", pmaindefMenu);
    } else {        //si si
        char op;
        struct book liborden[3] = {{-1}, {-1}, {-1}};
        do{
            system("cls");
            color(cons, amarillo);
            cout << "\n\t\tMIS LIBROS" << endl;
            color(cons, blanco);
            int conter = 0;
            for (int i = 0; i < 3; i++){
                if (!(users[currentuser].books[i] == -1)){ //rompe el bucle cuado no encuentre mas libros
                    conter++;
                    cout << "\n\t" << conter << ". ";
                    int curBook;
                    for (int j = 0; j < sizeof(books); j++){
                        if (books[j].bookID == users[currentuser].books[i]){
                            curBook = j;
                            cout << books[curBook].name;
                            liborden[conter - 1] = books[curBook];
                            break;
                        }
                    }       
                    if (books[curBook].disp == 1 && users[currentuser].entrega[i].month != -2){
                        cout << " / "; color(cons, amarillo); cout << "En tramite";
                    } else if (users[currentuser].entrega[i].month == -2){
                        cout << " / "; color(cons, rojo); cout << "Solicitud rechazada (ver mas info.)";
                    } else {
                        cout << " / ";
                        //si esta pasado de fecha
                        if (users[currentuser].ban[i]){
                            color(cons, rojo);
                        }
                        else {
                            color (cons, verde);
                        }
                        cout << "Entrega: " << users[currentuser].entrega[i].day <<"/"<< users[currentuser].entrega[i].month;
                    }
                    color(cons, blanco);
                } 
                
            }
            cout << "\n\n\tA. Regresar\n\n\t//"; cin >> op;
        } while (op != '1' && op != '2' && op != '3' && op != 'a');
        switch (op){
            case '1':
                if (liborden[0].bookID != -1){
                    return bookShowInfo(liborden[0], true, 0);
                }
            break;
            case '2':
                if (liborden[1].bookID != -1){
                    return bookShowInfo(liborden[1], true, 1);
                }
            break;
            case '3':
                if (liborden[2].bookID != -1){
                    return bookShowInfo(liborden[2], true, 2);
                }
            break;
            case 'a':
                return pmaindefMenu();
            break;
        }
    } 
}