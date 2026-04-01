package com.ultimate3dmodeling;

import android.opengl.GLSurfaceView;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class GLRenderer implements GLSurfaceView.Renderer {
    
    static {
        System.loadLibrary("native-lib");
    }
    
    private native void nativeSurfaceCreated();
    private native void nativeSurfaceChanged(int width, int height);
    private native void nativeDrawFrame();
    
    public native void onTouchStart(float x, float y, int mode);
    public native void onTouchMove(float x, float y);
    public native void onTouchEnd();
    public native void onPinch(float distance);
    
    public native void addCube(float x, float y, float z, float size);
    public native void clearAll();
    
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        nativeSurfaceCreated();
    }
    
    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        nativeSurfaceChanged(width, height);
    }
    
    @Override
    public void onDrawFrame(GL10 gl) {
        nativeDrawFrame();
    }
}