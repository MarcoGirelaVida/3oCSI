#ifndef OBJETOS_B3_H
#define OBJETOS_B3_H

#include <vector>
#include <random>
#include <GL/gl.h>
#include <GL/glut.h>
#include "vertex.h"
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
using namespace std;

const float AXIS_SIZE=5000;
typedef enum{POINTS,EDGES,SOLID,SOLID_COLORS,SOLID_PHONG_FLAT,SOLID_PHONG_GOURAUD,SOLID_TEXTURE} _modo;


float aleatorio(float minimo = 0.0, float maximo = 1.0);

//void temporizador(float duracion_total_segundos)
//{
//    float hora = 0.0;
//    const float limite_hora = 24.0f;
//
//    // Calcula el incremento necesario para que `hora` llegue a 24 en el tiempo especificado
//    float incremento = limite_hora / (duracion_total_segundos * 1000.0f / 100); // Ajusta 100 ms como paso
//
//    while (hora < limite_hora) {
//        hora += incremento;
//        if (hora > limite_hora) hora = limite_hora;  // Limitar `hora` a 24
//
//        // Muestra el valor de `hora` en cada incremento
//        cerr << "HORA DEL DIA: " << fixed << setprecision(2) << hora << endl;
//
//        // Espera 100 ms antes de incrementar nuevamente
//        this_thread::sleep_for(chrono::milliseconds(100));
//    }
//
//    cerr << "EL DIA HA TERMINADO" << endl;
//    exit(0);
//}
struct Onda
{
    GLfloat amplitud; // altura maxima en y
    GLfloat frecuencia; // = numero de sube y bajas por unidad de tiempo
    GLfloat longitud; // distancia entre picos
    GLfloat offset;

    Onda(GLfloat amplitud = 1.0, GLfloat frecuencia = 1, GLfloat longitud = 1.0, GLfloat offset = 0.0)
    : amplitud(amplitud), frecuencia(frecuencia), longitud(longitud), offset(offset)
    {}

    GLfloat operator()(GLfloat hora) const
    {
        //GLfloat frecuencia_temporal = (2*M_PI / 8.0) * frecuencia;
        GLfloat frecuencia_temporal = 2*M_PI * frecuencia;
        return amplitud * sin(hora*frecuencia_temporal + offset);
    }
    GLfloat operator()(GLfloat x, GLfloat hora) const
    {
        // REALMENTE LA PARTE DE LA LONGITUD DE ONDA DA IGUAL
        // PORQUE DIRETAMENTE SE INVIERTE EN LAS CURVAS
        // Un octavo de ciclo por paso, si fuese ciclo completo sería 2*M_PI / frecuencia
        // Si solo muestreo una vez por hora doy un octavo de paso por hora.
        // Si muestreo 50 veces por hora doy 50 octavos de paso por hora.
        // Y por tanto avanzo (2*M_PI / 8.0 * frecuencia) por cada 50avo de hora
        // Si pusiese solo 2*M_PI / 8.0 significaría que avanzo un octavo de ciclo por hora aun teniendo 50 de muestreo
        //GLfloat frecuencia_temporal = (2*M_PI / 8.0) * frecuencia; // ¿Cuántos angulos avanzo por paso (hora)?

        GLfloat frecuencia_temporal = 2*M_PI * frecuencia;
        GLfloat frecuencia_espacial = 2*M_PI / longitud; // cuantos angulos avanzo por unidad de longitud, cada 2 pasos avanzo medio ciclo
        return amplitud * sin(hora*frecuencia_temporal + x*frecuencia_espacial + offset);
    }

    GLfloat derivada_onda(GLfloat hora)
    {
        return amplitud * 2 * M_PI * frecuencia * cos(2 * M_PI * frecuencia * hora + offset);
    }
};
struct _color
{
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;

    _color(unsigned r, unsigned g, unsigned b, float a = 1.0)
    : r(r/255.0), g(g/255.0), b(b/255.0), a(a)
    {}

    _color(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0)
    : r(r), g(g), b(b), a(a)
    {}

