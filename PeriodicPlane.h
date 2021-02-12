#pragma once
#ifndef _PERIODIC_PLANE_H_
#define _PERIODIC_PLANE_H_

#include "GraphicalObject.h"

/// Namespace RayTracer
namespace rt {
  /// A sphere is a concrete GraphicalObject that represents a sphere in 3D space.
  struct PeriodicPlane : public GraphicalObject {
    virtual ~PeriodicPlane() {}
    PeriodicPlane( Point3 o, Vector3 u, Vector3 v,
                    Material main_m, Material band_m, Real w)
        : GraphicalObject(), o(o), u(u), v(v), main_m(main_m), band_m(band_m), w(w)
        {}
    void coordinates( Point3 p, Real& x, Real& y ) const;

    // ---------------- GraphicalObject services ----------------------------
  public:
    void init( Viewer& /* viewer */ ) {}
    void draw( Viewer& viewer );
    Vector3 getNormal( Point3 p );
    Material getMaterial( Point3 p );
    Real rayIntersection( const Ray& ray, Point3& p );

  public:
    Point3 o;
    Vector3 u;
    Vector3 v;
    Material main_m;
    Material band_m;
    Real w;
  };

} //

#endif