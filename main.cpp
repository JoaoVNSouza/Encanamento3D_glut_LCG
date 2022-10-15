/*
 * Universidade Federal da Grande Dourados - UFGD.
 * Faculdade de Ci�ncias Exatas e Computa��o - FACET.
 * Curso Bacharelado em Engenharia de Computa��o.
 *
 * Disciplina: Laborat�rio de Computa��o Gr�fica - LCG.
 * Professor: Adailton Jos� Alves da Cruz.
 * Per�odo: semestre 2022.1 || ocorr�ncia no 2� semestre de 2022.]
 *
 * Programa: main.cpp
 * Desenvolvido por: Jo�o Vitor Nascimento de Souza.
 *
 * Objetivos: Desenhar um sistema de tubula��o com circuitos de canos em 3D.
 * - Diversos formatos de canos.
 * - Anima��o de uma gota.
 * - Som representativo.
 * - Exibi��o em diversas telas.
 */

// Bibliotecas utilizadas.
#include <GL/gl.h>     // Para interface gr�fica.
#include <GL/glut.h>   // Para interface gr�fica.
#include <windows.h>   // Para permitir execu��o de instru��es no sistema.
#include <math.h>      // Para fazer c�lculos de trigonometria.
#include <time.h>      // Para gerar n�meros aleat�rios baseado no tempo do computador.
#include <MMsystem.h>  // Para colocar som no sistema.
#include "cabecalho.h" // Para organizar o c�digo (vari�veis e m�todos)
#include <stdio.h>     // Para escrever no prompt.

/*
 * Fun��o principal: respons�vel pela execu��o do programa do in�cio ao fim.
 * Entrada: argumentos passados pelos prompt de comando.
 * Sa�da:
 * - EXIT_SUCESS: finalizado corretamente.
 * - Outros valors: ocorr�ncia de erros na execu��o.
 */
int main(int argc, char *argv[])
{
    inicializa();                                // Inicializa vari�veis globais.
    glutInit(&argc, argv);                       // Inicicializa com os argumentos passados no prompt de comando.
    glutInitWindowSize(width, height);           // Define o tamanho da janela em pixels.
    glutInitWindowPosition(300, 10);             // Define a posi��o do canto superior esquerda da janela.
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // Descreve as configura��es da janela (cores suportadas e suas caracter�sticas).
    glutCreateWindow("Encanamento 3D");          // Cria a janela e inseri um t�tulo.
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);        // Seleciona a cor de fundo para limpeza da tela (R, G, B, A).

    PlaySound(TEXT("pingo.wav"), NULL, SND_ASYNC); // Coloca um som de gota em background.

    /*
     * Rotinas callback:
     *  Se ocorrer um evento de sistema devido a intera��o com a janela criada
     *  aciona uma dessas rotinas, executando suas fun��es internas.
     */

    glutDisplayFunc(display);     // Executa o procedimento display.
    glutReshapeFunc(resize);      // Executa o procedimento resize.
    glutKeyboardFunc(key);        // Executa o procedimento key.
    glutMouseFunc(mouse);         // Executa o procedimento mouse.
    glutMotionFunc(motionFunc);   // Executa o procedimento motionFunc.
    glutTimerFunc(fps, timer, 0); // Executa o procedimento timer a cada per�odo de tempo t em (ms).

    glutMainLoop(); // Mant�m o programa executando em loop, aguardando a ocorr�ncia de novos eventos com a janela.

    return EXIT_SUCCESS;
}

/* Rotinas callback. */

/*
 * Fun��o respons�vel por adaptar a janela de visualiza��o (Viewport) e os elementos contidos na janela (Proje��o),
 * caso ocorra um evento de redimensionamento de janela.
 * Entrada:
 *  - w: nova largura.
 *  - h: nova altura.
 */
static void resize(int w, int h)
{
    width = w;
    height = h;

    glViewport(0, 0, width, height); // Porta de visualiza��o, onde ser� feitos os desenhos dos elementos.

    // Se altura for igual a 0, altera passa a ser 1.
    if (height == 0)
        height = 1;

    fAspect = width / height;

    visao();
}

/*
 * Procedimento respons�vel por executar instru��es caso ocorra o pressionamento de teclas na janela exibida.
 * Entrada: uma tecla digitada.
 * - "ESC": fecha o programa.
 * Sa�da: Executa uma instru��o.
 */
static void key(unsigned char letra, int x, int y)
{
    switch (letra)
    {
    case 27:
        exit(0);
        break;
    default:
        break;
    }

    visao();
    glutPostRedisplay(); // Atualiza o display a cada evento do teclado.
}

/*
 * Rotina para executar instru��es caso ocorra o pressionamento dos bot�es do mouse.
 */
static void mouse(int button, int state, int x, int y)
{
    // Mover observador utilizando o mouse.
    if (state == GLUT_DOWN)
    {
        // Salva as coordenadas atuais.
        x_init = x;
        y_init = y;
        eyeX_init = eye[0];
        eyeY_init = eye[1];
        eyeZ_init = eye[2];
        rotX_init = rotX;
        rotY_init = rotY;

        acionar = button;
    }
    else
        acionar = -1;
}

/*
 * Rotina para executar uma a��o caso o mouse seja deslocado pela janela.
 */
static void motionFunc(int x, int y)
{
    // Vari�veis.
    int variaX, variaY, variaZ;

    // Mover observador pelo mouse.
    if (acionar == GLUT_LEFT_BUTTON)
    {
        variaX = x_init - x;
        variaY = y_init - y;

        rotY = rotY_init - variaX / 5;
        rotX = rotX_init - variaY / 5;
    }
    else
    {
        if (acionar == GLUT_RIGHT_BUTTON)
        {
            variaZ = y_init - y;
            eye[2] = eyeZ_init + variaZ / 10;
        }
        else
        {
            if (acionar == GLUT_MIDDLE_BUTTON)
            {
                variaX = x_init - x;
                variaY = y_init - y;
                eye[0] = eyeX_init + variaX / 30;
                eye[1] = eyeY_init - variaY / 30;
            }
        }
    }

    visao();
    glutPostRedisplay();
}

