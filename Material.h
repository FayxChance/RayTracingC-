/**
@file Material.h
@author JOL
*/
#pragma once
#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Color.h"

// @see http://devernay.free.fr/cours/opengl/materials.html

/// Namespace RayTracer
namespace rt {

    /// This structure stores the material associated to a graphical object.
    /// object should have.
    struct Material {
        /// ambient color
        Color ambient;
        /// diffuse color
        Color diffuse;
        /// specular color
        Color specular;
        /// shinyness (50: very high, 1: low )
        Real shinyness;

        // --------- The following data is used in later questions. --------------

        /// The material is perfectly diffuse if coef_diffusion == 1.0,
        /// otherwise it may have reflexion / transparency properties.
        Real coef_diffusion;
        /// The material has some specularity if coef_reflexion > 0.0
        Real coef_reflexion;
        /// The material is transparent if coef_refraction > 0.0
        Real coef_refraction;
        /// Refractive index (1.0f in vacuum, 1.31 in ice, 1.52 window glass, 1.33 water
        /// Inside refractive index (for transparent medium) (>= 1.0f)
        Real in_refractive_index;
        /// Outside refractive index (1.0f if object is in the air otherwise >= 1.0f)
        Real out_refractive_index;

        /// Mixes two material (t=0 gives m1, t=1 gives m2, t=0.5 gives their average)
        static Material mix(Real t, const Material &m1, const Material &m2) {
            Material m;
            Real s = 1.0f - t;
            m.ambient = s * m1.ambient + t * m2.ambient;
            m.diffuse = s * m1.diffuse + t * m2.diffuse;
            m.specular = s * m1.specular + t * m2.specular;
            m.shinyness = s * m1.shinyness + t * m2.shinyness;
            m.coef_diffusion = s * m1.coef_diffusion + t * m2.coef_diffusion;
            m.coef_reflexion = s * m1.coef_reflexion + t * m2.coef_reflexion;
            m.coef_refraction = s * m1.coef_refraction + t * m2.coef_refraction;
            m.in_refractive_index = s * m1.in_refractive_index + t * m2.in_refractive_index;
            m.out_refractive_index = s * m1.out_refractive_index + t * m2.out_refractive_index;
            return m;
        }

        /// Default constructor
        Material() {}

        /// Constructor from colors and shinyness.
        Material(Color amb, Color diff, Color spec, Real shiny = 0.0f,
                 Real cdiff = 1.0f, Real crefl = 0.0f, Real crefr = 0.0f,
                 Real in_ridx = 1.0f, Real out_ridx = 1.0f)
                : ambient(amb), diffuse(diff), specular(spec), shinyness(shiny),
                  coef_diffusion(cdiff), coef_reflexion(crefl), coef_refraction(crefr),
                  in_refractive_index(in_ridx), out_refractive_index(out_ridx) {}

        static Material whitePlastic() {
            Material m;
            m.ambient = Color(0.1, 0.1, 0.1);
            m.diffuse = Color(0.7, 0.7, 0.7);
            m.specular = Color(1.0, 1.0, 0.98);
            m.shinyness = 5.0;
            m.coef_diffusion = 0.9f;
            m.coef_reflexion = 0.0f;
            m.coef_refraction = 0.0f;
            m.in_refractive_index = 1.0f;
            m.out_refractive_index = 1.0f;
            return m;
        }

        static Material redPlastic() {
            Material m;
            m.ambient = Color(0.1, 0.0, 0.0);
            m.diffuse = Color(0.85, 0.05, 0.05);
            m.specular = Color(1.0, 0.8, 0.8);
            m.shinyness = 5.0;
            m.coef_diffusion = 1.0f;
            m.coef_reflexion = 0.05f;
            m.coef_refraction = 0.0f;
            m.in_refractive_index = 1.0f;
            m.out_refractive_index = 1.0f;
            return m;
        }

