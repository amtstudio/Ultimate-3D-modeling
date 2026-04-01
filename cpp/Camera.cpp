#include "Camera.h"
#include <cmath>

Camera::Camera() {
    eyeX = 0.0f; eyeY = 0.0f; eyeZ = 3.0f;
    centerX = 0.0f; centerY = 0.0f; centerZ = 0.0f;
    upX = 0.0f; upY = 1.0f; upZ = 0.0f;
    
    fov = 45.0f;
    aspect = 1.0f;
    nearPlane = 0.1f;
    farPlane = 100.0f;
    
    updateViewMatrix();
    updateProjMatrix();
    updateVPMatrix();
}

void Camera::setLookAt(float ex, float ey, float ez,
                      float cx, float cy, float cz,
                      float ux, float uy, float uz) {
    eyeX = ex; eyeY = ey; eyeZ = ez;
    centerX = cx; centerY = cy; centerZ = cz;
    upX = ux; upY = uy; upZ = uz;
    updateViewMatrix();
    updateVPMatrix();
}

void Camera::setPerspective(float f, float a, float n, float fa) {
    fov = f;
    aspect = a;
    nearPlane = n;
    farPlane = fa;
    updateProjMatrix();
    updateVPMatrix();
}

void Camera::orbit(float deltaX, float deltaY) {
    float dx = eyeX - centerX;
    float dy = eyeY - centerY;
    float dz = eyeZ - centerZ;
    
    float radius = sqrtf(dx*dx + dy*dy + dz*dz);
    float theta = atan2f(dx, dz);
    float phi = asinf(dy / radius);
    
    theta += deltaX * 0.01f;
    phi += deltaY * 0.01f;
    
    const float phiLimit = 1.4835f;
    if (phi > phiLimit) phi = phiLimit;
    if (phi < -phiLimit) phi = -phiLimit;
    
    eyeX = centerX + radius * sinf(theta) * cosf(phi);
    eyeY = centerY + radius * sinf(phi);
    eyeZ = centerZ + radius * cosf(theta) * cosf(phi);
    
    float upTheta = theta + 3.14159f * 0.5f;
    upX = sinf(upTheta) * cosf(phi);
    upY = sinf(phi);
    upZ = cosf(upTheta) * cosf(phi);
    
    updateViewMatrix();
    updateVPMatrix();
}

void Camera::pan(float deltaX, float deltaY) {
    float forward[3] = {centerX - eyeX, centerY - eyeY, centerZ - eyeZ};
    normalize(forward[0], forward[1], forward[2]);
    
    float right[3];
    crossProduct(forward[0], forward[1], forward[2],
                upX, upY, upZ,
                right[0], right[1], right[2]);
    normalize(right[0], right[1], right[2]);
    
    float realUp[3];
    crossProduct(right[0], right[1], right[2],
                forward[0], forward[1], forward[2],
                realUp[0], realUp[1], realUp[2]);
    normalize(realUp[0], realUp[1], realUp[2]);
    
    float panX = deltaX * 0.01f;
    float panY = deltaY * 0.01f;
    
    eyeX += right[0] * panX + realUp[0] * panY;
    eyeY += right[1] * panX + realUp[1] * panY;
    eyeZ += right[2] * panX + realUp[2] * panY;
    
    centerX += right[0] * panX + realUp[0] * panY;
    centerY += right[1] * panX + realUp[1] * panY;
    centerZ += right[2] * panX + realUp[2] * panY;
    
    updateViewMatrix();
    updateVPMatrix();
}

void Camera::dolly(float delta) {
    float dx = centerX - eyeX;
    float dy = centerY - eyeY;
    float dz = centerZ - eyeZ;
    
    float length = sqrtf(dx*dx + dy*dy + dz*dz);
    if (length > 0) {
        dx /= length; dy /= length; dz /= length;
    }
    
    float move = delta * 0.5f;
    eyeX += dx * move;
    eyeY += dy * move;
    eyeZ += dz * move;
    
    centerX += dx * move;
    centerY += dy * move;
    centerZ += dz * move;
    
    updateViewMatrix();
    updateVPMatrix();
}

void Camera::zoom(float delta) {
    fov -= delta * 5.0f;
    if (fov < 1.0f) fov = 1.0f;
    if (fov > 120.0f) fov = 120.0f;
    updateProjMatrix();
    updateVPMatrix();
}

