#ifndef MATERIAL_H
#define MATERIAL_H

#include "utilities.h"
#include "shader.h"
#include "texture.h"

class Material {
private:
    vector<shared_ptr<Texture>> textures;
public:
    string name;
    uint32_t illumModel;
    // specifies the illumination model to use in the material, range from 1 to 10:
    // Illumination model	Properties that are turned on in the Property Editor
    // 0	Color on and Ambient off
    // 1	Color on and Ambient on
    // 2	Highlight on
    // 3	Reflection on and Ray trace on
    // 4	Transparency: Glass on
    //		Reflection: Ray trace on
    // 5	Reflection: Fresnel on and Ray trace on
    // 6	Transparency: Refraction on
    //		Reflection: Fresnel off and Ray trace on
    // 7	Transparency: Refraction on
    //		Reflection: Fresnel on and Ray trace on
    // 8	Reflection on and Ray trace off
    // 9	Transparency: Glass on
    //		Reflection: Ray trace off
    // 10	Casts shadows onto invisible surfaces
    float shininess;
    // specifies the specular exponent for the current material, range from 0 to 1000. A high exponent results in a tight, concentrated highlight.
    float sharpness;
    // specifies the sharpness of the reflections from the local reflection map, range from 0 to 1000. The default is 60.  A high value results in a clear reflection of objects in the reflection map.
    float density;
    // specifies the optical density for the surface, range from 0.001 to 10. A value of 1.0 means that light does not bend as it passes through an object. Increasing the optical_density increases the amount of bending.
    vec3 ambient;
    //specifies the ambient reflectivity using RGB values
    vec3 diffuse;
    //specifies the diffuse reflectivity using RGB values
    vec3 specular;
    //specifies the specular reflectivity using RGB values
    Material() {
        name = "Untitled Material";
        loadAllDefault();
    }
    ~Material() {
        reportInfo("Material " + name + " has been deleted successfully.");
    }
    void addTexture(shared_ptr<Texture>);
    void loadAllDefault();
    void loadDefaultName();
    void loadDefaultShininess();
    void loadDefaultSharpness();
    void loadDefaultDensity();
    void loadDefaultAmbientRGB();
    void loadDefaultDiffuseRGB();
    void loadDefaultSpecularRGB();
    void bind(Shader&);
    void dumpinfo(string);
};

#endif
