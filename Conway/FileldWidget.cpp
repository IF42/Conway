#include "FileldWidget.h"
#include <iostream>


static int
count_neighbors(int x, int y, Cell ** field);


FieldWidget::FieldWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowState(Qt::WindowMaximized);
    this->init_filed();
    this->setup_cyclic_interrupt();

}


/*
** Setup cyclic interrupt function for cyclic repainting
*/
void
FieldWidget::setup_cyclic_interrupt()
{
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()), this, SLOT(update()));
    timer->start(INTERRUPT_DELAY);
}


/**
** Initialization of field where is live displayed with random black cells
*/
void
FieldWidget::init_filed()
{
    this->field = new Cell*[FIELD_WIDTH];

    for(int i = 0; i < FIELD_WIDTH; i ++)
    {
        this->field[i] = new Cell[FIELD_HEIGHT];

        for(int j = 0; j < FIELD_HEIGHT; j++)
        {
            if(rand() % 10 >= 1)
                this->field[i][j] = CellWhite;
            else
                this->field[i][j] = CellBlack;
        }
    }
}


void
FieldWidget::paintEvent(QPaintEvent * event)
{
    this->update_field();
    this->display_filed();
}


/*
** drawing life state from field
*/
void
FieldWidget::display_filed()
{
    QPainter paint(this);

    double ceel_width = ((double)QWidget::width())/((double)FIELD_WIDTH);
    double ceel_height = ((double)QWidget::height())/((double)FIELD_HEIGHT);

    for(int x = 0; x < FIELD_WIDTH; x++)
    {
        for(int y = 0; y < FIELD_HEIGHT; y++)
        {
            QRect rect(ceel_width*x, ceel_height*y, ceel_width, ceel_height);

            switch(this->field[x][y])
            {
            case CellWhite:
                paint.fillRect(rect, QBrush(Qt::white, Qt::SolidPattern));
                break;
            case CellBlack:
                paint.fillRect(rect, QBrush(Qt::black, Qt::SolidPattern));
                break;
            }
        }
    }
}


/*
** updating of life state into next generation field
*/
void
FieldWidget::update_field()
{
    Cell ** new_field = new Cell*[FIELD_WIDTH];
    
    for(int x = 0; x < FIELD_WIDTH; x++)
    {
        new_field[x] = new Cell[FIELD_HEIGHT];
        
        for(int y = 0; y < FIELD_HEIGHT; y++)
        {
            int neighbors = count_neighbors(x, y, this->field);

            if(this->field[x][y] == CellBlack)
            {
                if(neighbors == 2 || neighbors == 3)
                    new_field[x][y] = CellBlack;
                else
                    new_field[x][y] = CellWhite;
            }
            else
            {
                if(neighbors == 3)
                    new_field[x][y] = CellBlack;
                else
                    new_field[x][y] = CellWhite;
            }
        }
    }

    // deleting of old field
    for(int i = 0; i < FIELD_WIDTH; i ++)
    {
        delete this->field[i];
    }

    delete this->field;


    this->field = new_field;
}


/*
** counting black neigbors of the cell
*/
static int
count_neighbors(int x, int y, Cell ** field)
{
    int neighbrhood = 0;

    if(x > 0 && y > 0)
        neighbrhood += field[x-1][y-1] == CellBlack ? 1 : 0;

    if(x > 0)
        neighbrhood += field[x-1][y] == CellBlack ? 1 : 0;

    if(y > 0)
        neighbrhood += field[x][y-1] == CellBlack ? 1 : 0;

    if(x < FIELD_WIDTH-1)
        neighbrhood += field[x+1][y] == CellBlack ? 1 : 0;

    if(y < FIELD_HEIGHT-1)
        neighbrhood += field[x][y+1] == CellBlack ? 1 : 0;

    if(x < FIELD_WIDTH-1 && y < FIELD_HEIGHT-1)
        neighbrhood += field[x+1][y+1] == CellBlack ? 1 : 0;

    if(x < FIELD_WIDTH-1 && y > 0)
        neighbrhood += field[x+1][y-1] == CellBlack ? 1 : 0;

    if(x > 0 && y < FIELD_HEIGHT-1)
        neighbrhood += field[x-1][y+1] == CellBlack ? 1 : 0;

    return neighbrhood;
}


FieldWidget::~FieldWidget()
{

}


