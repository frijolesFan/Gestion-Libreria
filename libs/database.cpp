/* BASE DE DATOS

* Incluye los datos por defecto con los que funciona el programa, ademas, contiene
la información necesarioa para cambiar el color de la consola, tal es la definicion
de color como reemplazo de SetConsoleTextAttribute, la variable cons que maneja
la consola y los coddigos de los colores a usar.
Contiene además contiene
    -El usuario actual
    -Punteros a menus del main
    -Vectores de usuarios, libros y solicitudes por defecto
    -Informacion del administrador
    -Funcion para verificar si un usuario esta baneado
    -Fecha actual
    -Funcion para actualizar la fecha
    -Funcion para actualizar el baneo

* Realizado por: Juan Camilo Guevara
* Fecha: 14/11/2023
* Compilado Versión: g++ (Rev7, Built by MSYS2 project) 13.1.0 [C/C++ visual studio code]
* database.cpp
*/
#pragma once

#include "structs.cpp"  //incluye las estructuras de usuarios y libros y las librerias
#define color SetConsoleTextAttribute   //define la funcion de windows.h como color para hacerlo mas legible

int verde = 10, azul = 11, rojo = 12, rosa = 13,  amarillo = 14, blanco = 15;   //define los colores predeterminados a usar
HANDLE cons = GetStdHandle(STD_OUTPUT_HANDLE);                                  //usa esto para cambiar los colores de la consolas

int currentuser = 0;    //almacena el usuario actual

void (*pmaindefMenu)(void);
void (*pinitmenu)(void);

/// @brief Estructura inamovible del administrador
struct user adm = {
    3,                                  //Usuario administrador
    "admin",                            //nombre de usuario
    "admin",                            //contraseña de usuario
    "Administrador",                    //nombre completo
    1,                                  //tipo de documento
    "1105365091",                       //numero de documento
    "juan.guevara3@utp.edu.co",         //correo electronico
    "cra 13 #69-420",                   //direccion
    "3135242052",                       //numero de telefono
    {-1, -1, -1},                       //n. libros prestados
    {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}},   //fecha de entrega de libros prestados
    0
};

/// @brief Vector contenedor de los usuarios
vector<user> users = {  //crea un arreglo dinamico de tamaño inicial 3
    {
        1,
        "fiori_soji",
        "bolt828",
        "Fiorella Goyes Higuera",
        1,
        "1234567890",
        "fiorella.goyes@utp.edu.co",
        "cra 14 #14-14",
        "3248461590",
        {18, 6, 9},
        {{-1, -1, -1}, {2023, 11, 2}, {2023, 12, 25}},
        {false, false, false}
    },{
        1,
        "juanf.gomez",
        "onepiece",
        "Juan Felipe Gomez Cardona",
        2,
        "1234567890",
        "felipe.gomez4@utp.edu.co",
        "cra 1 #24-12",
        "3115024987",
        {-1, -1, -1},
        {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}},
        {false, false, false}
    },{
        2,
        "cataqq6",
        "kiwi2006",
        "Catalina Quintana Torres",
        1, 
        "1234567890",
        "catalina.torres@utp.edu.co",
        "cra 9 #27-98",
        "3135556287",
        {-1, -1, -1},
        {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}},
        {false, false, false}
    }
};

/// @brief Verifica si el usuario esta baneado o no
/// @return False: no -- True: si
bool verifyBan(){
    if (users[currentuser].ban[0] == false && users[currentuser].ban[1] == false && users[currentuser].ban[2] == false) return false;
    else return true;
}

