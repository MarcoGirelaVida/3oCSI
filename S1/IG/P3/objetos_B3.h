#include <vector>
#include <GL/gl.h>
#include "vertex.h"
#include <stdlib.h>

const float AXIS_SIZE=5000;
typedef enum{POINTS,EDGES,SOLID,SOLID_COLORS} _modo;

//*************************************************************************
// BASE DE TODO
//*************************************************************************
class _puntos3D
{
public:
    _puntos3D();
    void draw_puntos(float r, float g, float b, int grosor);
    vector<_vertex3f> vertices;
    vector<_vertex3f> colores_vertices;
};

class _triangulos3D: public _puntos3D
{
public:

    _triangulos3D();
    void draw_aristas(float r, float g, float b, int grosor);
    void draw_solido(float r, float g, float b);
    void draw_solido_colores();
    void draw(_modo modo, float r, float g, float b, float grosor);

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
    void parametros(vector<_vertex3f> perfil, int num, bool tapa_inferior, bool tapa_superior, int tipo);
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
    void  parametros_PLY(char *archivo, int num);
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
    _cilindro(float radio=1.0, float altura=2.0, int num=12);
};

class _cono: public _rotacion
{
public:
    _cono(float radio=1.0, float altura=2.0, int num=12);
};

class _esfera: public _rotacion
{
public:
    _esfera(float radio=1.0, int latitud=6, int longitud=6);
};


//----------------------------------------------------------------------
//----------------------------------------------------------------------
// PROYECTO PROPIO P3

