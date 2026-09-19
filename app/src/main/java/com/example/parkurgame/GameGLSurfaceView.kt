package com.example.parkurgame

import android.content.Context
import android.opengl.GLSurfaceView
import android.os.Handler
import android.os.Looper
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

/**
 * OpenGL ES 3.0 bağlamı isteyen GLSurfaceView. Tüm çizim işini JNI üzerinden
 * C++ tarafına (native-lib.cpp / GameRenderer.cpp) devreder.
 */
class GameGLSurfaceView(context: Context) : GLSurfaceView(context) {

    private val mainHandler = Handler(Looper.getMainLooper())

    init {
        setEGLContextClientVersion(3)
        setEGLConfigChooser(8, 8, 8, 8, 16, 0)
        preserveEGLContextOnPause = true
        renderer = object : Renderer {
            override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
                NativeLib.onSurfaceCreated()
                NativeLib.setGraphicsQuality(Prefs.graphicsQuality)
            }
            override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
                NativeLib.onSurfaceChanged(width, height)
            }
            override fun onDrawFrame(gl: GL10?) {
                val result = NativeLib.onDrawFrame()
                if (result == 1) {
                    mainHandler.post { onPlayerFell?.invoke() }
                }
            }
        }
        renderMode = RENDERMODE_CONTINUOUSLY
    }

    var onPlayerFell: (() -> Unit)? = null
}