    void operator=(const _color& c)
    {
        r = c.r;
        g = c.g;
        b = c.b;
        a = c.a;
    }
    void operator+=(const _color& c)
    {
        r += c.r;
        g += c.g;
        b += c.b;

    }
    void operator+=(const GLfloat f)
    {
        r += f;
        g += f;
        b += f;
    }
    void operator-=(const _color& c)
    {
        r -= c.r;
        g -= c.g;
        b -= c.b;
    }
    void operator-= (const GLfloat f)
    {
        r -= f;
        g -= f;
        b -= f;
    }
    _color operator-() const
    {
        return _color(-r, -g, -b);
    }
    _color operator+(const _color& c) const
    {
        _color result(*this);
        result += c;
        return result;
    }
    _color operator+(GLfloat f) const
    {
        _color result(*this);
        result += f;
        return result;
    }
    _color operator-(const _color& c) const
    {
        _color result(*this);
        result -= c;
        return result;
    }
    _color operator-(const GLfloat f) const
    {
        _color result(*this);
        result -= f;
        return result;
    }
    _color operator*(const GLfloat f) const
    {
        return _color(r*f, g*f, b*f);
    }

    //string to_string() const
    //{
    //    return to_string(r) + " " + to_string(g) + " " + to_string(b);
    //}
};
struct Color {
    
    public:
    _color actual;
    private:
    _color original;
    _color final;

    public:
    static const _color c_default; 

    Color (const _color& c1, const _color& c2) : actual(c1), original(c1), final(c2) {}

    Color (const _color& c1) :  actual(c1), original(c1), final(c1) {}

    Color (unsigned r, unsigned g, unsigned b, float a = 1.0) : actual(r, g, b, a), original(r, g, b, a), final(r, g, b, a) {}

    Color (GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0) : actual(r, g, b, a), original(r, g, b, a), final(r, g, b, a) {}

    void cambiar_a_inicial()
    {
        actual = original;
    }

    void cambiar_a_final()
    {
        actual = final;
    }

    void set_final(const _color& c)
    {
        final = c;
    }

    void set_original(const _color& c)
    {
        actual = c;
        original = c;
    }

    void actualizar(const float grado)
    {
        // 0 = r1, 1 = r2
        actual = original + (final-original)*grado;
    }

    void actualizar_hora(const GLfloat hora)
    {
        if (hora < 12)
            actualizar(1.0/12.0 * hora);
        else
            actualizar(1.0/12.0 * (23.0 - hora));
    }

    //void operator=(const Color& c)
    //{
    //    actual = c.actual;
    //    original = c.actual;
    //    final = c.final;
    //}
};

struct Coordenadas
{
    GLfloat x;
    GLfloat y;
    GLfloat z;

    Coordenadas(GLfloat x = 0.0f, GLfloat y = 0.0f, GLfloat z = 0.0f) : x(x), y(y), z(z) {}

    void operator-= (const Coordenadas& c)
    {
        x -= c.x;
        y -= c.y;
        z -= c.z;
    }

    void operator+= (const Coordenadas& c)
    {
        x += c.x;
        y += c.y;
        z += c.z;
    }

    Coordenadas operator-() const
    {
        return Coordenadas(-x, -y, -z);
    }

    Coordenadas operator-(const Coordenadas& c) const
    {
        return Coordenadas(x - c.x, y - c.y, z - c.z);
    }

    Coordenadas operator/(const GLfloat f) const
    {
        return Coordenadas(x/f, y/f, z/f);
    }
    Coordenadas operator=(const Coordenadas& c)
    {
        x = c.x;
        y = c.y;
        z = c.z;
        return *this;
    }

    bool operator==(const Coordenadas& otra) const {
        return (x == otra.x && y == otra.y && z == otra.z);
    }

    // Sobrecarga del operador !=
    bool operator!=(const Coordenadas& otra) const {
        return !(*this == otra);
    }

    Coordenadas ajustar_texto(unsigned linea)
    {
        return Coordenadas(x, y-z*linea, 0);
    }

    Coordenadas espejo_x() const
    {
        return Coordenadas(-x, y, z);
    }

    char *to_string() const
    {
        char *str = new char[100];
        sprintf(str, "(%fx%fx%f)", x, y, z);
        return str;
    }
};

const Coordenadas coordenadas_default = {0.0, 0.0, 0.0};

