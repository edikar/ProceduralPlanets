#version 330 core

#extension GL_OES_standard_derivatives : enable

/* Uniforms */
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform bool useHeightColor;

uniform vec3 color1;
uniform vec3 color2;
uniform vec3 color3;
uniform vec3 color4;
uniform vec3 color5;

uniform float steepnessThreshold = 0.9;

uniform sampler2D normalMap;

/* In attributes */
in vec3 FragPos;  
in vec3 Normal; 


/* Out attributes */
out vec4 FragColor;

void main()
{
	//vec3 Normal = normalize( 1 * normalize( cross( dFdx( FragPos.xyz ), dFdy( FragPos.xyz ) ) ) );

	//Calculate ambient light component
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    //Calculate light direction and normal
	vec3 norm = normalize(Normal);
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
	
	norm = normalize(Normal);
	vec3 fp = normalize(FragPos);

	float len = length(FragPos) * length(FragPos);
	finalColor += (len <= 0.9) ? color1 * len : vec3(0.0); 					//bottom
	finalColor += (len >= 0.9 && len < 1.02) ? (dot(norm, fp) > steepnessThreshold ? color2 : color4) : vec3(0.0);			//sand
	finalColor += (len >= 1.02 && len < 1.15) ? (dot(norm, fp) > steepnessThreshold ? color3 : color4) : vec3(0.0);			//grass
	finalColor += (len >= 1.15 && len < 1.2) ? color4: vec3(0.0);			//rocks
	finalColor += (len >= 1.2) ? (dot(norm, fp) > steepnessThreshold ? color5 : color4) : vec3(0.0); 						//snowc aps
	//finalColor = originalColor + brown + white + green + yellow;

	//finalColor = objectColor;
	

    vec3 result = (ambient + diffuse + specular) * finalColor;


/*     vec3 cX = vec3(texture(normalMap, FragPos.zy));
    vec3 cY = vec3(texture(normalMap, FragPos.xz));
    vec3 cZ = vec3(texture(normalMap, FragPos.xy));
    vec3 blendWeight = abs(Normal);
    blendWeight /= (blendWeight.x + blendWeight.y + blendWeight.z);
    vec3 col = cX *blendWeight.x + cY * blendWeight.y + cZ * blendWeight.z;
    //vec3 lightNormal = 
    float lightShading = (dot(Normal * cZ, vec3(0,0,5)));

    result = result * lightShading;
 */
    
    FragColor = vec4(result, 1.0);
}  