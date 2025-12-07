#define A_WIDTH  10  
#define A_HEIGHT 20  
#define T_WIDTH  4   
#define T_HEIGHT 4   


void newTetromino();


bool validPos(int tetromino, int rotation, int posX, int posY);


int rotate(int x, int y, int rotation);


void processInputs();


bool moveDown();


void addToArena();


void checkLines();


void drawArena();