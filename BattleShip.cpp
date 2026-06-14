#pragma warning(disable: 4267 4244 6385 6386)
#include<iostream>
#include<cstdlib>
#include<ctime>
#include<iomanip>
#include<string>
#include<cmath>
#include<fstream>
#include <windows.h> // for colors

#include <conio.h> //for getch();
using namespace std;

// Board declarations
char board1[10][10];
char board2[10][10];

// Color constants
const int BLACK = 0;
const int BLUE = 1;
const int GREEN = 2;
const int CYAN = 3;
const int RED = 4;
const int MAGENTA = 5;
const int YELLOW = 6;
const int WHITE = 7;
int col;

// Game variables
int pyr = 1;
int player1score = 0;
int player2score = 0;
int sizes[5] = { 5, 4, 3, 3, 2 };
int hlth[2][5] = { {5, 4, 3, 3, 2}, {5, 4, 3, 3, 2} };
int r_arr[2][5];
int c_arr[2][5];
bool ver[2][5] = { 0 };
string player1name, player2name;
int shipscolour[5];
int compshipsalive = 5;
int playershipsalive = 5;
const char* shipNames[5] = { "Aircraft Carrier","Battleship","Destroyer","Submarine","Patrol Boat" };
const int shipSizes[5] = { 5, 4, 3, 3, 2 };
const char shipMarkers[5] = { '1','2','3','4','5' };

// Leaderboard arrays
string leaderboardNames[100];
int leaderboardScores[100];
int totalPlayersInFile = 0;

// Function prototypes
void setColor(int textColor, int bgColor);
int turn_c(int player_num);
void printBoard(char board[10][10], bool hideShips);
void setManualships(char board[10][10]);
void setrandships(char board[10][10]);
void playerattack(char board[10][10], int& pyrscore);
void vsPlayer(char myboard[10][10], char oppboard[10][10]);
void showGameMenu();
void displayResult(int player1Score, int player2Score);
void initializeBoard(char board[10][10]);
void setRandomShips(char board[10][10]);
int computerAttack(char playerBoard[10][10], int& computerScore, int& attackRow, int& attackCol);
void vsComputer(char playerBoard[10][10], char computerBoard[10][10]);
void mereplayerkiShips(char board[10][10]);
int merayplayerkaattack(char computerBoard[10][10], int& playerScore);
void clearScreen() {
    system("cls");
}
void printTurnScreen(char board[10][10], bool hide, string turnLabel, int attackNum, int totalAttacks, int playerAlive, int compAlive, int playerScore, int compScore);
void clearBoard(char board[10][10]) {
    initializeBoard(board);
}


// Leaderboard functions
void loadLeaderboard(string playerNames[], int scores[], int& totalPlayers);
void saveLeaderboard(const string playerNames[], const int scores[], int totalPlayers);
void showLeaderboard(const string playerNames[], const int scores[], int totalPlayers);
void sortLeaderboard(string playerNames[], int scores[], int totalPlayers);
void updateLeaderboard(string playerNames[], int scores[], int& totalPlayers, string newPlayer, int newScore);

// Initialize board with water
void initializeBoard(char board[10][10])
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            board[i][j] = '~';
        }
    }
}

// Set color function
void setColor(int textColor, int bgColor)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), bgColor * 16 + textColor);
}

// Count remaining ships for a player
int turn_c(int player_num)
{
    int count = 0;
    for (int i = 0; i < 5; i++)
    {
        if (hlth[player_num - 1][i] != 0)
        {
            count++;
        }
    }
    return count;
}

// Board printing function   





void printBoard(char board[10][10], bool hideShips) {
    cout << "     0     1     2     3     4     5     6     7     8     9   " << endl;
    for (int i = 0; i < 10; i++) {
        cout << "    +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+" << endl;
        cout << "  " << i << " |";
        for (int j = 0; j < 10; j++) {
            char cell = board[i][j];

            // Handle old ship markers (p, q, r, s, t) as hits
            if (cell == 'p' || cell == 'q' || cell == 'r' || cell == 's' || cell == 't') {
                setColor(15, 12);
                cout << "  X  ";
                setColor(7, 0);
            }
            // Handle HIT ships (red 'X' or pink for sunk)
            else if (cell == 'X') {
                // Check if it's part of a sunk ship
                bool isSunkShip = false;
                for (int shipIdx = 0; shipIdx < 5; shipIdx++) {
                    if (hlth[pyr - 1][shipIdx] == 0) { // Ship is sunk
                        bool onThisShip = false;
                        // Check if this cell is part of this ship
                        if (ver[pyr - 1][shipIdx] == 0) { // Horizontal
                            if (i == r_arr[pyr - 1][shipIdx]) {
                                for (int k = 0; k < sizes[shipIdx]; k++) {
                                    if (j == c_arr[pyr - 1][shipIdx] + k) {
                                        onThisShip = true;
                                        break;
                                    }
                                }
                            }
                        }
                        else { // Vertical
                            if (j == c_arr[pyr - 1][shipIdx]) {
                                for (int k = 0; k < sizes[shipIdx]; k++) {
                                    if (i == r_arr[pyr - 1][shipIdx] + k) {
                                        onThisShip = true;
                                        break;
                                    }
                                }
                            }
                        }
                        if (onThisShip) {
                            setColor(15, 13); // Pink for sunk ship
                            cout << "  X  ";
                            setColor(7, 0);
                            isSunkShip = true;
                            break;
                        }
                    }
                }
                if (!isSunkShip) {
                    setColor(15, 12); // Red for regular hit
                    cout << "  X  ";
                    setColor(7, 0);
                }
            }
            // Handle special hit marker 'A'
            else if (cell == 'A') {
                setColor(0, 13);
                cout << "  X  ";
                setColor(7, 0);
            }
            // Handle MISSES - grey 'O'
            else if (cell == 'O') {
                setColor(0, 8);
                cout << "  O  ";
                setColor(7, 0);
            }
            // Handle ship marker 'S' (when not hiding)
            else if (!hideShips && cell == 'S') {
                setColor(7, col);
                cout << "  S  ";
                setColor(7, 0);
            }
            // Handle numbered ship markers (when not hiding)
            else if (!hideShips && cell == '1') {
                setColor(7, shipscolour[0]);
                cout << "  S  ";
                setColor(7, 0);
            }
            else if (!hideShips && cell == '2') {
                setColor(7, shipscolour[1]);
                cout << "  S  ";
                setColor(7, 0);
            }
            else if (!hideShips && cell == '3') {
                setColor(7, shipscolour[2]);
                cout << "  S  ";
                setColor(7, 0);
            }
            else if (!hideShips && cell == '4') {
                setColor(7, shipscolour[3]);
                cout << "  S  ";
                setColor(7, 0);
            }
            else if (!hideShips && cell == '5') {
                setColor(7, shipscolour[4]);
                cout << "  S  ";
                setColor(7, 0);
            }
            // Handle WATER or hidden ships
            else {
                setColor(7, 1); // Blue for water
                cout << "  ~  ";
                setColor(7, 0);
            }
            cout << "|";
        }
        cout << endl;
    }
    cout << "    +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+" << endl;
}












