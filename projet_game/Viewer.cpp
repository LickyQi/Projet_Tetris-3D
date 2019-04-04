#include <QtWidgets>
#include <QtOpenGL>
#include <QVector3D>
#include "Viewer.hpp"
#include <iostream>
#include <math.h>
#include <unistd.h>


Viewer::Viewer(const QGLFormat& format,QWidget *parent)
    : QGLWidget(format,parent)
    , mVertexBufferObject(QOpenGLBuffer::VertexBuffer)
    , mVertexArrayObject(this)
{   
    isfirst = true;
    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(update()));
    mTimer->start(1000/30);

    mTimer3 = new QTimer(this);
    connect(mTimer3, SIGNAL(timeout()), this, SLOT(rotate()));

}

Viewer::~Viewer() {

}

QSize Viewer::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize Viewer::sizeHint() const {
    return QSize(300, 600);
}

void Viewer::initializeGL() {

    QGLFormat glFormat = QGLWidget::format();
    if (!glFormat.sampleBuffers()) {
        std::cerr << "Could not enable sample buffers." << std::endl;
        return;
    }

    glClearColor(0.1, 0.1, 0.1, 0.0);

    if (!mProgram.addShaderFromSourceFile(QGLShader::Vertex, "/Users/liqi/Desktop/projet_game/shader.vert")) {
        std::cerr << "Cannot load vertex shader." << std::endl;
        exit(0);
    }

    if (!mProgram.addShaderFromSourceFile(QGLShader::Fragment, "/Users/liqi/Desktop/projet_game/shader.frag")) {
        std::cerr << "Cannot load fragment shader." << std::endl;
        exit(0);
    }

    if ( !mProgram.link() ) {
        std::cerr << "Cannot link shaders." << std::endl;
        exit(0);
    }


    // Oh this..
    float cubeData[] = {
        //  X     Y     Z
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, -1.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, -1.0f,
        0.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 0.0f, -1.0f,
        0.0f, 1.0f, -1.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, -1.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, -1.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, -1.0f,
        0.0f, 0.0f, 0.0f,
    };

    mVertexArrayObject.create();
    mVertexArrayObject.bind();

    mVertexBufferObject.create();
    mVertexBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);

    if (!mVertexBufferObject.bind()) {
        std::cerr << "could not bind vertex buffer to the context." << std::endl;
        return;
    }
    mVertexBufferObject.allocate(cubeData, 36 * 3 * sizeof(float));

    mProgram.enableAttributeArray("vert");
    mProgram.setAttributeBuffer("vert", GL_FLOAT, 0, 3);

    //mPerspMatrixLocation = mProgram.uniformLocation("cameraMatrix");
    mMvpMatrixLocation = mProgram.uniformLocation("mvpMatrix");


    // Get location value for "fragColor" GLuint
    // Set uniform using that location.
    GLint colorLocation = mProgram.uniformLocation("frag_color");
    mProgram.setUniformValue(colorLocation, 0.1f, 0.1f, 0.1f);


}

void Viewer::paintGL() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mVertexArrayObject.bind();
    mProgram.bind();
    GLint colorLocation = mProgram.uniformLocation("frag_color");

    for (int i = 0; i < 300; i++) {
        mModelMatrices[i] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
    }

    // backgroud
    int count = 1;
    for (int i = 1; i < 21; i++) {
        for(int j = 0; j < 10; j++){
            mModelMatrices[count].translate(j-5,-10 + i,0);
            colour[count] = -1;
            count += 1;
        }
    }

    for (int i = 0; i < 300; i++) {
        mProgram.setUniformValue(mMvpMatrixLocation, getCameraMatrix() * mModelMatrices[i]);
        mProgram.setUniformValue(colorLocation, 1.0f, 1.0f, 1.0f);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        glDrawArrays(GL_LINE_LOOP, 0, 6);
    }

    if (!isfirst) {
        for (int i = 0; i < 300; i++) {
            mModelMatrices[i] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
        }
        numCube = 0;

        //finished the well
        for (int i = 23; i >= 0; i--) {
            for (int j = 9; j >= 0; j--) {
                if (game->get(i,j) != -1) {
                    numCube++;
                    // we have: i - y = 9; j + x = 4
                    mModelMatrices[numCube-1].translate(4-j,i-9,0);
                    colour[numCube-1] = game->get(i,j);
                }
            }
        }
    }

    for (int i = 0; i < numCube; i++) {
        mProgram.setUniformValue(mMvpMatrixLocation, getCameraMatrix() * mModelMatrices[i]);
        switch(colour[i]) {
        case -1:
            mProgram.setUniformValue(colorLocation, 0.0f, 0.0f, 0.0f);
            break;
        case 0:
            mProgram.setUniformValue(colorLocation, 0.0f, 0.0f, 1.0f);
            break;
        case 1:
            mProgram.setUniformValue(colorLocation, 0.0f, 1.0f, 0.0f);
            break;
        case 2:
            mProgram.setUniformValue(colorLocation, 0.0f, 1.0f, 1.0f);
            break;
        case 3:
            mProgram.setUniformValue(colorLocation, 1.0f, 0.0f, 0.0f);
            break;
        case 4:
            mProgram.setUniformValue(colorLocation, 1.0f, 0.0f, 1.0f);
            break;
        case 5:
            mProgram.setUniformValue(colorLocation, 1.0f, 1.0f, 0.0f);
            break;
        case 6:
            mProgram.setUniformValue(colorLocation, 1.0f, 1.0f, 1.0f);
            break;
        default:
            break;
        }
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    mProgram.release();

}

