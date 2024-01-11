#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <inttypes.h>
#include <stdbool.h>
#include <math.h>

// struct for a vector with 3 components
struct Vec3 {
    float x, y, z;
};

struct Ray {
    struct Vec3 dir;
    struct Vec3 pos;
};

// struct for pixel-color
struct Color {
    uint8_t r; // red value
    uint8_t g; // green value
    uint8_t b; // blue value
};

// struct for pixel properties
struct Material {
    struct Vec3 color; // color struct
    bool reflective;    // reflective determinant
};

// Define a struct for a Sphere
struct Sphere {
    struct Vec3 pos;
    float radius;
    struct Material mat;
};

struct Triangle {
    struct Vec3 a, b, c;
    struct Material mat;
};

struct RayHit {
    float t;
    struct Vec3 hit;
    struct Vec3 normal;
    struct Material mat;
    int index;
};

// struct containing all image data
struct Image {
    struct Color *data;
    const int w, h;
};

// image dimensions
#define WIDTH 512
#define HEIGHT 512

// objects lengths
#define SPHERES_LEN 3
#define TRIANGLES_LEN 5

// predefined materials
const struct Material REFL  = { { 0.0f, 0.0f, 0.0f }, true  };
const struct Material BLUE  = { { 0.0f, 0.0f, 1.0f }, false };
const struct Material RED   = { { 1.0f, 0.0f, 0.0f }, false };
const struct Material WHITE = { { 1.0f, 1.0f, 1.0f }, false };

// constant material color
const struct Vec3 BLACK = { 0.0f, 0.0f, 0.0f };

// sphere objects
const struct Sphere SPHERES[SPHERES_LEN] = {
    { {  0.0f,  0.0f, -16.0f }, 2.0f, REFL  },
    { {  3.0f, -1.0f, -14.0f }, 1.0f, REFL },
    { { -3.0f, -1.0f, -14.0f }, 1.0f, RED  }
};

// triangle objects
const struct Triangle TRIANGLES[TRIANGLES_LEN] = {
    // back wall
    { { -8.0f, -2.0f, -20.0f }, { 8.0f, -2.0f, -20.0f }, {  8.0f, 10.0f, -20.0f }, BLUE },
    { { -8.0f, -2.0f, -20.0f }, { 8.0f, 10.0f, -20.0f }, { -8.0f, 10.0f, -20.0f }, BLUE },

    // floor
    { { -8.0f, -2.0f, -20.0f }, {  8.0f, -2.0f, -10.0f }, { 8.0f, -2.0f, -20.0f }, WHITE },
    { { -8.0f, -2.0f, -20.0f }, { -8.0f, -2.0f, -10.0f }, { 8.0f, -2.0f, -10.0f }, WHITE },

    // right RED triangle
    { { 8.0f, -2.0f, -20.0f }, { 8.0f, -2.0f, -10.0f }, { 8.0f, 10.0f, -20.0f }, RED }
};

// 3-dimensional environment sources
const struct Vec3 IMAGE_PLANE = { -1.0f, 1.0f, -2.0f };
const struct Vec3 CAMERA = { 0.0f, 0.0f, 0.0f };
const struct Vec3 LIGHT = { 3.0f, 5.0f, -15.0f };

// the sum of two vectors
struct Vec3 vec_add(const struct Vec3 u, const struct Vec3 v) {
    const struct Vec3 res = {
        u.x + v.x,
        u.y + v.y,
        u.z + v.z
    };
    return res;
}

// the difference of two vectors
struct Vec3 vec_sub(const struct Vec3 u, const struct Vec3 v) {
    const struct Vec3 dif = {
        u.x - v.x,
        u.y - v.y,
        u.z - v.z
    };
    return dif;
}

// the product of a scaled vector
struct Vec3 vec_scale(const struct Vec3 u, const float s) {
    const struct Vec3 prod = {
        u.x * s,
        u.y * s,
        u.z * s
    };
    return prod;
}

