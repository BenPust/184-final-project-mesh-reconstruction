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

class Voxel {
  // used for storing points for O(1) lookup
  
public:
  Voxel(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
  
  std::vector<CGL::Vector3D *>* neighbouringPointsFor(CGL::Vector3D);
  
  CGL::Vector3D & operator[](CGL::Vector3D);
  const CGL::Vector3D & operator[](CGL::Vector3D) const;
  
private:
  std::map<int, std::vector<CGL::Vector3D * > > point_map;
  
};

#endif /* Voxel_hpp */
