# 🕐 Reloj Analógico Estático mediante OpenGL Clásico y Técnicas Básicas de Renderizado 2D

**Laboratorio No. 1 — Informática Gráfica y Visualización**  
**Profesor:** Ing. Rogerio Orlando Beltrán Castro  
**Fecha:** 10 de mayo de 2026

---

## 📋 Descripción

Implementación de un **reloj analógico estático** desarrollado en lenguaje C utilizando **OpenGL clásico** y la biblioteca **GLUT/freeglut**. El programa renderiza una ventana gráfica de 600×600 píxeles mostrando un reloj analógico con la hora estática **10:10:30**, haciendo uso exclusivo de primitivas de dibujo 2D de OpenGL.

El proyecto fue diseñado bajo un **enfoque modular**, separando cada componente gráfico en funciones independientes para mejorar la legibilidad, mantenibilidad y reutilización del código.

---

## 🖼️ Screenshot

![Reloj Analógico OpenGL](screenshot.png)

> Ventana de 600×600 píxeles mostrando el reloj estático con marcas de horas, marcas de minutos, manillas de horas, minutos y segundos, aro dorado y efecto de sombra.

---

## 🏗️ Arquitectura del proyecto

```
Laboratorio No. 1/
│
├── solution.c                          # Código fuente principal
├── README.md                           # Documentación del proyecto
├── screenshot.png                      # Captura de pantalla del programa
└── Desarrollo_Proyecto_Alejandro_De_Mendoza.docx   # Informe académico completo
```

---

## ⚙️ Primitivas OpenGL utilizadas

| Primitiva | Uso en el proyecto |
|---|---|
| `GL_TRIANGLE_FAN` | Relleno de círculos (esfera del reloj, sombra, pivote) |
| `GL_LINE_LOOP` | Contornos circulares (aro dorado exterior, aro interior decorativo) |
| `GL_LINES` | Manillas de horas, minutos y segundos; marcas horarias y de minutos |

---

## 🧮 Fundamentos matemáticos

### Conversión de ángulo del reloj a coordenadas OpenGL

OpenGL usa coordenadas matemáticas (eje X positivo, sentido antihorario), mientras que el reloj mide ángulos desde las 12 en sentido horario. La conversión es:

```
θ_opengl = π/2 − θ_reloj
```

### Posición angular de las marcas

```
θ = π/2 − (2π · i / 60)      donde i ∈ [0, 59]
```

### Ángulo de la manilla de segundos

```
θs = 2π · S / 60
```

### Ángulo de la manilla de minutos (con interpolación de segundos)

```
θm = 2π · M / 60 + 2π · S / 3600
```

### Ángulo de la manilla de horas (con interpolación de minutos)

```
θh = 2π · (H mod 12) / 12 + 2π · M / 720
```

### Coordenadas de un vértice sobre la circunferencia

```
x = cx + r · cos(θ)
y = cy + r · sin(θ)
```

---

## 🔧 Requisitos

