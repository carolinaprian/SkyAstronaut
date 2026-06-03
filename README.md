# Sky Astronaut Runner

Este repositorio contiene un juego en C++ basado en SFML. Es un runner automático con un astronauta que salta para esquivar obstáculos y recoger coleccionables.

## 🎯 Objetivo del juego

El jugador controla a un astronauta con traje espacial normal que intenta avanzar lo más lejos posible.

Mecánicas principales:
- El personaje corre automáticamente hacia la derecha.
- El jugador presiona `Espacio` para saltar.
- Evita los obstáculos y recoge objetos dorados para sumar puntos.
- Si chocas contra un obstáculo, el juego termina y se puede reiniciar con `R`.

## 🎮 Controles

- `Espacio` — Saltar
- `R` — Reiniciar después de perder
- `Esc` o cerrar la ventana — Salir del juego

## 📁 Estructura del proyecto

- `src/demo.cpp` — código principal del juego.
- `makefile` — compila el juego con SFML.
- `assets/images/` — aquí van las imágenes y sprites.
- `assets/fonts/` — fuente para mostrar texto en pantalla.
- `bin/` — ejecutable compilado.
- `gallery/` — portada del proyecto.
- `screenshots/` — capturas del juego.
- `video/` — video de juego para entrega.

## 🛠️ Cómo compilar

Desde la carpeta del proyecto, usa:

```bash
make all
```

Luego ejecuta:

```bash
bin/demo.exe
```

> Nota: El proyecto requiere SFML instalado y accesible desde el compilador.

## 📌 Recursos necesarios

- `assets/images/astronaut.png` — sprite del astronauta.
- `assets/fonts/Minecraft.ttf` — fuente para mostrar el puntaje y mensajes.

Si no existieran, el juego usará una forma simple como reserva.

## ✅ Qué hice hasta ahora

- Reemplacé el demo original por un runner básico en C++ con SFML.
- Añadí físicas de salto, generación de obstáculos y coleccionables.
- Añadí texto en pantalla con puntuación y reinicio.
- El código es compatible con la plantilla original y no necesita cambios en el makefile.

## 📌 Próximos pasos

- Reemplazar `assets/images/astronaut.png` por la imagen de tu astronauta.
- Añadir `gallery/cover.png` y al menos 3 capturas en `screenshots/`.
- Generar `video/demo.mp4` mostrando el gameplay.

## 🚀 Créditos

- Juego creado con C++, SFML y la plantilla de proyecto escolar.
- Autor: [Tu nombre aquí]

- Assets de terceros utilizados
- Referencias o inspiraciones
- Agradecimientos

---