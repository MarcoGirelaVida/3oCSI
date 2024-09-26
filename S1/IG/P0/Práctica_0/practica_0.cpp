#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <iostream>
using namespace std;

int pulsaciones = 0;
float grado = 0.0;
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

void Circle (GLfloat radio, GLfloat cx, GLfloat cy, GLint n, GLenum modo)
{
   int i;
   if (modo==GL_LINE)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   else if (modo==GL_FILL)
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   else glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
   
   glBegin( GL_POLYGON );
      for (i=0;i<n;i++)
         glVertex2f( cx+radio*cos(2.0*M_PI*i/n), cy+radio*sin(2.0*M_PI*i/n));
   glEnd();
}

// Función interna copiada y pegada de chatgpt
void Ovalo(GLfloat gx, GLfloat gy, GLfloat rx, GLfloat ry, Color c, int segments = 100, GLenum modo = GL_FILL) {

   if (modo==GL_LINE)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   else if (modo==GL_FILL)
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   else glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

   glColor4f(c.r,c.g,c.b,c.a);
   glBegin(GL_POLYGON);
   for (int i = 0; i < segments; ++i) {
      GLfloat theta = 2.0f * M_PI * GLfloat(i) / GLfloat(segments);

      // Calculamos las coordenadas x e y del punto sobre la elipse
      GLfloat x = rx * cosf(theta); // Eje X escalado por el radio en X
      GLfloat y = ry * sinf(theta); // Eje Y escalado por el radio en Y

      // Desplazamos el óvalo según el origen dado (gx, gy)
      glVertex2f(x + gx, y + gy);
    }
    glEnd();
}

// Es a un cuadrado que ocupa la escena entera con degradado sin más
void CieloAtardecer(Color c1, Color c2)
{
   glBegin(GL_QUADS);
      c1.actualizar(grado);
      glColor3f(c1.r,c1.g,c1.b);
      glVertex3f(-1.0,-0.2,0.0);
      glVertex3f(1.0,-0.2,0.0);
      c2.actualizar(grado);
      glColor3f(c2.r,c2.g,c2.b);
      glVertex3f(1.0,1.0,0.0);
      glVertex3f(-1.0,1.0,0.0);
   glEnd();
}

void Tierra(Color c)
{
   c.actualizar(grado);
   glColor4f(c.r,c.g,c.b, c.a);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glBegin(GL_QUADS);
      glVertex3f(-1.0,-0.2,0.0);
      glVertex3f(1.0,-0.2,0.0);
      glVertex3f(1.0,-1.0,0.0);
      glVertex3f(-1.0,-1.0,0.0);
   glEnd();
}

void ParteAtrasTierra(Color c1, Color c2)
{
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glBegin(GL_QUADS);
      c1.actualizar(grado);
      glColor3f(c1.r,c1.g,c1.b);
      glVertex3f(-1.0,-0.5,0.0);
      glVertex3f(1.0,-0.5,0.0);
      c2.actualizar(grado);
      glColor3f(c2.r,c2.g,c2.b);
      glVertex3f(1.0,-1.0,0.0);
      glVertex3f(-1.0,-1.0,0.0);
   glEnd();
}
// Es un círculo sólido sin más
// - Ponerle triangulitos
// - Ponerle degradado
void Sol(GLfloat radio, GLfloat gx, GLfloat gy, Color c)
{
   glColor4f(c.r,c.g,c.b,c.a);
   Circle(radio,gx,gy,100,GL_FILL);
}

void AloDelSol(GLfloat radio_inicial, GLfloat gx_inicial, GLfloat gy_inicial, GLfloat radio, GLint n)
{
   for (int i=0; i<n; i++)
   {
      //glColor3f(0.98,0.89,0.86);
      glLineWidth(1);
      glLineStipple(6, 0xAAAA);
      glEnable(GL_LINE_STIPPLE);
      Circle(radio_inicial,gx_inicial,gy_inicial,100,GL_LINE);
      radio_inicial += radio;
   }
}
void LineaMontanias(GLfloat cy, GLfloat altura, GLfloat anchura, Color c, GLfloat offset = 0.0)
{
      c.actualizar(grado);
      glColor4f(c.r,c.g,c.b, c.a);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glBegin(GL_TRIANGLES);
         GLfloat progreso = -1.0 + offset;
         while (progreso < 2.0)
         {
            glVertex3f(progreso, cy, 0.0); //izquierda
            glVertex3f(progreso + anchura, cy, 0.0); // derecha
            glVertex3f(progreso + anchura*0.5, cy+altura, 0.0); // Pico
            progreso += anchura;
         }
      glEnd();
}

