#include <qapplication.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Viewer.h"
#include "Scene.h"
#include "Sphere.h"
#include "Material.h"
#include "PointLight.h"
#include "PeriodicPlane.h"

using namespace std;
using namespace rt;

void addBubble(Scene &scene, Point3 c, Real r, Material transp_m) {
    Material revert_m = transp_m;
    std::swap(revert_m.in_refractive_index, revert_m.out_refractive_index);
    Sphere *sphere_out = new Sphere(c, r, transp_m);
    Sphere *sphere_in = new Sphere(c, r - 0.02f, revert_m);
    scene.addObject(sphere_out);
    scene.addObject(sphere_in);
}


/**
  * @param centre : départ de la branche d'adn
  * @param rayonInterne : distance entre les 2 branches et le centre
  * @param rayonSphereExt : rayon des spheres des branches
  * @param hauteur : hauteur de la branche
  * @param bouleExterne : matériaux de la boule des branches
  * @param materials : tableau contenant 3 (et seulement 3) matériaux pour les liens entre les branches
*/
void addADN(Scene &scene, Point3 centre, Real rayonInterne, Real rayonSphereExt, Real hauteur, Material bouleExterne,
            Material *materials) {

    Vector3 u(1, 0, 0);
    Vector3 v(0, 1, 0);
    Vector3 w(0, 0, 1);
    Real theta = M_PI / 6;
    for (int i = 0; i < hauteur; ++i) {

        Vector3 xTheta = cos(theta * i) * u * rayonInterne;
        Vector3 yTheta = sin(theta * i) * v * rayonInterne;

        Point3 p = centre + xTheta + yTheta + w * rayonSphereExt * (Real) i;
        Point3 o = centre - xTheta - yTheta + w * rayonSphereExt * (Real) i;
        Vector3 op = (p - o);

        Point3 oP = o + (op / op.norm()) * rayonSphereExt;
        Point3 pP = p - (op / op.norm()) * rayonSphereExt;
        Vector3 oPpP = (pP - oP);

        Real rIntraBoulExt = oPpP.norm();
        Real rBouleInterne = rIntraBoulExt / 12;

        if (i % 3 == 1) {
            for (int j = 0; j < 12; ++j) {
                if (j % 2 == 1) {
                    Sphere *s;
                    if (j == 1 || j == 11)
                        s = new Sphere(oP + (1.0f / 12.0f) * oPpP * (Real) j, rBouleInterne,
                                       materials[0]);
                    else if (j == 3 || j == 9)
                        s = new Sphere(oP + (1.0f / 12.0f) * oPpP * (Real) j, rBouleInterne,
                                       materials[1]);
                    else
                        s = new Sphere(oP + (1.0f / 12.0f) * oPpP * (Real) j, rBouleInterne,
                                       materials[2]);

                    scene.addObject(s);
                }
            }
        }
        Sphere *sphereBorder1 = new Sphere(p, rayonSphereExt, bouleExterne);
        Sphere *sphereBorder2 = new Sphere(o, rayonSphereExt, bouleExterne);
        scene.addObject(sphereBorder1);
        scene.addObject(sphereBorder2);
    }
}