void dibujar_texto(const char *string, Coordenadas pos = coordenadas_default, Color color = Color::c_default, unsigned char fuente = 0);
//*************************************************************************
// BASE DE TODO
//*************************************************************************
class _puntos3D
{
public:
    _puntos3D();
    void draw_puntos(Color color, int grosor);
    vector<_vertex3f> vertices;
    //vector<_vertex3f> normales;
    //vector<_vertex3f> colores_vertices;
};

class _triangulos3D: public _puntos3D
{
public:
    // atributos
    vector<_vertex3i> caras;
    vector<_vertex3f> colores_caras;

    vector<_vertex3f> normales_caras;
    vector<_vertex3f> normales_vertices;

    vector<_vertex2f> texturas_vertices;

    bool calculadas_normales_caras = false;

    // material
    _vertex4f ambiente = _vertex4f(0.8,0.0,0.2,1.0);    //coeficientes ambiente 
    _vertex4f difuso = _vertex4f(0.8,0.0,0.2,1.0);      //coeficientes difuso
    _vertex4f especular = _vertex4f(0.5,0.5,0.5,1.0);   //coeficiente especular
    float brillo = 40;                                  //exponente del brillo 

    bool invertir_n = false;             // 1 en caso de invertir direcciones normales
public:

    _triangulos3D();
    void draw_aristas(Color color, int grosor);
    void draw_solido(Color color);
    void draw_solido_colores();
    void    draw_solido_phong_flat(Color color, GLfloat ratio_difuso, Color especular, float brillo);
    void    draw_solido_phong_gouraud(Color color, GLfloat ratio_difuso, Color especular, float brillo);
    void    draw_solido_textura(int etiqueta);
    void draw(_modo modo, Color color = Color::c_default, float grosor = 5, Coordenadas pos = coordenadas_default, GLfloat ratio_difuso = 1, Color especular = Color(0.6f,0.6,0.6,1.0), float brillo = 20);

    void colors_random();
    void colors_chess(float r1, float g1, float b1, float r2, float g2, float b2);

    /* calcular normales */

    void    calcular_normales_caras();
    void    calcular_normales_vertices();

};

//*************************************************************************
// OPERACIONES BÁSICAS
//*************************************************************************

class _objeto_ply: public _triangulos3D
{
public:
    _objeto_ply();
    void parametros(char *archivo);
};

class _rotacion: public _triangulos3D
{
public:
    _rotacion();
    void parametros(vector<_vertex3f> perfil, size_t num, bool tapa_inferior, bool tapa_superior, int tipo, unsigned char porcentaje_revolucion = 100, unsigned char porcentaje_generacion_esfera = 100, Coordenadas cuspide = coordenadas_default);
};

// tapa_in=0 sin tapa, tapa_in=1 con tapa
// tapa_su=0 sin tapa, tapa_su=1 con tapa
// tipo=0 revolucion normal
// tipo=1 esfera
// tipo=2 cono

class _extrusion: public _triangulos3D
{
public:
    _extrusion(vector<_vertex3f> poligono, float x, float y, float z);
};

class _rotacion_PLY: public _rotacion
{
public:
    _rotacion_PLY();
    void  parametros_PLY(char *archivo, size_t num);
};

//************************************************************************
// OBJETOS BASE
//************************************************************************
class _cubo: public _triangulos3D
{
public:
    _cubo(float tam=0.5);
};

class _cubo_tex: public _triangulos3D
{
public:
    _cubo_tex(float tam=0.5);
};
class _piramide: public _triangulos3D
{
public:
    Coordenadas tam;
    Coordenadas posicion;
    _piramide(Coordenadas tam);
};
class _cilindro: public _rotacion
{
public:
    _cilindro(float radio=1.0, float altura=2.0, size_t num=12);
};

class _cono: public _rotacion
{
public:
    Coordenadas cuspide;
    _cono(float radio=1.0, float altura=2.0, size_t num=12, Coordenadas cuspide = coordenadas_default);
};

