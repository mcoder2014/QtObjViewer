#include "OpenGLWidget.h"
#include <QDebug>
#include "Vertex.h"
#include <QKeyEvent>
#include <QOpenGLShaderProgram>
#include "Input.h"

// Front Verticies
#define VERTEX_FTR Vertex( QVector3D( 0.5f,  0.5f,  0.5f), QVector3D( 1.0f, 0.0f, 0.0f ) )
#define VERTEX_FTL Vertex( QVector3D(-0.5f,  0.5f,  0.5f), QVector3D( 0.0f, 1.0f, 0.0f ) )
#define VERTEX_FBL Vertex( QVector3D(-0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 1.0f ) )
#define VERTEX_FBR Vertex( QVector3D( 0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 0.0f ) )

// Back Verticies
#define VERTEX_BTR Vertex( QVector3D( 0.5f,  0.5f, -0.5f), QVector3D( 1.0f, 1.0f, 0.0f ) )
#define VERTEX_BTL Vertex( QVector3D(-0.5f,  0.5f, -0.5f), QVector3D( 0.0f, 1.0f, 1.0f ) )
#define VERTEX_BBL Vertex( QVector3D(-0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 0.0f, 1.0f ) )
#define VERTEX_BBR Vertex( QVector3D( 0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 1.0f, 1.0f ) )

// Create a colored cube
static const Vertex sg_vertexes[] = {
    // Face 1 (Front)
    VERTEX_FTR, VERTEX_FTL, VERTEX_FBL,
    VERTEX_FBL, VERTEX_FBR, VERTEX_FTR,
    // Face 2 (Back)
    VERTEX_BBR, VERTEX_BTL, VERTEX_BTR,
    VERTEX_BTL, VERTEX_BBR, VERTEX_BBL,
    // Face 3 (Top)
    VERTEX_FTR, VERTEX_BTR, VERTEX_BTL,
    VERTEX_BTL, VERTEX_FTL, VERTEX_FTR,
    // Face 4 (Bottom)
    VERTEX_FBR, VERTEX_FBL, VERTEX_BBL,
    VERTEX_BBL, VERTEX_BBR, VERTEX_FBR,
    // Face 5 (Left)
    VERTEX_FBL, VERTEX_FTL, VERTEX_BTL,
    VERTEX_FBL, VERTEX_BTL, VERTEX_BBL,
    // Face 6 (Right)
    VERTEX_FTR, VERTEX_FBR, VERTEX_BBR,
    VERTEX_BBR, VERTEX_BTR, VERTEX_FTR
};

#undef VERTEX_BBR
#undef VERTEX_BBL
#undef VERTEX_BTL
#undef VERTEX_BTR

#undef VERTEX_FBR
#undef VERTEX_FBL
#undef VERTEX_FTL
#undef VERTEX_FTR


OpenGLWidget::OpenGLWidget()
    :QOpenGLWidget(), QOpenGLFunctions()
{
    m_transform.translate(0.0f, 0.0f, -5.0f);
    this->grabKeyboard();
//    this->m_openMesh = NULL;
    this->m_mesh = NULL;
}

OpenGLWidget::~OpenGLWidget()
{
    this->teardownGL();
}

