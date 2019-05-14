//
//  BPFront.cpp
//  ColladaViewer
//
//  Created by Benjamin Pust on 5/10/19.
//

#include "BPFront.hpp"

BPFront::BPFront(std::vector<CGL::Vector3D> *vertices, std::vector<CGL::Vector3D> *normals, CGL::Polymesh* pm) {
  this->vertices = *vertices;
  this->normals = *normals;
  
  this->polymesh = pm;
  this->polymesh->vertices = *vertices;
  this->polymesh->normals = *normals;
  
  verticesUsed = std::vector<bool>(vertices->size(), false);
  verticesOnFront = std::vector<bool>(vertices->size(), false);
  
}

void BPFront::BP(double rho, BPFront *commonFront) {
  commonFront = this;
  std::vector<int> seed_triangle_indices;
  if (!findSeedTriangle(&seed_triangle_indices, rho, commonFront)) {
    //cout << "u ded" << endl;
    return;
  }
  
  std::cout << ">.< Why do you not love me?!\n";
  
  printf("Seed triangle indices: %d %d %d\n", seed_triangle_indices[0], seed_triangle_indices[1], seed_triangle_indices[2]);

  BPEdge * edge = getActiveEdge();
  while (true){
    while (edge != nullptr){
      int k;
      bool pivot_success = edge->pivotOperation(rho, &k, commonFront);

      if (pivot_success){
        outputTriangleToMesh(edge->i, edge->j, k);
        this->verticesOnFront[k] = true;
        this->verticesUsed[k] = true;
        //cout << "loop size: " << global_front->loops.size() << endl;
        join(edge, k);
        //cout << "loop size: " << global_front->loops.size() << endl;
        // add vertex to the front
        this->verticesOnFront[k] = true;
        // if (e_ki in F) glue(e_ik, e_ki , F);
        // if (e_jk in F) glue(e_kj, e_jk, F);
        //cout << " on front? " << global_front->vertices_on_front[k] << endl;
      }
      edge->markNotActive();
      //cout << "should be false" << edge->is_active << endl;
      edge = getActiveEdge();
    }
    if (!findSeedTriangle(&seed_triangle_indices ,rho, commonFront)){
      return;
    }
  }
}

void BPFront::join(BPEdge* e_ij, int k) {
  CGL::Vector3D v_k = vertices[k];
  BPEdge* e_ik;
  BPEdge* e_kj;
  // insert edges
  e_ik = new BPEdge(e_ij->i, k, e_ij->j, e_ij->prev, nullptr, e_ij->loop);
  e_kj = new BPEdge(k, e_ij->j, e_ij->i, e_ik, e_ij->next, e_ij->loop);
  e_ik->next = e_kj;
  
  e_ij->prev->next = e_ik;
  e_ij->next->prev = e_kj;
  
  this->verticesOnFront[k] = true;
  this->verticesUsed[k] = true;
  e_ij->loop->startEdge = e_ij;
}

void BPFront::outputTriangleToMesh(int i, int j, int k) {
  CGL::Polygon poly;
  std::vector<size_t> triangle_indices;
  triangle_indices.push_back(i);
  triangle_indices.push_back(j);
  triangle_indices.push_back(k);
  poly.vertex_indices = triangle_indices;
  polymesh->polygons.push_back(poly);
}

BPEdge* BPFront::getActiveEdge() {
  for (int i = 0; i < loops.size(); i++) {
    BPEdge* edge = loops[i]->startEdge;
    
    if (edge->isActive)
      return edge;
    
    edge = edge->next;
    
    while (edge->i != loops[i]->startEdge->i && edge->j != loops[i] -> startEdge -> j) {
      if (edge->isActive)
        return edge;
      
      edge = edge->next;
    }
  }
  return nullptr;
};

BPLoop *BPFront::insertEdge(BPEdge *edge) {
  BPLoop *loop = new BPLoop(edge, this);
  edge->loop = loop;
  this->loops.push_back(loop);
  return loop;
}

