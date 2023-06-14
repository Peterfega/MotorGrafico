#pragma once
#include <vector>

#include "Matrix4x4f.h"

typedef struct particle {
    Vector4f_t max, min;
};

class Collider {
   public:
    typedef enum { sphere,
                   AABB } collTypes;
    collTypes type;

    Collider(){};
    std::vector<particle> partList;
    std::vector<Collider*> sons;

    virtual void addParticle(particle part) = 0;
    virtual bool test(Collider* c2) = 0;
    virtual void update(Matrix4x4f::Matrix4x4f_t mat) = 0;
    virtual void subdivide() = 0;
};

class sphere : public Collider {
   public:
    Vector4f_t center = make_vector4f(0.0f, 0.0f, 0.0f, 1.0f);
    float radius = 0;
    Vector4f_t minCoord = make_vector4f(9999.0f, 9999.0f, 9999.0f, 1.0f);
    ;
    Vector4f_t maxCoord = make_vector4f(-9999.0f, -9999.0f, -9999.0f, 1.0f);
    ;

    void addParticle(particle part) override {
        partList.push_back(part);
        if (partList.size() == 1) {
            minCoord = part.min;
            maxCoord = part.max;

        } else {
            if (minCoord.x > part.min.x) minCoord.x = part.min.x;
            if (minCoord.y > part.min.y) minCoord.y = part.min.y;
            if (minCoord.z > part.min.z) minCoord.z = part.min.z;

            if (maxCoord.x < part.max.x) maxCoord.x = part.max.x;
            if (maxCoord.y < part.max.y) maxCoord.y = part.max.y;
            if (maxCoord.z < part.max.z) maxCoord.z = part.max.z;
        }
        center = (maxCoord + minCoord) / 2.0f;
        radius = Vector4f::lenght(maxCoord - minCoord)/2.0f;
    }

    bool test(Collider* c2) override {
        if (c2->type != collTypes::sphere)
            return false;
        sphere* s2 = (sphere*)c2;
        return (Vector4f::lenght(this->center - s2->center) < (this->radius + s2->radius));


        /*if (lenght(this->center - s2->center) < (this->radius + s2->radius)) {
            // ambos son hojas
            if (this->sons.size() == 0 && s2->sons.size() == 0) {
                return true;  // (libMath::length(this->center - s2->center) < (this->radius + s2->radius));
            } else            // una hoja y una rama
            {
                if (this->sons.size() > 0 && s2->sons.size() == 0) {
                    bool res = false;
                    for (auto& c : this->sons) {
                        res |= c->test(s2);
                    }
                    return res;
                } else if (s2->sons.size() > 0 && this->sons.size() == 0) {
                    bool res = false;
                    for (auto& c : s2->sons) {
                        res |= c->test(this);
                    }
                    return res;
                } else {  // ambos ramas
                    bool res = false;
                    for (auto& c : this->sons)
                        for (auto& c2 : s2->sons) {
                            res |= c->test(c2);
                        }
                    return res;
                }
            }
        } else {
            return false;
        }*/

    }

    void update(Matrix4x4f::Matrix4x4f_t mat) override {
        Vector4f_t max = Matrix4x4f::mult_vector(mat, maxCoord);
        ;
        Vector4f_t min = Matrix4x4f::mult_vector(mat, minCoord);
        ;

        center = (max + min) / 2.0f;
        radius = lenght(max - min) / 2.0f;

        for (auto& c : sons) {
            c->update(mat);
        }
        // center = mat * center;
    }

    void subdivide() {
        if (partList.size() == 1) return;

        sphere* SI = new sphere();
        sphere* SD = new sphere();
        sphere* II = new sphere();
        sphere* ID = new sphere();

        for (auto& p : partList) {
            Vector4f_t pcenter = (p.max + p.min) / 2.0f;
            // izquierda de centro
            if (pcenter.x < center.x) {
                // superior centro
                if (pcenter.y > center.y)
                    SI->addParticle(p);
                else
                    II->addParticle(p);
            } else {
                // superior centro
                if (pcenter.y > center.y)
                    SD->addParticle(p);
                else
                    ID->addParticle(p);
            }
        }

        if (SI->partList.size() > 0) {
            sons.push_back(SI);
            if (SI->partList.size() > 1) SI->subdivide();
        }
        if (SD->partList.size() > 0) {
            sons.push_back(SD);
            if (SD->partList.size() > 1)
                SD->subdivide();
        }
        if (II->partList.size() > 0) {
            sons.push_back(II);
            if (II->partList.size() > 1)
                II->subdivide();
        }
        if (ID->partList.size() > 0) {
            sons.push_back(ID);
            if (ID->partList.size() > 1)
                ID->subdivide();
        }
    }
    sphere() {
        this->type = collTypes::sphere;
    }
    ~sphere() {}
};
