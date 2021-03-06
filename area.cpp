#include "area.h"
#include <QPainter>
#include <QTimerEvent>
#include <iostream>
#include <math.h>

using namespace std;

Area::Area(QWidget *parent):QWidget(parent)
{

    gran = new int*[6];
    for (int i = 0; i < 6; i++) {
        gran[i] = new int[6];
    }
    gran[0][0] = 0;
    gran[0][1] = 1;
    gran[0][2] = 2;
    gran[0][3] = 3;
    gran[0][4] = 0;
    gran[0][5] = -1;
    gran[1][0] = 0;
    gran[1][1] = 1;
    gran[1][2] = 5;
    gran[1][3] = 4;
    gran[1][4] = 0;
    gran[1][5] = -1;
    gran[2][0] = 1;
    gran[2][1] = 2;
    gran[2][2] = 6;
    gran[2][3] = 5;
    gran[2][4] = 1;
    gran[2][5] = -1;
    gran[3][0] = 2;
    gran[3][1] = 3;
    gran[3][2] = 7;
    gran[3][3] = 6;
    gran[3][4] = 2;
    gran[3][5] = -1;
    gran[4][0] = 3;
    gran[4][1] = 0;
    gran[4][2] = 4;
    gran[4][3] = 7;
    gran[4][4] = 3;
    gran[4][5] = -1;
    gran[5][0] = 4;
    gran[5][1] = 5;
    gran[5][2] = 6;
    gran[5][3] = 7;
    gran[5][4] = 4;
    gran[5][5] = -1;

    setFixedSize(QSize(500, 500));
}

void Area::showEvent(QShowEvent *) {
    update();
}

void Area::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setPen(Qt::red);

Coords** shapeCoords = new Coords*[8];
    shapeCoords[0] = new Coords(0, 0, 0);
    shapeCoords[1] = new Coords(2, 0, 0);
    shapeCoords[2] = new Coords(2, 2, 0);
    shapeCoords[3] = new Coords(0, 2, 0);
    shapeCoords[4] = new Coords(0, 0, 2);
    shapeCoords[5] = new Coords(2, 0, 2);
    shapeCoords[6] = new Coords(2, 2, 2);
    shapeCoords[7] = new Coords(0, 2, 2);

    Connection** shapeConnections = new Connection*[12];
    shapeConnections[0] = new Connection(0, 1);
    shapeConnections[1] = new Connection(1, 2);
    shapeConnections[2] = new Connection(2, 3);
    shapeConnections[3] = new Connection(3, 0);
    shapeConnections[4] = new Connection(0, 4);
    shapeConnections[5] = new Connection(1, 5);
    shapeConnections[6] = new Connection(2, 6);
    shapeConnections[7] = new Connection(3, 7);
    shapeConnections[8] = new Connection(4, 5);
    shapeConnections[9] = new Connection(5, 6);
    shapeConnections[10] = new Connection(6, 7);
    shapeConnections[11] = new Connection(7, 4);

    Shape* shape = new Shape(8, shapeCoords, 12, shapeConnections);
    float** transformMatrix = new float*[4];
    float** W = new float*[4];

    for (int i = 0; i < 4; i++) {
        transformMatrix[i] = new float[4];
        W[i] = new float[6];
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                transformMatrix[i][j] = 1;
            }
            else
                transformMatrix[i][j] = 0;
        }
    }

    for (int j = 0; j < 6; j++) {
        W[0][j] = getA(shape->getCoords(gran[j][1]), shape->getCoords(gran[j][0]), shape->getCoords(gran[j][2]));
        W[1][j] = getB(shape->getCoords(gran[j][1]), shape->getCoords(gran[j][0]), shape->getCoords(gran[j][2]));
        W[2][j] = getC(shape->getCoords(gran[j][1]), shape->getCoords(gran[j][0]), shape->getCoords(gran[j][2]));
        W[3][j] = getD(W[0][j], W[1][j], W[2][j], shape->getCoords(gran[j][0]));
    }

    float centerX = 0;
    float centerY = 0;
    float centerZ = 0;

    for (int i = 0; i < 8; i++) {
        centerX += shape->getCoords(i)->getX();
        centerY += shape->getCoords(i)->getY();
        centerZ += shape->getCoords(i)->getZ();
    }

    centerX /= 8;
    centerY /= 8;
    centerZ /= 8;

    int sizeGV = 0;
    float **granVid = new float *[6];
    for(int i = 0; i < 6; i++) {
        granVid[i] = new float[6];
    }

    for (int i = 0; i < 6; i++) {
        float A = W[0][i];
        float B = W[1][i];
        float C = W[2][i];
        float D = W[3][i];

        if (A*centerX + B*centerY + C*centerZ + D < 0) {
            W[0][i] = -W[0][i];
            W[1][i] = -W[1][i];
            W[2][i] = -W[2][i];
            W[3][i] = -W[3][i];
        }
    }

    float _viewX = viewX;
    float _viewY = viewY;
    float _viewZ = viewZ;

    Coords* view = new Coords(_viewX, _viewY, _viewZ);

    float cos1 = cos(M_PI * alpha/ 180);
    float sin1 = sin(M_PI * alpha / 180);

    rotateShapeZ(transformMatrix, cos1, sin1);
    float** superCoords = new float*[4];
    for(int i = 0; i < 4 ; i++) {
        superCoords[i] = new float[1];
    }

    superCoords[0][0] = viewX;
    superCoords[0][1] = viewY;
    superCoords[0][2] = viewZ;
    superCoords[0][3] = 1;


    matrixMultiplication(superCoords, 1, 4, transformMatrix, 4);

    view->setX(superCoords[0][0]);
    view->setY(superCoords[0][1]);
    view->setZ(superCoords[0][2]);

    for (int i = 0; i < 6; i++) {
        float A = W[0][i];
        float B = W[1][i];
        float C = W[2][i];
        float D = W[3][i];
        float value = A*view->getX() + B*view->getY() + C*view->getZ() + D;
        if (value < 0) {
            for (int j = 0; j < 6; j++) {
                granVid[sizeGV][j] = gran[i][j];
            }
            sizeGV++;
        }
    }

    Coords* translateCoords = new Coords(-view->getX(), -view->getY(), -view->getZ());

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                transformMatrix[i][j] = 1;
            }
            else
                transformMatrix[i][j] = 0;
        }
    }

    translateShape(transformMatrix, translateCoords);

    scaleShape(transformMatrix, -1, 1, 1);

    rotateShapeX(transformMatrix, 0, 1);

    float d = sqrt(view->getX() * view->getX() + view->getY() * view->getY());

    float cosU, sinU;

    if (d == 0) {
        cosU = 1;
        sinU = 0;
    }
    else {
        cosU = view->getY() / d;
        sinU = view->getX() / d;
    }

    rotateShapeY(transformMatrix, cosU, sinU);

    float s = sqrt(d*d + view->getZ() * view->getZ());

    float cosW = d / s;
    float sinW = view->getZ() / s;

    rotateShapeX(transformMatrix, cosW, sinW);

    setNewCoords(shape, transformMatrix);

    float r = sqrt(view->getX() * view->getX() + view->getY() * view->getY() + view->getZ() * view->getZ());

    for (int i = 0; i < 8; i++) {
        Coords* first = shape->getCoords(i);

        float P = 5;
        float newXF = first->getX()*r/first->getZ();
//        float newXF = first->getX();
        newXF = newXF / P * 250 + 250;
        float newYF = first->getY()*r/first->getZ();
//        float newYF = first->getY();
        newYF =  newYF / P * 250 + 250;
        shape->SetCoords(i, newXF, 500 - newYF, first->getZ());
    }

    for (int i = 0; i < 4; i++) {
        delete [] transformMatrix[i];
//        delete [] superCoords[i];
    }
    delete [] transformMatrix;