void PiedraGato(Color c)
{
   c.actualizar(grado);
   glColor4f(c.r,c.g,c.b,c.a);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glBegin(GL_TRIANGLES);
      glVertex3f(1.0, -1.0, 0.0); // esquina inferior derecha
      glVertex3f(1.0, -0.2, 0.0); // esquina superior derecha
      glVertex3f(0.0, -0.2, 0.0); // esquina superior izquierda
   glEnd();
}

void gato(GLfloat cx, GLfloat cy, GLfloat tam)
{
   // Cabeza
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glBegin(GL_TRIANGLES);
      glVertex3f(cx, cy, 0.0); // Barbilla
      glVertex3f(cx + 0.5*tam, cy + 0.5*tam, 0.0); // Esquina superior derecha
      glVertex3f(cx + tam*0.5, cy + tam, 0.0);
   glEnd();
}

void nube(GLfloat radio, GLfloat cx, GLfloat cy, GLfloat tam, GLint num, Color c)
{
   c.actualizar(grado);
   glColor4f(c.r,c.g,c.b, c.a);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   GLfloat offset = tam/num;
   for (int i=0; i<num; i++)
   {
      Circle(radio, cx, cy, 100, GL_FILL);
      Circle(radio, cx-radio, cy-radio/2.0, 100, GL_FILL);
      cx += offset;
   }
}


static void Init( )
{

   glShadeModel(GL_SMOOTH); // Pone las sombras planas, vamos, que no hay sombras (creo)
}


static void Reshape( int width, int height )
{
    glViewport(0, 0, (GLint)width, (GLint)height);
    // Aquí se usa el sistema de coordenadas del dispositivos
    // Sirve para ajustar la vista del objeto a tu pantalla ya que openGL no usa dimensiones normalizadas a tu ordendaor
    glOrtho (-1.0, 1.0,-1.0, 1.0, -0.0, 0.0);
    // Te pone la cámara basicamente, tu ajustas cómo de cerca/lejos quieres que esté cada eje
    // En este caso 1 de tamaño a la izquierda, uno derecha, uno arriba y uno abajo y 10 de "near y far" (profundidad)
    // Si el objeto no tiene profundidad no se le podría poner 0 y ya?

    // NOTA: Dice el profesor que no se puede poner 0
}