/*
 * Procedimento respons�vel por atualizar o display da janela.
 *  -> Mostra todos os desenhos.
 */
static void display()
{
    switch (cena)
    {
    case 1: // Exibi a tela 1.
        // Reposiciona o observador e o ponto que est� olhando para melhor se adequar a cena.
        eye[0] = center[0] = 50;
        eye[1] = center[1] = 25;
        tela1();
        break;
    case 2: // Exibi a tela 2.
        // Reposiciona o observador e o ponto que est� olhando para melhor se adequar a cena.
        eye[0] = center[0] = 30;
        eye[1] = center[1] = -20;
        tela2();
        break;
    case 3: // Exibi a tela 3.
        // Reposiciona o observador e o ponto que est� olhando para melhor se adequar a cena.
        eye[0] = center[0] = 20;
        eye[1] = center[1] = 0;
        tela3();
        break;
    case 4: // Exibi a tela 4.
        tela4();
        break;
    default:
        break;
    }
}

/*
 * Procedimento que realiza as anima��es.
 */
static void timer(int valor)
{
    visao();
    glutPostRedisplay();          // Redesenha os objetos com as novas coordenadas.
    glutTimerFunc(fps, timer, 0); // Isso garante que seja executa inumeras vezes, mais de uma.

    switch (cena)
    {
    case 1:
        animation1();
        break;
    case 2:
        animation2();
        break;
    case 3:
        animation3();
        break;
    default:
        break;
    }
}

/* Telas utilizadas. */

void tela1()
{
    glClear(GL_COLOR_BUFFER_BIT); // Limpa o buffer de cores atual da janela.

    glLineWidth(4);     // Tamanho das bordas dos canos.
    glColor3f(1, 1, 0); // Cor dos canos Amarelo.

    DesenhaTexto("Cena 1", -75, 60); // Mostra texto Cena 1.

    /* Desenhar o circuito. */

    // Transforma��o 0.
    glTranslatef(x_InitPosition, y_InitPosition, 0); // T0
    tam = 30;                                        // Para padronizar o desenho, vai fazer todos os canos com tamanho relacionado a esse.
    canoReto(tam);

    // Transforma��o 1.
    glTranslatef(tam, 0, 0); // T1
    canoJoelho(360);

    // Transforma��o 2.
    glTranslatef((diametro / 2) + diametro, (diametro / 2) + diametro, 0); // T2
    glRotatef(90, 0, 0, 1);                                                // R0
    canoReto(tam * 3);

    // Transforma��o 3.
    glTranslatef(tam * 3, 0, 0); // T3
    canoJoelho(90);

    //  Transforma��o 4.
    glRotatef(-90, 0, 0, 1);                                               // R1
    glTranslatef((diametro / 2) + diametro, (diametro / 2) + diametro, 0); // T2
    canoReto(tam * 4);

    // Transforma��o 5.
    glTranslatef(tam * 4, 0, 0); // T4
    tamCruzeta = tam * 2;        // 60
    canoCruzeta(tamCruzeta);

    // Push.
    glTranslatef(tamCruzeta / 2, 0, 0); // T1 pois tamCruzeta / 2 = 30
    glPushMatrix();                     // Salva a matriz corrente.

    // Transforma��o 6.
    glTranslatef(0, tamCruzeta / 2, 0); // T5
    glRotatef(90, 0, 0, 1);             // R0
    canoJoelho(90);

    // Transforma��o 7.
    glRotatef(-90, 0, 0, 1);                                               // R1
    glTranslatef((diametro / 2) + diametro, (diametro / 2) + diametro, 0); // T2
    canoReto(tam * 2);

    // Transforma��o 8.
    glTranslatef(tam * 2, 0, 0); // T6
    canoJoelho(90);

    glPopMatrix(); // Baixa a matriz corrente da mem�ria.

    glPushMatrix(); // Salva a matriz corrente.

    // Transforma��o 9.
    glTranslatef(tamCruzeta / 2, 0, 0); // T1
    canoReto(tam * 2);

    // Transforma��o 10.
    glTranslatef(tam * 2, 0, 0); // T6
    canoCruzeta(tamCruzeta);

    glPopMatrix(); // Baixa a matriz corrente da mem�ria.

    glPushMatrix(); // Salva a matriz corrente.

    // Transforma��o 11.
    glTranslatef(0, -tamCruzeta / 2, 0); // T7
    glRotatef(-90, 0, 0, 1);             // R1
    canoJoelho(360);

    // Transforma��o 12.
    glRotatef(90, 0, 0, 1);                                                 // R0
    glTranslatef((diametro + diametro / 2), -(diametro + diametro / 2), 0); // T8
    canoReto(tam * 2);

    // Transforma��o 13.
    glTranslatef(tam * 2, 0, 0); // T6
    canoJoelho(360);
    glPopMatrix(); // Baixa a matriz corrente da mem�ria.

    // Transforma��o 14.
    glTranslatef(((tamCruzeta / 2) * 3) + tam * 2, 0, 0); // T9
    canoReto(tam * 3);

    /* Anima��o. */
    glLoadIdentity();
    visao();
    glTranslatef(x_InitPosition, y_InitPosition, 0); // Translada para a posi��o inicial do circuito.
    glTranslatef(x_position, y_position, 0);         // Vai transladar a gota por todo o circuito.
    gota();                                          // Desenha a gota circular.

    glutSwapBuffers(); // Necess�rio para anima��o para fazer a troca entre buffers e continua executando fun��es que n�o foram executadas.
}

