//
//  Voxel.cpp
//  ColladaViewer
//
//  Created by Benjamin Pust on 5/9/19.
//

#include "Voxel.hpp"
#include <iostream>

Voxel::Voxel(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {
  this->minX = minX;
  this->maxX = maxX;
  this->minY = minY;
  this->maxY = maxY;
  this->minZ = minZ;
  this->maxZ = maxZ;
  
  float xDistance = maxX - minX;
  float yDistance = maxY - minY;
  float zDistance = maxZ - minZ;
  
  deltaX = xDistance / splits;
  deltaY = yDistance / splits;
  deltaZ = zDistance / splits;
  
  this->rho = xDistance/10; // TODO: useless at this point
  
}

/// Adds a point into the voxel map
bool Voxel::addPoint(CGL::Vector3D vec) {
//  point_map[hashKey(&vec)].push_back(&vec);
  int x = floor((vec.x+abs(minX))/deltaX);
  int y = floor((vec.y+abs(minY))/deltaY);
  int z = floor((vec.z+abs(minZ))/deltaZ);
  
  point_map2[std::make_pair(std::make_pair(x, y), z)].push_back(&vec);
  return true;
}

/// Returns a vector pointer to a colleciton of Vector3D pointers that are closeby a passed in Vector3D
std::vector<CGL::Vector3D*>* Voxel::getNeighbourVoxels(CGL::Vector3D vec) {
  
  std::vector<CGL::Vector3D *> * points = new std::vector<CGL::Vector3D *>();
  
  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {
      for (int k = -1; k < 2; k++) {
        int x = floor((vec.x+abs(minX))/deltaX);
        int y = floor((vec.y+abs(minY))/deltaY);
        int z = floor((vec.z+abs(minZ))/deltaZ);
        
        std::pair<std::pair<int, int>, int> point =  std::make_pair(std::make_pair(x, y), z);
        
//        point_map2[];
        if (point_map2.find(point)  != point_map2.end()) {
          std::vector<CGL::Vector3D *> tmp = point_map2[point];
          copy(tmp.begin(), tmp.end(), back_inserter(*points)); // this should extend the list, NOT TESTED
        }
      }
    }
  }
  
  return points;
}

/// Debug purpose
void Voxel::printOutValues() {
  int total = 0;
  
  for (auto& any : point_map2) {
    total += any.second.size();
  }
  
  std::cout << "Final size: " << total << std::endl;
}


