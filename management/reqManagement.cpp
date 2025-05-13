/* MANEJO DE SOLICITUDES

* Se encarga del manejo de las solicitudes.
Incluye funciones para mostrar todas las solicitudes enlistadas, separadas por paginas,
mostrar información detallada de cada solicitud y funciones para aceptar, y negar solicitudes

* Realizado por: Juan Camilo Guevara
* Fecha: 17/11/2023
* Compilado Versión: g++ (Rev7, Built by MSYS2 project) 13.1.0 [C/C++ visual studio code]
* reqManagement.cpp
*/
#pragma once

#include "../libs/database.cpp"
#include "../libs/formats.cpp"
#include "../data/data.cpp"

int currentPage = 1;

/// @brief Muestra las solicitudes actuales
void showReq();

/// @brief Muestra una solicitud especifica
/// @param bokID ID del libro en el vector de solicitudes
/// @param usID ID del usuario en el vector de solicitudes
/// @param fechID ID de la fecha de solicitud en el vector de solicitues
void req(int bokID, int usID, int fechID);

/// @brief Cabecera por defecto del menu de solicitudes
void cabecera(){
    cout << "\n\t\t        SOLICITUDES          " << endl;
}

/// @brief Permite aceptar una solicitud
/// @param bokID ID del libro en el vector de solicitudes
/// @param usID ID del usuario en el vector de solicitudes
/// @param fechID ID de la fecha de solicitud en el vector de solicitues
void acptReq(int bokID, int usID, int fechID){
    struct date tempDate;
    char op;

    system("cls");
    cout << "\n\t        FECHA DE ENTREGA          " << endl;
    color(cons, amarillo); cout << "\n\tAno: "; color(cons, blanco); cin >> tempDate.year; fflush(stdin);
    color(cons, amarillo); cout << "\tMes: "; color(cons, blanco); cin >> tempDate.month; fflush(stdin);
    color(cons, amarillo); cout << "\tDia: "; color(cons, blanco); cin >> tempDate.day; fflush(stdin);

    //verifica que la fecha sea valida
    if ((currentDate.year > tempDate.year || currentDate.month > tempDate.month  || (currentDate.month >=tempDate.month && currentDate.day > tempDate.day)) || (tempDate.month > 12 || tempDate.month < 1) || (tempDate.day < 1 || tempDate.day > 31)){
        color(cons, rojo);
        system("cls");
        cout << "\n\t   --La fecha no es valida--" << endl;
        Sleep(2000);
        color(cons, blanco);
        return acptReq(bokID, usID, fechID);
    }

    do {
        system("cls");
        string libName;
        for (int i = 0; i < books.size(); i++){
            if (books[i].bookID == solicitudesLib[bokID - 1]){
                libName = string(books[i].name);
                break;
            }
        }
        cout << "\n\t\t     CONFIRMAR SOLICITUD          " << endl;
        cout << "\n\tLibro: "; color(cons, amarillo); cout << libName; color(cons, blanco);
        cout << "\n\tUsuario: "; color(cons, azul); cout << solicitudesUser[usID]; color(cons, blanco);
        cout << "\n\tFecha de entrega: "; formatDate(tempDate); color(cons, blanco);
        cout << "\n\n\tEsta informacion es correcta?";
        cout << "\n\t(y/n)\n\n\t//"; cin >> op;
        fflush(stdin);
    } while (op != 'y' && op != 'n');
    switch(op){
        case 'y':
            char conten[17];
            int userPos;
            strcpy(conten, solicitudesUser[usID].c_str());
            for (int i = 0; i < users.size(); i++){
                if (strcmp(conten, users[i].username) == 0){
                    for (int j = 0; j < 3; j++){
                        if (users[i].books[j] == solicitudesLib[bokID - 1]){
                            users[i].entrega[j].day = tempDate.day;
                            users[i].entrega[j].month = tempDate.month;
                            users[i].entrega[j].year = tempDate.year;
                            break;
                        }
                    }
                    break;
                }
            }
            for (int i = 0; i < books.size(); i++){
                if (books[i].bookID == solicitudesLib[bokID - 1]){
                    books[i].disp = 0;
                }
            }
            solicitudesLib.erase(solicitudesLib.begin() + (bokID - 1));
            solicitudesUser.erase(solicitudesUser.begin() + (usID));
            solicitudesDate.erase(solicitudesDate.begin() + (fechID));
            saveReqs();
            saveBooks();
            saveUsers();
            return interstitial(verde, "La solicitud se ha aceptado", showReq);
        break;
        case 'n':
            return req(bokID, usID, fechID);
        break;
    }
}

