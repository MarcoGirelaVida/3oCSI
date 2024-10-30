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
    void draw(_modo modo, Color color, float grosor);

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
    void parametros(vector<_vertex3f> perfil, size_t num, bool tapa_inferior, bool tapa_superior, int tipo);
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
    _piramide(float tam=0.5, float al=1.0);
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
    _esfera(float radio=1.0, size_t latitud=6, size_t longitud=6);
};


//----------------------------------------------------------------------
//----------------------------------------------------------------------
// PROYECTO PROPIO P3

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

class _escena_P3 : public _triangulos3D
{
public:
    Posicion posicion;
    
    _escena_P3(Posicion pos) : posicion(pos) {}
    void draw(_modo modo, Color color, float grosor);
};
class _suelo: public _triangulos3D
{
public:
    Dimensiones tam;

    _suelo(Dimensiones dimenisones) : tam(dimenisones) {}
    void draw(_modo modo, Color color, float grosor);
};

class _sol : public _triangulos3D
{
public:
    Posicion posicion;
    GLfloat radio;
    size_t resolucion;

    _sol(Posicion pos, GLfloat radio, size_t resolucion = 100) : posicion(pos), radio(radio), resolucion(resolucion) {};
    void draw(_modo modo, Color color, float grosor);
};

class _nube : public _triangulos3D
{
public:
    Posicion posicion;

    _nube(Posicion pos) : posicion(pos) {}
    void draw(_modo modo, Color color, float grosor);
};

class _lluvia : public _triangulos3D
{
public:
    Posicion posicion;

    _lluvia(Posicion pos) : posicion(pos) {}
    void draw(_modo modo, Color color, float grosor);
};

class _viento : public _triangulos3D
{
public:
    Posicion posicion;

    _viento(Posicion pos) : posicion(pos) {}
    void draw(_modo modo, Color color, float grosor);
};

class _molino : public _triangulos3D
{
public:
    Posicion posicion;

    _molino(Posicion pos) : posicion(pos) {}
    void draw(_modo modo, Color color, float grosor);
};

class _helice_molino : public _triangulos3D
{
public:
    Posicion posicion;

    _helice_molino(Posicion pos) : posicion(pos) {}
    void draw(_modo modo, Color color, float grosor);
};

class _girasol : public _triangulos3D
{
public:
    Posicion posicion;

    _girasol(Posicion pos) : posicion(pos) {}
    void draw(_modo modo, Color color, float grosor);
};

class _tallo_girasol : public _triangulos3D
{
public:
    Posicion posicion;

    _tallo_girasol(Posicion pos) : posicion(pos) {}
    void draw(_modo modo, Color color, float grosor);
};

class _cabeza_girasol : public _triangulos3D
{
public:
    Posicion posicion;

    _cabeza_girasol(Posicion pos) : posicion(pos) {}
    void draw(_modo modo, Color color, float grosor);
};

class _petalo_girasol : public _triangulos3D
{
public:
    Posicion posicion;

    _petalo_girasol(Posicion pos) : posicion(pos) {}
    void draw(_modo modo, Color color, float grosor);
};