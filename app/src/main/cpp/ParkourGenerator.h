#pragma once
#include <vector>
#include <cstdint>
#include "MathUtil.h"

// Parkur, sabit açısal adımlarla ilerleyen bir "segment" dizisidir.
// Her segment 3 şeride (iç/orta/dış) sahiptir; her şeritte platform olup olmadığı
// segment indeksine ve zorluk seviyesine göre prosedürel/deterministik üretilir.
// Spiral: theta arttıkça hem açı (x,z konumu) hem yükseklik (y) artar.

struct Segment {
    bool present[3] = {true, true, true}; // iç, orta, dış şerit
};

class ParkourGenerator {
public:
    static constexpr float kDTheta = 0.35f;         // her segmentin açısal genişliği (radyan)
    static constexpr float kBaseRadius = 4.0f;       // spiral yarıçapı
    static constexpr float kLaneWidth = 1.6f;        // şeritler arası mesafe
    static constexpr float kRiseRate = 0.65f;        // her segmentte ne kadar yükseleceği

    void reset();

    // Segmenti indeksle al (yoksa üret ve cache'le).
    const Segment& getSegment(int index);

    // Segment indeksine göre spiral taban yüksekliği (metre).
    static float baseHeightForSegment(int index) { return index * kRiseRate; }
    static float thetaForSegment(int index) { return index * kDTheta; }

    // theta -> segment indeksi
    static int segmentIndexForTheta(float theta) { return (int)(theta / kDTheta); }

    // Belirli bir segment+şerit için dünya pozisyonu (platform merkezi).
    static Vec3 worldPositionFor(int segmentIndex, int lane); // lane: 0=iç,1=orta,2=dış

    int difficultyLevel(int segmentIndex) const { return segmentIndex / kSegmentsPerDifficulty; }

private:
    static constexpr int kSegmentsPerDifficulty = 18;
    std::vector<Segment> cache;

    uint32_t hash(uint32_t x) const;
    Segment generateSegment(int index) const;
};