// the length between two vectors
float vec_len(const struct Vec3 u, const struct Vec3 v) {
    const float len = sqrtf(powf(u.x - v.x, 2.0f) + powf(u.y - v.y, 2.0f) + powf(u.z - v.z, 2.0f));
    return len;
}

// clamp the provided value between two boundaries
float clamp(float d, float min, float max) {
    const float t = d < min ? min : d;
    return t > max ? max : t;
}

// the dot product of two vectors
float dot(const struct Vec3 u, const struct Vec3 v) {
    return (u.x * v.x) + (u.y * v.y) + (u.z * v.z);
}

// the cross product of two vectors
struct Vec3 cross(const struct Vec3 u, const struct Vec3 v) {
    const struct Vec3 prod = {
        u.y * v.z - u.z * v.y,
        u.z * v.x - u.x * v.z,
        u.x * v.y - u.y * v.x
    };
    return prod;
}

// the unit length of the provided vector
struct Vec3 normalize(const struct Vec3 u) {
    const float len = sqrtf((u.x * u.x) + (u.y * u.y) + (u.z * u.z));
    const struct Vec3 unit = {u.x / len, u.y / len, u.z / len};
    return unit;
}

// the direction of a ray from the camera
struct Ray ray_dir(const int x, const int y) {
    const float PIXEL_SIZE = 2.0f / 512.0f;
    const float OFFSET = PIXEL_SIZE / 2.0f;

    // calculate the direction of ray
    const struct Vec3 pos = {
        IMAGE_PLANE.x + (x * PIXEL_SIZE + OFFSET), // x-component
        IMAGE_PLANE.y - (y * PIXEL_SIZE + OFFSET), // y-component
        IMAGE_PLANE.z                              // z-component
    };

    // normalize direction vector
    const struct Vec3 dir = normalize(pos);

    const struct Ray ray = {dir, CAMERA};
    return ray;
}

// the position of interaction (e + td)
struct Vec3 calculate_hit(const struct Ray ray, const float t) {
    struct Vec3 hit = {
        (ray.dir.x * t) + ray.pos.x,
        (ray.dir.y * t) + ray.pos.y,
        (ray.dir.z * t) + ray.pos.z
    };
    return hit;
}

// the intersection of a ray and a sphere
bool sphere_intersect(struct Ray ray, struct Sphere sphere, float *t) {
    const struct Vec3 d = ray.dir;    // ray direction
    const struct Vec3 e = ray.pos;    // ray start position
    const struct Vec3 c = sphere.pos; // sphere center
    const float R = sphere.radius;    // sphere radius

    // e - c
    const struct Vec3 e_c = vec_sub(e, c);

    // d * d
    const float n = dot(d, d);

    // -d * (e - c)
    const float l = dot(vec_scale(d, -1.0f), vec_sub(e, c));

    // sqrt(...)
    const float m = powf(dot(d, e_c), 2.0f) - n * (dot(e_c, e_c) - powf(R, 2.0f));

    if (m < 0.0f) {
        return false;
    } else {
        if (m == 0.0f) {
            *t = l / n;
        } else {
            const float o = sqrtf(m);

            const float t1 = (l + o) / n;
            const float t2 = (l - o) / n;

            *t = fmin(t1, t2);
        }
        return true;
    }
}

