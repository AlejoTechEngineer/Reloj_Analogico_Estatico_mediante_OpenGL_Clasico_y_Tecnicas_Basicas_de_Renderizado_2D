/*
 * Laboratorio #1: Introducción a OpenGL
 * Reloj Analógico Estático
 *
 * Compilación:
 *   gcc solution.c -o reloj -lGL -lGLU -lglut -lm
 *
 * En Windows (MinGW + freeglut):
 *   gcc solution.c -o reloj.exe -lfreeglut -lopengl32 -lglu32 -lm
 */

#include <GL/glut.h>
#include <math.h>

/* ─── Constantes ─────────────────────────────────────────── */
#define PI          3.14159265358979323846
#define TWO_PI      (2.0 * PI)
#define RAD(deg)    ((deg) * PI / 180.0)

/* Hora estática mostrada: 10:10:30 */
#define HORA_H      10
#define HORA_M      10
#define HORA_S      30

/* Dimensiones de ventana */
#define WIN_W       600
#define WIN_H       600

/* ─── Utilidades de dibujo ───────────────────────────────── */

/* Dibuja un círculo relleno centrado en (cx, cy) con radio r */
void dibujarCirculoRelleno(float cx, float cy, float r, int segmentos)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segmentos; i++) {
        float angulo = TWO_PI * i / segmentos;
        glVertex2f(cx + r * cosf(angulo), cy + r * sinf(angulo));
    }
    glEnd();
}

/* Dibuja el contorno de un círculo */
void dibujarCirculoContorno(float cx, float cy, float r, int segmentos)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segmentos; i++) {
        float angulo = TWO_PI * i / segmentos;
        glVertex2f(cx + r * cosf(angulo), cy + r * sinf(angulo));
    }
    glEnd();
}

/* Dibuja una línea desde el centro con ángulo (en radianes desde las 12)
   y longitud dada, con un grosor determinado */
void dibujarManilla(float angulo_desde_12, float longitud, float grosor,
                    float r, float g, float b)
{
    /* Convertimos "ángulo desde las 12 en sentido horario" a
       ángulo matemático (desde eje X, antihorario)            */
    float ang = PI / 2.0f - angulo_desde_12;

    glLineWidth(grosor);
    glColor3f(r, g, b);
    glBegin(GL_LINES);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(longitud * cosf(ang), longitud * sinf(ang));
    glEnd();
    glLineWidth(1.0f);
}

/* ─── Esfera central (pivot) ─────────────────────────────── */
void dibujarPivote(void)
{
    glColor3f(0.15f, 0.15f, 0.15f);
    dibujarCirculoRelleno(0.0f, 0.0f, 0.025f, 32);
}

/* ─── Marcas de horas y minutos ──────────────────────────── */
void dibujarMarcas(void)
{
    for (int i = 0; i < 60; i++) {
        float ang = PI / 2.0f - TWO_PI * i / 60.0f;
        float cos_a = cosf(ang);
        float sin_a = sinf(ang);

        if (i % 5 == 0) {
            /* Marca de hora: más larga y gruesa */
            glLineWidth(3.0f);
            glColor3f(0.1f, 0.1f, 0.1f);
            float r_ext = 0.90f;
            float r_int = 0.78f;
            glBegin(GL_LINES);
                glVertex2f(r_int * cos_a, r_int * sin_a);
                glVertex2f(r_ext * cos_a, r_ext * sin_a);
            glEnd();
        } else {
            /* Marca de minuto: más corta y fina */
            glLineWidth(1.0f);
            glColor3f(0.45f, 0.45f, 0.45f);
            float r_ext = 0.90f;
            float r_int = 0.85f;
            glBegin(GL_LINES);
                glVertex2f(r_int * cos_a, r_int * sin_a);
                glVertex2f(r_ext * cos_a, r_ext * sin_a);
            glEnd();
        }
    }
    glLineWidth(1.0f);
}

/* ─── Esfera / cara del reloj ────────────────────────────── */
void dibujarCara(void)
{
    /* Fondo blanco cremoso */
    glColor3f(0.97f, 0.96f, 0.92f);
    dibujarCirculoRelleno(0.0f, 0.0f, 0.92f, 128);

    /* Aro externo dorado */
    glLineWidth(6.0f);
    glColor3f(0.72f, 0.60f, 0.25f);
    dibujarCirculoContorno(0.0f, 0.0f, 0.92f, 128);

    /* Aro interno sutil */
    glLineWidth(1.5f);
    glColor3f(0.55f, 0.45f, 0.15f);
    dibujarCirculoContorno(0.0f, 0.0f, 0.76f, 128);

    glLineWidth(1.0f);
}

/* ─── Manillas ────────────────────────────────────────────── */
void dibujarManillas(void)
{
    /* ── Manilla de segundos ── */
    float ang_s = TWO_PI * HORA_S / 60.0f;
    dibujarManilla(ang_s, 0.72f, 1.5f, 0.85f, 0.12f, 0.10f);

    /* Cola de contrapeso de la manilla de segundos */
    {
        float ang = PI / 2.0f - ang_s + PI;   /* dirección opuesta */
        glLineWidth(1.5f);
        glColor3f(0.85f, 0.12f, 0.10f);
        glBegin(GL_LINES);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(0.18f * cosf(ang), 0.18f * sinf(ang));
        glEnd();
        glLineWidth(1.0f);
    }

    /* ── Manilla de minutos ── */
    float ang_m = TWO_PI * HORA_M / 60.0f
                + TWO_PI * HORA_S / 3600.0f;
    dibujarManilla(ang_m, 0.62f, 4.5f, 0.10f, 0.10f, 0.12f);

    /* ── Manilla de horas ── */
    float ang_h = TWO_PI * (HORA_H % 12) / 12.0f
                + TWO_PI * HORA_M / 720.0f;
    dibujarManilla(ang_h, 0.42f, 6.5f, 0.08f, 0.08f, 0.10f);
}

/* ─── Callback de display ─────────────────────────────────── */
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    /* Sombra del reloj */
    glColor4f(0.0f, 0.0f, 0.0f, 0.25f);
    dibujarCirculoRelleno(0.03f, -0.03f, 0.92f, 128);

    dibujarCara();
    dibujarMarcas();
    dibujarManillas();
    dibujarPivote();

    glFlush();
}

/* ─── Callback de redimensión ─────────────────────────────── */
void reshape(int w, int h)
{
    int lado = (w < h) ? w : h;
    glViewport((w - lado) / 2, (h - lado) / 2, lado, lado);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    /* Coordenadas de -1 a 1 en ambos ejes */
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/* ─── Main ────────────────────────────────────────────────── */
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIN_W, WIN_H);
    glutInitWindowPosition(200, 100);
    glutCreateWindow("Reloj Analogico - Lab 1 OpenGL");

    /* Color de fondo gris oscuro */
    glClearColor(0.18f, 0.18f, 0.20f, 1.0f);

    /* Activar blending para la sombra */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Antialiasing suave en líneas */
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();

    return 0;
}
