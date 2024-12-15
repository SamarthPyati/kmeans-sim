/* 
** Visualization of K-Means Algorithm using Raylib ** 

    - Author: Samarth Sanjay Pyati 
    - Date: 2-07-2024

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include <unistd.h>
#include "raylib.h"
#include "raymath.h"
#define NOB_IMPLEMENTATION
#include "nob.h"

#define ScreenHeight GetScreenHeight()
#define ScreenWidth GetScreenWidth()

#define POINT_RADIUS 5.0f
#define MEAN_POINT_RADIUS 10.0f
#define K 5                 // clusters
#define MIN_X -20.0
#define MAX_X 20.0
#define MIN_Y -20.0
#define MAX_Y 20.0

const Color bgColor = (Color){33, 34, 38, 255};

// Dynamic array of points
typedef struct 
{
    Vector2 *items;
    size_t count;
    size_t capacity;
} Points;

static float randf(void)
{   
    /* generate random floats between 0 and 1 */
    return ((float)rand() / (float)RAND_MAX);   
}

static float lerpf(const float MIN, const float MAX, const float amount)
{   
    /* Linearly interpolate over MAX and MIN with a certain amount */
    return (amount * (MAX - MIN) + MIN);
}

static void generateRandomClusters(Vector2 centroid, int radius, size_t count, Points *points)
{   
    /* Generate random clusters of points within a particular radius */
    for (size_t i = 0; i < count; ++i)
    {   
        float direction = randf() * 2 * PI;             // angle 
        float magnitude = sqrtf(randf()) * (radius);       // for even distribution of points
        // float magnitude = randf() * (radius);       // for even distribution of points
        float x = centroid.x + magnitude * cosf(direction);
        float y = centroid.y + magnitude * sinf(direction);
        Vector2 point = (Vector2){x, y};
        nob_da_append(points, point);
    }
}

static void clearPoints(Points *points)
{
    points->count = 0;
}

Vector2 GetRandomVector(const float MAX, const float MIN)
{
    return CLITERAL(Vector2) {
        .x = lerpf(MIN, MAX, randf()),
        .y = lerpf(MIN, MAX, randf())
    };
}

static Vector2 projectPointToScreen(Vector2 point)
{
    // Range of point (-20 .. 20)
    float lx = MAX_X - MIN_X;                               // length of range in x-axis
    float ly = MAX_Y - MIN_Y;                               // length of range in y-axis
    float normalized_x = (point.x - MIN_X) / lx;            // shift x by MIN_X and normalize
    float normalized_y = (point.y - MIN_Y) / ly;            // shift y by MIN_Y and normalize
    return CLITERAL(Vector2) {
        .x = normalized_x * ScreenWidth, 
        .y = ScreenHeight - normalized_y * ScreenHeight      // flip axis for more mathematical convenience
    };
}


static Color colors[] = {
    YELLOW,
    PINK,
    GREEN,
    LIGHTGRAY,
    MAROON,
    ORANGE,
    RED,
    GRAY,
    LIME,
    GOLD,
    DARKGREEN,
    SKYBLUE,
    DARKGRAY,
    BLUE,
    DARKBLUE,
    PURPLE,
    VIOLET,
    DARKPURPLE,
    BEIGE,
    BROWN,
    DARKBROWN
};
static Points clusters[K] = {0};  
static Vector2 means[K] = {0};
static Points set = {0};

void generateRandomBlobs(size_t blobRadius, size_t blobSize)
{
    /* Generate random blobs to train on */
    clearPoints(&set); // set counter back to 0

    for (size_t i = 0; i < K; ++i)
        generateRandomClusters(GetRandomVector(-10, 10), blobSize, blobRadius, &set);
}

void generateNewState(void)
{   
    /* Generate a new state i.e new sets of samples and means */
    clearPoints(&set); // set counter back to 0

    // // Generate the mouse shape dataset 
    generateRandomClusters((Vector2){0, 0}, 10, 1000, &set);
    generateRandomClusters((Vector2){MIN_X * 0.5f, MAX_Y * 0.5f}, 5, 100, &set);
    generateRandomClusters((Vector2){MAX_X * 0.5f, MAX_Y * 0.5f}, 5, 100, &set);
    generateRandomClusters((Vector2){MAX_X * 0.5f, -MAX_Y * 0.5f}, 5, 100, &set);
    generateRandomClusters((Vector2){MIN_X * 0.5f, -MAX_Y * 0.5f}, 5, 100, &set);
    // generateRandomClusters((Vector2){MIN_ * 0.25f, 0}, 5, 100, &set);


    // Generate random means or Centroids
    for (int i = 0; i < (int)K; ++i)
    {
        means[i].x = lerpf(MIN_X, MAX_X, randf());
        means[i].y = lerpf(MIN_Y, MAX_Y, randf());
    }
}

