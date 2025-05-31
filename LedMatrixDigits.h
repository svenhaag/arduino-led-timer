#define NO_OF_ROWS 8
#define NO_OF_COLS 12

byte currentFrame[NO_OF_ROWS][NO_OF_COLS];
byte rotatedFrame[NO_OF_ROWS][NO_OF_COLS];

typedef struct position {
  byte row;
  byte col;
} position;

// position of digits on the matrix display
const position first = { 5, 0 };
const position second = { 0, 0 };
const position third = { 5, 7 };
const position fourth = { 0, 7 };

const byte digits[][3][5] = {
  {//0
    {1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1}
  },
  {//1
    {0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1},
    {0, 1, 0, 0, 1}
    
  },
  {//2
    {1, 1, 1, 0, 1},
    {1, 0, 1, 0, 1},
    {1, 0, 1, 1, 1}
  },
  {//3
    {1, 1, 1, 1, 1},
    {1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1}
  },
  {//4
    {1, 1, 1, 1, 1},
    {0, 0, 1, 0, 0},
    {1, 1, 1, 0, 0}
  },
  {//5
    {1, 0, 1, 1, 1},
    {1, 0, 1, 0, 1},
    {1, 1, 1, 0, 1}
  },
  {//6
    {1, 0, 1, 1, 1},
    {1, 0, 1, 0, 1},
    {1, 1, 1, 1, 1}
  },
  {//7
    {1, 1, 0, 0, 0},
    {1, 0, 1, 0, 0},
    {1, 0, 0, 1, 1}
  },
  {//8
    {1, 1, 1, 1, 1},
    {1, 0, 1, 0, 1},
    {1, 1, 1, 1, 1}
  },
  {//9
    {1, 1, 1, 1, 1},
    {1, 0, 1, 0, 1},
    {1, 1, 1, 0, 1}
  }  
};

/* Set digit on Matrix */
void setDigit(position digitPosition, const byte digit[][5]) {
  for (byte r = 0; r < 3; r++) {
    for (byte c = 0; c < 5; c++) {
      currentFrame[r + digitPosition.row][c + digitPosition.col] = digit[r][c];
    }
  }
}

/* Rotate Matrix frame */
void rotateFrame() {
  for (byte r = 0; r < NO_OF_ROWS; r++) {
    for (byte c = 0; c < NO_OF_COLS; c++) {
      rotatedFrame[r][c] = currentFrame[NO_OF_ROWS - 1 - r][NO_OF_COLS - 1 - c];
    }
  }
  memcpy(currentFrame, rotatedFrame, sizeof rotatedFrame);
}