//    delete [] superCoords;
    delete view;

    for (int i = 0; i < sizeGV; i++) {
        for (int j = 0; granVid[i][j+1] != -1; j++) {
            Coords* first = shape->getCoords(granVid[i][j]);
            Coords* second = shape->getCoords(granVid[i][j+1]);
            painter.drawLine(first->getX(), first->getY(), second->getX(), second->getY());
        }
    }
    delete shape;

    for (int i = 0; i < 6; i++) {
        if (i < 4)
            delete [] W[i];
        delete [] granVid[i];
    }
    delete [] W;
    delete [] granVid;
}

void Area::timerEvent(QTimerEvent *) {
    alpha += 1;
    if (alpha > 180)
        killTimer(myTimer);
    else
        update();
}

void Area::hideEvent(QHideEvent *) {
}

void Area::start_prog() {
    myTimer = startTimer(90);
}

void Area::setNewCoords(Shape *shape, float** changeMatrix) {
    int n = shape->getN();
    Coords **coords = new Coords*[n];
    float **newMatrix = new float*[1];
    newMatrix[0] = new float[4];
    for (int i = 0; i < n; i++) {
        coords[i] = shape->getCoords(i);
    }
    for (int i = 0; i < n; i++) {
        newMatrix[0][0] = coords[i]->getX();
        newMatrix[0][1] = coords[i]->getY();
        newMatrix[0][2] = coords[i]->getZ();
        newMatrix[0][3] = 1;
        matrixMultiplication(newMatrix, 1, 4, changeMatrix, 4);
        shape->SetCoords(i, newMatrix[0][0], newMatrix[0][1], newMatrix[0][2]);
    }
    delete [] newMatrix[0];
    delete [] newMatrix;
    delete [] coords;
}

void Area::translateShape(float** oldMatrix, Coords* value) {
    float** newMatrix = new float*[4];
    for (int i = 0; i < 4; i++) {
        newMatrix[i] = new float[4];
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                newMatrix[i][j] = 1;
            }
            else
                newMatrix[i][j] = 0;
        }
    }
    newMatrix[3][0] = value->getX();
    newMatrix[3][1] = value->getY();
    newMatrix[3][2] = value->getZ();
    matrixMultiplication(oldMatrix, 4, 4, newMatrix, 4);
    for (int i = 0; i < 4; i++) {
        delete [] newMatrix[i];
    }
    delete [] newMatrix;
}

