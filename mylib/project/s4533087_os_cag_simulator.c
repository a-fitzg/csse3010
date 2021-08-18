/**
  *****************************************************************************
  * @file    s4533087_os_cag_simulator.c
  * @author  Alexander FitzGerald - s4533087
  * @date    16052020
  * @brief   CGoL simulator OS CAG driver
  *****************************************************************************
  * EXTERNAL FUNCTIONS
  *****************************************************************************
  * s4533087_os_cag_simualtor()
  * s4533087_os_cag_simulator_alt_refresher()
  *****************************************************************************
  */

/* Includes ---------------------------------------------------------------- */
#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

// Mylib includes
#include "s4533087_os_cag_display.h"
#include "s4533087_os_lta1000g.h"

/* Private typedef --------------------------------------------------------- */
/* Private define ---------------------------------------------------------- */
// Task priorities
#define SIMULATOR_DEFAULT_TASK_PRIORITY     (tskIDLE_PRIORITY + 2)

// Task stack allocations
#define SIMULATOR_DEFAULT_STACK_SIZE        (configMINIMAL_STACK_SIZE * 4)

#define ALIVE           1
#define DEAD            0

#define GAME_PAUSED     1
#define GAME_UNPAUSED   0

#define LIFE_MAX_SIZE   4

/* Private macro ----------------------------------------------------------- */
/* Private variables ------------------------------------------------------- */
uint8_t paused;

// Arrays representing the lifeforms
const uint8_t BLOCK[LIFE_MAX_SIZE][LIFE_MAX_SIZE] = {
    {1, 1, 0, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}};

const uint8_t BEEHIVE[LIFE_MAX_SIZE][LIFE_MAX_SIZE] = {
    {0, 1, 1, 0},
    {1, 0, 0, 1},
    {0, 1, 1, 0},
    {0, 0, 0, 0}};

const uint8_t LOAF[LIFE_MAX_SIZE][LIFE_MAX_SIZE] = {
    {0, 1, 1, 0},
    {1, 0, 0, 1},
    {0, 1, 0, 1},
    {0, 0, 1, 0}};

const uint8_t BLINKER[LIFE_MAX_SIZE][LIFE_MAX_SIZE] = {
    {0, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 0, 0}};

const uint8_t TOAD[LIFE_MAX_SIZE][LIFE_MAX_SIZE] = {
    {0, 1, 1, 1},
    {1, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}};

const uint8_t BEACON[LIFE_MAX_SIZE][LIFE_MAX_SIZE] = {
    {1, 1, 0, 0},
    {1, 0, 0, 0},
    {1, 0, 0, 1},
    {0, 0, 1, 1}};

const uint8_t GLIDER[LIFE_MAX_SIZE][LIFE_MAX_SIZE] = {
    {0, 0, 1, 0},
    {1, 0, 1, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0}};

/* Private function prototypes --------------------------------------------- */
uint8_t count_neighbours(uint8_t**, int8_t, int8_t);
void update_grid(uint8_t**, uint8_t**);
void evolve(uint8_t**);
void set_cell(uint8_t**, uint8_t, uint8_t, uint8_t);
void draw_life(uint8_t**, uint8_t, uint8_t, uint8_t, uint8_t);
void debug_print_grid(uint8_t**);
void draw_array(uint8_t**, const uint8_t[LIFE_MAX_SIZE][LIFE_MAX_SIZE], uint8_t, 
        uint8_t);

/**
  * @brief  Task for the keypad input
  * @params None
  * @retval None
  */
