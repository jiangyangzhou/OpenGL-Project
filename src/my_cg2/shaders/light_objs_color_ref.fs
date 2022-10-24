#version 330 core
#extension GL_NV_shadow_samplers_cube : enable


out vec4 FragColor;

#define NR_POINT_LIGHTS 2


struct Material {
    float shininess;
    vec4 color;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal; 
in vec4 Color;  
in vec4 FragPosLightSpace[NR_POINT_LIGHTS];


uniform vec3 viewPos;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Material material;
uniform samplerCube cubeTexture;
uniform sampler2D shadowMap[NR_POINT_LIGHTS];




vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir,  vec4 color);




float ShadowCalculation(PointLight light, vec4 fragPosLightSpace,  vec3 normal, sampler2D shadowMap)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 lightDir = normalize(light.position - FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}
vec4 lerp(vec4 a, vec4 b, float s){
    return vec4(a+(b-a)*s);
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = -normalize(viewPos - FragPos);
    vec3 R = reflect(viewDir, norm);
    vec4 reflect_color = textureCube(cubeTexture, R) * Color *2.0;
    vec3 r =refract(viewDir, norm, 1.0/2.4);
    vec4 refrac_color=textureCube(cubeTexture,r)* Color *2.0  ;

    float fresnel = 0.3 + 0.5 *pow(min(0.0, 1.0-dot(viewDir, norm)), 0.3);
    vec4 color = lerp(refrac_color, reflect_color, fresnel);
    vec3 result = vec3(0.2, 0.2, 0.2)* color.rgb ;
    for(int i=0; i<NR_POINT_LIGHTS; i++){
       result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, color);
    }
    FragColor = vec4(result, 1.0);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 color)
{
    vec3 lightDir = normalize(light.position - fragPos); 
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // attenuation 
    float distance = length(light.position- fragPos);
    float attenuation = 1.0 /(light.constant + light.linear * distance + light.quadratic *(distance * distance));

    vec3 ambient = light.ambient * color.rgb; //vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * color.rgb; //* vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(0.2);  // * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return ambient + (diffuse+ specular);
}

