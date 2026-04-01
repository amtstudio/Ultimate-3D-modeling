#ifndef CUBE_GENERATOR_H
#define CUBE_GENERATOR_H

#include "VertexData.h"

class CubeGenerator {
public:
    static void generateCube(VertexBuffer& buffer, 
                            float centerX = 0, float centerY = 0, float centerZ = 0,
                            float size = 1.0f,
                            float r = 0.8f, float g = 0.3f, float b = 0.2f, float a = 1.0f);
    
private:
    static void addTriangle(VertexBuffer& buffer,
                           float x1, float y1, float z1,
                           float x2, float y2, float z2,
                           float x3, float y3, float z3,
                           float r, float g, float b, float a);
    
    static void calculateNormal(float x1, float y1, float z1,
                               float x2, float y2, float z2,
                               float x3, float y3, float z3,
                               float& nx, float& ny, float& nz);
};

#endif