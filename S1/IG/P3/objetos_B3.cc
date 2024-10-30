//**************************************************************************
// Práctica 3 
//**************************************************************************

#include "objetos_B3.h"
#include "file_ply_stl.hpp"
#include <time.h> 


//*************************************************************************
// _puntos3D
//*************************************************************************

_puntos3D::_puntos3D() {}

//*************************************************************************
// _triangulos3D
//*************************************************************************

_triangulos3D::_triangulos3D() {}

//*************************************************************************
// MODOS DE DIBUJO
//*************************************************************************

void _triangulos3D::draw(_modo modo, Color color, float grosor)
{
    switch (modo)
    {
        case POINTS:
            draw_puntos(color, grosor);
            break;
        case EDGES:
            draw_aristas(color, grosor);
            break;
        case SOLID:
            draw_solido(color);
            break;
        case SOLID_COLORS:
            draw_solido_colores();
            break;
    }
}

void _puntos3D::draw_puntos(Color color, int grosor)
{
    glPointSize(grosor);
    glColor3f(color.r, color.g, color.b);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
    glDrawArrays(GL_POINTS, 0, vertices.size());
}

void _triangulos3D::draw_aristas(Color color, int grosor)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(grosor);
    glColor3f(color.r, color.g, color.b);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
    glDrawElements(GL_TRIANGLES, caras.size()*3, GL_UNSIGNED_INT, &caras[0]);
}

void _triangulos3D::draw_solido(Color color)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(color.r, color.g, color.b);
    glBegin(GL_TRIANGLES);
        for (size_t i = 0; i < caras.size(); i++)
        {
            glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
            glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
            glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
        }
    glEnd();
}

void _triangulos3D::draw_solido_colores( )
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_TRIANGLES);
        for (size_t i = 0; i < caras.size(); i++)
        {
            glColor3f(colores_caras[i].r, colores_caras[i].g, colores_caras[i].b);
            glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
            glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
            glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
        }
    glEnd();
}

//*************************************************************************
// COLORES
//*************************************************************************

void _triangulos3D::colors_random()
{
    colores_caras.resize(caras.size());
    srand(time(NULL));
    for (size_t i = 0; i < caras.size(); i++)  
    {
        colores_caras[i].r = rand() % 1000 / 1000.0;
        colores_caras[i].g = rand() % 1000 / 1000.0;
        colores_caras[i].b = rand() % 1000 / 1000.0;
    }
}

void _triangulos3D::colors_chess(float r1, float g1, float b1, float r2, float g2, float b2)
{
    size_t n_c = caras.size();
    colores_caras.resize(n_c);
    for (size_t i = 0; i < n_c; i++)  
    {
        if (i % 2 == 0) 
        {
            colores_caras[i].r=r1;
            colores_caras[i].g=g1;
            colores_caras[i].b=b1;
        }
        else 
        {
            colores_caras[i].r=r2;
            colores_caras[i].g=g2;
            colores_caras[i].b=b2;
        } 
    }
}


//*************************************************************************
// OPERACIONES BÁSICAS
//*************************************************************************

//*************************************************************************
// clase objeto ply
//*************************************************************************

_objeto_ply::_objeto_ply() {}

void _objeto_ply::parametros(char *archivo)
{
    vector<float> ver_ply ;
    vector<int>   car_ply ;
    
    _file_ply::read(archivo, ver_ply, car_ply );

    size_t num_vertices = ver_ply.size() / 3;
    size_t num_caras = car_ply.size() / 3;

    printf("Number of vertices=%ld\nNumber of faces=%ld\n", num_vertices, num_caras);

    vertices.resize(num_vertices);
    caras.resize(num_caras);

    // vértices
    for (size_t i = 0; i < num_vertices; i++)
    {
        vertices[i].x = ver_ply[3 * i];
        vertices[i].y = ver_ply[3 * i + 1];
        vertices[i].z = ver_ply[3 * i + 2];
    }

    // vértices
    for (size_t i = 0; i < num_caras; i++)
    {
        caras[i].x = car_ply[3 * i];
        caras[i].y = car_ply[3 * i + 1];
        caras[i].z = car_ply[3 * i + 2];
    }

    colors_random();
}

//************************************************************************
// rotacion archivo PLY (caso especial de rotacion)
//************************************************************************

_rotacion_PLY::_rotacion_PLY()  {}

