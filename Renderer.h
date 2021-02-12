/**
@file Renderer.h
@author JOL
*/
#pragma once
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "Color.h"
#include "Image2D.h"
#include "Ray.h"
#include "Background.h"

/// Namespace RayTracer
namespace rt {

    inline void progressBar(std::ostream &output,
                            const double currentValue, const double maximumValue) {
        static const int PROGRESSBARWIDTH = 60;
        static int myProgressBarRotation = 0;
        static int myProgressBarCurrent = 0;
        // how wide you want the progress meter to be
        double fraction = currentValue / maximumValue;

        // part of the progressmeter that's already "full"
        int dotz = static_cast<int>(floor(fraction * PROGRESSBARWIDTH));
        if (dotz > PROGRESSBARWIDTH) dotz = PROGRESSBARWIDTH;

        // if the fullness hasn't changed skip display
        if (dotz == myProgressBarCurrent) return;
        myProgressBarCurrent = dotz;
        myProgressBarRotation++;

        // create the "meter"
        int ii = 0;
        output << "[";
        // part  that's full already
        for (; ii < dotz; ii++) output << "#";
        // remaining part (spaces)
        for (; ii < PROGRESSBARWIDTH; ii++) output << " ";
        static const char *rotation_string = "|\\-/";
        myProgressBarRotation %= 4;
        output << "] " << rotation_string[myProgressBarRotation]
               << " " << (int) (fraction * 100) << "/100\r";
        output.flush();
    }

    /// This structure takes care of rendering a scene.
    struct Renderer {

        /// The scene to render
        Scene *ptrScene;
        /// Background
        Background *ptrBackground;
        /// The origin of the camera in space.
        Point3 myOrigin;
        /// (myOrigin, myOrigin+myDirUL) forms a ray going through the upper-left
        /// corner pixel of the viewport, i.e. pixel (0,0)
        Vector3 myDirUL;
        /// (myOrigin, myOrigin+myDirUR) forms a ray going through the upper-right
        /// corner pixel of the viewport, i.e. pixel (width,0)
        Vector3 myDirUR;
        /// (myOrigin, myOrigin+myDirLL) forms a ray going through the lower-left
        /// corner pixel of the viewport, i.e. pixel (0,height)
        Vector3 myDirLL;
        /// (myOrigin, myOrigin+myDirLR) forms a ray going through the lower-right
        /// corner pixel of the viewport, i.e. pixel (width,height)
        Vector3 myDirLR;

        int myWidth;
        int myHeight;

        Renderer() : ptrScene(0) {}

        Renderer(Scene &scene) : ptrScene(&scene) {}

        Renderer(Scene &scene, Background *background) : ptrScene(&scene), ptrBackground(background) {}

        void setScene(rt::Scene &aScene) { ptrScene = &aScene; }

        void setViewBox(Point3 origin,
                        Vector3 dirUL, Vector3 dirUR, Vector3 dirLL, Vector3 dirLR) {
            myOrigin = origin;
            myDirUL = dirUL;
            myDirUR = dirUR;
            myDirLL = dirLL;
            myDirLR = dirLR;
        }

        void setResolution(int width, int height) {
            myWidth = width;
            myHeight = height;
        }


        void RandoRender(Image2D<Color> &image, int max_depth) {
            std::cout << "Rendering into image ... might take a while." << std::endl;
            image = Image2D<Color>(myWidth, myHeight);
            for (int y = 0; y < myHeight; ++y) {
                Real ty = (Real) y / (Real) (myHeight - 1);
                progressBar(std::cout, ty, 1.0);
                Vector3 dirL = (1.0f - ty) * myDirUL + ty * myDirLL;
                Vector3 dirR = (1.0f - ty) * myDirUR + ty * myDirLR;
                dirL /= dirL.norm();
                dirR /= dirR.norm();
                for (int x = 0; x < myWidth; ++x) {
                    Real tx = (Real) x / (Real) (myWidth - 1);
                    Vector3 dir = (1.0f - tx) * dirL + tx * dirR;
                    Ray eye_ray = Ray(myOrigin, dir, max_depth);
                    int random = 10 + rand()%11;
                    Color result(0,0,0);
                    Color moyRes(0,0,0);
                    bool flag = false;
                    for (int i = 0; i < random && ! flag; ++i) {
                        if(i >= 4){
                            Color temp = (moyRes * (1.0f / i) + (-1.0f * result));
                            if(temp.b() < 0.01f && temp.g() < 0.01f && temp.r() < 0.01f){
                                flag = !flag;
                            }
                        }
                        result = trace(eye_ray);
                        moyRes+=result;
                    }
                    moyRes = moyRes*(1/random);
                    image.at(x, y) = moyRes.clamp();
                }
            }
            std::cout << "Done." << std::endl;
        }

        /// The main rendering routine
        void render(Image2D<Color> &image, int max_depth) {
            std::cout << "Rendering into image ... might take a while." << std::endl;
            image = Image2D<Color>(myWidth, myHeight);
            for (int y = 0; y < myHeight; ++y) {
                Real ty = (Real) y / (Real) (myHeight - 1);
                progressBar(std::cout, ty, 1.0);
                Vector3 dirL = (1.0f - ty) * myDirUL + ty * myDirLL;
                Vector3 dirR = (1.0f - ty) * myDirUR + ty * myDirLR;
                dirL /= dirL.norm();
                dirR /= dirR.norm();
                for (int x = 0; x < myWidth; ++x) {
                    Real tx = (Real) x / (Real) (myWidth - 1);
                    Vector3 dir = (1.0f - tx) * dirL + tx * dirR;
                    Ray eye_ray = Ray(myOrigin, dir, max_depth);
                    Color result = trace(eye_ray);
                    image.at(x, y) = result.clamp();
                }
            }
            std::cout << "Done." << std::endl;
        }