void tela2()
{
    glClear(GL_COLOR_BUFFER_BIT); // Limpa o buffer de cores atual da janela.

    glLineWidth(4);     // Tamanho das bordas dos canos.
    glColor3f(0, 1, 1); // Cor dos canos Azul claro.

    DesenhaTexto("Cena 2", -75, 60); // Mostra texto Cena 2.

    /* Desenhar o circuito. */

    // Transforma��o 0.
    glTranslatef(x_InitPosition, y_InitPosition, 0);
    tam = 30; // Para padronizar o desenho, vai fazer todos os canos com tamanho relacionado a esse.
    canoReto(tam);

    // Transforma��o 1.
    glTranslatef(tam, 0, 0); // T1
    canoJoelho(90);

    // Transforma��o 2.
    glTranslatef((diametro + diametro / 2), -(diametro + diametro / 2), 0); // T8
    glRotatef(-90, 0, 0, 1);                                                // R1
    canoJoelho(360);

    // Transforma��o 3.
    glRotatef(90, 0, 0, 1);                                                 // R0
    glTranslatef((diametro + diametro / 2), -(diametro + diametro / 2), 0); // T8
    canoReto(tam * 3);

    // Transforma��o 4.
    glTranslatef(tam * 3, 0, 0); // T3
    canoJoelho(360);

    // Transforma��o 5.
    glTranslatef((diametro + diametro / 2), (diametro + diametro / 2), 0); // T2
    glRotatef(90, 0, 0, 1);                                                // R0
    canoReto(tam * 2);

    // Transforma��o 6.
    glTranslatef(tam * 2, 0, 0); // T6
    tamCruzeta = tam * 2;
    canoCruzeta(tamCruzeta);

    // Push.
    glTranslatef((tamCruzeta / 2), 0, 0); // T1 pois TamCruzeta = tam
    glPushMatrix();                       // Salva a matriz corrente no meio da Cruzeta.

    // Transforma��o 7.
    glRotatef(-90, 0, 0, 1);                        // R1
    glTranslatef(-(diametro + diametro / 2), 0, 0); // T10
    canoJoelho(270);

    // Transforma��o 8.
    glTranslatef(-(diametro + diametro / 2), (diametro + diametro / 2), 0); // T11
    glRotatef(90, 0, 0, 1);                                                 // R0
    canoJoelho(90);

    // Transforma��o 9.
    glRotatef(-90, 0, 0, 1);                                               // R1
    glTranslatef((diametro + diametro / 2), (diametro + diametro / 2), 0); // T2
    canoJoelho(90);

    // Pop.
    glPopMatrix(); // Baixa a matriz corrente da mem�ria.

    // Transforma��o 10.
    glRotatef(-90, 0, 0, 1);              // R1.
    glTranslatef((tamCruzeta / 2), 0, 0); // T1 pois TamCruzeta = tam
    canoReto(tam * 2);

    /* Anima��o. */
    glLoadIdentity();
    visao();
    glTranslatef(x_InitPosition, y_InitPosition, 0); // Translada para a posi��o inicial do circuito.
    glTranslatef(x_position, y_position, 0);         // Vai transladar a gota por todo o circuito.
    gota();                                          // Desenha a gota circular.

    glutSwapBuffers(); // Necess�rio para anima��o para fazer a troca entre buffers e continua executando fun��es que n�o foram executadas.
}

void tela3()
{
    glClear(GL_COLOR_BUFFER_BIT); // Limpa o buffer de cores atual da janela.

    glLineWidth(4);     // Tamanho das bordas dos canos.
    glColor3f(0, 1, 0); // Cor dos canos Verde.

    DesenhaTexto("Cena 3", -75, 60); // Mostra texto Cena 3.

    /* Desenhar o circuito. */

    // Transforma��o 0.
    glTranslatef(x_InitPosition, y_InitPosition, 0); // T0
    tam = 30;
    canoReto(tam);

    // Transforma��o 1.
    glTranslatef(tam, 0, 0); // T1
    canoJoelho(360);

    // Transforma��o 2.
    glTranslatef((diametro + diametro / 2), (diametro + diametro / 2), 0); // T2
    glRotatef(90, 0, 0, 1);                                                // R0
    canoReto(tam * 3);

    // Transforma��o 3.
    glTranslatef(tam * 3, 0, 0); // T3
    canoJoelho(90);

    // Transforma��o 4.
    glRotatef(-90, 0, 0, 1);                                               // R1
    glTranslatef((diametro + diametro / 2), (diametro + diametro / 2), 0); // T2
    canoJoelho(90);

    // Transforma��o 5.
    glTranslatef((diametro + diametro / 2), -(diametro + diametro / 2), 0); // T8
    glRotatef(-90, 0, 0, 1);                                                // R1
    canoReto(tam * 3);

    // Transforma��o 6.
    glTranslatef(tam * 3, 0, 0); // T3
    canoJoelho(360);

    // Transforma��o 7.
    glRotatef(90, 0, 0, 1);                                                 // R0
    glTranslatef((diametro + diametro / 2), -(diametro + diametro / 2), 0); // T8
    canoJoelho(360);

    // Transforma��o 8.
    glTranslatef((diametro + diametro / 2), (diametro + diametro / 2), 0); // T2
    glRotatef(90, 0, 0, 1);                                                // R0
    canoReto(tam * 3);

    // Transforma��o 9.
    glTranslatef(tam * 3, 0, 0); // T3
    canoJoelho(90);

    // Transforma��o 10.
    glRotatef(-90, 0, 0, 1);                                               // R1
    glTranslatef((diametro + diametro / 2), (diametro + diametro / 2), 0); // T2
    canoJoelho(90);

    // Transforma�a� 11.
    glTranslatef((diametro + diametro / 2), -(diametro + diametro / 2), 0); // T8
    glRotatef(-90, 0, 0, 1);                                                // R1
    canoReto(tam * 3);

    // Transforma��o 12.
    glTranslatef(tam * 3, 0, 0); // T3
    canoJoelho(360);

    // Transforma��o 13.
    glRotatef(90, 0, 0, 1);                                                 // R0
    glTranslatef((diametro + diametro / 2), -(diametro + diametro / 2), 0); // T8
    canoReto(tam);

    /* Anima��o. */
    glLoadIdentity();
    visao();
    glTranslatef(x_InitPosition, y_InitPosition, 0); // Translada para a posi��o inicial do circuito.
    glTranslatef(x_position, y_position, 0);         // Vai transladar a gota por todo o circuito.
    gota();                                          // Desenha a gota circular.

    glutSwapBuffers(); // Necess�rio para anima��o para fazer a troca entre buffers e continua executando fun��es que n�o foram executadas.
}

