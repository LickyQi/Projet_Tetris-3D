#ifndef GAME_HPP
#define GAME_HPP

// @author Yueming YANG
class Piece {
public:
    // Constructor
    Piece();
    // @param desc: an array for represent a piece of tetris
    // @param cindex: the index for a color
    // @param left: the margin on the left of the piece
    // @param top: the margin on the top of the piece
    // @param right: the margin on the right of the piece
    // @param bottom: the margin on the bottom of the piece
    Piece(const char *desc, int cindex,
          int left, int top, int right, int bottom);

    Piece& operator =(const Piece& other);

    // Get the corresponding margin value
    int getLeftMargin() const;
    int getTopMargin() const;
    int getRightMargin() const;
    int getBottomMargin() const;

    // Get the corresponding index of colour
    int getColourIndex() const;

    // Set clockwise rotation
    Piece rotateCW() const;
    // Set counterclockwise rotation
    Piece rotateCCW() const;

    // Determine if it is part of a piece
    bool isOn(int row, int col) const;

private:
    // Get information for each column
    void getColumn(int col, char *buf) const;
    // Get the information of each column in reverse order
    void getColumnRev(int col, char *buf) const;

    // Declare an array of size 16
    char desc_[16];
    // Declare the index for a color
    int cindex_;
    // Declqre an array of size 4
    int margins_[4];
};


// @author Qi LI
class Game
{
public:
    // Create a new game instance with a well of the given dimensions.
    // Constructor
    Game(int width, int height);

    // Destructor
    ~Game();

    // Advance the game by one tick.
    int tick();

    // Move the currently falling piece left or right by one unit.
    // Returns whether the move was successful.
    bool moveLeft();
    bool moveRight();

    // Drop the current piece to the lowest position it can legally
    // occupy.  Returns whether anything happened.
    bool drop();

    // Rotate the piece clockwise.  Returns whether
    // the rotation was successful.
    bool rotateCW();


    // Declare and initilize the score
    int score = 0;

    // Get the width of the board
    int getWidth() const
    {
        return board_width_;
    }
    // Get the height of the board
    int getHeight() const
    {
        return board_height_;
    }

    // Get the contents of the cell at row r and column c.
    int get(int r, int c) const;
    int& get(int r, int c);

private:
    // Determine if the current position can place this piece
    bool doesPieceFit(const Piece& p, int x, int y) const;

    // Delete this row when a row is filled and move all of the above pieces down one row
    void removeRow(int y);

    // Repeat scanning all rows from bottom to top, removing the first full
    // row, stopping when there are no more full rows.
    int collapse();

    // Remove one piece
    void removePiece(const Piece& p, int x, int y);
    // Place a piece
    void placePiece(const Piece& p, int x, int y);

    // Generate a new piece
    void generateNewPiece();

private:
    // The size of the board
    int board_width_;
    int board_height_;

    // Game ending sign
    bool stopped_;

    // Declare a piece of tetrimino of type Piece
    Piece piece_;
    // Coordinates of current piece
    int px_;
    int py_;
    // The total number of pieces of a board
    int* board_;
};

#endif
