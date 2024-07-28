// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

MyGLWidget::MyGLWidget(QWidget *parent) : LL4GLWidget(parent) {
}

/*Slots*/
//Avançar cotxe
void MyGLWidget::forwardSlot()
{
  makeCurrent();
  forward();
  update();
}

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    angleX += (e->y() - yClick) * M_PI / alt;
    viewTransform ();
    actCarFocus();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_Up: {
      forward();
      break;
	}
  case Qt::Key_L: {      
    if (colorFocusCamera == glm::vec3(0.0))
    {
      colorFocusCamera =  glm::vec3(0.8);
      glClearColor(0.5, 0.7, 1.0, 1.0); 
    }
    else
    {
      colorFocusCamera = glm::vec3(0.0);
      glClearColor(0.3, 0.3, 0.3, 1.0); 
    }

    glUniform3fv (colorFocusCameraLoc, 1, &colorFocusCamera[0]);
    break;
	}
  case Qt::Key_S: {
    if (animat) timer.stop();
    else timer.start(50);
    animat = !animat;
    break;
	}	
  default: LL4GLWidget::keyPressEvent(event); break;
  }
  update();
}

/*Widget*/
//Inicialitzar
void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  
  MyGLWidget::carregaShaders();
  
  creaBuffersModels();
  creaBuffersTerra();

  iniEscena();
  iniCamera();
  iniLlum();

  connect (&timer, SIGNAL(timeout()), this, SLOT(forwardSlot()));
}
//Shaders
void MyGLWidget::carregaShaders ()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("./shaders/basicLlumShader.frag");
  vs.compileSourceFile("./shaders/basicLlumShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  // Obtenim identificador per a l'atribut “normal” del vertex shader
  normalLoc = glGetAttribLocation (program->programId(), "normal");
  // Obtenim identificador per a l'atribut “matamb” del vertex shader
  matambLoc = glGetAttribLocation (program->programId(), "matamb");
  // Obtenim identificador per a l'atribut “matdiff” del vertex shader
  matdiffLoc = glGetAttribLocation (program->programId(), "matdiff");
  // Obtenim identificador per a l'atribut “matspec” del vertex shader
  matspecLoc = glGetAttribLocation (program->programId(), "matspec");
  // Obtenim identificador per a l'atribut “matshin” del vertex shader
  matshinLoc = glGetAttribLocation (program->programId(), "matshin");

  // Demanem identificadors per als uniforms del vertex shader
  transLoc = glGetUniformLocation (program->programId(), "TG");
  projLoc = glGetUniformLocation (program->programId(), "Proj");
  viewLoc = glGetUniformLocation (program->programId(), "View");

  //Color cars
  matModifierLoc = glGetUniformLocation (program->programId(), "matModifier");
  glUniform3fv (matModifierLoc, 1, &defaultModifier[0]);
  //Llums
  colorFocusCameraLoc = glGetUniformLocation (program->programId(), "colorFocusCamera");
  greenCarFocusToSCOLoc = glGetUniformLocation (program->programId(), "greenCarFocusToSCO");
  redCarFocusToSCOLoc = glGetUniformLocation (program->programId(), "redCarFocusToSCO");
}
//Pintar
void MyGLWidget::paintGL () 
{
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
  // glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // TERRA
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLES, 0, 6);
  
  // Road
  glBindVertexArray (VAO_models[ROAD]);
  //Primer quadrant
  modelTransformRoad (glm::vec3(5, 0.01, -5), 0);
  glDrawArrays(GL_TRIANGLES, 0, models[ROAD].faces().size()*3);
  //Segon quadrant
  modelTransformRoad (glm::vec3(-5, 0.01, -5), float(M_PI/2));
  glDrawArrays(GL_TRIANGLES, 0, models[ROAD].faces().size()*3);
  //Tercer quadrant
  modelTransformRoad (glm::vec3(-5, 0.01, 5), 2*float(M_PI/2));
  glDrawArrays(GL_TRIANGLES, 0, models[ROAD].faces().size()*3);
  //Quart quadrant
  modelTransformRoad (glm::vec3(5, 0.01, 5), 3*float(M_PI/2));
  glDrawArrays(GL_TRIANGLES, 0, models[ROAD].faces().size()*3);


  // Car
  glBindVertexArray (VAO_models[CAR]);

  glUniform3fv (matModifierLoc, 1, &redModifier[0]);
  modelTransformCar (radiRed, angleRed);
  glDrawArrays(GL_TRIANGLES, 0, models[CAR].faces().size()*3);


  glUniform3fv (matModifierLoc, 1, &greenModifier[0]);
  modelTransformCar (radiGreen, angleGreen);
  glDrawArrays(GL_TRIANGLES, 0, models[CAR].faces().size()*3);


  glUniform3fv (matModifierLoc, 1, &defaultModifier[0]);
  // Pipe
  glBindVertexArray (VAO_models[PIPE]);
  modelTransformPipe ();
  glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);
  
  glBindVertexArray(0);
}

