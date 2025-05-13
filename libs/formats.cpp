/* FORMATOS Y DEMAS

* Este codigo incluye funciones que, en su mayoria, son de interfaz: muestran un
texto en cierto formato, estos son
    -Interstitals: Texto que ocupa toda la pantalla, redirige a un menu
    -Telefono: Muestra el telefono en formato XXX XXX XXXX
    -Parrafos: Muestra un parrafo separado en renglones, cada 32 caracteres
    -Fecha: Muestra una fecha en formato DD de MM, AA (31 de diciembre, 2023)

* Realizado por: Juan Camilo Guevara
* Fecha: 13/11/2023
* Compilado Versión: g++ (Rev7, Built by MSYS2 project) 13.1.0 [C/C++ visual studio code]
* formats.cpp
*/
#pragma once

#include "database.cpp"

/// @brief Muestra un mensaje en pantalla y llama a una funcion de regreso mediante punteros
/// @param consColor Color del texto
/// @param mensaje El texto en cuestion
/// @param funcion Funcion de regreso
void interstitial(int consColor, string mensaje, void (*funcion)(void)){
    system("cls");
    color(cons, consColor);
    cout << "\n\n\t   --" << mensaje << "--" << endl;
    Sleep(2000);
    color(cons, blanco);
    return funcion();
}

/// @brief muestra el telefono en formato xxx xxx xxxx (123 456 7890)
void formatPhone(){
    for (int i = 0; i < strlen(users[currentuser].phone); i++){
        if (i == 3 || i == 6)
            cout << " ";
        cout << users[currentuser].phone[i];
    }
}

/// @brief Muestra un parrafo separado en renglones (que funcion tan bonita)
/// @param parrafo El parrafo que se va a mostrar
/// @param limite caracter limite, donde se va a mostrar el salto de linea
void formatParraf(const char parrafo[], int limite){
    bool saltoLinea = false;
    cout << "\t";
    for (int i = 0; i < strlen(parrafo); i++){
        if ((i + 1) % limite == 0){ //activa la variable para poner un salto de linea
            saltoLinea = true;
        }
        if (saltoLinea)
            if (parrafo[i] == ' '){ //cuando alcane un espacio reemplaza por salto de linea
                cout << "\n\t";
                saltoLinea = false;
                i++;
            }
        cout << parrafo[i];
    }
}

/// @brief Muestra la fecha segun un formato
/// @param fecha La fecha que se mostrará
void formatDate(struct date fecha) {
    if (fecha.year < 0) { // Si es antes de Cristo
        fecha.year *= -1;
        cout << fecha.year << " a.C";
    } else if (fecha.year > 0 && fecha.year < 1000) { // Si es después de Cristo
        cout << fecha.year << " d.C";
    } else { // Si es una fecha normal
        string mes;
        switch (fecha.month) {
            case 1: mes = "enero"; break;
            case 2: mes = "febrero"; break;
            case 3: mes = "marzo"; break;
            case 4: mes = "abril"; break;
            case 5: mes = "mayo"; break;
            case 6: mes = "junio"; break;
            case 7: mes = "julio"; break;
            case 8: mes = "agosto"; break;
            case 9: mes = "septiembre"; break;
            case 10: mes = "octubre"; break;
            case 11: mes = "noviembre"; break;
            case 12: mes = "diciembre"; break;
        }
        cout << fecha.day << " de " << mes << ", " << fecha.year;
    }
}