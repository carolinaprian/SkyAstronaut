# Sky Astronaut Runner

Juego runner en C++ con SFML donde un astronauta salta automáticamente para evitar obstáculos y recoger coleccionables.

## 🎯 Objetivo del juego

Controlas a un astronauta que corre automáticamente hacia la derecha.
El objetivo es sobrevivir el mayor tiempo posible evitando obstáculos y acumulando puntos al recoger objetos.

## 🎮 Mecánicas y controles

- El jugador corre solo hacia la derecha.
- Presiona `Espacio` para saltar.
- Si golpeas un obstáculo, el juego termina.
- Presiona `R` para reiniciar después de perder.
- Presiona `Esc` o cierra la ventana para salir del juego.

## 📁 Estructura del proyecto

- `src/demo.cpp` — código principal del juego y bucle de ejecución.
- `include/AnimatedSprite.h` — animación de sprites por frames.
- `include/Player.h` — lógica del jugador, salto y dibujado.
- `include/Obstacle.h` — definición de obstáculos y su movimiento.
- `include/Collectible.h` — definición de objetos coleccionables.
- `makefile` — reglas de compilación con SFML e inclusión de `include/`.
- `assets/images/` — sprites e imágenes usadas por el juego.
- `assets/fonts/` — fuente para HUD y mensajes.
- `bin/` — ejecutable compilado.
- `gallery/` — portada del proyecto.
- `screenshots/` — capturas del juego.
- `video/` — video de juego para entrega.

## 🛠️ Cómo compilar

Desde la carpeta del proyecto, ejecuta:

```bash
make all
```

Luego ejecuta:

```bash
bin/demo.exe
```

> Nota: El proyecto requiere SFML instalado y accesible desde el compilador.

## 📌 Recursos necesarios

El juego usa estas rutas de assets:

- `assets/images/astronaut.png` — sprite principal del astronauta.
- `assets/images/astronaut_idle.png` — animación de idle del astronauta.
- `assets/images/astronaut_jump.png` — animación de salto.
- `assets/images/astronaut_fall.png` — animación de caída.
- `assets/images/jetpack_icon.png` — icono que se muestra en el HUD.
- `assets/fonts/Minecraft.ttf` — fuente para texto en pantalla.

Si no se cargan las imágenes del astronauta, el juego usa una forma simple como reserva.

## ✅ Estado actual

- Juego básico implementado en C++ y SFML.
- Saltos, obstacles y coleccionables están funcionando.
- Puntaje, mejor puntaje y mensaje de reinicio presentes.
- Código refactorizado en headers bajo `include/` para separar las clases.

## 📌 Próximos pasos recomendados

- Añadir o reemplazar `assets/images/astronaut.png` con el sprite final.
- Incluir `assets/images/astronaut_jump.png` y `assets/images/astronaut_fall.png` para animaciones completas.
- Agregar `gallery/cover.png` y al menos 3 capturas en `screenshots/`.
- Generar `video/demo.mp4` mostrando gameplay y reinicios.

## 🚀 Créditos

- Juego creado con C++ y SFML.
- Autor: [Tu nombre aquí]
- Assets usados: `assets/images/` y `assets/fonts/Minecraft.ttf`.
