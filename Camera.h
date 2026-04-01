#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>

class Camera {
private:
    float eyeX, eyeY, eyeZ;
    float centerX, centerY, centerZ;
    float upX, upY, upZ;
    
    float fov;
    float aspect;
    float nearPlane, farPlane;
    
    float viewMatrix[16];
    float projMatrix[16];
    float vpMatrix[16];
    
    void updateViewMatrix();
    void updateProjMatrix();
    void updateVPMatrix();
    
    void normalize(float& x, float& y, float& z);
    void crossProduct(float ax, float ay, float az,
                     float bx, float by, float bz,
                     float& rx, float& ry, float& rz);
    float dotProduct(float ax, float ay, float az,
                    float bx, float by, float bz);
    
public:
    Camera();
    
    void setLookAt(float eyeX, float eyeY, float eyeZ,
                  float centerX, float centerY, float centerZ,
                  float upX, float upY, float upZ);
    
    void setPerspective(float fov, float aspect, float near, float far);
    
    void orbit(float deltaX, float deltaY);
    void pan(float deltaX, float deltaY);
    void dolly(float delta);
    void zoom(float delta);
    
    const float* getViewMatrix() const;
    const float* getProjectionMatrix() const;
    const float* getVPMatrix() const;
    
    void setAspectRatio(float aspect);
    
    void getPosition(float& x, float& y, float& z) const;
    void getTarget(float& x, float& y, float& z) const;
    float getDistance() const;
};
#endif