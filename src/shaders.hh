#pragma once

#include <string>

inline std::string vertSrc = R"(
#version 450

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv_in;
out vec2 uv;
uniform mat4 mvp;

void main()
{
  uv = uv_in;
  gl_Position = mvp * vec4(pos, 1.0);
}
)";

inline std::string fragSrc = R"(
#version 450

in vec2 uv;
layout(binding = 0) uniform sampler2D tex;
out vec4 fragColor;

void main()
{
  fragColor = texture(tex, uv);
}
)";

inline std::string textFragSrc = R"(
#version 450

in vec2 uv;
uniform vec4 inputColor = vec4(1, 1, 1, 1);
layout(binding = 0) uniform sampler2D tex;
out vec4 fragColor;

void main()
{
  fragColor = vec4(inputColor.rgb, texture(tex, uv).r * inputColor.w);
}
)";
