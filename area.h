#ifndef AREA_H
#define AREA_H

#include <QWidget>
#include <shape.h>

class Area : public QWidget
{
    Q_OBJECT
private:
//    Shape* shape;
//    Shape* shapeOrigin;
    int myTimer;
    int** gran;
    float viewX = 5;
    float viewY = 0;
    float viewZ = 0;
    float alpha = 0;
    void setNewCoords(Shape* shape, float**);
    void translateShape(float** oldMatrix, Coords* value);
    void rotateShapeX(float** oldMatrix, float cosV, float sinV);
    void rotateShapeY(float** oldMatrix, float cosV, float sinV);
    void rotateShapeZ(float** oldMatrix, float cosV, float sinV);
    void scaleShape(float** oldMatrix, float valueX, float valueY, float valueZ);
    void matrixMultiplication(float**, int, int, float**, int);
    float getA(Coords*, Coords*, Coords*);
    float getB(Coords*, Coords*, Coords*);
    float getC(Coords*, Coords*, Coords*);
    float getD(float, float, float, Coords*);
public:
    Area(QWidget *parent = 0);
    ~Area() {
//        delete shape;
        killTimer(myTimer);
    };
public slots:
    void start_prog();
protected:
    //обработчики событий
    void paintEvent(QPaintEvent *event); //событие перерисовки
    void timerEvent(QTimerEvent *event); //событие таймера
    void showEvent(QShowEvent *event); // событие начала
    void hideEvent(QHideEvent *event); // событие конца
};

#endif // AREA_H
