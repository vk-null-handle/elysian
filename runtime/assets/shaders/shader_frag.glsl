#version 460

// Interpolated vertex color for current fragment
layout(location = 0) in vec3 inColor;
// Output color of the fragment
layout(location = 0) out vec4 fragColor;

void main() {
	fragColor = vec4(inColor, 1.0);
}