void Area::rotateShapeX(float** oldMatrix, float cosV, float sinV) {
    float** newMatrix = new float*[4];
    for (int i = 0; i < 4; i++) {
        newMatrix[i] = new float[4];
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                newMatrix[i][j] = 1;
            }
            else
                newMatrix[i][j] = 0;
        }
    }
    newMatrix[1][1] = cosV;
    newMatrix[1][2] = -sinV;
    newMatrix[2][1] = sinV;
    newMatrix[2][2] = cosV;
    matrixMultiplication(oldMatrix, 4, 4, newMatrix, 4);
    for (int i = 0; i < 4; i++) {
        delete [] newMatrix[i];
    }
    delete [] newMatrix;
}

void Area::rotateShapeY(float** oldMatrix, float cosV, float sinV) {
    float** newMatrix = new float*[4];
    for (int i = 0; i < 4; i++) {
        newMatrix[i] = new float[4];
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                newMatrix[i][j] = 1;
            }
            else
                newMatrix[i][j] = 0;
        }
    }
    newMatrix[0][0] = cosV;
    newMatrix[0][2] = sinV;
    newMatrix[2][0] = -sinV;
    newMatrix[2][2] = cosV;
    matrixMultiplication(oldMatrix, 4, 4, newMatrix, 4);
    for (int i = 0; i < 4; i++) {
        delete [] newMatrix[i];
    }
    delete [] newMatrix;
}
void Area::rotateShapeZ(float** oldMatrix, float cosV, float sinV) {
    float** newMatrix = new float*[4];
    for (int i = 0; i < 4; i++) {
        newMatrix[i] = new float[4];
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                newMatrix[i][j] = 1;
            }
            else
                newMatrix[i][j] = 0;
        }
    }
    newMatrix[0][0] = cosV;
    newMatrix[0][1] = -sinV;
    newMatrix[1][0] = sinV;
    newMatrix[1][1] = cosV;
    matrixMultiplication(oldMatrix, 4, 4, newMatrix, 4);
    for (int i = 0; i < 4; i++) {
        delete [] newMatrix[i];
    }
    delete [] newMatrix;
}
void Area::scaleShape(float** oldMatrix, float valueX, float valueY, float valueZ) {
    float** newMatrix = new float*[4];
    for (int i = 0; i < 4; i++) {
        newMatrix[i] = new float[4];
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                newMatrix[i][j] = 1;
            }
            else
                newMatrix[i][j] = 0;
        }
    }
    newMatrix[0][0] = valueX;
    newMatrix[1][1] = valueY;
    newMatrix[2][2] = valueZ;
    matrixMultiplication(oldMatrix, 4, 4, newMatrix, 4);
    for (int i = 0; i < 4; i++) {
        delete [] newMatrix[i];
    }
    delete [] newMatrix;
}

void Area::matrixMultiplication(float **first, int firstRow, int firstColumn, float** second, int secondColumn) {
    float** matrixCopy = new float*[firstRow];

    for (int i = 0; i < firstRow; i++) {
        matrixCopy[i] = new float[firstColumn];
        for (int j = 0; j < firstColumn; j++) {
            matrixCopy[i][j] = first[i][j];
            first[i][j] = 0;
        }
    }
    for (int i = 0; i < firstRow; i++) {
        for (int j = 0; j < secondColumn; j++) {
            for (int k = 0; k < firstColumn; k++) {
                first[i][j] += matrixCopy[i][k] * second[k][j];
            }
        }
    }
    for (int i = 0; i < firstRow; i++) {
        delete [] matrixCopy[i];
    }
    delete [] matrixCopy;
}

float Area::getA(Coords *first, Coords *second, Coords *third) {
    float y1 = first->getY();
    float y2 = second->getY();
    float y3 = third->getY();
    float z1 = first->getZ();
    float z2 = second->getZ();
    float z3 = third->getZ();

//    cout << '(' << y3<<'-'<<y1 << ")*(" << z2<<'-'<<z1<<")-("<<y2<<'-'<<y1<<")*("<<z3<<'-'<<z1<<')'<<endl;

    return (y3-y1)*(z2-z1)-(y2-y1)*(z3-z1);
}

float Area::getB(Coords *first, Coords *second, Coords *third) {

    float x1 = first->getX();
    float x2 = second->getX();
    float x3 = third->getX();
    float z1 = first->getZ();
    float z2 = second->getZ();
    float z3 = third->getZ();

    return (x2-x1)*(z3-z1)-(x3-x1)*(z2-z1);
}

float Area::getC(Coords *first, Coords *second, Coords *third) {


    float y1 = first->getY();
    float y2 = second->getY();
    float y3 = third->getY();
    float x1 = first->getX();
    float x2 = second->getX();
    float x3 = third->getX();

    return (x3-x1)*(y2-y1)-(x2-x1)*(y3-y1);
}

float Area::getD(float A, float B, float C, Coords *first) {
    float x1 = first->getX();
    float y1 = first->getY();
    float z1 = first->getZ();

    return -(A*x1+B*y1+C*z1);
}
