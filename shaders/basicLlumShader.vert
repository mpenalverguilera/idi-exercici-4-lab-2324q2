#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3  matamb;
in vec3  matdiff;
in vec3  matspec;
in float matshin;

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;
uniform vec3 matModifier;

out vec3  fmatamb;
out vec3  fmatdiff;
out vec3  fmatspec;
out float fmatshin;
out vec3  fvertex;
out vec3  fnormal;

void main()
{	
    vec4 vertexSCO = View * TG * vec4 (vertex, 1.0); //L
    // Passem les dades al fragment shader
    fmatamb  = matamb;
    fmatdiff = matdiff*matModifier;
    fmatspec = matspec;
    fmatshin = matshin;
    fvertex = vec3(vertexSCO);
    fnormal = normalize(inverse(transpose(mat3(View * TG)))*normal);

    gl_Position = Proj * vertexSCO;
}
