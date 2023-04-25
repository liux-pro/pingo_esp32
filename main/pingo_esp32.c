#include <stdio.h>
#include "render/material.h"
#include "render/mesh.h"
#include "render/object.h"
#include "render/pixel.h"
#include "render/renderer.h"
#include "render/scene.h"
#include "assets/cube.h"
#include "backend.h"
#include "esp_log.h"
#include "config.h"
#include "string.h"

#include "timeProbe.h"
#include "lcd.h"
#include "taskMonitor.h"

#define CUBE_NUMBER 27

Vec2i size = {SCREEN_WIDTH, SCREEN_HEIGHT};
Renderer renderer;
Scene s;
Object object[CUBE_NUMBER];
float phi = 0;
Mat4 t;
Texture tex1;
Material m;
LinuxWindowBackEnd backend;

Vec3f position[CUBE_NUMBER];

void init_position() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                position[i * 9 + j * 3 + k].x = (i - 1) * 12;
                position[i * 9 + j * 3 + k].y = (j - 1) * 12;
                position[i * 9 + j * 3 + k].z = (k - 1) * 12;
            }
        }
    }
}

void pingo_init() {
    init_position();

    linuxWindowBackEndInit(&backend, size);


    rendererInit(&renderer, size, (BackEnd *) &backend);
    rendererSetCamera(&renderer, (Vec4i) {0, 0, size.x, size.y});

    sceneInit(&s);
    rendererSetScene(&renderer, &s);


    for (int i = 0; i < CUBE_NUMBER; ++i) {
        object[i].mesh = &mesh_cube;
    }


#define N  2
    texture_init(&tex1, (Vec2i) {N, N}, malloc(N * N * sizeof(Pixel)));


    for (int i = 0; i < N; i++)
        for (int y = 0; y < N; y++) {
//            Pixel p;
//            p.red = 255;
//            p.green = 255;
//            p.blue = 255;
//            tex1.frameBuffer[(i * N + y)] = p;
            tex1.frameBuffer[(i * N + y)] = pixelRandom();;
        }
//    tex1.frameBuffer[0].red=255;
//    tex1.frameBuffer[1].green=255;
//    tex1.frameBuffer[2].blue=255;

    m.texture = &tex1;
    for (int i = 0; i < CUBE_NUMBER; ++i) {
        object[i].material = &m;
        sceneAddRenderable(&s, object_as_renderable(&object[i]));
    }


}

void setup() {
    pingo_init();
    // PROJECTION MATRIX - Defines the type of projection used
    renderer.camera_projection = mat4Perspective(1, 2500.0f, (float) size.x / (float) size.y, 0.6f);

    //VIEW MATRIX - Defines position and orientation of the "camera"
    Mat4 v = mat4Translate((Vec3f) {0, 0, -100});
    Mat4 rotateDown = mat4RotateX(0.40f); //Rotate around origin/orbit
    renderer.camera_view = mat4MultiplyM(&rotateDown, &v);

    for (int i = 0; i < CUBE_NUMBER; ++i) {
        //TEA TRANSFORM - Defines position and orientation of the object
        object[i].transform = mat4RotateZ(3.142128f);
        t = mat4Scale((Vec3f) {10, 10, 10});
        object[i].transform = mat4MultiplyM(&object[i].transform, &t);
//        t = mat4Translate((Vec3f) {i*11, 0, 0});
        t = mat4Translate(position[i]);
        object[i].transform = mat4MultiplyM(&object[i].transform, &t);
        t = mat4RotateX(3.14f * 0.1f);
        object[i].transform = mat4MultiplyM(&object[i].transform, &t);
    }
}

Pixel *getFrameBuffer(Renderer *ren, BackEnd *backEnd);

void loop(uint8_t *buffer) {
    s.transform = mat4RotateY(phi);
    phi += 0.02f;
    rendererRender(&renderer);
}

timeProbe_t fps;
//uint16_t buffer2[SCREEN_HEIGHT*SCREEN_WIDTH];

void app_main(void) {
    startTaskMonitor(5000);
    init_lcd();
    setup();
    while (1) {
        const int CNT = 888;
        timeProbe_start(&fps);
        for (int i = 0; i < CNT; ++i) {
            loop(0);
            esp_lcd_panel_draw_bitmap(lcd_panel_handle, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, getFrameBuffer(0, 0));
        }
        ESP_LOGI("fps", "fps: %f", 1000.0 / (timeProbe_stop(&fps) / 1000.0 / CNT));
        while (1){
            vTaskDelay(1);
        };
    }

}
