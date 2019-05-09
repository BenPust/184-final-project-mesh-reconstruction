# 184-final-project-mesh-reconstruction

## In depth description of Ball Pivoting algorithm
_Following from: https://vgc.poly.edu/~csilva/papers/tvcg99.pdf And https://lidarwidgets.com/samples/bpa_tvcg.pdf_

_useful video to see how ball pivoting works_
- https://tanmaybinaykiya.github.io/tetrahedralization-of-3d-meshes

## Background on mesh reconstruction algorithms. 
There are two type of existing interpolation techniques: sculpting based and region growing. Sculpting based technique use a volume tetrahedralization is computed from the data points, typically the 3D Delaunay triangulation. Region growing technique uses a seed triangle which takes a random unused point and combines it with either a pre-existing mesh or starts a new one from said point.

>Abstract—The Ball-Pivoting Algorithm (BPA) computes a triangle mesh interpolating a given point cloud. Typically the points are surface samples acquired with multiple range scans of an object. The principle of the BPA is very simple: Three points form a triangle if a ball of a user-specified radius rho touches them without containing any other point. Starting with a seed triangle, the ball pivots around an edge (i.e. it revolves around the edge while keeping in contact with the edge’s endpoints) until it touches another point, forming another triangle. The process continues until all reachable edges have been tried, and then starts from another seed triangle, until all points have been considered. We applied the BPA to datasets of millions of points representing actual scans of complex 3D objects. The relatively small amount of memory required by the BPA, its time efficiency, and the quality of the results obtained compare favorably with existing techniques.

## Surface Reconstruction and Ball-pivoting algorithm
- Manifold `M` - surface of a three-dimensional object
- `S` - point sampling of `M`


## Mesh reconstruction: according to the write up

- Front:
    - Stores the mesh
    - Keeps track of all points and polygons at all times.
    - __Front__ refers to set of active edges (contained in loop) that we are currently considering in order to expand the mesh. There is a single front per mesh

- Loop
    - Linked list
    - stores a pointer to the the first edge in the list
    - __Front__ contains a list of loops that are currently being expanded. 
    - Loops my merge into other loops or break apart as a result of join/glue operations.

- Edge
    - Ball pivot algorithm pivots over an edge. 
    - Half edges unnecessary complex
    - Bare polygons lacked the relationship between points i and j that we needed. 
    - We only keep track of edges on the constantly expanding loop front, so __every edge must have a corresponding triangle__.
    - Keep track of the third point and call it o; we will need it for out calculations later on.

## Made up of 2 parts
1. Ball Pivot step in which we discover new points and add to the mesh we are currently building on, 
2. Find Seed Triangle selection, which occurs when ball pivoting gets stuck and requires us to select a new set of edges to expand on. This continues until we have considered all points that the ball can reach

### Step 1: Ball pivot step

- Select an active edge
    - _active edge_ = any edge that has not been fully explored yet, can still be expanded
- Find all the points in a 2*rho radius from the midpoint m between i and j
    - For each __candidate point__ `x` we calculate the center of the sphere of radius rho that touches i, j, and x
    -  `c_ijx` r is the distance from m to the center of the current center of the ball
        - For each calculated center `c_ijx` we check if it is on the circle gamma, defined by the radius `r` and center `m`, `perpendicular to the pivot edge ij`. We then choose the c_ijx that we encounter first while pivoting on the edge.

        - __Then we have found k = x__, we need to incorporate the triangle __ijk__ into our mesh and update our loop and other pointers. 

        #### Join operations
        - We expand the loop to incorporate edges ik and kj, and we remove edge ij.

        #### Glue Operations [HARD]
        _The glue operations ensure that at the end of the while loop, all our loops merge and we are left with a single mesh._

        - As our loop expands to fit whatever complex geometry the point cloud describes
        - having to break apart loops creating more loops, or merging loops into a single loop.

### Step 2: Find seed triangle
- Find all pairs of points in a radius _2*rho_ of random unused point.

- Verify that the sphere of radius rho can lie on these three lines.

- Check that 
    - the sphere does not contain any other data points
    - that it lies on the "outside" of the point cloud


__Do we have this data?__





--- 

## Implementation list:
- Data structures for Ball Pivoting Algo
- Voxels for point lookup in O(1) time
    - _if there's time left?_