void OpenGLWidget::initializeGL()
{
    // initialize opengl
    initializeOpenGLFunctions();
    //    connect(context(), SIGNAL(aboutToBeDestroyed()),
    //            this, SLOT(teardownGL()), Qt::DirectConnection);
    connect(this, SIGNAL(frameSwapped()),
            this, SLOT(update()));
    this->printContextInformation();

    // 设置全局的gl设置
    glEnable(GL_CULL_FACE);                 // cull face
    glEnable(GL_DEPTH_TEST);                // 深度测试
    glLineWidth(10.f);
    glClearDepthf(1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    //  Application-specific initialization
    {
        // Create Shader (Do not release until VAO is created)
        m_program = new QOpenGLShaderProgram();
        m_program->addShaderFromSourceFile(
                    QOpenGLShader::Vertex,
                    ":/shaders/resources/shaders/simple.vert");
        m_program->addShaderFromSourceFile(
                    QOpenGLShader::Fragment,
                    ":/shaders/resources/shaders/simple.frag");
        m_program->link();
        m_program->bind();

        // Cache Uniform Location
        this->u_modelToWorld = m_program->uniformLocation("modelToWorld");
        this->u_worldToCamera = m_program->uniformLocation("worldToCamera");
        this->u_cameraToView = m_program->uniformLocation("cameraToView");

        m_program->release();       // realse unbind
    }

}

///
/// \brief OpenGLWidget::resizeGL
/// \param w
/// \param h
///
void OpenGLWidget::resizeGL(int w, int h)
{
    qDebug() << "openglWidget resize: " << w
             << " "<< h;
    m_projection.setToIdentity();
    m_projection.perspective(45.0f, w / float(h), 1.0f, 1000.0f);
}

///
/// \brief OpenGLWidget::paintGL
///
void OpenGLWidget::paintGL()
{
    //    qDebug() << "paintGL";

    // clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render using our shader
    m_program->bind();
    m_program->setUniformValue(u_worldToCamera, m_camera.toMatrix());
    m_program->setUniformValue(u_cameraToView, m_projection);
    // The calls to setUniformValue() allow us to
    // update the value of worldToView and modelToWorld.

    // 渲染模型
    if(this->m_openMesh != NULL)
    {
        m_program->setUniformValue(u_modelToWorld, m_transform.toMatrix());
//        m_openMesh->draw();
    }

    m_program->release();
}

void OpenGLWidget::loadMesh()
{

}

///
/// \brief OpenGLWidget::teardownGL
///
void OpenGLWidget::teardownGL()
{
    // Actually destroy our OpenGL information
    delete m_program;

    qDebug() << "teardown gl";
}

void OpenGLWidget::update()
{
    // Update input
    Input::update();


    static const float transSpeed = 0.2f;
    static const float rotSpeed = 0.5f;
    // Camera Transformation
    if(Input::buttonPressed(Qt::RightButton))
    {


        // handle rotations
        m_camera.rotate(
                    -rotSpeed * Input::mouseDelta().x(),
                    Camera3D::LocalUp);
        m_camera.rotate(
                    -rotSpeed * Input::mouseDelta().y(),
                    m_camera.right());
    }

        // handle translations
        QVector3D translation;
        if(Input::keyPressed(Qt::Key_W))
        {
            translation += m_camera.forward();
        }
        if (Input::keyPressed(Qt::Key_S))
        {
            translation -= m_camera.forward();
        }
        if (Input::keyPressed(Qt::Key_A))
        {
            translation -= m_camera.right();
        }
        if (Input::keyPressed(Qt::Key_D))
        {
            translation += m_camera.right();
        }
        if (Input::keyPressed(Qt::Key_Q))
        {
            translation -= m_camera.up();
        }
        if (Input::keyPressed(Qt::Key_E))
        {
            translation += m_camera.up();
        }
        m_camera.translate(transSpeed * translation);


    // update instance information
    m_transform.rotate(1.0f, QVector3D(0.4f, 0.3f, 0.3f));

    // Schedule a redraw
    QOpenGLWidget::update();
}

void OpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat())
    {
        event->ignore();
    }
    else
    {
        Input::registerKeyPress(event->key());
    }
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat())
    {
        event->ignore();
    }
    else
    {
        Input::registerKeyRelease(event->key());
    }
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    Input::registerMousePress(event->button());
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Input::registerMouseRelease(event->button());
}

///
/// \brief OpenGLWidget::printContextInformation
///
void OpenGLWidget::printContextInformation()
{
    QString glType;
    QString glVersion;
    QString glProfile;

    // Get Version Information
    glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

    // Get Profile Information
#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
    switch (format().profile())
    {
    CASE(NoProfile);
    CASE(CoreProfile);
    CASE(CompatibilityProfile);
    }
#undef CASE

    // qPrintable() will print our QString w/o quotes around it.
    qDebug() << "\n print ContextInformation" << glType << glVersion << "(" << glProfile << ")";
}
