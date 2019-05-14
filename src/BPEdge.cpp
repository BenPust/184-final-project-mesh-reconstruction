//
//  BPEdge.cpp
//  ColladaViewer
//
//  Created by Benjamin Pust on 5/10/19.
//

//#include "BPEdge.hpp"
#include "BPFront.hpp"

BPEdge::BPEdge() {
  throw "Should never run this!";
}

BPEdge::BPEdge(int i, int j, int o, BPEdge *prev, BPEdge *next, BPLoop *loop) {
  this->i = i;
  this->j = j;
  this->o = o;
  this->prev =  prev;
  this->next = next;
  this->loop = loop;
  
  isActive = true;
}

bool BPEdge::pivotOperation(double rho, int *k, BPFront *commonFront) {

  std::vector<CGL::Vector3D> vertices = commonFront->vertices;
  
  CGL::Vector3D m = (vertices[i] + vertices[j])/2;
  CGL::Vector3D i = vertices[this->i];
  CGL::Vector3D j = vertices[this->j];
  CGL::Vector3D o = vertices[this->o];

  CGL::Vector3D c_ijo = getSphereCenter(i,j,o,rho);
  double r = (m - c_ijo).norm();

  std::vector<int> candidates = findCandidatePoints(rho, m, this->i, this->j, commonFront->vertices, commonFront);
  std::vector<int> center_indices;
  std::vector<CGL::Vector3D> centers;
  
  for (int ix = 0; ix < candidates.size(); ix++) {
    int iix = candidates[ix];
    CGL::Vector3D x = vertices[iix];
    CGL::Vector3D c = getSphereCenter(i, j, x, rho);
    if (c.x == 9999 && c.y == 9999 && c.z == 9999)
      continue;
    
    if ((c-m).norm() == r && (commonFront->verticesOnFront[iix] || !commonFront->verticesUsed[iix])) {
      center_indices.push_back(iix);
      centers.push_back(c);
    }
  }
  
  if (centers.size() == 0) return false;
  
  CGL::Vector3D b = vertices[this->o] - m;
  CGL::Vector3D a;
  double max_proj = std::numeric_limits<double>::lowest();
  int first_index;
  for (std::size_t i = 0; i != centers.size(); ++i) {
    a = centers[i] - m;
    if (dot(a,b) > max_proj ){
      max_proj = dot(a,b);
      first_index = center_indices[i];
    }
  }
  if (max_proj == std::numeric_limits<double>::lowest()){
    *k = 99999999;
    return false;
  }
  *k = first_index;
  return true;
  
}

void BPEdge::markNotActive() {
  isActive = false;
}

// TODO:
CGL::Vector3D BPEdge::getSphereCenter(CGL::Vector3D i,CGL::Vector3D j, CGL::Vector3D x, double rho) {
  CGL::Vector3D ji = j-i;
  CGL::Vector3D xi = x-i;
  CGL::Vector3D n = cross(ji, xi);
  
  CGL::Vector3D p0 = cross(dot(ji, ji) * xi - dot(xi, xi) * ji, n) / (2 * dot(n, n)) + i;
  if (dot(n, n) == 0) {
    return CGL::Vector3D(9999, 9999, 9999);
  }
  double t1 = sqrt((rho*rho - dot(p0-i, p0-i))/ dot(n, n));

  CGL::Vector3D c1 = p0 + (n * t1);

  return c1;
}

// TODO:
std::vector<int> BPEdge::findCandidatePoints(double rho, CGL::Vector3D m, int e_i, int e_j, std::vector<CGL::Vector3D> vertices, BPFront *commonFront) {
  std::vector<int> candidates;
  for (std::size_t i = 0; i != vertices.size(); ++i) {
    if ((vertices[i] - m).norm()  < 2 * rho && i != e_i && i != e_j && !commonFront->verticesUsed[i])
      candidates.push_back(i);
  }
  return candidates;
}
