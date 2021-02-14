//
// Created by neptune on 12/02/2021.
//
#pragma once
#ifndef RAYTRACINGC_PERIODICPLANE_H
#define RAYTRACINGC_PERIODICPLANE_H

#include "GraphicalObject.h"

namespace rt {
    struct PeriodicPlane : public GraphicalObject // Vous devez dériver de GraphicalObject
    {

        virtual ~PeriodicPlane() {}

        PeriodicPlane(Point3 o, Vector3 u, Vector3 v, Material main_m, Material band_m, Real w)
                : GraphicalObject(), o(o), u(u/u.norm()), v(v/v.norm()), main_m(main_m), band_m(band_m), w(w) {}


    public:
        void coordinates(Point3 p, Real &x, Real &y);

        void init(Viewer & /* viewer */ ) {}

        void draw(Viewer &viewer);

        Vector3 getNormal(Point3 p);

        Material getMaterial(Point3 p);

        Real rayIntersection(const Ray &ray, Point3 &p);


    public:
        Point3 o;
        Vector3 u;
        Vector3 v;
        Material main_m;
        Material band_m;
        Real w;

    };
}


#endif //RAYTRACINGC_PERIODICPLANE_H
