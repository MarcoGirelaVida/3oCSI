#ifndef OBJETOS_B3_H
#define OBJETOS_B3_H

#include <vector>
#include <random>
#include <GL/gl.h>
#include "vertex.h"
#include <stdlib.h>
#include <iostream>
using namespace std;

const float AXIS_SIZE=5000;
typedef enum{POINTS,EDGES,SOLID,SOLID_COLORS} _modo;

float aleatorio(float minimo = 0.0, float maximo = 1.0);
struct Color {
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a = 1.0f;
    GLfloat r2 = 1.0f;
    GLfloat g2 = 1.0f;
    GLfloat b2 = 1.0f;
    GLfloat a2 = 1.0f;

    Color(int r, int g, int b)
    {
        this->r = r/255.0;
        this->g = g/255.0;
        this->b = b/255.0;
    }

    Color(GLfloat r, GLfloat g, GLfloat b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    void operator+=(Color c)
    {
        r += c.r;
        g += c.g;
        b += c.b;
    }
    void operator+=(GLfloat f)
    {
        r += f;
        g += f;
        b += f;
    }
    Color operator+(Color c_2)
    {
        Color c_1(*this);
        c_1 += c_2;
        return c_1;
    }
    Color operator+(GLfloat f)
    {
        Color c (*this);
        c += f;
        return c;
    }

    void actualizar(float grado)
    {
        r += (r2-r)*grado;
        g += (g2-g)*grado;
        b += (b2-b)*grado;
    }
};
const Color c_default = {0.0f, 0.0f, 0.0f};
struct Coordenadas
{
    GLfloat x;
    GLfloat y;
    GLfloat z;

    Coordenadas(GLfloat x = 0.0f, GLfloat y = 0.0f, GLfloat z = 0.0f) : x(x), y(y), z(z) {}

    bool operator==(const Coordenadas& otra) const {
        return (x == otra.x && y == otra.y && z == otra.z);
    }

    // Sobrecarga del operador !=
    bool operator!=(const Coordenadas& otra) const {
        return !(*this == otra);
    }


    Coordenadas espejo_x() const
    {
        return Coordenadas(-x, y, z);
    }
};

const Coordenadas coordenadas_default = {0.0, 0.0, 0.0};

//*************************************************************************
// BASE DE TODO
//*************************************************************************
class _puntos3D
{
public:
    _puntos3D();
    void draw_puntos(Color color, int grosor);
    vector<_vertex3f> vertices;
    vector<_vertex3f> colores_vertices;
};

class _triangulos3D: public _puntos3D
{
public:

    _triangulos3D();
    void draw_aristas(Color color, int grosor);
    void draw_solido(Color color);
    void draw_solido_colores();
    void draw(_modo modo, Color color = c_default, float grosor = 5, Coordenadas pos = coordenadas_default);

    void colors_random();
    void colors_chess(float r1, float g1, float b1, float r2, float g2, float b2);


    vector<_vertex3i> caras;
    vector<_vertex3f> colores_caras;
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

    _suelo(Coordenadas dimenisones = {100, 0.25, 100}, Coordenadas pos = coordenadas_default) : tam(dimenisones), posicion(pos), color_suelo(108, 136, 52) {}
    void draw(_modo modo, float grosor = 5); // , Coordenadas pos = coordenadas_default);
};

class _sol : public _triangulos3D
{
    const size_t resolucion = 100;
public:
    GLfloat radio;
    Coordenadas posicion;
    Color color_sol;

    _sol(GLfloat radio = 0.25, Coordenadas pos = coordenadas_default) : radio(radio), posicion(pos), color_sol(255, 205, 29) {};
    void draw(_modo modo, float grosor = 5); // , Coordenadas pos = coordenadas_default);, Coordenadas pos = coordenadas_default
};

class _nube : public _triangulos3D
{
public:
    Coordenadas posicion;

    _nube(Coordenadas pos = coordenadas_default) : posicion(pos) {}
    void draw(_modo modo, Color color, float grosor); // , Coordenadas pos = coordenadas_default);
};

class _lluvia : public _triangulos3D
{
public:
    Coordenadas posicion;