class _esfera: public _rotacion
{
public:
    GLfloat radio;
    const size_t resolucion;
    const unsigned char porcentaje_generacion_x, porcentaje_generacion_y;
    _esfera(GLfloat radio=1.0, size_t resolucion=6, bool tapa_su = true, bool tapa_in = true, unsigned char porcentaje_generacion_x = 100, unsigned char porcentaje_generacion_y = 100);
};


//----------------------------------------------------------------------
//----------------------------------------------------------------------
// PROYECTO PROPIO P3
class _suelo: public _triangulos3D
{
public:
    Coordenadas tam;
    Coordenadas posicion;
    Color color_suelo;

    _suelo(Coordenadas dimenisones = {100, 0.25, 100}, Coordenadas pos = coordenadas_default) : tam(dimenisones), posicion(pos), color_suelo(108u, 136, 52) {}
    void draw(_modo modo, float grosor = 5); // , Coordenadas pos = coordenadas_default);
};

class _pradera : public _triangulos3D
{
    const GLfloat max_desfase = 0.5;
    const GLfloat max_angulo_inclinacion = 30.0;
    GLfloat desfase_puntas = 0.0;
    GLfloat desfase_z = 0.0;
    //GLfloat angulo = 0.0;
    GLfloat max_desfase_z = 0.1; // Radio de la planta
    pair<GLfloat,GLfloat> **tamanios_plantas;
public:
    vector<GLfloat> oscilacion_linea;
    GLfloat oscilacion_estatica = 0.0;
    Coordenadas foco_viento = {-0.5, 0.0, 0.0};
    size_t densidad = 8; // 10 plantas por unidad de area
    GLfloat radio_plantas = 0.035;
    Coordenadas tam;
    Coordenadas posicion;
    Color color_pradera;

    _pradera(Coordenadas dimensiones = {10, 0.25, 10}, Coordenadas pos = coordenadas_default) : tam(dimensiones), posicion(pos), color_pradera(118u, 146, 62, 0.7)
    {
        tamanios_plantas = new pair<GLfloat,GLfloat>*[static_cast<size_t>(densidad * dimensiones.x)];
        for (size_t i = 0; i < densidad*dimensiones.x; i++)
        {
            tamanios_plantas[i] = new pair<GLfloat,GLfloat>[static_cast<size_t>(densidad * dimensiones.x)];
            for (size_t j = 0; j < densidad*dimensiones.z; j++)
            {
                tamanios_plantas[i][j].first = aleatorio(radio_plantas*0.5, radio_plantas*1.5);
                tamanios_plantas[i][j].second = aleatorio(dimensiones.y*0.5, dimensiones.y*1.5);
            }
        }

    }
    void draw(_modo modo, float grosor = 5); // , Coordenadas pos = coordenadas_default);
};

class _sol : public _triangulos3D
{
    const size_t resolucion = 50;
public:
    GLfloat radio;
    const GLfloat punto_algido_y = 5.0;
    const GLfloat punto_minimo_x = 7.0;
    const GLfloat desplazamiento_z = -5.0;
    size_t num_fases = 24;
    Coordenadas posicion;
    Coordenadas posicion_original = {punto_minimo_x, 0.0, desplazamiento_z};
    Color color_sol;
    //_color luz_ambiente_mediodia = {0.6f, 0.6f, 0.6f};
    //_color luz_ambiente_atardecer = {0.4f, 0.2f, 0.1f};
    Color color_cielo = Color({0.6f, 0.3f, 0.2f}, {102u, 151, 200});
    Color color_luz_difusa_especular = Color({0.2f, 0.1f, 0.1f, 1.0f}, {0.4f, 0.4f, 0.4f, 1.0f});
    Color material_especular = Color(1.0f, 1.0f, 1.0f, 1.0f);
    //Color luz_ambiente;


    _sol(GLfloat radio = 1, Coordenadas pos = coordenadas_default) : radio(radio), posicion(pos), color_sol({0.3f, 0.1, 0.04}, {0.7f, 0.65, 0.55}) {};
    void draw(_modo modo, float grosor = 5); // , Coordenadas pos = coordenadas_default);, Coordenadas pos = coordenadas_default
    void actualizar_sol_hora(GLfloat hora);     
};