// Set ships randomly
void setrandships(char board[10][10])
{
    // Set a random color for ships
    col = rand() % 8;

    int n, size = 0, r, c;

    // Array to track which ships have been placed
    bool shipsPlaced[5] = { false };
    int shipsPlacedCount = 0;

    while (shipsPlacedCount < 5)
    {
        // Randomly select a ship that hasn't been placed yet
        do
        {
            n = rand() % 5; // 0-4 for array indexing
        } while (shipsPlaced[n]);

        // Get ship size
        size = sizes[n];

        bool validPlacement = false;

        // Try to find a valid placement
        while (!validPlacement)
        {
            // Generate random starting position
            r = rand() % 10;
            c = rand() % 10;

            // Randomly choose horizontal (0) or vertical (1)
            int orientation = rand() % 2;

            // Check if placement is valid
            bool overlap = false;
            bool outOfBounds = false;

            // Check for overlap and boundaries
            if (orientation == 0) // Horizontal
            {
                if (c + size > 10)
                {
                    outOfBounds = true;
                }
                else
                {
                    for (int i = 0; i < size; i++)
                    {
                        if (board[r][c + i] == 'S')
                        {
                            overlap = true;
                            break;
                        }
                    }
                }
            }
            else // Vertical
            {
                if (r + size > 10)
                {
                    outOfBounds = true;
                }
                else
                {
                    for (int i = 0; i < size; i++)
                    {
                        if (board[r + i][c] == 'S')
                        {
                            overlap = true;
                            break;
                        }
                    }
                }
            }

            // If placement is valid
            if (!overlap && !outOfBounds)
            {
                validPlacement = true;

                // Place the ship on the board
                if (orientation == 0) // Horizontal
                {
                    ver[pyr - 1][n] = 0;
                    for (int i = 0; i < size; i++)
                    {
                        board[r][c + i] = 'S';
                    }
                }
                else // Vertical
                {
                    ver[pyr - 1][n] = 1;
                    for (int i = 0; i < size; i++)
                    {
                        board[r + i][c] = 'S';
                    }
                }

                // Store ship position
                r_arr[pyr - 1][n] = r;
                c_arr[pyr - 1][n] = c;

                // Mark ship as placed
                shipsPlaced[n] = true;
                shipsPlacedCount++;
            }
        }
    }
    cout << "Ships placed randomly!" << endl;
    printBoard(board, false);
}

// Set ships manually
void setManualships(char board[10][10])
{
    int n, size = 0, r, c, f = 0, idx[5] = { 0 };
    char ch;
    bool again, wrong, colour;

    do
    {
        colour = false;
        cout << "Enter the colour of your ship" << endl;
        cout << "BLACK(0)\n";
        cout << "BLUE(1)\n";
        cout << "GREEN(2)\n";
        cout << "CYAN (3)\n";
        cout << "RED  (4)\n";
        cout << "MAGENTA (5)\n";
        cout << "YELLOW  (6)\n";
        cout << "WHITE (7)\n";
        cin >> col;
        if (col > 7 || col < 0)
        {
            cout << "Incorrect colour input! try again" << endl;
            colour = true;
        }
    } while (colour);

    while (f < 5)
    {
        do
        {
            again = false;
            cout << "Select your ship from the menu\n";
            cout << "1) AirCraft Carrier (5 blocks)\n";
            cout << "2) Battleship( 4 blocks)\n";
            cout << "3) Destroyer (3 blocks)\n";
            cout << "4) Submarine (3 blocks)\n";
            cout << "5) Patrol Boat (2 blocks)\n";
            cin >> n;
            if (n > 5 || n < 1)
            {
                cout << "Invalid input!" << endl;
                again = true;
            }
            for (int i = 0; i < 5; i++)
            {
                if (n == idx[i])
                {
                    cout << "Already selected ship\n";
                    again = true;
                    break;
                }
            }
            if (!again)
                idx[f] = n;

        } while (again);

        switch (n)
        {
        case 1:
            size = 5;
            break;
        case 2:
            size = 4;
            break;
        case 3:
            size = 3;
            break;
        case 4:
            size = 3;
            break;
        case 5:
            size = 2;
            break;
        default:
            cout << "invalid input";
        }

        do
        {
            wrong = false;

            cout << "Enter the starting row:";
            cin >> r;
            cout << "Enter the starting column:";
            cin >> c;
            cout << "Do you want it vertical?(Y/N)";
            cin >> ch;

            if (ch == 'y' || ch == 'Y')
            {
                for (int i = 0; i < size; i++)
                {
                    if (board[r + i][c] == 'S')
                    {
                        cout << "The Ship is overlapping" << endl;
                        wrong = true;
                        break;
                    }
                }
                if (r + size > 10)
                {
                    cout << "Out of bound" << endl;
                    wrong = true;
                }
            }
            else
            {
                for (int i = 0; i < size; i++)
                {
                    if (board[r][c + i] == 'S')
                    {
                        cout << "The Ship is overlapping" << endl;
                        wrong = true;
                        break;
                    }
                }
                if (c + size > 10)
                {
                    cout << "Out of bound" << endl;
                    wrong = true;
                }
            }

        } while (wrong);

        for (int i = 0; i < size; i++)
        {
            if (ch == 'y' || ch == 'Y')
            {
                ver[pyr - 1][n - 1] = 1;
                board[r + i][c] = 'S';
            }
            else
                board[r][c + i] = 'S';
        }
        r_arr[pyr - 1][n - 1] = r;
        c_arr[pyr - 1][n - 1] = c;
        f++;
        printBoard(board, false);
    }
}