void Camera::updateViewMatrix() {
    float forward[3] = {centerX - eyeX, centerY - eyeY, centerZ - eyeZ};
    normalize(forward[0], forward[1], forward[2]);
    
    float right[3];
    crossProduct(forward[0], forward[1], forward[2],
                upX, upY, upZ,
                right[0], right[1], right[2]);
    normalize(right[0], right[1], right[2]);
    
    float realUp[3];
    crossProduct(right[0], right[1], right[2],
                forward[0], forward[1], forward[2],
                realUp[0], realUp[1], realUp[2]);
    normalize(realUp[0], realUp[1], realUp[2]);
    
    viewMatrix[0] = right[0];
    viewMatrix[4] = right[1];
    viewMatrix[8] = right[2];
    viewMatrix[12] = -dotProduct(right[0], right[1], right[2], eyeX, eyeY, eyeZ);
    
    viewMatrix[1] = realUp[0];
    viewMatrix[5] = realUp[1];
    viewMatrix[9] = realUp[2];
    viewMatrix[13] = -dotProduct(realUp[0], realUp[1], realUp[2], eyeX, eyeY, eyeZ);
    
    viewMatrix[2] = -forward[0];
    viewMatrix[6] = -forward[1];
    viewMatrix[10] = -forward[2];
    viewMatrix[14] = dotProduct(forward[0], forward[1], forward[2], eyeX, eyeY, eyeZ);
    
    viewMatrix[3] = 0.0f;
    viewMatrix[7] = 0.0f;
    viewMatrix[11] = 0.0f;
    viewMatrix[15] = 1.0f;
}

void Camera::updateProjMatrix() {
    float f = 1.0f / tanf(fov * 3.14159f / 360.0f);
    float range = nearPlane - farPlane;
    
    projMatrix[0] = f / aspect;
    projMatrix[4] = 0.0f;
    projMatrix[8] = 0.0f;
    projMatrix[12] = 0.0f;
    
    projMatrix[1] = 0.0f;
    projMatrix[5] = f;
    projMatrix[9] = 0.0f;
    projMatrix[13] = 0.0f;
    
    projMatrix[2] = 0.0f;
    projMatrix[6] = 0.0f;
    projMatrix[10] = (farPlane + nearPlane) / range;
    projMatrix[14] = 2.0f * farPlane * nearPlane / range;
    
    projMatrix[3] = 0.0f;
    projMatrix[7] = 0.0f;
    projMatrix[11] = -1.0f;
    projMatrix[15] = 0.0f;
}

void Camera::updateVPMatrix() {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            vpMatrix[i*4 + j] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                vpMatrix[i*4 + j] += projMatrix[i*4 + k] * viewMatrix[k*4 + j];
            }
        }
    }
}

const float* Camera::getViewMatrix() const {
    return viewMatrix;
}

const float* Camera::getProjectionMatrix() const {
    return projMatrix;
}

const float* Camera::getVPMatrix() const {
    return vpMatrix;
}

void Camera::setAspectRatio(float a) {
    aspect = a;
    updateProjMatrix();
    updateVPMatrix();
}

void Camera::getPosition(float& x, float& y, float& z) const {
    x = eyeX; y = eyeY; z = eyeZ;
}

void Camera::getTarget(float& x, float& y, float& z) const {
    x = centerX; y = centerY; z = centerZ;
}

float Camera::getDistance() const {
    float dx = centerX - eyeX;
    float dy = centerY - eyeY;
    float dz = centerZ - eyeZ;
    return sqrtf(dx*dx + dy*dy + dz*dz);
}

void Camera::normalize(float& x, float& y, float& z) {
    float length = sqrtf(x*x + y*y + z*z);
    if (length > 0) {
        x /= length; y /= length; z /= length;
    }
}

void Camera::crossProduct(float ax, float ay, float az,
                         float bx, float by, float bz,
                         float& rx, float& ry, float& rz) {
    rx = ay * bz - az * by;
    ry = az * bx - ax * bz;
    rz = ax * by - ay * bx;
}

float Camera::dotProduct(float ax, float ay, float az,
                        float bx, float by, float bz) {
    return ax * bx + ay * by + az * bz;
}