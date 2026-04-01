#include "CubeGenerator.h"
#include <cmath>

void CubeGenerator::generateCube(VertexBuffer& buffer, 
                                float centerX, float centerY, float centerZ,
                                float size, float r, float g, float b, float a) {
    float half = size * 0.5f;
    
    addTriangle(buffer,
                centerX - half, centerY - half, centerZ + half,
                centerX + half, centerY - half, centerZ + half,
                centerX - half, centerY + half, centerZ + half,
                r, g, b, a);
    addTriangle(buffer,
                centerX + half, centerY - half, centerZ + half,
                centerX + half, centerY + half, centerZ + half,
                centerX - half, centerY + half, centerZ + half,
                r, g, b, a);
    
    addTriangle(buffer,
                centerX - half, centerY - half, centerZ - half,
                centerX - half, centerY + half, centerZ - half,
                centerX + half, centerY - half, centerZ - half,
                r, g, b, a);
    addTriangle(buffer,
                centerX + half, centerY - half, centerZ - half,
                centerX - half, centerY + half, centerZ - half,
                centerX + half, centerY + half, centerZ - half,
                r, g, b, a);
    
    addTriangle(buffer,
                centerX - half, centerY - half, centerZ - half,
                centerX - half, centerY - half, centerZ + half,
                centerX - half, centerY + half, centerZ - half,
                r * 0.8f, g * 0.8f, b * 0.8f, a);
    addTriangle(buffer,
                centerX - half, centerY - half, centerZ + half,
                centerX - half, centerY + half, centerZ + half,
                centerX - half, centerY + half, centerZ - half,
                r * 0.8f, g * 0.8f, b * 0.8f, a);
    
    addTriangle(buffer,
                centerX + half, centerY - half, centerZ - half,
                centerX + half, centerY + half, centerZ - half,
                centerX + half, centerY - half, centerZ + half,
                r * 0.8f, g * 0.8f, b * 0.8f, a);
    addTriangle(buffer,
                centerX + half, centerY - half, centerZ + half,
                centerX + half, centerY + half, centerZ - half,
                centerX + half, centerY + half, centerZ + half,
                r * 0.8f, g * 0.8f, b * 0.8f, a);
    
    addTriangle(buffer,
                centerX - half, centerY + half, centerZ - half,
                centerX - half, centerY + half, centerZ + half,
                centerX + half, centerY + half, centerZ - half,
                r * 0.9f, g * 0.9f, b * 0.9f, a);
    addTriangle(buffer,
                centerX + half, centerY + half, centerZ - half,
                centerX - half, centerY + half, centerZ + half,
                centerX + half, centerY + half, centerZ + half,
                r * 0.9f, g * 0.9f, b * 0.9f, a);
    
    addTriangle(buffer,
                centerX - half, centerY - half, centerZ - half,
                centerX + half, centerY - half, centerZ - half,
                centerX - half, centerY - half, centerZ + half,
                r * 0.6f, g * 0.6f, b * 0.6f, a);
    addTriangle(buffer,
                centerX + half, centerY - half, centerZ - half,
                centerX + half, centerY - half, centerZ + half,
                centerX - half, centerY - half, centerZ + half,
                r * 0.6f, g * 0.6f, b * 0.6f, a);
}

void CubeGenerator::addTriangle(VertexBuffer& buffer,
                               float x1, float y1, float z1,
                               float x2, float y2, float z2,
                               float x3, float y3, float z3,
                               float r, float g, float b, float a) {
    buffer.addVertex(x1, y1, z1, r, g, b, a);
    buffer.addVertex(x2, y2, z2, r, g, b, a);
    buffer.addVertex(x3, y3, z3, r, g, b, a);
}

void CubeGenerator::calculateNormal(float x1, float y1, float z1,
                                   float x2, float y2, float z2,
                                   float x3, float y3, float z3,
                                   float& nx, float& ny, float& nz) {
    float ux = x2 - x1, uy = y2 - y1, uz = z2 - z1;
    float vx = x3 - x1, vy = y3 - y1, vz = z3 - z1;
    
    nx = uy * vz - uz * vy;
    ny = uz * vx - ux * vz;
    nz = ux * vy - uy * vx;
    
    float length = sqrtf(nx * nx + ny * ny + nz * nz);
    if (length > 0) {
        nx /= length;
        ny /= length;
        nz /= length;
    }
}