// Player attack function
void playerattack(char board[10][10], int& pyrscore)
{
    bool valid;
    int r1, c1;
    int ship;
    bool found;

    do
    {
        valid = true;

        cout << "Enter the row number for attack: ";
        cin >> r1;
        cout << "Enter the column number for attack: ";
        cin >> c1;

        if (board[r1][c1] == 'S')
        {
            cout << "HIT!" << endl;
            board[r1][c1] = 'X';
            pyrscore += 10;

        }
        else if (board[r1][c1] == 'O' || board[r1][c1] == 'X')
        {
            cout << "Already attacked" << endl;
            valid = false;
        }
        else
        {
            pyrscore -= 1;

            board[r1][c1] = 'O';

        }

    } while (!valid);

    // Health bar setting
    for (int i = 0; i < 5; i++)
    {
        found = false;
        if (ver[pyr - 1][i] == 0)
        {
            for (int j = 0; j < sizes[i]; j++)
            {
                if (r1 == r_arr[pyr - 1][i] && c1 == c_arr[pyr - 1][i] + j)
                {
                    found = true;
                    hlth[pyr - 1][i] -= 1;
                    ship = i;
                    break;
                }
            }
        }
        else
        {
            for (int k = 0; k < sizes[i]; k++)
            {
                if (r1 == r_arr[pyr - 1][i] + k && c1 == c_arr[pyr - 1][i])
                {
                    found = true;
                    ship = i;
                    hlth[pyr - 1][i] -= 1;
                    break;
                }
            }
        }

        if (hlth[pyr - 1][i] <= 0)
        {
            cout << "You have destroyed ship number " << i + 1 << endl;
            hlth[pyr - 1][i] = 0;
        }

        if (found)
            break;
    }
}

// Player vs Player mode
void vsPlayer(char myboard[10][10], char oppboard[10][10])
{
    bool over = false;
    while (!over)
    {
        int turns1 = turn_c(1) + 1;
        int turns2 = turn_c(2) + 1;

        cout << "\n" << player1name << "'s turn" << endl;
        for (int i = 0; i < turns1; i++)
        {
            printBoard(oppboard, true);
            pyr = 2;
            playerattack(oppboard, player1score);
            if (turn_c(2) == 0)
            {
                cout << player1name << " has destroyed all the ships!" << endl;
                over = true;
                break;
            }
        }

        if (over)
            break;

        // Pass the keyboard screen
        clearScreen();
        setColor(13, 0);
        cout << "\n\n\n\n\n\n\n\n\n\n";
        cout << "========================================================================================================================\n\n";
        setColor(7, 0);
        cout << "                                         " << player1name << "'s turn is over!\n";
        cout << "                                      Pass the keyboard to " << player2name << "\n\n";
        setColor(13, 0);
        cout << "========================================================================================================================\n\n";

        cout << "\n\n                                         Press any key when ready...\n";
        (void)_getch();
        clearScreen();

        cout << "\n" << player2name << "'s turn" << endl;
        for (int i = 0; i < turns2; i++)
        {
            printBoard(myboard, true);
            pyr = 1;
            playerattack(myboard, player2score);
            if (turn_c(1) == 0)
            {
                cout << player2name << " has destroyed all the ships!" << endl;
                over = true;
                break;
            }
        }
    }
}

// ========== LEADERBOARD FUNCTIONS ==========

// Load leaderboard from file
void loadLeaderboard(string playerNames[], int scores[], int& totalPlayers)
{
    ifstream inFile("highscores.txt");
    totalPlayers = 0;

    if (inFile.is_open())
    {
        string name;
        int score;

        while (inFile >> name >> score && totalPlayers < 100)
        {
            playerNames[totalPlayers] = name;
            scores[totalPlayers] = score;
            totalPlayers++;
        }

        inFile.close();
    }
}

// Save leaderboard to file
void saveLeaderboard(const string playerNames[], const int scores[], int totalPlayers)
{
    ofstream outFile("highscores.txt");

    if (outFile.is_open())
    {
        // Save only top 10 players
        int limit = (totalPlayers < 10) ? totalPlayers : 10;

        for (int i = 0; i < limit; i++)
        {
            outFile << playerNames[i] << " " << scores[i] << endl;
        }

        outFile.close();
    }
}