void tela4()
{
    glClear(GL_COLOR_BUFFER_BIT); // Limpa o buffer de cores atual da janela.

    glLoadIdentity();
    visao();

    // Mostra um quadrado atr�s da mensagem.
    glColor3f(0.5, 0.5, 0.5);
    glPointSize(10);
    glBegin(GL_QUADS);
    glVertex3f(-50, -50, -50);
    glVertex3f(150, -50, -50);
    glVertex3f(150, 50, -50);
    glVertex3f(-50, 50, -50);
    glEnd();

    glColor3f(0, 1, 1); // Cor da msg azul claro.
    // Mostra mensagem de FINISH na tela.
    DesenhaTexto("FINISH - Pressione ESC", -15, -5);

    // Desliga o som.
    PlaySound(NULL, 0, 0);

    glutSwapBuffers();
}

/* Anima��es utilizadas. */

void animation1()
{
    switch (c)
    {
    case 0: // Move pelo setor 0.
        if (x_position < tam)
            x_position += 1;
        else
        {
            c++;
            x_position = max_x += tam;
            y_position = max_y += 0;
        }
        // x = 30, y = 0.
        break;

    case 1: // Move pelo setor 1.
        if (y_position < max_y + (diametro + diametro / 2))
            rotaciona(270, 360);
        else
        {
            c++;
            aux++;
            x_position = max_x += (diametro + diametro / 2);
            y_position = max_y += (diametro + diametro / 2);
        }
        // x = 60, y = 30.
        break;

    case 2: // Move pelo setor 2.
        if (y_position < max_y + (tam * 3))
            y_position += 1;
        else
        {
            c++;
            x_position = max_x += 0;
            y_position = max_y += (tam * 3);
        }
        // x = 60, y = 120.
        break;

    case 3: // Move pelo setor 3.
        if (x_position < max_x + (diametro + diametro / 2))
            rotaciona(180, 90);
        else
        {
            c++;
            aux++;
            x_position = max_x += (diametro + diametro / 2);
            y_position = max_y += (diametro + diametro / 2);
        }
        // x = 90, y = 150.
        break;

    case 4: // Move pelo setor 4 e para o meio do setor 5.
        if (x_position < max_x + (tam * 4) + (tamCruzeta / 2))
            x_position += 1;
        else
        {
            // Escolher qual dos 3 caminhos ir� seguir.
            srand(time(NULL));

            switch (rand() % 3)
            {
            case 0: // Para cima da cruzeta.
                c = 5;
                break;
            case 1: // Para direita da cruzeta.
                c = 11;
                break;
            case 2: // Para baixo da cruzeta.
                c = 12;
                break;
            }

            x_position = max_x += (tam * 4) + (tamCruzeta / 2);
            y_position = max_y += 0;
        }
        // x = 240, y = 150.
        break;

    case 5: // Move pelo setor 5.
        if (y_position < max_y + (tamCruzeta / 2))
            y_position += 1;
        else
        {
            c++;
            x_position = max_x += 0;
            y_position = max_y += (tamCruzeta / 2);
        }
        // x = 240, y = 180.
        break;

    case 6: // Move pelo setor 6.
        if (x_position < max_x + (diametro + diametro / 2))
            rotaciona(180, 90);
        else
        {
            c++;
            aux++;
            x_position = max_x += (diametro + diametro / 2);
            y_position = max_y += (diametro + diametro / 2);
        }
        // x = 270, y = 210.
        break;

    case 7: // Move pelo setor 7.
        if (x_position < max_x + tam * 2)
            x_position += 1;
        else
        {
            c++;
            x_position = max_x += tam * 2;
            y_position = max_y += 0;
        }
        // x = 330, y = 210.
        break;

    case 8: // Move pelo setor 8.
        if (y_position > max_y - (diametro + diametro / 2))
            rotaciona(90, 0);
        else
        {
            c++;
            aux++;
            x_position = max_x += (diametro + diametro / 2);
            y_position = max_y -= (diametro + diametro / 2);
        }
        // x = 360, y = 180.
        break;

    case 9: // Move pelo setor 10.
        if (y_position > max_y - (tamCruzeta / 2))
            y_position -= 1;
        else
        {
            c++;
            x_position = max_x += 0;
            y_position = max_y -= (tamCruzeta / 2);
        }
        // x = 360, y = 150.
        break;

    case 10: // Move pelo setor 14.
        if (x_position < max_x + (tamCruzeta / 2) + (tam * 3))
            x_position += 1;
        else // Muda de cena.
            c = 100;
        // x = 480, y = 150.
        break;

    case 11: // Move pelo setor 9, 10 e 14.
        if (x_position < max_x + (tamCruzeta / 2) * 3 + (tam * 5))
            x_position += 1;
        else // Muda de cena.
            c = 100;

        break;

    case 12: // Move pelo setor 5.
        if (y_position > max_y - (tamCruzeta / 2))
            y_position -= 1;
        else
        {
            c++;
            x_position = max_x += 0;
            y_position = max_y -= (tamCruzeta / 2);
        }
        // x = 240, y = 120.
        break;

    case 13: // Move pelo setor 11.
        if (x_position < max_x + (diametro + diametro / 2))
            rotaciona(180, 270);
        else
        {
            c++;
            aux++;
            x_position = max_x += (diametro + diametro / 2);
            y_position = max_y -= (diametro + diametro / 2);
        }
        // x = 270, y = 90.
        break;

    case 14: // Move pelo setor 12.
        if (x_position < max_x + tam * 2)
            x_position += 1;
        else
        {
            c++;
            x_position = max_x += tam * 2;
            y_position = max_y += 0;
        }
        // x = 330, y = 90.
        break;

    case 15: // Move pelo setor 13.
        if (x_position < max_x + (diametro + diametro / 2))
            rotaciona(270, 360);
        else
        {
            c++;
            aux++;
            x_position = max_x += (diametro + diametro / 2);
            y_position = max_y += (diametro + diametro / 2);
        }
        // x = 360, y = 120.
        break;

    case 16: // Move pelo setor 10.
        if (y_position < max_y + tamCruzeta / 2)
            y_position += 1;
        else
        {
            c++;
            x_position = max_x += 0;
            y_position = max_y += (tamCruzeta / 2);
        }
        // x = 360, y = 150.
        break;

    case 17: // Move pelo setor 14.
        if (x_position < max_x + (tamCruzeta / 2) + (tam * 3))
            x_position += 1;
        else // Muda de cena.
            c = 100;
        // x = 480, y = 150.
        break;

    case 100: // Muda de cena.
        cena++;
        x_position = max_x = 0;
        y_position = max_y = 0;
        c = 0;
        break;
    }
}

