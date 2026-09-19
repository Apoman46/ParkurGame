#include "ParkourGenerator.h"
#include <cmath>

void ParkourGenerator::reset() {
    cache.clear();
}

uint32_t ParkourGenerator::hash(uint32_t x) const {
    // Basit deterministik karıştırma (xorshift benzeri) - platform diziliminin
    // her oyunda aynı ama rastgele hissettiren şekilde üretilmesi için.
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return x;
}

Segment ParkourGenerator::generateSegment(int index) const {
    Segment seg;
    if (index < 4) {
        // Başlangıçta tam güvenli platformlar (oyuncu ısınsın).
        seg.present[0] = seg.present[1] = seg.present[2] = true;
        return seg;
    }

    int diff = difficultyLevel(index);
    // Zorluk arttıkça bir şeridin "boş" (boşluk) olma ihtimali artar,
    // ve zaman zaman iki şerit birden boş olabilir (daha zor atlayışlar).
    float gapChance = 0.12f + 0.045f * (float)diff;
    if (gapChance > 0.62f) gapChance = 0.62f;

    for (int lane = 0; lane < 3; lane++) {
        uint32_t h = hash((uint32_t)(index * 7919 + lane * 104729 + 1));
        float r = (h % 10000) / 10000.0f;
        seg.present[lane] = r > gapChance;
    }

    // Aynı segmentte üç şerit de boşsa (imkansız atlayış), ortadakini garantiye al.
    if (!seg.present[0] && !seg.present[1] && !seg.present[2]) {
        seg.present[1] = true;
    }
    return seg;
}

const Segment& ParkourGenerator::getSegment(int index) {
    if (index < 0) index = 0;
    while ((int)cache.size() <= index) {
        cache.push_back(generateSegment((int)cache.size()));
    }
    return cache[index];
}

Vec3 ParkourGenerator::worldPositionFor(int segmentIndex, int lane) {
    float theta = thetaForSegment(segmentIndex) + kDTheta * 0.5f;
    float radius = kBaseRadius + (lane - 1) * kLaneWidth;
    float y = baseHeightForSegment(segmentIndex);
    float x = radius * std::cos(theta);
    float z = radius * std::sin(theta);
    return Vec3(x, y, z);
}