void s4533087_os_cag_simulator(void) {

    s4533087_SemaphoreDisplayRefresh    = xSemaphoreCreateBinary();
    s4533087_SemaphoreGamePause         = xSemaphoreCreateBinary();
    s4533087_SemaphoreGameResume        = xSemaphoreCreateBinary();

    // Message struct for displaying new lifes
    caMessage_t lifeMessage;

    uint8_t lifeType;
    uint8_t lifeDetails;
    uint8_t cell_x;
    uint8_t cell_y;

    // Set default update time = 2000ms (median joystick value)
    s4533087_displayUpdateTime = 2000;

    // Start un-paused
    s4533087_gamePaused = UNPAUSED;
    paused = 0;

    // Wait a bit for things to get set up
    vTaskDelay(1);

    for (;;) {

        #ifdef LEGACY_PAUSE
        // See if we have been sent semaphores to pause / resume game
        if ((s4533087_SemaphoreGamePause != NULL) && 
                (s4533087_SemaphoreGameResume)) {

            if (xSemaphoreTake(s4533087_SemaphoreGamePause, 10) == pdTRUE) {
                
                paused = GAME_PAUSED;
            }
            if (xSemaphoreTake(s4533087_SemaphoreGameResume, 10) == pdTRUE) {

                paused = GAME_UNPAUSED;
            }
        }
        #endif

        // Next, see if we have a message on the queue to add a new life
        if (s4533087_QueueDisplayLife != NULL) {
            
            if (xQueueReceive(s4533087_QueueDisplayLife, &lifeMessage, 10)) {
 
                // Add that life to the board
                lifeType    = ((lifeMessage.type & 0xF0) >> 4);
                lifeDetails = (lifeMessage.type & 0x0F);
                cell_x      = lifeMessage.cell_x;
                cell_y      = lifeMessage.cell_y;
  
                draw_life(s4533087_gameGrid, cell_x, cell_y, lifeType, 
                        lifeDetails);
            }

        }

        if (paused == GAME_UNPAUSED) {
        
            // Send off semaphore to update the display
            if (s4533087_SemaphoreDisplayRefresh != NULL) {

                #ifndef DEBUG_PRINTING
                xSemaphoreGive(s4533087_SemaphoreDisplayRefresh);
                #else
                portENTER_CRITICAL();
                debug_print_grid(s4533087_gameGrid);
                portEXIT_CRITICAL();
                #endif
            }

            // Evolve to next iteration
            evolve(s4533087_gameGrid);

            // The "refresh screen" variable controls this delay here.
            vTaskDelay(s4533087_displayUpdateTime);
        }
    }
}

/**
  * @brief  Task that updates the display when the simulator is not running
  * @params None
  * @retval None
  */
void s4533087_os_cag_simulator_alt_refresher(void) {

    // START SUSPENDED - suspend ourselves
    vTaskSuspend(NULL);

    vTaskDelay(100);

    // Just send off the "refresh" semaphores for when the simulator is disabled
    for (;;) {

        if (s4533087_SemaphoreDisplayRefresh != NULL) {

            xSemaphoreGive(s4533087_SemaphoreDisplayRefresh);
        }
        
        vTaskDelay(s4533087_displayUpdateTime);
    }
}


/**
  * @brief  Count the number of alive neighbours in your 3x3 vicinity
  * @params grid: The game grid
            x: x coordinate of a position to search around
            y: y coordinate of a position to search around
  * @retval count: The number of alive neighbours that point has
  */
uint8_t count_neighbours(uint8_t** grid, int8_t x, int8_t y) {

    uint8_t count = 0;
    
    // For a given (x, y), search its 3x3 vicinity
    for (int8_t relativeY = -1; relativeY <= 1; relativeY++) {
        
        for (int8_t relativeX = -1; relativeX <= 1; relativeX++) {
            
            // Don't count yourself as a neighbour
            if ((relativeX != 0) || (relativeY != 0)) {
                
                // Boundary checking - if this coord is out of bounds, skip it
                if (((x + relativeX) < 0) || ((x + relativeX >= GRID_X_SIZE)) ||
                        ((y + relativeY) < 0) || ((y + relativeY) >= 
                                GRID_Y_SIZE)) {
                    
                    // We've got a dodgy coordinate, skip this one
                    continue;
                }
                count += grid[y + relativeY][x + relativeX];
            }
        }
    }
    return count;
}

/**
  * @brief  Copies a game grid from newGrid to oldGrid
  * @params oldGrid: The "old" game grid - gets copied into
            newGrid" The "new" game grid - gets copied from
  * @retval None
  */
void update_grid(uint8_t** oldGrid, uint8_t** newGrid) {

    // Operates on the old grid, taking values from new grid
    // newGrid is typically freed after this
    for (uint8_t y = 0; y < GRID_Y_SIZE; y++) {
        
        for (uint8_t x = 0; x < GRID_X_SIZE; x++) {
            
            oldGrid[y][x] = newGrid[y][x];
        }
    }
}


void evolve(uint8_t** grid) {

    // Make a temporary working grid, operate on that one, and then copy those 
    // changes over to the existing grid, then delete the temporary grid

    //#######################################################
    // REMINDER: grid is accessed: grid[y][x] - y coord first
    //#######################################################

    // Make the temporary "working" grid
    uint8_t** nextGrid = pvPortMalloc(GRID_Y_SIZE * sizeof(uint8_t*));
    for (uint8_t i = 0; i < GRID_Y_SIZE; i++) {

        nextGrid[i] = pvPortMalloc(GRID_X_SIZE * sizeof(uint8_t));
    }
    
    // Now, we iterate over the working grid, determining each cell's fate
    for (uint8_t y = 0; y < GRID_Y_SIZE; y++) {
        
        for (uint8_t x = 0; x < GRID_X_SIZE; x++) {
            
            uint8_t neighbours = count_neighbours(grid, x, y);
            
            if (grid[y][x] == ALIVE) {
                
                if (neighbours == 2 || neighbours == 3) {
                    
                    // Cell lives for another round
                    nextGrid[y][x] = ALIVE;
                } else {
                    
                    // Cell will die
                    nextGrid[y][x] = DEAD;
                }
            } else {
                
                if (neighbours == 3) {
                    
                    // The dead cell has 3 neighbours - it is resurrected
                    nextGrid[y][x] = ALIVE;
                } else {
                    
                    // Cell stays dead
                    nextGrid[y][x] = DEAD;
                }
            }
        }
    }
    
    // Sends updates to the game grid - this is not to be interrupted
    portENTER_CRITICAL();
    update_grid(grid, nextGrid);
    portEXIT_CRITICAL();
    
    // Free our temporary grid now that we're done with it
    for (uint8_t i = 0; i < GRID_Y_SIZE; i++) {
        vPortFree(nextGrid[i]);
    }
    vPortFree(nextGrid);
}