void animation2()
{
    switch (c)
    {
    case 0: // Move pelo setor 0.
        if (x_position < tam)
            x_position += 1;
        else
        {
            c++;
            x_position = max_x += tam;
            y_position = max_y += 0;
        }
        // x = 30, y = 0.
        break;

    case 1: // Move pelo setor 1.
        if (x_position < max_x + (diametro + diametro / 2))
            rotaciona(90, 0);
        else
        {
            c++;
            aux++;
            x_position = max_x += (diametro + diametro / 2);
            y_position = max_y -= (diametro + diametro / 2);
        }
        // x = 60, y = -30.
        break;

    case 2: // Move pelo setor 2.
        if (x_position < max_x + (diametro + diametro / 2))
            rotaciona(180, 270);
        else
        {
            c++;
            aux++;
            x_position = max_x += (diametro + diametro / 2);
            y_position = max_y -= (diametro + diametro / 2);
        }
        // x = 90, y = -60.
        break;

    case 3: // Move pelo setor 3.
        if (x_position < max_x + (tam * 3))
            x_position += 1;
        else
        {
            c++;
            x_position = max_x += (tam * 3);
            y_position = max_y += 0;
        }
        // x = 180, y = -60.
        break;

    case 4: // Move pelo setor 4.
        if (x_position < max_x + (diametro + diametro / 2))
            rotaciona(270, 360);
        else
        {
            c++;
            aux++;
            x_position = max_x += (diametro + diametro / 2);
            y_position = max_y += (diametro + diametro / 2);
        }
        // x = 210, y = -30.
        break;

    case 5: // Move pelo setor 5 e meio do 6.
        if (y_position < max_y + (tam * 2) + (tamCruzeta / 2))
            y_position += 1;
        else
        {
            // Escolher qual dos 3 caminhos ir� seguir, ser� gerado um valor para isso.
            srand(time(NULL));
            switch (rand() % 2)
            {
            case 0: // Para cima da cruzeta.
                c = 6;
                break;
            case 1: // Para esquerda da cruzeta.
                c = 10;
                break;
            }
            x_position = max_x += 0;
            y_position = max_y += (tam * 2) + (tamCruzeta / 2);
        }
        // x = 210, y = 60.
        break;

    case 6: // Move pelo setor 6.
        if (y_position < max_y + (tamCruzeta / 2))
            y_position += 1;
        else
        {
            c++;
            max_x += 0;
            max_y += (tamCruzeta / 2);
        }
        // x = 210, y = 90.
        break;

    case 7: // Move pelo setor 9.
        if (x_position >= max_x - (diametro + diametro / 2))
            rotaciona(0, 90);
        else
        {
            c++;
            aux++;
            x_position = max_x -= (diametro + diametro / 2);
            y_position = max_y += (diametro + diametro / 2);
        }
        // x = 180, y = 120.
        break;

    case 8: // Move pelo setor 8.
        if (y_position > max_y - (diametro + diametro / 2))
            rotaciona(90, 180);
        else
        {
            c++;
            aux++;
            x_position = max_x -= (diametro + diametro / 2);
            y_position = max_y -= (diametro + diametro / 2);
        }
        // x = 150, y = 90.
        break;

    case 9: // Move pelo setor 7.
        if (x_position < max_x + (diametro + diametro / 2))
            rotaciona(180, 270);
        else
        {
            c++;
            aux++;
            x_position = max_x += (diametro + diametro / 2);
            y_position = max_y -= (diametro + diametro / 2);
        }
        // x = 180, y = 60.
        break;

    case 10: // Move pelo setor 10.
        if (x_position < max_x + (tamCruzeta) + (tam * 2))
            x_position += 1;
        else // Muda de cena.
            c = 100;
        // x = 300, y = 60.
        break;

    case 100: // Muda de cena
        cena++;
        x_position = max_x = 0;
        y_position = max_y = 0;
        c = 0;
        break;
    }
}

