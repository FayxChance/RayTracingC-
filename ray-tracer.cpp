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

void addBubble( Scene& scene, Point3 c, Real r, Material transp_m )
{
    Material revert_m = transp_m;
    std::swap( revert_m.in_refractive_index, revert_m.out_refractive_index );
    Sphere* sphere_out = new Sphere( c, r, transp_m );
    Sphere* sphere_in  = new Sphere( c, r-0.02f, revert_m );
    scene.addObject( sphere_out );
    scene.addObject( sphere_in );
}


int main(int argc, char** argv)
{
  // Read command lines arguments.
  QApplication application(argc,argv);
  
  // Creates a 3D scene
  Scene scene;

    // Light at infinity
    Light* light0 = new PointLight( GL_LIGHT0, Point4( 0,0,1,0 ),
                                    Color( 1.0, 1.0, 1.0 ) );
    Light* light1 = new PointLight( GL_LIGHT1, Point4( -10,-4,2,1 ),
                                    Color( 1.0, 1.0, 1.0 ) );
    scene.addLight( light0 );
    scene.addLight( light1 );
    Point3 bouleOriginelPos(1.0,1.0,1.0);
    Vector3 xUnit(1.0,0,0);
    Vector3 yUnit(0,1.0,0);
    Vector3 zUnit(0,0,1.0);
    Real radius = 1.0;
// Objects
    Sphere* sphere1 = new Sphere( bouleOriginelPos, radius, Material::bronze() );
    Sphere* sphere2 = new Sphere( bouleOriginelPos + 3.0f * xUnit + 3.0f * yUnit + 3.0f * zUnit, radius, Material::emerald() );
    Sphere* sphere3 = new Sphere( bouleOriginelPos + 6.0f * xUnit + 6.0f * yUnit + 6.0f * zUnit, radius, Material::whitePlastic() );
    Sphere* sphere4 = new Sphere( bouleOriginelPos + 9.0f * xUnit + 9.0f * yUnit + 9.0f * zUnit, radius, Material::glass() );
    scene.addObject( sphere1 );
    scene.addObject( sphere2 );
    scene.addObject( sphere3 );
    scene.addObject( sphere4 );

    addBubble(scene,bouleOriginelPos + 0.0f * xUnit + 6.0f * yUnit + 0.0f * zUnit,radius,Material::bronze());
    addBubble(scene,bouleOriginelPos + 3.0f * xUnit + 9.0f * yUnit + 0.0f * zUnit,radius,Material::emerald());
    addBubble(scene,bouleOriginelPos + 6.0f * xUnit + 12.0f * yUnit + 0.0f * zUnit,radius,Material::whitePlastic());
    addBubble(scene,bouleOriginelPos + 9.0f * xUnit + 15.0f * yUnit + 0.0f * zUnit,radius,Material::glass());


    PeriodicPlane* pplane = new PeriodicPlane( Point3( 0.0, 0.0, 0.0 ), Vector3( 5.0, 0.0, 0.0 ), Vector3( 0.0, 5.0, 0.0 ),
        Material::whitePlastic(), Material::blackMatter(), 0.05f ); 
    scene.addObject(pplane);

    // Instantiate the viewer.
  Viewer viewer;
  // Give a name
  viewer.setWindowTitle("Ray-tracer preview");

  // Sets the scene
  viewer.setScene( scene );

  // Make the viewer window visible on screen.
  viewer.show();
  // Run main loop.
  application.exec();
  return 0;
}
