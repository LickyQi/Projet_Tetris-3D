#ifndef VIEWER_HPP
#define VIEWER_HPP

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QMatrix4x4>
#include <QtGlobal>
#include "game.hpp"
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QGLBuffer>


class Viewer : public QGLWidget {
    
    Q_OBJECT

public:
    Viewer(const QGLFormat& format,QWidget *parent = nullptr);
    virtual ~Viewer();
    
    // windows' size
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    int numCube;

    void newgame();
    bool isfirst;
    

    QTimer* mTimer2;

    //implementing gravity
    QTimer* mTimer3;// for gravity, each tick rotates once
    int rotatevalue;

    //implementing holding multiple mouse buttons, rotating;
    int persistence;

    //implementing reset
    int rotatex;
    int rotatey;
    int rotatez;

    Game* game;

    // implement colour
    int colour[300];

private slots:
    void onestep();
    void rotate();

protected:
    // Called when GL is first initialized
    virtual void initializeGL();
    // Called when our window needs to be redrawn
    virtual void paintGL();
    // Called when the window is resized (formerly on_configure_event)
    virtual void resizeGL(int width, int height);
    virtual void mousePressEvent(QMouseEvent *event);

private:

    QMatrix4x4 getCameraMatrix();
    void rotateWorld(float angle, float x, float y, float z);

    QOpenGLBuffer mVertexBufferObject;
    QOpenGLVertexArrayObject mVertexArrayObject;

    int mVertexLocation;
    int mMvpMatrixLocation;

    QMatrix4x4 mPerspMatrix;
    QMatrix4x4 mModelMatrices[300];
    QMatrix4x4 mTransformMatrix;
    
    QTimer* mTimer;
    QGLShaderProgram mProgram;
};

#endif