void animation3()
{
    switch (c)
    {
    case 0: // Move pelo setor 0.
        if (x_position < tam)
            x_position += 1;
        else
        {
            c++;
            x_position = max_x += tam;
            y_position = max_y += 0;
        }
        break;

    case 1: // Move pelo setor 1.
        if (x_position < max_x + (diametro + diametro / 2))
            rotaciona(270, 360);
        else
        {
            c++;
            aux++;
            x_position = max_x += (diametro + diametro / 2);
            y_position = max_y += (diametro + diametro / 2);
        }
        break;

    case 2: // Move pelo setor 2.
        if (y_position < max_y + (tam * 3))
            y_position += 1;
        else
        {
            c++;
            x_position = max_x += 0;
            y_position = max_y += (tam * 3);
        }
        break;

    case 3: // Move pelo setor 3.
        if (x_position < max_x + (diametro + diametro / 2))
            rotaciona(180, 90);
        else
        {
            c++;
            aux++;
            x_position = max_x += (diametro + diametro / 2);
            y_position = max_y += (diametro + diametro / 2);
        }
        break;

    case 4: // Move pelo setor 4.
        if (y_position > max_y - (diametro + diametro / 2))
            rotaciona(90, 0);
        else
        {
            c++;
            aux++;
            x_position = max_x += (diametro + diametro / 2);
            y_position = max_y -= (diametro + diametro / 2);
        }
        break;

    case 5: // Move pelo setor 5.
        if (y_position > max_y - (tam * 3))
            y_position -= 1;
        else
        {
            c++;
            x_position = max_x += 0;
            y_position = max_y -= (tam * 3);
        }
        break;

    case 6: // Move pelo setor 6.
        if (y_position > max_y - (diametro + diametro / 2))
            rotaciona(180, 270);
        else
        {
            c++;
            aux++;
            x_position = max_x += (diametro + diametro / 2);
            y_position = max_y -= (diametro + diametro / 2);
        }
        break;

    case 7: // Move pelo setor 7.
        if (x_position < max_x + (diametro + diametro / 2))
            rotaciona(270, 360);
        else
        {
            c++;
            aux++;
            x_position = max_x += (diametro + diametro / 2);
            y_position = max_y += (diametro + diametro / 2);
        }
        break;

    case 8: // Move pelo setor 8.
        if (y_position < max_y + (tam * 3))
            y_position += 1;
        else
        {
            c++;
            x_position = max_x += 0;
            y_position = max_y += (tam * 3);
        }
        break;

    case 9: // Move pelo setor 9.
        if (x_position < max_x + (diametro + diametro / 2))
            rotaciona(180, 90);
        else
        {
            c++;
            aux++;
            x_position = max_x += (diametro + diametro / 2);
            y_position = max_y += (diametro + diametro / 2);
        }
        break;

    case 10: // Move pelo setor 10.
        if (y_position > max_y - (diametro + diametro / 2))
            rotaciona(90, 0);
        else
        {
            c++;
            aux++;
            x_position = max_x += (diametro + diametro / 2);
            y_position = max_y -= (diametro + diametro / 2);
        }
        break;

    case 11: // Move pelo setor 11.
        if (y_position > max_y - (tam * 3))
            y_position -= 1;
        else
        {
            c++;
            x_position = max_x += 0;
            y_position = max_y -= (tam * 3);
        }
        break;

    case 12: // Move pelo setor 12.
        if (y_position > max_y - (diametro + diametro / 2))
            rotaciona(180, 270);
        else
        {
            c++;
            aux++;
            x_position = max_x += (diametro + diametro / 2);
            y_position = max_y -= (diametro + diametro / 2);
        }
        break;

    case 13: // Move pelo setor 13.
        if (x_position < max_x + (tam))
            x_position += 1;
        else // Muda cena.
            cena = 4;
        break;
    }
}

/* Primitivas. */

/*
 * Primitiva para desenhar cano no formato de cruzeta 3D no eixo x.
 */
void canoCruzeta(int tamanho)
{
    GLfloat meioCruzeta = tamanho / 2;

    // Parte 1.
    glBegin(GL_QUADS);
    glVertex3f(0, diametro / 2, -diametro / 2);
    glVertex3f(0, diametro / 2, diametro / 2);
    glVertex3f(meioCruzeta - diametro / 2, diametro / 2, diametro / 2);
    glVertex3f(meioCruzeta - diametro / 2, diametro / 2, -diametro / 2);

    glVertex3f(meioCruzeta - diametro / 2, diametro / 2, -diametro / 2);
    glVertex3f(meioCruzeta - diametro / 2, meioCruzeta, -diametro / 2);
    glVertex3f(meioCruzeta - diametro / 2, meioCruzeta, diametro / 2);
    glVertex3f(meioCruzeta - diametro / 2, diametro / 2, diametro / 2);
    glEnd();

    //  Parte 2.
    glBegin(GL_QUADS);
    glVertex3f(meioCruzeta + diametro / 2, diametro / 2, -diametro / 2);
    glVertex3f(meioCruzeta + diametro / 2, meioCruzeta, -diametro / 2);
    glVertex3f(meioCruzeta + diametro / 2, meioCruzeta, diametro / 2);
    glVertex3f(meioCruzeta + diametro / 2, diametro / 2, diametro / 2);

    glVertex3f(meioCruzeta + diametro / 2, diametro / 2, -diametro / 2);
    glVertex3f(tamanho, diametro / 2, -diametro / 2);
    glVertex3f(tamanho, diametro / 2, diametro / 2);
    glVertex3f(meioCruzeta + diametro / 2, diametro / 2, diametro / 2);
    glEnd();

    //  Parte 3.
    glBegin(GL_QUADS);
    glVertex3f(0, -diametro / 2, -diametro / 2);
    glVertex3f(meioCruzeta - diametro / 2, -diametro / 2, -diametro / 2);
    glVertex3f(meioCruzeta - diametro / 2, -diametro / 2, diametro / 2);
    glVertex3f(0, -diametro / 2, diametro / 2);

    glVertex3f(meioCruzeta - diametro / 2, -meioCruzeta, -diametro / 2);
    glVertex3f(meioCruzeta - diametro / 2, -diametro / 2, -diametro / 2);
    glVertex3f(meioCruzeta - diametro / 2, -diametro / 2, diametro / 2);
    glVertex3f(meioCruzeta - diametro / 2, -meioCruzeta, diametro / 2);
    glEnd();

    // Parte 4.
    glBegin(GL_QUADS);
    glVertex3f(meioCruzeta + diametro / 2, -meioCruzeta, -diametro / 2);
    glVertex3f(meioCruzeta + diametro / 2, -diametro / 2, -diametro / 2);
    glVertex3f(meioCruzeta + diametro / 2, -diametro / 2, diametro / 2);
    glVertex3f(meioCruzeta + diametro / 2, -meioCruzeta, diametro / 2);

    glVertex3f(meioCruzeta + diametro / 2, -diametro / 2, -diametro / 2);
    glVertex3f(tamanho, -diametro / 2, -diametro / 2);
    glVertex3f(tamanho, -diametro / 2, diametro / 2);
    glVertex3f(meioCruzeta + diametro / 2, -diametro / 2, diametro / 2);
    glEnd();

    // Preenchimento horizontal
    glBegin(GL_QUADS);
    // Atr�s.
    glVertex3f(0, diametro / 2, -diametro / 2);
    glVertex3f(tamanho, diametro / 2, -diametro / 2);
    glVertex3f(tamanho, -diametro / 2, -diametro / 2);
    glVertex3f(0, -diametro / 2, -diametro / 2);

    // Frente.
    glVertex3f(0, diametro / 2, diametro / 2);
    glVertex3f(tamanho, diametro / 2, diametro / 2);
    glVertex3f(tamanho, -diametro / 2, diametro / 2);
    glVertex3f(0, -diametro / 2, diametro / 2);
    glEnd();

    // Preenchimento Vertical.
    glBegin(GL_QUADS);
    // Atr�s.
    glVertex3f(meioCruzeta - diametro / 2, -meioCruzeta, -diametro / 2);
    glVertex3f(meioCruzeta - diametro / 2, meioCruzeta, -diametro / 2);
    glVertex3f(meioCruzeta + diametro / 2, meioCruzeta, -diametro / 2);
    glVertex3f(meioCruzeta + diametro / 2, -meioCruzeta, -diametro / 2);

    // Frente.
    glVertex3f(meioCruzeta - diametro / 2, -meioCruzeta, diametro / 2);
    glVertex3f(meioCruzeta - diametro / 2, meioCruzeta, diametro / 2);
    glVertex3f(meioCruzeta + diametro / 2, meioCruzeta, diametro / 2);
    glVertex3f(meioCruzeta + diametro / 2, -meioCruzeta, diametro / 2);
    glEnd();
}

