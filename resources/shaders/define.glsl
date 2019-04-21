struct PhongMaterial { // struct size: 48
    //                    // base align  // aligned offset
    vec4 color;           // 16          // 0
    float ambient;        // 4           // 16
    float diffuse;        // 4           // 20
    float specular;       // 4           // 24
    float shininess;      // 4           // 28
    int useDiffuseMap;    // 4           // 32
    int useSpecularMap;   // 4           // 36
    int useBumpMap;       // 4           // 40
    int padding;          // 4           // 44
};

struct AmbientLight { // struct size: 16
    //                    // base align  // aligned offset
    vec4 color;           // 16          // 0
};

struct DirectionalLight { // struct size: 32
    //                    // base align  // aligned offset
    vec4 color;           // 16          // 0
    vec4 direction;       // 16          // 16
};

struct PointLight { // struct size: 48
    //                           // base align  // aligned offset
    vec4 color;                  // 16          // 0
    vec4 pos;                    // 16          // 16
    vec4 attenuation;            // 16          // 32
};

struct SpotLight { // struct size: 80
    //                           // base align  // aligned offset
    vec4 color;                  // 16          // 0
    vec4 pos;                    // 16          // 16
    vec4 direction;              // 16          // 32
    vec4 attenuation;            // 16          // 48
    vec4 cutOff;                 // 16          // 64
};
