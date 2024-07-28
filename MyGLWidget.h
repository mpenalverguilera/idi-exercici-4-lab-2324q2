// MyGLWidget.h
#include "LL4GLWidget.h"
#include <QTimer>

class MyGLWidget : public LL4GLWidget {
  Q_OBJECT
  public:
    MyGLWidget(QWidget *parent=0);
    ~MyGLWidget();
  public slots:
    //avançar cotxe
    void forwardSlot();
  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    /*Widget*/
    //Inicialitzar
    virtual void initializeGL ();
    //Shaders
    virtual void carregaShaders();
    //Pintar
    virtual void paintGL ();
    /*Models*/
    //Transformadores
    virtual void modelTransformTerra ();
    virtual void modelTransformRoad (glm::vec3 pos, float angle);
    virtual void modelTransformPipe ();
    virtual void modelTransformCar (float radi, float angle);
    //Moviment
    //avançar cotxe
    virtual void forward();
    //Materials
    virtual void iniMaterialTerra ();


    /*Camera*/
    virtual void iniEscena ();
    virtual void viewTransform();
    
    /*LLums*/
    virtual void iniLlum();
    glm::mat4 scaCarToSco(float radi, float angle);
    virtual void actCarFocus();

  private:
    int printOglError(const char file[], int line, const char func[]);
    /*Camera*/
    float angleX = float(M_PI/6); //30º
    
    /*Cars*/
    //Atributs auxiliars
    float angleRed = 0;
    float angleGreen = 0;
    float radiRed = 6.5;
    float radiGreen = 9;
    //Color
    GLuint matModifierLoc;
    glm::vec3 defaultModifier = glm::vec3(1.0);
    glm::vec3 redModifier = glm::vec3(1, 0, 0);
    glm::vec3 greenModifier = glm::vec3(0, 1, 0);
    //Moviment
    QTimer timer;
    bool animat = false;

    /*Llum*/
    GLuint colorFocusCameraLoc, greenCarFocusToSCOLoc, redCarFocusToSCOLoc;
    glm::vec3 colorFocusCamera = glm::vec3(0.8);
    glm::mat4 greenCarFocusToSCO, redCarFocusToSCO;

};
