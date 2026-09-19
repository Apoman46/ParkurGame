#pragma once
#include "MathUtil.h"
#include "ParkourGenerator.h"

class Player {
public:
    void reset();
    // dt: saniye, turnInput: -1/0/1, jumpRequested: bir kerelik tetikleyici
    // dönüş: true ise oyuncu düştü (oyun bitti)
    bool update(float dt, int turnInput, bool jumpRequested, ParkourGenerator& gen);

    Vec3 getWorldPosition() const;
    float getFacingAngle() const { return facingAngle; }
    float getHeightScore() const { return ParkourGenerator::baseHeightForSegment(0) + theta * (ParkourGenerator::kRiseRate / ParkourGenerator::kDTheta); }
    float getTheta() const { return theta; }
    int getCurrentSegment() const { return ParkourGenerator::segmentIndexForTheta(theta); }

    float squashStretch = 1.0f; // animasyon için Y ölçek çarpanı
    Vec3 color = Vec3(0.2f, 0.78f, 1.0f);

private:
    float theta = 0.0f;       // spiral açısı (ilerleme)
    float lane = 1.0f;        // 0=iç,1=orta,2=dış (sürekli değer, hedefe yumuşak geçiş)
    float targetLane = 1.0f;
    float jumpOffsetY = 0.0f; // zemine göre ekstra yükseklik (ziplama/düşme)
    float velocityY = 0.0f;
    bool grounded = true;
    float facingAngle = 0.0f;

    static constexpr float kForwardAngularSpeed = 0.55f; // rad/sn (otomatik ileri gidiş)
    static constexpr float kGravity = -14.0f;
    static constexpr float kJumpImpulse = 5.2f;
    static constexpr float kLaneChangeSpeed = 6.0f;
    static constexpr float kFallDeathThreshold = -6.0f;
};
