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

#include "timeProbe.h"
#include "lcd.h"
#include "taskMonitor.h"

Vec2i size = {SCREEN_WIDTH, SCREEN_HEIGHT};
Renderer renderer;
Scene s;
Object object;
float phi = 0;
Mat4 t;
Texture tex1;
Material m;
LinuxWindowBackEnd backend;

void pingo_init() {

    linuxWindowBackEndInit(&backend, size);


    rendererInit(&renderer, size, (BackEnd *) &backend);
    rendererSetCamera(&renderer, (Vec4i) {0, 0, size.x, size.y});

    sceneInit(&s);
    rendererSetScene(&renderer, &s);


    object.mesh = &mesh_cube;


#define N  1
    texture_init(&tex1, (Vec2i) {N, N}, malloc(N * N * sizeof(Pixel)));


    for (int i = 0; i < N; i++)
        for (int y = 0; y < N; y++) {
            Pixel p;
            p.red = 0;
            p.green = 255;
            p.blue = 0;
            tex1.frameBuffer[(i * N + y)] = p;
//            tex1.frameBuffer[(i * N + y)] = pixelRandom();;
        }

    m.texture = &tex1;
    object.material = &m;


    sceneAddRenderable(&s, object_as_renderable(&object));


}

void setup() {
    pingo_init();
    // PROJECTION MATRIX - Defines the type of projection used
    renderer.camera_projection = mat4Perspective(1, 2500.0f, (float) size.x / (float) size.y, 0.6);

    //VIEW MATRIX - Defines position and orientation of the "camera"
    Mat4 v = mat4Translate((Vec3f) {0, 50, -250});
    Mat4 rotateDown = mat4RotateX(0.80f); //Rotate around origin/orbit
    renderer.camera_view = mat4MultiplyM(&rotateDown, &v);
}
Pixel *getFrameBuffer(Renderer *ren, BackEnd *backEnd);

void loop(uint8_t *buffer) {
    //TEA TRANSFORM - Defines position and orientation of the object
    object.transform = mat4RotateZ(3.142128f);
    t = mat4Scale((Vec3f) {80, 80, 80});
    object.transform = mat4MultiplyM(&object.transform, &t);
    t = mat4Translate((Vec3f) {0, 45, 0});
    object.transform = mat4MultiplyM(&object.transform, &t);
    t = mat4RotateX(3.14f);
    object.transform = mat4MultiplyM(&object.transform, &t);

    //SCENE
    s.transform = mat4RotateY(phi);
    phi += 0.01f;

    rendererRender(&renderer);
    //111111111111111111111111111111111

//    getFrameBuffer(0, 0);
//    memcpy(buffer, getFrameBuffer(0, 0), SCREEN_WIDTH * SCREEN_HEIGHT * 2);


}

timeProbe_t fps;

void app_main(void) {
    startTaskMonitor(5000);
    init_lcd();
    setup();
    while (1) {
        timeProbe_start(&fps);
        esp_lcd_panel_draw_bitmap(lcd_panel_handle, 0, 0, 240, 240, getFrameBuffer(0, 0));

        loop(0);
//        while (esp_timer_get_time()<fps.time+41*1000){
//            vTaskDelay(pdMS_TO_TICKS(1));
//        }
        ESP_LOGI("fps", "fps: %f", 1000.0f / (timeProbe_stop(&fps) / 1000.0));

    }

}
