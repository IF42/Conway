#ifndef FILELDWIDGET_H
#define FILELDWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>


/*
** game settings
*/
#define FIELD_WIDTH 100
#define FIELD_HEIGHT 100
#define INTERRUPT_DELAY 50


typedef enum
{
    CellWhite
    , CellBlack
}Cell;


class FieldWidget : public QWidget
{
    Q_OBJECT
private:
    Cell ** field;

    virtual void paintEvent(QPaintEvent * event);

    void init_filed();
    void display_filed();
    void update_field();
    void setup_cyclic_interrupt();

public:
    FieldWidget(QWidget *parent = nullptr);
    ~FieldWidget();

};

#endif // FILELDWIDGET_H
