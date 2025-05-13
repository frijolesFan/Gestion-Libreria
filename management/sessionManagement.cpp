/*  MANEJO DE SESIONES

* Se encarga del manejo de las sesiones. Permite:
    -Iniciar sesión
    -Registrar usuario
    -Ingresar como invitado

Este codigo depende enteramente del main.cpp, ya que todas las funciones de este codigo
se manejan desde ahi.

* Realizado por: Juan Camilo Guevara
* Fecha: 17/11/2023
* Compilado Versión: g++ (Rev7, Built by MSYS2 project) 13.1.0 [C/C++ visual studio code]
* sessionManagement.cpp
*/
#pragma once

#include "../admin/admin.cpp"
#include "../libs/database.cpp"
#include "../libs/formats.cpp"
#include "../data/data.cpp"

/* Verifica si el usuario existe
-1: cotraseña incorrecta
-2: usuario no encontrado */

/// @brief Verifica si un usuario existe
/// @param n Vector de caracteres correspondiente al nombre de usuario
/// @param c Vector de caracteres correspondiente a la contraseña
/// @return i + 1: Usuario encontrado / -1: Contraseña incorrecta / -2: Usuario no encontrado
int validUser(char n[17], char c[17]){
    if (strcmp(c, adm.password) == 0 && strcmp(n, adm.username) == 0){   //si el usuario es el administrador
        return 0; 
    }else{
        for (int i = 0; i < users.size(); i++)
            if (strcmp(n, users[i].username) == 0 && strcmp(c, users[i].password) == 0) //si es un usuario existente
                return i + 1;
        
        //      --posibles casos donde no coindicen usuario y contraseña--

        for (int i = 0; i < users.size(); i++)    //contraseña incorrecta
            if (strcmp(n, users[i].username) == 0)
                return -1;
        return -2;                          //usuario no encontrado
    }
}

/// @brief Funcion para inicio de sesion
/// @param error Codigos de error al ingresar datos
/// @param initmenu Puntero a la funcion initmenu en el main.cpp
void login(int error, void (*initmenu)(void)){
    system("cls");
    string name, pass;
    char cname[16], cpass[16];

    color(cons, amarillo);
    cout << "\t  (introduzca 0 para regresar)" << endl;   //muestra instruccion para regresar
    color(cons, blanco);
    cout << "\n\t        INICIAR SESION" << endl;
    switch (error){
        case -1:    //verifica si la contraseña es incorrecta
            color(cons, rojo);
            cout << "\t   --Contrasena incorrecta--   \n";
        break;
        case -2:    //verifica si el usuario el usuario existe
            color(cons, rojo);
            cout << "\t   --El usuario no existe--    \n";
        break;
        case -3:    //verifica si el usuario es de mas de 16 caracteres
            color(cons, rojo);
            cout << "\t     --Usuario invalido--      \n";
        break;
        case -4:    //verifica si la contraseña es de mas de 8 caracteres
            color(cons, rojo);
            cout << "\t    --Contrasena invalida--      \n";
        break;
        default: break; //sino, no muestra nada
    }
    color(cons, blanco);
    cout << "\n\tNombre de usuario: //"; getline(cin, name);  //pide el nombre
    fflush(stdin);

    if (name[0] == '0' && name.length() == 1) return initmenu();    //verifica si se quiere regresar
    if (name.length() > 16) return login(-3, initmenu);                       //verifica si el tamaño del usuario es valido  
    strcpy(cname, name.c_str());                                    //pasa el std::string a un char [17]

    char seguro[16];                                        //*/         por algun motivo, cname se borra algunas veces al pasar a validUser         /*//
    strcpy(seguro, cname);                                  //*/entonces, creo un seguro que es una copia de cname para no perderlo en cualquier caso/*//

    cout << "\tContrasena: //"; getline(cin, pass);           //pide la contraseña
    fflush(stdin);

    if (pass[0] == '0' && pass.length() == 1) return initmenu();    //verifica si se quiere regresar
    if (pass.length() > 16) return login(-4, initmenu);                       //verifica si el tamaño de la contraseña es valido  
    strcpy(cpass, pass.c_str());                                    //pasa el std::string a un char [16]

    int val = validUser(seguro, cpass);    //valida si existe el usuario

    switch(val){
        case -1: case -2:
            return login(val, initmenu);  //pide datos de nuevo si validUser regresa un error
        break;
        default:
            currentuser = val - 1;
            switch(currentuser){
                case -1:
                    return adminMenu();     //muestra el menu del administrador
                break;
                default:
                    updateBan();                //actualiza si esta baneado o no
                    return pmaindefMenu();
                break;
            }
        break;
    }
}