/*
 * Primitiva para desenhar um ret�ngulo 3D no eixo x.
 */
void canoReto(int tamanho)
{
    // Quadrado superior.
    glBegin(GL_QUADS);
    glVertex3f(0, diametro / 2, diametro / 2);
    glVertex3f(0, diametro / 2, -diametro / 2);
    glVertex3f(tamanho, diametro / 2, -diametro / 2);
    glVertex3f(tamanho, diametro / 2, diametro / 2);
    glEnd();

    // Quadrado inferior.
    // Aplica uma transforma��o geom�trica Linear de REFLEX�O em y.
    glBegin(GL_QUADS);
    glVertex3f(0, -diametro / 2, diametro / 2);
    glVertex3f(0, -diametro / 2, -diametro / 2);
    glVertex3f(tamanho, -diametro / 2, -diametro / 2);
    glVertex3f(tamanho, -diametro / 2, diametro / 2);
    glEnd();

    // Quadrado atr�s.
    glBegin(GL_QUADS);
    glVertex3f(0, -diametro / 2, -diametro / 2);
    glVertex3f(0, diametro / 2, -diametro / 2);
    glVertex3f(tamanho, diametro / 2, -diametro / 2);
    glVertex3f(tamanho, -diametro / 2, -diametro / 2);
    glEnd();

    // Quadrado frente.
    // Aplica uma transforma��o geom�trica Linear de REFLEX�O em z.
    glBegin(GL_QUADS);
    glVertex3f(0, -diametro / 2, diametro / 2);
    glVertex3f(0, diametro / 2, diametro / 2);
    glVertex3f(tamanho, diametro / 2, diametro / 2);
    glVertex3f(tamanho, -diametro / 2, diametro / 2);
    glEnd();
}

/*
 * Primitiva para desenhar a gota no centro do eixo de coordenadas.
 */
void gota()
{
    glColor3f(0, 0, 1); // Cor Azul.
    glutSolidSphere(diametro / 4, 64, 64);
}

/*
 * Primitiva para desenhar at� 4 formas do semicirculo de um cano, variando o Theta:
 * - 90�: desenha c�rculo de 0� at� 90�.
 * - 180�: desenha c�rculo de 90� at� 180�.
 * - 270�: desenha c�rculo de 180� at� 270�.
 * - 360�: desenha c�rculo de 270� at� 360�.
 */
void canoJoelho(int theta)
{
    // Vari�veis locais.
    int i;

    glPushMatrix(); // Salva a matriz corrente.

    // Translada o canoJoelho para ser desenhado no centro do eixo.
    if (theta >= 270)
        glTranslatef(0, diametro + (diametro / 2), 0);
    else
        glTranslatef(0, -(diametro + (diametro / 2)), 0);

    // Frente.
    glBegin(GL_QUAD_STRIP);
    angulo2 = theta - 90;
    for (i = 0; i < pts; i++)
    {
        radiano2 = angulo2 * (PI / 180); // Convers�o de �ngulos para radianos.
        glVertex3f(diametro * cos(radiano2), diametro * sin(radiano2), diametro / 2);
        glVertex3f(diametro * 2 * cos(radiano2), diametro * 2 * sin(radiano2), diametro / 2);
        angulo2 += aumento2;
    }
    glEnd();

    // Tr�s.
    // Aplica uma transforma��o de reflex�o no desenho da frente.
    glBegin(GL_QUAD_STRIP);
    angulo2 = theta - 90;
    for (i = 0; i < pts; i++)
    {
        radiano2 = angulo2 * (PI / 180); // Convers�o de �ngulos para radianos.
        glVertex3f(diametro * cos(radiano2), diametro * sin(radiano2), -diametro / 2);
        glVertex3f(diametro * 2 * cos(radiano2), diametro * 2 * sin(radiano2), -diametro / 2);
        angulo2 += aumento2;
    }
    glEnd();

    // Superior.
    glBegin(GL_QUAD_STRIP);
    angulo2 = theta - 90;
    for (i = 0; i < pts; i++)
    {
        radiano2 = angulo2 * (PI / 180); // Convers�o de �ngulos para radianos.
        glVertex3f(diametro * cos(radiano2), diametro * sin(radiano2), diametro / 2);
        glVertex3f(diametro * cos(radiano2), diametro * sin(radiano2), -diametro / 2);
        angulo2 += aumento2;
    }
    glEnd();

    // Inferior.
    glBegin(GL_QUAD_STRIP);
    angulo2 = theta - 90;
    for (i = 0; i < pts; i++)
    {
        radiano2 = angulo2 * (PI / 180); // Convers�o de �ngulos para radianos.
        glVertex3f(diametro * 2 * cos(radiano2), diametro * 2 * sin(radiano2), diametro / 2);
        glVertex3f(diametro * 2 * cos(radiano2), diametro * 2 * sin(radiano2), -diametro / 2);
        angulo2 += aumento2;
    }
    glEnd();

    glPopMatrix(); // Baixa a matriz armazenada na mem�ria.
}