class _helice_molino : public _triangulos3D
{
protected:
    _piramide aspa;
    _cilindro palo_central;
    _esfera bola_central;

public:
    static constexpr GLfloat largo_aspas = 0.6;
    static constexpr GLfloat grosor_aspas = largo_aspas / 10.0;
    static constexpr GLfloat ancho_aspas = largo_aspas / 4.0;
    static constexpr size_t num_aspas = 10;
    static constexpr GLfloat radio_palo_central = grosor_aspas / 2.0;
    static constexpr GLfloat radio_bola_central = radio_palo_central * 3.0;
    static constexpr GLfloat largo_palo_central = largo_aspas / 2.0;
    Coordenadas posicion;
    Color color_helice;

    _helice_molino(Coordenadas pos = coordenadas_default)
        :
        aspa({ancho_aspas, grosor_aspas, largo_aspas}),
        palo_central(radio_palo_central, largo_palo_central, 30),
        bola_central(radio_bola_central, 30, true, true, 50, 100),
        posicion(pos),
        color_helice(86u, 43, 5)
        {}
    void draw(_modo modo, float grosor = 5); // , Coordenadas pos = coordenadas_default);
};

class _molino : public _triangulos3D
{
protected:
    _helice_molino helice;
    _cono tejado;
    _cilindro casa;

public:
    GLfloat angulo_helice = 0.0;
    static constexpr GLfloat altura_casa = 1.5;
    static constexpr GLfloat radio_casa = 0.75;
    static constexpr GLfloat altura_tejado = 0.45*altura_casa;
    static constexpr GLfloat radio_tejado = radio_casa + 0.2*radio_casa;
    Coordenadas posicion;
    Color color_molino_casa;
    Color color_molino_tejado;

    _molino(Coordenadas pos = coordenadas_default)
    :
    tejado(radio_tejado, altura_tejado),
    casa(radio_casa, altura_casa, 30),
    posicion(pos),
    color_molino_casa(206u, 219, 214),
    color_molino_tejado(86u, 5, 5)
    {}
    void draw(_modo modo, float grosor = 5); // , Coordenadas pos = coordenadas_default);
};

class _petalo_girasol : public _triangulos3D
{
public:
    Coordenadas punto_curva_1;
    Coordenadas punto_curva_2;
    GLfloat ancho = 0.07;
    GLfloat largo = 0.1;
    Coordenadas posicion;
    Color color_petalo;

    _petalo_girasol(Coordenadas punto_curva_1 = {0.4f*0.07f, 0.1f*0.1f} , Coordenadas punto_curva2 = {0.4f*0.07f, 0.8f*0.1f}, GLfloat ancho = 0.07, GLfloat largo = 0.1, Coordenadas pos = coordenadas_default, size_t resolucion = 30);
    //void draw(_modo modo, Color color, float grosor); // , Coordenadas pos = coordenadas_default);
};

class _hoja_girasol : public _triangulos3D
{
//protected :
//    _petalo_girasol hoja;

public:
    GLfloat ancho = 0.07;
    GLfloat largo = 0.1;
    Coordenadas punto_curva_1 = {0.068, 0.01, -0.02};
    Coordenadas punto_curva_2 = {0.028, 0.08, -0.01};
    Coordenadas posicion;
    Color color_hoja;

    _hoja_girasol(size_t resolucion = 30, Coordenadas pos = coordenadas_default)
    :
    posicion(pos), color_hoja(57u, 96, 51)
    {}
    void draw(_modo modo, float grosor = 5); // , Coordenadas pos = coordenadas_default);
    
};
class _tallo_girasol : public _triangulos3D
{
public:
    GLfloat radio_tallo = 0.1;
    GLfloat largo_tallo = 1.0;
    Coordenadas posicion;
    Color color_tallo;

