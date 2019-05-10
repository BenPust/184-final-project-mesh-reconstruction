//
//  BPLoop.cpp
//  ColladaViewer
//
//  Created by Benjamin Pust on 5/10/19.
//

#include "BPFront.hpp"

BPLoop::BPLoop(BPEdge* startEdge, BPFront *front) {
  this->startEdge = startEdge;
  this->front = front;
}
