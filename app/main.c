#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <raylib.h>


#define win_width 800
#define win_height 600


typedef enum {
    Cell_White = 0
    , Cell_Black
} Cell;


typedef struct {
    int field_index;
    
    int width;
    int height;

    Cell * array;
} Cell_Field;


#define cell_field_at(T, index, x, y) \
    (T)->array[(index * (T)->width * (T)->height) + ((T)->height * x) + y]


static Cell_Field cell_field_init(int width, int height) {
    size_t memsize = sizeof(Cell) * width * height * 2;
    Cell * array = malloc(memsize); 
 
    for(size_t i = 0, max = width * height * 2; i < max; i++) {
        array[i] = rand() % (Cell_Black + 1);
    } 

    return (Cell_Field) {
        .field_index = 0
        , .width = width
        , .height = height
        , .array = array
    };
}


static void cell_field_finalize(Cell_Field * self) {
    if(self->array != NULL) {
        free(self->array);
    }
}


static int cell_field_count_neighbors(Cell_Field * self, int index, int x, int y) {
    int count = 0;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue; 
            } 

            int nx = x + i;
            int ny = y + j;

            if (nx >= 0 && nx < self->width && ny >= 0 && ny < self->height) {
                count += cell_field_at(self, index, nx, ny) == Cell_Black ? 1 : 0;
            }
        }
    }

    return count;
}


static void cell_field_update(Cell_Field * self) {
    int next_index = (self->field_index + 1) % 2;

    for(int i = 0; i < self->width; i ++) {
        for(int j = 0; j < self->height; j++) {
            int neighbors = cell_field_count_neighbors(self, self->field_index, i, j);
             switch(cell_field_at(self, self->field_index, i, j)) {
                case Cell_Black:
                    if(neighbors == 2 || neighbors == 3) {
                        cell_field_at(self, next_index, i, j) = Cell_Black;
                    } else {
                        cell_field_at(self, next_index, i, j) = Cell_White;
                    }
                    break;
                default:
                    if(neighbors == 3) {
                        cell_field_at(self, next_index, i, j) = Cell_Black;
                    } else {
                        cell_field_at(self, next_index, i, j) = Cell_White;
                    }
             }
        }
    }

    self->field_index = next_index;
}


int main(void) {
    srand(time(NULL));
    Cell_Field cell_field = cell_field_init(win_width, win_height);       
    
    // initialization of graphics window
    InitWindow(win_width, win_height, "Coway's game of life"); 
    SetTargetFPS(10);

    while(WindowShouldClose() == 0) {
        cell_field_update(&cell_field);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for(int i = 0; i < cell_field.width; i++) {
            for(int j = 0; j < cell_field.height; j++) {
                switch(cell_field_at(&cell_field, cell_field.field_index, i, j)) { 
                    case Cell_White: 
                        DrawRectangle(i*10, j*10, 10, 10, RAYWHITE);
                        break;
                    case Cell_Black:
                        DrawRectangle(i*10, j*10, 10, 10, BLACK);
                        break;
                }
            }
        }

        DrawFPS(10, 10);  
        EndDrawing();
    }

    CloseWindow();
    cell_field_finalize(&cell_field);

    return EXIT_SUCCESS;
}


