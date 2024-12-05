//**************************************************************************
// Práctica 3 
//**************************************************************************

#include <GL/glut.h>
#include <ctype.h>
#include <math.h>
#include <string>
#include <vector>
#include "objetos_final.hh"
using namespace std;

// Para las texturas
#include "CImg.h"
using namespace cimg_library;


GLfloat hora = 0.0;
unsigned duracion_real_dia = 30; // Segundos
unsigned fotogramas_por_segundo = 60;
unsigned movimientos_por_hora_ingame = (duracion_real_dia/24.0) * fotogramas_por_segundo;
GLfloat duracion_ingame_movimiento = (duracion_real_dia/24.0) / movimientos_por_hora_ingame;
// tipos
typedef enum
{
    ESCENA_P3,
    DEFAULT,
    CALIBRACION_CURVAS
} _modo_interfaz;

typedef enum
{
    HORA,
    VIENTO,
    GIRO_MOLINO,
    GIRASOL,
    TAMANIO_NUBE,
    LLUVIA
    //OSCILACION_PRADERA
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
    CUBO_TEX,

    // PRACTICA 3
    SUELO,
    SOL,
    HELICE,
    MOLINO,
    TALLO_GIRASOL,
    PETALO,
    CABEZA_GIRASOL,
    HOJA_GIRASOL,
    GIRASOL_OBJ,
    VIENTO_OBJ,
    NUBE,
    TEXTO,
    ESCENA_FINAL
} _tipo_objeto;

bool paso_tiempo_automatico = false;
bool mostrar_controles = false;
Coordenadas ultima_posicion = {0, 0, 0};
GLfloat ultimo_angulo_x = 0;
GLfloat ultimo_angulo_y = 0;
int ultima_pos_raton_x = 0;
int ultima_pos_raton_y = 0;
bool boton_rotacion_pulsado = false;
bool boton_movimiento_pulsado = false;
_punto_en_calibracion punto_en_calibracion=PUNTO_1;
_tipo_objeto    t_objeto=CUBO_TEX;
_modo           modo=SOLID;
_modo_interfaz  modo_interfaz=_modo_interfaz::DEFAULT;
_variable_seleccionada variable_seleccionada = HORA;
int id_tex;

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

// Nube
_nube nube;

// Molino
_helice_molino helice_molino;
_molino molino;

//GIRASOL
_petalo_girasol petalo_girasol;
_cabeza_girasol cabeza_girasol;
_tallo_girasol tallo_girasol;
_hoja_girasol hoja_girasol;
_girasol girasol;
_cubo_tex cubo_tex;
// ESCENA
_escena_P3 escena_p3;

// **************************************************************************
// variables que definen la posicion de la camara en coordenadas polares
Coordenadas Observer_position = {0.0, 0.0, 0.0};
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
    glTranslatef(Observer_position.x,Observer_position.y,-Observer_position.z);
    glRotatef(Observer_angle_x, 1, 0, 0);
    glRotatef(Observer_angle_y, 0, 1, 0);
}

//**************************************************************************
// Funcion que dibuja los ejes utilizando la primitiva grafica de lineas
//***************************************************************************

