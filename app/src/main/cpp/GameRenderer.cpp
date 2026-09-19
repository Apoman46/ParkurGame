#include "GameRenderer.h"
#include "Shaders.h"
#include <GLES3/gl3.h>
#include <time.h>
#include <cmath>
#include <algorithm>

void GameRenderer::onSurfaceCreated() {
    shader.compile(VERTEX_SHADER_SRC, FRAGMENT_SHADER_SRC);
    boxMesh.createUnitBox();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    resetGame();
    hasLastTime = false;
}

void GameRenderer::onSurfaceChanged(int width, int height) {
    screenWidth = width > 0 ? width : 1;
    screenHeight = height > 0 ? height : 1;
    glViewport(0, 0, screenWidth, screenHeight);
}

void GameRenderer::resetGame() {
    gen.reset();
    player.reset();
    jumpRequested.store(false);
    turnInput.store(0);
}

void GameRenderer::drawBox(const Vec3& pos, const Vec3& scale, float rotationY, const Vec3& color,
                            const Mat4& viewProj, const Vec3& viewPos, float emissive) {
    Mat4 model = Mat4::translate(pos).multiply(Mat4::rotateY(rotationY)).multiply(Mat4::scale(scale));
    Mat3 normalMat = Mat3::rotateY(rotationY);

    shader.setMat4("uModel", model);
    shader.setMat3("uNormalMatrix", normalMat);
    shader.setVec3("uBaseColor", color);
    shader.setFloat("uEmissive", emissive);
    boxMesh.draw();
    (void)viewProj; (void)viewPos;
}

int GameRenderer::onDrawFrame() {
    // Delta zaman hesapla
    struct timespec ts{};
    clock_gettime(CLOCK_MONOTONIC, &ts);
    long long now = (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
    float dt = 0.016f;
    if (hasLastTime) {
        dt = (float)((now - lastTimeNanos) / 1e9);
        dt = std::min(dt, 0.05f); // ani sıçramaları engelle (arka plana alma vs.)
    }
    lastTimeNanos = now;
    hasLastTime = true;

    bool jumpFlag = jumpRequested.exchange(false);
    bool died = player.update(dt, turnInput.load(), jumpFlag, gen);

    // --- Çizim ---
    glClearColor(0.04f, 0.06f, 0.10f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.use();

    Vec3 playerPos = player.getWorldPosition();
    float facing = player.getFacingAngle();
    Vec3 forwardTangent(std::cos(facing), 0, std::sin(facing));

    Vec3 eye = playerPos - forwardTangent * 6.0f + Vec3(0, 3.2f, 0)
               + Vec3(std::cos(player.getTheta())*1.5f, 0, std::sin(player.getTheta())*1.5f);
    Vec3 center = playerPos + forwardTangent * 2.0f;
    Mat4 view = Mat4::lookAt(eye, center, Vec3(0,1,0));
    float aspect = (float)screenWidth / (float)screenHeight;
    Mat4 proj = Mat4::perspective(60.0f * 3.14159265f / 180.0f, aspect, 0.1f, 120.0f);
    Mat4 viewProj = proj.multiply(view);

    shader.setMat4("uViewProj", viewProj);
    shader.setVec3("uViewPos", eye);
    shader.setVec3("uLightDir", normalize(Vec3(0.4f, 1.0f, 0.3f)));
    shader.setVec3("uFogColor", Vec3(0.04f, 0.06f, 0.10f));
    shader.setFloat("uFogDensity", graphicsQuality == 0 ? 3.5f : 1.6f);

    // Görüş mesafesi grafik kalitesine göre (düşük kalite = daha az segment çizimi -> daha yüksek FPS)
    int aheadSegments = graphicsQuality == 0 ? 10 : (graphicsQuality == 1 ? 16 : 24);

    int curSeg = player.getCurrentSegment();
    for (int i = curSeg - 1; i <= curSeg + aheadSegments; i++) {
        if (i < 0) continue;
        const Segment& seg = gen.getSegment(i);
        float theta = ParkourGenerator::thetaForSegment(i) + ParkourGenerator::kDTheta * 0.5f;
        for (int lane = 0; lane < 3; lane++) {
            if (!seg.present[lane]) continue;
            Vec3 pos = ParkourGenerator::worldPositionFor(i, lane);
            pos.y -= 0.15f; // platform üst yüzeyi theta'nın hedef y'sinde olsun
            Vec3 scale(1.5f, 0.3f, ParkourGenerator::kBaseRadius * ParkourGenerator::kDTheta * 1.05f);
            Vec3 color = (lane == 1) ? Vec3(0.30f, 0.55f, 0.85f) : Vec3(0.22f, 0.30f, 0.42f);
            float emissive = 0.05f;
            drawBox(pos, scale, theta + 1.5707963f, color, viewProj, eye, emissive);
        }
    }

    // Oyuncu (squash/stretch animasyonlu kutu)
    Vec3 playerScale(0.7f, 0.9f * player.squashStretch, 0.7f);
    drawBox(playerPos, playerScale, facing, player.color, viewProj, eye, 0.25f);

    return died ? 1 : 0;
}
