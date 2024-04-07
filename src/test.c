#include <libs/glfw.h>
#include <libs/stb_image.h>
#include <render/init.h>
#include <render/render.h>
#include <render/texture.h>
#include <window/window.h>
#include <utils/vector.h>

typedef struct f32_v2 {
    f32 x;
    f32 y;
} f32_v2;

f32_v2 rot_point(f32_v2 pos, f32 sin, f32 cos)
{
    return (f32_v2) {
        .x = pos.x * cos - pos.y * sin,
        .y = pos.x * sin + pos.y * cos,
    };
}

typedef struct test_obj {
    f32_v2 size;
    f32_v2 pos;
    f32 rot;
    r_tex tex;
} test_obj;

VEC(test_obj) objs;

int main()
{
    VEC_INIT(objs, 1);
    r_init();

    u32 width;
    u32 height;
    UNUSED u32 channels;
    u8* raw = stbi_load (
        "tests/test.tga",
        (i32*)&width,
        (i32*)&height,
        (i32*)&channels,
        0
    );

    if (raw == NULL)
        printf("Failed to load test image");

    r_tex test_tex = r_tex_from_image(raw, width, height);
    stbi_image_free(raw);

    test_obj _obj = {
        .size = { 0.5, 0.5 },
        .pos = { -0.2, -0.2 },
        .rot = 0.0,
        .tex = test_tex,
    };

    VEC_APPEND(objs, &_obj);

    _obj = (test_obj) {
        .size = { 0.3, 0.3 },
        .pos = { 0.4, 0.4 },
        .rot = 0.0,
        .tex = test_tex,
    };

    VEC_APPEND(objs, &_obj);

    while (!w_should_close()) {
        r_clear();

        #define TEX_MAG (1 << 2)
        for VEC_ITER(objs, obj) {
            glBindTexture(GL_TEXTURE_2D, obj->tex);
            glBegin(GL_QUADS);
            obj->rot = glfwGetTime();
            f32 s = sin(obj->rot);
            f32 c = cos(obj->rot);
            f32_v2 p0 = rot_point((f32_v2) {-obj->size.x,-obj->size.y}, s, c);
            f32_v2 p1 = rot_point((f32_v2) {obj->size.x,-obj->size.y}, s, c);
            f32_v2 p2 = rot_point((f32_v2) {obj->size.x,obj->size.y}, s, c);
            f32_v2 p3 = rot_point((f32_v2) {-obj->size.x,obj->size.y}, s, c);

            /* Adjusting for target screen ratio. */
            p0.x *= W_RATIO;
            p1.x *= W_RATIO;
            p2.x *= W_RATIO;
            p3.x *= W_RATIO;

            //glTexCoord2f(p0.x * TEX_MAG + 1.0f, p0.y * TEX_MAG + 1.0f);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(p0.x + obj->pos.x + s / 3, p0.y + obj->pos.y + c / 3);
            //glTexCoord2f(p1.x * TEX_MAG + 1.0f, p1.y * TEX_MAG + 1.0f);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(p1.x + obj->pos.x + s / 3, p1.y + obj->pos.y + c / 3);
            //glTexCoord2f(p2.x * TEX_MAG + 1.0f, p2.y * TEX_MAG + 1.0f);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(p2.x + obj->pos.x + s / 3, p2.y + obj->pos.y + c / 3);
            //glTexCoord2f(p3.x * TEX_MAG + 1.0f, p3.y * TEX_MAG + 1.0f);
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(p3.x + obj->pos.x + s / 3, p3.y + obj->pos.y + c / 3);
            glEnd();
        }

        r_flush();
    }

    r_uninit();
}
