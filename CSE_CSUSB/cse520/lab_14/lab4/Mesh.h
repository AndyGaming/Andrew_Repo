#ifndef MESH_H
#define MESH_H

#include <vector>
#include <fstream>
#include <GL/glut.h>
#include "util3D.h"

using namespace std;

class Polygon {
public:
  int n;			//n sides
  vector <int> vertices;	//vertex indices of vertexList;
  vector <int> normals;		//indices of normals at vertices
};

class Mesh {
public:
  int nVertices;		//number of vertices
  int nNormals;			//number of normals
  int nFaces;		        //number of polygons 
  vector<Point3> vertexList;
  vector<Vector3> normalList;
  vector <Polygon> faceList;	//each face is a polygon
  Mesh();
  bool readData( char fileName[] );
  void renderMesh();		//render the mesh
  void setColor ( int n );
};

#endif