// Sort leaderboard in descending order (bubble sort)
void sortLeaderboard(string playerNames[], int scores[], int totalPlayers)
{
    for (int i = 0; i < totalPlayers - 1; i++)
    {
        for (int j = 0; j < totalPlayers - i - 1; j++)
        {
            if (scores[j] < scores[j + 1])
            {
                // Swap scores
                int tempScore = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = tempScore;

                // Swap names
                string tempName = playerNames[j];
                playerNames[j] = playerNames[j + 1];
                playerNames[j + 1] = tempName;
            }
        }
    }
}

// Update leaderboard with new player score
void updateLeaderboard(string playerNames[], int scores[], int& totalPlayers, string newPlayer, int newScore)
{
    // Check if player already exists
    bool found = false;
    for (int i = 0; i < totalPlayers; i++)
    {
        if (playerNames[i] == newPlayer)
        {
            // Update score if new score is higher
            if (newScore > scores[i])
            {
                scores[i] = newScore;
            }
            found = true;
            break;
        }
    }

    // If player doesn't exist, add them
    if (!found && totalPlayers < 100)
    {
        playerNames[totalPlayers] = newPlayer;
        scores[totalPlayers] = newScore;
        totalPlayers++;
    }

    // Sort the leaderboard
    sortLeaderboard(playerNames, scores, totalPlayers);
}

// Display leaderboard
void showLeaderboard(const string playerNames[], const int scores[], int totalPlayers)
{
    setColor(13, 0);
    cout << "========================================================================================================================\n";
    setColor(7, 0);
    cout << "                                          LEADERBOARD                  " << endl;
    setColor(13, 0);
    cout << "========================================================================================================================\n";

    if (totalPlayers == 0)
    {
        setColor(7, 0);
        cout << "No scores recorded yet!" << endl;
    }
    else
    {
        setColor(7, 0);
        cout << "\nRank   Player Name        Score" << endl;
        cout << "----   -----------        -----" << endl;

        int limit = (totalPlayers < 10) ? totalPlayers : 10;

        for (int i = 0; i < limit; i++)
        {
            cout << " " << i + 1;
            if (i < 9)
                cout << "     ";
            else
                cout << "    ";

            cout << playerNames[i];

            // Add spacing
            int spaces = (int)(20 - playerNames[i].length());
            for (int j = 0; j < spaces; j++)
                cout << " ";

            cout << scores[i] << endl;
        }
    }
    setColor(13, 0);
    cout << "========================================================================================================================\n\n";
}

// Display result and update leaderboard
void displayResult(int player1Score, int player2Score)
{
    cout << "\n========================================" << endl;
    cout << "           GAME OVER!                   " << endl;
    cout << "========================================" << endl;

    cout << "\nFinal Scores:" << endl;
    cout << player1name << ": " << player1Score << endl;
    cout << player2name << ": " << player2Score << endl;

    if (player1Score > player2Score)
    {
        cout << "\n" << player1name << " WINS!" << endl;
        // Update leaderboard with winner's score
        updateLeaderboard(leaderboardNames, leaderboardScores, totalPlayersInFile, player1name, player1Score);
    }
    else if (player2Score > player1Score)
    {
        cout << "\n" << player2name << " WINS!" << endl;
        // Update leaderboard with winner's score
        updateLeaderboard(leaderboardNames, leaderboardScores, totalPlayersInFile, player2name, player2Score);
    }
    else
    {
        cout << "\nIt's a DRAW!" << endl;
        // Update leaderboard for both in case of draw
        updateLeaderboard(leaderboardNames, leaderboardScores, totalPlayersInFile, player1name, player1Score);
        updateLeaderboard(leaderboardNames, leaderboardScores, totalPlayersInFile, player2name, player2Score);
    }

    // Save updated leaderboard to file
    saveLeaderboard(leaderboardNames, leaderboardScores, totalPlayersInFile);

    cout << "\nLeaderboard has been updated!" << endl;
}

