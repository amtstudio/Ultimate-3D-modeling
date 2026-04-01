package com.ultimate3dmodeling;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.Window;
import android.view.WindowManager;

public class MainActivity extends Activity {
    private GLSurfaceView glSurfaceView;
    private GLRenderer renderer;
    private int touchMode = 1;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, 
                            WindowManager.LayoutParams.FLAG_FULLSCREEN);
        
        glSurfaceView = new GLSurfaceView(this);
        glSurfaceView.setEGLContextClientVersion(3);
        
        renderer = new GLRenderer();
        glSurfaceView.setRenderer(renderer);
        glSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
        
        setContentView(glSurfaceView);
    }
    
    @Override
    public boolean onTouchEvent(MotionEvent event) {
        float x = event.getX();
        float y = event.getY();
        
        switch (event.getActionMasked()) {
            case MotionEvent.ACTION_DOWN:
                touchMode = 1;
                renderer.onTouchStart(x, y, touchMode);
                return true;
                
            case MotionEvent.ACTION_POINTER_DOWN:
                if (event.getPointerCount() == 2) {
                    touchMode = 3;
                    float x1 = event.getX(0);
                    float y1 = event.getY(0);
                    float x2 = event.getX(1);
                    float y2 = event.getY(1);
                    float distance = (float)Math.sqrt(Math.pow(x2-x1,2) + Math.pow(y2-y1,2));
                    renderer.onPinch(distance);
                }
                return true;
                
            case MotionEvent.ACTION_MOVE:
                if (event.getPointerCount() == 1) {
                    renderer.onTouchMove(x, y);
                } else if (event.getPointerCount() == 2) {
                    float x1 = event.getX(0);
                    float y1 = event.getY(0);
                    float x2 = event.getX(1);
                    float y2 = event.getY(1);
                    float distance = (float)Math.sqrt(Math.pow(x2-x1,2) + Math.pow(y2-y1,2));
                    renderer.onPinch(distance);
                }
                return true;
                
            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_POINTER_UP:
                renderer.onTouchEnd();
                return true;
        }
        
        return super.onTouchEvent(event);
    }
    
    @Override
    protected void onPause() {
        super.onPause();
        if (glSurfaceView != null) {
            glSurfaceView.onPause();
        }
    }
    
    @Override
    protected void onResume() {
        super.onResume();
        if (glSurfaceView != null) {
            glSurfaceView.onResume();
        }
    }
}