    _lluvia(Coordenadas pos = coordenadas_default) : posicion(pos) {}
    void draw(_modo modo, Color color, float grosor); // , Coordenadas pos = coordenadas_default);
};

class _viento : public _triangulos3D
{
public:
    Coordenadas posicion;

    _viento(Coordenadas pos = coordenadas_default) : posicion(pos) {}
    void draw(_modo modo, Color color, float grosor); // , Coordenadas pos = coordenadas_default);
};


class _helice_molino : public _triangulos3D
{
protected:
    _piramide aspa;
    _cilindro palo_central;
    _esfera bola_central;

public:
    static constexpr GLfloat largo_aspas = 2.0;
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
        color_helice(86, 43, 5)
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
    static constexpr GLfloat altura_casa = 1.0;
    static constexpr GLfloat radio_casa = 0.5;
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
    color_molino_casa(206, 219, 214),
    color_molino_tejado(86, 5, 5)
    {}
    void draw(_modo modo, float grosor = 5); // , Coordenadas pos = coordenadas_default);
};

class _petalo_girasol : public _triangulos3D
{
public:
    static constexpr GLfloat ancho = 0.7;
    static constexpr GLfloat largo = 1.0;
    Coordenadas punto_curva_1;
    Coordenadas punto_curva_2;
    Coordenadas posicion;
    Color color_petalo;

    _petalo_girasol(Coordenadas punto_curva_1 = {0.4f*ancho, 0.1f*largo} , Coordenadas punto_curva2 = {0.4f*ancho, 0.8f*largo}, Coordenadas pos = coordenadas_default, size_t resolucion = 30);
    //void draw(_modo modo, Color color, float grosor); // , Coordenadas pos = coordenadas_default);
};

class _hoja_girasol : public _triangulos3D
{
//protected :
//    _petalo_girasol hoja;

public:
    const GLfloat ancho = 0.7;
    const GLfloat largo = 1.0;
    Coordenadas punto_curva_1 = {0.68, 0.1, -0.2};
    Coordenadas punto_curva_2 = {0.28, 0.8, -0.1};
    Coordenadas posicion;
    Color color_hoja;

    _hoja_girasol(size_t resolucion = 30, Coordenadas pos = coordenadas_default)
    :
    posicion(pos), color_hoja(57, 96, 51)
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

    _tallo_girasol(Coordenadas pos = coordenadas_default) : posicion(pos), color_tallo(94, 112, 39) {}
    void draw(_modo modo, float grosor = 5); // , Coordenadas pos = coordenadas_default);
};
class _cabeza_girasol : public _triangulos3D
{
protected:

public:
    _hoja_girasol hoja;
    _petalo_girasol petalo;
    _tallo_girasol cuello;
    //_esfera semillero;
    GLfloat radio_semillero = petalo.largo*0.5;
    size_t num_petalos = 6;
    size_t num_hojitas = 12;
    size_t num_capas_hojas = 3;
    size_t num_capas_petalo = 3;
    Coordenadas posicion;
    Color color_semillero;

    _cabeza_girasol(Coordenadas pos = coordenadas_default)
    :
    posicion(pos), color_semillero(59, 46, 33)
    {}
    void draw(_modo modo, GLfloat grosor = 5); // , Coordenadas pos = coordenadas_default);

};

class _girasol : public _triangulos3D
{
private:
    vector<GLfloat> variacion_tam_rama;
    vector<GLfloat> angulo_rama;
    GLfloat longitud_min_rama;
    GLfloat angulo_min_rama = 15;
    GLfloat angulo_max_rama = 75;

protected:
    _tallo_girasol tallo;
    _cabeza_girasol cabeza;
    _hoja_girasol hoja;
    _petalo_girasol petalo;
public:
    size_t num_ramas = 5;
    Coordenadas posicion;

    _girasol(Coordenadas pos = coordenadas_default);
    void draw(_modo modo, float grosor = 5); // , Coordenadas pos = coordenadas_default);
};

class _escena_P3 : public _triangulos3D
{

protected:
    _suelo suelo;
    _sol sol;
    _molino molino;
    _girasol girasol;

public:
    Coordenadas posicion;
    
    _escena_P3(Coordenadas pos = coordenadas_default) : posicion(pos) {}
    void draw(_modo modo, float grosor = 5); // , Coordenadas pos = coordenadas_default);
};
#endif