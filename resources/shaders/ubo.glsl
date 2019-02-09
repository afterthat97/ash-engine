layout (std140) uniform CameraInfo { // uniform size: 144
    //                    // base align  // aligned offset
    mat4 projMat;         // 64          // 0
    mat4 viewMat;         // 64          // 64
    vec4 viewPos;         // 16          // 128
};

layout (std140) uniform ModelInfo { // uniform size: 144
    //                    // base align  // aligned offset
    mat4 modelMat;        // 64          // 0
    mat4 normalMat;       // 64          // 64
    int sizeFixed;        // 4           // 128
    int selected;         // 4           // 132
    int highlighted;      // 4           // 136
    uint pickingID;       // 4           // 140
};

layout (std140) uniform MaterialInfo { // uniform size: 48
    PhongMaterial material;
};

layout (std140) uniform LightInfo { // uniform size: 1424
    //                                    // base align  // aligned offset
    int ambientLightNum;                  // 4           // 0
    int directionalLightNum;              // 4           // 4
    int pointLightNum;                    // 4           // 8
    int spotLightNum;                     // 4           // 12
    AmbientLight ambientLight[8];         // 16          // 16
    DirectionalLight directionalLight[8]; // 32          // 144
    PointLight pointLight[8];             // 48          // 400
    SpotLight spotLight[8];               // 80          // 784
};
