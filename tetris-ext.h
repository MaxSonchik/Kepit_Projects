#define A_WIDTH  10  
#define A_HEIGHT 20 
#define T_WIDTH  4  
#define T_HEIGHT 4 

typedef struct {
    char *buffer;
    size_t buffLen;
    size_t byteLen;
} BufferBuilder;


clock_t getMs(clock_t start, clock_t end);


BufferBuilder *bbCreate(size_t initialSize);


int bbAppend(BufferBuilder *bb, const char *str);


void bbFree(BufferBuilder *bb);


void newTetromino();


bool validPos(int tetromino, int rotation, int posX, int posY);


int rotate(int x, int y, int rotation);


void processInputs();


bool moveDown();


void addToArena();


void checkLines();


void drawArena();