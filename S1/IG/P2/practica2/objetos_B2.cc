//**************************************************************************
// Práctica 2 
//**************************************************************************
// SE PUEDE OBTENER UN OBJETO PLY CON BLENDER
#include "objetos_B2.h"
#include "file_ply_stl.hpp"


//*************************************************************************
// _puntos3D
//*************************************************************************

_puntos3D::_puntos3D()
{
}

//*************************************************************************
// dibujar puntos
//*************************************************************************

void _puntos3D::draw_puntos(float r, float g, float b, int grosor)
{
int i;
glPointSize(grosor);
glColor3f(r,g,b);
glBegin(GL_POINTS);
for (i=0;i<vertices.size();i++){
	glVertex3fv((GLfloat *) &vertices[i]);
	}
glEnd();
}


//*************************************************************************
// _triangulos3D
//*************************************************************************

_triangulos3D::_triangulos3D()
{
}


//*************************************************************************
// dibujar en modo arista
//*************************************************************************

void _triangulos3D::draw_aristas(float r, float g, float b, int grosor)
{
int i;
glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
glLineWidth(grosor);
glColor3f(r,g,b);
glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();
}

//*************************************************************************
// dibujar en modo sólido con un único color
//*************************************************************************

void _triangulos3D::draw_solido(float r, float g, float b)
{
	int i;
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glColor3f(r,g,b);
	glBegin(GL_TRIANGLES);
	for (i=0;i<caras.size();i++){
		glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
		glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
		glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
		}
	glEnd();
}

//*************************************************************************
// dibujar en modo sólido con colores diferentes para cada cara
//*************************************************************************

void _triangulos3D::draw_solido_colores()
{
	int i;
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	//asignar_gama_verdes();
	glBegin(GL_TRIANGLES);
	for (i=0;i<caras.size();i++){
		glColor3f(c_caras[i].r,c_caras[i].g,c_caras[i].b);
		glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
		glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
		glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
		}
	glEnd();
}

void _triangulos3D::asignar_gama_verdes()
{
	c_caras.resize(caras.size());
	for (size_t i = 0; i < caras.size(); i++)
	{
		c_caras[i].r = 0.0;
		c_caras[i].g = 0.8/caras.size()*i+0.2;
		c_caras[i].b = 0.0;
	}
}

void _triangulos3D::asignar_colores_aleatorios()
{
	c_caras.resize(caras.size());
	for (size_t i = 0; i < caras.size(); i++)
	{
		c_caras[i].r = rand() % 1000 / 1000.0;
		c_caras[i].g = rand() % 1000 / 1000.0;
		c_caras[i].b = rand() % 1000 / 1000.0;
	}
	
}

void _triangulos3D::asignar_colores_triangulos()
{
    // Si la componente y del primer vértice de la cara es negativa se ponen en magenta y sion en color oro
    c_caras.resize(caras.size());
    for (size_t i = 0; i < caras.size(); i++)  
    {
        if (vertices[caras[i]._0].y < 0)
        {
            c_caras[i].r = 139.0 / 255.0;
            c_caras[i].g = 0.0;
            c_caras[i].b = 139.0 / 255.0;
        }
        else
        {
            c_caras[i].r = 255.0 / 255.0;
            c_caras[i].g = 215.0 / 255.0;
            c_caras[i].b = 0.0;
        }
    }
}
//*************************************************************************
// dibujar con distintos modos
//*************************************************************************

void _triangulos3D::draw(_modo modo, float r, float g, float b, float grosor)
{
switch (modo){
	case POINTS:draw_puntos(r, g, b, grosor);break;
	case EDGES:draw_aristas(r, g, b, grosor);break;
	case SOLID:draw_solido(r, g, b);break;
	case SOLID_COLORS:draw_solido_colores();break;
	}
}

//*************************************************************************
// clase cubo
//*************************************************************************

