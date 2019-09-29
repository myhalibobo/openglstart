#version 330 core
out vec4 FragColor;

in vec2 outUV;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300.0;
const float mohu = 16.0;
void main(){
	vec2 TexCoords = outUV;
	//反向	
	/*FragColor = vec4(vec3(1.0 - texture(screenTexture, outUV)), 1.0);
	//灰度
	FragColor = texture(screenTexture, TexCoords);
    float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    FragColor = vec4(average, average, average, 1.0);

	FragColor = texture(screenTexture, TexCoords);
    average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    FragColor = vec4(average, average, average, 1.0);*/

	vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // 左上
        vec2( 0.0f,    offset), // 正上
        vec2( offset,  offset), // 右上
        vec2(-offset,  0.0f),   // 左
        vec2( 0.0f,    0.0f),   // 中
        vec2( offset,  0.0f),   // 右
        vec2(-offset, -offset), // 左下
        vec2( 0.0f,   -offset), // 正下
        vec2( offset, -offset)  // 右下
    );

    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
	
	kernel = float[](
		1.0 / mohu, 2.0 / mohu, 1.0 / mohu,
		2.0 / mohu, 4.0 / mohu, 2.0 / mohu,
		1.0 / mohu, 2.0 / mohu, 1.0 / mohu  
	);
	kernel = float[](
        1, 1, 1,
        1,  -8, 1,
        1, 1, 1
    );
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    FragColor = vec4(col, 1.0);
}
