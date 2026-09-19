#include "Player.h"
#include <cmath>
#include <algorithm>

void Player::reset() {
    theta = 0.0f;
    lane = 1.0f;
    targetLane = 1.0f;
    jumpOffsetY = 0.0f;
    velocityY = 0.0f;
    grounded = true;
    facingAngle = 0.0f;
    squashStretch = 1.0f;
}

bool Player::update(float dt, int turnInput, bool jumpRequested, ParkourGenerator& gen) {
    // Zorluk arttıkça ileri hız hafifçe artar (daha akıcı ve zorlayıcı hissettirir).
    int seg = ParkourGenerator::segmentIndexForTheta(theta);
    int diff = gen.difficultyLevel(seg);
    float speedMul = 1.0f + std::min(diff, 10) * 0.045f;

    theta += kForwardAngularSpeed * speedMul * dt;
    facingAngle = theta + 1.5707963f; // teğet yön (çember üzerinde ilerleme yönü)

    if (turnInput < 0) targetLane = std::max(0.0f, targetLane - 1.0f);
    else if (turnInput > 0) targetLane = std::min(2.0f, targetLane + 1.0f);
    // not: turnInput her karede aynı değeri koruyabileceğinden hedef şerit
    // yalnızca gerçek bir "geçiş" istendiğinde native tarafta bir kerelik ayarlanmalı;
    // basitlik için burada sürekli basılı tutmak şeridi sınırlarda tutar (0/2'yi aşmaz).

    float laneDelta = targetLane - lane;
    float step = kLaneChangeSpeed * dt;
    if (std::fabs(laneDelta) < step) lane = targetLane;
    else lane += (laneDelta > 0 ? step : -step);

    if (jumpRequested && grounded) {
        velocityY = kJumpImpulse;
        grounded = false;
    }

    velocityY += kGravity * dt;
    jumpOffsetY += velocityY * dt;

    // Basit animasyon: zıplarken hafif uzama, inişte hafif ezilme, sonra normale dönüş.
    float targetSquash = grounded ? 1.0f : (velocityY > 0 ? 1.18f : 0.9f);
    squashStretch += (targetSquash - squashStretch) * std::min(1.0f, dt * 10.0f);

    if (jumpOffsetY <= 0.0f) {
        // Zemine (bir sonraki segmentin tabanına) değme kontrolü.
        int laneIdx = (int)std::round(lane);
        laneIdx = std::max(0, std::min(2, laneIdx));
        int curSeg = ParkourGenerator::segmentIndexForTheta(theta);
        const Segment& s = gen.getSegment(curSeg);

        if (s.present[laneIdx]) {
            jumpOffsetY = 0.0f;
            velocityY = 0.0f;
            grounded = true;
        } else {
            grounded = false; // boşluğa düşüyor
            if (jumpOffsetY < kFallDeathThreshold) {
                return true; // oyun bitti
            }
        }
    }

    return false;
}

Vec3 Player::getWorldPosition() const {
    int curSeg = ParkourGenerator::segmentIndexForTheta(theta);
    float baseY = ParkourGenerator::baseHeightForSegment(curSeg);
    // Segment içi ilerlemeyle yüksekliği yumuşakça interpole et (basamaklı görünmesin).
    float segTheta0 = ParkourGenerator::thetaForSegment(curSeg);
    float t = (theta - segTheta0) / ParkourGenerator::kDTheta;
    float nextY = ParkourGenerator::baseHeightForSegment(curSeg + 1);
    float smoothBaseY = baseY + (nextY - baseY) * std::min(1.0f, std::max(0.0f, t));

    float radius = ParkourGenerator::kBaseRadius + (lane - 1.0f) * ParkourGenerator::kLaneWidth;
    float x = radius * std::cos(theta);
    float z = radius * std::sin(theta);
    float y = smoothBaseY + 0.5f + jumpOffsetY; // 0.5 platform yüzeyi üstü oyuncu merkezi

    return Vec3(x, y, z);
}
