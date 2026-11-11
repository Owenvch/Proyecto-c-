#include <iostream>

using namespace std;

//Herramientas

void copiarTexto(char*& destino, const char* origen)
{
    int longitud = 0;
    while (origen[longitud] != '\0')
    {
        longitud++;
    }

    destino = new char[longitud + 1];

    for (int i = 0; i <= longitud; i++)
    {
        destino[i] = origen[i];
    }
}

bool sonIguales(const char* texto1, const char* texto2)
{
    int i = 0;
    while (texto1[i] != '\0' && texto2[i] != '\0')
    {
        if (texto1[i] != texto2[i])
        {
            return false;
        }
        i++;
    }
    return texto1[i] == texto2[i];
}

//Interfaz Grafica (Dibujo en pantalla)

class Pantalla
{
public:
    void limpiar()
    {
        // Código ANSI multiplataforma
        cout << "\033[2J\033[1;1H";
    }

    void dibujarMenuLogin()
    {
        limpiar();
        cout << "========================================\n";
        cout << "    SISTEMA PHARMALOGIC - PRE-INGRESO   \n";
        cout << "========================================\n";
        cout << "1. Iniciar Sesion\n";
        cout << "2. Registrar Nuevo Empleado\n";
        cout << "3. Salir del Sistema\n";
        cout << "========================================\n";
        cout << "Elige una opcion: ";
    }

    void mostrarMensaje(const char* mensaje)
    {
        cout << "\n" << mensaje << "\n";
        cout << "Escribe 'c' y presiona Enter para continuar... ";
        char p[2];
        cin >> p;
    }
};

//Capa Modelo y Logica

class Empleado
{
private:
    char* usuario;
    char* password;

public:
    Empleado(const char* _usuario, const char* _password)
    {
        copiarTexto(usuario, _usuario);
        copiarTexto(password, _password);
    }

    ~Empleado()
    {
        delete[] usuario;
        delete[] password;
    }

    bool validarCredenciales(const char* _userIntent, const char* _passIntent)
    {
        return sonIguales(usuario, _userIntent) && sonIguales(password, _passIntent);
    }
};

class SistemaLogin
{
private:
    // Arreglo de punteros estático en RAM
    Empleado* nomina[10];
    int totalEmpleados;

    // ATRIBUTO PROTEGIDO: La clave maestra encapsulada(Para simular seguridad)
    const char* CLAVE_MAESTRA = "ADMIN123";

public:
    SistemaLogin()
    {
        totalEmpleados = 0;
        // Creamos una cuenta por defecto
        nomina[totalEmpleados] = new Empleado("admin", "1234");
        totalEmpleados++;
    }

    ~SistemaLogin()
    {
        for (int i = 0; i < totalEmpleados; i++)
        {
            delete nomina[i];
        }
    }

    bool verificarLogin(const char* usr, const char* pass)
    {
        for (int i = 0; i < totalEmpleados; i++)
        {
            if (nomina[i]->validarCredenciales(usr, pass))
            {
                return true;
            }
        }
        return false;
    }

    bool registrarNuevo(const char* intentoClave, const char* nUser, const char* nPass)
    {
        if (sonIguales(intentoClave, CLAVE_MAESTRA))
        {
            if (totalEmpleados < 10)
            {
                nomina[totalEmpleados] = new Empleado(nUser, nPass);
                totalEmpleados++;
                return true;
            }
        }
        return false;
    }
};

//MAIN(Nucleo)

int main()
{
    SistemaLogin seguridad;
    Pantalla ui;

    bool accesoConcedido = false;
    int opcion;

    while (!accesoConcedido)
    {
        ui.dibujarMenuLogin();
        cin >> opcion;

        if (opcion == 1)
        {
            char inputUser[50];
            char inputPass[50];

            cout << "\n--- INICIO DE SESION ---\n";
            cout << "Usuario: ";
            cin >> inputUser;
            cout << "Contrasena: ";
            cin >> inputPass;

            if (seguridad.verificarLogin(inputUser, inputPass))
            {
                accesoConcedido = true;
                ui.mostrarMensaje("[!] Acceso concedido. Cargando modulos del almacen...");
            }
            else
            {
                ui.mostrarMensaje("[x] Credenciales incorrectas o usuario inexistente.");
            }
        }
        else if (opcion == 2)
        {
            char inputMaestra[50];

            cout << "\n--- AUTORIZACION REQUERIDA ---\n";
            cout << "Clave Maestra: ";
            cin >> inputMaestra;

            char nuevoUser[50];
            char nuevaPass[50];

            cout << "Nuevo Usuario: ";
            cin >> nuevoUser;
            cout << "Nueva Contrasena: ";
            cin >> nuevaPass;

            if (seguridad.registrarNuevo(inputMaestra, nuevoUser, nuevaPass))
            {
                ui.mostrarMensaje("[!] Nuevo empleado registrado con exito en la base de datos temporal.");
            }
            else
            {
                ui.mostrarMensaje("[x] REGISTRO FALLIDO: Clave maestra incorrecta o limite de usuarios.");
            }
        }
        else if (opcion == 3)
        {
            ui.mostrarMensaje("Apagando sistema de seguridad...");
            return 0; // Termina el programa completamente
        }
    }

//INGRESO AL ALMACEN

    if (accesoConcedido)
    {
        ui.limpiar();
        cout << "========================================\n";
        cout << "  SISTEMA DE ALMACEN PRINCIPAL ACTIVO   \n";
        cout << "========================================\n";
        cout << "Bienvenido. El menu de 9 opciones de gestion va aqui - EN CONSTRUCCION wad.\n\n";

        // Aqui se instanciara la clase Almacen y se llamara a su menu.
    }

    return 0;
}

