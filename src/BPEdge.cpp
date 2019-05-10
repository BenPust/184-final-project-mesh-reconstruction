//
//  BPEdge.cpp
//  ColladaViewer
//
//  Created by Benjamin Pust on 5/10/19.
//

#include "BPEdge.hpp"

BPEdge::BPEdge(int i, int j, int o, BPEdge *prev, BPEdge *next, BPLoop *loop) {
  this->i = i;
  this->j = j;
  this->o = o;
  this->prev =  prev;
  this->next = next;
  this->loop = loop;
  
  isActive = true;
}

bool BPEdge::pivotOperation(BPFront *front, double rho, int k) {
  
}