// Show game menu
void showGameMenu()
{
    int choice, shipChoice;
    bool exitGame = false;

    // Load leaderboard at start
    loadLeaderboard(leaderboardNames, leaderboardScores, totalPlayersInFile);

    while (!exitGame)
    {
        setColor(13, 0); // yellow borders
        cout << "\n========================================" << endl;
        setColor(7, 0); // cyan title
        cout << "       BATTLESHIP GAME MENU             " << endl;
        setColor(13, 0); // yellow border
        cout << "========================================" << endl;
        setColor(7, 0);  // white options
        cout << "1. Start New Game" << endl;
        cout << "2. Instructions" << endl;
        cout << "3. View Leaderboard" << endl;
        cout << "4. Exit" << endl;
        setColor(13, 0);
        cout << "========================================" << endl;
        setColor(7, 0);
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1: // Start New Game
        {
            clearScreen();
            int gameMode;

            cout << "\n\n\n\n\n\n";
            setColor(13, 0);
            cout << "========================================================================================================================\n";
            cout << "                                          \n";
            setColor(7, 0);
            cout << "                                               G A M E   M O D E          \n";


            setColor(13, 0);
            cout << "\n";
            cout << "========================================================================================================================\n";
            cout << "\n\n";
            cout << "                                                  Select One Option \n";


            cout << endl;
            setColor(13, 0);
            cout << "1. "; setColor(7, 0);
            cout << "Player vs Computer" << endl;
            setColor(13, 0);
            cout << "2. "; setColor(7, 0);
            cout << "Player vs Player" << endl;
            cout << endl;
            setColor(13, 0);
            cout << "Enter your choice: ";
            cin >> gameMode;
            setColor(7, 0);
            if (gameMode == 1)
            {
                char playerBoard[10][10];
                char computerBoard[10][10];
                vsComputer(playerBoard, computerBoard);
            }
            else if (gameMode == 2)
            {
                // Reset game variables
                player1score = 0;
                player2score = 0;
                for (int i = 0; i < 2; i++)
                {
                    for (int j = 0; j < 5; j++)
                    {
                        hlth[i][j] = sizes[j];
                        ver[i][j] = 0;
                    }
                }

                // Initialize boards
                initializeBoard(board1);
                initializeBoard(board2);

                cout << "\nEnter Player 1 name: ";
                cin >> player1name;
                cout << "Enter Player 2 name: ";
                cin >> player2name;

                // Player 1 setup
                cout << "\n=== " << player1name << " SETUP ===" << endl;
                cout << "How would you like to place your ships?" << endl;
                cout << "1. Manual Placement" << endl;
                cout << "2. Random Placement" << endl;
                cout << "Enter your choice: ";
                cin >> shipChoice;

                pyr = 1;
                if (shipChoice == 1)
                {
                    setManualships(board1);
                }
                else
                {
                    setrandships(board1);
                }

                // Player 2 setup
                cout << "\n=== " << player2name << " SETUP ===" << endl;
                cout << "How would you like to place your ships?" << endl;
                cout << "1. Manual Placement" << endl;
                cout << "2. Random Placement" << endl;
                cout << "Enter your choice: ";
                cin >> shipChoice;

                pyr = 2;
                if (shipChoice == 1)
                {
                    setManualships(board2);
                }
                else
                {
                    setrandships(board2);
                }

                // Start game
                cout << "\n========================================" << endl;
                cout << "       LET THE BATTLE BEGIN!            " << endl;
                cout << "========================================" << endl;

                vsPlayer(board1, board2);

                // Display results
                displayResult(player1score, player2score);
            }
            else
            {
                cout << "Invalid choice!" << endl;
            }
            break;
        }

        case 2: // Instructions 
            clearScreen();
            setColor(13, 0);
            cout << "========================================================================================================================\n"; setColor(7, 0);
            cout << "                                          GAME INSTRUCTIONS            " << endl; setColor(13, 0);
            cout << "========================================================================================================================\n\n";
            setColor(13, 0);
            cout << "1. "; setColor(7, 0);
            cout << "Each player places 5 ships on a 10x10 grid" << endl;
            setColor(13, 0);
            cout << "2. "; setColor(7, 0);
            cout << "Ships: Aircraft Carrier(5), Battleship(4)," << endl;
            cout << "   Destroyer(3), Submarine(3), Patrol Boat(2)" << endl;
            setColor(13, 0);
            cout << "3. "; setColor(7, 0);
            cout << "Players take turns attacking opponent's grid" << endl;
            setColor(13, 0);
            cout << "4. "; setColor(7, 0);
            cout << "Each turn, you get (remaining ships + 1) attacks" << endl;
            setColor(13, 0);
            cout << "5. "; setColor(7, 0);
            cout << "Hit = +10 points, Miss = -1 point" << endl;
            setColor(13, 0);
            cout << "6. "; setColor(7, 0);
            cout << "Red 'X' = Hit, Pink 'X' = Sunk, Grey 'O' = Miss" << endl;
            setColor(13, 0);
            cout << "7. "; setColor(7, 0);
            cout << "First to sink all opponent ships wins!" << endl << endl; setColor(13, 0);
            cout << "========================================================================================================================\n\n";
            cout << "Press any key to return to Menu \n";
            (void)_getch();
            clearScreen();
            break;

        case 3: // View Leaderboard
            clearScreen();
            showLeaderboard(leaderboardNames, leaderboardScores, totalPlayersInFile);
            cout << "Press any key to return to Menu \n";
            (void)_getch();
            clearScreen();
            break;

        case 4: // Exit
            cout << "\nThank you for playing Battleship!" << endl;
            exitGame = true;
            break;

        default:
            cout << "Invalid choice! Please try again." << endl;
        }
    }
}

// Main function
int main()
{
    srand((unsigned int)time(0));


    clearScreen();
    cout << "\n\n\n\n\n\n";
    setColor(13, 0);
    cout << "========================================================================================================================\n";
    cout << "                                          \n";
    setColor(7, 0);
    cout << "                                          B  A  T  T  L  E  S  H  I  P          \n";
    cout << "\n";
    setColor(13, 0);
    cout << "                                                      |\n";
    cout << "                                                  ____|____\n";
    cout << "                                                 |  * * *  |\n";
    cout << "                                              ___|_________|___\n";
    cout << "                                             |_________________|\n";
    cout << "                                            ~~~~~~~~~~~~~~~~~~~~~\n";
    cout << "\n";
    setColor(7, 0);
    cout << "                                            ~ Sink or be sunk ~\n";
    setColor(13, 0);
    cout << "\n";
    cout << "========================================================================================================================\n";

    cout << "\n";
    setColor(13, 0);
    cout << "                                          Press any key to continue...\n";
    (void)_getch();


    clearScreen();



    cout << "\n";
    setColor(13, 0);
    cout << "========================================================================================================================\n";
    cout << "                                          \n";
    setColor(7, 0);
    cout << "                                          B  A  T   T  L  E  S  H  I  P          \n";
    setColor(13, 0);
    cout << "                                         \n";

    setColor(7, 0);
    cout << "                                              ~ Sink or be sunk ~\n";
    setColor(13, 0);
    cout << "\n";
    cout << "========================================================================================================================\n";

    cout << "\n";
    //setColor(13, 0);
    //cout << "                                          Press any key to continue...\n";
    //(void)_getch();







    showGameMenu();
    return 0;
}

