#include "CGL/CGL.h"

#include "collada.h"
#include "meshEdit.h"
#include "bezierPatch.h"
#include "bezierCurve.h"
#include "mergeVertices.h"
#include "shaderUtils.h"
#include "pointCloud.h"
#include "Voxel.hpp"

#include <iostream>

using namespace std;
using namespace CGL;

#define msg(s) cerr << "[Collada Viewer] " << s << endl;

int parsePly(const char *path, vector<Vector3D> *vertices) {
  FILE* file = fopen(path, "r");
  int vertex_count = 0;
  int face_count = 0;
  
  char lineHeader[512];
  while (true) {
    int res = fscanf(file, "%s", lineHeader);
    
    if (res == EOF || strcmp(lineHeader, "end_header") == 0) {
      break;
    }
    
    if (strcmp(lineHeader, "element") == 0) {
      char type[128];
      int count;
      
      fscanf(file, "%s %d\n", type, &count);
      if (strcmp(type, "vertex") == 0)
        vertex_count = count;
      
      if (strcmp(type, "face") == 0)
        face_count = count;
    }
  }
  
  cout << "ply parsing\nVertex count: " << vertex_count << "\nFace count: " << face_count << endl;
  
  while (--vertex_count >= 0) {
    Vector3D vertex;
    fscanf(file, "%lf %lf %lf\n", &vertex.x, &vertex.y, &vertex.z);
    vertices->push_back(vertex);
  }
  
  return 1;
}

int loadFile(MeshEdit* collada_viewer, const char* path) {

  Scene* scene = new Scene();

  std::string path_str = path;
  if (path_str.substr(path_str.length()-4, 4) == ".dae")
  {
    if (ColladaParser::load(path, scene) < 0) {
      delete scene;
      return -1;
    }
  }
  else if (path_str.substr(path_str.length()-4, 4) == ".bez")
  {
    Camera* cam = new Camera();
    cam->type = CAMERA;
    Node node;
    node.instance = cam;
    scene->nodes.push_back(node);
    Polymesh* mesh = new Polymesh();

    FILE* file = fopen(path, "r");
    int n = 0;
    fscanf(file, "%d", &n);
    for (int i = 0; i < n; i++)
    {
      BezierPatch patch;
      patch.loadControlPoints(file);
      patch.add2mesh(mesh);
      mergeVertices(mesh);
    }
    fclose(file);

    mesh->type = POLYMESH;
    node.instance = mesh;
    scene->nodes.push_back(node);
  }

  else if (path_str.substr(path_str.length()-4, 4) == ".ply") {
    vector<Vector3D> vertices = vector<Vector3D>();
    
    parsePly(path, &vertices);
    
    Camera* cam = new Camera();
    cam->type = CAMERA;
    Node node;
    node.instance = cam;
    scene->nodes.push_back(node);
    PointCloud* point_cloud = new PointCloud();
    Polymesh *polymesh = new Polymesh();
    
    // Add vertices to scene
    for (Vector3D v : vertices) {
      point_cloud->vertices.push_back(v);
      polymesh->vertices.push_back(v);
    }
    
    point_cloud->type = POINT_CLOUD;
    node.instance = point_cloud;
    scene->nodes.push_back(node);
  }
  else
  {
    return -1;
  }

  collada_viewer->load( scene );

  GLuint tex = makeTex("envmap/envmap.png");
  if(!tex) tex = makeTex("../envmap/envmap.png");
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, tex);
  glActiveTexture(GL_TEXTURE2);

  return 0;
}

int main( int argc, char** argv ) {

  const char* path = argv[1];
  std::string path_str = path;
  
  //////////////////////////////
  // Bezier curve viewer code //
  //////////////////////////////
  
  if (path_str.substr(path_str.length()-4, 4) == ".bzc")
  {
    // Each file contains a single Bezier curve's control points
    FILE* file = fopen(path, "r");
    
    int numControlPoints;
    fscanf(file, "%d", &numControlPoints);
    
    BezierCurve curve(numControlPoints);
    curve.loadControlPoints(file);
    fclose(file);
    
    // Create viewer
    Viewer viewer = Viewer();
    viewer.set_renderer(&curve);
    viewer.init();
    viewer.start();
    
    exit(EXIT_SUCCESS);
    
    return 0;
  }
  
  
  // create viewer
  Viewer viewer = Viewer();

  // create collada_viewer
  MeshEdit* collada_viewer = new MeshEdit();

  // set collada_viewer as renderer
  viewer.set_renderer(collada_viewer);

  // init viewer
  viewer.init();

  // load tests
  if( argc == 2 ) {
    if (loadFile(collada_viewer, argv[1]) < 0) exit(0);
  } else {
    msg("Usage: ./meshedit <path to scene file>"); exit(0);
  }

  Voxel a = Voxel(1, 1, 1, 1, 1, 1);
  
  // find a bounding box of the points
  float maxX, minX, maxY, minY, maxZ, minZ;
  bool init_bb = false;
  for (Vector3D a: collada_viewer->pointCloudNodes[0].point_cloud.vertices) {
//    cout << a.x << " " << a.y << " " << a.z << endl;
    if (!init_bb) {
      maxX = a.x;
      minX = a.x;
      maxY = a.y;
      minY = a.y;
      maxZ = a.z;
      minZ = a.z;
      
      init_bb = true;
    }
    
    if (maxX < a.x)
      maxX = a.x;
    
    if (minX > a.x)
      minX = a.x;
    
    if (maxY < a.y)
      maxY = a.y;
    
    if (minY > a.y)
      minY = a.y;
    
    if (maxZ < a.z)
      maxZ = a.z;
    
    if (minZ > a.z)
      minZ = a.z;
  }
  
  cout << "Bounding box coordinates: " << minX << " -- " << maxX;
  cout << " -- " << minY << " -- " << maxY;
  cout << " -- " << minZ << " -- " << maxZ << endl;
  
  // start viewer
  viewer.start();
  
  return 0;
}
