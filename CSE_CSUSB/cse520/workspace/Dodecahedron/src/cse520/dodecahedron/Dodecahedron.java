package cse520.dodecahedron;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import javax.microedition.khronos.opengles.GL10;

public class Dodecahedron {
  private FloatBuffer vertexBuffer; 		// buffer holding vertices
  private ByteBuffer[] faceIndexBuffer = new ByteBuffer[12]; // buffer holding faces
  //private FloatBuffer[] colorBuffer = new FloatBuffer[12]; // buffer holding colors

  private final int nfaces = 12;    //number of faces in object
  //Vertices
  float vertices[] = new float[] {
		  0.0f,0.847214f,0.323607f,
		  0.0f,0.847214f,-0.323607f,
		  -0.523607f,0.523607f,0.523607f,
		  -0.523607f,0.523607f,-0.523607f,
		  -0.847214f,0.323607f,-0.0f,
		  0.523607f,0.523607f,-0.523607f,
		  0.523607f,0.523607f,0.523607f,
		  0.847214f,0.323607f,-0.0f,
		  0.323607f,-0.0f,-0.847214f,
		  -0.323607f,-0.0f,-0.847214f,
		  -0.323607f,0.0f,0.847214f,
		  0.323607f,0.0f,0.847214f,
		  -0.523607f,-0.523607f,-0.523607f,
		  -0.847214f,-0.323607f,0.0f,
		  -0.523607f,-0.523607f,0.523607f,
		  0.523607f,-0.523607f,0.523607f,
		  0.847214f,-0.323607f,0.0f,
		  0.523607f,-0.523607f,-0.523607f,
		  0.0f,-0.847214f,-0.323607f,
		  0.0f,-0.847214f,0.323607f,
  };
  
  //Faces
  byte faceIndices[][] = {
		  {0,1,2, 1,3,2, 3,4,2},
		  {1,0,5, 0,6,5, 6,7,5},
		  {1,5,3, 5,8,3, 8,9,3},
		  {0,2,6, 2,10,6, 10,11,6},
		  {3,9,4, 9,12,4, 12,13,4},
		  {2,4,10, 4,13,10, 13,14,10},
		  {6,11,7, 11,15,7, 15,16,7},
		  {5,7,8, 7,16,8, 16,17,8},
		  {9,8,12, 8,17,12, 17,18,12},
		  {11,10,15, 10,14,15, 14,19,15},
		  {13,12,14, 12,18,14, 18,19,14},
		  {16,15,17, 15,19,17, 19,18,17}
  };
    
  public Dodecahedron() {
	// a float has 4 bytes so we allocate for each coordinate 4 bytes
	ByteBuffer byteBuffer = ByteBuffer.allocateDirect(this.vertices.length * 4);
	byteBuffer.order(ByteOrder.nativeOrder());
	vertexBuffer = byteBuffer.asFloatBuffer();
	vertexBuffer.put(vertices);
	vertexBuffer.position(0);
	for(int i = 0; i < nfaces; i++){
	  faceIndexBuffer[i] = ByteBuffer.allocateDirect(this.faceIndices[i].length);
	  faceIndexBuffer[i].put( faceIndices[i] );
	  faceIndexBuffer[i].position(0);
	}
  }

  private void setColor ( GL10 gl,  int i )
  {
     float R = (float) (i % 5) / 5;  
     float G = (float) (i % 3) / 3;  
     float B = (float) (i % 4) / 4;  
     gl.glColor4f( R, G, B, 0 );
 
  }  
  public void draw(GL10 gl) {
	gl.glFrontFace(GL10.GL_CW);
	gl.glVertexPointer(3, GL10.GL_FLOAT, 0, vertexBuffer);
	for(int i = 0; i < nfaces; i++){
 	  setColor ( gl, i );
	  gl.glDrawElements(GL10.GL_TRIANGLES, faceIndexBuffer[i].limit(), 
			       GL10.GL_UNSIGNED_BYTE, faceIndexBuffer[i]);
    }
  }
}