void reclusterSet(void)
{
    // Group the points into clusters (K-Means Algorithm)
    for (int i = 0; i < (int)K; ++i)
    {
        clearPoints(&clusters[i]);           // clear the clusters before
    }
    for (size_t i = 0; i < set.count; ++i)
    {
        Vector2 p = set.items[i];
        int _k = -1; 
        float s = FLT_MAX;
        for (size_t j = 0; j < K; ++j)
        {
            Vector2 m = means[j];
            float sm = Vector2LengthSqr(Vector2Subtract(p, m));
            if (sm < s)
            {
                s = sm;
                _k = j;
            }
        }
        nob_da_append(&clusters[_k], p);
    }
}

void updateMeans()
{   
    // iterate over each cluster
    for (size_t i = 0; i < K; ++i)
    {   
        Vector2 clusterSum = {0};
        Points cluster = clusters[i];
        if (cluster.count > 0) {
            for (size_t j = 0; j < clusters[i].count; ++j)
                clusterSum = Vector2Add(clusterSum, cluster.items[j]);       
            
            means[i] = clusterSum;
            // divide means[i] by its count
            means[i].x /= cluster.count;
            means[i].y /= cluster.count;
        } else {
            // regenerate the cluster if there are none to begin with 
            for (int i = 0; i < (int)K; ++i)
            {
                means[i].x = lerpf(MIN_X, MAX_X, randf());
                means[i].y = lerpf(MIN_Y, MAX_Y, randf());
            }
        }
    }
}


typedef enum {
    FLOWER_SEPAL_LENGTH,
    FLOWER_SEPAL_WIDTH,
    FLOWER_PETAL_LENGTH,
    FLOWER_PETAL_WIDTH,
} FlowerAttr;

int parseCSV(void)
{
    const char *fp = "./iris.csv";
    Nob_String_Builder sb = {0};
    if (!nob_read_entire_file(fp, &sb)) return 1;
    
    Nob_String_View content = nob_sv_from_parts(sb.items, sb.count);
    while (content.count > 0)
    {
        Nob_String_View line = nob_sv_chop_by_delim(&content, '\n');
        // nob_log(NOB_INFO, "|"SV_Fmt"|", SV_Arg(line));
        for (size_t i = 0; i < line.count; ++i)
        {
            Nob_String_View attr = nob_sv_chop_by_delim(&line, ',');
            nob_log(NOB_INFO, "%zu: "SV_Fmt, i, SV_Arg(attr));
        }
        nob_log(NOB_INFO, "------");    
    }
    return 0;
}

 
int main(void)
{   
    // Parsing CSV file 
    // parseCSV();
    // return 0;

    srand(time(NULL));      // Set seed to random generator

    // Window Configs 
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(ScreenWidth, ScreenHeight, "K MEANS CLUSTERING");
    
    generateNewState();
    // generateRandomBlobs(5, 100);
    reclusterSet();
    
    while (!WindowShouldClose())
    {   
        if (IsKeyPressed(KEY_K))
        {
            generateNewState();
            // generateRandomBlobs(10, 10);
            reclusterSet();
        }
        if (IsKeyPressed(KEY_SPACE))
        {   
            // for (size_t i = 0; i < 15; ++i)
            // {
                updateMeans();
                reclusterSet();
            // }
        }

        BeginDrawing();
            ClearBackground(bgColor);

            // Drawing the means & clusters 
            for (size_t i = 0; i < K; ++i)
            {   
                Color c = colors[i % NOB_ARRAY_LEN(colors)];
                Points cluster = clusters[i];
                for (size_t j = 0; j < cluster.count; ++j)
                    DrawCircleV(projectPointToScreen(cluster.items[j]), POINT_RADIUS, ColorAlpha(c, 0.55));
                
                // Draw means 
                DrawCircleV(projectPointToScreen(means[i]), MEAN_POINT_RADIUS, c);
                DrawCircleLinesV(projectPointToScreen(means[i]), MEAN_POINT_RADIUS + 0.2, BLACK); 
            }            
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
