#include <jni.h>
#include <android/log.h>
#include <GLES3/gl3.h>
#include "VertexData.h"
#include "CubeGenerator.h"
#include "Camera.h"

#define LOG_TAG "OpenGLRenderer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static VertexBuffer gVertexBuffer;
static Camera gCamera;
static GLuint gShaderProgram = 0;
static GLuint gVBO = 0;
static GLuint gVAO = 0;
static float gLastX = 0;
static float gLastY = 0;
static int gTouchState = 0;
static float gLastPinch = 0;

static const char* gVertexShaderSource = 
    "#version 300 es\n"
    "layout(location=0) in vec3 aPosition;"
    "layout(location=1) in vec4 aColor;"
    "uniform mat4 uMVPMatrix;"
    "out vec4 vColor;"
    "void main() {"
    "    gl_Position = uMVPMatrix * vec4(aPosition, 1.0);"
    "    vColor = aColor;"
    "}";

static const char* gFragmentShaderSource = 
    "#version 300 es\n"
    "precision mediump float;"
    "in vec4 vColor;"
    "out vec4 fragColor;"
    "void main() {"
    "    fragColor = vColor;"
    "}";

GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) { 
        char infoLog[512]; 
        glGetShaderInfoLog(shader, 512, nullptr, infoLog); 
        LOGE("Shader compile: %s", infoLog); 
    }
    return shader;
}

GLuint createShaderProgram() {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, gVertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, gFragmentShaderSource);
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) { 
        char infoLog[512]; 
        glGetProgramInfoLog(program, 512, nullptr, infoLog); 
        LOGE("Program link: %s", infoLog); 
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return program;
}

extern "C" {

JNIEXPORT void JNICALL
Java_com_ultimate3dmodeling_GLRenderer_nativeSurfaceCreated(JNIEnv *env, jobject thiz) {
    glClearColor(0.1f, 0.15f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    
    gShaderProgram = createShaderProgram();
    
    gVertexBuffer.clear();
    CubeGenerator::generateCube(gVertexBuffer, 0.0f, 0.0f, 0.0f, 0.5f);
    
    glGenVertexArrays(1, &gVAO);
    glBindVertexArray(gVAO);
    
    glGenBuffers(1, &gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glBufferData(GL_ARRAY_BUFFER, gVertexBuffer.getDataSize(), gVertexBuffer.getData(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

JNIEXPORT void JNICALL
Java_com_ultimate3dmodeling_GLRenderer_nativeSurfaceChanged(JNIEnv *env, jobject thiz, jint width, jint height) {
    glViewport(0, 0, width, height);
    gCamera.setAspectRatio((float)width/(float)height);
}

JNIEXPORT void JNICALL
Java_com_ultimate3dmodeling_GLRenderer_nativeDrawFrame(JNIEnv *env, jobject thiz) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(gShaderProgram);
    
    GLint mvpLoc = glGetUniformLocation(gShaderProgram, "uMVPMatrix");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, gCamera.getVPMatrix());
    
    if(gVertexBuffer.isDirty()) {
        glBindBuffer(GL_ARRAY_BUFFER, gVBO);
        glBufferData(GL_ARRAY_BUFFER, gVertexBuffer.getDataSize(), gVertexBuffer.getData(), GL_STATIC_DRAW);
        gVertexBuffer.markClean();
    }
    
    glBindVertexArray(gVAO);
    glDrawArrays(GL_TRIANGLES, 0, gVertexBuffer.getVertexCount());
    glBindVertexArray(0);
}

JNIEXPORT void JNICALL
Java_com_ultimate3dmodeling_GLRenderer_onTouchStart(JNIEnv *env, jobject thiz, jfloat x, jfloat y, jint mode) {
    gLastX = x;
    gLastY = y;
    gTouchState = mode;
}

JNIEXPORT void JNICALL
Java_com_ultimate3dmodeling_GLRenderer_onTouchMove(JNIEnv *env, jobject thiz, jfloat x, jfloat y) {
    float dx = x - gLastX;
    float dy = y - gLastY;
    
    if(gTouchState == 1) {
        gCamera.orbit(dx, dy);
    } else if(gTouchState == 2) {
        gCamera.pan(dx, dy);
    }
    
    gLastX = x;
    gLastY = y;
}

JNIEXPORT void JNICALL
Java_com_ultimate3dmodeling_GLRenderer_onTouchEnd(JNIEnv *env, jobject thiz) {
    gTouchState = 0;
}

JNIEXPORT void JNICALL
Java_com_ultimate3dmodeling_GLRenderer_onPinch(JNIEnv *env, jobject thiz, jfloat distance) {
    if(gLastPinch > 0) {
        float delta = distance - gLastPinch;
        gCamera.dolly(delta * 0.01f);
    }
    gLastPinch = distance;
}

JNIEXPORT void JNICALL
Java_com_ultimate3dmodeling_GLRenderer_addCube(JNIEnv *env, jobject thiz, jfloat x, jfloat y, jfloat z, jfloat size) {
    CubeGenerator::generateCube(gVertexBuffer, x, y, z, size);
}

JNIEXPORT void JNICALL
Java_com_ultimate3dmodeling_GLRenderer_clearAll(JNIEnv *env, jobject thiz) {
    gVertexBuffer.clear();
}

}