void Viewer::resizeGL(int width, int height) {
    if (height == 0) {
        height = 1;
    }

    mPerspMatrix.setToIdentity();
    mPerspMatrix.perspective(60.0, (float) width / (float) height, 0.001, 1000);

    glViewport(0, 0, width, height);
}

QMatrix4x4 Viewer::getCameraMatrix() {
    QMatrix4x4 vMatrix;

    QMatrix4x4 cameraTransformation;
    QVector3D cameraPosition = cameraTransformation * QVector3D(-5, -5, 20.0);
    //QVector3D cameraPosition = cameraTransformation * QVector3D(0, 0, 20.0);
    QVector3D cameraUpDirection = cameraTransformation * QVector3D(0, 1, 0);

    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);

    return mPerspMatrix * vMatrix * mTransformMatrix;
}

void Viewer::rotateWorld(float angle, float x, float y, float z) {
    mTransformMatrix.rotate(angle, x, y, z);
}

void Viewer::newgame() {
    // set number of cubes to be 0
    numCube = 0;
    // reset the mModelMatrtices
    for (int i = 0; i < 300; i++) {
        mModelMatrices[i] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
    }
    //finished the well
    game = new Game(10,20);
    for (int i = 23; i >= 0; i--) {
        for (int j = 9; j >= 0; j--) {
            if (game->get(i,j) != -1) {
                numCube++;
                // we have: i - y = 9; j + x = 4
                mModelMatrices[numCube-1].translate(4-j,i-9,0);
            }
        }
    }
    update();

    if (isfirst){
        mTimer2 = new QTimer(this);
        connect(mTimer2, SIGNAL(timeout()), this, SLOT(onestep()));
        mTimer2->start(300);
        isfirst = false;
    }
}


void Viewer::onestep() {
    game->tick();
    // transformations handled in GL code
    update();
}

void Viewer::rotate() {

    // use a switch would be better
    if (persistence == 1) {
        rotateWorld(rotatevalue, 1.0, 0.0, 0.0);
        rotatex += rotatevalue;
    } else if (persistence == 2) {
        rotateWorld(rotatevalue, 0.0, 0.0, 1.0);
        rotatez += rotatevalue;
    } else if (persistence == 3) {
        rotateWorld(rotatevalue, 0.0, 0.0, 1.0);
        rotatex += rotatevalue;
        rotateWorld(rotatevalue, 1.0, 0.0, 0.0);
        rotatez += rotatevalue;
    } else if (persistence == 4) {
        rotateWorld(rotatevalue, 0.0, 1.0, 0.0);
        rotatey += rotatevalue;
    } else if (persistence == 5) {
        rotateWorld(rotatevalue, 1.0, 0.0, 0.0);
        rotateWorld(rotatevalue, 0.0, 1.0, 0.0);
        rotatex += rotatevalue;
        rotatey += rotatevalue;
    } else if (persistence == 6) {
        rotateWorld(rotatevalue, 0.0, 1.0, 0.0);
        rotateWorld(rotatevalue, 0.0, 0.0, 1.0);
        rotatey += rotatevalue;
        rotatez += rotatevalue;
    } else if (persistence == 7) {
        rotateWorld((rotatevalue), 1.0, 0.0, 0.0);
        rotateWorld((rotatevalue), 0.0, 1.0, 0.0);
        rotateWorld((rotatevalue), 0.0, 0.0, 1.0);
        rotatex += rotatevalue;
        rotatey += rotatevalue;
        rotatez += rotatevalue;
    }
}
void Viewer::mousePressEvent(QMouseEvent *event){
    int x = event->x();
    int y = event->y();
    qDebug() << x <<endl;
    qDebug() << y <<endl;
    if (x  and  y){
        newgame();
    }
}
