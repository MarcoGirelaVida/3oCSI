//**************************************************************************
// Práctica 3 
//**************************************************************************

#include "objetos_B3.h"
#include "file_ply_stl.hpp"
#include <time.h> 

const _color Color::c_default = _color(0.0f, 0.0f, 0.0f);
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

void _triangulos3D::draw(_modo modo, Color color, float grosor, Coordenadas pos) // , Coordenadas pos)
{
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
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
    glPopMatrix();
}

void _puntos3D::draw_puntos(Color color, int grosor)
{
    glPointSize(grosor);
    glColor3f(color.actual.r, color.actual.g, color.actual.b);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
    glDrawArrays(GL_POINTS, 0, vertices.size());
}

void _triangulos3D::draw_aristas(Color color, int grosor)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(grosor);
    glColor3f(color.actual.r, color.actual.g, color.actual.b);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
    glDrawElements(GL_TRIANGLES, caras.size()*3, GL_UNSIGNED_INT, &caras[0]);
}

void _triangulos3D::draw_solido(Color color)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(color.actual.r, color.actual.g, color.actual.b);
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

void _rotacion::parametros(vector<_vertex3f> perfil, size_t num, bool tapa_inferior, bool tapa_superior, int tipo, unsigned char porcentaje_revolucion, unsigned char porcentaje_generacion_esfera, Coordenadas cuspide)
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

    size_t num_revoluciones = static_cast<size_t>((porcentaje_revolucion / 100.0) * num);

    for (size_t j = 0; j < num_revoluciones; j++)
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
        for (size_t j = 0; j < num_revoluciones; j++)
        {
            for (size_t i = 0; i < num_aux - 1; i++)
            {
                cara_aux._0 = i + j * num_aux;
                cara_aux._1 = i + ((j + 1) % num_revoluciones) * num_aux;
                cara_aux._2 = i + 1 + j * num_aux;
                caras.push_back(cara_aux);

                cara_aux._0 = i + ((j + 1) % num_revoluciones) * num_aux;
                cara_aux._1 = i + 1 + ((j + 1) % num_revoluciones) * num_aux;
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
        vertice_aux.z = 0.0;
        vertice_aux.y = perfil[num_aux - 1].y;
        if (tipo == 1 and porcentaje_generacion_esfera == 100)
            vertice_aux.y = radio;
        else
            vertice_aux.y = perfil[num_aux - 1].y;
        if (tipo == 2 and cuspide != coordenadas_default)
        {
            vertice_aux.x = cuspide.x;
            vertice_aux.y = cuspide.y;
            vertice_aux.z = cuspide.z;
        }
        else
            vertice_aux.y = perfil[1].y;

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

_piramide::_piramide(Coordenadas tam) : tam(tam)
{
    //vertices 
    vertices.resize(5); 
    vertices[0].x = -tam.x / 2;  vertices[0].y = 0;  vertices[0].z = 0;         // Vértice inferior izquierdo de la base
    vertices[1].x = tam.x / 2;   vertices[1].y = 0;  vertices[1].z = 0;         // Vértice inferior derecho de la base
    vertices[2].x = tam.x / 2;   vertices[2].y = tam.y;   vertices[2].z = 0;         // Vértice superior derecho de la base
    vertices[3].x = -tam.x / 2;  vertices[3].y = tam.y;   vertices[3].z = 0;         // Vértice superior izquierdo de la base
    vertices[4].x = 0;           vertices[4].y = 0;          vertices[4].z = tam.z;     // Vértice de la punta

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

    aux.x = radio; aux.y = 0.0;   aux.z = 0.0;
    perfil.push_back(aux);
    aux.x = radio; aux.y = altura;    aux.z = 0.0;
    perfil.push_back(aux);

    parametros(perfil, num, true, true, 0);
}

_cono::_cono(float radio, float altura, size_t num, Coordenadas cuspide)
: cuspide(cuspide)
{
    vector<_vertex3f> perfil;
    _vertex3f aux;

    aux.x = radio;  aux.y = 0;    aux.z = 0.0;
    perfil.push_back(aux);
    aux.x = 0.0;    aux.y = altura;         aux.z = 0.0;
    perfil.push_back(aux);

    parametros(perfil, num, true, true, 2, 100, 100, cuspide);
}

_esfera::_esfera(GLfloat radio, size_t resolucion, bool tapa_su, bool tapa_in, unsigned char porcentaje_generacion_x, unsigned char porcentaje_generacion_y)
        : radio(radio), resolucion(resolucion), porcentaje_generacion_x(porcentaje_generacion_x), porcentaje_generacion_y(porcentaje_generacion_y)

{
    vector<_vertex3f> perfil;
    _vertex3f aux;

    if (porcentaje_generacion_x > 100 or porcentaje_generacion_x < 0 or porcentaje_generacion_y > 100 or porcentaje_generacion_y < 0)
    {
        cerr << "Error al generar esfera, porcentaje incorrecto" << endl;
        exit(1);
    }

    size_t limite = static_cast<size_t>((porcentaje_generacion_x / 100.0) * resolucion);
    for (size_t i = 1; i < limite; i++)
    {
        aux.x = radio * cos(M_PI * i / (resolucion * 1.0) - M_PI / 2.0);
        aux.y = radio * sin(M_PI * i / (resolucion * 1.0) - M_PI / 2.0);
        aux.z = 0.0;
        perfil.push_back(aux);
    }
    parametros(perfil, resolucion, 1, tapa_in, tapa_su, porcentaje_generacion_y, porcentaje_generacion_x);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

// PRACTICA 3 PROPIA

//************************************************************************
// NATURALEZA
//************************************************************************
//_suelo::_suelo(Coordenadas pos) : posicion(pos) {}
void _suelo::draw(_modo modo, float grosor) // , Coordenadas pos)
{
    glPushMatrix();
        glTranslatef(posicion.x, posicion.y, posicion.z);
        glTranslatef(0, -(tam.y/2), 0);
        glScalef(tam.x, tam.y, tam.z);
        _cubo cubo;
        cubo.draw(modo, color_suelo, grosor);
    glPopMatrix();
}

//_sol::_sol(GLfloat) {}
void _sol::draw(_modo modo, float grosor) // , Coordenadas pos)
{
    glPushMatrix();
        glTranslatef(posicion.x, posicion.y, posicion.z);
        glColor3f(color_sol.actual.r, color_sol.actual.g, color_sol.actual.b);
        glutSolidSphere(radio, resolucion, resolucion);
    glPopMatrix();
}

void _sol::actualizar_sol_hora(GLfloat hora)
{
    // Primero actualizo el color
    color_sol.actualizar_hora(hora);

    // Ahora actualizo la posición
    // M_PI es la semielipse
    GLfloat angulo = (M_PI / 24.0) * hora;
    posicion.x = punto_minimo_x * cos(angulo);
    posicion.y = punto_algido_y * sin(angulo);


    //// Actualizo el color y posicion de la luz
    //luz_ambiente.actualizar_hora(hora);
    //GLfloat color_luz_normal[] = {color_sol.actual.r, color_sol.actual.g, color_sol.actual.b, 1.0};
    //GLfloat color_luz_ambiente[] = {luz_ambiente.actual.r, luz_ambiente.actual.g, luz_ambiente.actual.b, 1.0};
    //GLfloat posicion_luz[] = {0.0, 0.0, 0.0, 0.0};
    //glLightfv(GL_LIGHT0, GL_AMBIENT, color_luz_ambiente);
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, color_luz_normal);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, color_luz_normal);
    //glLightfv(GL_LIGHT0, GL_POSITION, posicion_luz);
}

//_nube::_nube(Coordenadas pos) : posicion(pos) {}
void _nube::draw(_modo modo, Color color, float grosor) // , Coordenadas pos)
{}

//_lluvia::_lluvia(Coordenadas pos) : posicion(pos) {}
void _lluvia::draw(_modo modo, Color color, float grosor) // , Coordenadas pos)
{}

//_viento::_viento(Coordenadas pos) : posicion(pos) {}
void _viento::draw(_modo modo, bool tiempo_ingame, GLfloat hora_ingame, float grosor) // , Coordenadas pos)
{
    if (velocidad > velocidad_max)
        velocidad = velocidad_max;
    else if (velocidad < 0)
        velocidad = 0.0;

    // Si el viento no se mueve no hay viento xd
    if (velocidad)
    {
        glPushMatrix();
            glTranslatef(posicion.x, posicion.y, posicion.z);
            glTranslatef(-area_efecto.x/2.0, 0.0, -area_efecto.z/2.0);
            // Actualizo la hora actual y la distancia transcurrida
            unsigned instante_actual = tiempo_ingame ? hora_ingame*3600*1000 : glutGet(GLUT_ELAPSED_TIME);
            //float tiempo_transcurrido = (instante_actual - instante_previo) / 1000.0f; // Diferencia de tiempo en segundos
            instante_previo = instante_actual;

            // Ajusto los parametros de la honda y el escalado
            // (Supongo que la velocidad está en kilometros por hora)
            GLfloat distancia_recorrida_en_un_segundo = (velocidad/3600)*1000;
            GLfloat distancia_recorrida = distancia_recorrida_en_un_segundo*instante_actual/1000.0f; // (en "metros")
            //GLfloat escalado_brisa = (velocidad / velocidad_max) * escalado_max_brisas;
            onda.frecuencia = frecuencia_min + (((velocidad / velocidad_max) * (frecuencia_max-frecuencia_min)*10)/10);
            onda.longitud = distancia_recorrida_en_un_segundo / onda.frecuencia;
            //cerr << "Frecuencia: " << onda.frecuencia << endl;
            // (Lo de 10 es para que no perjudique a los decimales)

            // Necesito que una brisa recorra el area de efecto [onda.periodo] veces por segundo
            // Teniendo en cuenta la velocidad a la que van
            // Si con mi velocidad no soy capaz de recorrer la distancia que me corresponde genero más brisas
            // ¿Cada cuanta distancia spawneo una brisa?: Si usa brisa tarda 3s en recorrer un km y quiero ver
            // una brisa en un punto cada segundo necesito generar una brisa por segundo hasta que llegue la original
            GLfloat cuantas_vueltas_quiero_dar_por_segundo = 1 / onda.frecuencia;
            GLfloat cuantas_vueltas_doy_por_segundo = distancia_recorrida_en_un_segundo / area_efecto.x;
            //cerr << "Vueltas / segundo: " << cuantas_vueltas_doy_por_segundo << endl;
            //cerr << "Vueltas deseadas / segundo: " << cuantas_vueltas_quiero_dar_por_segundo << endl;
            //cerr << "Vueltas deseadas / Vueltas dadas: " << (cuantas_vueltas_quiero_dar_por_segundo / cuantas_vueltas_doy_por_segundo) << endl;
            GLfloat distancia_entre_brisas = 0.0; //= distancia_recorrida_en_un_segundo / area_efecto.x <= (area_efecto.x * onda.frecuencia)-(brisa_viento.largo*2) ? distancia_recorrida_en_un_segundo / onda.frecuencia : 0;
            if (cuantas_vueltas_doy_por_segundo < cuantas_vueltas_quiero_dar_por_segundo)
                distancia_entre_brisas = area_efecto.x / (cuantas_vueltas_quiero_dar_por_segundo / cuantas_vueltas_doy_por_segundo);
                
            distancia_entre_brisas = max(distancia_entre_brisas, brisa_viento.largo*2);

            //cerr << "Distancia entre brisas: " << distancia_entre_brisas << endl;
            //cerr << "Distancia recorrida: " << distancia_recorrida << endl;
            //cerr << "Area de efecto: " << area_efecto.x << endl;
            GLfloat x_i = 0.0;
            do
            {
                glPushMatrix();
                    GLfloat pos_x = fmod(x_i + distancia_recorrida, area_efecto.x);
                    //cerr << "Pos x: " << pos_x << endl;
                    //if ((x_i+distancia_recorrida) < area_efecto.x)
                    //    pos_x = x_i+distancia_recorrida;
                    //else if (((x_i+distancia_recorrida)-area_efecto.x) < (distancia_entre_brisas+distancia_recorrida))
                    //    pos_x = (x_i+distancia_recorrida) - area_efecto.x;
                    //else
                    //    break;
                    // Traslado la lámina a su posición correspondiente
                    glTranslatef(pos_x, 0.0, 0.0);
                    //brisa_viento.punto_curva_1.z = onda(pos_x+brisa_viento.punto_curva_1.y, instante_actual/1000.f);
                    //brisa_viento.punto_curva_2.z = onda(pos_x+brisa_viento.punto_curva_2.y, instante_actual/1000.f);
                    brisa_viento.punto_curva_1.z = onda(instante_actual/1000.f);
                    brisa_viento.punto_curva_2.z = -brisa_viento.punto_curva_1.z;
                    // Dibujo las laminas y capas con un huequito entre capas
                    bool offset = true;
                    GLfloat offset_z = (area_efecto.z/num_brisas_por_lado)/2.0;
                    for (size_t y_i = 0; y_i < num_brisas_por_lado; y_i++)
                    {
                        offset = !offset;
                        for (size_t z_i = 0; z_i < num_brisas_por_lado; z_i++)
                        {
                            glPushMatrix();
                                // La traslado a donde toque 
                                //glScalef(escalado_brisa, 0.0, escalado_brisa/2.0);
                                glTranslatef(0, 0, (area_efecto.z/num_brisas_por_lado)*(z_i+1)+offset_z*offset);
                                glTranslatef(0, (area_efecto.y/num_brisas_por_lado)*(y_i+1), 0);

                                // Dibujo la brisa
                                glRotatef(90, 0, 1, 0);
                                glRotatef(-90, 1, 0, 0);
                                brisa_viento.draw(modo, grosor);
                            glPopMatrix();

                            if (offset and z_i == num_brisas_por_lado - 2)
                                break;
                        }
                    }
                glPopMatrix();
                x_i += distancia_entre_brisas;
            } while (x_i < area_efecto.x and distancia_entre_brisas != 0);
        glPopMatrix();
    }
}
//************************************************************************
// GIRASOL
//************************************************************************
//_tallo_girasol::_tallo_girasol(Coordenadas pos) : posicion(pos) {}
void _tallo_girasol::draw(_modo modo, float grosor) // , Coordenadas pos)
{
    _cilindro tallo(radio_tallo, largo_tallo, 30);
    glPushMatrix();
        glTranslatef(posicion.x, posicion.y, posicion.z);
        //glRotatef(-90, 1, 0, 0);
        tallo.draw(modo, color_tallo, grosor);
    glPopMatrix();
}


_petalo_girasol::_petalo_girasol(Coordenadas punto_curva_1, Coordenadas punto_curva_2, GLfloat ancho, GLfloat largo, Coordenadas pos, size_t resolucion)
    :
    punto_curva_1(punto_curva_1), punto_curva_2(punto_curva_2),
    ancho(ancho), largo(largo),
    posicion(pos), color_petalo(246u, 169, 3)
{
        vertices.clear();
        caras.clear();

        // Puntos de control para las curvas de Bézier del borde izquierdo y derecho
        Coordenadas puntos_control_izquierda[4] =
        {
            Coordenadas(),
            punto_curva_1,
            punto_curva_2,
            Coordenadas(0.0f, largo, 0.0f)
        };

        Coordenadas puntos_control_derecha[4] =
        {
            Coordenadas(),
            punto_curva_1.espejo_x(),
            punto_curva_2.espejo_x(),
            Coordenadas(0.0f, largo, 0.0f)
        };


        // Puede ser que se estén duplicando los vertices de las puntas
        // pero no estoy como para preocuparme por eso

            
            // Calculo los puntos de la curva de Bézier izquierda
        for (size_t i = 0; i <= resolucion; ++i)
        {
            GLfloat t = static_cast<float>(i) / resolucion;
            GLfloat x_izq = (1-t) * (1-t) * (1-t) * puntos_control_izquierda[0].x +
                         3 * (1-t) * (1-t) * t * puntos_control_izquierda[1].x +
                         3 * (1-t) * t * t * puntos_control_izquierda[2].x +
                         t * t * t * puntos_control_izquierda[3].x;
            GLfloat y_izq = (1-t) * (1-t) * (1-t) * puntos_control_izquierda[0].y +
                         3 * (1-t) * (1-t) * t * puntos_control_izquierda[1].y +
                         3 * (1-t) * t * t * puntos_control_izquierda[2].y +
                         t * t * t * puntos_control_izquierda[3].y;
            GLfloat z_izq = (1-t) * (1-t) * (1-t) * puntos_control_izquierda[0].z +
                         3 * (1-t) * (1-t) * t * puntos_control_izquierda[1].z +
                         3 * (1-t) * t * t * puntos_control_izquierda[2].z +
                         t * t * t * puntos_control_izquierda[3].z;

            vertices.push_back({x_izq, y_izq, z_izq});

        }
        // Caluculo los puntos de la curva de Bézier derecha
        for (size_t i = 0; i <= resolucion; ++i)
        {
            GLfloat t = static_cast<float>(i) / resolucion;
            GLfloat x_der = (1-t) * (1-t) * (1-t) * puntos_control_derecha[0].x +
                         3 * (1-t) * (1-t) * t * puntos_control_derecha[1].x +
                         3 * (1-t) * t * t * puntos_control_derecha[2].x +
                         t * t * t * puntos_control_derecha[3].x;
            GLfloat y_der = (1-t) * (1-t) * (1-t) * puntos_control_derecha[0].y +
                         3 * (1-t) * (1-t) * t * puntos_control_derecha[1].y +
                         3 * (1-t) * t * t * puntos_control_derecha[2].y +
                         t * t * t * puntos_control_derecha[3].y;
            GLfloat z_izq = (1-t) * (1-t) * (1-t) * puntos_control_izquierda[0].z +
                         3 * (1-t) * (1-t) * t * puntos_control_izquierda[1].z +
                         3 * (1-t) * t * t * puntos_control_izquierda[2].z +
                         t * t * t * puntos_control_izquierda[3].z;

            vertices.push_back({x_der, y_der, z_izq});
        }
        // Creo las caras uniendo los puntos
        for (size_t i = 0; i < resolucion; ++i) {
            int izq_actual = i;
            int izq_siguiente = i + 1;
            int der_actual = i + resolucion + 1;
            int der_siguiente = i + resolucion + 2;

            caras.push_back({izq_actual, izq_siguiente, der_actual});
            caras.push_back({izq_siguiente, der_siguiente, der_actual});
        }

        // Asigno los colores
        colors_random();
}
//void _petalo_girasol::draw(_modo modo, Color color, float grosor) // , Coordenadas pos)
//{}

void _hoja_girasol::draw(_modo modo, float grosor)
{
    _petalo_girasol hoja(punto_curva_1, punto_curva_2, ancho, largo);
    //hoja.punto_curva_1 = punto_curva_1;
    //hoja.punto_curva_2 = punto_curva_2;
    glPushMatrix();
        glTranslatef(posicion.x, posicion.y, posicion.z);
        hoja.draw(modo, color_hoja, grosor);
    glPopMatrix();
}

//_cabeza_girasol::_cabeza_girasol(Coordenadas pos) : posicion(pos) {}
void _cabeza_girasol::draw(_modo modo, float grosor) // , Coordenadas pos)
{
    glPushMatrix();
    glTranslatef(posicion.x, posicion.y, posicion.z);

    //cuello.largo_tallo = 0.4;
        glPushMatrix();
        glTranslatef(0.0, 0.0, cuello.largo_tallo);
        // Dibujo la parte de abajo del semillero
        hoja.punto_curva_1 = {0.68, -0.1, 0.0};
        hoja.punto_curva_2 = {-0.02, 0.7, -0.2};
        for (size_t capa_i = 0; capa_i < num_capas_hojas; ++capa_i)
        {
            GLfloat angulo_offset = (360.0f / num_hojitas) / num_capas_hojas * capa_i;
            GLfloat z_offset = 0.02 * capa_i;
            for (size_t hoja_i = 0; hoja_i < num_hojitas; ++hoja_i)
            {
                glPushMatrix();
                    // Voy rotando cada pétalo
                    glRotatef((360.0f / num_hojitas * hoja_i) - angulo_offset, 0, 0, 1);
                    // La ajusto al centro
                    glTranslatef(0.0, 0.0, radio_semillero*0.05+z_offset);
                    hoja.draw(modo);
                glPopMatrix();
            }
        }
        // Dibujo el semillero
        glPushMatrix();
            glRotatef(-90, 1, 0, 0);
            _esfera semillero(radio_semillero, 100, true, true, 50, 100);
            semillero.draw(modo, color_semillero, grosor);
        glPopMatrix();

        
        // Dibujo los pétalos
        for (size_t capa_i = 0; capa_i < num_capas_petalo; ++capa_i)
        {
            GLfloat angulo_offset = (360.0f / num_petalos) / num_capas_petalo * capa_i;
            GLfloat z_offset = 0.03 * capa_i;
            //cerr << "Petalo color: " << petalo.color_petalo.r << " " << petalo.color_petalo.g << " " << petalo.color_petalo.b << endl;
            for (size_t petalo_i = 0; petalo_i < num_petalos; ++petalo_i)
            {
                glPushMatrix();
                    // Voy rotando cada pétalo
                    glRotatef(360.0f / num_petalos * petalo_i + angulo_offset, 0, 0, 1);
                    // La ajusto al centro
                    //glTranslatef(0.0, radio_semillero*0.8, 0.0);
                    petalo.draw(modo, petalo.color_petalo.actual + 0.05*(capa_i + 1), grosor, {0.0, radio_semillero*0.8f, radio_semillero*0.25f + z_offset});
                glPopMatrix();
            }
        }
        glPopMatrix();
        // Dibujo el cuello
        cuello.radio_tallo = radio_semillero*0.381;
        cuello.largo_tallo = 0.04;
        glPushMatrix();
            glRotatef(90, 1, 0, 0);
            cuello.draw(modo);
        glPopMatrix();
    glPopMatrix();
}

_girasol::_girasol(Coordenadas pos) : posicion(pos)
{
    //longitud_min_rama = 0.25 * (cabeza.radio_semillero + cabeza.petalo.largo);
    //tallo.largo_tallo = 10;
    tallo.radio_tallo = cabeza.radio_semillero*0.381;

    for (size_t i = 0; i < num_ramas; i++)
    {
        //variacion_tam_rama.push_back(aleatorio(0.0, 1.5)); // La rama más grande será un 150% más grande
        longitudes_ramas.push_back(aleatorio(longitud_min_rama, longitud_max_rama)); // La rama más grande será un 50% más grande
        angulo_rama.push_back(aleatorio(angulo_min_rama, angulo_max_rama) * M_PI / 180.0);
    }
    
}
void _girasol::draw(_modo modo, float grosor) // , Coordenadas pos)
{
    glPushMatrix();
    glTranslatef(posicion.x, posicion.y, posicion.z);
    
        // Dibujo el tallo
        tallo.draw(modo, grosor);

        // Dibujo la cabeza
        glPushMatrix();
            glTranslatef(0.0, tallo.largo_tallo, 0.0);
            cabeza.draw(modo, grosor);
        glPopMatrix();

        // Dibujo las ramas
        for (size_t rama_i = 0; rama_i < num_ramas; ++rama_i)
        {
            GLfloat angulo_rotacion = 360.0f / num_ramas * rama_i;
            GLfloat altura_rama = (tallo.largo_tallo - cabeza.petalo.largo) / (num_ramas+1) * (rama_i+1);
            //GLfloat longitud_rama = longitud_min_rama + longitud_min_rama*variacion_tam_rama[rama_i];
            GLfloat longitud_rama = longitudes_ramas[rama_i];
            GLfloat x_rama = sin(angulo_rama[rama_i]) * longitud_rama;
            GLfloat longitud_cateto_superior = sqrt(longitud_rama*longitud_rama - x_rama*x_rama);

            glPushMatrix();
                glRotatef(angulo_rotacion, 0, 1, 0);
                glPushMatrix();
                    _cono rama(0.8*tallo.radio_tallo, 0.0, 20, {x_rama, longitud_rama, 0.0});
                    glTranslatef(-0.8*tallo.radio_tallo, altura_rama, 0.0);
                    glRotatef(90, 0, 0, 1);
                    rama.draw(modo, tallo.color_tallo);
                glPopMatrix();

                glPushMatrix();
                    //cerr << "Longitud cateto superior: " << longitud_cateto_superior << endl;
                    //cerr << "Longitud rama: " << longitud_rama << endl;
                    //cerr << "Altura rama: " << altura_rama << endl;
                    //cerr << "x_rama: " << x_rama << endl;
                    //cerr << "variacion_tam_rama_i: " << variacion_tam_rama[rama_i] + 1 << endl;
                    //cerr << "angulo " << rama_i << " :" << angulo_rama[rama_i] << endl <<endl;
                    glTranslatef(-(longitud_cateto_superior + 0.8*tallo.radio_tallo), altura_rama + x_rama, 0.0);
                    glRotatef(180, 0, 1, 0);
                    glRotatef(-90, 0, 0, 1);
                    glRotatef(90, 0, 1, 0);
                    //glScalef(1+variacion_tam_rama[rama_i], 1+variacion_tam_rama[rama_i], 1+variacion_tam_rama[rama_i]);
                    GLfloat escalado = 1.5 + (longitud_rama-longitud_min_rama)/(longitud_max_rama-longitud_min_rama);
                    glScalef(escalado, escalado, escalado);
                    hoja.draw(modo, grosor);
                glPopMatrix();
            glPopMatrix();
        }
    glPopMatrix();
}

//************************************************************************
// MOLINO
//************************************************************************
//_helice_molino::_helice_molino(Coordenadas pos) : posicion(pos) {}
void _helice_molino::draw(_modo modo, float grosor) // , Coordenadas pos)
{
    if (num_aspas == 0)
    {
        cerr << "Error, número de aspas = 0" << endl;
        exit(0);
    }

    glPushMatrix();
    glTranslatef(posicion.x, posicion.y, posicion.z);

    // Dibujo palo central
    glPushMatrix();
        glTranslatef(0.0, grosor_aspas/2.0, 0.0);
        palo_central.draw(modo, color_helice, grosor);
    glPopMatrix();

    // Dibujo la bola central
    glPushMatrix();
        glTranslatef(0.0, grosor_aspas/2.0, 0.0);
        bola_central.draw(modo, color_helice, grosor);
    glPopMatrix();

    // Dibujo las hélices
    for (size_t aspa_i = 0; aspa_i < num_aspas; ++aspa_i)
    {
        glPushMatrix();
            // Voy rotando cada aspa
            glRotatef(360.0f / num_aspas * aspa_i, 0, 1, 0);
            // La ajusto al centro
            glTranslatef(0.0, 0.0, -largo_aspas);
            aspa.draw(modo, color_helice, grosor);
        glPopMatrix();
    }
    glPopMatrix();
}

//_molino::_molino(Coordenadas pos) : posicion(pos) {}
void _molino::draw(_modo modo, float grosor) // , Coordenadas pos)
{
    glPushMatrix();
    glTranslatef(posicion.x, posicion.y, posicion.z);
        // Dibujo el pirulo
        glPushMatrix();
            // Lo subo al tejado
            glTranslatef(0.0, altura_casa, 0.0);
            // Lo elevo sobre el nivel base
            tejado.draw(modo, color_molino_tejado, grosor);
        glPopMatrix();

        // Dibulo la casa
        glPushMatrix();
            casa.draw(modo, color_molino_casa, grosor);
        glPopMatrix();

        // Dibujo la hélice
        glPushMatrix();
            glTranslatef(0.0, 0.382*altura_tejado + altura_casa, -((radio_tejado+0.2)-helice.largo_palo_central));
            //glScalef(0.2, 0.2, 0.2);
            glTranslatef(0.0, 0.0, -(helice.largo_palo_central + helice.radio_bola_central/2.0));
            glRotatef(90, 1, 0, 0);
            helice.draw(modo, grosor);
        glPopMatrix();
    glPopMatrix();
}

//************************************************************************
// BASE
//************************************************************************
//_escena_P3::_escena_P3(Coordenadas pos) : posicion(pos) {}
void _escena_P3::draw(_modo modo, float grosor) // , Coordenadas pos)
{
    if (paso_tiempo_automatico)
        actualizar_hora();

    glPushMatrix();
        // Sol
        sol.actualizar_sol_hora(hora);
        glPushMatrix();
            sol.draw(modo, grosor);
        glPopMatrix();

        // Dibulo el viento
        glPushMatrix();
            viento.draw(modo);
        glPopMatrix();

        // Suelo
        glPushMatrix();
            suelo.draw(modo, grosor);
        glPopMatrix();

        // Molino
        glPushMatrix();
            glRotatef(180, 0, 1, 0);
            //glScalef(1.5, 1.5, 1.5);
            molino.draw(modo, grosor);
        glPopMatrix();

        // Girasol/es
        glPushMatrix();
            glTranslatef(molino.radio_tejado*2, 0.0, molino.radio_tejado*2);
            //glScalef(0.1, 0.1, 0.1);
            girasol.draw(modo, grosor);
        glPopMatrix();



    glPopMatrix();
};

void _escena_P3::actualizar_hora()
{
    unsigned instante_actual = glutGet(GLUT_ELAPSED_TIME);
    float tiempo_transcurrido = (instante_actual - instante_previo) / 1000.0f; // Diferencia de tiempo en segundos
    instante_previo = instante_actual;

    float porcion_del_dia_transcurrido = tiempo_transcurrido / duracion_dia_real;

    if (hora < 24.0)
        hora += porcion_del_dia_transcurrido*24.0;
}

//************************************************************************
// OTRAS COSAS
//************************************************************************
float aleatorio(float minimo, float maximo)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> distribucion(minimo, maximo);

    return distribucion(gen);
}