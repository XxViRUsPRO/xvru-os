#include "stdio.h"
#include "string.h"
#include "magic.h"
#include "math.h"
#include "display.h"
#include <hal/hal.h>
#include <types.h>
#include <drivers/timer.h>
#include "vector.h"

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

// VECT_GENERATE_NAME(i32, i32);

void _main32(void)
{
    HAL_init();
    timer_init();

    clear_screen();
    f32 time = 0.0f;
    while (1)
    {
        printf("Hello World! %f\r\n", time);
        time += 0.1f;
        sleep(100);
    }

    // vect_i32 *v = vect_init_i32(VECT_CAPACITY);
    // vect_push_i32(v, 1);
    // vect_push_i32(v, 2);

    // i32 *data = vect_ptr_i32(v, 0);
    // u32 size = v->size;
    // for (u32 i = 0; i < size; i++)
    // {
    //     printf("%d\r\n", data[i]);
    // }
    // printf("Vector<i32> Size: %d Capacity: %d Data Size: %d\r\n", v->size, v->capacity, v->data_size);

    // mesh_t mesh = {
    //     // South face
    //     (triangle_t){VEC3D(0.0f, 0.0f, 0.0f), VEC3D(0.0f, 1.0f, 0.0f), VEC3D(1.0f, 1.0f, 0.0f)},
    //     (triangle_t){VEC3D(0.0f, 0.0f, 0.0f), VEC3D(1.0f, 1.0f, 0.0f), VEC3D(1.0f, 0.0f, 0.0f)},

    //     // East face
    //     (triangle_t){VEC3D(1.0f, 0.0f, 0.0f), VEC3D(1.0f, 1.0f, 0.0f), VEC3D(1.0f, 1.0f, 1.0f)},
    //     (triangle_t){VEC3D(1.0f, 0.0f, 0.0f), VEC3D(1.0f, 1.0f, 1.0f), VEC3D(1.0f, 0.0f, 1.0f)},

    //     // North face
    //     (triangle_t){VEC3D(1.0f, 0.0f, 1.0f), VEC3D(1.0f, 1.0f, 1.0f), VEC3D(0.0f, 1.0f, 1.0f)},
    //     (triangle_t){VEC3D(1.0f, 0.0f, 1.0f), VEC3D(0.0f, 1.0f, 1.0f), VEC3D(0.0f, 0.0f, 1.0f)},

    //     // West face
    //     (triangle_t){VEC3D(0.0f, 0.0f, 1.0f), VEC3D(0.0f, 1.0f, 1.0f), VEC3D(0.0f, 1.0f, 0.0f)},
    //     (triangle_t){VEC3D(0.0f, 0.0f, 1.0f), VEC3D(0.0f, 1.0f, 0.0f), VEC3D(0.0f, 0.0f, 0.0f)},

    //     // Top face
    //     (triangle_t){VEC3D(0.0f, 1.0f, 0.0f), VEC3D(0.0f, 1.0f, 1.0f), VEC3D(1.0f, 1.0f, 1.0f)},
    //     (triangle_t){VEC3D(0.0f, 1.0f, 0.0f), VEC3D(1.0f, 1.0f, 1.0f), VEC3D(1.0f, 1.0f, 0.0f)},

    //     // Bottom face
    //     (triangle_t){VEC3D(1.0f, 0.0f, 1.0f), VEC3D(0.0f, 0.0f, 1.0f), VEC3D(0.0f, 0.0f, 0.0f)},
    //     (triangle_t){VEC3D(1.0f, 0.0f, 1.0f), VEC3D(0.0f, 0.0f, 0.0f), VEC3D(1.0f, 0.0f, 0.0f)},
    // };
    // vec3d vCamera = {0.0f, 0.0f, 0.0f};

    // mat4x4 matProj;
    // mat4x4 matRotZ, matRotX;

    // f32 fNear = 0.1f;
    // f32 fFar = 1000.0f;
    // f32 fFov = 90.0f;
    // f32 fAspectRatio = (f32)SCREEN_H / (f32)SCREEN_W;
    // f32 fFovRad = 1.0f / tan(fFov * 0.5f / 180.0f * PI);

    // // Projection Matrix
    // matProj.m[0][0] = fAspectRatio * fFovRad;
    // matProj.m[1][1] = fFovRad;
    // matProj.m[2][2] = fFar / (fFar - fNear);
    // matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    // matProj.m[2][3] = 1.0f;

    // f32 time_elapsed = 0.0f;
    // while (1)
    // {
    //     f32 fTheta = time_elapsed;
    //     // Rotation Z Matrix
    //     matRotZ.m[0][0] = cos(fTheta);
    //     matRotZ.m[0][1] = sin(fTheta);
    //     matRotZ.m[1][0] = -sin(fTheta);
    //     matRotZ.m[1][1] = cos(fTheta);
    //     matRotZ.m[2][2] = 1.0f;
    //     matRotZ.m[3][3] = 1.0f;

    //     // Rotation X Matrix
    //     matRotX.m[0][0] = 1.0f;
    //     matRotX.m[1][1] = cos(fTheta * 0.5f);
    //     matRotX.m[1][2] = sin(fTheta * 0.5f);
    // matRotX.m[2][1] = -matRotX.m[1][2];
    // matRotX.m[2][2] = cos(fTheta * 0.5f);
    // matRotX.m[3][3] = 1.0f;

    // for (int i = 0; i < 12; i++)
    // {
    //     triangle_t t, triProjected, triTranslated, triRotatedZ, triRotatedZX;
    //     t = mesh.t[i];

    //     // Rotate in Z-Axis
    //     multiply_matrix_vector(t.p[0], &triRotatedZ.p[0], matRotZ);
    //     multiply_matrix_vector(t.p[1], &triRotatedZ.p[1], matRotZ);
    //     multiply_matrix_vector(t.p[2], &triRotatedZ.p[2], matRotZ);

    //     // Rotate in X-Axis
    //     multiply_matrix_vector(triRotatedZ.p[0], &triRotatedZX.p[0], matRotX);
    //     multiply_matrix_vector(triRotatedZ.p[1], &triRotatedZX.p[1], matRotX);
    //     multiply_matrix_vector(triRotatedZ.p[2], &triRotatedZX.p[2], matRotX);

    //     // Offset into the screen
    //     triTranslated = triRotatedZX;
    //     triTranslated.p[0].z = triRotatedZX.p[0].z + 2.5f;
    //     triTranslated.p[1].z = triRotatedZX.p[1].z + 2.5f;
    //     triTranslated.p[2].z = triRotatedZX.p[2].z + 2.5f;

    //     // Compute triangle normal
    //     vec3d normal, line1, line2;
    //     line1.x = triTranslated.p[1].x - triTranslated.p[0].x;
    //     line1.y = triTranslated.p[1].y - triTranslated.p[0].y;
    //     line1.z = triTranslated.p[1].z - triTranslated.p[0].z;

    //     line2.x = triTranslated.p[2].x - triTranslated.p[0].x;
    //     line2.y = triTranslated.p[2].y - triTranslated.p[0].y;
    //     line2.z = triTranslated.p[2].z - triTranslated.p[0].z;

    //     normal.x = line1.y * line2.z - line1.z * line2.y;
    //     normal.y = line1.z * line2.x - line1.x * line2.z;
    //     normal.z = line1.x * line2.y - line1.y * line2.x;

    //     // Normalize
    //     f32 l = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
    //     normal.x /= l;
    //     normal.y /= l;
    //     normal.z /= l;

    //     if (normal.x * (triTranslated.p[0].x - vCamera.x) + normal.y * (triTranslated.p[0].y - vCamera.y) + normal.z * (triTranslated.p[0].z - vCamera.z) < 0.0f)
    //     {
    //         // Illumination
    //         vec3d light_direction = {0.0f, 0.0f, -1.0f};
    //         f32 l = sqrt(light_direction.x * light_direction.x + light_direction.y * light_direction.y + light_direction.z * light_direction.z);
    //         light_direction.x /= l;
    //         light_direction.y /= l;
    //         light_direction.z /= l;

    //         f32 dp = MAX(0.1f, normal.x * light_direction.x + normal.y * light_direction.y + normal.z * light_direction.z);

    //         u8 color = (u8)(dp * 15.0f);

    //         // Project triangles from 3D --> 2D
    //         multiply_matrix_vector(triTranslated.p[0], &triProjected.p[0], matProj);
    //         multiply_matrix_vector(triTranslated.p[1], &triProjected.p[1], matProj);
    //         multiply_matrix_vector(triTranslated.p[2], &triProjected.p[2], matProj);

    //         fill_triangle(&triProjected, 100, color);
    //     }
    // }

    //     draw_line(VEC2D(0, 0), VEC2D(cos(fTheta) * 100, sin(fTheta) * 100), 15);

    //     render();
    //     sleep(100);
    //     time_elapsed += 0.1f;
    //     clear_vga_buffer();
    // }

    // Infinite loop to prevent the kernel from exiting
    while (1)
        ;

    return;
}
