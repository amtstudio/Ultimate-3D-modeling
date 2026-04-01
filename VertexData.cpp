#include "VertexData.h"

VertexBuffer::VertexBuffer() : dirty(false) {}

void VertexBuffer::addVertex(float x, float y, float z, float r, float g, float b, float a) {
    Vertex v;
    v.x = x; v.y = y; v.z = z;
    v.r = r; v.g = g; v.b = b; v.a = a;
    v.nx = 0; v.ny = 0; v.nz = 1;
    
    vertices.push_back(v);
    dirty = true;
}

void VertexBuffer::clear() {
    vertices.clear();
    dirty = true;
}

const float* VertexBuffer::getData() const {
    return reinterpret_cast<const float*>(vertices.data());
}

int VertexBuffer::getVertexCount() const {
    return static_cast<int>(vertices.size());
}

int VertexBuffer::getDataSize() const {
    return getVertexCount() * sizeof(Vertex);
}

bool VertexBuffer::isDirty() const {
    return dirty;
}

void VertexBuffer::markClean() {
    dirty = false;
}