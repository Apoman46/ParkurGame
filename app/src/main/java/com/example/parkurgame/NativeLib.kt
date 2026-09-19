package com.example.parkurgame

/**
 * C++ tarafındaki OpenGL ES 3.0 render/oyun mantığına köprü (JNI).
 * Gerçek uygulama native-lib.cpp içindedir.
 */
object NativeLib {
    init {
        System.loadLibrary("parkurgame")
    }

    external fun onSurfaceCreated()
    external fun onSurfaceChanged(width: Int, height: Int)
    external fun onDrawFrame(): Int  // 0 = devam, 1 = oyuncu düştü/öldü

    // input: -1 sol, 0 nötr, 1 sağ
    external fun setTurnInput(direction: Int)
    external fun requestJump()

    external fun resetGame()
    external fun getHeightScore(): Float
    external fun getDifficultyLevel(): Int
    external fun setCostumeColor(r: Float, g: Float, b: Float)
    external fun setGraphicsQuality(level: Int) // 0 düşük,1 orta,2 yüksek
}
