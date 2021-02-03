/**
@file Sphere.cpp
*/
#include <cmath>
#include "Sphere.h"

void
rt::Sphere::draw(Viewer & /* viewer */ ) {
    Material m = material;
    // Taking care of south pole
    glBegin(GL_TRIANGLE_FAN);
    glColor4fv(m.ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, m.diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, m.specular);
    glMaterialf(GL_FRONT, GL_SHININESS, m.shinyness);
    Point3 south_pole = localize(-90, 0);
    glNormal3fv(getNormal(south_pole));
    glVertex3fv(south_pole);
    for (int x = 0; x <= NLON; ++x) {
        Point3 p = localize(-90 + 180 / NLAT, x * 360 / NLON);
        glNormal3fv(getNormal(p));
        glVertex3fv(p);
    }
    glEnd();
    // Taking care of in-between poles
    for (int y = 1; y < NLAT - 1; ++y) {
        glBegin(GL_QUAD_STRIP);
        glColor4fv(m.ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, m.diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, m.specular);
        glMaterialf(GL_FRONT, GL_SHININESS, m.shinyness);
        for (int x = 0; x <= NLON; ++x) {
            Point3 p = localize(-90 + y * 180 / NLAT, x * 360 / NLON);
            Point3 q = localize(-90 + (y + 1) * 180 / NLAT, x * 360 / NLON);
            glNormal3fv(getNormal(p));
            glVertex3fv(p);
            glNormal3fv(getNormal(q));
            glVertex3fv(q);
        }
        glEnd();
    }
    // Taking care of north pole
    glBegin(GL_TRIANGLE_FAN);
    glColor4fv(m.ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, m.diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, m.specular);
    glMaterialf(GL_FRONT, GL_SHININESS, m.shinyness);
    Point3 north_pole = localize(90, 0);
    glNormal3fv(getNormal(north_pole));
    glVertex3fv(north_pole);
    for (int x = NLON; x >= 0; --x) {
        Point3 p = localize(-90 + (NLAT - 1) * 180 / NLAT, x * 360 / NLON);
        glNormal3fv(getNormal(p));
        glVertex3fv(p);
    }
    glEnd();
}

rt::Point3
rt::Sphere::localize(Real latitude, Real longitude) const {
    static const Real conv_deg_rad = 2.0 * M_PI / 360.0;
    latitude *= conv_deg_rad;
    longitude *= conv_deg_rad;
    return center
           + radius * Point3(cos(longitude) * cos(latitude),
                             sin(longitude) * cos(latitude),
                             sin(latitude));
}

rt::Vector3
rt::Sphere::getNormal(Point3 p) {
    Vector3 u = p - center;
    Real l2 = u.dot(u);
    if (l2 != 0.0) u /= sqrt(l2);
    return u;
}

rt::Material
rt::Sphere::getMaterial(Point3 /* p */) {
    return material; // the material is constant along the sphere.
}

rt::Real
rt::Sphere::rayIntersection(const Ray &ray, Point3 &p) {

    Vector3 origineCentre( ray.origin[0] - center[0],
                           ray.origin[1] - center[1],
                           ray.origin[2] - center[2]);

    Point3 pointNormal = ray.origin + ray.direction.dot(origineCentre * (-1.0f)) *
                                      ray.direction;

    if (distance2(pointNormal, center) > radius * radius) {
        return 1.0f;
    }

    float c = distance2(center,ray.origin)-pow(radius, 2);

    float b = 2 * ray.direction.dot(origineCentre);

    float a = 1;

    float delta = pow(b, 2) - 4 * a * c;

    if (delta >= 0.0f) {
        float sol1 = (-b - sqrt(delta)) / (2 * a);
        float sol2 = (-b + sqrt(delta)) / (2 * a);

        if(sol2 < 0.0f){
            return 1.0f;
        }
        else if (sol1 < 0.0f){
            p = Point3(ray.origin + ray.direction * sol2);
            return -1.0f;
        }
        else {
            p = Point3(ray.origin + ray.direction * sol1);
            return -1.0f;
        }
    }
    return 1.0f;
}
