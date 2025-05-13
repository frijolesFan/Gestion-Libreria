# Sistema de gestión de librería

IMPORTARTE - Leer los comentarios de cada codigo para entender su funcionalidad, es de uso libre para modificar o redistribuir, agradecería cualquier crédito que pueda obtener por la realización de este proyecto en el que trabajé mucho, funciona perfectamente e implementa todo lo que nos pidió el profesor solo solo para obtener un 8/10 :c

HECHO POR UAN CAMILO GUEVARA OSORIO PARA LA ASIGNATURA DE PROGRAMACIÓN II CON CARLOS ALBERTO OCAMPO SEPÚLVEDA EN LA UNIVERSIDAD TECNOLÓGICA DE PEREIRA, PRESENTADO EL 18 DE NOVIEMBRE, 2023

---

El programa es un sistema de gestión de librería, este permite el uso de usuarios, trabajadores y
un administrador, los trabajadores tienen todos los permisos de los usuarios, además de poder acceder
a las solicitudes. El administrador tiene funcionalidades diferentes.

El programa inicia con un inicio de sesión, registro de un nuevo usuario o ingreso como un invitado.
De aquí el programa deriva en los siguientes caminos:

1. Inicio de sesión: Si se inicia sesión correctamente, el programa muestra las siguientes opciones
    1.1- Buscar libros: El programa muestra todos los libros disponibles
        I. Buscar libros por nombre
        II. Buscar libros por autor
        III. Ordenar libros alfabeticamente
            III.I- A-Z por nombre
            III.II- A-Z por autor
        IV. Mostrar información de un libro seleccionado:
            IV.I- Nombre
            IV.II- Autor
            IV.III- Descripcion
            IV.IV- Solicitar libro
                IV.IV.I- Si el libro está disponible envía la solicitud a los trabajadores
                IV.IV.II- Si el libro no está disponible no deja Solicitar
                IV.IV.III- Si el usuario se encuentra baneado no deja Solicitar
                IV.IV.IV- Si el usuario tiene 3 libros en posesión no deja Solicitar
    1.2- Mis libros: Muestra los libros tanto en posesion, como solicitados
        I. información del libro en posesión:
            I.I- Si el libro en posesion está expirado y se devuelve, lo libera
            I.II- Si el libro está solicitado y se devuelve, libera la solicitud
            I.III- Si la solicitud del libro fue rechazada, informa del rechazo y libera la solicitud
    1.3.1 (usuarios) / 1.4 (trabajadores). Perfil: Muestra información y opciones del Perfil
        I. Información básica: Nombre de usuario, tipo de usuario, correo y nombre completo
        II. Información completa: Documento, teléfono, dirección, contraseña
        III. Configuración del perfil.
            III.I- Modificar datos básicos:
                III.I.I- Nombre de usuario
                III.I.II- Correo electronico
                III.I.III- Número de teléfono
                III.I.IV- Dirección de vivienda
            III.II- Modificar contraseña
            III.III- Eliminar cuenta
    1.3.2- Solicitudes (solo trabajadores): Muestra las solicitudes enlistadas por páginas
        I. Información de una solicitud:
            I.I- Aceptar solicitud
            I.II- Rechazar solicitud

2. Inicio de sesión (administrador): Permite el uso de funciones de administrador
    2.1- Modificar permisos de usuario
        I. Cambiar usuario a trabajador
        II. Cambiar trabajador a usuario
    2.2- Modificar libros
        I. Agregar libro: Añade un libro al vector de libros
        II. Modificar libro: Modificar información básica de libros
            II.I- Nombre
            II.II- Autor
            II.III- Descripcion
            II.IV- Fecha de publicación
        III. Dar libro de baja: "Elimina" un libro, mas sin embargo, este se sigue mostrando a los usuarios
        IV. Eliminar libro: Elimina un libro del vector de libros

3. Registro de usuario: Permite registrar un usuario y lo almacena en el vector de usuarios

4. Ingresar como invitado: Permite Ingresar como invitado, solamente permite
    -Buscar libros: El programa muestra todos los libros disponibles
        I. Buscar libros por nombre
        II. Buscar libros por autor
        III. Ordenar libros alfabeticamente
            III.I- A-Z por nombre
            III.II- A-Z por autor
        IV. Mostrar información de un libro seleccionado:
            IV.I- Nombre
            IV.II- Autor
            IV.III- Descripcion

---

El programa guarda toda la información del programa en la carpeta "data/" en archivos .dat y la lee
y guarda mediante el codigo data.cpp contenido en esta carpeta.

Cuando el programa inicia, verifica la existencia de los usuarios, libros o solicitudes y segun si
existe o no, crea los archivos con los datos por defecto del programa o carga los datos guardados en
los archivos.

---

El programa es abierto para todo aquel que quiera modificarlo y mejorarlo.