/// @brief Vector contenedor de los libros
vector<book> books = {
    {
        1,
        "El Principito",
        "Antoine de Saint-Exupery",
        "Un cuento poetico que viene acompanado de ilustraciones con acuarelas hechas por su mismo autor",
        {1943, 4, 6},
        1
    },{
        2,
        "Cien anos de soledad",
        "Gabriel Garcia Marquez",
        "Narra la historia de la familia Buendia a lo largo de siete generaciones en el pueblo ficticio de Macondo",
        {1967, 6, 5},
        1
    },{
        3,
        "Harry Potter y la piedra filosofal",
        "J. K. Rowling",
        "Harry Potter descubre que es hijo de dos conocidos hechiceros y debe acudir a la famosa escuela de magia: Hogwarts",
        {1997, 9, 1},
        1
    },{
        4,
        "La Iliada",
        "Homero",
        "Narra los acontecimientos ocurridos durante 51 dias en el decimo y ultimo ano de la guerra de Troya",
        {-750, 0, 0},
        -1
    },{
        5,
        "El codigo Da Vinci",
        "Dan Brown",
        "Narra los intentos de Robert Langdon para resolver el misotioros asesinado de Jacques Sauniere en el Museo del Louvre",
        {2003, 3, 18},
        1
    },{
        6,
        "50 Sombras de Grey",
        "E. L. James",
        "Primera parte de la trilogia totalmente adictiva que te poseera y quedara para siempre en tu memoria",
        {2012, 6, 6},
        0
    },{
        7,
        "El Perfume",
        "Patrick Suskind",
        "Transporta al lector a un mundo con el que no esta familiarizado, a traves de su protagonista, Jean-Baptiste Grenouille",
        {1985, 4, 4},
        1
    },{
        8,
        "12 Reglas para vivir",
        "Jordan Peterson",
        "Proporciona consejos a trabes de ensayos que abarcan temas como la etica, la psicologia, la mitologia y la religion",
        {2018, 1, 23},
        1
    },{
        9,
        "Boulevard",
        "Flor M. Salvador",
        "Narra la dificil relacion entre una chica con costumbres corrientes y un chico con pasado tormentoso",
        {2022, 2, 11},
        0
    },{
        10,
        "El Escarabajo de oro",
        "Edgar Allan Poe",
        "La aventura de un individuo que encuentra un mensaje cifrado en el que se indica la localizacion del tesoro del capitan Kidd",
        {1843, 6, 21},
        1
    },{
        11,
        "Narraciones Extraordinarias",
        "Edgar Allan Poe",
        "Una recopilacion de cuentos y poemas de Poe, tales como: \"El escarabajo dorado\", \"El cuervo\", \"El gato negro\", etc.",
        {1845, 2, 15},
        1
    },{
        12,
        "El castillo ambulante",
        "Diana Wynne Jones",
        "Habla de la historia de Sophie, quien cree que esta condenada a fracasar, contrastando con Howl quien parece dueno del destino",
        {1986, 4, 1},
        1
    },{
        13,
        "El Alquimista",
        "Paulo Coelho",
        "Relata las aventuras de Santiago, quien emprende un viaje por el desierto en busca de un tesoro",
        {1994, 10, 7},
        1
    },{
        14,
        "Still with you",
        "Lily del Pilar",
        "El oficial Jong Sungguk nunca imagino que dentro de una casa normal encontraria una mujer muerta y un chico encerrao en el atico",
        {2021, 5, 1},
        1
    },{
        15,
        "Al Final Mueren los dos",
        "Adam Silvera",
        "Imagina como vivirias tu vida en solo un dia, Estarias listo para decir adios?",
        {2017, 9, 5},
        1
    },{
        16,
        "1984",
        "George Orwell",
        "En 1984, Londres, la policia del pensamiento controla de forma asfixiante la vida de los ciudadanos",
        {1949, 6, 8},
        1
    },{
        17,
        "Dios es gay",
        "Nicolas Barbosa",
        "El amor no es un sentimiento, es una forma de vivir. Te has preguntado la orientacion sexual de Dios?",
        {2022, 10, 27},
        1
    },{
        18,
        "El extrano caso del Dr. Jekyll y Mr. Hyde",
        "Robert Louis Stevenson",
        "Si me hubiera aproximado a mi descubrimiento con un espiritu mas noble, habria salido de estas agonias convertido en un angel en lugar de un demonio",
        {1886, 1, 5},
        1
    },{
        19,
        "El Jardin de las Mariposas",
        "Dot Hutchison",
        "Cerca de una aislada mansin existe un ardin en el que un delicado jardinero cultiva delicadas flores.",
        {2016, 5, 5},
        1
    },{
        20,
        "El Resplandor",
        "Stephen King",
        "Esa es la palabra que Danny habia visto en el espejo. Y, aunque no sabia leer, entendio que era un mensaje de horror",
        {1977, 1, 28},
        1
    },{
        21,
        "Wigetta: un viaje magico",
        "Vegetta777, Willyrex",
        "Una historia ilustrada e interactiva que se inspira en el mundo de los videojuegos y en el universo de los celebres Willyrex y Vegetta777",
        {2015, 3, 17},
        1,
    }
};

/// @brief Contenedor del identificador de los libros solicitados
vector<int> solicitudesLib = {18};
/// @brief contenedor de los nombres de los usuarios que solicitaron libro
vector<string> solicitudesUser = {"fiori_soji"};
/// @brief Contenedor de la fecha en que los libros fueron solicitados
vector<date> solicitudesDate = {{2023, 11, 5}};

/// @brief Fecha actual
struct date currentDate;

/// @brief Actualiza la fecha actual, mediante la libreria chrono
void updateDate(){
    using namespace chrono;
    // Obtiene la fecha actual
    auto now = system_clock::now();
    time_t current_time = system_clock::to_time_t(now);

    // Convierte la fecha y hora actual en una estructura "tm"
    tm* local_time = localtime(&current_time);

    currentDate.year = local_time->tm_year + 1900;   //el año tiene un offset de 1900
    currentDate.month = local_time->tm_mon + 1;     //el mes tiene un offset de 1
    currentDate.day = local_time->tm_mday;         //el dia no tiene offset
}

/// @brief Actualiza el estado de baneo del usuario
void updateBan(){
    for (int i = 0; i < 3; i++){
        if (currentDate.year > users[currentuser].entrega[i].year || currentDate.month > users[currentuser].entrega[i].month  || ( currentDate.month >= users[currentuser].entrega[i].month && currentDate.day > users[currentuser].entrega[i].day)){
            if (users[currentuser].entrega[i].day == -1){
                users[currentuser].ban[i] = false;
            } else {
                users[currentuser].ban[i] = true;
            }
        }
        else {
            users[currentuser].ban[i] = false;
        }
    }
}