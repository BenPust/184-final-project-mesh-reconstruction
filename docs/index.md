---
title: "3D Mesh Reconstruction"
---

- TODO:
    - What are the challenges:
    - 

### Sejal Popat, Merlin Shi, Benjamin Pust

We are going to implement part of the pipeline described in 3DCapture: 3D Reconstruction for a Smartphone, which takes a TSDF point cloud and uses this to reconstruct a textured mesh. 

**Why is it important:** With the ability to construct polygonal mesh from point cloud, we can manipulate the geometry and overlay various textures. This allows us to create artistic images based on the images we take.

**What are the challenges:**

## Goals and Deliverables
**Baseline Deliverable:** Given TSDF point cloud, reconstruct 3D polygonal mesh, and then do texture mapping to build a textured mesh.
- Mesh Reconstruction:
- Texture Mapping: BEN

**Reach Deliverable:** If we have time our reach goal would be to implement one or two of the previous/later steps in the pipeline:

- Depth map estimation or Depth fusion (which produces the TSDF point cloud from keyframes with poses and sparse scene structure).  
- Because people are sensitive to distortion in face geometry, 

**Evaluation:** We will measure evaluate the performance with the Ignatius dataset described here.

## Schedule

**April 30 (Graded Milestone due):**
- Mesh Reconstruction from point cloud
- At least part of texture mapping

**May 9 (Final Presentation):**

**May 14 (Final Deliverables due):**
- Reach Goals (if possible)


## Resources

Papers:
- [3DCapture: 3D Reconstruction for a Smartphone](http://openaccess.thecvf.com/content_cvpr_2016_workshops/w14/papers/Muratov_3DCapture_3D_Reconstruction_CVPR_2016_paper.pdf)
- [Unconstrained Isosurface Extraction on Arbitrary Octrees](http://www.cs.jhu.edu/~misha/MyPapers/SGP07a.pdf)
- [Tanks and Temples: Benchmarking Large-Scale Scene Reconstruction](https://storage.googleapis.com/t2-downloads/paper/tanks-and-temples.pdf)


Software: 
- [COLMAP](https://colmap.github.io/)
