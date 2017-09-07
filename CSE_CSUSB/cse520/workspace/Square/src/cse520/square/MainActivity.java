package cse520.square;

import android.app.Activity;
import android.os.Bundle;
import android.content.Context;
import android.opengl.GLSurfaceView;

public class MainActivity extends Activity {
	private GLSurfaceView mGLView;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        // Create a GLSurfaceView instance and set it
        // as the ContentView for this Activity.
        mGLView = new HelloESSurfaceView(this);
        setContentView(mGLView);
    }
    
    @Override
    protected void onPause() {
        super.onPause();
        // The following call pauses the rendering thread.
        mGLView.onPause();
    }
    
    @Override
    protected void onResume() {
        super.onResume();
        // The following call resumes a paused rendering thread.
        mGLView.onResume();
    }
    
    class HelloESSurfaceView extends GLSurfaceView {

        public HelloESSurfaceView(Context context){
            super(context);
            
            // Set the Renderer for drawing on the GLSurfaceView
            setRenderer(new HelloESRenderer());
        }
    }
}