        static Material bronze() {
            Material m;
            m.ambient = Color(0.1125, 0.0675, 0.054);
            m.diffuse = Color(0.714, 0.4284, 0.18144);
            m.specular = Color(0.9, 0.8, 0.7);
            // m.specular  = Color( 0.393548, 0.271906, 0.166721 );
            m.shinyness = 56; // 25.6;
            m.coef_diffusion = 0.5f;
            m.coef_reflexion = 0.75f;
            m.coef_refraction = 0.0f;
            m.in_refractive_index = 1.0f;
            m.out_refractive_index = 1.0f;
            return m;
        }


        static Material emerald() {
            Material m;
            m.ambient = Color(0.0215f, 0.1745f, 0.0215f); //Color( 0.0215, 0.1745, 0.0215 );
            m.diffuse = Color(0.07568f, 0.61424f, 0.07568f);
            m.specular = Color(0.633, 0.727811, 0.633); // Color( 0.633, 0.727811, 0.633 );
            m.shinyness = 0.6 * 128.0;
            m.coef_diffusion = 0.15f;
            m.coef_reflexion = 0.5f;
            m.coef_refraction = .70f;
            m.in_refractive_index = 1.5f;
            m.out_refractive_index = 1.0f;
            return m;
        }

        static Material jade() {
            Material m;
            m.ambient = Color(0.135, 0.2225f, 0.1575);
            m.diffuse = Color(0.54, 0.89, 0.63);
            m.specular = Color(0.316228, 0.316228, 0.316228);
            m.shinyness = 0.1 * 128.0;
            m.coef_diffusion = 0.7f;
            m.coef_reflexion = 0.2f;
            m.coef_refraction = 0.0f;
            m.in_refractive_index = 1.5f;
            m.out_refractive_index = 1.0f;
            return m;
        }

        static Material turquoise() {
            Material m;
            m.ambient = Color(0.1, 0.18725, 0.1745);
            m.diffuse = Color(0.396, 0.74151, 0.69102);
            m.specular = Color(0.297254, 0.30829, 0.306678);
            m.shinyness = 0.1 * 128.0;
            m.coef_diffusion = 0.7f;
            m.coef_reflexion = 0.3f;
            m.coef_refraction = 0.0f;
            m.in_refractive_index = 1.5f;
            m.out_refractive_index = 1.0f;
            return m;
        }

        static Material copper() {
            Material m;
            m.ambient = Color(0.19125, 0.0735, 0.0225);
            m.diffuse = Color(0.7038, 0.27048, 0.0828);
            m.specular = Color(0.256777, 0.137622, 0.086014);
            m.shinyness = 0.1 * 128.0;
            m.coef_diffusion = 0.5f;
            m.coef_reflexion = 0.75f;
            m.coef_refraction = 0.0f;
            m.in_refractive_index = 1.5f;
            m.out_refractive_index = 1.0f;
            return m;
        }

        static Material gold() {
            Material m;
            m.ambient = Color(0.24725, 0.1995, 0.0745);
            m.diffuse = Color(0.75164, 0.60648, 0.22648);
            m.specular = Color(0.628281, 0.555802, 0.366065);
            m.shinyness = 0.4 * 128.0;
            m.coef_diffusion = 0.5f;
            m.coef_reflexion = 0.75f;
            m.coef_refraction = 0.0f;
            m.in_refractive_index = 1.5f;
            m.out_refractive_index = 1.0f;
            return m;
        }

        static Material silver() {
            Material m;
            m.ambient = Color(0.19225, 0.19225, 0.19225);
            m.diffuse = Color(0.50754, 0.50754, 0.50754);
            m.specular = Color(0.508273, 0.508273, 0.508273);
            m.shinyness = 0.4 * 128.0;
            m.coef_diffusion = 0.5f;
            m.coef_reflexion = 0.75f;
            m.coef_refraction = 0.0f;
            m.in_refractive_index = 1.5f;
            m.out_refractive_index = 1.0f;
            return m;
        }