// the intersection of a ray and a triangle
bool triangle_intersect(struct Ray ray, struct Triangle tri, float *t) {
    const struct Vec3 a = tri.a;
    const struct Vec3 b = tri.b;
    const struct Vec3 c = tri.c;
    const struct Vec3 d = ray.dir;
    const struct Vec3 e = ray.pos;

    const float A = a.x - b.x;
    const float B = a.y - b.y;
    const float C = a.z - b.z;
    const float D = a.x - c.x;
    const float E = a.y - c.y;
    const float F = a.z - c.z;
    const float G = d.x;
    const float H = d.y;
    const float I = d.z;
    const float J = a.x - e.x;
    const float K = a.y - e.y;
    const float L = a.z - e.z;

    const float M = A * (E * I - H * F) + B * (G * F - D * I) + C * (D * H - E * G);
    if (M == 0.0f) {
        return false;
    }

    const float f = -((F * (A * K - J * B)) + E * (J * C - A * L) + D * (B * L - K * C)) / M;
    if (f < 0.0f) {
        return false;
    }

    const float gamma = (I * (A * K - J * B) + H * (J * C - A * L) + G * (B * L - K * C)) / M;
    if (gamma < 0.0f || gamma > 1.0f) {
        return false;
    }

    const float beta = (J * (E * I - H * F) + K * (G * F - D * I) + L * (D * H - E * G)) / M;
    if (beta < 0.0f || beta > 1.0f - gamma) {
        return false;
    }
    *t = f;
    return true;
}

// determine whether the ray intersects an object
bool contains_object(const struct Ray d, const int j) {
    float t;

    for (int i = 0; i < SPHERES_LEN; i++) {
        if (i != j && sphere_intersect(d, SPHERES[i], &t)) {
            return true;
        }
    }
    for (int i = 0; i < TRIANGLES_LEN; i++) {
        if (triangle_intersect(d, TRIANGLES[i], &t)) {
            return true;
        }
    }
    return false;
}

// information of the closest object if any 
bool closest_object(const struct Ray d, struct RayHit *rh, const int j) {
    bool contains = false;
    float t;

    for (int i = 0; i < SPHERES_LEN; i++) {
        if (i == j) {
            continue;
        }

        const struct Sphere obj = SPHERES[i];
        if (sphere_intersect(d, obj, &t)) {
            if (!contains || t < rh->t) {
                struct Vec3 hit = calculate_hit(d, t);
                struct Vec3 normal = normalize(vec_sub(hit, obj.pos));

                *rh = (struct RayHit) {
                    t, hit, normal, obj.mat, i
                };
                contains = true;
            }
        }
    }
    for (int i = 0; i < TRIANGLES_LEN; i++) {
        const struct Triangle obj = TRIANGLES[i];
        if (triangle_intersect(d, obj, &t)) {
            if (!contains || t < rh->t) {
                struct Vec3 hit = calculate_hit(d, t);
                struct Vec3 normal = normalize(cross(vec_sub(obj.b, obj.a), vec_sub(obj.c, obj.a)));

                *rh = (struct RayHit) {
                    t, hit, normal, obj.mat, i
                };
                contains = true;
            }
        }
    }
    return contains;
}

// the diffuse value baesd on the position of intersection and light source
float calc_diffuse(struct RayHit rh, const int i) {
    const struct Vec3 original_hit = rh.hit;
    const struct Vec3 original_normal = rh.normal;

    const struct Vec3 to_light = normalize(vec_sub(LIGHT, original_hit));

    // adjust for floating-point error
    const struct Vec3 hit_adjust = {
        original_hit.x + to_light.x * 0.001f,
        original_hit.y + to_light.y * 0.001f,
        original_hit.z + to_light.z * 0.001f
    };

    const struct Ray ray = { to_light, hit_adjust };
    
    if (closest_object(ray, &rh, i) && rh.t < vec_len(original_hit, LIGHT)) {
        return 0.2f; // shadow
    } else {
        // normal diffuse
        float prod = dot(original_normal, to_light);
        return clamp(prod, 0.2f, 1.0f);
    }
}

// reflection of provided ray based on the normal and hit position
struct Vec3 reflect(const struct Vec3 d, const struct Vec3 n) {
    const struct Vec3 r = vec_add(d, vec_scale(n, -2.0f * dot(d, n)));
    return r;
}

// properly instantiate the `Image` struct with the given dimensions
struct Image init(const int w, const int h) {
    return (struct Image) { calloc(w * h, sizeof(struct Color)), w, h };
}

// method for writing an `Image` to a png type file
void write_to_png(const char *filename, const struct Image img) {
    stbi_write_png(filename, img.w, img.h, 3, img.data, img.w * 3);
}