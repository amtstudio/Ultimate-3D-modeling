#ifndef VERTEX_DATA_H
#define VERTEX_DATA_H

#include <vector>

struct Vertex {
    float x, y, z;
    float r, g, b, a;
    float nx, ny, nz;
};

class VertexBuffer {
private:
    std::vector<Vertex> vertices;
    bool dirty;
    
public:
    VertexBuffer();
    
    void addVertex(float x, float y, float z, 
                   float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);
    
    void clear();
    
    const float* getData() const;
    int getVertexCount() const;
    int getDataSize() const;
    
    bool isDirty() const;
    void markClean();
};

#endif