/// @brief Muestra una animacion y cierra la sesion actual
/// @param funcion Puntero a la funcion de regreso
void logout(void (*funcion)(void)){
    system("cls");
    char bye[16] = "Cerrando sesion";
    cout << "\n\n\t        " << bye;

    int recent, last;       //guarda el color actual y el anterior para que no se repitan

    srand(time(NULL));      //semilla para los colores del texto
    Sleep((rand() % 500) + 50);    //espera dinamica para simular carga

    for (int i = strlen(bye); i >= 0; i--){
        Sleep(25);  //le da espera al siguiente frame
        system("cls");
        cout << "\n\n\t        ";

        recent = (rand() % 6) + 9;      //atribuye un valor aleatorio al color
        while(recent == last){          //lo cambia mientras este sea igual al atnerior
            recent = (rand() % 6) + 9;  //cambia el color a uno solido aleatorio (los colores solidos van de 9 a 15)
        }
        color(cons, recent);    //cambia el color de la consola al reciente
        last = recent;          //actualiza el color para la siguiente iteracion

        for (int j = 0; j < i; j++){
            cout << bye[j];             //dibuja el texto, con -1 caracter por paso
        }
    }
    color(cons, blanco);    //reestablece el color de la consola
    currentuser = -420;     //borra el usuario actual
    Sleep(rand() % 500);    //espera dinamica para simular carga
    return funcion();       //regresa al menu principal mediante punteros
}

/// @brief Muestra que se esta registrando un usuario (para evitar reescribir codigo)
void registerHeader(){
    system("cls");
    color(cons, amarillo);
    cout << "\t  (introduzca 0 para regresar)" << endl;   //muestra instruccion para regresar
    color(cons, blanco);
    cout << "\n\t       REGISTRAR USUARIO" << endl;
}

