//**************************************************************************
// Práctica 3 
//**************************************************************************

#include <GL/glut.h>
#include <ctype.h>
#include <math.h>
#include <string>
#include <vector>
#include "objetos_B3.h"
using namespace std;

GLfloat hora = 0.0;
unsigned duracion_real_dia = 60; // Segundos
unsigned fotogramas_por_segundo = 60;
unsigned movimientos_por_hora_ingame = (duracion_real_dia/24.0) * fotogramas_por_segundo;
GLfloat duracion_ingame_movimiento = (duracion_real_dia/24.0) / movimientos_por_hora_ingame;
// tipos
typedef enum
{
    ESCENA_P3,
    NORMAL,
    CALIBRACION_CURVAS
} _modo_interfaz;

typedef enum
{
    PASO_TIEMPO_MANUAL,
    PASO_TIEMPO_AUTOMATICO, // Animación
    VIENTO
    //INTENSIDAD_VIENTO,
    //INTENSIDAD_LLUVIA
} _variable_seleccionada;
typedef enum
{
    PUNTO_1,
    PUNTO_2
} _punto_en_calibracion;
typedef enum
{
    CUBO,
    PIRAMIDE,
    OBJETO_PLY,
    ROTACION,
    CILINDRO,
    CONO,
    ESFERA, 
    EXTRUSION,

    // PRACTICA 3
    SUELO,
    SOL,
    HELICE,
    MOLINO,
    TALLO_GIRASOL,
    PETALO,
    CABEZA_GIRASOL,
    HOJA_GIRASOL,
    GIRASOL,
    VIENTO_OBJ,
    TEXTO,
    ESCENA_FINAL
} _tipo_objeto;

_punto_en_calibracion punto_en_calibracion=PUNTO_1;
_tipo_objeto    t_objeto=VIENTO_OBJ;
_modo           modo=SOLID;
_modo_interfaz  modo_interfaz=_modo_interfaz::ESCENA_P3;
_variable_seleccionada variable_seleccionada = PASO_TIEMPO_MANUAL;

// objetos
_cubo       cubo;
_piramide   piramide({0.5, 0.2, 2});
_objeto_ply ply; 
_rotacion   rotacion;
_cilindro   cilindro(1, 2, 100); 
_cono       cono(1, 2, 20, {2, 2, 0});
_esfera     esfera(0.5, 100, true, true, 50, 100);
_extrusion  *extrusion;


// PRACTICA 3 OBJETOS Y ATRIBUTOS
// Suelo
_suelo suelo({100, 0.25, 100});

// Sol
Coordenadas pos_inicial_sol ={0, 2, 0};
GLfloat tamanio_sol = 0.25;
_sol sol(tamanio_sol, pos_inicial_sol);

// Viento
_viento viento;

// Molino
_helice_molino helice_molino;
_molino molino;

//GIRASOL
_petalo_girasol petalo_girasol;
_cabeza_girasol cabeza_girasol;
_tallo_girasol tallo_girasol;
_hoja_girasol hoja_girasol;
_girasol girasol;

// ESCENA
_escena_P3 escena_p3;

// **************************************************************************
// variables que definen la posicion de la camara en coordenadas polares
GLfloat Observer_distance;
GLfloat Observer_angle_x;
GLfloat Observer_angle_y;

// variables que controlan la ventana y la transformacion de perspectiva
GLfloat Size_x, Size_y, Front_plane, Back_plane;

// variables que determninan la posicion y tamaño de la ventana X
int Window_x=50,    Window_y=50,    Window_width=800,   Window_high=800;


void clean_window()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

//**************************************************************************
// Funcion para definir la transformación de proyeccion
//***************************************************************************

void change_projection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // formato(x_minimo,x_maximo, y_minimo, y_maximo,plano_delantero, plano_traser)
    //  plano_delantero>0  plano_trasero>PlanoDelantero)
    glFrustum(-Size_x,Size_x,-Size_y,Size_y,Front_plane,Back_plane);
}

//**************************************************************************
// Funcion para definir la transformación*ply1 de vista (posicionar la camara)
//***************************************************************************

void change_observer()
{
    // posicion del observador
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0,0,-Observer_distance);
    glRotatef(Observer_angle_x, 1, 0, 0);
    glRotatef(Observer_angle_y, 0, 1, 0);
}

