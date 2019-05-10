//
//  BPFront.hpp
//  ColladaViewer
//
//  Created by Benjamin Pust on 5/10/19.
//

#ifndef BPFront_hpp
#define BPFront_hpp

#include <stdio.h>
#include <vector>
#include <CGL/CGL.h>
#include "mesh.h"
#include "BPLoop.hpp"
#include "BPEdge.cpp"

class BPFront {
  BPFront(std::vector<CGL::Vector3D> *vertices, CGL::Polymesh* pm);
  
  std::vector<BPLoop *> loops;
  std::vector<CGL::Vector3D *> vertices;
  
  void join(BPEdge* edge_ij, int k);
  
  void outputTriangleToMesh(int i, int j, int k);
  
  CGL::Polymesh *polymesh;
};

#endif /* BPFront_hpp */
