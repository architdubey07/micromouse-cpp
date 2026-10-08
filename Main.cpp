// ==========================================================
//             MICROMOUSE - FLOOD FILL
// ==========================================================

#define N 16

int maze[N][N];
int dist[N][N];

int x = 0;
int y = 0;

// Direction:
// 0 = North
// 1 = East
// 2 = South
// 3 = West
int dir = 0;


// ----------------------------------------------------------
// Initialize flood-fill distances
// Goal = center 2x2 cells
// ----------------------------------------------------------

void initDistances()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int d1 = abs(i - 7) + abs(j - 7);
            int d2 = abs(i - 7) + abs(j - 8);
            int d3 = abs(i - 8) + abs(j - 7);
            int d4 = abs(i - 8) + abs(j - 8);

            dist[i][j] = min(min(d1, d2), min(d3, d4));
        }
    }
}


// ----------------------------------------------------------
// Read walls around current cell
// ----------------------------------------------------------

void readWalls()
{
    bool front = wallFront();
    bool left  = wallLeft();
    bool right = wallRight();

    // Store walls according to current direction

    if (front)
        maze[y][x] |= (1 << dir);

    if (left)
        maze[y][x] |= (1 << ((dir + 3) % 4));

    if (right)
        maze[y][x] |= (1 << ((dir + 1) % 4));
}


// ----------------------------------------------------------
// Check whether movement is possible
// ----------------------------------------------------------

bool canMove(int nx, int ny)
{
    if (nx < 0 || nx >= N || ny < 0 || ny >= N)
        return false;

    return true;
}


// ----------------------------------------------------------
// Recalculate flood-fill values
// ----------------------------------------------------------

void floodFill()
{
    bool changed = true;

    while (changed)
    {
        changed = false;

        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                // Don't modify goal cells
                if ((i == 7 || i == 8) &&
                    (j == 7 || j == 8))
                    continue;

                int best = 1000;

                // North
                if (i + 1 < N)
                {
                    if (!(maze[i][j] & (1 << 0)))
                        best = min(best, dist[i + 1][j] + 1);
                }

                // East
                if (j + 1 < N)
                {
                    if (!(maze[i][j] & (1 << 1)))
                        best = min(best, dist[i][j + 1] + 1);
                }

                // South
                if (i - 1 >= 0)
                {
                    if (!(maze[i][j] & (1 << 2)))
                        best = min(best, dist[i - 1][j] + 1);
                }

                // West
                if (j - 1 >= 0)
                {
                    if (!(maze[i][j] & (1 << 3)))
                        best = min(best, dist[i][j - 1] + 1);
                }

                if (dist[i][j] != best)
                {
                    dist[i][j] = best;
                    changed = true;
                }
            }
        }
    }
}


// ----------------------------------------------------------
// Choose best neighboring cell
// ----------------------------------------------------------

int bestDirection()
{
    int bestDir = dir;
    int bestDist = 1000;

    // North
    if (y + 1 < N &&
        !(maze[y][x] & (1 << 0)))
    {
        if (dist[y + 1][x] < bestDist)
        {
            bestDist = dist[y + 1][x];
            bestDir = 0;
        }
    }

    // East
    if (x + 1 < N &&
        !(maze[y][x] & (1 << 1)))
    {
        if (dist[y][x + 1] < bestDist)
        {
            bestDist = dist[y][x + 1];
            bestDir = 1;
        }
    }

    // South
    if (y - 1 >= 0 &&
        !(maze[y][x] & (1 << 2)))
    {
        if (dist[y - 1][x] < bestDist)
        {
            bestDist = dist[y - 1][x];
            bestDir = 2;
        }
    }

    // West
    if (x - 1 >= 0 &&
        !(maze[y][x] & (1 << 3)))
    {
        if (dist[y][x - 1] < bestDist)
        {
            bestDist = dist[y][x - 1];
            bestDir = 3;
        }
    }

    return bestDir;
}


// ----------------------------------------------------------
// Rotate to required direction
// ----------------------------------------------------------

void rotateTo(int target)
{
    int diff = (target - dir + 4) % 4;

    if (diff == 1)
    {
        turnRight();
    }
    else if (diff == 2)
    {
        turnRight();
        turnRight();
    }
    else if (diff == 3)
    {
        turnLeft();
    }

    dir = target;
}


// ----------------------------------------------------------
// Move to next cell
// ----------------------------------------------------------

void moveTo(int target)
{
    rotateTo(target);

    moveForward();

    if (dir == 0)
        y++;

    else if (dir == 1)
        x++;

    else if (dir == 2)
        y--;

    else if (dir == 3)
        x--;
}


// ----------------------------------------------------------
// Main
// ----------------------------------------------------------

void setup()
{
    initDistances();

    // Start position
    x = 0;
    y = 0;
    dir = 0;
}


void loop()
{
    // Stop when center is reached
    if ((x == 7 || x == 8) &&
        (y == 7 || y == 8))
    {
        return;
    }

    // Detect walls
    readWalls();

    // Update flood-fill
    floodFill();

    // Find best direction
    int nextDir = bestDirection();

    // Move
    moveTo(nextDir);
}