void draw_axis()
{
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
        case CUBO_TEX:
            {
                glPushMatrix();
                glScalef(50, 50, 50);
                cubo_tex.draw_solido_textura(id_tex);
                glPopMatrix();
                //molino.draw(modo);
            }
            break;
        // PRACTICA 3
        case SUELO:
            suelo.draw(modo);
            break;
        case SOL:
            //sol.color_sol.cambiar_a_final();
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
        case GIRASOL_OBJ:
            girasol.draw(modo);
            break;
        case VIENTO_OBJ:
            viento.draw(modo);
            break;
        case NUBE:
            nube.draw(modo);
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
        {
            glPushMatrix();
            glScalef(50, 50, 50);
            cubo_tex.draw_solido_textura(id_tex);
            glPopMatrix();
            escena_p3.draw(modo);
            break;
        }
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
            case ESCENA_P3: modo_interfaz_str = "[ESCENA P3]"; break;
            case DEFAULT: modo_interfaz_str = "[DEFAULT]"; break;
            case CALIBRACION_CURVAS: modo_interfaz_str = "[CALIBRACION CURVAS]"; break;
        }
        float entre_lineas = 0.25;
        unsigned linea = 0;
        Coordenadas pos_texto = {1.5, 8, entre_lineas};
        Color color_texto = {211u, 211, 211, 0.8};
        if (modo_interfaz != ESCENA_P3)
            dibujar_texto(modo_interfaz_str, pos_texto.ajustar_texto(linea), color_texto, 2);
        if (modo_interfaz == ESCENA_P3)
        {
            char *variable_seleccionada_str;
            //char *modo_tiempo_str;
            switch (variable_seleccionada)
            {
                case HORA: variable_seleccionada_str = "    SELECCIONADO: [HORA]"; break;
                case VIENTO: variable_seleccionada_str = "    SELECCIONADO: [VIENTO]"; break;
                case GIRO_MOLINO: variable_seleccionada_str = "    SELECCIONADO: [MOLINO]"; break;
                //case OSCILACION_PRADERA: variable_seleccionada_str = "    SELECCIONADO: [PRADERA]"; break;
                case TAMANIO_NUBE: variable_seleccionada_str = "    SELECCIONADO: [NUBE]"; break;
                case GIRASOL: variable_seleccionada_str = "    SELECCIONADO: [GIRASOL]"; break;
                case LLUVIA: variable_seleccionada_str = "    SELECCIONADO: [LLUVIA]"; break;
            }
            linea++;
            dibujar_texto("-----ESCENA------", pos_texto.ajustar_texto(++linea), color_texto, 2);
            dibujar_texto(variable_seleccionada_str, pos_texto.ajustar_texto(++linea), color_texto);
            dibujar_texto(("    HORA: " + to_string_with_precision(escena_p3.hora)).c_str(), pos_texto.ajustar_texto(++linea), color_texto);
            dibujar_texto(("    VELOCIDAD VIENTO: " + to_string_with_precision(escena_p3.viento.velocidad) + " km/h").c_str(), pos_texto.ajustar_texto(++linea), color_texto);
            dibujar_texto(("    FRECUENCIA OSCILACIONES: " + to_string_with_precision(escena_p3.viento.onda.frecuencia, 3) + " Hz").c_str(), pos_texto.ajustar_texto(++linea), color_texto);
            dibujar_texto(("    AMPLITUD OSCILACIONES: " + to_string_with_precision(escena_p3.viento.onda.amplitud, 1) + " m").c_str(), pos_texto.ajustar_texto(++linea), color_texto);
            dibujar_texto(("    ANGULO MOLINO: " + to_string_with_precision(escena_p3.molino.angulo_helice) + " grados").c_str(), pos_texto.ajustar_texto(++linea), color_texto);
            dibujar_texto(("    TAMANIO NUBE: x-" + to_string_with_precision(escena_p3.nube.tam.x, 1) + " y-" + to_string_with_precision(escena_p3.nube.tam.y, 1) + " z-" + to_string_with_precision(escena_p3.nube.tam.x, 1)).c_str(), pos_texto.ajustar_texto(++linea), color_texto);
            dibujar_texto(("    DENSIDAD NUBE: " + to_string_with_precision(escena_p3.nube.densidad*escena_p3.nube.densidad*escena_p3.nube.densidad*0.1, 0) + " p/m3").c_str(), pos_texto.ajustar_texto(++linea), color_texto);
            dibujar_texto(("    INTENSIDAD LLUVIA: " + to_string_with_precision(escena_p3.nube.intensidad_lluvia*10, 1)).c_str(), pos_texto.ajustar_texto(++linea), color_texto);
            dibujar_texto("------------------", pos_texto.ajustar_texto(++linea), color_texto, 2);
        }

    glPopMatrix();
}
void draw_controles()
{
    Color color_fondo(34u, 39, 46);
    glClearColor(color_fondo.actual.r, color_fondo.actual.g, color_fondo.actual.b, 1);
    float entre_lineas = 0.5;
    unsigned linea = 0;
    Coordenadas pos_texto = {-7, 7, entre_lineas};
    Color color_texto = {173u, 186, 199};
    Color color_texto_titulos = {150u, 208, 255};
    Color color_texto_rojo = {244u, 112, 103};
    unsigned char fuente = 2;
    unsigned char fuente_titulos = 4;
    dibujar_texto("----------------- CONTROLES -----------------", pos_texto.ajustar_texto(linea), color_texto_titulos, fuente_titulos);
    dibujar_texto("---Controles Generales---", pos_texto.ajustar_texto(++linea), color_texto_titulos, fuente_titulos);
    dibujar_texto("[ESC]   --> Mostrar/Ocultar controles", pos_texto.ajustar_texto(++linea), color_texto, fuente);
    dibujar_texto("[ENTER] --> Alternar interfaz Default/Calibracion_curvas/Escena", pos_texto.ajustar_texto(++linea), color_texto, fuente);
    ++linea;
    dibujar_texto("---Controles de Camara--- (USAR PREFERIBLEMENTE, MUY COMODO)", pos_texto.ajustar_texto(++linea), color_texto_titulos, fuente_titulos);
    dibujar_texto("CLICK IZQUIERDO + RATON --> Movimiento camara", pos_texto.ajustar_texto(++linea), color_texto, fuente);
    dibujar_texto("CLICK DERECHO   + RATON --> Rotar camara", pos_texto.ajustar_texto(++linea), color_texto, fuente);
    dibujar_texto("RUEDA RATON             --> Alejar/Acercar camara", pos_texto.ajustar_texto(++linea), color_texto, fuente);
    dibujar_texto("---Controles Alternativos de Camara--- (solo si no tiene raton)", pos_texto.ajustar_texto(++linea), color_texto_titulos, fuente);
    dibujar_texto("[W][S][A][D]    --> Rotar camara arriba/abajo/izquierda/derecha", pos_texto.ajustar_texto(++linea), color_texto, fuente);
    dibujar_texto("[CTRL] + [W][S] --> Alejar/Acercar camara", pos_texto.ajustar_texto(++linea), color_texto, fuente);
    ++linea;
    dibujar_texto("---Controles Escena---", pos_texto.ajustar_texto(++linea), color_texto_titulos, fuente_titulos);
    dibujar_texto("[ESPACIO] --> Pausar/Reanudar Tiempo (Animación si se le quiere llamar así)", pos_texto.ajustar_texto(++linea), color_texto, fuente);
    //++linea;
    dibujar_texto("[V]       --> Seleccionar Viento", pos_texto.ajustar_texto(++linea), color_texto, fuente);
    dibujar_texto("¡¡Pulsar CTRL + RUEDECILLA (o flechas arriba/abajo si no tiene raton) para modificar su velocidad!!", pos_texto.ajustar_texto(++linea), color_texto, fuente);
    dibujar_texto("¡¡Pulsar SHIFT + RUEDECILLA (o SHIFT + flechas) para modificar su frecuencia!!", pos_texto.ajustar_texto(++linea), color_texto, fuente);
    dibujar_texto("¡¡Pulsar ALT + RUEDECILLA (o ALT + flechas) para modificar su amplitud!!", pos_texto.ajustar_texto(++linea), color_texto, fuente);
    //++linea;
    dibujar_texto("[M]       --> Seleccionar Molino", pos_texto.ajustar_texto(++linea), color_texto, fuente);
    dibujar_texto("¡¡Pulsar CTRL + RUEDECILLA (o flechas arriba/abajo) para modificar su angulo de giro!!", pos_texto.ajustar_texto(++linea), color_texto, fuente);
    //++linea;
    dibujar_texto("[N]       --> Seleccionar Nube", pos_texto.ajustar_texto(++linea), color_texto, fuente);
    dibujar_texto("¡¡Pulsar CTRL + RUEDECILLA para modificar su densidad!! (No existe alternativa sin ratón)", pos_texto.ajustar_texto(++linea), color_texto, fuente);
    dibujar_texto("¡¡Use las flechas de arriba/abajo,izq/der,RePag/AvPag para modificar su tamanio en (+y-y,+x-x,+z-z)!!", pos_texto.ajustar_texto(++linea), color_texto, fuente);
    //++linea;
    dibujar_texto("[L]       --> Seleccionar Lluvia", pos_texto.ajustar_texto(++linea), color_texto, fuente);
    dibujar_texto("¡¡Pulsar CTRL + RUEDECILLA (o flechas arriba/abajo) para modificar su intensidad!!", pos_texto.ajustar_texto(++linea), color_texto, fuente);
    //++linea;
    //dibujar_texto("[G]       --> Seleccionar Girasol", pos_texto.ajustar_texto(++linea), color_texto, fuente);
    //dibujar_texto("¡¡Pulsar CTRL + MUEVE EL RATON por la pantalla para que te siga!!", pos_texto.ajustar_texto(++linea), color_texto, fuente);
    //dibujar_texto("Sin raton no podras usar esta funcion :(", pos_texto.ajustar_texto(++linea), color_texto, fuente);
    ++linea;
    dibujar_texto("---------- [PULSE CUALQUIER TECLA PARA SALIR] ----------", pos_texto.ajustar_texto(++linea), color_texto_rojo, fuente_titulos);
}
void configuracion_luz()
{
    // CREAR UNA CLASE EXTRA PARA LA LUZ
    //cerr << "Color cielo: " << escena_p3.sol.color_cielo.actual.r << " " << escena_p3.sol.color_cielo.actual.g << " " << sol.color_cielo.actual.b << endl;
    escena_p3.sol.color_cielo.cambiar_a_final();
    glClearColor(escena_p3.sol.color_cielo.actual.r, escena_p3.sol.color_cielo.actual.g, escena_p3.sol.color_cielo.actual.b, 1);
    // Configuración de la luz
    GLfloat luz_ambiental[]=    {0.05,0.05,0.05,0.0}, // Es la que tiene cuando no le da la luz, entonces es una fracción
            luz_difusa[]    =   {0.5,0.5,0.5,0.5}, // Difusa y especular iguales
            luz_especular[] =   {0.5,0.5,0.5,0.5},
            luz_posicion[]  =   {0.0,0.0,0.0,0.0};

    glLightfv(GL_LIGHT1, GL_AMBIENT, luz_ambiental);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, luz_difusa);
    glLightfv(GL_LIGHT1, GL_SPECULAR, luz_especular);
    glLightfv(GL_LIGHT1, GL_POSITION, luz_posicion);

    GLfloat luz_difusa_sol[]    =   {1.0,1.0,1.0,1.0}, // Difusa y especular iguales
            luz_especular_sol[] =   {1.0,1.0,1.0,1.0};            
            
    glLightfv(GL_LIGHT2, GL_AMBIENT, (GLfloat *) &escena_p3.sol.color_sol.actual);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, luz_difusa_sol);
    glLightfv(GL_LIGHT2, GL_SPECULAR, luz_especular_sol);
    glLightfv(GL_LIGHT2, GL_POSITION, (GLfloat *) &escena_p3.sol.posicion);

    glDisable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);  
}