_cubo::_cubo(float tam)
{
	//vertices 
	vertices.resize(9); 
	vertices[0].x = 0;	vertices[0].y = 0;	vertices[0].z = 0;
	vertices[1].x = 0;	vertices[1].y = 0;	vertices[1].z = tam; 	// Z
	vertices[2].x = tam;vertices[2].y = 0;	vertices[2].z = tam; 	// X Z
	vertices[3].x = tam;vertices[3].y = 0;	vertices[3].z = 0; 		// X
	vertices[4].x = 0;	vertices[4].y = 0;	vertices[4].z = 0; 		// origen
	vertices[5].x = 0;	vertices[5].y = tam;vertices[5].z = tam; 	// Y y Z
	vertices[6].x = tam;vertices[6].y = tam;vertices[6].z = tam; 	// X, Y, Z;
	vertices[7].x = tam;vertices[7].y = tam;vertices[7].z = 0;		// x e y
	vertices[8].x = 0;	vertices[8].y = tam;vertices[8].z = 0;		// Solo Y

	// triangulos
	caras.resize(12);
	caras[0]._0 = 1;	caras[0]._1 = 2;	caras[0]._2 = 5;
	caras[1]._0 = 2;	caras[1]._1 = 6;	caras[1]._2 = 5;
	caras[2]._0 = 2;	caras[2]._1 = 3;	caras[2]._2 = 6;
	caras[3]._0 = 3;	caras[3]._1 = 7;	caras[3]._2 = 6;
	caras[4]._0 = 3;	caras[4]._1 = 4;	caras[4]._2 = 7;
	caras[5]._0 = 4;	caras[5]._1 = 8;	caras[5]._2 = 7;
	caras[6]._0 = 4;	caras[6]._1 = 1;	caras[6]._2 = 8;
	caras[7]._0 = 1;	caras[7]._1 = 5;	caras[7]._2 = 8;
	caras[8]._0 = 5;	caras[8]._1 = 6;	caras[8]._2 = 7;
	caras[9]._0 = 5;	caras[9]._1 = 7;	caras[9]._2 = 8;
	caras[10]._0 = 4;	caras[10]._1 = 2;	caras[10]._2 = 1;
	caras[11]._0 = 4;	caras[11]._1 = 3;	caras[11]._2 = 2;

  asignar_colores_aleatorios();
}


//*************************************************************************
// clase piramide
//*************************************************************************

_piramide::_piramide(float tam, float al)
{
	//vertices 
	vertices.resize(5); 
	vertices[0].x=-tam;vertices[0].y=0;vertices[0].z=tam;
	vertices[1].x=tam;vertices[1].y=0;vertices[1].z=tam;
	vertices[2].x=tam;vertices[2].y=0;vertices[2].z=-tam;
	vertices[3].x=-tam;vertices[3].y=0;vertices[3].z=-tam;
	vertices[4].x=0;vertices[4].y=al;vertices[4].z=0;

	caras.resize(6);
	caras[0]._0=0;caras[0]._1=1;caras[0]._2=4;
	caras[1]._0=1;caras[1]._1=2;caras[1]._2=4;
	caras[2]._0=2;caras[2]._1=3;caras[2]._2=4;
	caras[3]._0=3;caras[3]._1=0;caras[3]._2=4;
	caras[4]._0=3;caras[4]._1=1;caras[4]._2=0;
	caras[5]._0=3;caras[5]._1=2;caras[5]._2=1;

  asignar_colores_aleatorios();
}


//*************************************************************************
// clase esfera
//*************************************************************************

_esfera::_esfera(float radio, int latitud, int longitud)
{
  vector<_vertex3f> perfil;
  _vertex3f aux;
  // Empiezo en 1 porque si empezas en 0 se repetira el primer punto tantas veces 
  // como lados (latitudes) tenga la esfera
  for (int i = 1; i < latitud; i++)
  {
	// -M_PI/2, es el lugar por el que empieza para que empiece en el polo sur
	aux.x = radio*cos(M_PI*i/(latitud*1.0)-M_PI/2.0);
	aux.y = radio*sin(M_PI*i/(latitud*1.0)-M_PI/2.0);
	aux.z = 0;
	perfil.push_back(aux);
  }
  parametros(perfil, longitud, true, true, 1);
}

