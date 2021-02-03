//
// Created by neptune on 02/02/2021.
//
#pragma once

#ifndef TP2_BACKGROUND_H
#define TP2_BACKGROUND_H

#include "Color.h"
#include "Image2D.h"
#include "Ray.h"

namespace rt {

    struct Background {
        virtual Color backgroundColor(const Ray &ray) = 0;
    };

    struct MyBackground : public Background {
        Color backgroundColor(const Ray &ray) {
            Color result;
            if (0.0f < ray.direction[2] && ray.direction[2] < 0.5f) {
                result += (1.0f - ray.direction[2]) * Color(1, 1, 1) + ray.direction[2] * Color(0, 0, 1);
            } else if (0.5f < ray.direction[2] && ray.direction[2] < 1.0f) {
                result += (1.0f - ray.direction[2]) * Color(1, 1, 1) + ray.direction[2] * Color(0, 0, 1);
            } else {
                Real x = -0.5f * ray.direction[0] / ray.direction[2];
                Real y = -0.5f * ray.direction[1] / ray.direction[2];
                Real d = sqrt(x * x + y * y);
                Real t = std::min(d, 30.0f) / 30.0f;
                x -= floor(x);
                y -= floor(y);
                if (((x >= 0.5f) && (y >= 0.5f)) || ((x < 0.5f) && (y < 0.5f)))
                    result += (1.0f - t) * Color(0.2f, 0.2f, 0.2f) + t * Color(1.0f, 1.0f, 1.0f);
                else
                    result += (1.0f - t) * Color(0.4f, 0.4f, 0.4f) + t * Color(1.0f, 1.0f, 1.0f);
            }
            return result;
        }
    };
}

#endif //TP2_BACKGROUND_H
