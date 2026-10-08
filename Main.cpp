// ==========================================================
//                 MICROMOUSE - FLOOD FILL
//                    MMS C++ VERSION
// ==========================================================

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include "API.h"

using namespace std;

#define N 16

// ==========================================================
// COORDINATES
// ==========================================================
// x -> East / West
// y -> North / South
//
// Direction:
// 0 = North
// 1 = East
// 2 = South
// 3 = West
// ==========================================================

int x = 0;
int y = 0;
int dir = 0;

// wall[y][x]
// Bit 0 = North
// Bit 1 = East
// Bit 2 = South
// Bit 3 = West
int wall[N][N];

// Flood-fill distance
int dist[N][N];


// ==========================================================
// CHECK IF CURRENT CELL IS GOAL
// ==========================================================

bool isGoal()
{
    return ((x == 7 || x == 8) &&
            (y == 7 || y == 8));
}


// ==========================================================
// SET WALL
// Also sets the opposite wall in neighboring cell
// ==========================================================

void setWall(int cx, int cy, int d)
{
    if (cx < 0 || cx >= N ||
        cy < 0 || cy >= N)
        return;

    // Set wall in current cell
    wall[cy][cx] |= (1 << d);

    // Find neighboring cell
    int nx = cx;
    int ny = cy;

    if (d == 0)       // North
        ny++;

    else if (d == 1)  // East
        nx++;

    else if (d == 2)  // South
        ny--;

    else if (d == 3)  // West
        nx--;

    // Set opposite wall in neighboring cell
    if (nx >= 0 && nx < N &&
        ny >= 0 && ny < N)
    {
        int opposite = (d + 2) % 4;

        wall[ny][nx] |= (1 << opposite);
    }
}


// ==========================================================
// INITIALIZE MAZE
// ==========================================================

void initializeMaze()
{
    // Clear wall information
    for (int y = 0; y < N; y++)
    {
        for (int x = 0; x < N; x++)
        {
            wall[y][x] = 0;
        }
    }

    // South boundary
    for (int x = 0; x < N; x++)
        setWall(x, 0, 2);

    // North boundary
    for (int x = 0; x < N; x++)
        setWall(x, N - 1, 0);

    // West boundary
    for (int y = 0; y < N; y++)
        setWall(0, y, 3);

    // East boundary
    for (int y = 0; y < N; y++)
        setWall(N - 1, y, 1);
}


// ==========================================================
// INITIAL FLOOD-FILL VALUES
// Goal = center 2x2
// ==========================================================

void initializeDistances()
{
    for (int y = 0; y < N; y++)
    {
        for (int x = 0; x < N; x++)
        {
            int d1 = abs(x - 7) + abs(y - 7);
            int d2 = abs(x - 7) + abs(y - 8);
            int d3 = abs(x - 8) + abs(y - 7);
            int d4 = abs(x - 8) + abs(y - 8);

            dist[y][x] =
                min(min(d1, d2),
                    min(d3, d4));
        }
    }
}


// ==========================================================
// READ WALLS FROM MMS
// ==========================================================

void updateWalls()
{
    // Front wall
    if (API::wallFront())
    {
        setWall(x, y, dir);
    }

    // Right wall
    if (API::wallRight())
    {
        setWall(x, y, (dir + 1) % 4);
    }

    // Left wall
    if (API::wallLeft())
    {
        setWall(x, y, (dir + 3) % 4);
    }
}


// ==========================================================
// FLOOD FILL
// ==========================================================

void floodFill()
{
    bool changed = true;

    while (changed)
    {
        changed = false;

        for (int cy = 0; cy < N; cy++)
        {
            for (int cx = 0; cx < N; cx++)
            {
                // Goal cells remain zero
                if ((cx == 7 || cx == 8) &&
                    (cy == 7 || cy == 8))
                {
                    continue;
                }

                int best = 1000;

                // --------------------------
                // NORTH
                // --------------------------

                if (cy + 1 < N &&
                    !(wall[cy][cx] & (1 << 0)))
                {
                    best = min(
                        best,
                        dist[cy + 1][cx] + 1
                    );
                }

                // --------------------------
                // EAST
                // --------------------------

                if (cx + 1 < N &&
                    !(wall[cy][cx] & (1 << 1)))
                {
                    best = min(
                        best,
                        dist[cy][cx + 1] + 1
                    );
                }

                // --------------------------
                // SOUTH
                // --------------------------

                if (cy - 1 >= 0 &&
                    !(wall[cy][cx] & (1 << 2)))
                {
                    best = min(
                        best,
                        dist[cy - 1][cx] + 1
                    );
                }

                // --------------------------
                // WEST
                // --------------------------

                if (cx - 1 >= 0 &&
                    !(wall[cy][cx] & (1 << 3)))
                {
                    best = min(
                        best,
                        dist[cy][cx - 1] + 1
                    );
                }

                // Update distance
                if (dist[cy][cx] != best)
                {
                    dist[cy][cx] = best;
                    changed = true;
                }
            }
        }
    }
}