/**
  * @brief  Sets a nominated cell to a value (1 or 0)
  * @params grid: The game grid
            x: The x value of the cell
            y: The y value of the cell
            value: The value of the cell (1 or 0 for alive or dead)
  * @retval None
  */
void set_cell(uint8_t** grid, uint8_t x, uint8_t y, uint8_t value) {

    grid[y][x] = !!value;
}

/**
  * @brief  Prints the grid as text. FOR DEBUGGING PURPOSES ONLY
  * @params grid: the game grid to print
  * @retval None
  */
void debug_print_grid(uint8_t** grid) {

    for (uint8_t row = 0; row < GRID_Y_SIZE; row++) {
    
        for (uint8_t column = 0; column < GRID_X_SIZE; column++) {

            debug_printf("%c ", grid[row][column] ? '#' : ' ');
        }
        debug_printf("\r\n");
    }
}

/**
  * @brief  Draws a lifeform at the designated coordinate
  * @params grid: The game grid
            x_coord: X coordinate for lifeform
            y_coord: Y coordinate for lifeform
            lifeType: General life type (type bits [7 to 4])
            lifeDetails: Specific life sub-type (type bits [3 to 0])
  * @retval None
  */
void draw_life(uint8_t** grid, uint8_t x_coord, uint8_t y_coord, 
        uint8_t lifeType, uint8_t lifeDetails) {

    switch (lifeType) {

        case 1:
            
            // Single cell
            set_cell(grid, x_coord, y_coord, lifeDetails);
            break;

        case 2:

            // Still life
            switch (lifeDetails) {

                case 0:

                    // Block
                    // ## 
                    // ##
                    draw_array(s4533087_gameGrid, BLOCK, x_coord, y_coord);
                    break;

                case 1:

                    // Beehive
                    //  ##
                    // #  #
                    //  ##
                    draw_array(s4533087_gameGrid, BEEHIVE, x_coord, y_coord);
                    break;

                case 2:

                    // Loaf
                    //  ##
                    // #  #
                    //  # #
                    //   #
                    draw_array(s4533087_gameGrid, LOAF, x_coord, y_coord);
                    break;

                default:

                    break;

            }
            break;

        case 3:

            // Oscillator
            switch (lifeDetails) {

                case 0:

                    // Blinker
                    // ###
                    draw_array(s4533087_gameGrid, BLINKER, x_coord, y_coord);
                    break;

                case 1:

                    // Toad
                    //  ###
                    // ###
                    draw_array(s4533087_gameGrid, TOAD, x_coord, y_coord);
                    break;

                case 2:

                    // Beacon
                    // ##
                    // # 
                    //    #
                    //   ##
                    draw_array(s4533087_gameGrid, BEACON, x_coord, y_coord);
                    break;

                default:

                    break;
            }
            break;

        case 4:

            // Space ship
            switch (lifeDetails) {

                case 0:

                    // Glider
                    //   #
                    // # #
                    //  ##
                    draw_array(s4533087_gameGrid, GLIDER, x_coord, y_coord);
                    break;

                default:
                    
                    break;

            }
            break;

        default:

            break;

    }    
}

/**
  * @brief  Draws a specified array onto the game grid
  * @params grid: The game grid
            life: The life (as an array) to draw
            x: The x coordinate to start drawing at
            y: The y coordinate to start drawing at
  * @retval None
  */
void draw_array(uint8_t** grid, const uint8_t life[LIFE_MAX_SIZE][
        LIFE_MAX_SIZE], uint8_t x, uint8_t y) {

    // Iterate through the life array, and draw it on the board
    for (uint8_t row = 0; row < LIFE_MAX_SIZE; row++) {

        for (uint8_t column = 0; column < LIFE_MAX_SIZE; column++) {

            // Only draw the cell if the life is defined at that cell, prevent
            // overwriting gameGrid's alive cells with cells that are not 
            // defined in life
            if (life[row][column]) {

                grid[y + row][x + column] = life[row][column];
            }
        }
    }
}