static void Display( )
{

   glClearColor(0.5,0.5,0.5,0.0);
   // Sirve para resetear el buffer de color al que tu quieras y creo que para establecer el fondo que quieras
   glClear(GL_COLOR_BUFFER_BIT);
   
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glDepthMask(GL_FALSE);
  
   GLfloat anchura_montanias = 0.7, altura_montanias = 0.3, gx_base = -0.2, offset_lineas = 0.1;
   CieloAtardecer({0.88,0.44,0.22, 1.0, 0.92, 0.82, 0.58},{0.21,0.21,0.35, 1.0, 0.2, 0.66, 0.8});
   GLfloat theta = M_PI * GLfloat(20-pulsaciones) / GLfloat(20);
   GLfloat posx_sol = 0.4 * cosf(theta);
   GLfloat posy_sol = 0.5 * sinf(theta);
   LineaMontanias(gx_base+offset_lineas,altura_montanias,anchura_montanias,{0.45,0.19,0.27, 0.5, 0.0, 0.62, 0.72}, -anchura_montanias/2);
   Sol(0.25, posx_sol, posy_sol, {0.98,0.89,0.86, 1.0});
   AloDelSol(0.25,posx_sol, posy_sol,0.05,7);
   glDisable(GL_LINE_STIPPLE);
   glLineWidth(5);
   Ovalo(0.0, 0.00-0.2, 0.6, 0.4,{1.0, 0.0, 0.0, 0.3}, 100, GL_LINE);
   Ovalo(0.0, 0.01-0.2, 0.6, 0.4,{1.0, 0.7, 0.0, 0.3}, 100, GL_LINE);
   Ovalo(0.0, 0.02-0.2, 0.6, 0.4,{1.0, 1.0, 0.0, 0.3}, 100, GL_LINE);
   Ovalo(0.0, 0.03-0.2, 0.6, 0.4,{0.2, 1.0, 0.2, 0.3}, 100, GL_LINE);
   Ovalo(0.0, 0.04-0.2, 0.6, 0.4,{0.2, 0.2, 1.0, 0.3}, 100, GL_LINE);
   Ovalo(0.0, 0.05-0.2, 0.6, 0.4,{0.5, 0.1, 1.0, 0.3}, 100, GL_LINE);
   Ovalo(0.0, 0.06-0.2, 0.6, 0.4,{0.6, 0.2, 1.0, 0.3}, 100, GL_LINE);
   ParteAtrasTierra({0.71,0.41,0.36, 1.0, 0.78, 0.86, 1.16}, {0.37, 0.29, 0.42, 1.0, 0.42, 0.76, 0.81});
   // Sol del reflejo
   Sol(0.25, posx_sol+0.2, -posy_sol-0.3, {0.98,0.89,0.86, 0.3});
   // Montañas invertidas para hacer el reflejo sobre el lago
   //LineaMontanias(gx_base-offset_lineas*4,-altura_montanias,anchura_montanias, {0.24,0.16,0.28, 0.3, 0.06, 0.5, 0.58}, -anchura_montanias/2);
   LineaMontanias(gx_base-offset_lineas*3,-altura_montanias-0.2+altura_montanias*grado,anchura_montanias, {0.34,0.18,0.27, 0.3, 0.0, 0.55, 0.65});
   LineaMontanias(gx_base-offset_lineas*2,-altura_montanias-0.2+altura_montanias*grado,anchura_montanias,{0.45,0.19,0.27, 0.3, 0.0, 0.62, 0.72}, -anchura_montanias/2);
   LineaMontanias(gx_base-offset_lineas,-altura_montanias-0.2+altura_montanias*grado, anchura_montanias, {0.55,0.26,0.26, 0.3, 0.34, 0.7, 0.75});
   
   // Activamos como un modo para evitar que se pinte donde se le diga (el ovalo)
   glEnable(GL_STENCIL_TEST);
   glStencilFunc(GL_ALWAYS, 1, 0xFF);
   glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
   glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); 
   glStencilMask(0xFF);
   Ovalo(0.2, -0.8, 0.7, 0.3,{1.0, 1.0, 1.0, 1.0});
   // Desactivamos el modo raro y volvemos al normal
   glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
   glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
   glStencilMask(0x00);
   Tierra({0.24,0.16,0.28, 1.0, 0.07, 0.43, 0.51});
   glDisable(GL_STENCIL_TEST);

   // Color antiguo nube: {0.89,0.53,0.23}
   nube(0.02+0.01*grado,0.75-1.5*0.05*pulsaciones,0.3,0.25+0.1*grado,7, {0.9, 0.7, 0.4, 0.3});
   nube(0.02+0.01*grado,0.4-0.8*0.05*pulsaciones,0.25,0.25+0.1*grado,7, {0.9, 0.7, 0.4, 0.3});

   LineaMontanias(gx_base,altura_montanias, anchura_montanias, {0.55,0.26,0.26, 1.0, 0.34, 0.7, 0.75});
   LineaMontanias(gx_base-offset_lineas,altura_montanias,anchura_montanias,{0.45,0.19,0.27, 1.0, 0.0, 0.62, 0.72}, -anchura_montanias/2);
   LineaMontanias(gx_base-offset_lineas*2,altura_montanias,anchura_montanias, {0.34,0.18,0.27, 1.0, 0.0, 0.55, 0.65});
   LineaMontanias(gx_base-offset_lineas*3,altura_montanias,anchura_montanias, {0.24,0.16,0.28, 1.0, 0.06, 0.5, 0.58}, -anchura_montanias/2);


   //PiedraGato({0.19,0.15,0.26});
   //gato(0.0,0.0,0.5);
   glDepthMask(GL_TRUE);
   glDisable(GL_BLEND);
   
   glFlush(); // Esto básicamente le dice a la gráfica "ejecuta todos las intrucciones de renderizado que te he mandado hasta ahora"
   glutSwapBuffers(); // Esto hace falta para lo de GLUT_DOUBLE
}


static void Keyboard(unsigned char key, int x, int y )
{
 
  if (key==27) // tecla escape
      exit(0);

}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:   // amanecer
            //cerr << "Tecla arriba" << endl;
            if (pulsaciones < 20)
               pulsaciones++;
            if (pulsaciones < 10)
               grado+=0.1;
            else if (pulsaciones >= 10 and pulsaciones < 20)
               grado-=0.1;
            break;
        case GLUT_KEY_DOWN: // atardecer
            //cerr << "Tecla abajo" << endl;
            if (pulsaciones > 0)
               pulsaciones--;
            if (pulsaciones > 0 and pulsaciones < 10)
               grado-=0.1;
            else if (pulsaciones >= 10 and pulsaciones < 20)
               grado+=0.1;
            break;
        case 27:
            exit(0);
            break;
    }
    glutPostRedisplay(); // Redibujo
}

int main( int argc, char **argv )
{
   glutInit(&argc,argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL); // Si quisieramos que fuese

   glutInitWindowPosition( 900, 500 );
   glutInitWindowSize(1000, 1000 );
   glutCreateWindow("IMPORTANTE!! PULSE LA FLECHA HACIA ARRIBA!!!");


   Init();

   glutReshapeFunc(Reshape);
   glutDisplayFunc(Display); // Su tapo una ventana 
   glutSpecialFunc(specialKeys);
  
   glutMainLoop( );

   return 0;
}

