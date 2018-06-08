#version 330 core
// Ouput data

out vec4 outCol;

// Input data
in vec3 in_normal;
in vec2 in_tex;
in vec3 in_fragPos;

uniform vec3 objectColor;
uniform vec3 viewPos;

uniform float ourColor;

struct Material {
    sampler2D ambient;
    sampler2D diffuse1;
    sampler2D diffuse2;
    sampler2D diffuse3;
    sampler2D specular1;
    sampler2D specular2;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Spotlight {
    vec3 position;
    vec3 direction;

    vec3 diffuse;
    vec3 specular;

    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform DirectionalLight dirLight;
uniform Light light;
uniform Spotlight spotlight;

/**
 * ******************************************************
 * Calculate light attenuation.
 *
 * Light fades off with distance;
 * The formula is presented here:
 * https://learnopengl.com/Lighting/Light-casters
 * and here:
 * http://wiki.ogre3d.org/-Point+Light+Attenuation
 *
 * But no mention is made of how they came across it.
 * Also: no idea, what the values for the constants should
 * be. They present a light/distance graph, but don't mention what
 * they measure the distance in...
 *
 * @param light source
 * @param the object position
 * ******************************************************
**/
float calculateLightAttenuation(Light source, vec3 objPos) {
    float distance = length(source.position - objPos);
    return (1.0 / 
            (source.constant + source.linear * distance + 
             source.quadratic * (distance * distance)));
}

/**
 * ******************************************************
 * Light ambient.
 *
 * This is just based on the value of the light ambient
 * (light color) and the material ambient (material color).
 * ******************************************************
**/
vec3 calculateAmbient(vec3 lightAmbient, sampler2D materialAmbient, vec2 texPos) {
    return (lightAmbient * vec3(texture(materialAmbient, texPos)));
}

/**
 * ******************************************************
 * Calculate Diffuse
 *
 * The diffuse lighting is calculated using the normal
 * and the vector of the light direction in a dot product.
 *
 * The normal in this case is not the actual normal due
 * to abnormalities that can appear on un-uniform scaling.
 * It's actually the transpose-inversed Normal. From here on
 * simply shortened as the normal_MV
 * See more here:
 * http://www.lighthouse3d.com/tutorials/glsl-tutorial/the-normal-matrix/
 * 
 * The light source can also have diffuse properties and
 * the material as well. As such, after the dot product
 * we need to multiply by the light source diffuse property
 * and the material diffuse property.
 * ******************************************************
**/
vec3 calculateDiffuse(vec3 normal_MV, vec3 lightDir, vec3 lightDiffuse, sampler2D materialDiffuse, vec2 texPos) {
    /* Diffuse impact. Dot product */
    float diff = max(dot(normal_MV, lightDir), 0.0);
    return (lightDiffuse * (diff * vec3(texture(materialDiffuse, texPos))));
}

/**
 * ******************************************************
 * Calculate Specular
 *
 * Depending on the angle the viewer, the specular will 
 * differ. This is the shinny part on the object.
 *
 * First we need to calculate the reflection around the
 * normal_MV. It uses the reflect function, but, in
 * summary, a reflection is a projection followed by an
 * extension of the the projected vector on the mirrored
 * side. Which amounts to the following formulas.
 *
 * Algebraic:
 * 1. u = u_par + u_proj
 * 2. u'= u_par - u_proj   (u_proj is the extension. same vec, different direction)
 * Which means the projetion will ultimately be: 
 * 3. u'=(v^-1)uv
 * Step 3. is a long jump, see video for more:
 * https://www.youtube.com/watch?v=ERpcSJzX448
 *
 * Geometric:
 * v.w = |v||w|*cos(2*theta) where theta is angle(v,n)
 * Since we are working with unit vectors:
 * v.w = 2cos(theta)^2 -1; where v.n = cos(theta)
 * v.w = 2(v.n)^2 -1;   but v.v = 1
 * v.w = 2(v.n)(v.n) - v.v; divide with v^-1 
 *
 * w = 2(v.n)n - v
 *
 * 
 * We then multiply by the specular constant. A very large power.
 * The larger the less light scaters, the more the object shines.
 * Aptly called in this case: materialShininess
 *
 * We then apply the light and material specular properties
 *
 * ******************************************************
**/
vec3 calculateSpecular(vec3 normal_MV, vec3 viewDir, vec3 lightDir, vec3 lightSpecular, sampler2D materialSpecular, float materialShininess, vec2 texPos) {
    vec3 reflectDir = reflect(-lightDir, normal_MV);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
    return (lightSpecular * (spec * vec3(texture(materialSpecular, texPos))));
}

/**
 * ******************************************************
 * The directional Lighing.
 *
 * Has fewer components and is easier to calculate.
 * And does not take attenuation into account.
 * The sun shines bright. Grossly incandescent.
 * ******************************************************
**/
vec3 calculateDirectionalLight(DirectionalLight dl, Material material,
                               vec3 normal_MV,
                               vec3 viewDir,
                               vec2 texPos) {
    vec3 lved /* light vector direction */ = normalize(-dl.direction);

    /* Ambient */
    //vec3 ambient = dl.ambient * vec3(texture(material.ambient, texPos));
    vec3 ambient = calculateAmbient(dl.ambient, material.ambient, texPos);

    /* Diffuse */
    vec3 diffuse = calculateDiffuse(normal_MV, lved , dl.diffuse, material.diffuse1, texPos);

    /* Specular */
    vec3 specular = calculateSpecular(normal_MV, viewDir, lved , light.specular, material.specular1, material.shininess, texPos);

    return (ambient + diffuse + specular);
}

/**
 * ******************************************************
 * Point light. 
 *
 * Like a light bulb.
 * Calculate ambient, diffuse and specula, just like 
 * the directional light, only it does take into account
 * the attenuation. Sort of like how lightbulbs mention
 * lumens on the package.
 * ******************************************************
**/
vec3 calculatePointLight(float attenuation, vec2 texPos, 
                        vec3 normal_MV, 
                        vec3 viewDir,
                        Light light,
                        Material material) {
    
    vec3 lved /* light vector direction */ = normalize(light.position - in_fragPos);

    /* Setting material */
    //vec3 ambient = light.ambient * vec3(texture(material.ambient, texPos));
    vec3 ambient = calculateAmbient(light.ambient, material.ambient, texPos);
    ambient *= attenuation;

    /* Diffuse */
    vec3 diffuse = calculateDiffuse(normal_MV, lved , light.diffuse, material.diffuse1, texPos);
    diffuse *= attenuation;

    /* Specular */
    vec3 specular = calculateSpecular(normal_MV, viewDir, lved , light.specular, material.specular1, material.shininess, texPos);
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

/**
 * ******************************************************
 * Spotlight
 *
 * It's a point light with cutoff angles.
 * ******************************************************
**/
vec3 calculateSpotlight(float attenuation,
                        vec3 normal_MV,
                        vec3 viewDir,
                        vec2 texPos,
                        Spotlight spotlight,
                        Material material) {
    vec3 lved /* light vector direction */ = normalize(spotlight.position - in_fragPos);

    /* Diffuse */
    vec3 diffuse = calculateDiffuse(normal_MV, lved, spotlight.diffuse, material.diffuse1, texPos);
    diffuse *= attenuation;

    /* Diffuse */
    vec3 specular = calculateSpecular(normal_MV, viewDir, lved, spotlight.specular, material.specular1, material.shininess, texPos);
    specular *= attenuation;

    float theta = dot(lved, normalize(-spotlight.direction));
    float epsilon = (spotlight.cutOff - spotlight.outerCutOff);
    float intensity = clamp((theta - spotlight.outerCutOff) / epsilon, 0.0, 1.0);

    diffuse  *= intensity;
    specular *= intensity;

    return (diffuse + specular);
}

/**
 * ******************************************************
 * TODO DOC THIS
 * sources:
 * https://learnopengl.com/Advanced-OpenGL/Depth-testing
 * http://www.songho.ca/opengl/gl_projectionmatrix.html
 * ******************************************************
**/
float linearizeDepth(float depth, float near, float far) {
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}

/**
 * ******************************************************
 * Main 
 * ******************************************************
**/
void main()
{
    /* Normalization */
    vec3 normal_MV = normalize(in_normal);
    vec3 lightDir = normalize(light.position - in_fragPos);
    // the viewer is always at viewPos = (0,0,0) in view-space, 
    // in that case 
    // viewDir = (0,0,0) - Position
    // viewDir = -Position
    vec3 viewDir = normalize(viewPos - in_fragPos);

    /* Attenuation */
    float attenuation = calculateLightAttenuation(light, in_fragPos);

    /* Point Light */
    vec3 result = calculatePointLight(attenuation, in_tex,
                        normal_MV, viewDir,
                        light, material);
    result *= objectColor; 
    /* Spotlight */
    result += calculateSpotlight(attenuation, normal_MV,
                   viewDir, in_tex, spotlight, material);

    /* Directional Light  */
    result += calculateDirectionalLight(dirLight, material, normal_MV, viewDir, in_tex);

    //outCol = vec4(result, 1.0);
    float near = 0.1;
    float far = 100.0;
    float depth = linearizeDepth(gl_FragCoord.z, near, far);
    depth /= far; // divide by far for demonstration; TODO what does he mean here, why not just divide in func
    outCol = vec4(vec3(depth), 1.0);
}