//**************************************************************************
// Funcion que dibuja los ejes utilizando la primitiva grafica de lineas
//***************************************************************************

void draw_axis()
{
    glDisable(GL_LIGHTING);
    glLineWidth(2);
    glBegin(GL_LINES);
        // eje X, color rojo
        glColor4f(1, 0, 0, 0.5);
        glVertex3f(-AXIS_SIZE, 0, 0);
        glVertex3f(AXIS_SIZE, 0, 0);

        // eje Y, color verde
        glColor4f(0, 1, 0, 0.5);
        glVertex3f(0, -AXIS_SIZE, 0);
        glVertex3f(0, AXIS_SIZE, 0);

        // eje Z, color azul
        glColor4f(0, 0, 1, 0.5);
        glVertex3f(0, 0, -AXIS_SIZE);
        glVertex3f(0, 0, AXIS_SIZE);
    glEnd();
}


//**************************************************************************
// Funcion que dibuja los objetos
//***************************************************************************

void draw_objects()
{

    switch (t_objeto)
    {
        case CUBO:
            cubo.draw(modo,{1.0f,0.0f,0.0f},5);
            break;
        case PIRAMIDE:
            piramide.draw(modo,{1.0f,0.0f,0.0f},5);
            break;
        case OBJETO_PLY:
            ply.draw(modo,{1.0f,0.6f,0.0f},3);
            break;
        case ROTACION:
            rotacion.draw(modo,{1.0f,0.0f,0.0f},5);
            break;
        case CILINDRO:
            cilindro.draw(modo,{1.0f,0.0f,0.0f},5);
            break;
        case CONO:
            cono.draw(modo,{1.0f,0.0f,0.0f},5);
            break;
        case ESFERA:
            esfera.draw(modo,{1.0f,0.0f,0.0f},5);
            break;
        case EXTRUSION:
            extrusion->draw(modo,{1.0f,0.0f,0.0f},5);
            break;

        // PRACTICA 3
        case SUELO:
            suelo.draw(modo);
            break;
        case SOL:
            sol.color_sol.cambiar_a_final();
            //sol.radio = 5;
            sol.draw(modo);
            break;
        case HELICE:
            helice_molino.draw(modo);
            break;
        case MOLINO:
            molino.draw(modo);
            break;
        case PETALO:
            petalo_girasol.draw(modo, petalo_girasol.color_petalo);
            break;
        case CABEZA_GIRASOL:
            cabeza_girasol.draw(modo);
            break;
        case TALLO_GIRASOL:
            tallo_girasol.draw(modo);
            break;
        case HOJA_GIRASOL:
        {
            glPushMatrix();
                glTranslatef(0, 0, 4);
                glRotatef(-90, 1, 0, 0);
                hoja_girasol.largo = 10;
                hoja_girasol.ancho = 0.2;
                hoja_girasol.color_hoja.set_original(viento.color_viento.actual);
                Onda onda;
                onda.amplitud = 0.4;
                hoja_girasol.punto_curva_1 = Coordenadas(0.68, 0.5, onda(escena_p3.hora));
                hoja_girasol.punto_curva_2 = Coordenadas(0.28, 2.2, -hoja_girasol.punto_curva_1.z);
                hoja_girasol.draw(modo);
            glPopMatrix();   
            break;
        }
        case GIRASOL:
            girasol.draw(modo);
            break;
        case VIENTO_OBJ:
            viento.draw(modo);
            break;
        case TEXTO:
            {
                glPushMatrix();
                //glScalef(3, 3, 3);
                dibujar_texto("Practica 3", {2, 2});
                glPopMatrix();
                break;
            }
        case ESCENA_FINAL:
            escena_p3.draw(modo);
            break;
	}

}
std::string to_string_with_precision(float value, int precision = 2) {
    std::ostringstream out;
    out << std::fixed << std::setprecision(precision) << value;
    return out.str();
}
void printear_info()
{
    glPushMatrix();
        char *modo_interfaz_str;
        switch (modo_interfaz)
        {
            case ESCENA_P3: modo_interfaz_str = "MODO INTERFAZ: [ESCENA P3]"; break;
            case NORMAL: modo_interfaz_str = "MODO INTERFAZ: [NORMAL]"; break;
            case CALIBRACION_CURVAS: modo_interfaz_str = "MODO INTERFAZ: [CALIBRACION CURVAS]"; break;
        }
        float entre_lineas = 0.2;
        dibujar_texto(modo_interfaz_str, {10, 5+entre_lineas*2}, {211u, 211, 211, 0.8});
        if (modo_interfaz == ESCENA_P3)
        {
            dibujar_texto("-----------------", {3, 5}, {211u, 211, 211, 0.8});
            dibujar_texto("INFORMACION SOBRE ESCENA", {3, 5-entre_lineas}, {211u, 211, 211, 0.8});
            dibujar_texto(("VARIABLE SELECCIONADA: " + to_string(variable_seleccionada)).c_str(), {3, 5-entre_lineas*2}, {211u, 211, 211, 0.8});
            dibujar_texto(("HORA: " + to_string_with_precision(escena_p3.hora)).c_str(), {3, 5-entre_lineas*3}, {211u, 211, 211, 0.8});
            dibujar_texto(("VELOCIDAD VIENTO: " + to_string_with_precision(escena_p3.viento.velocidad)).c_str(), {3, 5-entre_lineas*4}, {211u, 211, 211, 0.8});
            dibujar_texto("-----------------", {3, 5-entre_lineas*5}, {211u, 211, 211, 0.8});
        }

    glPopMatrix();
}

