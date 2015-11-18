/*
  ==============================================================================

   This file is part of the Projucer live coding demo created for CppCon 2015.
   Copyright (c) 2015 - ROLI Ltd.

   Use this code at your own risk & have some fun with C++ live coding!

   This code is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.

  ==============================================================================
*/

#ifndef GAMEOFLIFECOMPONENT_H_INCLUDED
#define GAMEOFLIFECOMPONENT_H_INCLUDED

#include <array>
#include <numeric>

//==============================================================================
enum class CellStatus
{
    dead  = 0,
    alive = 1
    // note: this is deliberately not a bool so you can add more states!
};

//==============================================================================
class GameOfLifeArray
{
public:
    //==========================================================================
    GameOfLifeArray (int numberOfRows, int numberOfColumns)
        : numRows (numberOfRows),
          numCols (numberOfColumns),
          gridData (numRows * numCols)
    {
    }

    CellStatus& getCell (int row, int col) const noexcept
    {
        return gridData [getIndex (row, col)];
    }

    int getNumAliveNeighbours (int row, int col) const noexcept
    {
        std::array <int, 8> neighbourIndices = {
            getIndex (row - 1, col - 1),
            getIndex (row - 1, col    ),
            getIndex (row - 1, col + 1),
            getIndex (row,     col - 1),
            getIndex (row,     col + 1),
            getIndex (row + 1, col - 1),
            getIndex (row + 1, col    ),
            getIndex (row + 1, col + 1)
        };

        return std::count_if (
            neighbourIndices.begin(),
            neighbourIndices.end(),
            [this] (int i) { return  gridData[i] == CellStatus::alive; }
        );
    }

    void clear()
    {
        std::fill (gridData.getData(),
                   gridData.getData() + (numRows * numCols),
                   CellStatus::dead);
    }

    void swapWith (GameOfLifeArray& other) noexcept
    {
        jassert (numRows == other.numRows && numCols == other.numCols);

        std::swap (gridData, other.gridData);
    }

private:
    //==========================================================================
    int getIndex (int row, int col) const noexcept
    {
        // for indices out of bounds, we simply wrap around the edges:
        row = negativeAwareModulo (row, numRows);
        col = negativeAwareModulo (col, numCols);

        int result =  (row * numCols) + col;
        return result;
    }

    int numRows, numCols;
    HeapBlock<CellStatus> gridData;
};


//==============================================================================
class GameOfLifeBoard : public Component,
                        private Timer
{
public:
    //==========================================================================
    GameOfLifeBoard (int numberOfRows, int numberOfColumns)
        : numRows (numberOfRows), numCols (numberOfColumns),
          thisBoard (numRows, numCols),
          nextBoard (numRows, numCols)
    {
        setSize (400, 400);
        generateBoardInitialConditions();

        startTimer (timestepInMilliseconds);
    }

    void paint (Graphics& g) override
    {
        float cellWidth  = (float) getWidth()  / (float) numCols;
        float cellHeight = (float) getHeight() / (float) numRows;

        for (int row = 0; row < numRows; ++row)
        {
            for (int col = 0; col < numCols; ++col)
            {
                float cellMargin = 2.0f;

                Rectangle<float> cellPosition ((float) col * cellWidth  + (0.5f * cellMargin),
                                               (float) row * cellHeight + (0.5f * cellMargin),
                                               cellWidth - cellMargin,
                                               cellHeight - cellMargin);

                g.setColour ((thisBoard.getCell (row, col) == CellStatus::alive)
                              ? Colours::black
                              : Colours::white);

                g.fillRect (cellPosition);
            }
        }
    }

    void restart()
    {
        stopTimer();
        generateBoardInitialConditions();
        startTimer (timestepInMilliseconds);
    }

private:
    //==========================================================================
    void timerCallback() override
    {
        advanceBoard();
        repaint();
    }

    void generateBoardInitialConditions()
    {
        // here we create the initial conditions of the game!

        thisBoard.clear();

        // we are placing an R-pentomino in the middle of the board,
        // that's s a particular pattern which will evolve for a while before stabilising.
        // --> try generating other initial conditions!
        //     (maybe using a random number generator?)
        thisBoard.getCell (10, 11) = CellStatus::alive;
        thisBoard.getCell (10, 12) = CellStatus::alive;
        thisBoard.getCell (11, 10) = CellStatus::alive;
        thisBoard.getCell (11, 11) = CellStatus::alive;
        thisBoard.getCell (12, 11) = CellStatus::alive;
    }

    void advanceBoard() noexcept
    {
        for (int row = 0; row < numRows; ++row)
            for (int col = 0; col < numCols; ++col)
                nextBoard.getCell (row, col) = getNextCellStatus (thisBoard.getCell (row, col),
                                                                  thisBoard.getNumAliveNeighbours (row, col));

        thisBoard.swapWith (nextBoard);
    }

    // this function determines the "rules" of the game.
    // --> modify the rules while the game runs and watch the effects!
    CellStatus getNextCellStatus (CellStatus currentCellStatus, unsigned numLiveNeighbours) noexcept
    {
        if (currentCellStatus == CellStatus::alive)
        {
            if (numLiveNeighbours < 2 || numLiveNeighbours > 3)
                return CellStatus::dead;

            return CellStatus::alive;
        }

        if (numLiveNeighbours == 3)
            return CellStatus::alive;

        return CellStatus::dead;
    }

    int numRows, numCols;
    GameOfLifeArray thisBoard, nextBoard;

    const int timestepInMilliseconds = 500;
};

//==============================================================================
class GameOfLifeComponent : public Component,
                            private Button::Listener
{
public:
    GameOfLifeComponent()
    {
        addAndMakeVisible (board);
        addAndMakeVisible (restartButton);

        restartButton.addListener (this);

        setSize (600, 600);
    }

    void paint (Graphics& g) override
    {
        g.fillAll (Colours::lightgreen);
    }

    void resized() override
    {
        restartButton.setBounds (20, 15, 100, 25);
        board.setBounds (20, 55, getWidth() - 40, getHeight() - 75);
    }

private:
    //==========================================================================
    void buttonClicked (Button* b) override
    {
        if (b == &restartButton)
            board.restart();
    }

    GameOfLifeBoard board { 20, 20 };
    TextButton restartButton { "Restart" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GameOfLifeComponent)
};



#endif  // GAMEOFLIFECOMPONENT_H_INCLUDED