    _tallo_girasol(Coordenadas pos = coordenadas_default) : posicion(pos), color_tallo(94u, 112, 39) {}
    void draw(_modo modo, float grosor = 5); // , Coordenadas pos = coordenadas_default);
};
class _cabeza_girasol : public _triangulos3D
{
private:
    GLfloat instante_inicio_despertar = 0.0;
    GLfloat instante_inicio_descanso = 0.0;
    bool animacion_despertar = false;
    bool animacion_descanso = false;

public:
    _hoja_girasol hoja;
    _petalo_girasol petalo;
    _tallo_girasol cuello;
    //_esfera semillero;
    GLfloat duracion_cinematica = 2.0; // Segundos
    Coordenadas rotacion = {0.0, 0.0, 0.0};
    const GLfloat max_angulo_petalos = 45.0;
    GLfloat angulo_petalos = 0.0;
    GLfloat radio_semillero = petalo.largo * 0.62;
    size_t num_petalos = 6;
    size_t num_hojitas = 12;
    size_t num_capas_hojas = 3;
    size_t num_capas_petalo = 3;
    Coordenadas posicion;
    Color color_semillero;

    _cabeza_girasol(Coordenadas pos = coordenadas_default)
    :
    posicion(pos), color_semillero(59u, 46, 33)
    {}
    void draw(_modo modo, GLfloat grosor = 5); // , Coordenadas pos = coordenadas_default);
    void mirar_al_sol(Coordenadas pos_sol);
};

class _girasol : public _triangulos3D
{
private:
    bool recibiendo_agua = false;
    vector<GLfloat> variacion_tam_rama;
    vector<GLfloat> angulo_rama;
    GLfloat longitud_min_rama;
    GLfloat angulo_min_rama = 15;
    GLfloat angulo_max_rama = 75;
    GLfloat max_angulo_oscilacion = 15;
    GLfloat altura_tallo_min = 0.5;
    GLfloat altura_tallo_max = 1.5;

protected:
    _tallo_girasol tallo;
    _cabeza_girasol cabeza;
    _hoja_girasol hoja;
    _petalo_girasol petalo;
public:
    GLfloat oscilacion = 0.0;
    size_t num_ramas = 5;
    Coordenadas posicion;

    _girasol(Coordenadas pos = coordenadas_default);
    void draw(_modo modo, float grosor = 5, Coordenadas pos_sol = coordenadas_default);

};

struct Particula
{
    Coordenadas posicion;
    GLfloat radio;
    GLfloat transparencia;
    GLfloat fase_onda;
};

class _nube : public _triangulos3D
{
private:
    vector<Particula> particulas;
    unsigned instante_previo = 0;
    GLfloat instante_actual = 0.0;
    unsigned max_particulas = 10000;
    GLfloat max_frecuencia = 10;
    GLfloat min_frecuencia = 0.3;

    GLfloat velocidad_caida_max = 10.0; //m/s
    unsigned min_densidad_lluvia = 10;
    unsigned max_densidad_lluvia = 500;
    GLfloat frecuencia_min_nubes = 0.1;
    GLfloat frecuencia_max_nubes = 0.5;
public:
    GLfloat radio_medio = 0.2;
    GLfloat radio_medio_lluvia = 0.01;
    bool pausar = false;
    bool lluvia_activa = true;
    GLfloat intensidad_lluvia = 0.2; // Número de 0 a 1
    Onda onda;
    unsigned densidad;
    Coordenadas tam;
    Coordenadas posicion = {0, 0, 0};
    GLfloat transparencia_sin_lluvia = 0.33;
    Color color_nube = {{255u, 255, 255, 0.3}, {55u, 61, 75, 0.8}};
    Color color_lluvia = {0.5f, 0.5f, 0.6f, 0.3};


    _nube(unsigned densidad = 10, Coordenadas tam = {10, 1.5, 5}) : densidad(densidad), tam(tam)
    {
        onda.frecuencia = 0.1;
        onda.amplitud = 0.5;
        for (size_t i = 0; i < max_particulas; i++)
        {
            GLfloat x = aleatorio();
            GLfloat y = aleatorio();
            GLfloat z = aleatorio();
            GLfloat transparencia = aleatorio();
            GLfloat radio = aleatorio();
            GLfloat fase_onda = aleatorio(0, 2*M_PI);
            //GLfloat transparencia = aleatorio(0.5, 1.0);
            particulas.push_back({{x, y, z}, radio, transparencia, fase_onda});
        }
    }
    void draw(_modo modo, float grosor = 5);
};

