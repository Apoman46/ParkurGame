#include <jni.h>
#include <memory>
#include "GameRenderer.h"

// Tek bir global GameRenderer örneği: GLSurfaceView yaşam döngüsüyle birebir eşleşir.
static std::unique_ptr<GameRenderer> g_renderer;

extern "C" {

JNIEXPORT void JNICALL
Java_com_example_parkurgame_NativeLib_onSurfaceCreated(JNIEnv*, jobject) {
    if (!g_renderer) g_renderer = std::make_unique<GameRenderer>();
    g_renderer->onSurfaceCreated();
}

JNIEXPORT void JNICALL
Java_com_example_parkurgame_NativeLib_onSurfaceChanged(JNIEnv*, jobject, jint width, jint height) {
    if (g_renderer) g_renderer->onSurfaceChanged(width, height);
}

JNIEXPORT jint JNICALL
Java_com_example_parkurgame_NativeLib_onDrawFrame(JNIEnv*, jobject) {
    if (!g_renderer) return 0;
    return g_renderer->onDrawFrame();
}

JNIEXPORT void JNICALL
Java_com_example_parkurgame_NativeLib_setTurnInput(JNIEnv*, jobject, jint direction) {
    if (g_renderer) g_renderer->setTurnInput(direction);
}

JNIEXPORT void JNICALL
Java_com_example_parkurgame_NativeLib_requestJump(JNIEnv*, jobject) {
    if (g_renderer) g_renderer->requestJump();
}

JNIEXPORT void JNICALL
Java_com_example_parkurgame_NativeLib_resetGame(JNIEnv*, jobject) {
    if (g_renderer) g_renderer->resetGame();
}

JNIEXPORT jfloat JNICALL
Java_com_example_parkurgame_NativeLib_getHeightScore(JNIEnv*, jobject) {
    return g_renderer ? g_renderer->getHeightScore() : 0.0f;
}

JNIEXPORT jint JNICALL
Java_com_example_parkurgame_NativeLib_getDifficultyLevel(JNIEnv*, jobject) {
    return g_renderer ? g_renderer->getDifficultyLevel() : 0;
}

JNIEXPORT void JNICALL
Java_com_example_parkurgame_NativeLib_setCostumeColor(JNIEnv*, jobject, jfloat r, jfloat g, jfloat b) {
    if (g_renderer) g_renderer->setCostumeColor(r, g, b);
}

JNIEXPORT void JNICALL
Java_com_example_parkurgame_NativeLib_setGraphicsQuality(JNIEnv*, jobject, jint level) {
    if (g_renderer) g_renderer->setGraphicsQuality(level);
}

} // extern "C"
