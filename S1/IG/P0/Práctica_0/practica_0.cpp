#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

// Son convenios o palabras reservadas
// qué parte se corresponde con visualización, síntesis, captación
// No se le podría poner 0 de profundidad y ya en glOrtho
// Por qué indicamos el tamaño de linea antes de dibujar el relleno

void Ejes (int width) // Al crear los ejes le podemos meter el grosor que queramos
{   
    glLineWidth(width);
    glBegin(GL_LINES); // Vamos a empezar a dibujar lineas
       glColor3f(1.0,0.0,0.0); // Pon color a rojo
       glVertex3f(-1.0,0.0,0.0); // Dibuja punto en eje X
       glVertex3f(1.0,0.0,0.0); // Dibuja punto en eje X arriba
       glColor3f(0.0,1.0,0.0); // Pon color a verde
       glVertex3f(0.0,-1.0,0.0); // Dibuja punto en eje Y abajo
       glColor3f(1.0,1.0,0.0); // Pon color a amarillo? <- Si no interpolo en reshape toma el último color, sino hace un degradado
       glVertex3f(0.0,1.0,0.0);   // Dibuja punto en eje Y arriba
    glEnd();
   
       // NO se podría poner 2f y ya está?
}

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

void Monigote ()
{
  
 
// cara
   glLineWidth(1); // Grosor de la linea 1 (debería ir donde el contorno no?)
   glColor3f(1.0,0.8,0.6); // Pongo un color random (crema)
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Rellena un polígono tanto por dentrá como por delante
   glBegin(GL_POLYGON); // Vamos a empezar a dibujar un polígono
      glVertex3f(-0.2,0.0,0.0);
      glVertex3f(0.2,0.0,0.0);
      glVertex3f(0.2,0.55,0.0);
      glVertex3f(-0.2,0.55,0.0); // Basicamente es un cuadrado de anchura 0.2 y altura 0.55
   glEnd(); 

   glColor3f(0.0,0.0,0.); // Pongo a negro ¿no falta un 0?
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Creo un polígono pero rellenandolo con lineas en lugar de relleno
   glBegin(GL_POLYGON);
      glVertex3f(-0.2,0.0,0.0);
      glVertex3f(0.2,0.0,0.0);
      glVertex3f(0.2,0.55,0.0);
      glVertex3f(-0.2,0.55,0.0); // Básicamente le ha puesto un contorno negro sin más al polígono
   glEnd(); 
   
   glColor3f(0.2, 0.0, 1.0);
   Circle(0.05,0.1,0.4,50,GL_FILL);
   Circle(0.05,-0.1,0.4,50,GL_FILL);

   // ala de abajo
   glColor3f(0.0,0.0,0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Creo un polígono pero rellenandolo con lineas en lugar de relleno
   glBegin(GL_POLYGON);
      glVertex3f(-0.3,0.65,0.0);
      glVertex3f(0.3,0.65,0.0);
      glVertex3f(0.3,0.55,0.0);
      glVertex3f(-0.3,0.55,0.0);
   glEnd(); 

//ala de arriba
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Creo un polígono pero rellenandolo con lineas en lugar de relleno
  glBegin(GL_POLYGON);
      glVertex3f(-0.2,0.85,0.0);
      glVertex3f(0.2,0.85,0.0);
      glVertex3f(0.2,0.65,0.0);
      glVertex3f(-0.2,0.65,0.0);
  glEnd(); 

   // oreja derecha
   glColor3f(1.0,0.8,0.6);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Creo un polígono pero rellenandolo con lineas en lugar de relleno
   glBegin(GL_POLYGON);
      glVertex3f(-0.25,0.4,0.0);
      glVertex3f(-0.2,0.4,0.0);
      glVertex3f(-0.2,0.3,0.0);
      glVertex3f(-0.25,0.3,0.0); // Básicamente le ha puesto un contorno negro sin más al polígono
   glEnd(); 


   // oreja izquierda
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glBegin(GL_POLYGON);
      glVertex3f(0.2,0.4,0.0);
      glVertex3f(0.25,0.4,0.0);
      glVertex3f(0.25,0.3,0.0);
      glVertex3f(0.2,0.3,0.0);
   glEnd();

   glColor3f(7.0,0.0,0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
   glBegin(GL_TRIANGLES);
      glVertex3f(-0.035, 0.3,0.0);
      glVertex3f(0.0, 0.35, 0.0);
      glVertex3f(0.035, 0.3, 0.0);
   glEnd();

   glColor3f(7.0, 0.2, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glBegin(GL_POLYGON);
      glVertex3f(-0.75,0.15,0.0);
      glVertex3f(0.75,0.15,0.0);
      glVertex3f(0.75,0.1,0.0);
      glVertex3f(-0.75,0.1,0.0);
   glEnd();

   Circle(0.025,0.75,0.125,50,GL_FILL);
   Circle(0.025,-0.75,0.125,50,GL_FILL);

   // Pómulo izquierdo
   //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   //glBegin(GL_POLYGON);
   //   glVertex3f(-0.2, 0.0,0.0);
   //   glVertex3f(-0.175, 0.0,0.0);
   //   glVertex3f(-0.2, 0.25,0.0);
   //   glVertex3f(-0.175, 0.25,0.0);
   //glEnd();

   // Pómulo derecho
   //----- 
   glColor3f(0.0,0.0,0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glBegin(GL_POLYGON);
      glVertex3f(-0.25,0.4,0.0);
      glVertex3f(-0.2,0.4,0.0);
      glVertex3f(-0.2,0.3,0.0);
      glVertex3f(-0.25,0.3,0.0); // Básicamente le ha puesto un contorno negro sin más al polígono
   glEnd(); 

   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Creo un polígono pero rellenandolo con lineas en lugar de relleno
   glBegin(GL_POLYGON);
      glVertex3f(0.2,0.4,0.0);
      glVertex3f(0.25,0.4,0.0);
      glVertex3f(0.25,0.3,0.0);
      glVertex3f(0.2,0.3,0.0); // Básicamente le ha puesto un contorno negro sin más al polígono
   glEnd(); 

   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
   glBegin(GL_TRIANGLES);
      glVertex3f(-0.035, 0.3,0.0);
      glVertex3f(0.0, 0.35, 0.0);
      glVertex3f(0.035, 0.3, 0.0);
   glEnd();
}




static void Init( )
{

   glShadeModel( GL_FLAT); // Pone las sombras planas, vamos, que no hay sombras (creo)
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
  glClear( GL_COLOR_BUFFER_BIT );
   
  
   
   Ejes(6); // Pinta los ejes con grosor 6
   Monigote();

   
   glFlush(); // Esto básicamente le dice a la gráfica "ejecuta todos las intrucciones de renderizado que te he mandado hasta ahora"
}


static void Keyboard(unsigned char key, int x, int y )
{
 
  if (key==27) // tecla escape
      exit(0);

}


int main( int argc, char **argv )
{
   glutInit(&argc,argv);
   glutInitDisplayMode( GLUT_RGB ); // Si quisieramos que fuese

   glutInitWindowPosition( 900, 500 );
   glutInitWindowSize(700, 700 );
   glutCreateWindow("Practica 0 IG");


   Init();

   glutReshapeFunc(Reshape);
   glutDisplayFunc(Display); // Su tapo una ventana 
   glutKeyboardFunc(Keyboard);
  
   glutMainLoop( );

   return 0;
}