//*************************************************************************
// clase cilindro
//*************************************************************************

_cilindro::_cilindro(float radio, float altura, int num)
{
  vector<_vertex3f> perfil;
  _vertex3f aux;
  aux.x = radio; aux.y = -altura/2.0; aux.z = 0.0;
  perfil.push_back(aux);
  aux.x = radio; aux.y = altura/2.0; aux.z = 0.0;
  perfil.push_back(aux);
  parametros(perfil, num, true, true, 0);
  //for (int i = 0; i < num; i++)
  //{
//	aux.x = radio*cos(2*M_PI*i/(num-1));
//	aux.y = radio*sin(2*M_PI*i/(num-1));
//	aux.z = 0;
//	perfil.push_back(aux);
  //}
  //parametros(perfil, num, true, true, 0);
}

//*************************************************************************
// clase cono
//*************************************************************************

_cono::_cono(float radio, float altura, int num)
{
  vector<_vertex3f> perfil;
  _vertex3f aux;
  aux.x = radio; aux.y = -altura/2.0; aux.z = 0.0;
  perfil.push_back(aux);
  aux.x = 0.0; aux.y = altura/2.0; aux.z = 0.0;
  perfil.push_back(aux);
  parametros(perfil, num, true, true, 2);
}

//*************************************************************************
// clase objeto ply
//*************************************************************************


_objeto_ply::_objeto_ply() 
{
   // leer lista de coordenadas de vértices y lista de indices de vértices
 
}



void _objeto_ply::parametros(char *archivo)
{
int n_ver,n_car;

vector<float> ver_ply ;
vector<int>   car_ply ;
 
_file_ply::read(archivo, ver_ply, car_ply );

n_ver=ver_ply.size()/3;
n_car=car_ply.size()/3;

printf("Number of vertices=%d\nNumber of faces=%d\n", n_ver, n_car);

vertices.resize(n_ver);
caras.resize(n_car);

for (int i = 0; i < n_ver; i++)
{
  vertices[i].x=ver_ply[i*3];
  vertices[i].y=ver_ply[i*3+1];
  vertices[i].z=ver_ply[i*3+2];
}

for (int i = 0; i < n_car; i++)
{
  caras[i].x=car_ply[i*3];
  caras[i].y=car_ply[i*3+1];
  caras[i].z=car_ply[i*3+2];
}
asignar_colores_triangulos();
}

//************************************************************************
// retacion archivo PLY
//************************************************************************

_rotacion_PLY::_rotacion_PLY()
{
	// Leer lista de coordenadas para un perfil en un archivo PLY
}

void _rotacion_PLY::parametros_PLY(char * archivo, int num)
{
int n_ver,n_car;

vector<_vertex3f> perfil;
_vertex3f aux;

vector<float> ver_ply ;
vector<int>   car_ply ;
 
_file_ply::read(archivo, ver_ply, car_ply );

n_ver=ver_ply.size()/3;
n_car=car_ply.size()/3;

printf("Number of vertices=%d\nNumber of faces=%d\n", n_ver, n_car);

for (int i = 0; i < n_ver; i++)
{
  aux.x=ver_ply[i*3];
  aux.y=ver_ply[i*3+1];
  aux.z=ver_ply[i*3+2];
  perfil.push_back(aux);
}
cerr << "NUM: " << num << endl;
parametros(perfil, num, false, false, 0);
}
//************************************************************************
// objeto por revolucion
//************************************************************************

_rotacion::_rotacion()
{

}


