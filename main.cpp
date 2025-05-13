/* CODIGO PRINCIPAL

* Se encarga de iniciar el programa, primero, carga los archivos de guardado, o en su
defecto, los crea, luego, apunta dos punteros globales a sus dos funciones para ser usadas
a lo largo del programa sin necesidad de incluír este archivo en los demás.

Contiene los menús principales: Menu de inicio de sesion y menú de usuario
Este archivo se encarga de unir los diferentes codigos en uno solo y hacer que el programa funcione.
Al igual que en el resto de archivos, no se incluyen librerias ya que todas se encuentran
incluidas en el archivo "structs.cpp"

* Realizado por: Juan Camilo Guevara
* Fecha: 18/11/2023
* Compilado Versión: g++ (Rev7, Built by MSYS2 project) 13.1.0 [C/C++ visual studio code]
* main.cpp
*/
#include "admin/admin.cpp"
#include "data/data.cpp"
#include "libs/database.cpp"
#include "libs/structs.cpp"
#include "libs/formats.cpp"
#include "management/sessionManagement.cpp"
#include "management/userSettings.cpp"
#include "management/booksManagement.cpp"
#include "management/reqManagement.cpp"

void initmenu();    //menu de inicio de sesion y registro 
void maindefMenu(); //menus de usuario

/// @brief Menu por defecto para usuarios y trabajadores
void maindefMenu(){
    int op, maxop = 5;
    if (currentuser != -69 && users[currentuser].usertype == 2) maxop = 6;    //modifica el numero de opciones a validar en funcion del tipo de usuario
    else if (currentuser == -69) maxop = 2;             //y si el usuario es un invitado
    do{
        system("cls");
        cout << "\n\t          PAPULIBRERIA         " << endl;
        cout << "\n\t1. Buscar libros               ";
        if (currentuser != -69){
            cout << "\n\t2. Mis libros                  ";
            if (users[currentuser].usertype == 1){          //menu de usuario
                cout << "\n\t3. Perfil                      ";
                cout << "\n\t4. Cerrar sesion               ";
            }else if (users[currentuser].usertype == 2){    //menu de trabajador
                cout << "\n\t3. Solicitudes                 ";
                cout << "\n\t4. Perfil                      ";
                cout << "\n\t5. Cerrar sesion               ";
            }
        } else {
            cout << "\n\t2. Regresar a inicio de sesion";
        }
        cout << "\n\n\t//"; cin >> op;
        fflush(stdin);
    }while (op < 1 || op > maxop);

    switch(op){
        case 1:
            searchBooks();
        break;
        case 2:
            if (currentuser != -69){
                return myBooks();
            } else {
                return initmenu();  //cierre de sesion de invitado
            }
        break;
        case 3:
            if (maxop == 5)
                return profileMenu();   //menu de perfil para el usuario
            else
                return showReq();
        break;
        case 4:
            if (maxop == 5)
                return logout(initmenu);        //cierre de sesion de usuario
            else
                return profileMenu();   //menu de perfil para el trabajador
        break;
        case 5:
            return logout(initmenu);    //cierre de sesion de trabajador
        break;
    }
}

/// @brief Menu inicial con opciones de inicio de sesio
void initmenu(){
    int op;
    pagina = 1;     //reinicia la pagina de busqueda de libros
    do{
        system("cls");
        cout << "\n\t          PAPULIBRERIA          " << endl;
        cout << "\n\t1. Iniciar sesion               ";
        cout << "\n\t2. Registrarse                  ";
        cout << "\n\t3. Ingresar como invitado       ";
        color(cons, 4);
        cout << "\n\t4. Salir                        ";
        color(cons, blanco);
        cout << "\n\n\t//"; cin >> op;
        fflush(stdin);
    } while (op < 1 || op > 4);
    switch (op){
        case 1:     //inicio de sesion
            return login(0, initmenu);
        break;
        case 2:     //registro de usuario
            registerUser(0, 1, initmenu);
        break;
        case 3:     //iniciar sesion de invitado
            currentuser = -69;
            pmaindefMenu();
        break;
        case 4:     //cierre de programa
            system("cls");
            exit(0);
        break;
    }
}

/// @brief Funcion que inicia el programa
int main(){
    verifyFile("data/users.dat", saveUsers, loadUsers); //verifica si existe el fichero para, en caso de no existir, ponerlo
    verifyFile("data/books.dat", saveBooks, loadBooks); //con los valores predeterminados y, en caso de existir, cargarlos
    verifyFile("data/req/rusers.dat", saveReqs, loadReqs);
    updateDate();   //actualiza la fecha a la fecha en que se abrio el programa
    pmaindefMenu = maindefMenu; //actualiza el puntero que retorna al menu principal
    pinitmenu = initmenu;
    initmenu();
}
