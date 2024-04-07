#include <libs/glfw.h>
#include <libs/stb_image.h>
#include <render/init.h>
#include <render/render.h>
#include <render/texture.h>
#include <window/window.h>

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

int main()
{
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

    while (!w_should_close()) {
        r_clear();

#if 1
        glBindTexture(GL_TEXTURE_2D, test_tex);
        glBegin(GL_QUADS);
        //glColor3i(1 << 28, 2147483647, 1 << 28);
        f32 p = 0.4f;
        f32 r = glfwGetTime();
        f32 s = sin(r);
        f32 c = cos(r);
        f32_v2 p0 = rot_point((f32_v2) { -p * 8, -p }, s, c);
        f32_v2 p1 = rot_point((f32_v2) { p * 8, -p }, s, c);
        f32_v2 p2 = rot_point((f32_v2) { p * 8, p }, s, c);
        f32_v2 p3 = rot_point((f32_v2) { -p * 8, p }, s, c);
#define A (1 << 1)
        //glTexCoord2f(0.0f, 0.0f);
        glTexCoord2f(p0.x * A + 1.0f, p0.y * A + 1.0f);
        glVertex2f(p0.x, p0.y);
        //glTexCoord2f(1.0f, 0.0f);
        glTexCoord2f(p1.x * A + 1.0f, p1.y * A + 1.0f);
        glVertex2f(p1.x, p1.y);
        //glTexCoord2f(1.0f, 1.0f);
        glTexCoord2f(p2.x * A + 1.0f, p2.y * A + 1.0f);
        glVertex2f(p2.x, p2.y);
        //glTexCoord2f(0.0f, 1.0f);
        glTexCoord2f(p3.x * A + 1.0f, p3.y * A + 1.0f);
        glVertex2f(p3.x, p3.y);
        glEnd();
#else
        glBindTexture(GL_TEXTURE_2D, test_tex);
        glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
        float p = 0.4f;
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(-p, -p);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f( p, -p);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f( p,  p);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(-p,  p);
        glEnd();
#endif

        r_flush();
    }

    r_uninit();
}