void _rotacion::parametros(vector<_vertex3f> perfil, int num, bool tapa_in, bool tapa_su, int tipo)
{
int i,j;
_vertex3f vertice_aux;
_vertex3i cara_aux;
int num_aux;

// tratamiento de los vértice
float radio = sqrt(perfil[0].x*perfil[0].x+perfil[0].y*perfil[0].y); // pitágoras de toda la vida, z = raiz de (x^2 + y^2)
num_aux=perfil.size(); 			// Número de puntos del perfil
if (tipo==2) num_aux = 1; // Si es un cono, solo tiene un punto en el perfil
vertices.resize(num_aux*num);
for (j=0;j<num;j++) // j es el perfil en el que estamos
  {for (i=0;i<num_aux;i++)	// i es el punto del perfil en el que estamos
     {
      vertice_aux.x = perfil[i].x*cos(2.0*M_PI*j/(1.0*num)) + perfil[i].z*sin(2.0*M_PI*j/(1.0*num)); // j en este caso representa
      vertice_aux.z = -perfil[i].x*sin(2.0*M_PI*j/(1.0*num)) + perfil[i].z*cos(2.0*M_PI*j/(1.0*num)); // roto el punto en el eje z
      vertice_aux.y = perfil[i].y; // La y es constante siempre
      vertices[i+j*num_aux] = vertice_aux;
     }
  }

// tratamiento de las caras 
caras.resize((num_aux-1)*2*num); // Lo que está antes que num es el tamaño del perfil
for (j=0; j < num; j++) // Cada perfil
{
  for (size_t i = 0; i < num_aux-1; i++)
  {
    //caras[2*i + j*(num_aux-1)*2]._0= j*num_aux + i;
    //caras[2*i + j*(num_aux-1)*2]._1= j*num_aux + i + num_aux; // el que es paralelo pero del siguiente perfil
    //caras[2*i + j*(num_aux-1)*2]._2= j*num_aux + i + 1; // el que está arriba
    //caras[2*i+1 + j*(num_aux-1)*2]._0= j*num_aux + i + num_aux;
    //caras[2*i+1 + j*(num_aux-1)*2]._1= j*num_aux + i + num_aux + 1;
    //caras[2*i+1 + j*(num_aux-1)*2]._2= j*num_aux + i + 1;
	// caras.push_back(cara_aux);

    caras[2*i + j*(num_aux-1)*2]._0 = j*num_aux + i;
    caras[2*i + j*(num_aux-1)*2]._1 = ((j+1)%num)*num_aux + i;
    caras[2*i + j*(num_aux-1)*2]._2 = j*num_aux + i+1;
    caras[2*i+1 + j*(num_aux-1)*2]._0 = ((j+1)%num)*num_aux + i;
    caras[2*i+1 + j*(num_aux-1)*2]._1 = ((j+1)%num)*num_aux + i+1;
    caras[2*i+1 + j*(num_aux-1)*2]._2 = j*num_aux + i+1;
  }
}

// tapa inferior

if (tapa_in == 1)
{
	_vertex3f vert_aux;
	vert_aux.x = 0.0, vert_aux.z = 0.0;
	if (tipo == 1) vert_aux.y = -radio; // si es una esfera
	else vert_aux.y = perfil[0].y;	// si es una figura normal o un cono
	vertices.push_back(vert_aux);

	// Conectamos el eje de la tapa a los puntos del perfil
	cara_aux._0 = num_aux*num;
	for (int i = 0; i < num; i++)
	{
		cara_aux._1 = i*num_aux;
		cara_aux._2 = ((i+1)%num)*num_aux;
		caras.push_back(cara_aux);
	}
}
 

// tapa superior
if (tapa_su == 1)
{
	_vertex3f vert_aux;
	// punto central de la tapa
	vert_aux.x = 0.0;
	vert_aux.y = perfil[num_aux - 1].y;
	if (tipo == 1) vert_aux.y = radio;
	if (tipo == 2) vert_aux.y = perfil[1].y;
	vert_aux.z = 0.0;
	vertices.push_back (vert_aux);

	//caras tapa superior
	if (tapa_in == 1) cara_aux._0 = num_aux*num+1;
	else cara_aux._0 = num_aux*num;

	for (int i = 0; i < num; i++)
	{
		cara_aux._1 = i*num_aux + num_aux - 1;
		cara_aux._2 = ((i+1)%num)*num_aux + num_aux - 1;
		caras.push_back(cara_aux);
	}
}

asignar_colores_aleatorios();
}

