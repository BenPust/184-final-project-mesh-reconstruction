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
  pm->vertices = *vertices;
  pm->normals = *normals;
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
  CGL::Vector3D b, c, iV, jV;
  for (int index = 0; index < vertices.size(); index++) {
    if (!verticesUsed[index]) {
      b = vertices[index]; // TODO: +
      
      std::vector<int> nV = findNearbyPoints(2*rho, index, vertices, commonFront);
      
      for (int i = 0; i < nV.size(); i++){
        for (int j = 0; j < nV.size(); j++) {
          iV = nV[i];
          jV = nV[j];
          c = (b+iV+jV)/3.0;
          if (i == j || commonFront->verticesUsed[i] || commonFront->verticesUsed[j]) {
            continue;
          }
          
          for (CGL::Vector3D v : vertices) {
            if ((v - c).norm() < rho) {
              continue;
            }
            indices->push_back(index);
            indices->push_back(i);
            indices->push_back(j);
            
            commonFront->verticesOnFront[i] = true;
            commonFront->verticesOnFront[j] = true;
            commonFront->verticesOnFront[index] = true;
            commonFront->verticesUsed[i] = true;
            commonFront->verticesUsed[j] = true;
            commonFront->verticesUsed[index] = true;
            return true;
          }
        }
      }
    }
  }
  return false;
}

std::vector<int> BPFront::findNearbyPoints(double rho, int cand_idx, std::vector<CGL::Vector3D> vertices, BPFront *commonFront) {
  std::vector<int> c = std::vector<int>();
  for (std::size_t i = 0; i != vertices.size(); ++i) {
    // //cout << (vertices[i] - vertices[cand_idx]).norm() << endl;
    if (((vertices[i] - vertices[cand_idx]).norm()  < 2 * rho) && (cand_idx != i) && !commonFront->verticesUsed[i]){
      c.push_back(i);
    }
  }
  return c;
}