void draw(void)
{
    clean_window();
    change_observer();

    //GLfloat color_luz_ambiental[] = {0.1f, 0.1f, 0.1f, 1.0f};
    //GLfloat color_luz_difusa[] = {1.0f, 1.0f, 1.0f, 1.0f};
    //GLfloat posicion_luz[] = {0.0f, 10.0f, 10.0f, 1.0f}; // Coloca la luz por encima y al frente de la escena
    //glLightfv(GL_LIGHT0, GL_AMBIENT, color_luz_ambiental);
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, color_luz_difusa);
    //glLightfv(GL_LIGHT0, GL_POSITION, posicion_luz);
    //// Configuración del material básico
    //GLfloat material_difuso[] = {0.8f, 0.5f, 0.3f, 1.0f};
    //GLfloat material_especular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    //GLfloat material_brillo[] = {50.0f};
    //glMaterialfv(GL_FRONT, GL_DIFFUSE, material_difuso);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, material_especular);
    //glMaterialfv(GL_FRONT, GL_SHININESS, material_brillo);
    
    draw_axis();
    draw_objects();
    printear_info();
    glutSwapBuffers();
}

//***************************************************************************
// Funcion llamada cuando se produce un cambio en el tamaño de la ventana
//
// el evento manda a la funcion:
// nuevo ancho
// nuevo alto
//***************************************************************************

void change_window_size(int Ancho1, int Alto1)
{
    float Aspect_ratio;
    Aspect_ratio = (float) Alto1 / (float) Ancho1;
    Size_y = Size_x * Aspect_ratio;
    change_projection();
    glViewport(0, 0, Ancho1, Alto1);
    glutPostRedisplay();
}


//***************************************************************************
// Funcion llamada cuando se aprieta una tecla normal
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton
//***************************************************************************

