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
  
  this->rho = xDistance/10;
}


/// Computes a hashmap key for lookup give a 3d point
//int Voxel::hashKey(CGL::Vector3D *point) {
//  int x = floor((point->x+abs(minX))/rho);
//  int y = floor((point->y+abs(minY))/rho);
//  int z = floor((point->z+abs(minZ))/rho);
//
////  int aaa = (x*107 + y)*13 + z;
////  if (check_dup.find(aaa) != check_dup.end()) {
////    throw "Should never be here!";
////  } else {
////    check_dup[aaa] = true;
////  }
//
//  // 107 and 13 are just prime numbers
////  return (x*11587 + y)*2749 + z*107;
//
//  return x + y*100 + z*10000;
//}


bool Voxel::addPoint(CGL::Vector3D vec) {
//  point_map[hashKey(&vec)].push_back(&vec);
  int x = floor((vec.x+abs(minX))/rho);
  int y = floor((vec.y+abs(minY))/rho);
  int z = floor((vec.z+abs(minZ))/rho);
  
  point_map2[std::make_pair(std::make_pair(x, y), z)].push_back(&vec);
//  point_map2[VoxelPoint((
//                        ((vec.y+abs(minY))/rho),
//                        ((vec.z+abs(minZ))/rho))].push_back(&vec);
  return true;
}

std::vector<CGL::Vector3D*>* Voxel::getNeighbourVoxels(CGL::Vector3D vec) {
  
  
  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {
      for (int k = -1; k < 2; k++) {
        int x = floor((vec.x+abs(minX))/rho);
        int y = floor((vec.y+abs(minY))/rho);
        int z = floor((vec.z+abs(minZ))/rho);
      }
    }
  }
  
  return &point_map2[std::make_pair(std::make_pair(x, y), z)];
  
}

void Voxel::printOutValues() {
  int total = 0;
  
  for (auto& any : point_map2) {
    total += any.second.size();
    
  }
  
  std::cout << "Final size: " << total << std::endl;
}