        Color background( const Ray& ray )
        {
            Color result = Color( 0.0, 0.0, 0.0 );
            for ( Light* light : ptrScene->myLights )
            {
                Real cos_a = light->direction( ray.origin ).dot( ray.direction );
                if ( cos_a > 0.99f )
                {
                    Real a = acos( cos_a ) * 360.0 / M_PI / 8.0;
                    a = std::max( 1.0f - a, 0.0f );
                    result += light->color( ray.origin ) * a * a;
                }
            }
            if ( ptrBackground != 0 ) result += ptrBackground->backgroundColor( ray );
            return result;
        }

        /// The rendering routine for one ray.
        /// @return the color for the given ray.
        Color trace(const Ray &ray) {

            assert(ptrScene != 0);

            Color result = Color(0.0, 0.0, 0.0);
            GraphicalObject *obj_i = 0;
            Point3 p_i;

            Real ri = ptrScene->rayIntersection(ray, obj_i, p_i);


            if (ri >= 0.0f) return background(ray); // some background color

            Vector3 normal = obj_i->getNormal(p_i) / obj_i->getNormal(p_i).norm();
            Material m = obj_i->getMaterial(p_i);

            //Reflexion
            if (ray.depth > 0 && m.coef_reflexion != 0.0f) {
                Vector3 rayonSortant = reflect(ray.direction / (ray.direction.norm()), normal);
                Ray ray_refl(p_i + rayonSortant * 0.1f, rayonSortant, ray.depth - 1);
                Color c_refl = trace(ray_refl);
                result += c_refl * m.specular * m.coef_reflexion;
            }

            //Refraction
            if (ray.depth > 0 && m.coef_refraction != 0.0f) {
                Ray rayonSortant = refractionRay(ray, p_i, normal, m);
                Color c_refr = trace(rayonSortant);
                result += c_refr * m.diffuse * m.coef_refraction;
            }

            //result +=  illumination(ray, obj_i, p_i) ;

            result +=  ray.depth == 0 ? illumination(ray, obj_i, p_i) :
                     illumination(ray, obj_i, p_i) * m.coef_diffusion;

            return result;

        }

        /// Calcule l'illumination de l'objet \a obj au point \a p, sachant que l'observateur est le rayon \a ray.
        Color illumination(const Ray &ray, GraphicalObject *obj, Point3 p) {

            Material m = obj->getMaterial(p);
            Color c = m.ambient;
            Vector3 v = ray.direction;

            for (auto it = ptrScene->myLights.begin(),
                         itE = ptrScene->myLights.end(); it != itE; it++) {

                Light *l = *it;

                Vector3 L = (l->direction(p) / l->direction(p).norm());
                Vector3 normalPNormalise = obj->getNormal(p) / obj->getNormal(p).norm();

                Vector3 wNormalise = reflect(v, normalPNormalise);
                Real beta = wNormalise.dot(L);
                Real coeffSpecu = 0;

                if (beta > 0) {
                    coeffSpecu = pow(beta, m.shinyness);
                }

                Real produitScalaire = L.dot(normalPNormalise);
                Real k = produitScalaire < 0.0f ? 0.0f : produitScalaire;

                Color lightColor = (*l).color(p);
                Color shadowColor = shadow(Ray(p, L),lightColor);

                c = c + k * m.diffuse * (shadowColor) + coeffSpecu * m.specular * (shadowColor);
            }
            return c;
        }

        /// Les vecteurs \a w et \a n doivent être normalisés
        Vector3 reflect(const Vector3 &w, Vector3 n) {

            return (w + 2.0f * (n.dot(-1.0f * w)) * n) / (w + 2.0f * ( n.dot(-1.0f * w)) * n).norm();
        }

        Ray refractionRay(const Ray &aRay, const Point3 &p, Vector3 N, const Material &m) {

            Vector3 V = aRay.direction / aRay.direction.norm();
            Vector3 n = N/N.norm();
            Real c = - n.dot(V);

            Vector3 refract;
            Vector3 vRefract;
            Ray res;

            if (V.dot(n) < 0.0f) {
                Real r = m.out_refractive_index / m.in_refractive_index;
                refract = r * V + (r * c - sqrt(std::max(1.0f - r * r * (1.0f - c * c), 0.0f))) * n;
            } else {
                Real r = m.in_refractive_index / m.out_refractive_index;
                refract = r * V + (r * c + sqrt(std::max(1.0f - r * r * (1.0f - c * c), 0.0f))) * n;
            }

            vRefract = refract / refract.norm();
            res = Ray(p + vRefract * 0.0001f, vRefract, aRay.depth - 1);

            return res;
        }

        /// Calcule la couleur de la lumière (donnée par light_color) dans la
        /// direction donnée par le rayon. Si aucun objet n'est traversé,
        /// retourne light_color, sinon si un des objets traversés est opaque,
        /// retourne du noir, et enfin si les objets traversés sont
        /// transparents, attenue la couleur.
        Color shadow(const Ray &ray, Color light_color) {
            Point3 p = ray.origin;
            Vector3 L = ray.direction/ ray.direction.norm();

            Color C = light_color;
            while (C.max() > 0.003f) {
                p = p + L * 0.01f;
                Ray otherRay(p, L);
                GraphicalObject *object = nullptr;
                Point3 pPrime;
                if (ptrScene->rayIntersection(otherRay, object, pPrime) <= 0.0f) {
                    Material m = object->getMaterial(pPrime);
                    C = C * m.diffuse * m.coef_refraction;
                    p = pPrime;
                } else {
                    break;
                }
            }
            return C;
        }
    };

} // namespace rt

#endif // #define _RENDERER_H_