void printTurnScreen(char board[10][10], bool hide, string turnLabel, int attackNum, int totalAttacks, int playerAlive, int compAlive, int playerScore, int compScore) {
    clearScreen();
    setColor(13, 0);
    cout << "==========================================\n"; setColor(7, 0);
    cout << "             BATTLESHIP\n"; setColor(13, 0);
    cout << "==========================================\n"; setColor(7, 0);
    cout << "  " << turnLabel << "        Attack " << attackNum << " of " << totalAttacks << "\n"; setColor(13, 0);
    cout << "==========================================\n\n"; setColor(7, 0);
    printBoard(board, hide);


    cout << "\n\n  Score ->  You: " << playerScore << "   |   Computer: " << compScore << "\n";
}

void vsComputer(char playerBoard[10][10], char computerBoard[10][10]) {
    initializeBoard(playerBoard);
    initializeBoard(computerBoard);
    compshipsalive = 5;
    playershipsalive = 5;

    clearScreen();
    setColor(13, 0);
    cout << "==========================================\n";
    setColor(7, 0);
    cout << "             BATTLESHIP\n";
    setColor(13, 0);
    cout << "==========================================\n";
    setColor(7, 0);
    cout << "Enter your name: ";
    cin >> player1name;
    player2name = "Computer";

    cout << "\n--- SETUP PHASE ---\n";
    mereplayerkiShips(playerBoard);
    cout << "\nYour Board:\n";
    printBoard(playerBoard, false);
    cout << "\nComputer is placing ships...\n";
    setRandomShips(computerBoard);
    setColor(13, 0);
    cout << "Done! Press any key to start.\n";
    (void)_getch();

    int playerscore = 0;
    int compscore = 0;
    int totalAttacks, attackRow, attackCol, result;

    while (playershipsalive > 0 && compshipsalive > 0) {

        // --- Player's turn ---
        totalAttacks = compshipsalive + 1;
        for (int i = 0; i < totalAttacks; i++) {
            printTurnScreen(computerBoard, true, "YOUR TURN", i + 1, totalAttacks,
                playershipsalive, compshipsalive, playerscore, compscore);
            result = merayplayerkaattack(computerBoard, playerscore);
            if (result == 1) {
                setColor(15, 4); cout << "\n  >> HIT!"; setColor(7, 0); cout << "\n";
            }
            else {
                setColor(0, 8); cout << "\n  >> MISS!"; setColor(7, 0); cout << "\n";
            }
            if (compshipsalive == 0) break;
            setColor(7, 0);
            cout << "  Press any key for next attack...\n";
            (void)_getch();
        }























        if (compshipsalive == 0) {
            clearScreen();
            setColor(13, 0);
            cout << "==========================================\n";
            setColor(7, 0);
            cout << "       CONGRATULATIONS! YOU WON!\n"; setColor(13, 0);
            cout << "==========================================\n";
            break;
        }
        setColor(13, 0);
        cout << "  Press any key for computer's turn...\n";
        (void)_getch();


        // --- Computer's turn ---
        totalAttacks = playershipsalive + 1;
        for (int i = 0; i < totalAttacks; i++) {
            clearScreen();
            cout << "==========================================\n";
            cout << "  COMPUTER'S TURN       Attack " << i + 1 << " of " << totalAttacks << "\n";
            cout << "==========================================\n\n";
            printBoard(playerBoard, false);
            cout << "\n  Score ->  You: " << playerscore << "   |   Computer: " << compscore << "\n";
            cout << "==========================================\n";

            result = computerAttack(playerBoard, compscore, attackRow, attackCol);
            cout << "\n  Computer attacked (" << attackRow << ", " << attackCol << ") -> ";
            if (result == 1) { setColor(15, 4); cout << "HIT!\n"; setColor(7, 0); }
            else { setColor(0, 8);  cout << "MISS!\n"; setColor(7, 0); }

            if (playershipsalive == 0) break;
            setColor(13, 0);
            cout << "  Press any key for computer's next attack...\n";
            (void)_getch();
        }


    }

    // --- Game Over ---
    clearScreen();
    setColor(13, 0);
    cout << "==========================================\n"; setColor(7, 0);
    cout << "             GAME OVER\n"; setColor(13, 0);
    cout << "==========================================\n";  setColor(7, 0);
    cout << "  Player:   " << playerscore << "\n";
    cout << "  Computer: " << compscore << "\n"; setColor(13, 0);
    cout << "==========================================\n";
    if (playerscore > compscore) { setColor(15, 2); cout << "  YOU WON!\n";   setColor(7, 0); }
    else if (playerscore < compscore) { setColor(15, 4); cout << "  YOU LOSE!\n";  setColor(7, 0); }
    else { cout << "  IT'S A TIE!\n"; }

    updateLeaderboard(leaderboardNames, leaderboardScores, totalPlayersInFile, player1name, playerscore);
    saveLeaderboard(leaderboardNames, leaderboardScores, totalPlayersInFile);
    setColor(13, 0);
    cout << "\n  Press any key to return to menu...\n";
    (void)_getch();



}