// ==========================================================
// FIND BEST NEXT DIRECTION
// ==========================================================

int getBestDirection()
{
    int bestDir = -1;
    int bestValue = 1000;

    // --------------------------
    // NORTH
    // --------------------------

    if (y + 1 < N &&
        !(wall[y][x] & (1 << 0)))
    {
        if (dist[y + 1][x] < bestValue)
        {
            bestValue = dist[y + 1][x];
            bestDir = 0;
        }
    }

    // --------------------------
    // EAST
    // --------------------------

    if (x + 1 < N &&
        !(wall[y][x] & (1 << 1)))
    {
        if (dist[y][x + 1] < bestValue)
        {
            bestValue = dist[y][x + 1];
            bestDir = 1;
        }
    }

    // --------------------------
    // SOUTH
    // --------------------------

    if (y - 1 >= 0 &&
        !(wall[y][x] & (1 << 2)))
    {
        if (dist[y - 1][x] < bestValue)
        {
            bestValue = dist[y - 1][x];
            bestDir = 2;
        }
    }

    // --------------------------
    // WEST
    // --------------------------

    if (x - 1 >= 0 &&
        !(wall[y][x] & (1 << 3)))
    {
        if (dist[y][x - 1] < bestValue)
        {
            bestValue = dist[y][x - 1];
            bestDir = 3;
        }
    }

    return bestDir;
}


// ==========================================================
// TURN TO REQUIRED DIRECTION
// ==========================================================

void turnTo(int target)
{
    int diff = (target - dir + 4) % 4;

    // 90° right
    if (diff == 1)
    {
        API::turnRight();
    }

    // 180°
    else if (diff == 2)
    {
        API::turnRight();
        API::turnRight();
    }

    // 90° left
    else if (diff == 3)
    {
        API::turnLeft();
    }

    dir = target;
}


// ==========================================================
// MOVE ONE CELL
// ==========================================================

void moveTo(int target)
{
    // Turn first
    turnTo(target);

    // Move forward
    API::moveForward();

    // Update our coordinates
    if (dir == 0)
    {
        // North
        y++;
    }

    else if (dir == 1)
    {
        // East
        x++;
    }

    else if (dir == 2)
    {
        // South
        y--;
    }

    else if (dir == 3)
    {
        // West
        x--;
    }
}


// ==========================================================
// MAIN
// ==========================================================

int main()
{
    // ------------------------------------
    // INITIALIZATION
    // ------------------------------------

    initializeMaze();
    initializeDistances();

    API::setColor(0, 0, 'G');

    cerr << "Micromouse started!" << endl;


    // ------------------------------------
    // MAIN FLOOD-FILL LOOP
    // ------------------------------------

    while (true)
    {
        // Check goal
        if (isGoal())
        {
            API::setColor(x, y, 'G');
            API::setText(x, y, "GOAL");

            cerr << "=========================" << endl;
            cerr << "       GOAL REACHED      " << endl;
            cerr << "Position: "
                 << x << ", "
                 << y << endl;
            cerr << "=========================" << endl;

            break;
        }


        // --------------------------------
        // 1. READ WALLS
        // --------------------------------

        updateWalls();


        // --------------------------------
        // 2. RECALCULATE FLOOD FILL
        // --------------------------------

        floodFill();


        // --------------------------------
        // 3. FIND BEST CELL
        // --------------------------------

        int nextDir = getBestDirection();


        // --------------------------------
        // SAFETY CHECK
        // --------------------------------

        if (nextDir == -1)
        {
            cerr << "ERROR: No valid direction!" << endl;
            break;
        }


        // --------------------------------
        // 4. MOVE
        // --------------------------------

        moveTo(nextDir);


        // --------------------------------
        // DEBUG
        // --------------------------------

        cerr << "Position: ("
             << x << ", "
             << y << ")  "
             << "Direction: "
             << dir
             << "  Distance: "
             << dist[y][x]
             << endl;
    }

    return 0;
}
