#include <cmath>
#include "PeriodicPlane.h"

void
rt::PeriodicPlane::draw(Viewer & /* viewer */) {
    rt::Material m = main_m;

    glColor4fv(m.ambient);
    glBegin(GL_TRIANGLES);

    glVertex2d(0, 0);
    glVertex2d(0 + 20, 0);
    glVertex2d(0, 0 + 20);

    glEnd();

    m = band_m;

    glColor4fv(m.ambient);
    glBegin(GL_TRIANGLES);

    glVertex2d(20, 20);
    glVertex2d(0 + 20, 0);
    glVertex2d(0, 0 + 20);

    glEnd();

}

rt::Vector3
rt::PeriodicPlane::getNormal( Point3 p ){
    return u.cross(v);
}

rt::Material
rt::PeriodicPlane::getMaterial( Point3 p ) {
    Real x;
    Real y;
    coordinates(p, x, y);

    if(x - floorf(x) > 1.0 - w
        || x - floorf(x) < 0.0 + w
        || y - floorf(y) > 1.0 - w
        || y - floorf(y) < 0.0 + w)
        return band_m;
    return main_m;
}

rt::Real
rt::PeriodicPlane::rayIntersection( const Ray& ray, Point3& p ) {
    Point3 O = ray.origin;
    Vector3 D = ray.direction;
    Vector3 n = getNormal(*p);
    Real d = distance(O, o);

    
    float t = -1.0*( (o.dot(n) + d) / (D.dot(n)) );
    if (t < 0)
        return 1.0f;
    else {
        p = Point3(o + D*t);
        return -1.0f;
    }
    return 1.0f;
}

void
rt::PeriodicPlane::coordinates( Point3 p, Real& x, Real& y ) const {
    x = p[0];
    y = p[1];
}