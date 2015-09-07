#version 330

out vec4 color;

uniform float diameter;
uniform sampler2D diffuse_map;

in vec2 interpolated_texCoord;

// Gaussian coefficients
#define C0 (0.159999996 / 0.9800000106)
#define C1 (0.150000006 / 0.9800000106)
#define C2 (0.119999997 / 0.9800000106)
#define C3 (0.0900000036 / 0.9800000106)
#define C4 (0.0500000007 / 0.9800000106)

void main(void)
{
  float stp = diameter * (0.125 / 2.0);
  float y_pos = interpolated_texCoord.y - (4 * stp);
  float x_pos = interpolated_texCoord.x;

  color = texture2D(diffuse_map, vec2(x_pos, y_pos));
  color += C4 * max(texture2D(diffuse_map, vec2(x_pos, y_pos)) - 1.0f, 0.0f);
  y_pos += stp;
  color += C3 * max(texture2D(diffuse_map, vec2(x_pos, y_pos)) - 1.0f, 0.0f);
  y_pos += stp;
  color += C2 * max(texture2D(diffuse_map, vec2(x_pos, y_pos)) - 1.0f, 0.0f);
  y_pos += stp;
  color += C1 * max(texture2D(diffuse_map, vec2(x_pos, y_pos)) - 1.0f, 0.0f);
  y_pos += stp;
  color += C0 * max(texture2D(diffuse_map, vec2(x_pos, y_pos)) - 1.0f, 0.0f);
  y_pos += stp;
  color += C1 * max(texture2D(diffuse_map, vec2(x_pos, y_pos)) - 1.0f, 0.0f);
  y_pos += stp;
  color += C2 * max(texture2D(diffuse_map, vec2(x_pos, y_pos)) - 1.0f, 0.0f);
  y_pos += stp;
  color += C3 * max(texture2D(diffuse_map, vec2(x_pos, y_pos)) - 1.0f, 0.0f);
  y_pos += stp;
  color += C4 * max(texture2D(diffuse_map, vec2(x_pos, y_pos)) - 1.0f, 0.0f);
}