void setRandomShips(char board[10][10]) {
    //ships ke naam
    /*1= AirCraft Carrier 5 blocks
    2= Battleship 4 blocks
    3= Destroyer 3 blocks
    4= Submarine 3 blocks
    5= Patrol Boat 2 blocks
 */
    int ships[5] = { 5,4,3,3,2 };
    char shipsclr[5] = { '1','2','3','4','5' };

    for (int i = 0; i < 5; i++) {
        shipscolour[i] = rand() % 14;

        while (shipscolour[i] == 0 || shipscolour[i] == 1 || shipscolour[i] == 8 || shipscolour[i] == 12 || shipscolour[i] == 4 || shipscolour[i] == 13 || shipscolour[i] == 8 || shipscolour[i] == 7 || shipscolour[i] == 11 || shipscolour[i] == 9) {
            shipscolour[i] = rand() % 14;
        }
        bool duplicate = false;
        for (int j = 0; j < i; j++) {
            if (shipscolour[i] == shipscolour[j]) {
                duplicate = true;
                break;
            }
        }
        if (duplicate) { i--; }
    }

    for (int i = 0; i < 5; i++) {

        int row = rand() % 10;
        int column = rand() % 10;
        int allign = rand() % 2;
        if (allign == 1) { //horizontal
            for (int j = 0; j < ships[i]; j++) {
                while (board[row][column + j] != '~')  //cells khali hain ya nahi
                {
                    row = rand() % 10;
                    column = rand() % 10;
                }
            }


            while ((column + ships[i]) > 9) {  //random number valid hay ya nahi 
                row = rand() % 10;
                column = rand() % 10;
            }


            for (int k = 0; k < ships[i]; k++) {
                if (board[row][column + k] != '~') {
                    i--;
                    goto next;
                }
            }
            for (int k = 0; k < ships[i]; k++) {
                board[row][column + k] = shipsclr[i];
            }
        }
        else if (allign == 0) { //vertical
            for (int j = 0; j < ships[i]; j++) {
                while (board[row + j][column] != '~')  //cells khali hain ya nahi
                {
                    row = rand() % 10;
                    column = rand() % 10;
                }
            }


            while ((row + ships[i]) > 9) {  //random number valid hay ya nahi 
                row = rand() % 10;
                column = rand() % 10;
            }


            for (int k = 0; k < ships[i]; k++) {
                if (board[row + k][column] != '~') {
                    i--;
                    goto next;
                }
            }
            for (int k = 0; k < ships[i]; k++) {
                board[row + k][column] = shipsclr[i];
            }

        }

    next:;
    }

}

int computerAttack(char playerBoard[10][10], int& computerScore, int& attackRow, int& attackCol)
{
    static int shiplife1 = 0, shiplife2 = 0, shiplife3 = 0, shiplife4 = 0, shiplife5 = 0;
    int row, column;

    row = rand() % 10;
    column = rand() % 10;
    while (playerBoard[row][column] == 'O' || playerBoard[row][column] == 'X' ||
        playerBoard[row][column] == 'A' || playerBoard[row][column] == 'p' ||
        playerBoard[row][column] == 'q' || playerBoard[row][column] == 'r' ||
        playerBoard[row][column] == 's' || playerBoard[row][column] == 't') {
        row = rand() % 10;
        column = rand() % 10;
    }

    attackRow = row;
    attackCol = column;
    int result = 0;

    if (playerBoard[row][column] == '1') { playerBoard[row][column] = 'p'; computerScore += 10; shiplife1++; result = 1; }
    else if (playerBoard[row][column] == '2') { playerBoard[row][column] = 'q'; computerScore += 10; shiplife2++; result = 1; }
    else if (playerBoard[row][column] == '3') { playerBoard[row][column] = 'r'; computerScore += 10; shiplife3++; result = 1; }
    else if (playerBoard[row][column] == '4') { playerBoard[row][column] = 's'; computerScore += 10; shiplife4++; result = 1; }
    else if (playerBoard[row][column] == '5') { playerBoard[row][column] = 't'; computerScore += 10; shiplife5++; result = 1; }
    else if (playerBoard[row][column] == '~') { playerBoard[row][column] = 'O'; computerScore -= 1;  result = 0; }

    playershipsalive = 0;
    if (shiplife1 < 5) playershipsalive++;
    if (shiplife2 < 4) playershipsalive++;
    if (shiplife3 < 3) playershipsalive++;
    if (shiplife4 < 3) playershipsalive++;
    if (shiplife5 < 2) playershipsalive++;

    if (shiplife1 == 5) { for (int z = 0; z < 10; z++) for (int y = 0; y < 10; y++) if (playerBoard[z][y] == 'p') playerBoard[z][y] = 'A'; }
    if (shiplife2 == 4) { for (int z = 0; z < 10; z++) for (int y = 0; y < 10; y++) if (playerBoard[z][y] == 'q') playerBoard[z][y] = 'A'; }
    if (shiplife3 == 3) { for (int z = 0; z < 10; z++) for (int y = 0; y < 10; y++) if (playerBoard[z][y] == 'r') playerBoard[z][y] = 'A'; }
    if (shiplife4 == 3) { for (int z = 0; z < 10; z++) for (int y = 0; y < 10; y++) if (playerBoard[z][y] == 's') playerBoard[z][y] = 'A'; }
    if (shiplife5 == 2) { for (int z = 0; z < 10; z++) for (int y = 0; y < 10; y++) if (playerBoard[z][y] == 't') playerBoard[z][y] = 'A'; }

    return result;
}


