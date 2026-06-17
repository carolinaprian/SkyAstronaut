# Sky Astronaut Runner

Juego runner en C++ usando SFML, donde un astronauta corre automáticamente y debe saltar para evitar obstáculos mientras recoge coleccionables.

## 🎯 Descripción

En `Sky Astronaut Runner`, controlas a un astronauta que avanza solo hacia la derecha. Tu misión es sobrevivir el mayor tiempo posible, esquivar los ojos cósmicos y planetas, y recolectar cargas de jetpack para ganar puntaje.

El juego incluye:
- pantalla de portada (`COVER`)
- modo de juego activo (`PLAYING`)
- pantalla de fin de juego (`GAMEOVER`)
- sistema de puntaje y mejor puntaje
- recarga de saltos con coleccionables

## 🎮 Controles

- `Espacio` — saltar / usar carga de salto
- `R` — reiniciar después de perder
- `Esc` / cerrar ventana — salir del juego

## 🧠 Mecánicas principales

- El astronauta avanza automáticamente hacia la derecha.
- El jugador controla el salto usando cargas limitadas.
- Al recoger un coleccionable, sumas `10` puntos y ganas una carga adicional.
- El choque con un obstáculo o salir de la pantalla produce `GAME OVER`.
- El juego registra el `puntaje` actual y el `mejor puntaje`.

## 📁 Estructura del proyecto

- `src/JuegoProyecto.cpp` — lógica principal, bucle de juego, generación de obstáculos y coleccionables.
- `include/Player.hpp` — lógica de movimiento, gravedad, saltos, animación y colisiones del jugador.
- `include/AnimatedSprite.hpp` — gestión simple de sprites animados por frames.
- `include/Obstacle.hpp` — definición de obstáculos móviles.
- `include/Collectible.hpp` — definición de coleccionables.
- `makefile` — reglas de compilación para `g++` con SFML.
- `assets/images/` — texturas y sprites.
- `assets/fonts/` — fuente para HUD y mensajes.
- `bin/` — ejecutable compilado.
- `gallery/` — material extra para entrega.
- `gallery/screenshots/` — capturas del juego.
- `gallery/video/` — video de demo.

## 🛠️ Requisitos

- `g++` compatible con C++11 o superior
- SFML con los módulos: `graphics`, `window`, `system`, `audio`
- `make` disponible en el sistema

## 💻 Compilación

Desde la carpeta raíz del proyecto:

```bash
make all
```

Esto compila `src/JuegoProyecto.cpp` y genera `bin/JuegoProyecto.exe`.

Si prefieres compilar manualmente:

```bash
g++ src/JuegoProyecto.cpp -o bin/JuegoProyecto.exe -Iinclude -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lbox2d -std=c++17
```

## ▶️ Ejecución

```bash
bin/JuegoProyecto.exe
```

## 📦 Assets usados

El juego carga las siguientes texturas y fuentes:

- `assets/images/cover.png`
- `assets/images/alien.png`
- `assets/images/astronaut_idle.png`
- `assets/images/astronaut_jump.png`
- `assets/images/jetpack_icon.png`
- `assets/images/jetpack_collectible.png`
- `assets/images/planet_obstacule.png`
- `assets/fonts/Minecraft.ttf`

> Nota: Si no se encuentra el sprite principal del astronauta, el juego usa una forma simple como reserva.

## ✅ Estado actual del proyecto

- Runner funcional con físicas simples de salto y gravedad.
- Obstáculos y coleccionables generados dinámicamente.
- Puntaje y mejor puntaje en pantalla.
- Pantalla de inicio y mensaje de reinicio.
- Animaciones por frames para el jugador si las texturas están disponibles.

## 🔧 Notas importantes

- `make clean` elimina los ejecutables generados en `bin/`.
- El juego lee `assets/fonts/Minecraft.ttf` para mostrar texto; si falta la fuente, se imprime un mensaje en la consola.
- La pantalla de portada usa `assets/images/cover.png` si está disponible.

## 🚀 Créditos

- Desarrollo: C++ y SFML
- Autores: Carolina Prian y Karla Hernandez
- Ruta de los recursos: `assets/` y `gallery/`
