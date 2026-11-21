#include <iostream>
#include <fstream>

using namespace std;

// ==========================================
// HERRAMIENTAS DE TEXTO Y FORMATO MANUAL
// ==========================================

int obtenerLongitud(const char* texto)
{
    int longitud = 0;
    while (texto[longitud] != '\0')
    {
        longitud++;
    }
    return longitud;
}

void copiarTexto(char*& destino, const char* origen)
{
    int longitud = obtenerLongitud(origen);
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

void imprimirTextoFijo(const char* texto, int ancho)
{
    int len = obtenerLongitud(texto);
    if (len > ancho)
    {
        for (int i = 0; i < ancho - 2; i++)
        {
            cout << texto[i];
        }
        cout << "..";
    }
    else
    {
        cout << texto;
        for (int i = len; i < ancho; i++)
        {
            cout << " ";
        }
    }
}

void imprimirMonto(double monto)
{
    int totalCentavos = (int)(monto * 100.0 + 0.5);
    int soles = totalCentavos / 100;
    int centavos = totalCentavos % 100;

    cout << "S/ " << soles << ".";
    if (centavos < 10)
    {
        cout << "0";
    }
    cout << centavos;
}

// ==========================================
// INTERFAZ GRAFICA CENTRADA (PANTALLA)
// ==========================================

class Pantalla
{
private:
    int anchoConsola;

    void imprimirEspacios(int cantidad) const
    {
        for (int i = 0; i < cantidad; i++)
        {
            cout << " ";
        }
    }

public:
    Pantalla(int _anchoConsola = 96)
    {
        anchoConsola = _anchoConsola;
    }

    void limpiar() const
    {
        cout << "\033[2J\033[1;1H";
    }

    void imprimirCentrado(const char* texto, const char* color = "\033[0m") const
    {
        int len = obtenerLongitud(texto);
        int margen = (anchoConsola - len) / 2;
        if (margen < 0)
        {
            margen = 0;
        }
        imprimirEspacios(margen);
        cout << color << texto << "\033[0m\n";
    }

    void imprimirBordeCaja() const
    {
        imprimirCentrado("+----------------------------------------------------------+", "\033[1;36m");
    }

    void imprimirFilaCaja(const char* texto, const char* colorTexto = "\033[0m") const
    {
        const int anchoCaja = 60;
        int margen = (anchoConsola - anchoCaja) / 2;
        if (margen < 0)
        {
            margen = 0;
        }

        int lenTexto = obtenerLongitud(texto);
        int relleno = anchoCaja - 4 - lenTexto;
        if (relleno < 0)
        {
            relleno = 0;
        }

        imprimirEspacios(margen);
        cout << "\033[1;36m| \033[0m" << colorTexto << texto << "\033[0m";
        imprimirEspacios(relleno);
        cout << "\033[1;36m |\033[0m\n";
    }

    void dibujarMenuLogin() const
    {
        limpiar();
        cout << "\n\n";
        imprimirBordeCaja();
        imprimirFilaCaja("          SISTEMA PHARMALOGIC - PRE-INGRESO", "\033[1;33m");
        imprimirBordeCaja();
        imprimirFilaCaja("1. Iniciar Sesion");
        imprimirFilaCaja("2. Registrar Nuevo Empleado");
        imprimirFilaCaja("3. Salir del Sistema", "\033[1;31m");
        imprimirBordeCaja();
    }

    void dibujarMenuPrincipal(const char* usuarioActivo, int totalItems, int alertasStock) const
    {
        limpiar();
        cout << "\n";
        imprimirBordeCaja();
        imprimirFilaCaja("        PHARMALOGIC ERP - CONTROL DE ALMACEN", "\033[1;33m");
        imprimirBordeCaja();

        int margen = (anchoConsola - 60) / 2;
        imprimirEspacios(margen);
        cout << "\033[1;36m| \033[0mOperador: \033[1;32m";
        imprimirTextoFijo(usuarioActivo, 12);
        cout << "\033[0m | Items: \033[1;33m" << totalItems
             << "\033[0m | Stock Bajo: \033[1;31m" << alertasStock << "\033[0m\n";

        imprimirBordeCaja();
        imprimirFilaCaja("[ INVENTARIO ]", "\033[1;36m");
        imprimirFilaCaja("  1. Ver inventario completo");
        imprimirFilaCaja("  2. Buscar producto por codigo");
        imprimirFilaCaja("  3. Ordenar inventario por stock (Menor a Mayor)");
        imprimirFilaCaja("[ GESTION CRUD ]", "\033[1;36m");
        imprimirFilaCaja("  4. Registrar nuevo Medicamento");
        imprimirFilaCaja("  5. Registrar nuevo Insumo Medico");
        imprimirFilaCaja("  6. Actualizar stock o precio de producto");
        imprimirFilaCaja("  7. Eliminar producto del almacen");
        imprimirFilaCaja("[ DESPACHO Y ARCHIVOS ]", "\033[1;36m");
        imprimirFilaCaja("  8. Generar Pedido de salida");
        imprimirFilaCaja("  9. Ver registros guardados (.txt)");
        imprimirFilaCaja("  0. Cerrar sesion", "\033[1;31m");
        imprimirBordeCaja();
    }

    void mostrarMensaje(const char* mensaje, const char* color = "\033[1;33m") const
    {
        cout << "\n";
        imprimirCentrado(mensaje, color);
        imprimirCentrado("Presiona Enter para continuar...", "\033[0;37m");
        cin.get();
    }

    int leerEntero(const char* prompt) const
    {
        int valor;
        int margen = (anchoConsola - 60) / 2;
        while (true)
        {
            imprimirEspacios(margen);
            cout << "\033[1;32m" << prompt << "\033[0m";
            cin >> valor;

            if (!cin.fail())
            {
                cin.ignore(1000, '\n');
                return valor;
            }
            cin.clear();
            cin.ignore(1000, '\n');
            imprimirCentrado("[x] Entrada invalida. Ingrese un numero entero.", "\033[1;31m");
        }
    }

    double leerDecimal(const char* prompt) const
    {
        double valor;
        int margen = (anchoConsola - 60) / 2;
        while (true)
        {
            imprimirEspacios(margen);
            cout << "\033[1;32m" << prompt << "\033[0m";
            cin >> valor;

            if (!cin.fail())
            {
                cin.ignore(1000, '\n');
                return valor;
            }
            cin.clear();
            cin.ignore(1000, '\n');
            imprimirCentrado("[x] Entrada invalida. Ingrese un monto numerico.", "\033[1;31m");
        }
    }

    void leerTexto(const char* prompt, char* buffer, int maxLen) const
    {
        int margen = (anchoConsola - 60) / 2;
        imprimirEspacios(margen);
        cout << "\033[1;32m" << prompt << "\033[0m";
        cin.getline(buffer, maxLen);

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
};

// ==========================================
// CAPA MODELO: USUARIOS
// ==========================================

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

    const char* getUsuario() const
    {
        return usuario;
    }

    bool validarCredenciales(const char* _userIntent, const char* _passIntent) const
    {
        return sonIguales(usuario, _userIntent) && sonIguales(password, _passIntent);
    }
};

// ==========================================
// PRODUCTOS DEL ALMACEN
// ==========================================

class Producto
{
private:
    char* codigo;
    char* nombre;
    double precio;
    int stock;

protected:
    void mostrarDatosBase() const
    {
        cout << "  ";
        imprimirTextoFijo(tipo(), 14);
        cout << " | ";
        imprimirTextoFijo(codigo, 8);
        cout << " | ";
        imprimirTextoFijo(nombre, 20);
        cout << " | ";
        imprimirMonto(precio);
        cout << " | Stock: ";
        if (stock < 10)
        {
            cout << "\033[1;31m" << stock << "\033[0m";
        }
        else
        {
            cout << "\033[1;32m" << stock << "\033[0m";
        }
    }

public:
    Producto(const char* _codigo, const char* _nombre, double _precio, int _stock)
    {
        copiarTexto(codigo, _codigo);
        copiarTexto(nombre, _nombre);
        precio = (_precio < 0) ? 0 : _precio;
        stock = (_stock < 0) ? 0 : _stock;
    }

    virtual ~Producto()
    {
        delete[] codigo;
        delete[] nombre;
    }

    Producto(const Producto&) = delete;
    Producto& operator=(const Producto&) = delete;

    const char* getCodigo() const { return codigo; }
    const char* getNombre() const { return nombre; }
    double getPrecio() const { return precio; }
    int getStock() const { return stock; }

    bool setNombre(const char* nuevo)
    {
        if (nuevo[0] == '\0')
        {
            return false;
        }
        delete[] nombre;
        copiarTexto(nombre, nuevo);
        return true;
    }

    bool setPrecio(double nuevo)
    {
        if (nuevo < 0)
        {
            return false;
        }
        precio = nuevo;
        return true;
    }

    bool agregarStock(int cantidad)
    {
        if (cantidad <= 0)
        {
            return false;
        }
        stock += cantidad;
        return true;
    }

    bool retirarStock(int cantidad)
    {
        if (cantidad <= 0 || cantidad > stock)
        {
            return false;
        }
        stock -= cantidad;
        return true;
    }

    bool operator==(const Producto& otro) const
    {
        return sonIguales(codigo, otro.codigo);
    }

    bool operator==(const char* otroCodigo) const
    {
        return sonIguales(codigo, otroCodigo);
    }

    virtual const char* tipo() const = 0;
    virtual void mostrar() const = 0;
};

class Medicamento : public Producto
{
private:
    char* fechaVencimiento;
    bool requiereReceta;

public:
    Medicamento(const char* _codigo, const char* _nombre, double _precio, int _stock,
                const char* _fechaVencimiento, bool _requiereReceta)
        : Producto(_codigo, _nombre, _precio, _stock)
    {
        copiarTexto(fechaVencimiento, _fechaVencimiento);
        requiereReceta = _requiereReceta;
    }

    ~Medicamento() override
    {
        delete[] fechaVencimiento;
    }

    const char* getFechaVencimiento() const { return fechaVencimiento; }
    bool getRequiereReceta() const { return requiereReceta; }
    void setRequiereReceta(bool valor) { requiereReceta = valor; }

    const char* tipo() const override
    {
        return "Medicamento";
    }

    void mostrar() const override
    {
        mostrarDatosBase();
        cout << " | Vence: " << fechaVencimiento
             << " | Receta: " << (requiereReceta ? "Si" : "No") << "\n";
    }
};

class InsumoMedico : public Producto
{
private:
    char* categoria;
    bool esteril;

public:
    InsumoMedico(const char* _codigo, const char* _nombre, double _precio, int _stock,
                 const char* _categoria, bool _esteril)
        : Producto(_codigo, _nombre, _precio, _stock)
    {
        copiarTexto(categoria, _categoria);
        esteril = _esteril;
    }

    ~InsumoMedico() override
    {
        delete[] categoria;
    }

    const char* getCategoria() const { return categoria; }
    bool esEsteril() const { return esteril; }

    const char* tipo() const override
    {
        return "Insumo Medico";
    }

    void mostrar() const override
    {
        mostrarDatosBase();
        cout << " | Cat: " << categoria
             << " | Esteril: " << (esteril ? "Si" : "No") << "\n";
    }
};

// ==========================================
// GESTOR DE ARCHIVOS
// ==========================================

class GestorArchivo
{
public:
    bool registrarUsuario(const char* usr, const char* pass)
    {
        ofstream archivo("Usuarios.txt", ios::app);
        if (!archivo.is_open())
        {
            cout << "Error, el archivo no se cargo correctamente\n";
            return false;
        }
        archivo << usr << "," << pass << "\n";
        archivo.close();
        return true;
    }

    bool registrarProducto(const Medicamento* med)
    {
        ofstream archivo("Medicamentos.txt", ios::app);
        if (!archivo.is_open())
        {
            cout << "Error, el archivo no se cargo correctamente\n";
            return false;
        }
        archivo << med->getCodigo() << ","
                << med->getNombre() << ","
                << med->getPrecio() << ","
                << med->getStock() << ","
                << med->getFechaVencimiento() << ","
                << med->getRequiereReceta() << "\n";
        archivo.close();
        return true;
    }

    bool registrarProducto(const InsumoMedico* insM)
    {
        ofstream archivo("Insumos_Medicos.txt", ios::app);
        if (!archivo.is_open())
        {
            cout << "Error, el archivo no se cargo correctamente\n";
            return false;
        }
        archivo << insM->getCodigo() << ","
                << insM->getNombre() << ","
                << insM->getPrecio() << ","
                << insM->getStock() << ","
                << insM->getCategoria() << ","
                << insM->esEsteril() << "\n";
        archivo.close();
        return true;
    }

    void mostrarArchivo(const char* nombreArchivo)
    {
        ifstream archivo(nombreArchivo);
        if (!archivo.is_open())
        {
            cout << "  [!] Archivo aun no creado o vacio: " << nombreArchivo << "\n";
            return;
        }
        char caracter;
        cout << "\n  --- CONTENIDO DE " << nombreArchivo << " ---\n  ";
        while (archivo.get(caracter))
        {
            cout << caracter;
            if (caracter == '\n')
            {
                cout << "  ";
            }
        }
        archivo.close();
        cout << "\n  ----------------------------------------\n";
    }
    //Eduardo lo q este comentado dentro de las clases es lo q falta para q completemos el proyecto pe
    //Implementar lectura de Usuarios.txt al iniciar el sistema
    //Implementar lectura de Medicamentos.txt e Insumos_Medicos.txt al iniciar
    //Implementar actualizacion/reescritura de archivos al editar o eliminar productos
};

// ==========================================
// COLECCIONES Y PEDIDOS
// ==========================================

class ColeccionProductos
{
private:
    Producto** inventario;
    int elemActuales;
    int cantidad;
    GestorArchivo gestor;

    void redimensionar()
    {
        int nuevaCantidad = cantidad * 2;
        Producto** nuevoInventario = new Producto*[nuevaCantidad];
        for (int i = 0; i < elemActuales; i++)
        {
            nuevoInventario[i] = inventario[i];
        }
        delete[] inventario;
        inventario = nuevoInventario;
        cantidad = nuevaCantidad;
    }

public:
    ColeccionProductos()
    {
        elemActuales = 0;
        cantidad = 5;
        inventario = new Producto*[cantidad];
    }

    ~ColeccionProductos()
    {
        for (int i = 0; i < elemActuales; i++)
        {
            delete inventario[i];
        }
        delete[] inventario;
    }

    // El parametro guardarEnArchivo te permite cargar desde .txt sin duplicar registros
    void agregarProducto(Medicamento* med, bool guardarEnArchivo = true)
    {
        if (elemActuales == cantidad)
        {
            redimensionar();
        }
        inventario[elemActuales] = med;
        elemActuales++;
        if (guardarEnArchivo)
        {
            gestor.registrarProducto(med);
        }
    }

    void agregarProducto(InsumoMedico* insM, bool guardarEnArchivo = true)
    {
        if (elemActuales == cantidad)
        {
            redimensionar();
        }
        inventario[elemActuales] = insM;
        elemActuales++;
        if (guardarEnArchivo)
        {
            gestor.registrarProducto(insM);
        }
    }

    bool eliminarProducto(const char* codigoBuscado)
    {
        int idx = buscarProducto(codigoBuscado);
        if (idx == -1)
        {
            return false;
        }

        delete inventario[idx];
        for (int i = idx; i < elemActuales - 1; i++)
        {
            inventario[i] = inventario[i + 1];
        }
        elemActuales--;
        return true;
    }

    int buscarProducto(const char* codigoBuscado) const
    {
        for (int i = 0; i < elemActuales; i++)
        {
            if (*inventario[i] == codigoBuscado)
            {
                return i;
            }
        }
        return -1;
    }

    void ordenarPorStock()
    {
        for (int i = 0; i < elemActuales - 1; i++)
        {
            for (int j = 0; j < elemActuales - i - 1; j++)
            {
                if (inventario[j]->getStock() > inventario[j + 1]->getStock())
                {
                    Producto* temp = inventario[j];
                    inventario[j] = inventario[j + 1];
                    inventario[j + 1] = temp;
                }
            }
        }
    }

    void mostrarInventario() const
    {
        if (elemActuales == 0)
        {
            cout << "\n  [!] El inventario esta vacio.\n";
            return;
        }

        cout << "\n  ====================================================================================================\n";
        for (int i = 0; i < elemActuales; i++)
        {
            inventario[i]->mostrar();
        }
        cout << "  ====================================================================================================\n";
    }

    int getElemActuales() const
    {
        return elemActuales;
    }

    int contarStockBajo(int umbral = 10) const
    {
        int alertas = 0;
        for (int i = 0; i < elemActuales; i++)
        {
            if (inventario[i]->getStock() < umbral)
            {
                alertas++;
            }
        }
        return alertas;
    }

    Producto* obtenerProducto(int indice) const
    {
        if (indice >= 0 && indice < elemActuales)
        {
            return inventario[indice];
        }
        return nullptr;
    }
};

class LineaDetalle
{
private:
    Producto* producto; // AGREGACION: apunta a un producto existente sin ser su dueno
    int cantidad;

public:
    LineaDetalle(Producto* _producto, int _cantidad)
    {
        producto = _producto;
        cantidad = (_cantidad <= 0) ? 1 : _cantidad;
    }

    double calcularSubtotal() const
    {
        return producto->getPrecio() * cantidad;
    }

    void mostrarLinea() const
    {
        cout << "  -> ";
        imprimirTextoFijo(producto->getNombre(), 22);
        cout << " | Cant: " << cantidad << " | Subtotal: ";
        imprimirMonto(calcularSubtotal());
        cout << "\n";
    }

    Producto* getProducto() const { return producto; }
    int getCantidad() const { return cantidad; }
};

class Pedido
{
private:
    char* codigoPedido;
    Empleado* responsable;      // ASOCIACION: empleado que atendio el pedido
    LineaDetalle** detalles;    // COMPOSICION: lineas creadas y destruidas por el pedido
    int numLineas;
    int capacidad;

    void redimensionar()
    {
        int nuevaCapacidad = capacidad * 2;
        LineaDetalle** nuevoArreglo = new LineaDetalle*[nuevaCapacidad];
        for (int i = 0; i < numLineas; i++)
        {
            nuevoArreglo[i] = detalles[i];
        }
        delete[] detalles;
        detalles = nuevoArreglo;
        capacidad = nuevaCapacidad;
    }

public:
    Pedido(const char* _codigoPedido, Empleado* _responsable)
    {
        copiarTexto(codigoPedido, _codigoPedido);
        responsable = _responsable;
        numLineas = 0;
        capacidad = 5;
        detalles = new LineaDetalle*[capacidad];
    }

    ~Pedido()
    {
        delete[] codigoPedido;
        for (int i = 0; i < numLineas; i++)
        {
            delete detalles[i];
        }
        delete[] detalles;
    }

    bool agregarLinea(Producto* producto, int cantidad)
    {
        if (!producto->retirarStock(cantidad))
        {
            return false;
        }
        if (numLineas == capacidad)
        {
            redimensionar();
        }
        detalles[numLineas] = new LineaDetalle(producto, cantidad);
        numLineas++;
        return true;
    }

    double calcularTotal() const
    {
        double total = 0;
        for (int i = 0; i < numLineas; i++)
        {
            total += detalles[i]->calcularSubtotal();
        }
        return total;
    }

    void mostrarPedido() const
    {
        cout << "\n  ================ COMPROBANTE DE PEDIDO: " << codigoPedido << " ================\n";
        if (responsable != nullptr)
        {
            cout << "  Atendido por: " << responsable->getUsuario() << "\n";
        }
        cout << "  ------------------------------------------------------------------\n";
        for (int i = 0; i < numLineas; i++)
        {
            detalles[i]->mostrarLinea();
        }
        cout << "  ------------------------------------------------------------------\n";
        cout << "  TOTAL DEL PEDIDO: ";
        imprimirMonto(calcularTotal());
        cout << "\n  ==================================================================\n";
    }
};

// ==========================================
// SISTEMA DE LOGIN Y SEGURIDAD
// ==========================================

class SistemaLogin
{
private:
    Empleado* nomina[10];
    int totalEmpleados;
    const char* CLAVE_MAESTRA = "ADMIN123";
    GestorArchivo gestor;

public:
    SistemaLogin()
    {
        totalEmpleados = 0;
        nomina[totalEmpleados] = new Empleado("admin", "1234");
        totalEmpleados++;
        //Llamar aqui a la carga de Usuarios.txt para poblar nomina[]
    }

    ~SistemaLogin()
    {
        for (int i = 0; i < totalEmpleados; i++)
        {
            delete nomina[i];
        }
    }

    Empleado* verificarLogin(const char* usr, const char* pass)
    {
        for (int i = 0; i < totalEmpleados; i++)
        {
            if (nomina[i]->validarCredenciales(usr, pass))
            {
                return nomina[i];
            }
        }
        return nullptr;
    }

    bool registrarNuevo(const char* intentoClave, const char* nUser, const char* nPass)
    {
        if (sonIguales(intentoClave, CLAVE_MAESTRA))
        {
            if (totalEmpleados < 10)
            {
                nomina[totalEmpleados] = new Empleado(nUser, nPass);
                totalEmpleados++;
                gestor.registrarUsuario(nUser, nPass);
                return true;
            }
        }
        return false;
    }
};

// ==========================================
// CONTROLADOR DEL MENU PRINCIPAL DEL ALMACEN
// ==========================================

class ControladorAlmacen
{
private:
    ColeccionProductos inventario;
    GestorArchivo verificadorArchivos;
    Pantalla* ui;
    Empleado* operadorActivo; // ASOCIACION con el Empleado autenticado

    void cargarDatosIniciales()
    {
        // Datos iniciales en RAM para probar sin duplicar .txt en cada ejecucion
        // Reemplazar o complementar con la lectura de Medicamentos.txt e Insumos_Medicos.txt
        inventario.agregarProducto(new Medicamento("MED001", "Paracetamol 500mg", 2.50, 100, "2027-05-20", false), false);
        inventario.agregarProducto(new Medicamento("MED002", "Amoxicilina 500mg", 14.80, 8, "2027-03-10", true), false);
        inventario.agregarProducto(new InsumoMedico("INS001", "Guantes Nitrilo", 15.00, 50, "Proteccion", true), false);
    }

    void opcionBuscarProducto()
    {
        char codigo[20];
        ui->limpiar();
        cout << "\n";
        ui->imprimirCentrado("=== BUSCAR PRODUCTO POR CODIGO ===", "\033[1;36m");
        ui->leerTexto("Ingrese el codigo del producto: ", codigo, 20);

        int idx = inventario.buscarProducto(codigo);
        if (idx != -1)
        {
            cout << "\n";
            inventario.obtenerProducto(idx)->mostrar();
            ui->mostrarMensaje("[!] Producto encontrado.", "\033[1;32m");
        }
        else
        {
            ui->mostrarMensaje("[x] No existe ningun producto con ese codigo.", "\033[1;31m");
        }
    }

    void opcionRegistrarMedicamento()
    {
        char codigo[20];
        char nombre[50];
        char fecha[20];

        ui->limpiar();
        cout << "\n";
        ui->imprimirCentrado("=== REGISTRAR NUEVO MEDICAMENTO ===", "\033[1;36m");

        ui->leerTexto("Codigo (ej. MED003): ", codigo, 20);
        if (inventario.buscarProducto(codigo) != -1)
        {
            ui->mostrarMensaje("[x] Ya existe un producto registrado con ese codigo.", "\033[1;31m");
            return;
        }

        ui->leerTexto("Nombre del medicamento: ", nombre, 50);
        double precio = ui->leerDecimal("Precio unitario (S/): ");
        int stock = ui->leerEntero("Cantidad en stock inicial: ");
        ui->leerTexto("Fecha de vencimiento (AAAA-MM-DD): ", fecha, 20);
        int recetaInt = ui->leerEntero("Requiere receta medica? (1 = Si, 0 = No): ");

        Medicamento* nuevo = new Medicamento(codigo, nombre, precio, stock, fecha, recetaInt == 1);
        inventario.agregarProducto(nuevo, true);
        ui->mostrarMensaje("[!] Medicamento registrado en memoria y en Medicamentos.txt.", "\033[1;32m");
    }

    void opcionRegistrarInsumo()
    {
        char codigo[20];
        char nombre[50];
        char categoria[30];

        ui->limpiar();
        cout << "\n";
        ui->imprimirCentrado("=== REGISTRAR NUEVO INSUMO MEDICO ===", "\033[1;36m");

        ui->leerTexto("Codigo (ej. INS002): ", codigo, 20);
        if (inventario.buscarProducto(codigo) != -1)
        {
            ui->mostrarMensaje("[x] Ya existe un producto registrado con ese codigo.", "\033[1;31m");
            return;
        }

        ui->leerTexto("Nombre del insumo: ", nombre, 50);
        double precio = ui->leerDecimal("Precio unitario (S/): ");
        int stock = ui->leerEntero("Cantidad en stock inicial: ");
        ui->leerTexto("Categoria (Curacion / Proteccion / Inyectables): ", categoria, 30);
        int esterilInt = ui->leerEntero("Es material esteril? (1 = Si, 0 = No): ");

        InsumoMedico* nuevo = new InsumoMedico(codigo, nombre, precio, stock, categoria, esterilInt == 1);
        inventario.agregarProducto(nuevo, true);
        ui->mostrarMensaje("[!] Insumo registrado en memoria y en Insumos_Medicos.txt.", "\033[1;32m");
    }

    void opcionActualizarProducto()
    {
        char codigo[20];
        ui->limpiar();
        cout << "\n";
        ui->imprimirCentrado("=== ACTUALIZAR STOCK O PRECIO ===", "\033[1;36m");
        ui->leerTexto("Codigo del producto a modificar: ", codigo, 20);

        int idx = inventario.buscarProducto(codigo);
        if (idx == -1)
        {
            ui->mostrarMensaje("[x] Producto no encontrado.", "\033[1;31m");
            return;
        }

        Producto* p = inventario.obtenerProducto(idx);
        cout << "\n";
        p->mostrar();
        cout << "\n";

        int subOpcion = ui->leerEntero("1. Agregar Stock | 2. Cambiar Precio: ");
        if (subOpcion == 1)
        {
            int cantidad = ui->leerEntero("Unidades a ingresar al almacen: ");
            if (p->agregarStock(cantidad))
            {
                //Actualizar archivo tras modificar stock
                ui->mostrarMensaje("[!] Stock actualizado correctamente.", "\033[1;32m");
            }
            else
            {
                ui->mostrarMensaje("[x] La cantidad debe ser mayor a 0.", "\033[1;31m");
            }
        }
        else if (subOpcion == 2)
        {
            double nuevoPrecio = ui->leerDecimal("Nuevo precio unitario (S/): ");
            if (p->setPrecio(nuevoPrecio))
            {
                //Actualizar archivo tras modificar precio
                ui->mostrarMensaje("[!] Precio actualizado correctamente.", "\033[1;32m");
            }
            else
            {
                ui->mostrarMensaje("[x] El precio no puede ser negativo.", "\033[1;31m");
            }
        }
        else
        {
            ui->mostrarMensaje("[x] Sub-opcion no valida.", "\033[1;31m");
        }
    }

    void opcionEliminarProducto()
    {
        char codigo[20];
        ui->limpiar();
        cout << "\n";
        ui->imprimirCentrado("=== ELIMINAR PRODUCTO DEL ALMACEN ===", "\033[1;31m");
        ui->leerTexto("Ingrese el codigo del producto a eliminar: ", codigo, 20);

        if (inventario.eliminarProducto(codigo))
        {
            //Actualizar archivo tras eliminar producto
            ui->mostrarMensaje("[!] Producto eliminado y memoria liberada.", "\033[1;32m");
        }
        else
        {
            ui->mostrarMensaje("[x] No se encontro ningun producto con ese codigo.", "\033[1;31m");
        }
    }

    void opcionGenerarPedido()
    {
        char codPedido[20];
        ui->limpiar();
        cout << "\n";
        ui->imprimirCentrado("=== GENERAR PEDIDO DE SALIDA ===", "\033[1;36m");
        ui->leerTexto("Codigo del pedido (ej. PED-01): ", codPedido, 20);

        Pedido* pedido = new Pedido(codPedido, operadorActivo);
        int continuar = 1;

        while (continuar == 1)
        {
            char codProd[20];
            ui->leerTexto("Codigo del producto a despachar: ", codProd, 20);

            int idx = inventario.buscarProducto(codProd);
            if (idx == -1)
            {
                ui->imprimirCentrado("[x] Producto no encontrado en inventario.", "\033[1;31m");
            }
            else
            {
                Producto* prod = inventario.obtenerProducto(idx);
                int cant = ui->leerEntero("Cantidad solicitada: ");

                if (pedido->agregarLinea(prod, cant))
                {
                    ui->imprimirCentrado("[!] Producto agregado al pedido y stock descontado.", "\033[1;32m");
                }
                else
                {
                    ui->imprimirCentrado("[x] Stock insuficiente o cantidad invalida.", "\033[1;31m");
                }
            }
            continuar = ui->leerEntero("Agregar otro producto al pedido? (1 = Si, 0 = Finalizar): ");
        }

        pedido->mostrarPedido();
        delete pedido; // Composicion: destruye las LineaDetalle, mantiene intactos los Productos
        ui->mostrarMensaje("[!] Pedido finalizado y memoria temporal de detalle liberada.", "\033[1;33m");
    }

    void opcionVerArchivos()
    {
        ui->limpiar();
        cout << "\n";
        ui->imprimirCentrado("=== REGISTROS PERSISTENTES EN DISCO ===", "\033[1;36m");
        verificadorArchivos.mostrarArchivo("Usuarios.txt");
        verificadorArchivos.mostrarArchivo("Medicamentos.txt");
        verificadorArchivos.mostrarArchivo("Insumos_Medicos.txt");
        ui->mostrarMensaje("Fin de lectura de archivos.");
    }

public:
    ControladorAlmacen(Pantalla* _ui, Empleado* _operadorActivo)
    {
        ui = _ui;
        operadorActivo = _operadorActivo;
        cargarDatosIniciales();
    }

    void ejecutar()
    {
        int opcion = -1;
        while (opcion != 0)
        {
            ui->dibujarMenuPrincipal(
                operadorActivo->getUsuario(),
                inventario.getElemActuales(),
                inventario.contarStockBajo(10)
            );

            opcion = ui->leerEntero("> Elige una opcion: ");

            switch (opcion)
            {
                case 1:
                    ui->limpiar();
                    cout << "\n";
                    ui->imprimirCentrado("=== INVENTARIO ACTUAL DEL ALMACEN ===", "\033[1;36m");
                    inventario.mostrarInventario();
                    ui->mostrarMensaje("Fin del listado.");
                    break;
                case 2:
                    opcionBuscarProducto();
                    break;
                case 3:
                    inventario.ordenarPorStock();
                    ui->mostrarMensaje("[!] Inventario ordenado por stock ascendente.", "\033[1;32m");
                    break;
                case 4:
                    opcionRegistrarMedicamento();
                    break;
                case 5:
                    opcionRegistrarInsumo();
                    break;
                case 6:
                    opcionActualizarProducto();
                    break;
                case 7:
                    opcionEliminarProducto();
                    break;
                case 8:
                    opcionGenerarPedido();
                    break;
                case 9:
                    opcionVerArchivos();
                    break;
                case 0:
                    ui->mostrarMensaje("Cerrando sesion del almacen...", "\033[1;33m");
                    break;
                default:
                    ui->mostrarMensaje("[x] Opcion fuera de rango (0 - 9).", "\033[1;31m");
                    break;
            }
        }
    }
};

// ==========================================
// MAIN (NUCLEO)
// ==========================================

int main()
{
    SistemaLogin seguridad;
    Pantalla ui(96);

    Empleado* usuarioAutenticado = nullptr;
    int opcion = 0;

    while (usuarioAutenticado == nullptr)
    {
        ui.dibujarMenuLogin();
        opcion = ui.leerEntero("> Elige una opcion: ");

        if (opcion == 1)
        {
            char inputUser[50];
            char inputPass[50];

            cout << "\n";
            ui.imprimirCentrado("--- INICIO DE SESION ---", "\033[1;36m");
            ui.leerTexto("Usuario: ", inputUser, 50);
            ui.leerTexto("Contrasena: ", inputPass, 50);

            usuarioAutenticado = seguridad.verificarLogin(inputUser, inputPass);
            if (usuarioAutenticado != nullptr)
            {
                ui.mostrarMensaje("[!] Acceso concedido. Cargando modulos del almacen...", "\033[1;32m");
            }
            else
            {
                ui.mostrarMensaje("[x] Credenciales incorrectas o usuario inexistente.", "\033[1;31m");
            }
        }
        else if (opcion == 2)
        {
            char inputMaestra[50];
            char nuevoUser[50];
            char nuevaPass[50];

            cout << "\n";
            ui.imprimirCentrado("--- AUTORIZACION REQUERIDA ---", "\033[1;33m");
            ui.leerTexto("Clave Maestra: ", inputMaestra, 50);
            ui.leerTexto("Nuevo Usuario: ", nuevoUser, 50);
            ui.leerTexto("Nueva Contrasena: ", nuevaPass, 50);

            if (seguridad.registrarNuevo(inputMaestra, nuevoUser, nuevaPass))
            {
                ui.mostrarMensaje("[!] Nuevo empleado registrado con exito.", "\033[1;32m");
            }
            else
            {
                ui.mostrarMensaje("[x] REGISTRO FALLIDO: Clave maestra incorrecta o limite alcanzado.", "\033[1;31m");
            }
        }
        else if (opcion == 3)
        {
            ui.mostrarMensaje("Apagando sistema de seguridad...", "\033[1;33m");
            return 0;
        }
        else
        {
            ui.mostrarMensaje("[x] Opcion invalida. Elija 1, 2 o 3.", "\033[1;31m");
        }
    }

    ControladorAlmacen almacen(&ui, usuarioAutenticado);
    almacen.ejecutar();

    return 0;
}
