#ifndef CABECALHO_H_INCLUDED
#define CABECALHO_H_INCLUDED

/* Constantes. */
#define PI 3.14159    // Constante PI.
#define fps 1000 / 60 // Constante quantidade de frames em 1 segundo para anima��o (30 FPS ou 60 FPS), precisa ser convertido para milisegundos.

/* Vari�veis globais. */
GLfloat width, height;                                                // Armazena o tamanho da janela, para eventos de redimensionar a janela.
GLfloat diametro;                                                     // Armazena o diametro entre as bordas dos canos.
GLfloat x_position, y_position;                                       // Armazena a posi��o (x, y) para realizar a anima��o.
GLfloat x_InitPosition, y_InitPosition;                               // Armazena a posi��o (x, y) inicial para come�ar a (desenhar + animar).
GLfloat pts;                                                          // Vari�vel com a quantidade de pontos para fazer as curvas e c�rculos.
GLfloat tam, tamCruzeta;                                              // Armazena um valor de tamanho, para padronizar o desenho dos canos.
GLfloat angulo, radiano, aumento;                                     // Utilizado para fazer a rota��o nas anima��es dos semic�rculos.
GLfloat x_rotate, y_rotate, x_atual, y_atual, x_anterior, y_anterior; // Vari�veis para c�lculo do incremento em x e y nas anima��es dos semic�rculos.
GLfloat max_x, max_y;                                                 // Vari�veis para encurtar os parametros, armazenam o quanto j� foi transladado na anima��o.
GLfloat angulo2, radiano2, aumento2;                                  // Vari�veis para realizar o desenho das primitivas gota e canoJoelho().
GLint cena;                                                           // Define qual das 3 telas ser�o exibidas na janela.
GLint c, aux;                                                         // Vari�veis para auxiliar na execu��o da anima��o.

// 3D
GLfloat angle, fAspect, zNear, zFar;                   // Vari�veis representando os par�metros internos do observador dentro da rotina vis�o.
GLfloat eye[3], center[3], up[3];                      // Vetores para armazenar os par�metros de coordenadas do observador.
GLfloat rotX, rotY, rotX_init, rotY_init;              // Vari�veis para armazenar os valores para alterar a vis�o da janela.
GLint x_init, y_init, eyeX_init, eyeY_init, eyeZ_init; // Vari�veis para mover a c�mera pela cena.
GLint acionar;                                         // Vari�vel auxiliar, para armazenar qual bot�o do mouse foi pressionado.

/* Prot�tipos de rotinas. */

// Rotinas callback.
static void resize(int, int);
static void display();
static void key(unsigned char, int, int);
static void mouse(int, int, int, int);
static void motionFunc(int, int);
static void timer(int);

// Diversas rotinas.
void inicializa();
void planoCartesiano();
void tela1();
void tela2();
void tela3();
void tela4();
void animation1();
void animation2();
void animation3();
void rotaciona(int, int);
void DesenhaTexto(char *, int, int);

// 3D.
void visao();

// Primitivas.
void gota();
void canoReto(int);
void canoCruzeta(int);
void canoJoelho(int);

#endif // CABECALHO_H_INCLUDED
