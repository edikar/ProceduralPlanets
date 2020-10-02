#version 330 core

#extension GL_OES_standard_derivatives : enable

/* Uniforms */
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform bool useHeightColor;

uniform vec3 color0;
uniform vec3 color1;
uniform vec3 color2;
uniform vec3 color3;
uniform vec3 color4;

uniform float steepnessThreshold;
uniform float rockBlendingFactor;

uniform float normalMapScale;
uniform sampler2D normalMap;

/* In attributes */
in vec3 FragPos;  
in vec3 origFrag;
in vec3 Normal; 

/* Out attributes */
out vec4 FragColor;

vec3 getNormalMap(){
/* 
float blendSharpness = 1;
	// This code used for tri-planar mapping, which is ok, but has some stretching... probably my bug.
 	vec3 blending = abs( Normal );
	blending = normalize(pow(max(blending, 0.00001), vec3(blendSharpness))); // Force weights to sum to 1.0
	float b = (blending.x + blending.y + blending.z);
	blending /= vec3(b, b, b);


	vec3 xaxis = texture2D( normalMap, origFrag.zy * normalMapScale).rgb;
	vec3 yaxis = texture2D( normalMap, origFrag.xz * normalMapScale).rgb;
	vec3 zaxis = texture2D( normalMap, origFrag.xy * normalMapScale).rgb;

	// blend the results of the 3 planar projections.
	vec3 normal = xaxis * blending.x + yaxis * blending.y + zaxis * blending.z;
	return normalize(normal);
  
 */

/*  float phy = 0.5 + atan(origFrag.x, origFrag.z) / 6.28318531; 
 float theta = 0.5 - acos(origFrag.y) / 3.14159265;
 vec3 normal = 	texture2D( normalMap, vec2(phy, theta) * normalMapScale).rgb;
 return normalize(normal * 2.0 - 1.0);
 */

 	//This is not the correct way to map 2d texture on a sphere, but it works better than the attempts above :D
 	float phy = atan(origFrag.x/origFrag.z); 
	float theta = acos(origFrag.y);
	vec3 normal = texture2D( normalMap, vec2(phy, theta) * normalMapScale).rgb;
	return normalize(normal * 2.0 - 1.0); 
 
}

void main()
{
	//not the perfect way to calculate TBN matrix, but I'm too lazy to calculate tangents and bitangents myself, and this trick seems to work
	mat3 TBN = mat3(normalize(dFdx( FragPos.xyz)), normalize(dFdy( FragPos.xyz)), normalize(Normal)); 
	vec3 normal = getNormalMap();
	normal = normalize(TBN * normal); 

	//Calculate ambient light component
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    //Calculate light direction and normal
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - FragPos); 
	//Calculate the diffuse light component
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	//calculate the specular light component
	float specularStrength = 0.2;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
	vec3 specular = specularStrength * spec * lightColor;  

	//Calculate final color with all the components altogether
	vec3 finalColor = vec3(0,0,0);
	
	vec3 nfp = normalize(FragPos);
	float steepness = dot(normal, nfp);
	float len = length(FragPos) * length(FragPos);

	float steepnessInterpolationValue = smoothstep(clamp(steepnessThreshold - rockBlendingFactor, 0, 1), clamp(steepnessThreshold + rockBlendingFactor, 0, 1), steepness);
 	if(len < 0.9){
 		float f = clamp((0.9- len)  /(0.9 - 0.88) , 0 , 1);
 		vec3 c = f * color0 + (1-f)*color1;
 		finalColor = c * len; 					//bottom
 	}
 	if(len >= 0.9 && len < 1.05){
 		float f = clamp((1.05 - len)  /(1.05 - 1.02) , 0 , 1);
 		vec3 c = f * color1 + (1-f)*color2;
 		finalColor = steepnessInterpolationValue * c + (1 - steepnessInterpolationValue)*color4;
 	}
 	if(len >= 1.05 && len < 1.2){
 		float f = clamp((1.2 - len)  /(1.2 - 1.18) , 0 , 1);
 		vec3 c = f * color2 + (1-f)*color3;
 		finalColor = steepnessInterpolationValue * c + (1 - steepnessInterpolationValue)*color4;
 	}
 	if(len >= 1.2){
 		finalColor = steepnessInterpolationValue * color3 + (1 - steepnessInterpolationValue)*color4;
 	}
 	
    vec3 result = (ambient + diffuse + specular) * finalColor;
    
    FragColor = vec4(result, 1.0);
}  