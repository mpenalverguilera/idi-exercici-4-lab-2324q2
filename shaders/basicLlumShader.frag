#version 330 core

in vec3  fmatamb;
in vec3  fmatdiff;
in vec3  fmatspec;
in float fmatshin;
in vec3  fvertex;
in vec3  fnormal;

const vec3 llumAmbient = vec3(0.1, 0.1, 0.1);

uniform vec3 colorFocusCamera;
const vec3 posFocusCamera = vec3(0.0);  // en SCO

uniform mat4 greenCarFocusToSCO;
uniform mat4 redCarFocusToSCO;
const vec4 distFocusCar = vec4(2.48, 0.4, -3.2, 1);// en SCM
const vec3 colorFocusCar = vec3(0.6, 0.6, 0);

out vec4 FragColor;


vec3 Ambient() {
  return llumAmbient*fmatamb;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme de Lambert!
  // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
  vec3 colRes = vec3(0);
  if (dot (L, NormSCO) > 0)
    colRes = colFocus * fmatdiff * dot (L, NormSCO);
  return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec3 vertSCO, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme especular!
  // Assumim que els vectors estan normalitzats
  vec3 colRes = vec3 (0);
  // Si la llum ve de darrera o el material és mate no fem res
  if ((dot(NormSCO,L) < 0) || (fmatshin == 0))
    return colRes;  // no hi ha component especular

  vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
  vec3 V = normalize(-vertSCO); // perquè la càmera està a (0,0,0) en SCO

  if (dot(R, V) < 0)
    return colRes;  // no hi ha component especular

  float shine = pow(max(0.0, dot(R, V)), fmatshin);
  return (colRes + fmatspec * colFocus * shine);
}

void main()
{
    vec3 posFocusRed = vec3(redCarFocusToSCO * distFocusCar);
    vec3 posFocusGreen = vec3(greenCarFocusToSCO * distFocusCar);

    vec3 LCamera = normalize(posFocusCamera - vec3(fvertex));
    vec3 LRed = normalize(posFocusRed - vec3(fvertex));
    vec3 LGreen = normalize(posFocusGreen - vec3(fvertex));

    vec3 difusCamera, disfusRed, difusGreen;
    difusCamera = Difus(fnormal, LCamera, colorFocusCamera);
    disfusRed = Difus(fnormal, LRed, colorFocusCar);
    difusGreen = Difus(fnormal, LGreen, colorFocusCar);

    vec3 difusTotal = difusCamera + disfusRed + difusGreen;

    vec3 especularCamera, especularRed, especularGreen;
    especularCamera = Especular(fnormal, LCamera, vec3(fvertex), colorFocusCamera);
    especularRed = Especular(fnormal, LRed, vec3(fvertex), colorFocusCar);
    difusGreen = Especular(fnormal, LGreen, vec3(fvertex), colorFocusCar);
    vec3 especularTotal = especularCamera + especularRed + especularGreen;

    FragColor = vec4(Ambient() + difusTotal + especularTotal, 1);
}