void normal_key(unsigned char tecla_pulsada, int x, int y)
{
    if (tecla_pulsada == 13) // Enter
    {
        modo_interfaz = static_cast<_modo_interfaz>((modo_interfaz + 1) % 3);
        switch (modo_interfaz)
        {
            case ESCENA_P3: cerr << "Modo escena P3" << endl; t_objeto = ESCENA_FINAL; break;
            case NORMAL: cerr << "Modo normal" << endl; t_objeto = CUBO; break;
            case CALIBRACION_CURVAS: cerr << "Modo calibración curvas" << endl; t_objeto = HOJA_GIRASOL; break;
        }  
    }   
    if (tecla_pulsada == 32) // Espacio = animación
    {
        if(variable_seleccionada != PASO_TIEMPO_AUTOMATICO)
        {
            cerr << "MODO TIEMPO AUTOMATICO" << endl;
            variable_seleccionada = PASO_TIEMPO_AUTOMATICO;
        }
        else // Paro el tiempo
        {
            cerr << "MODO TIMPO MANUAL" << endl;
            variable_seleccionada = PASO_TIEMPO_MANUAL;
        }

        escena_p3.instante_previo = 0.0;
        escena_p3.paso_tiempo_automatico = variable_seleccionada == PASO_TIEMPO_AUTOMATICO;
    }   

    switch (toupper(tecla_pulsada))
    {
	case 'Q':exit(0);
	case '1':   modo = POINTS;          break;
	case '2':   modo = EDGES;           break;
	case '3':   modo = SOLID;           break;
	case '4':   modo = SOLID_COLORS;    break;

    case 'O':   t_objeto = OBJETO_PLY;  break;	
    case 'R':   t_objeto = ROTACION;    break;
    case 'P':   if(modo_interfaz==CALIBRACION_CURVAS)
                {
                    if (punto_en_calibracion == PUNTO_1)
                        punto_en_calibracion = PUNTO_2;
                    else
                        punto_en_calibracion = PUNTO_1;
                }
                else
                    t_objeto = PIRAMIDE;
                break;
    case 'C':   t_objeto = CUBO;        break;
    case 'L':   t_objeto = CILINDRO;    break;
    case 'N':   t_objeto = CONO;        break;
    case 'E':   t_objeto = ESFERA;      break;

    // PRÁCTICA 3
    case 'V':
        variable_seleccionada = VIENTO;
        break; 
    case 'W':
        switch (variable_seleccionada)
        {
            case PASO_TIEMPO_MANUAL:
                if (escena_p3.hora < 24)
                    escena_p3.hora += 1.0/escena_p3.movimientos_por_hora;
            break;
            case PASO_TIEMPO_AUTOMATICO:
                if (escena_p3.hora < 24)
                    escena_p3.hora += 1.0/escena_p3.movimientos_por_hora;
            break;
            case VIENTO:
                    escena_p3.viento.velocidad += 10.0;
            break;
        }
        break;
    case 'S':
        switch (variable_seleccionada)
        {
            case PASO_TIEMPO_MANUAL:
                if (escena_p3.hora)
                    escena_p3.hora -= 1.0/escena_p3.movimientos_por_hora;
            break;
            case PASO_TIEMPO_AUTOMATICO:
                if (escena_p3.hora)
                    escena_p3.hora -= 1.0/escena_p3.movimientos_por_hora;
            break;
            case VIENTO:
                    escena_p3.viento.velocidad -= 10.0;
            break;
        }
        break;
	}
    glutPostRedisplay();
}

//***************************************************************************
// Funcion llamada cuando se aprieta una tecla especial
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton

//***************************************************************************

void special_key(int tecla_pulsada, int x, int y)
{

    switch (tecla_pulsada)
    {
        case GLUT_KEY_LEFT:
            if (modo_interfaz == CALIBRACION_CURVAS)
            {
                if (punto_en_calibracion == PUNTO_1)
                    hoja_girasol.punto_curva_1.x -= 0.1;
                else
                    hoja_girasol.punto_curva_2.x -= 0.1;
            }
            else
                Observer_angle_y--;
            break;
        case GLUT_KEY_RIGHT:
            if (modo_interfaz == CALIBRACION_CURVAS)
            {
                if (punto_en_calibracion == PUNTO_1)
                    hoja_girasol.punto_curva_1.x += 0.1;
                else
                    hoja_girasol.punto_curva_2.x += 0.1;
            }
            else
                Observer_angle_y++;
            break;
        case GLUT_KEY_UP:
            if (modo_interfaz == CALIBRACION_CURVAS)
            {
                if (punto_en_calibracion == PUNTO_1)
                    hoja_girasol.punto_curva_1.y += 0.1;
                else
                    hoja_girasol.punto_curva_2.y += 0.1;
            }
            else
                Observer_angle_x--;
            break;
        case GLUT_KEY_DOWN:
            if (modo_interfaz == CALIBRACION_CURVAS)
            {
                if (punto_en_calibracion == PUNTO_1)
                    hoja_girasol.punto_curva_1.y -= 0.1;
                else
                    hoja_girasol.punto_curva_2.y -= 0.1;
            }
            else
                Observer_angle_x++;
            break;
        case GLUT_KEY_PAGE_UP:
            if (modo_interfaz == CALIBRACION_CURVAS)
            {
                if (punto_en_calibracion == PUNTO_1)
                    hoja_girasol.punto_curva_1.z += 0.1;
                else
                    hoja_girasol.punto_curva_2.z += 0.1;
            }
            else
                Observer_distance *= 1.2;
            break;
        case GLUT_KEY_PAGE_DOWN:
            if (modo_interfaz == CALIBRACION_CURVAS)
            {
                if (punto_en_calibracion == PUNTO_1)
                    hoja_girasol.punto_curva_1.z -= 0.1;
                else
                    hoja_girasol.punto_curva_2.z -= 0.1;
            }
            else
                Observer_distance /= 1.2;
            break;
    }

    //cerr << "Punto del observador: " << Observer_angle_x << " " << Observer_angle_y << " " << Observer_distance << endl;
    if (modo_interfaz == CALIBRACION_CURVAS)
    {
        cerr << "Punto 1: " << hoja_girasol.punto_curva_1.x << " " << hoja_girasol.punto_curva_1.y << " " << hoja_girasol.punto_curva_1.z << endl;
        cerr << "Punto 2: " << hoja_girasol.punto_curva_2.x << " " << hoja_girasol.punto_curva_2.y << " " << hoja_girasol.punto_curva_2.z << endl;
    }
    glutPostRedisplay();
}