int merayplayerkaattack(char computerBoard[10][10], int& playerScore)
{
    static int shiplife1 = 0, shiplife2 = 0, shiplife3 = 0, shiplife4 = 0, shiplife5 = 0;
    int row, column;

m:
    cout << "Enter row to attack (0-9): ";
    cin >> row;
    if (row < 0 || row > 9) { cout << "Invalid row.\n"; goto m; }
n:
    cout << "Enter column to attack (0-9): ";
    cin >> column;
    if (column < 0 || column > 9) { cout << "Invalid column.\n"; goto n; }

    while (computerBoard[row][column] == 'O' || computerBoard[row][column] == 'X' ||
        computerBoard[row][column] == 'A' || computerBoard[row][column] == 'p' ||
        computerBoard[row][column] == 'q' || computerBoard[row][column] == 'r' ||
        computerBoard[row][column] == 's' || computerBoard[row][column] == 't') {
        cout << "Already attacked. Choose again.\n";
        goto m;
    }

    int result = 0;
    if (computerBoard[row][column] == '1') { computerBoard[row][column] = 'p'; playerScore += 10; shiplife1++; result = 1; }
    else if (computerBoard[row][column] == '2') { computerBoard[row][column] = 'q'; playerScore += 10; shiplife2++; result = 1; }
    else if (computerBoard[row][column] == '3') { computerBoard[row][column] = 'r'; playerScore += 10; shiplife3++; result = 1; }
    else if (computerBoard[row][column] == '4') { computerBoard[row][column] = 's'; playerScore += 10; shiplife4++; result = 1; }
    else if (computerBoard[row][column] == '5') { computerBoard[row][column] = 't'; playerScore += 10; shiplife5++; result = 1; }
    else if (computerBoard[row][column] == '~') { computerBoard[row][column] = 'O'; playerScore -= 1;  result = 0; }

    compshipsalive = 0;
    if (shiplife1 < 5) compshipsalive++;
    if (shiplife2 < 4) compshipsalive++;
    if (shiplife3 < 3) compshipsalive++;
    if (shiplife4 < 3) compshipsalive++;
    if (shiplife5 < 2) compshipsalive++;

    if (shiplife1 == 5) { for (int z = 0; z < 10; z++) for (int y = 0; y < 10; y++) if (computerBoard[z][y] == 'p') computerBoard[z][y] = 'A'; }
    if (shiplife2 == 4) { for (int z = 0; z < 10; z++) for (int y = 0; y < 10; y++) if (computerBoard[z][y] == 'q') computerBoard[z][y] = 'A'; }
    if (shiplife3 == 3) { for (int z = 0; z < 10; z++) for (int y = 0; y < 10; y++) if (computerBoard[z][y] == 'r') computerBoard[z][y] = 'A'; }
    if (shiplife4 == 3) { for (int z = 0; z < 10; z++) for (int y = 0; y < 10; y++) if (computerBoard[z][y] == 's') computerBoard[z][y] = 'A'; }
    if (shiplife5 == 2) { for (int z = 0; z < 10; z++) for (int y = 0; y < 10; y++) if (computerBoard[z][y] == 't') computerBoard[z][y] = 'A'; }

    return result;
}

void mereplayerkiShips(char board[10][10]) {
    //ships ke naam
    /*1= AirCraft Carrier 5 blocks
    2= Battleship 4 blocks
    3= Destroyer 3 blocks
    4= Submarine 3 blocks
    5= Patrol Boat 2 blocks */
    setColor(7, 0);
    cout << endl;
    cout << player1name << "!  PLACE YOUR SHIPS \n\n";
    char xd;

    while (true) {
        cout << "Place randomly? (y/n): ";
        cin >> xd;
        if (xd == 'y' || xd == 'Y') { setRandomShips(board); return; }
        if (xd == 'n' || xd == 'N') break;
        cout << "Type y or n.\n";
    }


    bool shipsplaced[5] = { false,false,false,false,false };
    const char* shipNames[5] = { "Aircraft Carrier","Battleship","Destroyer","Submarine","Patrol Boat" };
    const int shipSizes[5] = { 5,4,3,3,2 };
    const char shipMarkers[5] = { '1','2','3','4','5' };
    const int validColours[] = { 2,3,5,6,14 };
    int placedCount = 0;

    while (placedCount < 5) {

        // Ship selection
        int cd;
        while (true) {
            cout << "\nSelect ship to place:\n";
            for (int i = 0; i < 5; i++) {
                cout << (i + 1) << ") " << shipNames[i] << " (" << shipSizes[i] << " blocks)";
                if (shipsplaced[i]) cout << " [PLACED]";
                cout << "\n";
            }
            cin >> cd;
            if (cd < 1 || cd > 5) { cout << "Invalid.\n"; continue; }
            if (shipsplaced[cd - 1]) { cout << "Already placed.\n"; continue; }
            break;
        }

        int shipIdx = cd - 1;
        int size = shipSizes[shipIdx];

        // Colour selection
        while (true) {
            cout << "Colour for " << shipNames[shipIdx] << ":\n";
            cout << "2.Green  3.Cyan  5.Purple  6.Yellow  14.Beige\n";
            cin >> shipscolour[shipIdx];
            bool ok = false;
            for (int i = 0; i < 5; i++) if (shipscolour[shipIdx] == validColours[i]) { ok = true; break; }
            if (ok) break;
            cout << "Invalid colour.\n";
        }

        // Orientation
        char allign;
        while (true) {
            cout << "Vertical? (y/n): ";
            cin >> allign;
            if (allign == 'y' || allign == 'Y' || allign == 'n' || allign == 'N') break;
            cout << "Type y or n.\n";
        }
        bool vertical = (allign == 'y' || allign == 'Y');

        // Row/column input + validation
        int row, column;
        int maxRow = vertical ? (10 - size) : 9;
        int maxCol = vertical ? 9 : (10 - size);

        while (true) {
            cout << "Starting row (0-" << maxRow << "): ";
            cin >> row;
            if (row < 0 || row > maxRow) { cout << "Invalid row.\n"; continue; }

            cout << "Starting column (0-" << maxCol << "): ";
            cin >> column;
            if (column < 0 || column > maxCol) { cout << "Invalid column.\n"; continue; }

            bool overlap = false;
            for (int k = 0; k < size; k++) {
                int r = row + (vertical ? k : 0);
                int c = column + (vertical ? 0 : k);
                if (board[r][c] != '~') { overlap = true; break; }
            }
            if (overlap) { cout << "Overlap! Try again.\n"; continue; }

            break;
        }

        // Place ship
        for (int k = 0; k < size; k++) {
            int r = row + (vertical ? k : 0);
            int c = column + (vertical ? 0 : k);
            board[r][c] = shipMarkers[shipIdx];
        }

        shipsplaced[shipIdx] = true;
        placedCount++;
        cout << shipNames[shipIdx] << " placed!\n";
        printBoard(board, false);
    }

}


