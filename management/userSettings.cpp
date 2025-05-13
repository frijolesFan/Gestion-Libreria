/*  CONFIGURACIONES DEL USUARIO

* Se encarga de las opciones y el manejo de los usuarios.

Se llama desde el main a la función profileMenu() que contiene la información general del
usuario como lo es:
    -Nombre de usuario
    -Tipo de usuario (trabajador o usuario)
    -Nombre completo
    -Correo electrónico

las opciones del menú son:
    -Información completa: Muestra toda la información del usuario actual.
    -Opciones del perfil: Incluye las diferentes acciones que se pueden hacer en el perfil,
    estas son:
        1. Modificar información
            I. Nombre de usuario
            II. Correo electrónico
            III. Número de teléfono
            IV. Dirección de vivienda
        2. Cambiar contraseña
        3. Eliminar cuenta

* Realizado por: Juan Camilo Guevara
* Fecha: 18/11/2023
* Compilado Versión: g++ (Rev7, Built by MSYS2 project) 13.1.0 [C/C++ visual studio code]
* userSettings.cpp
*/
#pragma once

#include "../data/data.cpp"
#include "../libs/database.cpp"
#include "../libs/formats.cpp"

void profileMenu();             
void profileSettings();         //menu de configuracion del perfil
void settingsModInfo();         //menu de modificacion de informacion del perfil

/// @brief Interfaz con informacion basica del usuario
void profileInfo(){
    cout << "\t--"; color(cons, amarillo);
    cout << users[currentuser].username; color(cons, blanco);
    cout << "--";
    color(cons, blanco);
    cout << "   /   ";
    switch(users[currentuser].usertype){    //muestra el tipo de usuario con un color distintivos
        case 1:
            color(cons, azul);
            cout << "Usuario";
        break;
        case 2:
            color(cons, rosa);
            cout << "Trabajador";
        break;
    }
    color(cons, blanco);
    cout << "\n\n\t" << users[currentuser].fullname;
    cout << "\n\t" << users[currentuser].email << endl;
}

/// @brief Muestra la informacion completa del usuario
/// @param profMenu Puntero al menu de opciones del usuario
void profileFullinfo(void (*profMenu)(void)){
    system("cls");
    profileInfo();
    cout << ((users[currentuser].doctype == 1) ? "\n\tCC. " : "\n\tTI. ") << users[currentuser].DNI;    //muestra el documento y su tipo
    cout << "\n\tTelefono: "; formatPhone();
    cout << "\n\tDireccion: " << users[currentuser].direct;
    cout << "\n\tContrasena: ";
    for (int i = 0; i < strlen(users[currentuser].password) + (rand() % strlen(users[currentuser].password)); i++){
        cout << "*";
    }
    cout << "\n\n\tEstado de la cuenta: ";
    if (verifyBan() == false){   //si está paz y salvo (no tiene libros pasados de fecha)
        color(cons, verde); 
        cout << "Paz y salvo" << endl << endl;
    }else{                              //si esta bloqueado (tiene libros pasados de fecha)
        color(cons, rojo);
        cout << "Bloqueado" << endl << endl;
    }
    color(cons, blanco);
    system("pause");
    return profMenu();
}

