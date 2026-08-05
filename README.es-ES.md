

# Código fuente de Coral Dev Board Micro (coralmicro)

Este repositorio contiene todo el código necesario para desarrollar aplicaciones para el [Coral Dev
Board Micro](https://coral.ai/products/dev-board-micro). El Dev Board Micro se
basa en el microcontrolador NXP RT1176 (MCU de doble núcleo con Cortex M7 y M4)
e incluye una cámara a bordo (324x324 px), un micrófono y un Coral Edge TPU
para acelerar los modelos de TensorFlow Lite.

La plataforma de software para Dev Board Micro se llama `coralmicro` y se basa
en [FreeRTOS](https://www.freertos.org/). También incluye bibliotecas para
garantizar la compatibilidad con el lenguaje de programación Arduino.

El sistema de compilación de `coralmicro` se basa en CMake e incluye soporte para compilaciones con Make
y Ninja. Después de compilar los proyectos incluidos, puedes flashearlos
en tu placa con la herramienta de flasheo incluida (`scripts/flashtool.py`).

![main](https://github.com/google-coral/coralmicro/actions/workflows/ci.yml/badge.svg?event=push)
![arduino](https://github.com/google-coral/coralmicro/actions/workflows/arduino.yml/badge.svg?event=push)


## Nuevas características

- [ ] soporte para micro ros jazzy
- [x] compatible con la última versión de arduino cli
- [x] soporte para la plataforma aarch64 (raspberry pi, placas nv jetson, etc...)

## Documentación

+ [Comienza con el Dev Board Micro](https://coral.ai/docs/dev-board-micro/get-started/)

+ [Comienza con Arduino](https://coral.ai/docs/dev-board-micro/arduino/)

+ [Compila un proyecto fuera del árbol](https://github.com/google-coral/coralmicro-out-of-tree-sample/blob/main/README.md)

+ [Referencia de la API de coralmicro](http://coral.ai/docs/reference/micro/)

+ [Ejemplos de coralmicro](/examples/)



## Obtener el código

1. Clona `coralmicro` y todos sus submódulos:

    ```bash
    git clone --recurse-submodules -j8 https://github.com/google-coral/coralmicro
    ```

2. Instala las herramientas requeridas:

    ```bash
    cd coralmicro && bash setup.sh
    ```


## Compilar el código

Esto compila todo en una carpeta llamada `build` (o puedes especificar una
ruta diferente con `-b`, pero si lo haces, también debes especificar esa ruta
cada vez que llames a `flashtool.py`):

```bash
bash build.sh
```

## Flashear la placa

Este ejemplo hace parpadear el LED verde de la placa:

```bash
python3 scripts/flashtool.py -e blink_led
```

Puedes ver el código en [examples/blink_led/](examples/blink_led/).


### Reiniciar la placa al modo Serial Downloader

El flasheo del Dev Board Micro puede fallar a veces y generalmente puedes solucionarlo
iniciando el modo Serial Downloader de una de las siguientes dos maneras:

+ Mantén presionado el botón User mientras presionas el botón Reset.
+ O bien, mantén presionado el botón User mientras conectas el cable USB.

Luego, intenta flashear la placa nuevamente.

Para más detalles, consulta la [información de solución de problemas en
coral.ai](https://coral.ai/docs/dev-board-micro/get-started/#serial-downloader).


## Actualizar el repositorio

Usa los siguientes comandos para mantener todos los submódulos de coralmicro sincronizados (realizando un rebase de tu rama actual):

```bash
git fetch origin

git rebase origin/main

git submodule update --init --recursive
```