/* Outras rotinas. */

/*
 * Espec�fica os par�metros de visualiza��o 3D.
 * Definindo os par�metros internos e de coordenadas do observador em rela��o ao espa�o.
 */
void visao()
{
    // Par�metros internos do observador.
    glMatrixMode(GL_PROJECTION);                 // Inicializa os valores de visualiza��o.
    glLoadIdentity();                            // Faz com que a matriz corrente seja inicializada com a matriz identidade.
    gluPerspective(angle, fAspect, zNear, zFar); // Especifica a proje��o perspectiva.

    // Par�metros do sistema de coordenadas do observador.
    glMatrixMode(GL_MODELVIEW);                                                              // Matriz corrente, contendo todas as transforma��es geom�tricas em um determinado momento.
    glLoadIdentity();                                                                        // Faz com que a matriz corrente seja inicializada com a matriz identidade.
    gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]); // Especifica a posi��o, o alvo do observador e a inclina��o do observador.

    // Transforma��es geom�tricas para mover a c�mera pela cena com o mouse.
    glTranslatef(-eye[0], -eye[1], -eye[2]); // Translada a c�mera.
    glRotatef(rotX, 1, 0, 0);                // Rotacionar em X.
    glRotatef(rotY, 0, 1, 0);                // Rotacionar em Y.
}

/*
 * Desenha plano cartesiano no centro dos eixos de coordenadas.
 */
void planoCartesiano()
{

    // Desenha as linhas.
    glBegin(GL_LINES);

    // x.
    glColor3f(1, 1, 1); // Cor Branco.
    glVertex3f(-50, 0, 0);
    glVertex3f(50, 0, 0);

    // y.
    glColor3f(1, 0, 0); // Cor Vermelho.
    glVertex3f(0, -50, 0);
    glVertex3f(0, 50, 0);

    // z.
    glColor3f(0, 0, 1); // Cor Azul.
    glVertex3f(0, 0, -50);
    glVertex3f(0, 0, 50);
    glEnd();
}

/*
 * Inicializa��o das vari�veis globais.
 */
void inicializa()
{
    width = 1024;
    height = 768;
    diametro = 20;
    x_InitPosition = -100;
    y_InitPosition = -100;
    x_position = max_x = 0;
    y_position = max_y = 0;
    cena = 1;
    pts = 30;
    aux = 1;
    c = 0;

    aumento2 = 95 / pts; // Vai aumentar at� 90�.
    aumento = 90 / pts;  // Vai aumentar at� 90�.

    // Var 3D.
    zNear = 0.1;
    zFar = 700;
    angle = 60;
    eye[0] = 10;    // eyex.
    eye[1] = 0;     // eyey.
    eye[2] = 200;   // eyez.
    center[0] = 10; // centerx.
    center[1] = 0;  // centery.
    center[2] = 0;  // centerz.
    up[0] = 0;      // upx.
    up[1] = 1;      // upy.
    up[2] = 0;      // upz.

    SetConsoleTitle(" Encanamento 3D | Joao Vitor"); // Coloca um t�tulo no prompt.
}

/*
 * Procedimento para rotacionar a gota pelas curvas.
 */
void rotaciona(int inicio, int fim)
{
    if (aux == 1)
    {
        angulo = inicio; // inicio� at� fim�.
        aux = 0;

        switch (inicio)
        {
        case 0:
        case 360:
            x_atual = (diametro + diametro / 2);
            y_atual = 0;
            break;
        case 90:
            x_atual = 0;
            y_atual = (diametro + diametro / 2);
            break;
        case 180:
            x_atual = -(diametro + diametro / 2);
            y_atual = 0;
            break;
        case 270:
            x_atual = 0;
            y_atual = -(diametro + diametro / 2);
            break;
        default:
            break;
        }
    }

    radiano = angulo * (PI / 180); // Convers�o de �ngulos para radianos.

    // Calcula todos os pontos necess�rios e subtrai para ver qual deve ser o incremento.
    x_anterior = x_atual;
    x_atual = (diametro + diametro / 2) * cos(radiano);
    x_rotate = x_atual - x_anterior;

    y_anterior = y_atual;
    y_atual = (diametro + diametro / 2) * sin(radiano);
    y_rotate = y_atual - y_anterior;

    if (fim > inicio)
        angulo += aumento;
    else
        angulo -= aumento;

    x_position += x_rotate;
    y_position += y_rotate;
}

/*
 * Desenha um texto na janela GLUT
 */
void DesenhaTexto(char *string, int pos_x, int pos_y)
{
    glPushMatrix();

    // Posi��o no universo onde o texto ser� colocado
    glRasterPos2f(pos_x, pos_y);

    // Exibe caracter a caracter
    while (*string)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string++);
    glPopMatrix();
}