/// @brief Funcion para modificar la informacion de un usuario
/// @param tamano Largo del dato a modificar
/// @param dato Dato a cambiar (1 nombre de usuario. 2 correo. 3 numero de telefono. 4 direccion)
void modInfo(int tamano, int dato){
    system("cls");
    char *cadenachr = new char[tamano + 1]; //define un puntero de tipo char para cambiar directamente en memoria
    string datoacambiar;        //igualmente con el std::string. datoacambiar es un string que se muestra en la consola
    string InDato;                          //define la variable que almacena el nuevo dato
    int spaceN;                             //y ps define el numero de espacios pa que se vea bonito
    switch (dato){
        case 1:                                         //segun el caso, asigna el numero de espacios, puntero y el string correspondiente
            datoacambiar = "nombre de usuario";         //al dato que se desee cambiar
            spaceN = 2;
            cadenachr = users[currentuser].username;
        break;
        case 2:
            datoacambiar = "correo electronico";
            spaceN = 1;
            cadenachr = users[currentuser].email;
        break;
        case 3:
            datoacambiar = "numero de telefono";
            spaceN = 1;
            cadenachr = users[currentuser].phone;
        break;
        case 4:
            datoacambiar = "direccion";
            spaceN = 7;
            cadenachr = users[currentuser].direct;
        break;
    }
    color(cons, amarillo);
    cout << "\t  (introduzca 0 para regresar)" << endl;   //muestra instruccion para regresar
    color(cons, blanco);
    cout << "\n\t";

    for (int i = 0; i < spaceN; i++) cout << " ";   //muestra los espacios correspondientes

    cout << "Modificar " << datoacambiar << endl; 
    cout << "\n\tNuev" << ((datoacambiar == "direccion") ? "a " : "o ") << datoacambiar << " //";
    getline(cin, InDato);
    fflush(stdin);
    
    if (InDato[0] == '0' && InDato.length() == 1)  //si ingresa el numero para regresar
            return settingsModInfo();

    while (InDato.length() < 0 || InDato.length() > tamano){        //en caso de que el tamaño no sea el adecuado
        system("cls"); 
        color(cons, amarillo);
        cout << "\t  (introduzca 0 para regresar)" << endl;   //muestra instruccion para regresar
        color(cons, blanco);
        cout << "\n\t";
        for (int i = 0; i < spaceN; i++) cout << " ";               //vuelve a mostrar todo
        cout << "Modificar " << datoacambiar << endl;               //con mensaje de error indicando el tamaño maixmo

        color (cons, rojo);
        cout << "\t--TAMANO MAXIMO: " << tamano << " CARACTERES--" << endl; 
        color (cons, blanco);
        
        cout << "\n\tNuev" << ((datoacambiar == "direccion") ? "a " : "o ") << datoacambiar << " //";
        getline(cin, InDato);
        fflush(stdin);
        if (InDato[0] == '0' && InDato.length() == 1)  //si ingresa el numero para regresar
            return settingsModInfo();
    }

    char op;
    do{                 //confirma que la informacion sea correcta
        system("cls");
        cout << "\n\tNuev" << ((datoacambiar == "direccion") ? "a" : "o")<< " " << datoacambiar << endl;
        cout << "\n\t\t--"; color(cons, amarillo); cout << InDato << endl;
        color(cons, blanco);
        cout << "\n\tDesea continuar? (y/n)\n\t//"; cin >> op;
        op = tolower(op);               //convierte la opcion a minuscula
        fflush(stdin);
    }while (op != 'y' && op != 'n');    //y verifica que la que se ingrese sea correcta

    if (op == 'n') return modInfo(tamano, dato);  //si no desea continuar

    strcpy(cadenachr, InDato.c_str());
    
    saveUsers();
    return interstitial(verde, "Informacion actualizada", settingsModInfo); //muestra mensaje de exito y regresa al menu de configuracion
}

/// @brief Menu de configuracion de usuario
void settingsModInfo(){
    int op;
    do {
        system("cls");
        profileInfo();
        cout << "\tTelefono: "; formatPhone();
        cout << "\n\tDireccion: " << users[currentuser].direct << endl;
        cout << "\n\t1. Modificar nombre de usuario";
        cout << "\n\t2. Modificar correo electronico";
        cout << "\n\t3. Modificar numero de telefono";
        cout << "\n\t4. Modificar direccion";
        cout << "\n\t5. Regresar" << endl;
        cout << "\n\t//"; cin >> op;
        fflush(stdin);
    } while (op < 1 || op > 5);
    switch(op){
        case 1:
            for (int i = 0; i < 3; i++){                    //impide que se cambie el nombre de usuario si hay una solicitud activa
                if (users[currentuser].books[i] != -1){
                    for (int j = 0; j < books.size(); j++){
                        if (users[currentuser].books[i] == books[i].bookID && books[i].disp == 1){
                            return interstitial(rojo, "No puedes modificar tu nombre de usuario ahora", settingsModInfo);
                        }
                    }
                }
            }
            modInfo(16, op);  //modifica el nombre de usuario
        break;
        case 2:
            modInfo(100, op);  //modifica el correo electronico
        break;
        case 3:
            modInfo( 10, op);  //modifica el numero de telefono
        break;
        case 4:
            modInfo(100, op);  //modifica la direccion
        break;
        case 5:
            return profileSettings();
        break;
    } 
}