### Windows (MinGW + freeglut)
- Windows 10 (1809+) o superior — 64 bits
- [MSYS2](https://www.msys2.org/) con entorno **MinGW64**
- GCC (instalado vía `pacman`)
- freeglut (instalado vía `pacman`)

### Linux (Ubuntu/Debian)
- GCC
- freeglut3-dev
- libglu1-mesa-dev

---

## 📦 Instalación de dependencias

### Windows — MSYS2 MinGW64

```bash
# Actualizar repositorios
pacman -Syu

# Instalar GCC
pacman -S mingw-w64-ucrt-x86_64-gcc

# Instalar freeglut
pacman -S mingw-w64-x86_64-freeglut
```

### Linux — Ubuntu/Debian

```bash
sudo apt install gcc freeglut3-dev libglu1-mesa-dev
```

---

## 🚀 Compilación y ejecución

### Windows (terminal MinGW64)

```bash
# 1. Navegar al directorio del proyecto
cd "/c/Users/P1 3-3/Documents/Estudio/Carrera Ingeniería Informatica/Septimo Semestre/Informática Gráfica y Visualización/Laboratorio No. 1"

# 2. Compilar
gcc solution.c -o reloj.exe -lfreeglut -lopengl32 -lglu32 -lm -Wl,--subsystem,windows

# 3. Ejecutar
./reloj.exe
```

### Linux

```bash
gcc solution.c -o reloj -lGL -lGLU -lglut -lm
./reloj
```

---

## 🗂️ Descripción del código fuente

### Constantes principales

```c
#define HORA_H  10    // Hora estática
#define HORA_M  10    // Minutos estáticos
#define HORA_S  30    // Segundos estáticos
#define WIN_W   600   // Ancho de ventana
#define WIN_H   600   // Alto de ventana
```

Para cambiar la hora mostrada, modificar `HORA_H`, `HORA_M` y `HORA_S` y recompilar.

### Funciones implementadas

| Función | Descripción |
|---|---|
| `dibujarCirculoRelleno()` | Círculo sólido con `GL_TRIANGLE_FAN` |
| `dibujarCirculoContorno()` | Borde circular con `GL_LINE_LOOP` |
| `dibujarManilla()` | Línea desde el centro con ángulo, longitud y color configurables |
| `dibujarPivote()` | Punto central del reloj |
| `dibujarMarcas()` | 60 marcas (12 de hora + 48 de minuto) |
| `dibujarCara()` | Esfera del reloj con fondo crema y aros dorados |
| `dibujarManillas()` | Calcula y renderiza las tres manillas |
| `display()` | Callback principal de renderizado |
| `reshape()` | Mantiene el aspect ratio 1:1 al redimensionar |
| `main()` | Inicialización de GLUT y ciclo principal |

### Orden de renderizado en `display()`

```
1. Sombra difusa (blending con alpha)
2. Esfera del reloj (cara)
3. Marcas de horas y minutos
4. Manillas (segundos → minutos → horas)
5. Pivote central
```

---

## 🎨 Características visuales

- **Antialiasing** en líneas (`GL_LINE_SMOOTH`) para bordes suavizados
- **Blending** (`GL_BLEND`) para sombra semitransparente con efecto de profundidad
- **Aro dorado** exterior (`glLineWidth(6.0f)`) y aro interior decorativo
- **Manilla de segundos** en rojo con contrapeso posterior
- **Fondo gris oscuro** `(0.18, 0.18, 0.20)` característico de la aplicación
- **Viewport cuadrado** centrado garantizando proporciones correctas

---

## ⚠️ Limitaciones conocidas

| Limitación | Descripción |
|---|---|
| Sin animación | El reloj es estático por requerimiento del laboratorio (animación en Tema 12) |
| Sin dígitos | OpenGL clásico no tiene funciones nativas de texto; requeriría GLUT Bitmap Fonts |
| OpenGL legado | Usa modo inmediato (`glBegin/glEnd`), deprecado desde OpenGL 3.2 |
| Sin manejo de errores | No verifica disponibilidad de GLUT ni creación de ventana |
| Aspect ratio fijo | El viewport cuadrado puede quedar pequeño en ventanas muy alargadas |

---

## 📚 Referencias

- OpenGL Programming Guide — Addison-Wesley Professional
- Hearn, D. & Baker, M. P. — *Computer Graphics with OpenGL* — Pearson Education
- Foley, J. D. et al. — *Computer Graphics: Principles and Practice* — Addison-Wesley
- [Khronos Group — Documentación oficial OpenGL](https://www.khronos.org/opengl/)
- [MSYS2 — msys2.org](https://www.msys2.org/)
- [freeglut — sourceforge.net/projects/freeglut](https://sourceforge.net/projects/freeglut/)
- Material académico — Informática Gráfica y Visualización (COLGII) — UNIR 2026

---

## 👨‍💻 Autor

**Alejandro De Mendoza Tovar**  
Ingeniería Informática  
Fundación Universitaria Internacional de La Rioja  
GitHub: [@AlejoTechEngineer](https://github.com/AlejoTechEngineer)
