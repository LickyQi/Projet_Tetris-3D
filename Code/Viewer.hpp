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


// @author Qi LI

// L'interface du jeu
class Viewer : public QGLWidget {
    
    Q_OBJECT

public:
    // Constructor
    Viewer(const QGLFormat& format,QWidget *parent = nullptr);
    // Destructor
    virtual ~Viewer();
    
    // windows' size
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    // The position of the cube in the array
    int numCube;

    // Initialize all cube's information
    void newgame();
    // Flag for start the timer
    bool isfirst;
    
    // The timer for a piece move one step
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

    // Declare a game
    Game* game;

    // implement colour
    int colour[300];

private slots:
    // Advance the game by one tick.
    void onestep();
    // Rotate the tetrismino(In fact, here we only do translation transformation)
    void rotate();

protected:
    // Called when GL is first initialized
    virtual void initializeGL();
    // Called when our window needs to be redrawn
    virtual void paintGL();
    // Called when the window is resized (formerly on_configure_event)
    virtual void resizeGL(int width, int height);
    // Function for mouse interaction management to start the game after we click on the screen of game
    virtual void mousePressEvent(QMouseEvent *event);

private:

    // Create a matrix for camera that supports transformation functions
    QMatrix4x4 getCameraMatrix();
    // Function for make rotation
    void rotateWorld(float angle, float x, float y, float z);

    // Create a VertexBufferObject
    QOpenGLBuffer mVertexBufferObject;
    // Create a VertexArrayObject
    QOpenGLVertexArrayObject mVertexArrayObject;

    // The location of vertex
    int mVertexLocation;
    // The final transformation matrix
    int mMvpMatrixLocation;

    // A matrix representing the visual range
    QMatrix4x4 mPerspMatrix;
    // A matrix that holds cube information
    QMatrix4x4 mModelMatrices[300];
    // A matrix for transformation
    QMatrix4x4 mTransformMatrix;
    
    // The timer to update the interface
    QTimer* mTimer;
    // Create a object for shader program
    QGLShaderProgram mProgram;
};

#endif
