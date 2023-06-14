#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

#include "Vector4f.h"
#define TO_RADIANS(x) x*(M_PI / 180.0f)
#define TO_DEGREE (x) x * (180.0f / M_PI)

using namespace Vector4f;
namespace Matrix4x4f {

    typedef struct Matrix4x4f_t {
        union {
            struct {
                float x0, y0, z0, w0;
                float x1, y1, z1, w1;
                float x2, y2, z2, w2;
                float x3, y3, z3, w3;
            };
            struct {
                Vector4f_t v0;
                Vector4f_t v1;
                Vector4f_t v2;
                Vector4f_t v3;
            };

            float data[16];
            float data2d[4][4];
        };
    } Matrix4x4f_t;

    inline Matrix4x4f_t make_identityf() {
        Matrix4x4f_t m = { 0 };

        for (int i = 0; i < 4; i++) {
            m.data2d[i][i] = 1;
        }

        return m;
    }

    inline Matrix4x4f_t make_translate(float x, float y, float z) {
        Matrix4x4f_t m = make_identityf();
        m.data2d[0][3] = x;
        m.data2d[1][3] = y;
        m.data2d[2][3] = z;
        return m;
    }

    inline Matrix4x4f_t suma_matrix(Matrix4x4f_t m1, Matrix4x4f_t m2) {
        Matrix4x4f_t matrix_suma;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                matrix_suma.data2d[i][j] = m1.data2d[i][j] + m2.data2d[i][j];
            }
        }
        return matrix_suma;
    }

    inline Matrix4x4f_t resta_matrix(Matrix4x4f_t m1, Matrix4x4f_t m2) {
        Matrix4x4f_t matrix_resta;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                matrix_resta.data2d[i][j] = m1.data2d[i][j] - m2.data2d[i][j];
            }
        }
        return matrix_resta;
    }

    inline Matrix4x4f_t mult_matrix(Matrix4x4f_t m1, Matrix4x4f_t m2) {
        Matrix4x4f_t mult_matrix;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                mult_matrix.data2d[i][j] = 0;
                for (int k = 0; k < 4; k++) {
                        mult_matrix.data2d[i][j] += m1.data2d[i][k] * m2.data2d[k][j];
                }
            }
        }
        return mult_matrix;
    }

    inline Vector4f_t mult_vector(Matrix4x4f_t m1, Vector4f_t v1) {
        float aux[4];
        for (int i = 0; i < 4; i++) {
            aux[i] = 0;

            for (int j = 0; j < 4; j++) {
                aux[i] += m1.data2d[i][j] * v1.data[j];
            }
        }

        Vector4f_t mult_vector = Vector4f::make_vector4f(aux[0], aux[1], aux[2], aux[3]);

        return mult_vector;
    }

    inline Matrix4x4f_t escalar_matrix(Matrix4x4f_t m1, int escalar) {
        Matrix4x4f_t matrix_escalar;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                matrix_escalar.data2d[i][j] = m1.data2d[i][j] * escalar;
            }
        }
        return matrix_escalar;
    }

    inline Matrix4x4f_t inversa(Matrix4x4f_t m1) {
        Matrix4x4f_t mat, cof, adj, cofaux, inv, aux_inv;
        int aux_cof, det3, det4;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int d = 0; d < 4; d++) {
                    for (int e = 0; e < 4; e++) {
                        for (int k = 0; k < 4; k++) {
                            if ((i + k) % 2 == 0) {
                                aux_cof = 1;
                            }
                            else if ((i + k) % 2 != 0) {
                                aux_cof = -1;
                            }
                            if ((d == k || d != k) && (e == k || e == d)) {
                                cofaux.data2d[i][j] = aux_cof * ((mat.data2d[i][k] * mat.data2d[d][j]) -
                                    (mat.data2d[i][k] * mat.data2d[e][j]));
                            }
                        }
                    }
                }
            }
        }

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                cof.data2d[i][j] = cofaux.data2d[i][j];
            }
        }

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                adj.data2d[i][j] = cof.data2d[j][i];
            }
        }

        // buscamos determinante
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    for (int e = 0; e < 4; e++) {
                        int det = 0;
                        int det2 = 0;
                        for (int d = 0; d < 4; d++) {
                            if ((d != k || d == k) && (e == k || e == d) && i != j) {
                                if (i % 2 == 0 && j % 2 == 0) {
                                    det = det + ((mat.data2d[i][d] * mat.data2d[k][j]) -
                                        (mat.data2d[i][e] * mat.data2d[d][j]));
                                }
                                else if (i % 2 != 0 && j % 2 != 0) {
                                    det2 = det - ((mat.data2d[i][d] * mat.data2d[k][j]) -
                                        (mat.data2d[i][d] * mat.data2d[e][j]));
                                }
                            }
                            if (i == j) {
                                det3 = det3 + (mat.data2d[i][j] * det);
                                det4 = det4 + (mat.data2d[i][j] * det2);
                            }
                        }
                    }
                }
            }
        }

        int det5 = det3 + det4;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                aux_inv.data2d[i][j] = (1 / det5) * adj.data2d[i][j];
                inv.data2d[i][j] = aux_inv.data2d[i][j];
            }
        }

        return inv;
    }

    inline Matrix4x4f_t transpose(Matrix4x4f_t m1) {
        Matrix4x4f_t transp;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                transp.data2d[j][i] = m1.data2d[i][j];
            }
        }
        return transp;
    }

    inline Matrix4x4f_t make_rotate(Vector4f_t quaternion) {
        Matrix4x4f_t rotation;
        float x2 = quaternion.data[0] * quaternion.data[0];
        float y2 = quaternion.data[1] * quaternion.data[1];
        float z2 = quaternion.data[2] * quaternion.data[2];
        float xy = quaternion.data[0] * quaternion.data[1];
        float xz = quaternion.data[0] * quaternion.data[2];
        float yz = quaternion.data[1] * quaternion.data[2];
        float wx = quaternion.data[3] * quaternion.data[0];
        float wy = quaternion.data[3] * quaternion.data[1];
        float wz = quaternion.data[3] * quaternion.data[2];

        rotation.data2d[0][0] = 1.0f - 2.0f * (y2 + z2);
        rotation.data2d[1][0] = 2.0f * (xy - wz);
        rotation.data2d[2][0] = 2.0f * (xz + wy);
        rotation.data2d[3][0] = 0.0f;

        rotation.data2d[0][1] = 2.0f * (xy + wz);
        rotation.data2d[1][1] = 1.0f - 2.0f * (x2 + z2);
        rotation.data2d[2][1] = 2.0f * (yz - wx);
        rotation.data2d[3][1] = 0.0f;

        rotation.data2d[0][2] = 2.0f * (xz - wy);
        rotation.data2d[1][2] = 2.0f * (yz + wx);
        rotation.data2d[2][2] = 1.0f - 2.0f * (x2 + y2);
        rotation.data2d[3][2] = 0.0f;

        rotation.data2d[0][3] = 0.0f;
        rotation.data2d[1][3] = 0.0f;
        rotation.data2d[2][3] = 0.0f;
        rotation.data2d[3][3] = 1.0f;

        return rotation;
    }

    inline Matrix4x4f_t make_rotate(float angleX, float angleY, float angleZ) {
        Matrix4x4f_t matriz_rotacion = make_identityf();
        Matrix4x4f_t rot_x = make_identityf();
        Matrix4x4f_t rot_y = make_identityf();
        Matrix4x4f_t rot_z = make_identityf();

        // Paso de radianes a grados (en la practica se pide que rote 45º asi
        // que mejor asi, 45ª≈0,785398rad)
        // float x_degrees = angleX * 180 / M_PI;
        // float y_degrees = angleY * 180 / M_PI;
        // float z_degrees = angleZ * 180 / M_PI;

        // Radianes a grados
        float x_degrees = angleX * M_PI / 180;
        float y_degrees = angleY * M_PI / 180;
        float z_degrees = angleZ * M_PI / 180;

        // Rotacion en x
        rot_x.data2d[1][1] = cos(x_degrees);
        rot_x.data2d[1][2] = -sin(x_degrees);
        rot_x.data2d[2][1] = sin(x_degrees);
        rot_x.data2d[2][2] = cos(x_degrees);

        // Rotacion en y
        rot_y.data2d[0][0] = cos(y_degrees);
        rot_y.data2d[0][2] = sin(y_degrees);
        rot_y.data2d[2][0] = -sin(y_degrees);
        rot_y.data2d[2][2] = cos(y_degrees);

        // Rotacion en z
        rot_z.data2d[0][0] = cos(z_degrees);
        rot_z.data2d[0][1] = -sin(z_degrees);
        rot_z.data2d[1][0] = sin(z_degrees);
        rot_z.data2d[1][1] = cos(z_degrees);

        // rot_x * rot_y * rot_z
        matriz_rotacion = mult_matrix(mult_matrix(rot_x, rot_y), rot_z);

        return matriz_rotacion;
    }

    inline Matrix4x4f_t make_scale(float X, float Y, float Z) {
        Matrix4x4f_t scale;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                scale.data2d[i][j] = 0;
            }
        }

        scale.data2d[0][0] = X;
        scale.data2d[1][1] = Y;
        scale.data2d[2][2] = Z;
        scale.data2d[3][3] = 1;

        return scale;
    }

};  // namespace Matrix4x4f