void draw(void)
{
    clean_window();
    change_observer();
    if (mostrar_controles)
        draw_controles();
    else
    {
        configuracion_luz();
        draw_objects();
        draw_axis();
        printear_info();
    }
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
void menu(int key)
{
    //glutAddMenuEntry("HORA", 0);
    //glutAddMenuEntry("GIRO_MOLINO", 1);
    //glutAddMenuEntry("GIRASOL", 2);
    //glutAddMenuEntry("TAMANIO_NUBE", 3);
    //glutAddMenuEntry("LLUVIA", 4);
    switch (key)
    {
    case 0:
        variable_seleccionada = HORA;
        break;
    case 1:
        variable_seleccionada = GIRO_MOLINO;
        break;
    case 2:
        variable_seleccionada = GIRASOL;
        break;
    case 3:
        variable_seleccionada = TAMANIO_NUBE;
        break;
    case 4:
        variable_seleccionada = LLUVIA;
        break;
    }
}

void normal_key(unsigned char tecla_pulsada, int x, int y)
{
    int modificadores = glutGetModifiers();
    if (mostrar_controles)
    {
        mostrar_controles = false;
        Observer_position = ultima_posicion;
        Observer_angle_x = ultimo_angulo_x;
        Observer_angle_y = ultimo_angulo_y;
    }
    else if (tecla_pulsada == 27) // Escape
    {
        mostrar_controles = true;
        ultima_posicion = Observer_position;
        ultimo_angulo_x = Observer_angle_x;
        ultimo_angulo_y = Observer_angle_y;
        Observer_position = {0.0, 0.0, 15*Front_plane};
        Observer_angle_x = 0.0;
        Observer_angle_y = 0.0;
    }

    if (tecla_pulsada == 13) // Enter
    {
        modo_interfaz = static_cast<_modo_interfaz>((modo_interfaz + 1) % 3);
        switch (modo_interfaz)
        {
            case ESCENA_P3: cerr << "Modo escena P3" << endl; t_objeto = ESCENA_FINAL; break;
            case DEFAULT: cerr << "Modo normal" << endl; t_objeto = HOJA_GIRASOL; break;
            case CALIBRACION_CURVAS: cerr << "Modo calibración curvas" << endl; t_objeto = HOJA_GIRASOL; break;
        }  
    }   
    if (tecla_pulsada == 32) // Espacio = animación
    {
        paso_tiempo_automatico = !paso_tiempo_automatico;
        escena_p3.instante_previo = glutGet(GLUT_ELAPSED_TIME);
        escena_p3.paso_tiempo_automatico = paso_tiempo_automatico;
    }   
    escena_p3.liberar_sol = escena_p3.liberar_sol and (tecla_pulsada == 32 or tecla_pulsada == 13 or tecla_pulsada == 27);
    escena_p3.viento.frecuencia_manual = escena_p3.viento.frecuencia_manual and (tecla_pulsada == 32 or tecla_pulsada == 13 or tecla_pulsada == 27);
    switch (toupper(tecla_pulsada))
    {
	case 'Q':exit(0);
	case '1':   modo = POINTS;          break;
	case '2':   modo = EDGES;           break;
	case '3':   modo = SOLID;           break;
	case '4':   modo = SOLID_COLORS;    break;
    case '5':   modo = SOLID_PHONG_FLAT;break;
    case '6':   modo = SOLID_PHONG_GOURAUD;break;
    case '7':   modo = SOLID_TEXTURE;   break;

    case 'O':   t_objeto = OBJETO_PLY;  break;	
    case 'R':   t_objeto = ROTACION;    break;
    //case 'P':   if(modo_interfaz==CALIBRACION_CURVAS)
    //            {
    //                if (punto_en_calibracion == PUNTO_1)
    //                    punto_en_calibracion = PUNTO_2;
    //                else
    //                    punto_en_calibracion = PUNTO_1;
    //            }
    //            else
    //                t_objeto = PIRAMIDE;
    //            break;
    case 'C':   t_objeto = CUBO;        break;
    //case 'L':   t_objeto = CILINDRO;    break;
    //case 'N':   t_objeto = CONO;        break;
    case 'E':   t_objeto = ESFERA;      break;

    // PRÁCTICA 3
    case 'L': variable_seleccionada = LLUVIA; break;
    case 'H': variable_seleccionada = HORA; break;
    //case 'P': variable_seleccionada = OSCILACION_PRADERA; break;
    case 'M': variable_seleccionada = GIRO_MOLINO; break;
    case 'N': variable_seleccionada = TAMANIO_NUBE; break;
    case 'G': variable_seleccionada = GIRASOL; break;
    case 'V': variable_seleccionada = VIENTO; escena_p3.viento.frecuencia_manual = true; break; 
    case 'W':
        if (modificadores == GLUT_ACTIVE_CTRL)
            Observer_position.z /= 1.1;
        else
            Observer_angle_x -= 0.1*Observer_position.z;
        break;
    case 'S':
        if (modificadores == GLUT_ACTIVE_CTRL)
            Observer_position.z *= 1.1;
        else
            Observer_angle_x += 0.1*Observer_position.z;
        break;
    case 'A':
        Observer_angle_y -= 0.1*Observer_position.z;
        break;
    case 'D':
        Observer_angle_y += 0.1*Observer_position.z;
        break;
	}
    glutPostRedisplay();
}

//***************************************************************************
// Funcion llamada cuando se aprieta una tecla especial
//
// el evento manda a la funcion:
// codigo de la tecla

//***************************************************************************
void movimiento_raton(int x, int y)
{
    int modificadores = glutGetModifiers();
    //cerr << "Punto del observador: " << Observer_angle_x << " " << Observer_angle_y << endl;
    //cerr << "Posicion del observador: " << Observer_position.x << " " << Observer_position.y << " " << Observer_position.z << endl;
    if ((boton_movimiento_pulsado or boton_rotacion_pulsado) and !mostrar_controles)
    {
        int dx = x - ultima_pos_raton_x;
        int dy = y - ultima_pos_raton_y;
        if (boton_rotacion_pulsado)
        {
            Observer_angle_y += dx * 0.1f;
            Observer_angle_x += dy * 0.1f;
        }
        else if (boton_movimiento_pulsado)
        {
            if (variable_seleccionada == GIRASOL)
            {
                //escena_p3.liberar_sol = true;
                //cerr << "Pos sol al entrar: " << escena_p3.sol.posicion.x << " " << escena_p3.sol.posicion.z << endl;
                //GLfloat porcentaje_x = ((float)x/glutGet(GLUT_WINDOW_WIDTH));
                //GLfloat valor_x = Observer_position.x*0.75 + porcentaje_x*Observer_position.x*0.75;
                //GLfloat pendiente_recta  = (-Observer_position.z*0.75)/(Observer_position.x*0.75);
                //cerr << "Observer position: " << Observer_position.x << " " << Observer_position.z << endl;
                //escena_p3.sol.posicion.x = valor_x;
                //escena_p3.sol.posicion.z = pendiente_recta*valor_x;
                //cerr << "Pendiente recta: " << pendiente_recta << endl;
                //cerr << "Valor x: " << valor_x << endl;
                //cerr << "Porcentaje x: " << porcentaje_x << endl;
                //escena_p3.sol.posicion.x = porcentaje_x*escena_p3.sol.punto_minimo_x
                //cerr << "Pos sol al salir: " << escena_p3.sol.posicion.x << " " << escena_p3.sol.posicion.z << endl;
            }
            else
            {
                Observer_position.x += dx * 0.0005*Observer_position.z;
                Observer_position.y -= dy * 0.0005*Observer_position.z;
            }
        }
        ultima_pos_raton_x = x;
        ultima_pos_raton_y = y;
        glutPostRedisplay();
    }
}

void botones_raton(int button, int state, int x, int y)
{
    int modificadores = glutGetModifiers();
    boton_rotacion_pulsado = (button == GLUT_RIGHT_BUTTON or button == GLUT_MIDDLE_BUTTON) and state == GLUT_DOWN;
    boton_movimiento_pulsado = button == GLUT_LEFT_BUTTON and state == GLUT_DOWN;
    if (state == GLUT_DOWN)
    {
        if (mostrar_controles)
        {
            mostrar_controles = false;
            Observer_position = ultima_posicion;
            Observer_angle_x = ultimo_angulo_x;
            Observer_angle_y = ultimo_angulo_y;
        }
        else if (button == GLUT_RIGHT_BUTTON or button == GLUT_LEFT_BUTTON or button == GLUT_MIDDLE_BUTTON)
        {
            ultima_pos_raton_x = x;
            ultima_pos_raton_y = y;
        }
        else if (button == 3 or button == 4)
        {
            if (modificadores != GLUT_ACTIVE_CTRL and modificadores != GLUT_ACTIVE_SHIFT and modificadores != GLUT_ACTIVE_ALT)
                Observer_position.z = button == 4 ? Observer_position.z * 1.1 : Observer_position.z / 1.1;
            else if (modo_interfaz == ESCENA_P3)
            {
                switch (variable_seleccionada)
                {
                    case HORA:
                            escena_p3.hora = button == 4 ? escena_p3.hora < 24 ? escena_p3.hora + 1.0/escena_p3.movimientos_por_hora : 24 : escena_p3.hora > 0 ? escena_p3.hora - 1.0/escena_p3.movimientos_por_hora : 0;
                    break;
                    case VIENTO:
                    {
                        if (modificadores == GLUT_ACTIVE_CTRL)
                            escena_p3.viento.velocidad = button == 4 ? escena_p3.viento.velocidad * 1.1 : (escena_p3.viento.velocidad < 10 ? 10 : escena_p3.viento.velocidad * 0.9);
                        else if (modificadores == GLUT_ACTIVE_SHIFT)
                            escena_p3.viento.onda.frecuencia = button == 4 ? escena_p3.viento.onda.frecuencia * 1.1 : escena_p3.viento.onda.frecuencia * 0.9;
                        else if (modificadores == GLUT_ACTIVE_ALT)
                            escena_p3.viento.onda.amplitud = button == 4 ? escena_p3.viento.onda.amplitud * 1.1 : escena_p3.viento.onda.amplitud * 0.9;
                    }
                    break;
                    case GIRO_MOLINO:
                        escena_p3.molino.angulo_helice += button == 4 ? (90/7) : -(90/7);
                        //cerr << "Angulo helice: " << escena_p3.molino.angulo_helice << endl;
                    break;
                    case TAMANIO_NUBE:
                        escena_p3.nube.densidad += button == 4 ? 5 : -5;
                    break;
                    case LLUVIA:
                        escena_p3.nube.intensidad_lluvia += button == 3 ? (escena_p3.nube.intensidad_lluvia < 1.0 ? 0.025 : 0) : (escena_p3.nube.intensidad_lluvia >= 0.0 ? -0.025 : 0);
                    break;
                }
            }
        }

        glutPostRedisplay();
    }

}

void special_key(int tecla_pulsada, int x, int y)
{
    int modificadores = glutGetModifiers();
    if (mostrar_controles)
    {
        mostrar_controles = false;
        Observer_position = ultima_posicion;
        Observer_angle_x = ultimo_angulo_x;
        Observer_angle_y = ultimo_angulo_y;
    }
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
            else if (modo_interfaz == ESCENA_P3)
            {
                switch (variable_seleccionada)
                {
                    case TAMANIO_NUBE: escena_p3.nube.tam.x -= 0.1; break;
                }
            }
            break;
        case GLUT_KEY_RIGHT:
            if (modo_interfaz == CALIBRACION_CURVAS)
            {
                if (punto_en_calibracion == PUNTO_1)
                    hoja_girasol.punto_curva_1.x += 0.1;
                else
                    hoja_girasol.punto_curva_2.x += 0.1;
            }
            else if (modo_interfaz == ESCENA_P3)
            {
                switch (variable_seleccionada)
                {
                    case TAMANIO_NUBE: escena_p3.nube.tam.x += 0.1; break;
                }
            }
            break;
        case GLUT_KEY_UP:
            if (modo_interfaz == CALIBRACION_CURVAS)
            {
                if (punto_en_calibracion == PUNTO_1)
                    hoja_girasol.punto_curva_1.y += 0.1;
                else
                    hoja_girasol.punto_curva_2.y += 0.1;
            }
            else  if (modo_interfaz == ESCENA_P3)
            {
                switch (variable_seleccionada)
                {
                    case HORA:
                        if (escena_p3.hora < 24)
                            escena_p3.hora += 1.0/escena_p3.movimientos_por_hora;
                    break;
                    case VIENTO:
                        modificadores == GLUT_ACTIVE_SHIFT ? escena_p3.viento.velocidad += 10.0 : modificadores == GLUT_ACTIVE_ALT ? escena_p3.viento.onda.amplitud *= 1.1 : escena_p3.viento.onda.frecuencia *= 1.1;
                    break;
                    case GIRO_MOLINO:
                        escena_p3.molino.angulo_helice += 90/7;
                    break;
                    case TAMANIO_NUBE:
                        escena_p3.nube.tam.z += 0.1;
                    break;
                    case LLUVIA:
                        escena_p3.nube.intensidad_lluvia += escena_p3.nube.intensidad_lluvia < 1.0 ? 0.025 : 0;
                    break;
                }
            }
            break;
        case GLUT_KEY_DOWN:
            if (modo_interfaz == CALIBRACION_CURVAS)
            {
                if (punto_en_calibracion == PUNTO_1)
                    hoja_girasol.punto_curva_1.y -= 0.1;
                else
                    hoja_girasol.punto_curva_2.y -= 0.1;
            }
            else if (modo_interfaz == ESCENA_P3)
            {
                switch (variable_seleccionada)
                {
                    case HORA:
                        if (escena_p3.hora > 0)
                            escena_p3.hora -= 1.0/escena_p3.movimientos_por_hora;
                    break;
                    case VIENTO:
                        modificadores == GLUT_ACTIVE_SHIFT ? escena_p3.viento.velocidad -= 10.0 : modificadores == GLUT_ACTIVE_ALT ? escena_p3.viento.onda.amplitud *= 0.9 : escena_p3.viento.onda.frecuencia *= 0.9;
                    break;
                    case GIRO_MOLINO:
                        escena_p3.molino.angulo_helice -= 90/7;
                    break;
                    case TAMANIO_NUBE:
                        escena_p3.nube.tam.z -= 0.1;
                    break;
                    case LLUVIA:
                        escena_p3.nube.intensidad_lluvia -= escena_p3.nube.intensidad_lluvia > 0.0 ? 0.025 : 0;
                    break;
                }
            }
            break;
        case GLUT_KEY_PAGE_UP:
            if (modo_interfaz == CALIBRACION_CURVAS)
            {
                if (punto_en_calibracion == PUNTO_1)
                    hoja_girasol.punto_curva_1.z += 0.1;
                else
                    hoja_girasol.punto_curva_2.z += 0.1;
            }
            else if (modo_interfaz == ESCENA_P3)
            {
                switch (variable_seleccionada)
                {
                    case TAMANIO_NUBE: escena_p3.nube.tam.y += 0.1; break;
                }
            }
            break;
        case GLUT_KEY_PAGE_DOWN:
            if (modo_interfaz == CALIBRACION_CURVAS)
            {
                if (punto_en_calibracion == PUNTO_1)
                    hoja_girasol.punto_curva_1.z -= 0.1;
                else
                    hoja_girasol.punto_curva_2.z -= 0.1;
            }
            else if (modo_interfaz == ESCENA_P3)
            {
                switch (variable_seleccionada)
                {
                    case TAMANIO_NUBE: escena_p3.nube.tam.y -= 0.1; break;
                }
            }
            break;
    }


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
    // La animación es "activar o desactivar" el paso del tiempo
    // Lo hago de esta manera porque se usa una equivalencia del tiempo real
    // Y los objetos necesitan sincronizarse, por eso lo hago "integrado" en el código en lugar de por pasos
    // Cada clase le manda la orden a la siguiente de que tiene que hacer algo
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
    Observer_position.z=7*Front_plane;
    Observer_angle_x=18.7;
    Observer_angle_y=25.1;
    Observer_position = {0.1, -0.7, 9.317};

    // se indica el color para limpiar la ventana	(r,v,a,al)
    // blanco=(1,1,1,1) rojo=(1,0,0,1), ...);
    //glClearColor(0.0, 0.0, 0.0, 1.0);
    escena_p3.duracion_dia_real = duracion_real_dia;
    // se habilita el z-bufer
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    change_projection();
    glViewport(0,0,Window_width,Window_high);
}

