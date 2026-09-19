#pragma once
#include <atomic>
#include "Shader.h"
#include "Mesh.h"
#include "ParkourGenerator.h"
#include "Player.h"

class GameRenderer {
public:
    void onSurfaceCreated();
    void onSurfaceChanged(int width, int height);
    // dönüş: 0 devam, 1 oyuncu düştü
    int onDrawFrame();

    void setTurnInput(int dir) { turnInput.store(dir); }
    void requestJump() { jumpRequested.store(true); }
    void resetGame();
    float getHeightScore() const { return player.getHeightScore(); }
    int getDifficultyLevel() const { return gen.difficultyLevel(player.getCurrentSegment()); }
    void setCostumeColor(float r, float g, float b) { player.color = Vec3(r,g,b); }
    void setGraphicsQuality(int level) { graphicsQuality = level; }

private:
    Shader shader;
    Mesh boxMesh;
    ParkourGenerator gen;
    Player player;

    int screenWidth = 1, screenHeight = 1;
    long long lastTimeNanos = 0;
    bool hasLastTime = false;

    std::atomic<int> turnInput{0};
    std::atomic<bool> jumpRequested{false};

    int graphicsQuality = 1;

    void drawBox(const Vec3& pos, const Vec3& scale, float rotationY, const Vec3& color,
                 const Mat4& viewProj, const Vec3& viewPos, float emissive);
};