class _lluvia : public _triangulos3D
{
private:
    vector<Particula> particulas;
    unsigned instante_previo = 0;
    GLfloat instante_actual = 0.0;
    unsigned max_particulas = 10000;
public:
    _nube nube;
    Coordenadas tam;
    Coordenadas posicion;
    Color color_lluvia = {0u, 0, 255, 0.2};

    _lluvia(Coordenadas tam = {10, 1.5, 5}) : tam(tam) {}
    void draw(_modo modo, float grosor = 5); // , Coordenadas pos = coordenadas_default);
};

class _viento : public _triangulos3D
{

private:
    unsigned instante_previo = 0.0;
    GLfloat instante_actual = 0.0;
    const GLfloat escalado_max_brisas = 1.5;
    const GLfloat frecuencia_min = 0.2;
    const GLfloat frecuencia_max = 2;
    vector<GLfloat> consulta_viento_pradera = {0.0};
    vector<unsigned> instante_ultima_brisa_toco = {0};
protected:
    _hoja_girasol brisa_viento;
public:
    Onda onda;
    bool pausar = false;
    bool viento_en_0 = false;
    vector<GLfloat> respuesta_viento_pradera;
    const GLfloat velocidad_max = 255;
    bool frecuencia_manual = false;
    GLfloat velocidad = 30.0;
    GLfloat velocidad_giro_molino = velocidad;
    GLfloat densidad_brisas = 0.5;
    Coordenadas area_efecto = {20, 7, 20};
    Coordenadas posicion;
    Color color_viento;
    // color_viento antiguo = 190u, 231, 237
    _viento(Coordenadas pos = coordenadas_default) : posicion(pos), color_viento(240u, 255, 255, 0.5)
    {
        brisa_viento.largo = 5;
        brisa_viento.ancho = 0.25;
        //brisa_viento.punto_curva_1 = Coordenadas(0.68, 0.5, 0.0);
        //brisa_viento.punto_curva_2 = Coordenadas(0.28, 2.2, 0.0);
        brisa_viento.punto_curva_1 = Coordenadas(brisa_viento.ancho, brisa_viento.largo/3.0, 0.0);
        brisa_viento.punto_curva_2 = Coordenadas(brisa_viento.ancho, brisa_viento.largo/3.0*2, 0.0);
        //onda.longitud = abs(lamina_viento.punto_curva_2.y - lamina_viento.punto_curva_1.y);
        brisa_viento.color_hoja.set_original(color_viento.actual);
        //lamina_viento   

        //GLfloat distancia_recorrida_en_un_segundo = (velocidad/3600)*1000;
        onda.frecuencia = frecuencia_min + (((velocidad / velocidad_max) * (frecuencia_max-frecuencia_min)*10)/10);
        //onda.longitud = distancia_recorrida_en_un_segundo / onda.frecuencia;
        onda.offset = M_PI*0.5;
    }
    void draw(_modo modo, float grosor=5); // , Coordenadas pos = coordenadas_default);
    GLfloat giro_helice(GLfloat rozamiento = 0.006, Coordenadas pos_molino  = coordenadas_default);
    GLfloat oscilacion_pradera_estatica();
    void consulta_pradera(GLfloat densidad, GLfloat tam_x, Coordenadas pos_pradera = coordenadas_default);
};

class _escena_P3 : public _triangulos3D
{
private:
    size_t ticket_consulta_viento = 0;
public:
    _viento viento;
    _nube nube;
    _suelo suelo;
    _sol sol;
    _molino molino;
    _girasol girasol;
    _pradera pradera;
    unsigned num_girasoles = 20;
    unsigned instante_previo = 0;
    const unsigned movimientos_por_hora = 10;
    unsigned duracion_dia_real = 60; // EN segundos
    GLfloat hora = 0.0;
    bool paso_tiempo_automatico = false;
    bool liberar_sol = false;
    //GLfloat velocidad_viento = 30.0;
    Coordenadas posicion;
    
    _escena_P3(Coordenadas pos = coordenadas_default) : posicion(pos)
    {
        viento.consulta_pradera(pradera.densidad, pradera.tam.x, pradera.posicion);
    }
    void draw(_modo modo, float grosor = 5); // , Coordenadas pos = coordenadas_default);
    void actualizar_hora();
    void printear_info();
};
#endif