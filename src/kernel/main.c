#include "stdio.h"
#include "string.h"
#include "memory.h"
#include "magic.h"
#include "math.h"
#include "display.h"
#include <hal/hal.h>
#include <types.h>

typedef struct
{
    vec2d o;
    vec2d d;
} ray;

void multiply_matrix_vector(vec3d i, vec3d *o, mat4x4 m)
{
    o->x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
    o->y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
    o->z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
    f32 w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

    if (w != 0.0f)
    {
        o->x /= w;
        o->y /= w;
        o->z /= w;
    }
}

void _main32(void)
{
    HAL_init();

    cube_mesh mesh = {
        // South face
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        // East face
        1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

        // North face
        1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        // West face
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        // Top face
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

        // Bottom face
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f};

    vec3d vCamera = {0.0f, 0.0f, 0.0f};

    mat4x4 matProj;
    mat4x4 matRotZ, matRotX;

    f32 fNear = 0.1f;
    f32 fFar = 1000.0f;
    f32 fFov = 90.0f;
    f32 fAspectRatio = (f32)SCREEN_H / (f32)SCREEN_W;
    f32 fFovRad = 1.0f / tan(fFov * 0.5f / 180.0f * PI);

    // Projection Matrix
    matProj.m[0][0] = fAspectRatio * fFovRad;
    matProj.m[1][1] = fFovRad;
    matProj.m[2][2] = fFar / (fFar - fNear);
    matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    matProj.m[2][3] = 1.0f;

    f32 time_elapsed = 0.0f;
    while (1)
    {
        // Clear Screen after some time
        for (u32 i = 0; i < 20000000; i++)
            ;
        clear_vga_buffer();

        f32 fTheta = time_elapsed;
        // Rotation Z Matrix
        matRotZ.m[0][0] = cos(fTheta);
        matRotZ.m[0][1] = sin(fTheta);
        matRotZ.m[1][0] = -sin(fTheta);
        matRotZ.m[1][1] = cos(fTheta);
        matRotZ.m[2][2] = 1.0f;
        matRotZ.m[3][3] = 1.0f;

        // Rotation X Matrix
        matRotX.m[0][0] = 1.0f;
        matRotX.m[1][1] = cos(fTheta * 0.5f);
        matRotX.m[1][2] = sin(fTheta * 0.5f);
        matRotX.m[2][1] = -sin(fTheta * 0.5f);
        matRotX.m[2][2] = cos(fTheta * 0.5f);
        matRotX.m[3][3] = 1.0f;

        for (int i = 0; i < 12; i++)
        {
            triangle t, triProjected, triTranslated, triRotatedZ, triRotatedZX;
            t = mesh.t[i];

            // Rotate in Z-Axis
            multiply_matrix_vector(t.p[0], &triRotatedZ.p[0], matRotZ);
            multiply_matrix_vector(t.p[1], &triRotatedZ.p[1], matRotZ);
            multiply_matrix_vector(t.p[2], &triRotatedZ.p[2], matRotZ);

            // Rotate in X-Axis
            multiply_matrix_vector(triRotatedZ.p[0], &triRotatedZX.p[0], matRotX);
            multiply_matrix_vector(triRotatedZ.p[1], &triRotatedZX.p[1], matRotX);
            multiply_matrix_vector(triRotatedZ.p[2], &triRotatedZX.p[2], matRotX);

            // Offset into the screen
            triTranslated = triRotatedZX;
            triTranslated.p[0].z = triRotatedZX.p[0].z + 2.5f;
            triTranslated.p[1].z = triRotatedZX.p[1].z + 2.5f;
            triTranslated.p[2].z = triRotatedZX.p[2].z + 2.5f;

            // Compute triangle normal
            vec3d normal, line1, line2;
            line1.x = triTranslated.p[1].x - triTranslated.p[0].x;
            line1.y = triTranslated.p[1].y - triTranslated.p[0].y;
            line1.z = triTranslated.p[1].z - triTranslated.p[0].z;

            line2.x = triTranslated.p[2].x - triTranslated.p[0].x;
            line2.y = triTranslated.p[2].y - triTranslated.p[0].y;
            line2.z = triTranslated.p[2].z - triTranslated.p[0].z;

            normal.x = line1.y * line2.z - line1.z * line2.y;
            normal.y = line1.z * line2.x - line1.x * line2.z;
            normal.z = line1.x * line2.y - line1.y * line2.x;

            // Normalize
            f32 l = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
            normal.x /= l;
            normal.y /= l;
            normal.z /= l;

            if (normal.x * (triTranslated.p[0].x - vCamera.x) + normal.y * (triTranslated.p[0].y - vCamera.y) + normal.z * (triTranslated.p[0].z - vCamera.z) < 0.0f)
            {
                // Illumination
                vec3d light_direction = {0.0f, 0.0f, -1.0f};
                f32 l = sqrt(light_direction.x * light_direction.x + light_direction.y * light_direction.y + light_direction.z * light_direction.z);
                light_direction.x /= l;
                light_direction.y /= l;
                light_direction.z /= l;

                f32 dp = MAX(0.1f, normal.x * light_direction.x + normal.y * light_direction.y + normal.z * light_direction.z);

                u8 color;
                if (dp > 0.9f)
                    color = VGA_COLOR_WHITE;
                else if (dp > 0.6f)
                    color = VGA_COLOR_LIGHT_GRAY;
                else
                    color = VGA_COLOR_DARK_GRAY;

                // Project triangles from 3D --> 2D
                multiply_matrix_vector(triTranslated.p[0], &triProjected.p[0], matProj);
                multiply_matrix_vector(triTranslated.p[1], &triProjected.p[1], matProj);
                multiply_matrix_vector(triTranslated.p[2], &triProjected.p[2], matProj);

                // triProjected.p[0].x += 1.0f;
                // triProjected.p[0].y += 1.0f;
                // triProjected.p[1].x += 1.0f;
                // triProjected.p[1].y += 1.0f;
                // triProjected.p[2].x += 1.0f;
                // triProjected.p[2].y += 1.0f;

                // triProjected.p[0].x *= 0.5f * (f32)SCREEN_W;
                // triProjected.p[0].y *= 0.5f * (f32)SCREEN_H;
                // triProjected.p[1].x *= 0.5f * (f32)SCREEN_W;
                // triProjected.p[1].y *= 0.5f * (f32)SCREEN_H;
                // triProjected.p[2].x *= 0.5f * (f32)SCREEN_W;
                // triProjected.p[2].y *= 0.5f * (f32)SCREEN_H;

                fill_triangle(&triProjected, color);
            }
        }

        render();
        time_elapsed += 0.1f;
    }

    render();

    // Infinite loop to prevent the kernel from exiting
    while (1)
        ;

    return;
}