//***************************************************************************
// Funcion de animación automática
//***************************************************************************


void animacion()
{
    glutPostRedisplay();
}

//***************************************************************************
// Funcion de incializacion
//***************************************************************************

void initialize(void)
{
    // se inicalizan la ventana y los planos de corte
    Size_x=0.5;
    Size_y=0.5;
    Front_plane=1;
    Back_plane=1000;

    // se incia la posicion del observador, en el eje z
    Observer_distance=7*Front_plane;
    Observer_angle_x=20;
    Observer_angle_y=22;

    // se indica el color para limpiar la ventana	(r,v,a,al)
    // blanco=(1,1,1,1) rojo=(1,0,0,1), ...);
    glClearColor(escena_p3.color_cielo.actual.r, escena_p3.color_cielo.actual.g, escena_p3.color_cielo.actual.b, 1);
    //glClearColor(0.0, 0.0, 0.0, 1.0);

    // se habilita el z-bufer
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    //glEnable(GL_NORMALIZE);
    //glShadeModel(GL_SMOOTH);

    change_projection();
    glViewport(0,0,Window_width,Window_high);
}


//***************************************************************************
// Programa principal
//
// Se encarga de iniciar la ventana, asignar las funciones e comenzar el
// bucle de eventos
//***************************************************************************


int main(int argc, char *argv[] )
{
    // se llama a la inicialización de glut
    glutInit(&argc, argv);

    // se indica las caracteristicas que se desean para la visualización con OpenGL
    // Las posibilidades son:
    // GLUT_SIMPLE -> memoria de imagen simple
    // GLUT_DOUBLE -> memoria de imagen doble
    // GLUT_INDEX -> memoria de imagen con color indizado
    // GLUT_RGB -> memoria de imagen con componentes rojo, verde y azul para cada pixel
    // GLUT_RGBA -> memoria de imagen con componentes rojo, verde, azul y alfa para cada pixel
    // GLUT_DEPTH -> memoria de profundidad o z-bufer
    // GLUT_STENCIL -> memoria de estarcido_rotation Rotation;
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    // posicion de la esquina inferior izquierdad de la ventana
    glutInitWindowPosition(Window_x,Window_y);

    // tamaño de la ventana (ancho y alto)
    glutInitWindowSize(Window_width,Window_high);

    // llamada para crear la ventana, indicando el titulo (no se visualiza hasta que se llama
    // al bucle de eventos)
    glutCreateWindow("PRACTICA - 3");

    // asignación de la funcion llamada "dibujar" al evento de dibujo
    glutDisplayFunc(draw);
    // asignación de la funcion llamada "change_window_size" al evento correspondiente
    glutReshapeFunc(change_window_size);
    // asignación de la funcion llamada "normal_key" al evento correspondiente
    glutKeyboardFunc(normal_key);
    // asignación de la funcion llamada "tecla_Especial" al evento correspondiente
    glutSpecialFunc(special_key);

    glutIdleFunc(animacion);

    // funcion de inicialización
    initialize();

    // creación del objeto ply
    ply.parametros(argv[1]);

    //ply = new _objeto_ply(argv[1]);

    // inicio del bucle de eventos
    glutMainLoop();
    return 0;
}