void _rotacion_PLY::parametros_PLY(char *archivo, size_t num)
{
    vector<_vertex3f> perfil;
    _vertex3f aux;

    vector<float> ver_ply ;
    vector<int>   car_ply ;
    
    _file_ply::read(archivo, ver_ply, car_ply );

    size_t num_vertices = ver_ply.size() / 3;
    size_t num_caras    = car_ply.size() / 3;

    printf("Number of vertices=%ld\nNumber of faces=%ld\n", num_vertices, num_caras);

    for (size_t i = 0; i < num_vertices; i++)
    {
        aux.x = ver_ply[ i * 3];
        aux.y = ver_ply[ i * 3 + 1];
        aux.z = ver_ply[ i * 3 + 2];
        perfil.push_back(aux);
    }
    cerr << "NUM: " << num << endl;
    parametros(perfil, num, false, false, 0);
}

//************************************************************************
// objeto por revolucion
//************************************************************************

_rotacion::_rotacion() {}

void _rotacion::parametros(vector<_vertex3f> perfil, size_t num, bool tapa_inferior, bool tapa_superior, int tipo)
{
    _vertex3f vertice_aux;
    _vertex3i cara_aux;
    size_t num_aux;
    float radio;

    // tratamiento de los vértice
    radio = sqrt(perfil[0].x * perfil[0].x + perfil[0].y * perfil[0].y);

    num_aux = perfil.size();
    if (tipo == 2) num_aux = 1;
    vertices.resize(num_aux * num);

    for (size_t j = 0; j < num; j++)
    {
        for (size_t i = 0; i < num_aux; i++)
        {
            vertice_aux.x = perfil[i].x * cos(2.0 * M_PI * j / (1.0 * num)) +
                            perfil[i].z * sin(2.0 * M_PI * j / (1.0 * num));
            vertice_aux.z = -perfil[i].x * sin(2.0 * M_PI * j / (1.0 * num)) +
                            perfil[i].z * cos(2.0 * M_PI * j / (1.0 * num));
            vertice_aux.y = perfil[i].y;
            vertices[i + j * num_aux] = vertice_aux;
        }
    }

    // tratamiento de las caras 
    if (tipo < 2)
    {
        for (size_t j = 0; j < num; j++)
        {
            for (size_t i = 0; i < num_aux - 1; i++)
            {
                cara_aux._0 = i + j * num_aux;
                cara_aux._1 = i + ((j + 1) % num) * num_aux;
                cara_aux._2 = i + 1 + j * num_aux;
                caras.push_back(cara_aux);

                cara_aux._0 = i + ((j + 1) % num) * num_aux;
                cara_aux._1 = i + 1 + ((j + 1) % num) * num_aux;
                cara_aux._2 = i + 1 + j * num_aux;
                caras.push_back(cara_aux);
            }
        }
    }
    
    // tapa inferior
    if (tapa_inferior)
    {
        // punto central de la tapa
        vertice_aux.x = 0.0;
        if (tipo == 1)
            vertice_aux.y = -radio;
        else
            vertice_aux.y = perfil[0].y;

        vertice_aux.z = 0.0;
        vertices.push_back(vertice_aux);

        // Conectamos el eje de la tapa a los puntos del perfil
        cara_aux._0 = num_aux * num;
        for (size_t i = 0; i < num; i++)
        {
            cara_aux._1 = i * num_aux;
            cara_aux._2 = ((i + 1) % num) * num_aux;
            caras.push_back(cara_aux);
        }
    }
    
    // tapa superior

    if (tapa_superior)
    {
        // punto central de la tapa
        vertice_aux.x = 0.0;
        vertice_aux.y = perfil[num_aux - 1].y;
        if (tipo == 1)
            vertice_aux.y = radio;
        if (tipo == 2)
            vertice_aux.y = perfil[1].y;
        vertice_aux.z = 0.0;
        vertices.push_back(vertice_aux);

        //caras tapa superior
        if (tapa_inferior)
            cara_aux._0 = num_aux * num + 1;
        else
            cara_aux._0 = num_aux * num;

        for (size_t i = 0; i < num; i++)
        {
            cara_aux._1 = i * num_aux + num_aux - 1;
            cara_aux._2 = ((i + 1) % num) * num_aux + num_aux - 1;
            caras.push_back(cara_aux);
        }
    }

    //colores de las caras
    colors_random();
}