//************************************************************************
// objeto por extrusión
//************************************************************************


_extrusion::_extrusion(vector<_vertex3f> poligono, float x, float y, float z)
{
int i;
_vertex3f vertice_aux;
_vertex3i cara_aux;
int num_aux;

// tratamiento de los vértice

num_aux=poligono.size();
vertices.resize(num_aux*2);
for (i=0;i<num_aux;i++)
    {
      vertices[2*i]=poligono[i];
      vertices[2*i+1].x=poligono[i].x+x;
      vertices[2*i+1].y=poligono[i].y+y;
      vertices[2*i+1].z=poligono[i].z+z;
    }
    
// tratamiento de las caras 

caras.resize(num_aux*2);
int c=0;
for (i=0;i<num_aux;i++)         
  {
   caras[c]._0=i*2;
   caras[c]._1=(i*2+2)%(num_aux*2);
   caras[c]._2=i*2+1;    
   c=c+1;
   caras[c]._0=(i*2+2)%(num_aux*2);
   caras[c]._1=(i*2+2)%(num_aux*2)+1;
   caras[c]._2=i*2+1;    
   c=c+1;    
      
   }  

asignar_colores_aleatorios(); 
}

// OBJETOS EXAMEN
_copa::_copa(size_t resolucion)
{
    GLfloat radio = 1.0;
    GLfloat desplazamiento_x = 0.5;
    GLfloat desplazamiento_y = 1.0; // Ajustado para que haya 1 de desplazamiento en y
    vector<_vertex3f> perfil;
    _vertex3f aux;
    GLfloat angulo = (M_PI / 2.0) / (float) resolucion;  // Dividimos el cuarto de círculo
    // Semi esfera inferior (cuarto de esfera mirando hacia abajo)
    for (int i = 0; i <= resolucion; i++) {
        aux.x = desplazamiento_x + radio * cos(-M_PI / 2.0 + angulo * i); // Angulo desde el ecuador hacia el polo sur
        aux.y = desplazamiento_y + radio*2 + radio * sin(-M_PI / 2.0 + angulo * i); // Empezando en y=-1 (abajo)
        aux.z = 0;
        perfil.push_back(aux);
    }

    // Semi esfera superior (cuarto de esfera mirando hacia arriba)
    for (int i = 0; i <= resolucion; i++) {
        aux.x = desplazamiento_x + radio * cos(M_PI / 2.0 - angulo * i); // Angulo desde el polo sur hacia el ecuador
        aux.y =  radio * sin(M_PI / 2.0 - angulo * i); // Empezando en y=1 (arriba)
        aux.z = 0;
        perfil.push_back(aux);
    }


    parametros(perfil, resolucion, true, true, 0);
}


void _martillo::draw(_modo modo, float grosor)
{
	glPushMatrix();
		// Pongo el cubo pequeño centrado en su esquina
		cubo_base.draw(modo, color_r, color_g, color_b, grosor);
		glPushMatrix();
			glTranslatef(tam_cubo_base-tam_cubo_esquina*0.5, tam_cubo_base, tam_cubo_base-tam_cubo_esquina*0.5);
			glRotatef(ANGULO_CUBO, 0, 1, 0);
			cubo_esquina.draw(modo, color_r, color_g, color_b, grosor);
			glPushMatrix();
				glTranslatef(tam_cubo_esquina-radio_cilindro_y, tam_cubo_esquina, tam_cubo_esquina-radio_cilindro_y);
				glRotatef(-ANGULO_MARTILLO, 1, 0, 0);
				// Cilindro vertical
				glPushMatrix();
					glTranslatef (0, longitud_cilindro_y*0.5, 0);
					cilindro_y.draw(modo, color_r, color_g, color_b, grosor);
				glPopMatrix();
				// Cilindro tumbado
				glPushMatrix();
					glTranslatef(0, longitud_cilindro_y+radio_cilindro_z, 0);
					glRotatef(90, 0, 0, 1);
					cilindro_z.draw(modo, color_r, color_g, color_b, grosor);
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}