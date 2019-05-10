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

class BPLoop;
class BPEdge;

class BPFront {
  BPFront(std::vector<CGL::Vector3D> *vertices, CGL::Polymesh* pm);
  
  CGL::Polymesh *polymesh;
  std::vector<BPLoop *> loops;
  std::vector<CGL::Vector3D> vertices;
  std::vector<bool> verticesOnFront;
  std::vector<bool> verticesUsed;
  
  void join(BPEdge* edge_ij, int k);
  void outputTriangleToMesh(int i, int j, int k);
  BPEdge* getActiveEdge();
  BPLoop* insertEdge(BPEdge *edge);
  bool findSeedTriangle(std::vector<int> *indices, double rho, BPFront *commonFront);
  bool findSeedTrangleIndices(std::vector<int>* indices, double rho, BPFront *commonFront);
  std::vector<int> findNearbyPoints(double rho, int cand_idx, std::vector<CGL::Vector3D> vertices, BPFront *commonFront);
};

class BPEdge {
public:
  BPEdge(int i, int j, int o, BPEdge *prev, BPEdge *next, BPLoop *loop);
  
  int i, j, o;
  BPEdge *prev;
  BPEdge *next;
  BPLoop *loop;
  
  bool isActive;
  
  bool pivotOperation(BPFront *front, double rho, int k);
};

class BPLoop {
public:
  BPLoop(BPEdge* startEdge, BPFront *front);
  BPEdge* startEdge;
  BPFront *front;
};

#endif /* BPFront_hpp */
