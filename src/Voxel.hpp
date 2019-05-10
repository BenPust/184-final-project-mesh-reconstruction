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


class Voxel {
  // used for storing points for O(1) lookup
  
public:
  Voxel(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
  
  bool addPoint(CGL::Vector3D);
  std::vector<CGL::Vector3D*>* getNeighbourVoxels(CGL::Vector3D);
  
  void printOutValues();
  
private:
  float minX,  maxX,  minY,  maxY,  minZ,  maxZ;
  float rho = 0;
  
  std::map<std::pair<std::pair<int, int>, int> , std::vector<CGL::Vector3D * > > point_map2;
  
};

#endif /* Voxel_hpp */