void req(int bokID, int usID, int fechID){
    char op;
    do {
        system("cls");
        string libName;
        for (int i = 0; i < books.size(); i++){
            if (books[i].bookID == solicitudesLib[bokID - 1]){
                libName = string(books[i].name);
                break;
            }
        }
        cout << "\n\t    --"; color(cons, amarillo); cout << libName << endl; color(cons, blanco);
        cout << "\n\tSolicitado por: ";color(cons, azul); cout << solicitudesUser[usID]; color(cons, blanco); cout << " / "; formatDate(solicitudesDate[fechID]);
        cout << "\n\n\t<<---------------------------------->>" << endl;
        cout << "\n\tA. Aceptar / B. Rechazar / C. Regresar";
        cout << "\n\n\t//"; cin >> op;
        fflush(stdin);
        op = tolower(op);
    } while (op != 'a' && op != 'b' && op != 'c');
    switch(op){
        case 'a':
            acptReq(bokID, usID, fechID);
        break;
        case 'b':
            do {
                system("cls");
                color(cons, amarillo);
                cout << "\n\t\t--Desea rechazar esta solicitud?--";
                color(cons, blanco);
                cout << "\n\t(y/n)\n\n\t//"; cin >> op;
                fflush(stdin);
                op = tolower(op);
            } while (op != 'y' && op != 'n');
            switch (op){
                case 'y':
                    for (int i = 0; i < users.size(); i++){
                        bool find = false;
                        for (int j = 0; j < 3; j++){
                            if (users[i].books[j] == solicitudesLib[bokID - 1]){
                                users[i].entrega[j].month = -2; //indica error al usuario (lo almacena en entrega por conveniencia xd)
                                find = true;
                                break;
                            }
                        }
                        if (find) break;
                    }
                    solicitudesLib.erase(solicitudesLib.begin() + (bokID - 1));
                    solicitudesUser.erase(solicitudesUser.begin() + (usID));
                    solicitudesDate.erase(solicitudesDate.begin() + (fechID));
                    saveReqs();
                    saveBooks();
                    saveUsers();
                    interstitial(amarillo, "La solicitud ha sido rechazada", showReq);
                break;
                case 'n':
                    return req(bokID, usID, fechID);
                break;
            }
        break;
        case 'c':
            return showReq();
        break;
    }
}

int maxP = 5;
/// @brief Muestra todas las solicitudes, enlistadas por paginas
void showReq(){
    currentPage = 1;
    string op;
    int pages = ceil(static_cast<float>(solicitudesLib.size()) / maxP);
    if (pages == 0){
        return interstitial(amarillo, "No hay solicitudes en este momento", pmaindefMenu);
    }
    do {
        system("cls");
        cabecera();
        color(cons, amarillo);
        cout << endl << "\t\t       Pagina " << currentPage << " de " << pages;
        color(cons, blanco);
        cout << endl << "\t<<----------------------------------------->>" << endl;
        for (int i = 0 + (maxP * (currentPage - 1)); i < maxP + (maxP * (currentPage - 1)); i++){
            if (i == solicitudesLib.size()){
                break;
            }
            string libName;
            for (int j = 0; j < books.size(); j++){
                if (books[j].bookID == solicitudesLib[i]){
                    libName = string(books[j].name);
                    break;
                }
            }
            cout << endl << "\t" << (i + 1) << ". " << libName;
            color(cons, amarillo); cout << " -- ";  color(cons, azul);
            cout << solicitudesUser[i];
            color(cons, amarillo); cout << " -- ";  color(cons, blanco);
            formatDate(solicitudesDate[i]);
        }
        cout << endl << "\n\t<<----------------------------------------->>" << endl;
        cout << "\n\t   A. Anterior / B. Siguiente / C. Volver";
        cout << "\n\n\t//"; getline(cin, op);
        fflush(stdin);
        op[0] = tolower(op[0]);
    } while (!(op[0] == '1' || op[0] == '2'|| op[0] == '3' || op[0] == '4' || op[0] == '5' || op[0] == '6' || op[0] == '7' || op[0] == '8' || op[0] == '9' ||
    op == "a" || op == "b" || op == "c") || op.length() > 2);

    switch(op[0]){  //opciones en base al pimer caracter de op
        case 'a':   //paiga anterior
            if (currentPage == 1) return showReq();  //si es la primera pagina no deja devovler
            else {
                currentPage--;
                return showReq();               //sino si
            }
        break;
        case 'b':   //pagina siguiente
        if (currentPage == pages) return showReq();  //si es la ultima pagina no deja seguir
            else{
                currentPage++;
                return showReq();               //sino si
            }
        break;
        case 'c':   //volver al menu
            return pmaindefMenu();
        break;
        default:
            int cursol = stoi(op);   //pasa el numero ingresado a una variable en entero
            if ((cursol > maxP * currentPage) || (cursol < (maxP * (currentPage - 1)) + 1) || cursol > solicitudesLib.size()){
                return showReq();
            }
            req(cursol, cursol - 1, cursol - 1);
        break;
    }
}