bool BPFront::findSeedTriangle(std::vector<int> *indices, double rho, BPFront *commonFront) {
  if (!findSeedTriangleIndices(indices, rho, commonFront))
    return false;
  
  std::cout << "Passing seed triangle indices\n";
  int i = (*indices)[0];
  int j = (*indices)[1];
  int k = (*indices)[2];
  
  std::vector<int> tIndex = std::vector<int>();
  tIndex.push_back(i);
  tIndex.push_back(j);
  tIndex.push_back(k);
  
  *indices = tIndex;
  outputTriangleToMesh(i, j, k);
  
  BPEdge *e0, *e1, *e2;
  BPLoop* loop;
  loop = new BPLoop(nullptr, this);
  e0 = new BPEdge(i,j,k, nullptr, nullptr, loop);
  e1 = new BPEdge(j,k,i, nullptr, nullptr, loop);
  e2 = new BPEdge(k,j,i, nullptr, nullptr, loop);
  loop->startEdge = e0;
  e0->prev = e2;
  e0->next = e1;
  e1->prev = e0;
  e1->next = e2;
  e2->prev = e1;
  e2->next = e0;
  this->loops.push_back(loop);
  
  return true;
}

bool BPFront::findSeedTriangleIndices(std::vector<int>* indices, double rho, BPFront *commonFront) {
  CGL::Vector3D b, iV, jV;
  for (int index = 0; index < vertices.size(); index++) {
    if (!verticesUsed[index]) {
      b = vertices[index]; // TODO: +

      std::vector<int> nV = findNearbyPoints(2 * rho, index, commonFront);
//      std::cout << nV.size() << std::endl;          // A practical to set rho is to observe nV.size(). Adjust rho until nV.size() is ~10 or ~100 (not too large).
      // possibly TODO: sort neighbor_vertices in order of distance from v.

      for (int j = 1; j < nV.size(); j++) {
        for (int i = 0; i < j; i++) {
          if (index == nV[i] || index == nV[j] || commonFront->verticesUsed[nV[i]] || commonFront->verticesUsed[nV[j]]) {
            continue;
          }
          iV = vertices[nV[i]];
          jV = vertices[nV[j]];
          CGL::Vector3D n = cross(iV - b, jV - b);         // Don't normalize here.
//          c = (b+iV+jV)/3.0;
          // Find ball center.
          CGL::Vector3D bi = iV - b;
          CGL::Vector3D bj = jV - b;
          CGL::Vector3D circle_center = cross(bi.norm2() * jV - bj.norm2() * iV, n) / 2 / n.norm2() +
                                        b;          // Center of the circle on the plane of 3 given points.
          if (rho * rho - dot(circle_center - iV, circle_center - iV) >= 0) {
            double t = sqrt((rho * rho - (circle_center - iV).norm2()) / n.norm2());
            CGL::Vector3D ball_center;
            if (dot(n, normals[index]) > 0) {
              ball_center = circle_center + n * t;
            } else {
              ball_center = circle_center - n * t;
            }

            // Check if the ball contains no other points.
            bool empty_ball = true;
            for (CGL::Vector3D v : vertices) {
              if ((v - b).norm() > 1e-6 && (v - iV).norm() > 1e-6 && (v - jV).norm() > 1e-6 && (v - circle_center).norm() < rho) {
                empty_ball = false;
                break;
              }
            }
            if (!empty_ball) {
              indices->push_back(index);
              indices->push_back(nV[i]);
              indices->push_back(nV[j]);

              commonFront->verticesOnFront[nV[i]] = true;
              commonFront->verticesOnFront[nV[j]] = true;
              commonFront->verticesOnFront[index] = true;
              commonFront->verticesUsed[nV[i]] = true;
              commonFront->verticesUsed[nV[j]] = true;
              commonFront->verticesUsed[index] = true;
              return true;
            }
          }
        }
      }
    }
  }
  return false;
}

/// Searches all the vertices for points that are withing 2rho of the candidate index vertex
/// Return all the indecies of vertex vector that are withing 2tho of that point
std::vector<int> BPFront::findNearbyPoints(double rho, int cand_idx, BPFront *commonFront) {
  std::vector<int> c = std::vector<int>();
  for (std::size_t i = 0; i != vertices.size(); ++i) {
    // //cout << (vertices[i] - vertices[cand_idx]).norm() << endl;
    if (((vertices[i] - vertices[cand_idx]).norm()  < 2 * rho) && (cand_idx != i) && !commonFront->verticesUsed[i]){
      c.push_back(i);
    }
  }
  return c;
}
