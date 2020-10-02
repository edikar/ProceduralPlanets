#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texture0;
uniform mat3 postProcKernel;

/* the following main() uses blur kernel */
const float offset = 1.0 / 300.0;  

void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );
/* 
    float kernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16  
    );  */
 
    float kernel[9];
    kernel[0] = postProcKernel[0][0];
    kernel[1] = postProcKernel[0][1];
    kernel[2] = postProcKernel[0][2];
    kernel[3] = postProcKernel[1][0];
    kernel[4] = postProcKernel[1][1];
    kernel[5] = postProcKernel[1][2];
    kernel[6] = postProcKernel[2][0];
    kernel[7] = postProcKernel[2][1];
    kernel[8] = postProcKernel[2][2];
 
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(texture0, TexCoord.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    FragColor = vec4(col, 1.0);

}  