//************************************************************************
// objeto por extrusión
//************************************************************************

_extrusion::_extrusion(vector<_vertex3f> poligono, float x, float y, float z)
{
    _vertex3f vertice_aux;
    _vertex3i cara_aux;

    // tratamiento de los vértice
    size_t num_aux=poligono.size();
    vertices.resize(num_aux * 2);
    for (size_t i = 0; i < num_aux; i++)
    {
      vertices[2 * i]       = poligono[i];
      vertices[2 * i + 1].x = poligono[i].x + x;
      vertices[2 * i + 1].y = poligono[i].y + y;
      vertices[2 * i + 1].z = poligono[i].z + z;
    }
    
    // tratamiento de las caras 
    caras.resize(num_aux * 2);
    size_t c=0;
    for (size_t i = 0; i < num_aux; i++)         
    {
        caras[c]._0 = i * 2;
        caras[c]._1 = (i * 2 + 2) % (num_aux * 2);
        caras[c]._2 = i * 2 + 1;    
        c++;
        caras[c]._0 = (i * 2 + 2) % (num_aux * 2);
        caras[c]._1 = (i * 2 + 2) % (num_aux * 2) + 1;
        caras[c]._2 = i * 2 + 1;    
        c++;    
    }  
   
    //colores de las caras
    colors_random();
}

//************************************************************************
// OBJETOS BÁSICOS
//************************************************************************

_cubo::_cubo(float tam)
{
    //vertices
    vertices.resize(8);
    vertices[0].x = -tam;     vertices[0].y = -tam;     vertices[0].z = tam;
    vertices[1].x = tam;      vertices[1].y = -tam;     vertices[1].z = tam;
    vertices[2].x = tam;      vertices[2].y = tam;      vertices[2].z = tam;
    vertices[3].x = -tam;     vertices[3].y = tam;      vertices[3].z = tam;
    vertices[4].x = -tam;     vertices[4].y = -tam;     vertices[4].z = -tam;
    vertices[5].x = tam;      vertices[5].y = -tam;     vertices[5].z = -tam;
    vertices[6].x = tam;      vertices[6].y = tam;      vertices[6].z = -tam;
    vertices[7].x = -tam;     vertices[7].y = tam;      vertices[7].z = -tam;

    // triangulos
    caras.resize(12);
    caras[0]._0 = 0;    caras[0]._1 = 1;    caras[0]._2 = 3;
    caras[1]._0 = 3;    caras[1]._1 = 1;    caras[1]._2 = 2;
    caras[2]._0 = 1;    caras[2]._1 = 5;    caras[2]._2 = 2;
    caras[3]._0 = 5;    caras[3]._1 = 6;    caras[3]._2 = 2;
    caras[4]._0 = 5;    caras[4]._1 = 4;    caras[4]._2 = 6;
    caras[5]._0 = 4;    caras[5]._1 = 7;    caras[5]._2 = 6;
    caras[6]._0 = 0;    caras[6]._1 = 7;    caras[6]._2 = 4;
    caras[7]._0 = 0;    caras[7]._1 = 3;    caras[7]._2 = 7;
    caras[8]._0 = 3;    caras[8]._1 = 2;    caras[8]._2 = 7;
    caras[9]._0 = 2;    caras[9]._1 = 6;    caras[9]._2 = 7;
    caras[10]._0 =0;    caras[10]._1 =1;    caras[10]._2= 4;
    caras[11]._0 =1;    caras[11]._1 =5;    caras[11]._2= 4; 

    //colores de las caras
    colors_random();
}

_piramide::_piramide(float tam, float al)
{
    //vertices 
    vertices.resize(5); 
    vertices[0].x = -tam; vertices[0].y = 0;    vertices[0].z = tam;
    vertices[1].x = tam;  vertices[1].y = 0;    vertices[1].z = tam;
    vertices[2].x = tam;  vertices[2].y = 0;    vertices[2].z = -tam;
    vertices[3].x = -tam; vertices[3].y = 0;    vertices[3].z = -tam;
    vertices[4].x = 0;    vertices[4].y = al;   vertices[4].z = 0;

    //caras
    caras.resize(6);
    caras[0]._0 = 0;  caras[0]._1 = 1;  caras[0]._2 = 4;
    caras[1]._0 = 1;  caras[1]._1 = 2;  caras[1]._2 = 4;
    caras[2]._0 = 2;  caras[2]._1 = 3;  caras[2]._2 = 4;
    caras[3]._0 = 3;  caras[3]._1 = 0;  caras[3]._2 = 4;
    caras[4]._0 = 3;  caras[4]._1 = 1;  caras[4]._2 = 0;
    caras[5]._0 = 3;  caras[5]._1 = 2;  caras[5]._2 = 1;

    //colores de las caras
    colors_random();
}

