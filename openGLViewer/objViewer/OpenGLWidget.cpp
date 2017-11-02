#include "OpenGLWidget.h"
#include <QDebug>
#include "Vertex.h"
#include <QKeyEvent>
#include <QOpenGLShaderProgram>
#include "Input.h"
#include "objLoader.h"


// Front Verticies
#define VERTEX_FTR Vertex( QVector3D( 0.5f,  0.5f,  0.5f), QVector3D( 1.0f, 0.0f, 0.0f ),QVector2D(0,0) )
#define VERTEX_FTL Vertex( QVector3D(-0.5f,  0.5f,  0.5f), QVector3D( 0.0f, 1.0f, 0.0f ),QVector2D(0,0) )
#define VERTEX_FBL Vertex( QVector3D(-0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 1.0f ),QVector2D(1,0) )
#define VERTEX_FBR Vertex( QVector3D( 0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 0.0f ),QVector2D(0,1) )

// Back Verticies
#define VERTEX_BTR Vertex( QVector3D( 0.5f,  0.5f, -0.5f), QVector3D( 1.0f, 1.0f, 0.0f ),QVector2D(0,0) )
#define VERTEX_BTL Vertex( QVector3D(-0.5f,  0.5f, -0.5f), QVector3D( 0.0f, 1.0f, 1.0f ),QVector2D(0,0) )
#define VERTEX_BBL Vertex( QVector3D(-0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 0.0f, 1.0f ),QVector2D(1,0) )
#define VERTEX_BBR Vertex( QVector3D( 0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 1.0f, 1.0f ),QVector2D(0,1) )

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
    m_camera.translate(0,0,200);        // 设置相机初始位置
    this->grabKeyboard();               // 跟踪按键
    this->m_mesh = NULL;                // 初始化场景中的mesh
    this->m_cube = NULL;

}

OpenGLWidget::~OpenGLWidget()
{
    this->teardownGL();
}

void OpenGLWidget::initializeGL()
{
    // initialize opengl
    initializeOpenGLFunctions();
//    connect(this, SIGNAL(frameSwapped()),
//            this, SLOT(update()));          // 动画渲染
    connect(&this->timer, SIGNAL(timeout()),
            this,SLOT(update()));           // 链接信号槽
    this->printContextInformation();        // 输出系统配置信息

    // 设置全局的gl设置
    glEnable(GL_CULL_FACE);                 // cull face
    glEnable(GL_DEPTH_TEST);                // 深度测试
//    glLineWidth(10.f);
    glClearDepthf(1.0f);
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);

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

//        this->createCube();         // 创建cube


        // Create Vertex Array Object
        m_object.create();
        m_object.bind();
        // Create Buffer (Do not release until VAO is created)
        m_vertex.create();
        m_vertex.bind();
        m_vertex.setUsagePattern(QOpenGLBuffer::StaticDraw);
        m_vertex.allocate(sizeof(sg_vertexes));
        Vertex *data = static_cast<Vertex*>(m_vertex.map(QOpenGLBuffer::WriteOnly));
        for(int i = 0; i<sizeof(sg_vertexes)/sizeof(Vertex); i++)
        {
            data[i] = sg_vertexes[i];
        }
        m_vertex.unmap();

        glVertexAttribPointer(
                    0,
                    Vertex::PositionTupleSize,
                    GL_FLOAT,
                    GL_FALSE,
                    Vertex::stride(),
                    (void*)Vertex::positionOffset());

        glVertexAttribPointer(
                    1,
                    Vertex::ColorTupleSize,
                    GL_FLOAT,
                    GL_FALSE,
                    Vertex::stride(),
                    (void*)Vertex::colorOffset());

        glVertexAttribPointer(
                    2,
                    Vertex::TexCoordsTupleSize,
                    GL_FLOAT,
                    GL_FALSE,
                    Vertex::stride(),
                    (void*)Vertex::texCoordsOffset());
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        // Release (unbind) all
        m_object.release();
        m_vertex.release();
        m_program->release();       // realse unbind
    }

    timer.start(17);                // 每20ms 唤醒一次
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
    m_projection.perspective(45.0f, w / float(h), 0.1f, 2000.0f);
}

///
/// \brief OpenGLWidget::paintGL
///
void OpenGLWidget::paintGL()
{
//        qDebug() << "paintGL";

    // clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind();
    for(int i = 0; i < this->meshes.size(); i++)
    {
        CustomMesh* mesh = this->meshes[i];
        if(mesh == NULL)
            continue;
        m_program->setUniformValue(u_worldToCamera, m_camera.toMatrix());               // worldToCamera
        m_program->setUniformValue(u_cameraToView, m_projection);                       // cameraToView
        m_program->setUniformValue(u_modelToWorld, mesh->transform()->toMatrix());      // modelToWorld

        mesh->draw(m_program);

    }
    m_program->release();

}

///
/// \brief OpenGLWidget::loadMesh
/// \param filePath
///
void OpenGLWidget::loadMesh(QString filePath)
{
    this->makeCurrent();        // 其他函数中必须先调用makeCurrent才可以
    objLoader loader;


    m_program->bind();
    CustomMesh *mesh = loader.loadMesh(filePath);
    this->meshes.append(mesh);  // 追加到数组队列的最后一个
    m_program->release();
}

void OpenGLWidget::cleanScene()
{
    this->meshes.clear();
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
    static const float rotSpeed = 0.3f;
    static const float cameraSpeed = 0.05f;

    // Camera ---让中间的物体旋转
    if(Input::buttonPressed(Qt::LeftButton))
    {

        // handle rotations
//        m_camera.rotate(
//                    -rotSpeed * Input::mouseDelta().x(),
//                    Camera3D::LocalUp);
//        m_camera.rotate(
//                    -rotSpeed * Input::mouseDelta().y(),
//                    m_camera.right());

        for(int i= 0; i < this->meshes.size(); i++)
        {
            CustomMesh *mesh = meshes[i];

            mesh->transform()->rotate(-rotSpeed * Input::mouseDelta().x(),
                                      Transform3D::LocalUp);

            mesh->transform()->rotate(-rotSpeed * Input::mouseDelta().y(),
                                      mesh->transform()->right());
        }

    }
    else if(Input::buttonPressed(Qt::RightButton))
    {
        m_camera.translate(cameraSpeed * -Input::mouseDelta().x() * m_camera.right());
        m_camera.translate(cameraSpeed * Input::mouseDelta().y() * m_camera.up());
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

void OpenGLWidget::wheelEvent(QWheelEvent *event)
{

    static const float transSpeed = 0.2f;

    QVector3D translation;
    if(event->delta() > 0)
    {
        translation += m_camera.forward();
    }
    else
    {
        translation -= m_camera.forward();
    }
    m_camera.translate(transSpeed * translation);

    QOpenGLWidget::wheelEvent(event);
}


///
/// \brief OpenGLWidget::createCube
/// 创建一个立方体验证
///
void OpenGLWidget::createCube()
{
    this->m_cube = new CustomMesh();
    this->m_cube->createCube(QVector3D(2,0,0),0.3);

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