int main(int argc, char **argv) {
    // Read command lines arguments.
    QApplication application(argc, argv);

    // Creates a 3D scene
    Scene scene;


    Light *light1 = new PointLight(GL_LIGHT1, Point4(15, -4, 3, 1),

                                   Color(1.0, 1.0, 1.0));
    scene.addLight(light1);


    PeriodicPlane *pplane = new PeriodicPlane(Point3(0, 0, 0), Vector3(5, 0, 0), Vector3(0, 5, 0),
                                              Material::whitePlastic(), Material::blackMatter(), 0.05f);

   // scene.addObject(pplane);

    Point3 bouleOriginelPos(0, 0, 0);
    Vector3 xUnit(1.0, 0, 0);
    Vector3 yUnit(0, 1.0, 0);
    Vector3 zUnit(0, 0, 1.0);
    Real radius = 1.0;


// Objects
    Sphere *sphere1 = new Sphere(bouleOriginelPos, radius, Material::bronze());
    Sphere *sphere2 = new Sphere(bouleOriginelPos + 3.0f * xUnit + 0.0f * yUnit + 0.0f * zUnit, radius,
                                 Material::emerald());
    Sphere *sphere3 = new Sphere(bouleOriginelPos + 6.0f * xUnit + 0.0f * yUnit + 0.0f * zUnit, radius,
                                 Material::turquoise());
    Sphere *sphere4 = new Sphere(bouleOriginelPos + 9.0f * xUnit + 0.0f * yUnit + 0.0f * zUnit, radius,
                                 Material::glass());
    Sphere *sphere5 = new Sphere(bouleOriginelPos + 12.0f * xUnit + 0.0f * yUnit + 0.0f * zUnit, radius,
                                 Material::ruby());
    Sphere *sphere6 = new Sphere(bouleOriginelPos + 15.0f * xUnit + 0.0f * yUnit + 0.0f * zUnit, radius,
                                 Material::obsidian());
    Sphere *sphere7 = new Sphere(bouleOriginelPos + 18.0f * xUnit + 0.0f * yUnit + 0.0f * zUnit, radius,
                                 Material::jade());
    Sphere *sphere8 = new Sphere(bouleOriginelPos + 21.0f * xUnit + 0.0f * yUnit + 0.0f * zUnit, radius,
                                 Material::gold());
    Sphere *sphere9 = new Sphere(bouleOriginelPos + 24.0f * xUnit + 0.0f * yUnit + 0.0f * zUnit, radius,
                                 Material::silver());
    Sphere *sphere10 = new Sphere(bouleOriginelPos + 27.0f * xUnit + 0.0f * yUnit + 0.0f * zUnit, radius,
                                  Material::copper());
    Sphere *sphere11 = new Sphere(bouleOriginelPos + 30.0f * xUnit + 0.0f * yUnit + 0.0f * zUnit, radius,
                                  Material::pearl());
    scene.addObject(sphere1);
    scene.addObject(sphere2);
    scene.addObject(sphere3);
    scene.addObject(sphere4);
    scene.addObject(sphere5);
    scene.addObject(sphere6);
    scene.addObject(sphere7);
    scene.addObject(sphere8);
    scene.addObject(sphere11);
    scene.addObject(sphere10);
    scene.addObject(sphere9);


    addBubble(scene, bouleOriginelPos + 0.0f * xUnit + 3.0f * yUnit + 0.0f * zUnit, radius, Material::bronze());
    addBubble(scene, bouleOriginelPos + 3.0f * xUnit + 3.0f * yUnit + 0.0f * zUnit, radius, Material::emerald());
    addBubble(scene, bouleOriginelPos + 6.0f * xUnit + 3.0f * yUnit + 0.0f * zUnit, radius, Material::whitePlastic());
    addBubble(scene, bouleOriginelPos + 9.0f * xUnit + 3.0f * yUnit + 0.0f * zUnit, radius, Material::glass());

    Material m1s[] = {Material::ruby(), Material::turquoise(), Material::pearl()};
    Material m2s[] = {Material::gold(), Material::copper(), Material::chrome()};
    Material m3s[] = {Material::jade(), Material::jade(), Material::jade()};
    Material m4s[] = {Material::obsidian(), Material::obsidian(), Material::obsidian()};
    //addADN(scene, Point3(40, 40, 0), 20, 3, 30, Material::emerald(), m1s);
    //addADN(scene, Point3(-40, -40, 0), 20, 3, 30, Material::silver(), m2s);
    //addADN(scene, Point3(-40, 40, 0), 20, 3, 30, Material::obsidian(), m3s);
    //addADN(scene, Point3(40, -40, 0), 20, 3, 30, Material::jade(), m4s);




    // Instantiate the viewer.
    Viewer viewer;
    // Give a name
    viewer.setWindowTitle("Ray-tracer preview");

    // Sets the scene
    viewer.setScene(scene);

    // Make the viewer window visible on screen.
    viewer.show();
    // Run main loop.
    application.exec();
    return 0;
}