//***************************************************************************

int prepara_textura (char *file)
{
    int tex_id;
    
    //Dejar comentado hasta no incorporar texturas
    vector<unsigned char> data; 
    CImg<unsigned char> image;

    image.load(file);

    // empaquetamos bien los datos
    for (long y = 0; y < image.height(); y ++)
        for (long x = 0; x < image.width(); x ++)
        {
        unsigned char *r = image.data(x, y, 0, 0);
        unsigned char *g = image.data(x, y, 0, 1);
        unsigned char *b = image.data(x, y, 0, 2);
        data.push_back(*r);
        data.push_back(*g);
        data.push_back(*b);
        }

    glGenTextures(1,(GLuint *) &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //TRASFIERE LOS DATOS A GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(),
        0, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);
    
    return tex_id;
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
    //glutDisplayFunc(display_prueba);
    // asignación de la funcion llamada "change_window_size" al evento correspondiente
    glutReshapeFunc(change_window_size);
    // asignación de la funcion llamada "normal_key" al evento correspondiente
    glutKeyboardFunc(normal_key);
    // asignación de la funcion llamada "tecla_Especial" al evento correspondiente
    glutSpecialFunc(special_key);


    // Creamos menú
    glutCreateMenu(menu);
    glutAddMenuEntry("HORA", 0);
    glutAddMenuEntry("MOLINO", 1);
    glutAddMenuEntry("GIRASOL", 2);
    glutAddMenuEntry("NUBE", 3);
    glutAddMenuEntry("LLUVIA", 4);
    //glutAttachMenu(GLUT_LEFT_BUTTON);
    // Control del ratón
    glutMouseFunc(botones_raton);
    glutMotionFunc(movimiento_raton);

    glutDisplayFunc(draw);
    glutIdleFunc(animacion);
    initialize();
    id_tex=prepara_textura("./skybox.jpg");


    // funcion de inicialización
    // creación del objeto ply
    //ply.parametros(argv[1]);

    //ply = new _objeto_ply(argv[1]);

    // inicio del bucle de eventos
    glutMainLoop();
    return 0;
}
