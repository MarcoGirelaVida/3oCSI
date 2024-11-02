#ifndef OBJETOS_B3_H
#define OBJETOS_B3_H

#include <vector>
#include <GL/gl.h>
#include "vertex.h"
#include <stdlib.h>

const float AXIS_SIZE=5000;
typedef enum{POINTS,EDGES,SOLID,SOLID_COLORS} _modo;

struct Color {
   GLfloat r;
   GLfloat g;
   GLfloat b;
   GLfloat a = 1.0;
   GLfloat r2 = 1.0;
   GLfloat g2 = 1.0;
   GLfloat b2 = 1.0;
   GLfloat a2 = 1.0;

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

    void operator+(Color c)
    {
        r += c.r;
        g += c.g;
        b += c.b;
    }
    void operator+(GLfloat f)
    {
        r += f;
        g += f;
        b += f;
    }

    void actualizar(float grado)
    {
        r += (r2-r)*grado;
        g += (g2-g)*grado;
        b += (b2-b)*grado;
    }
};

struct Dimensiones
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

struct Posicion
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

const Posicion pos_default = {0.0, 0.0, 0.0};

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
    void draw(_modo modo, Color color, float grosor); // , Posicion pos = pos_default);

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
    void parametros(vector<_vertex3f> perfil, size_t num, bool tapa_inferior, bool tapa_superior, int tipo, unsigned char porcentaje_revolucion = 100, unsigned char porcentaje_generacion_esfera = 100);
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
    Dimensiones tam;
    Posicion posicion;
    _piramide(Dimensiones tam);
};
class _cilindro: public _rotacion
{
public:
    _cilindro(float radio=1.0, float altura=2.0, size_t num=12);
};

class _cono: public _rotacion
{
public:
    _cono(float radio=1.0, float altura=2.0, size_t num=12);
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

class _escena_P3 : public _triangulos3D
{
public:
    Posicion posicion;
    
    _escena_P3(Posicion pos = pos_default) : posicion(pos) {}
    void draw(_modo modo, Color color, float grosor); // , Posicion pos = pos_default);
};
class _suelo: public _triangulos3D
{
public:
    Dimensiones tam;
    Posicion posicion;
    Color color_suelo;

    _suelo(Dimensiones dimenisones, Posicion pos = pos_default) : tam(dimenisones), posicion(pos), color_suelo(0, 255, 0) {}
    void draw(_modo modo, Color color, float grosor); // , Posicion pos = pos_default);
};

class _sol : public _triangulos3D
{
    const size_t resolucion = 100;
public:
    GLfloat radio;
    Posicion posicion;
    Color color_sol;

    _sol(GLfloat radio, Posicion pos = pos_default) : radio(radio), posicion(pos), color_sol(1, 1, 0) {};
    void draw(_modo modo, Color color, float grosor); // , Posicion pos = pos_default);, Posicion pos = pos_default
};

class _nube : public _triangulos3D
{
public:
    Posicion posicion;

    _nube(Posicion pos = pos_default) : posicion(pos) {}
    void draw(_modo modo, Color color, float grosor); // , Posicion pos = pos_default);
};

class _lluvia : public _triangulos3D
{
public:
    Posicion posicion;

    _lluvia(Posicion pos = pos_default) : posicion(pos) {}
    void draw(_modo modo, Color color, float grosor); // , Posicion pos = pos_default);
};

class _viento : public _triangulos3D
{
public:
    Posicion posicion;

    _viento(Posicion pos = pos_default) : posicion(pos) {}
    void draw(_modo modo, Color color, float grosor); // , Posicion pos = pos_default);
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
    Posicion posicion;
    Color color_helice;

    _helice_molino(Posicion pos = pos_default)
        :
        aspa({ancho_aspas, grosor_aspas, largo_aspas}),
        palo_central(radio_palo_central, largo_palo_central, 30),
        bola_central(radio_bola_central, 30, true, true, 50, 100),
        posicion(pos),
        color_helice(86, 43, 5)
        {}
    void draw(_modo modo, Color color, float grosor); // , Posicion pos = pos_default);
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
    Posicion posicion;
    Color color_molino_casa;
    Color color_molino_tejado;

    _molino(Posicion pos = pos_default)
    :
    tejado(radio_tejado, altura_tejado),
    casa(radio_casa, altura_casa, 30),
    posicion(pos),
    color_molino_casa(206, 219, 214),
    color_molino_tejado(86, 5, 5)
    {}
    void draw(_modo modo, Color color, float grosor); // , Posicion pos = pos_default);
};

class _girasol : public _triangulos3D
{
public:
    Posicion posicion;

    _girasol(Posicion pos = pos_default) : posicion(pos) {}
    void draw(_modo modo, Color color, float grosor); // , Posicion pos = pos_default);
};

class _tallo_girasol : public _triangulos3D
{
public:
    Posicion posicion;

    _tallo_girasol(Posicion pos = pos_default) : posicion(pos) {}
    void draw(_modo modo, Color color, float grosor); // , Posicion pos = pos_default);
};

class _cabeza_girasol : public _triangulos3D
{
public:
    Posicion posicion;

    _cabeza_girasol(Posicion pos = pos_default) : posicion(pos) {}
    void draw(_modo modo, Color color, float grosor); // , Posicion pos = pos_default);
};

class _petalo_girasol : public _triangulos3D
{
public:
    Posicion posicion;

    _petalo_girasol(Posicion pos = pos_default) : posicion(pos) {}
    void draw(_modo modo, Color color, float grosor); // , Posicion pos = pos_default);
};

#endif