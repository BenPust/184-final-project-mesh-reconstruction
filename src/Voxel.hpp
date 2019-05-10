//
//  Voxel.hpp
//  ColladaViewer
//
//  Created by Benjamin Pust on 5/9/19.
//

#ifndef Voxel_hpp
#define Voxel_hpp

#include <stdio.h>
#include <map>
#include <vector>
#include "CGL/CGL.h"
#include<tuple>

struct VoxelPoint {

  int x,y,z;
  VoxelPoint(float x, float y, float z) {
    this->x = floor(x);
    this->y = floor(y);
    this->z = floor(z);
  }
};



class Voxel {
  // used for storing points for O(1) lookup
  
public:
  Voxel(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
  
//  std::vector<CGL::Vector3D *> arr[1][1][1];
  int hashKey(CGL::Vector3D*);
  
  bool addPoint(CGL::Vector3D);
  std::vector<CGL::Vector3D*>* getNeighbourVoxels(CGL::Vector3D);
  
  void printOutValues();
  
private:
  float minX,  maxX,  minY,  maxY,  minZ,  maxZ;
  float rho = 0;
  
//  std::map<int, std::vector<CGL::Vector3D * > > point_map;
  std::map<std::pair<std::pair<int, int>, int> , std::vector<CGL::Vector3D * > > point_map2;
  
  // only for debugging. delete later
//  std::map<int, bool> check_dup;
};

#endif /* Voxel_hpp */
