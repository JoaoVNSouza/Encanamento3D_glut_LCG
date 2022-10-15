#ifndef CABECALHO_H_INCLUDED
#define CABECALHO_H_INCLUDED

/* Constantes. */
#define PI 3.14159    // Constante PI.
#define fps 1000 / 60 // Constante quantidade de frames em 1 segundo para animação (30 FPS ou 60 FPS), precisa ser convertido para milisegundos.

/* Variáveis globais. */
GLfloat width, height;                                                // Armazena o tamanho da janela, para eventos de redimensionar a janela.
GLfloat diametro;                                                     // Armazena o diametro entre as bordas dos canos.
GLfloat x_position, y_position;                                       // Armazena a posição (x, y) para realizar a animação.
GLfloat x_InitPosition, y_InitPosition;                               // Armazena a posição (x, y) inicial para começar a (desenhar + animar).
GLfloat pts;                                                          // Variável com a quantidade de pontos para fazer as curvas e círculos.
GLfloat tam, tamCruzeta;                                              // Armazena um valor de tamanho, para padronizar o desenho dos canos.
GLfloat angulo, radiano, aumento;                                     // Utilizado para fazer a rotação nas animações dos semicírculos.
GLfloat x_rotate, y_rotate, x_atual, y_atual, x_anterior, y_anterior; // Variáveis para cálculo do incremento em x e y nas animações dos semicírculos.
GLfloat max_x, max_y;                                                 // Variáveis para encurtar os parametros, armazenam o quanto já foi transladado na animação.
GLfloat angulo2, radiano2, aumento2;                                  // Variáveis para realizar o desenho das primitivas gota e canoJoelho().
GLint cena;                                                           // Define qual das 3 telas serão exibidas na janela.
GLint c, aux;                                                         // Variáveis para auxiliar na execução da animação.

// 3D
GLfloat angle, fAspect, zNear, zFar;                   // Variáveis representando os parâmetros internos do observador dentro da rotina visão.
GLfloat eye[3], center[3], up[3];                      // Vetores para armazenar os parâmetros de coordenadas do observador.
GLfloat rotX, rotY, rotX_init, rotY_init;              // Variáveis para armazenar os valores para alterar a visão da janela.
GLint x_init, y_init, eyeX_init, eyeY_init, eyeZ_init; // Variáveis para mover a câmera pela cena.
GLint acionar;                                         // Variável auxiliar, para armazenar qual botão do mouse foi pressionado.

/* Protótipos de rotinas. */

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
