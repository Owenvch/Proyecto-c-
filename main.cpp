#include <iostream>
#include <fstream>

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

//Productos del Almacen

// Clase base abstracta de todo lo que se guarda en el almacen
class Producto
{
private:
    char* codigo;
    char* nombre;
    double precio;
    int stock;

protected:
    // Imprime los datos comunes; las clases hijas la usan dentro de mostrar()
    void mostrarDatosBase() const
    {
        cout << "[" << tipo() << "] " << codigo
             << " | " << nombre
             << " | S/ " << precio
             << " | Stock: " << stock;
    }

public:
    // Un precio o stock negativo se reemplaza por 0
    Producto(const char* _codigo, const char* _nombre, double _precio, int _stock)
    {
        copiarTexto(codigo, _codigo);
        copiarTexto(nombre, _nombre);
        precio = (_precio < 0) ? 0 : _precio;
        stock = (_stock < 0) ? 0 : _stock;
    }

    // Virtual para que al hacer delete desde un Producto* se libere tambien la clase hija
    virtual ~Producto()
    {
        delete[] codigo;
        delete[] nombre;
    }

    // Se prohibe copiar: cada producto es dueno de su memoria y se maneja por puntero
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

    // Dos productos son el mismo si tienen el mismo codigo
    bool operator==(const Producto& otro) const
    {
        return sonIguales(codigo, otro.codigo);
    }

    // Permite comparar directo contra un codigo: *producto == "MED001"
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
    // Texto con formato AAAA-MM-DD
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
    // Ejemplos: "Curacion", "Proteccion", "Inyectables"
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
        cout << " | Categoria: " << categoria
             << " | Esteril: " << (esteril ? "Si" : "No") << "\n";
    }
};

class ColeccionProductos{
    private:
    Producto** inventario;
    int elemActuales;
    int cantidad;
    void redimensionar(){
        int nuevaCantidad=cantidad*2;
        Producto** nuevoInventario=new Producto*[nuevaCantidad];
        for(int i=0;i<elemActuales;i++){
            nuevoInventario[i]=inventario[i];
        }
        delete[] inventario;
        inventario=nuevoInventario;
        cantidad=nuevaCantidad;  
    }
    public:
    ColeccionProductos(){
        elemActuales=0;
        cantidad=5;
        inventario=new Producto*[cantidad];
    }
    void agregarProducto(Producto* nuevoProd){
        if(elemActuales==cantidad){
            redimensionar();
        }
        inventario[elemActuales]=nuevoProd;
        elemActuales++;
    }
    ~ColeccionProductos(){
        delete[] inventario;
    }
    int buscarProducto(const char* loteBuscado){
        for(int i=0;i<elemActuales;i++){
            if(sonIguales(inventario[i]->getCodigo(), loteBuscado)){
                return i;
            }
        }
        return -1; //producto no encontrado
    }
    void ordenarPorStock(){
        for(int i=0;i<elemActuales-1;i++){
            for(int j=0;j<elemActuales-i-1;j++){
                if(inventario[j]->getStock()>inventario[j+1]->getStock()){
                    Producto* temp=inventario[j];
                    inventario[j]=inventario[j+1];
                    inventario[j+1]=temp;
                }
            }
        }
    }
    void mostrarInventario(){
        for(int i=0;i<elemActuales;i++){
            inventario[i]->mostrar();
        }
    }
    int getElemActuales() {
        return elemActuales;
    }
    Producto* obtenerProducto(int indice){
        if(indice>=0 && indice<elemActuales){
            return inventario[indice];
        }
        return nullptr; //indice fuera de rango.
    }
};
class LineaDetalle{
    private:
    Producto* producto;
    int cantidad;
    public:
    LineaDetalle(Producto* _producto, int _cantidad){
        producto=_producto;
        cantidad=_cantidad;
        if(cantidad<=0){
            cantidad=1; //cantidad minima de 1
        }
    }
    double calcularSubtotal(){
        return producto->getPrecio()*cantidad;
    }
    void mostrarLinea(){
        cout<<"Producto: "<<producto->getNombre()<<" | Cantidad: "<<cantidad<<" | Subtotal: S/ "<<calcularSubtotal()<<endl;
    }
    Producto* getProducto(){
        return producto;
    }
    int getCantidad(){
        return cantidad;
    }
};
class Pedido{
    private:
    char* codigoPedido;
    LineaDetalle** detalles;
    int numLineas;
    int capacidad;
    void redimensionar(){
        int nuevaCapacidad=capacidad*2;
        LineaDetalle** nuevoArreglo=new LineaDetalle*[nuevaCapacidad];
        for(int i=0;i<numLineas;i++){
            nuevoArreglo[i]=detalles[i];
        }
        delete[] detalles;
        detalles=nuevoArreglo;
        capacidad=nuevaCapacidad;
    }
    public:
    Pedido(const char* _codigoPedido){
        copiarTexto(codigoPedido, _codigoPedido);
        numLineas=0;
        capacidad=5;
        detalles=new LineaDetalle*[capacidad];
    }
    ~Pedido(){
        delete[] codigoPedido;
        for(int i=0;i<numLineas;i++){
            delete detalles[i];
        }
        delete[] detalles;
    }
    bool agregarLinea(Producto* producto, int cantidad){
        if(!producto->retirarStock(cantidad)){
            cout<<"Error: No hay suficiente stock para el producto "<<producto->getNombre()<<endl;
            return false;
        }
        if(numLineas==capacidad){
            redimensionar();
        }
        detalles[numLineas]=new LineaDetalle(producto, cantidad);
        numLineas++;
        return true;
    }
    double calcularTotal(){
        double total=0;
        for(int i=0;i<numLineas;i++){
            total=total+detalles[i]->calcularSubtotal();
        }
        return total;
    }
    void mostrarPedido(){
        cout<<"Codigo de Pedido: "<<codigoPedido<<endl;
        cout<<"Detalles del Pedido:"<<endl;
        for(int i=0;i<numLineas;i++){
            detalles[i]->mostrarLinea();
        }
        cout<<"Total del pedido: S/ "<<calcularTotal()<<endl;
    }
};
//Clase Gestora de Archivos
class GestorArchivo{
public:
    bool registrarUsuario(const char* usr, const char* pass){
        ofstream archivo("Usuarios.txt",ios::app);
        if (!archivo.is_open()){
            cout<<"Error, el archivo no se cargó correctamente"<<endl;
            return false;
        }
        archivo<<usr<<","<<pass<<endl;
        archivo.close();
        return archivo.good();
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
    GestorArchivo gestor;

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
                gestor.registrarUsuario(nUser,nPass);
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
                // COMPROBACION DEL ARCHIVO txt
                ifstream archivo("Usuarios.txt");
                char caracter;
                cout <<endl<<"--- CONTENIDO DE Usuarios.txt ---"<<endl;
                while (archivo.get(caracter)){
                    cout << caracter;
                }
                archivo.close();
                cout<<endl<<"---------------------------------"<<endl;
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

