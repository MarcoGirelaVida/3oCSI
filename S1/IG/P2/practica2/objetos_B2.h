//**************************************************************************
// Práctica 2 
//**************************************************************************

#include <vector>
#include <GL/gl.h>
#include "vertex.h"
#include <stdlib.h>


const float AXIS_SIZE=5000;
typedef enum{POINTS,EDGES,SOLID,SOLID_COLORS} _modo;

//*************************************************************************
// clase punto
//*************************************************************************

class _puntos3D
{
public:

  
	_puntos3D();
void 	draw_puntos(float r, float g, float b, int grosor);

vector<_vertex3f> vertices;
};

//*************************************************************************
// clase triángulo
//*************************************************************************

class _triangulos3D: public _puntos3D
{
public:

	_triangulos3D();
void 	draw_aristas(float r, float g, float b, int grosor);
void    draw_solido(float r, float g, float b);
void 	draw_solido_colores();
void 	draw(_modo modo, float r, float g, float b, float grosor);
void 	asignar_gama_verdes();
void	asignar_colores_aleatorios();

vector<_vertex3i> caras;
vector<_vertex3f> c_caras;
};


//*************************************************************************
// clase cubo
//*************************************************************************

class _cubo: public _triangulos3D
{
public:

	_cubo(float tam=0.5);
};


//*************************************************************************
// clase piramide
//*************************************************************************

class _piramide: public _triangulos3D
{
public:

	_piramide(float tam=0.5, float al=1.0);
};

//*************************************************************************
// clase objeto ply
//*************************************************************************

class _objeto_ply: public _triangulos3D
{
public:
   _objeto_ply();

void  parametros(char *archivo);
};

//************************************************************************
// objeto por revolución
//************************************************************************

class _rotacion: public _triangulos3D
{
public:
       _rotacion();
       
void  parametros(vector<_vertex3f> perfil, int num, bool tapa_in, bool tapa_su, int tipo);
// tapa_in = 0, isn tapa superior, tapa_in = 1, con tapa superior
// idem para tapa_su
// tipo 0 = revolución normal
// tipo 1 = esfera
// tipo 2 = cono
};

//************************************************************************
// objeto por revolucion casos particulares
//************************************************************************
class _esfera: public _rotacion
{
public: _esfera(float radio= 0.5, int latitud=10, int longitud=10);
};
class _cono: public _rotacion
{
public: _cono(float radio=1.0, float altura=2.0, int num=12);
};
class _cilindro: public _rotacion
{
public: _cilindro(float radio=1.0, float altura=2.0, int num=12);
};

class _rotacion_PLY: public _rotacion
{
public: _rotacion_PLY();

void parametros_PLY(char *archivo, int num = 10);
};
//************************************************************************
// objeto por extrusión
//************************************************************************

class _extrusion: public _triangulos3D
{
public:
       _extrusion(vector<_vertex3f> poligono, float x, float y, float z);
};