/*Models*/
//Transformadores
void MyGLWidget::modelTransformTerra ()
{
  glm::vec3 centreBaseTerra = glm::vec3(5, 0, 5); //centre = (pMin + pMax)/2 = ((0, 0, 0) + (10, 0, 10))/2
  glm::vec3 escalaTerra = glm::vec3(25.0/10.0);

  glm::mat4 TG(1.0f);
  TG = glm::scale(TG, glm::vec3(escalaTerra));
  TG = glm::translate(TG, -centreBaseTerra);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformRoad (glm::vec3 pos, float angle)
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, pos);
  TG = glm::rotate(TG, angle, glm::vec3(0, 1, 0));
  TG = glm::scale(TG, glm::vec3(5*escalaModels[ROAD]));
  TG = glm::translate(TG, -centreBaseModels[ROAD]);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPipe () {
    glm::vec3 escalaPipe = glm::vec3(5*escalaModels[PIPE]);

  glm::mat4 TG(1.0f);
  TG = glm::scale(TG, glm::vec3(escalaPipe));
  TG = glm::translate(TG, -centreBaseModels[PIPE]);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformCar (float radi, float angle)
{
  glm::mat4 TG(1.0f);
  TG = glm::rotate(TG, angle, glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(radi, 0, 0));
  TG = glm::scale(TG, glm::vec3(escalaModels[CAR]));
  TG = glm::translate(TG, -centreBaseModels[CAR]);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

//Moviment
void MyGLWidget::forward()
{
  angleRed += float(M_PI/90.0); //2º
  if (angleRed > float(2*M_PI)) angleRed -= float(2*M_PI);
  angleGreen += float(M_PI/60.0); //3º
  if (angleGreen > float(2*M_PI)) angleGreen -= float(2*M_PI);

  actCarFocus();
}

//Materials
void MyGLWidget::iniMaterialTerra ()
{
  // Donem valors al material del terra
  amb = glm::vec3(0.0,0.0,0.1);
  diff = glm::vec3(0,0.5,0.5);
  spec = glm::vec3(0.8);
  shin = 100;
}


/*Camera*/
void MyGLWidget::iniEscena ()
{
  centreEsc = glm::vec3(0, 2.5, 0); //(pMin + pMax)/2 = ((-12.5, 0, -12.5) + (12.5, 5, 12.5)) /2
  radiEsc= float(17.85357); //dist(pMin, pMax)/2 = sqrt(25^2+5^2+25^2)/2
}

void MyGLWidget::viewTransform ()
{
  View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
  View = glm::rotate(View, angleX, glm::vec3(1, 0, 0));
  View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
  View = glm::translate(View, -centreEsc);

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

/*Llum*/
glm::mat4 MyGLWidget::scaCarToSco(float radi, float angle)
{
  glm::mat4 scaToSco(1.0f);  

  scaToSco = glm::translate(scaToSco, glm::vec3(0, 0, -2*radiEsc));
  scaToSco = glm::rotate(scaToSco, angleX, glm::vec3(1, 0, 0));
  scaToSco = glm::rotate(scaToSco, -angleY, glm::vec3(0, 1, 0));
  scaToSco = glm::translate(scaToSco, -centreEsc);
  scaToSco = glm::rotate(scaToSco, angle, glm::vec3(0,1,0));
  scaToSco = glm::translate(scaToSco, glm::vec3(radi, 0, 0));
  scaToSco = glm::scale(scaToSco, glm::vec3(escalaModels[CAR]));
  scaToSco = glm::translate(scaToSco, -centreBaseModels[CAR]);
  
  return scaToSco;
}

void MyGLWidget::iniLlum()
{
  glUniform3fv (colorFocusCameraLoc, 1, &colorFocusCamera[0]);
  actCarFocus();
}

void MyGLWidget::actCarFocus()
{
  redCarFocusToSCO = scaCarToSco(radiRed, angleRed);
  greenCarFocusToSCO = scaCarToSco(radiGreen, angleGreen);
  glUniformMatrix4fv (redCarFocusToSCOLoc, 1, GL_FALSE, &redCarFocusToSCO[0][0]);
  glUniformMatrix4fv (greenCarFocusToSCOLoc, 1, GL_FALSE, &greenCarFocusToSCO[0][0]);
}