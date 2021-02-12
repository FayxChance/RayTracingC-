//
// Created by neptune on 12/02/2021.
//

#include "PeriodicPlane.h"

void rt::PeriodicPlane::coordinates(rt::Point3 p, rt::Real &x, rt::Real &y) {
    Vector3 op(p[0] - o[0], p[1] - o[1], p[2] - o[2]);
    x = u.dot(op);
    y = v.dot(op);
}

void rt::PeriodicPlane::draw(rt::Viewer &viewer) {
    rt::Material m = main_m;

    glColor4fv(m.ambient);
    glBegin(GL_TRIANGLES);

    glVertex3d(o[0], o[1], o[2]);
    glVertex3d((o + 10.0f * u)[0], (o + 10.0f * u)[1], (o + 10.0f * u)[2]);
    glVertex3d((o + 10.0f * v)[0], (o + 10.0f * v)[1], (o + 10.0f * v)[2]);

    glEnd();

    m = band_m;

    glColor4fv(m.ambient);
    glBegin(GL_TRIANGLES);

    Point3 p = o + 10.0f * u + 10.0f * v;

    glVertex3d(p[0], p[1], p[2]);
    glVertex3d((o + 10.0f * u)[0], (o + 10.0f * u)[1], (o + 10.0f * u)[2]);
    glVertex3d((o + 10.0f * v)[0], (o + 10.0f * v)[1], (o + 10.0f * v)[2]);

    glEnd();
}

rt::Vector3 rt::PeriodicPlane::getNormal(rt::Point3 p) {
    return u.cross(v)/ u.cross(v).norm();;
}

rt::Material rt::PeriodicPlane::getMaterial(rt::Point3 p) {
    Real x;
    Real y;
    coordinates(p, x, y);

    if (x - floorf(x) > 1.0 - w
        || x - floorf(x) < 0.0 + w
        || y - floorf(y) > 1.0 - w
        || y - floorf(y) < 0.0 + w)
        return band_m;
    return main_m;
}

rt::Real rt::PeriodicPlane::rayIntersection(const rt::Ray &ray, rt::Point3 &p) {
    Point3 O = ray.origin;
    Vector3 D = ray.direction / ray.direction.norm();
    Vector3 n = getNormal(p) / getNormal(p).norm();
    Real d = distance(O, o);


    float t = -1.0 * ((o.dot(n) + d) / (D.dot(n)));
    if (t < 0.0f)
        return 1.0f;
    else {
        p = Point3(o + D * t);
        return -1.0f;
    }
}
