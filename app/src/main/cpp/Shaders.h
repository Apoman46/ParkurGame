#pragma once

// OpenGL ES 3.0 (GLSL ES 300) shader kaynakları.
// Vertex shader: MVP dönüşümü + dünya-uzayı normal/pozisyon aktarımı.
// Fragment shader: Basit Lambert (yayınık) + Blinn-Phong parlaklık + mesafeye göre sis (fog),
// ayrıca platformlar için ince kenar-vurgu (rim light) efekti.

static const char* VERTEX_SHADER_SRC = R"GLSL(
#version 300 es
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uViewProj;
uniform mat3 uNormalMatrix;

out vec3 vWorldPos;
out vec3 vNormal;

void main() {
    vec4 worldPos = uModel * vec4(aPosition, 1.0);
    vWorldPos = worldPos.xyz;
    vNormal = normalize(uNormalMatrix * aNormal);
    gl_Position = uViewProj * worldPos;
}
)GLSL";

static const char* FRAGMENT_SHADER_SRC = R"GLSL(
#version 300 es
precision mediump float;

in vec3 vWorldPos;
in vec3 vNormal;

uniform vec3 uBaseColor;
uniform vec3 uLightDir;      // normalize edilmiş, ışığa doğru
uniform vec3 uViewPos;
uniform vec3 uFogColor;
uniform float uFogDensity;
uniform float uEmissive;     // 0..1 kendinden ışıma (oyuncu/vurgu efektleri için)

out vec4 fragColor;

void main() {
    vec3 N = normalize(vNormal);
    vec3 L = normalize(uLightDir);
    vec3 V = normalize(uViewPos - vWorldPos);
    vec3 H = normalize(L + V);

    float diff = max(dot(N, L), 0.0);
    float spec = pow(max(dot(N, H), 0.0), 32.0);
    float rim = pow(1.0 - max(dot(N, V), 0.0), 2.5);

    vec3 ambient = uBaseColor * 0.28;
    vec3 diffuse = uBaseColor * diff * 0.85;
    vec3 specular = vec3(1.0) * spec * 0.35;
    vec3 rimLight = uBaseColor * rim * 0.5;

    vec3 color = ambient + diffuse + specular + rimLight + uBaseColor * uEmissive;

    float dist = length(uViewPos - vWorldPos);
    float fogFactor = 1.0 - exp(-uFogDensity * dist * dist * 0.001);
    fogFactor = clamp(fogFactor, 0.0, 1.0);
    color = mix(color, uFogColor, fogFactor);

    fragColor = vec4(color, 1.0);
}
)GLSL";