_cilindro::_cilindro(float radio, float altura, size_t num)
{
    vector<_vertex3f> perfil;
    _vertex3f aux;

    aux.x = radio; aux.y = -altura / 2.0;   aux.z = 0.0;
    perfil.push_back(aux);
    aux.x = radio; aux.y = altura / 2.0;    aux.z = 0.0;
    perfil.push_back(aux);

    parametros(perfil, num, true, true, 0);
}

_cono::_cono(float radio, float altura, size_t num)
{
    vector<_vertex3f> perfil;
    _vertex3f aux;

    aux.x = radio;  aux.y = -altura/2.0;    aux.z = 0.0;
    perfil.push_back(aux);
    aux.x = 0.0;    aux.y = altura;         aux.z = 0.0;
    perfil.push_back(aux);

    parametros(perfil, num, true, true, 2);
}

_esfera::_esfera(float radio, size_t latitud, size_t longitud)
{
    vector<_vertex3f> perfil;
    _vertex3f aux;
    for (size_t i = 1; i < latitud; i++)
    {
        aux.x = radio * cos(M_PI * i / (latitud * 1.0) - M_PI / 2.0);
        aux.y = radio * sin(M_PI * i / (latitud * 1.0) - M_PI / 2.0);
        aux.z = 0.0;
        perfil.push_back(aux);
    }
    parametros(perfil, longitud, true, true, 1);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

// PRACTICA 3 PROPIA

//************************************************************************
// NATURALEZA
//************************************************************************
//_suelo::_suelo(Posicion pos) : posicion(pos) {}
void _suelo::draw(_modo modo, Color color, float grosor)
{
    glPushMatrix();
        glTranslatef(0, -(tam.y/2), 0);
        glScalef(tam.x, tam.y, tam.z);
        _cubo suelo;
        suelo.draw(modo, color, grosor);
    glPopMatrix();
}

//_sol::_sol(GLfloat) {}
void _sol::draw(_modo modo, Color color, float grosor)
{
    glPushMatrix();
        glTranslatef(posicion.x, posicion.y, posicion.z);
        _esfera sol(radio, resolucion, resolucion);
        sol.draw(modo, color, grosor);
    glPopMatrix();
}

//_nube::_nube(Posicion pos) : posicion(pos) {}
void _nube::draw(_modo modo, Color color, float grosor)
{}

//_lluvia::_lluvia(Posicion pos) : posicion(pos) {}
void _lluvia::draw(_modo modo, Color color, float grosor)
{}

//_viento::_viento(Posicion pos) : posicion(pos) {}
void _viento::draw(_modo modo, Color color, float grosor)
{}

//************************************************************************
// GIRASOL
//************************************************************************
//_tallo_girasol::_tallo_girasol(Posicion pos) : posicion(pos) {}
void _tallo_girasol::draw(_modo modo, Color color, float grosor)
{}

//_petalo_girasol::_petalo_girasol(Posicion pos) : posicion(pos) {}
void _petalo_girasol::draw(_modo modo, Color color, float grosor)
{}

//_cabeza_girasol::_cabeza_girasol(Posicion pos) : posicion(pos) {}
void _cabeza_girasol::draw(_modo modo, Color color, float grosor)
{}

//_girasol::_girasol(Posicion pos) : posicion(pos) {}
void _girasol::draw(_modo modo, Color color, float grosor)
{}

//************************************************************************
// MOLINO
//************************************************************************
//_helice_molino::_helice_molino(Posicion pos) : posicion(pos) {}
void _helice_molino::draw(_modo modo, Color color, float grosor)
{}

//_molino::_molino(Posicion pos) : posicion(pos) {}
void _molino::draw(_modo modo, Color color, float grosor)
{}

//************************************************************************
// BASE
//************************************************************************
//_escena_P3::_escena_P3(Posicion pos) : posicion(pos) {}
void _escena_P3::draw(_modo modo, Color color, float grosor)
{}