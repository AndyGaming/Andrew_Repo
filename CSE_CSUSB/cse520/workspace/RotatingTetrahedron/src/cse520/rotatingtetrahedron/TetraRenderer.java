package cse520.rotatingtetrahedron;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLU;
import android.opengl.GLSurfaceView;
import android.os.SystemClock;

public class TetraRenderer implements GLSurfaceView.Renderer {
	
	private FloatBuffer triangle;
	private FloatBuffer triangle2;
	private FloatBuffer triangle3;
	private FloatBuffer triangle4;

	public float angle = 0.0f;
	
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        // Set the background frame color to blue
        gl.glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
        gl.glEnable( GL10.GL_CULL_FACE ); //Enable culling faces
        gl.glCullFace ( GL10.GL_BACK ); //don’t render back faces
        // initialize the triangle vertex array
        initShapes();
        // Enable use of vertex arrays
        gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
    }
    
    public void onDrawFrame(GL10 gl) {
        // Redraw background color
        gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
        gl.glMatrixMode(GL10.GL_MODELVIEW);
        gl.glLoadIdentity(); 
        //gl.glRotatef(-50, 1, 0, 0);
        
        GLU.gluLookAt(gl, -4, -4, 5, 0.2f, 0.2f, 0f, 0f, 1.0f, 0.0f);
        SystemClock.sleep ( 20 ); 
        angle += 2; 
        //rotate triangle 
        gl.glRotatef(angle, 0, 0, 1);
        //magnify triangle 
        gl.glScalef ( 1, 0.8f, 0.8f);
        
        // Draw the triangle
        gl.glColor4f(1.0f, 1.0f, 0.0f, 0.0f);
        gl.glVertexPointer(3, GL10.GL_FLOAT, 0, triangle);
        gl.glDrawArrays(GL10.GL_TRIANGLES, 0, 3);
        gl.glColor4f(1.0f, 0.0f, 1.0f, 0.0f);
        gl.glVertexPointer(3, GL10.GL_FLOAT, 0, triangle2);
        gl.glDrawArrays(GL10.GL_TRIANGLES, 0, 3);
        gl.glColor4f(0.0f, 1.0f, 0.0f, 0.0f);
        gl.glVertexPointer(3, GL10.GL_FLOAT, 0, triangle3);
        gl.glDrawArrays(GL10.GL_TRIANGLES, 0, 3);
        gl.glColor4f(0.0f, 1.0f, 1.0f, 0.0f);
        gl.glVertexPointer(3, GL10.GL_FLOAT, 0, triangle4);
        gl.glDrawArrays(GL10.GL_TRIANGLES, 0, 3);

    }
    
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        gl.glViewport(0, 0, width, height);
    }
    
    private void initShapes(){        
            	
    	float vertices_1[] = {
    			-0.6f, -0.6f, 0.85f, 
    			0.6f, -0.6f, 0.85f,
    			0.0f, 0.6f, 0
    	};
    	
    	float vertices_2[] = {
    			0.0f, 0.6f, 0,
    			0.6f, -0.6f, 0.85f,
    			0, 0, -0.6f,
    	};
    	
    	float vertices_3[] = {
    			0, 0.6f, 0,
    			0, 0, -0.6f,
    			-0.6f, -0.6f, 0.85f
    	};
    	
    	float vertices_4[] = {
    			0, 0, -0.6f,
    			0.6f, -0.6f, 0.85f,
    			-0.6f, -0.6f, 0.85f, 
    	};
    	
        // initialize vertex Buffer for triangle  
        ByteBuffer v1 = ByteBuffer.allocateDirect(
                // (# of coordinate values * 4 bytes per float)
                vertices_1.length * 4); 
        v1.order(ByteOrder.nativeOrder());// use the device hardware's native byte order
        triangle = v1.asFloatBuffer();  // create a floating point buffer from the ByteBuffer
        triangle.put(vertices_1);    // add the coordinates to the FloatBuffer
        triangle.position(0);            // set the buffer to read the first coordinate    
        
        ByteBuffer v2 = ByteBuffer.allocateDirect(vertices_2.length * 4); 
        v2.order(ByteOrder.nativeOrder());// use the device hardware's native byte order
        triangle2 = v2.asFloatBuffer();  // create a floating point buffer from the ByteBuffer
        triangle2.put(vertices_2);    // add the coordinates to the FloatBuffer
        triangle2.position(0);            // set the buffer to read the first coordinate    
        
        ByteBuffer v3 = ByteBuffer.allocateDirect(vertices_3.length * 4); 
        v3.order(ByteOrder.nativeOrder());// use the device hardware's native byte order
        triangle3 = v3.asFloatBuffer();  // create a floating point buffer from the ByteBuffer
        triangle3.put(vertices_3);    // add the coordinates to the FloatBuffer
        triangle3.position(0);            // set the buffer to read the first coordinate 
        
        ByteBuffer v4 = ByteBuffer.allocateDirect(vertices_4.length * 4); 
        v4.order(ByteOrder.nativeOrder());// use the device hardware's native byte order
        triangle4 = v4.asFloatBuffer();  // create a floating point buffer from the ByteBuffer
        triangle4.put(vertices_4);    // add the coordinates to the FloatBuffer
        triangle4.position(0);
    }
}