        static Material chrome() {
            Material m;
            m.ambient = Color(0.25, 0.25, 0.25);
            m.diffuse = Color(0.4, 0.4, 0.4);
            m.specular = Color(0.774597, 0.774597, 0.774597);
            m.shinyness = 0.6 * 128.0;
            m.coef_diffusion = 0.5f;
            m.coef_reflexion = 0.75f;
            m.coef_refraction = 0.0f;
            m.in_refractive_index = 1.5f;
            m.out_refractive_index = 1.0f;
            return m;
        }

//        static Material() {
//            Material m;
//            m.ambient = Color(, ,);
//            m.diffuse = Color(, ,);
//            m.specular = Color(, ,);
//            m.shinyness = 0. * 128.0;
//            m.coef_diffusion = 0.15f;
//            m.coef_reflexion = 0.5f;
//            m.coef_refraction = 1.0f;
//            m.in_refractive_index = 1.5f;
//            m.out_refractive_index = 1.0f;
//            return m;
//        }

        static Material ruby() {
            Material m;
            m.ambient = Color(0.1745, 0.01175, 0.01175);
            m.diffuse = Color(0.61424, 0.04136, 0.04136);
            m.specular = Color(0.727811, 0.626959, 0.626959);
            m.shinyness = 0.6 * 128.0;
            m.coef_diffusion = 0.15f;
            m.coef_reflexion = 0.5f;
            m.coef_refraction = 0.70f;
            m.in_refractive_index = 1.5f;
            m.out_refractive_index = 1.0f;
            return m;
        }

        static Material pearl() {
            Material m;
            m.ambient = Color(0.25, 0.20725, 0.20725);
            m.diffuse = Color(1, 0.829, 0.829);
            m.specular = Color(0.296648, 0.296648, 0.296648);
            m.shinyness = 0.088 * 128.0;
            m.coef_diffusion = 0.75f;
            m.coef_reflexion = 0.2f;
            m.coef_refraction = 0.0f;
            m.in_refractive_index = 1.5f;
            m.out_refractive_index = 1.0f;
            return m;
        }

        static Material obsidian() {
            Material m;
            m.ambient = Color(0.05375, 0.05, 0.06625);
            m.diffuse = Color(0.18275, 0.17, 0.22525);
            m.specular = Color(0.332741, 0.328634, 0.346435);
            m.shinyness = 0.3 * 128.0;
            m.coef_diffusion = 0.8f;
            m.coef_reflexion = 0.1f;
            m.coef_refraction = 0.0f;
            m.in_refractive_index = 1.5f;
            m.out_refractive_index = 1.0f;
            return m;
        }


        static Material glass() {
            Material m;
            m.ambient = Color(0.0, 0.0, 0.0);
            m.diffuse = Color(0.95, 0.95, 1.0);
            m.specular = Color(1.0, 1.0, 1.0);
            m.shinyness = 80.0f;
            m.coef_diffusion = 0.01f;
            m.coef_reflexion = 0.00f;
            m.coef_refraction = 0.98f;
            m.in_refractive_index = 1.5f;
            m.out_refractive_index = 1.0f;
            return m;
        }

        static Material mirror() {
            Material m;
            m.ambient = Color(0.0, 0.0, 0.0);
            m.diffuse = Color(0.95, 0.95, 1.0);
            m.specular = Color(1.0, 1.0, 1.0);
            m.shinyness = 80.0f;
            m.coef_diffusion = 0.00f;
            m.coef_reflexion = 0.00f;
            m.coef_refraction = 0.99f;
            m.in_refractive_index = 1.5f;
            m.out_refractive_index = 1.0f;
            return m;
        }

        static Material blackMatter() {
            Material m;
            m.ambient = Color(0.2, 0.2, 0.2);
            m.diffuse = Color(0.01, 0.01, 0.01);
            m.specular = Color(0.4, 0.4, 0.4);
            m.shinyness = .078125;
            m.coef_diffusion = 0.9f;
            m.coef_reflexion = 0.1f;
            m.coef_refraction = 0.0f;
            m.in_refractive_index = 1.0f;
            m.out_refractive_index = 1.0f;
            return m;
        }
    };


} // namespace rt

#endif // #define _MATERIAL_H_