/// @brief Funcion para registrar un usuario
/// @param error Codigo de error al registrar un usuario
/// @param fase La fase actual de registro
/// @param initmenu Puntero al menu de inicio
/// @param temporal Usuario temporal, el que se va a almacenar en el vector
/// @param tempC Contraseña temporal
void registerUser(int error, int fase, void (*initmenu)(void), struct user temporal = {}, string tempC = ""){
    string username, c1, c2, fullname, dni, direccion, email, telefono;
    int op;
    switch (fase){
        case 1: //para ingresar el usuario
            registerHeader();
            switch (error){
                case -1:    //verifica si el usuario es de mas de 16 caracteres
                    color(cons, rojo);
                    cout << "\t      --Usuario invalido--      \n";
                break;
                case -2:    //verifica si el usuario es de mas de 16 caracteres
                    color(cons, rojo);
                    cout << "\t   --Este usuario ya existe--   \n";
                break;
                default: break; //sino, no muestra nada
            }
            color(cons, blanco);
            cout << "\n\tNombre de usuario: //"; getline(cin, username);
            fflush(stdin);
            
            if (username[0] == '0' && username.length() == 1) return initmenu();        //si ingresa el codigo para regresar
            if (username.length() > 16) return registerUser(-1, 1, initmenu, temporal); //manda error de tamaño de usuario
            for (int i = 0; i < users.size(); i++){
                if (strcmp(username.c_str(), users[i].username) == 0){
                    return registerUser(-2, 1, initmenu, temporal);                     //manda error de repeticion de usuario
                    break;
                }
            }
            strcpy(temporal.username, username.c_str());

            return registerUser(0, 2, initmenu, temporal);  //pasa a la siguiente fase
        break;
        case 2: //para ingresar contraseña
            registerHeader();
            switch (error){
                case -1:    //verifica si el usuario es de mas de 16 caracteres
                    color(cons, rojo);
                    cout << "\t    --Contrasena invalida--      \n";
                break;
                default: break; //sino, no muestra nada
            }
            color(cons, blanco);
            cout << "\n\tNombre de usuario: //" << temporal.username;
            cout << "\n\tContrasena: //"; getline(cin, c1);
            fflush(stdin);

            if (c1[0] == '0' && c1.length() == 1) return registerUser(0, 1, initmenu, temporal);   //si ingresa el codigo para regresar
            if (c1.length() > 16) return registerUser(-1, 2, initmenu, temporal);                       //manda error de tamaño de usuario
            
            return registerUser(0, 3, initmenu, temporal, c1);
        break;
        case 3: //repetir contraseña
            registerHeader();
            switch (error){
                case -1:
                    color(cons, rojo);
                    cout << "\t --Las contrasenas no coinciden--\n";
                default: break; //sino, no muestra nada
            }
            color(cons, blanco);
            cout << "\n\tNombre de usuario: //" << temporal.username;   //muestra los datos ingresados anteriormente
            cout << "\n\tContrasena: //" << tempC;
            cout << "\n\tConfirmar contrasena: //"; getline(cin, c2);
            fflush(stdin);

            if (c2[0] == '0' && c2.length() == 1) return registerUser(0, 2, initmenu, temporal);   //si ingresa el codigo para regresar
            if (strcmp(tempC.c_str(), c2.c_str()) != 0) return registerUser(-1, 3, initmenu, temporal, tempC);  //si las contraseñas no son iguales

            strcpy(temporal.password, c2.c_str());
            return registerUser(0, 4, initmenu, temporal);
        break;
        case 4: //pide el nombre completo
            registerHeader();
            switch (error){
                case -1:
                    color(cons, rojo);
                    cout << "\t --Ingrese un nombre mas corto--\n";
                default: break; //sino, no muestra nada
            }
            cout << "\n\tNombre completo: //"; getline(cin, fullname);
            fflush(stdin);

            if (fullname[0] == '0' && fullname.length() == 1) return registerUser(0, 2, initmenu, temporal);   //si ingresa el codigo para regresar
            if (fullname.length() > 100) return registerUser(-1, 4, initmenu, temporal);
            strcpy(temporal.fullname, fullname.c_str());
            
            return registerUser(0, 5, initmenu, temporal);
        break;
        case 5: //pide el tipo de documento
            do {
                registerHeader();
                cout << "\n\tTipo de documento: \n\t  1. CC.\n\t  2. TI.";
                cout << "\n\n\t//"; cin >> op;
                fflush(stdin);
            } while (op != 1 && op != 2 && op != 0);

            if (op == 0) return registerUser(0, 4, initmenu, temporal);
            temporal.doctype = op;

            return registerUser(0, 6, initmenu, temporal);
        break;
        case 6: //pide el numero de documento
            registerHeader();
            switch (error){
                case -1:
                    color(cons, rojo);
                    cout << "\t--Ingrese un documento mas corto--\n";
                default: break; //sino, no muestra nada
            }
            cout << "\n\tNumero de " << ((temporal.doctype == 1) ? "CC. " : "TI. ") << ": //";
            getline(cin, dni);
            fflush(stdin);

            if (dni[0] == '0' && dni.length() == 1) return registerUser(0, 5, initmenu, temporal);
            if (dni.length() > 20) return registerUser(-1, 6, initmenu, temporal);

            strcpy(temporal.DNI, dni.c_str());
            return registerUser(0, 7, initmenu, temporal);
        break;
        case 7: //pide la direccion de vivienda
            registerHeader();
            switch (error){
                case -1:
                    color(cons, rojo);
                    cout << "\t--Ingrese una direccion mas corta--\n";
                default: break; //sino, no muestra nada
            }
            cout << "\n\tDireccion de vivienda: //"; getline(cin, direccion);
            fflush(stdin);

            if (direccion[0] == '0' && direccion.length() == 1) return registerUser(0, 6, initmenu, temporal);
            if (direccion.length() > 100) return registerUser(-1, 7, initmenu, temporal);
            strcpy(temporal.direct, direccion.c_str());

            return registerUser(0, 8, initmenu, temporal);
        break;
        case 8: //pide el correo electronico    ---------------------------------------------------
            registerHeader();
            switch (error){
                case -1:
                    color(cons, rojo);
                    cout << "\t  --Ingrese un correo mas corto--\n";
                default: break; //sino, no muestra nada
            }
            cout << "\n\tCorreo electronico: //"; getline(cin, email);
            fflush(stdin);

            if (email[0] == '0' && email.length() == 1) return registerUser(0, 7, initmenu, temporal);
            if (email.length() > 100) return registerUser(-1, 8, initmenu, temporal);
            strcpy(temporal.email, email.c_str());

            return registerUser(0, 9, initmenu, temporal);
        break;
        case 9: //pide el numero de telefo
            registerHeader();
            switch (error){
                case -1:
                    color(cons, rojo);
                    cout << "\t     --Telefono invalido--      \n";
                default: break; //sino, no muestra nada
            }
            cout << "\n\tCorreo electronico: //" << temporal.email;
            cout << "\n\tTelefono: //"; getline(cin, telefono);
            fflush(stdin);

            if (telefono[0] == '0' && telefono.length() == 1) return registerUser(0, 9, initmenu, temporal);
            if (telefono.length() > 10) return registerUser(-1, 9, initmenu, temporal);                       //manda error de tamaño de usuario

            strcpy(temporal.phone, telefono.c_str());

            return registerUser(0, 10, initmenu, temporal);
        break;
        case 10:    //finaliza el proceso de registros
            temporal.usertype = 1;              //establece la clase de usuario
            for (int i = 0; i < 3; i++){        //pone toda informacion de los libros en blanco
                temporal.books[i] = -1;
                temporal.ban[i] = false;
                temporal.entrega[i].day = -1;
                temporal.entrega[i].month = -1;
                temporal.entrega[i].year = -1;
            }
            users.push_back(temporal);          //añade el usuario al vector
            saveUsers();
            return interstitial(verde, "El usuario se ha registrado correctamente", initmenu);
        break;
    };
}