/// @brief Función para cambiar de contraseña
void settingsChangePassword(){
    string pass1, pass2;                        //para almacenar las contraseñas temporalmente
    char *finalPass = new char[17];             //puntero para modificar la contraseña
    finalPass = users[currentuser].password;    //apunto el puntero a la contraseña el usuario actual

    int errortype = 0;  //1: Tamaño, 2: contraseña muy poco segura, 3: la contraseña no puede ser igual al usuario
    do {
        system("cls");
        color(cons, amarillo);
        cout << "\t  (introduzca 0 para regresar)" << endl;   //muestra instruccion para regresar
        color(cons, blanco);
        cout << "\n\t      CAMBIAR CONTRASENA       " << endl;

        switch (errortype){         //en caso de que haya encontrado un error, muestra el mensaje correspondiente
            case 1:
                color (cons, rojo); 
                cout << "\t--Tamano maximo: 16 caracteres--" << endl;  //error por tamaño
                color (cons, blanco);
            break;
            case 2:
                color (cons, rojo); 
                cout << "\t --Use una contrasena segura--" << endl;   //error por contraseña muy predecible
                color (cons, blanco);
            break;
            case 3:
                color (cons, rojo); 
                cout << "\t    --Use otra contrasena--" << endl;      //error por usar el mismo nombre de usuario
                color (cons, blanco);
            break;
        }
        cout << "\n\tNueva contrasena: //"; getline(cin, pass1);
        fflush(stdin);

        if (pass1[0] == '0' && pass1.length() == 1)  //si ingresa el numero para regresar
            return profileSettings();

        //verifica que no haya ningun error en la contraseña
        if (pass1.length() > 16 || pass1.length() < 1) errortype = 1;
        else if (pass1 == "12345678" || pass1 == "abcdefg" || pass1 == "abc12345") errortype = 2;
        else if (strcmp(pass1.c_str(), users[currentuser].username) == 0) errortype = 3;
        else errortype = 0;
    } while (errortype != 0);
    
    do {                //pide confirmar la contraseña
        system("cls");
        color(cons, amarillo);
        cout << "\t(0 para modificar la contrasena)" << endl;   //muestra instruccion para regresar
        color(cons, blanco);
        cout << "\n\t      CAMBIAR CONTRASENA       " << endl;
        
        if (errortype != 0){        //si las contraseñas no coinciden
            color (cons, rojo);
            cout << "\t--Las contrasenas no coinciden--" << endl;
            color (cons, blanco);
        }
        cout << "\n\tNueva contrasena: //" << pass1;  //muestra la contrasena anterior
        cout << "\n\tRepita la contrasena: //";       //pide que se repita la contrasena 
        getline(cin, pass2);
        fflush(stdin);

        if (pass2[0] == '0' && pass2.length() == 1) return settingsChangePassword();    //regresa a la contrasena anterior
        if (pass1 != pass2) errortype = 1;
        else if (pass1 == pass2) errortype = 0;

    } while (errortype != 0);

    char op;
    do{                 //confirma que la informacion sea correcta
        system("cls");
        cout << "\n\tNueva contrasena: "<< endl;
        cout << "\n\t\t--"; color(cons, amarillo); cout << pass1 << endl;
        color(cons, blanco);
        cout << "\n\tDesea continuar? (y/n)\n\t//"; cin >> op;
        op = tolower(op);               //convierte la opcion a minuscula
        fflush(stdin);
    }while (op != 'y' && op != 'n');

    if (op == 'n') return profileSettings();

    strcpy(finalPass, pass1.c_str());

    saveUsers();
    interstitial(verde, "Contrasena cambiada correctamente", profileSettings);
}

/// @brief Funcion para eliminar una cuenta
void deleteAccount(){
    string contrasena, rep;
    system("cls");
    cout << "\n\tContrasena: //"; getline(cin, contrasena);
    fflush(stdin);
    cout << "\tRepetir contrasena: //"; getline(cin, rep);
    fflush(stdin);
    if (strcmp(contrasena.c_str(), users[currentuser].password) != 0){
        return interstitial(rojo, "Contrasena incorrecta", profileSettings);
    }
    if (strcmp(contrasena.c_str(), rep.c_str()) != 0){
        return interstitial(rojo, "Las contrasenas no coinciden", profileSettings);
    } else {
        char op;
        do {
            color(cons, amarillo);
            cout << "\n\tEsta seguro de eliminar su cuenta?";
            color(cons, rojo);
            cout << "\n\t     (No se puede deshacer)";
            color(cons, blanco);
            cout << "\n\n\t(y/n)";
            cout << "\n\n\t//"; cin >> op;
        } while (op != 'y' && op != 'n');
        switch (op){
            case 'y':
                users.erase(users.begin() + currentuser);
                saveUsers();
                return interstitial(rojo, "Su cuenta ha sido eliminada", pinitmenu);
            break;
            case 'n':
                return profileSettings();
            break;
        }
    }
}

/// @brief Muestra la configuracion en el perfil
void profileSettings(){
    int op;
    do {
        system("cls");
        cout << "\n\t         CONFIGURACION         " << endl;
        cout << "\n\t1. Modificar Informacion";
        cout << "\n\t2. Cambiar contrasena";
        color (cons, rojo);
        cout << "\n\t3. Eliminar cuenta";
        color (cons, blanco);
        cout << "\n\t4. Regresar" << endl;
        cout << "\n\t//"; cin >> op;
        fflush(stdin);
    } while (op < 1 || op > 4);
    switch(op){
        case 1:
            return settingsModInfo();
        break;
        case 2:
            return settingsChangePassword();
        break;
        case 3:
            return deleteAccount();
        break;
        case 4:
            return profileMenu();   //regresa al menu de usuario
        break;
    }
}

/// @brief Menu de opciones de perfil de usuario
void profileMenu(){
    int op;
    do{
        system("cls");
        profileInfo();  //muestra los datos basicos
        cout << "\n\t1. Informacion completa";
        cout << "\n\t2. Configuracion";
        cout << "\n\t3. Regresar" << endl;
        cout << "\n\t//"; cin >> op;
        fflush(stdin);
    } while (op < 1 || op > 3);
    switch(op){
        case 1:
            return profileFullinfo(profileMenu);
        break;
        case 2:
            return profileSettings();
        break;
        case 3:
            return pmaindefMenu();
        break;
    }
}