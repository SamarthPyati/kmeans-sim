# K-Means Algorithm Visualization using Raylib

**Author**: Samarth Sanjay Pyati  
**Date**: 2-07-2024

## Description

This project visualizes the K-Means clustering algorithm using Raylib. The program demonstrates the step-by-step process of the algorithm, showing how data points are grouped into clusters and how the centroids of these clusters are updated iteratively.

## Compilation and Execution

### Compile the No Builder

```sh
cc nob.c -o nob
```

### Compile and Run K-Means Visualization

```sh
./nob && ./kmeans
```

## Key Features

- **Random Cluster Generation**: Generates random clusters of points within a specified radius.
- **Dynamic Clustering**: Groups the points into clusters using the K-Means algorithm.
- **Centroid Update**: Updates the centroids of the clusters iteratively.

## Controls

- **Press `K`**: Generate a new state with random points and initial centroids.
- **Press `SPACE`**: Update the centroids and recluster the points.

