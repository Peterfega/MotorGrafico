#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

namespace Vector4f {
    typedef struct Vector4f_t {
        union {
            float data[4];
            struct {
                float x, y, z, w;
            };
            struct {
                float r, g, b, a;
            };
        };
    } Vector4f_t;

    inline Vector4f_t make_vector4f(float x, float y, float z, float w) {
        Vector4f_t vector;
        vector.data[0] = x;
        vector.data[1] = y;
        vector.data[2] = z;
        vector.data[3] = w;
        return vector;
    }

    inline Vector4f_t normalize(Vector4f_t v) {
        float modulo = sqrt(pow(v.data[0], 2) + pow(v.data[1], 2) + pow(v.data[2], 2));
        Vector4f_t normalized_vector;
        normalized_vector.data[0] = v.data[0] / modulo;
        normalized_vector.data[1] = v.data[1] / modulo;
        normalized_vector.data[2] = v.data[2] / modulo;
        normalized_vector.data[3] = 0;
        return normalized_vector;
    }

    inline Vector4f_t suma_vector(Vector4f_t v1, Vector4f_t v2) {
        Vector4f_t vector_suma;
        vector_suma.data[0] = v1.data[0] + v2.data[0];
        vector_suma.data[1] = v1.data[1] + v2.data[1];
        vector_suma.data[2] = v1.data[2] + v2.data[2];
        vector_suma.data[3]=v1.data[3]+v2.data[3]; 
        // vector_suma.data[3]=0;
        return vector_suma;
    }

    inline Vector4f_t resta_vector(Vector4f_t v1, Vector4f_t v2) {
        Vector4f_t vector_resta;
        vector_resta.data[0] = v1.data[0] - v2.data[0];
        vector_resta.data[1] = v1.data[1] - v2.data[1];
        vector_resta.data[2] = v1.data[2] - v2.data[2];
        vector_resta.data[3]=v1.data[3]+v2.data[3]; 
        // vector_resta.data[3]=0;
        return vector_resta;
    }

    inline float dot_product_vector(Vector4f_t v1, Vector4f_t v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
    }

    inline Vector4f_t cross_product_vector(Vector4f_t v1, Vector4f_t v2) {
        // Vector4f_t crossed_vector;
        // float i = (v1.data[1] * v2.data[2]) - (v1.data[2] * v2.data[1]);
        // float j = (v1.data[2] * v2.data[0]) - (v1.data[0] * v2.data[2]);
        // float k = (v1.data[0] * v2.data[1]) - (v1.data[1] * v2.data[0]);
        // crossed_vector.data[0] = i;
        // crossed_vector.data[1] = j;
        // crossed_vector.data[2] = k;
        // crossed_vector.data[3] = 0;
        // return crossed_vector;

        return make_vector4f(v1.y * v2.z - v1.z * v2.y, -v1.x * v2.z + v1.z * v2.x,
            v1.x * v2.y - v1.y * v2.x, 0.0f);
    }

    inline Vector4f_t make_quaternion(float x, float y, float z, float angle) {
        Vector4f_t q;
        q.data[1] = x * sin(angle / 2);
        q.data[2] = y * sin(angle / 2);
        q.data[3] = z * sin(angle / 2);
        q.data[4] = cos(angle / 2);
        return q;
    }
    inline  Vector4f_t operator+(Vector4f_t v1, Vector4f_t v2) {
        return make_vector4f(v1.data[0] + v2.data[0], v1.data[1] + v2.data[1], v1.data[2] + v2.data[2], v1.data[3] + v2.data[3]);
    }

    inline  Vector4f_t operator-(Vector4f_t v1, Vector4f_t v2) {
        return make_vector4f(v1.data[0] - v2.data[0], v1.data[1] - v2.data[1], v1.data[2] - v2.data[2], v1.data[3] - v2.data[3]);
    }

    inline  Vector4f_t operator*(Vector4f_t v1, float v2)
    {
        return make_vector4f(v1.data[0] * v2, v1.data[1] * v2, v1.data[2] * v2, v1.data[3] * v2);

    }
    inline  Vector4f_t operator*(float v2, Vector4f_t v1)
    {
        return make_vector4f(v1.data[0] * v2, v1.data[1] * v2, v1.data[2] * v2, v1.data[3] * v2);

    }

    inline  Vector4f_t operator/(float v2, Vector4f_t v1)
    {
        return make_vector4f(v1.data[0] / v2, v1.data[1] / v2, v1.data[2] / v2, v1.data[3] / v2);

    }


    inline  Vector4f_t operator/(Vector4f_t v1, float v2)
    {
        return make_vector4f(v1.data[0] / v2, v1.data[1] / v2, v1.data[2] / v2, v1.data[3] / v2);

    }

    inline float lenght(Vector4f_t v) {
       float lenght = sqrt(v.data[0] * v.data[0] + v.data[1] * v.data[1] + v.data[2] * v.data[2]);
       return lenght;
    }

};  // namespace Vector4f
