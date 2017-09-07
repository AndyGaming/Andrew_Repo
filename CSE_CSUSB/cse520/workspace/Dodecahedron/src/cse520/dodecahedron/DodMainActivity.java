package cse520.dodecahedron;

//import cse520.dodcahedron.DodecahedronRenderer;
import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.Window;
import android.view.WindowManager;
import android.opengl.GLSurfaceView;

public class DodMainActivity extends Activity {
    /** Called when the activity is first created. */
	private GLSurfaceView mGLSurfaceView;
    @Override
    public void onCreate(Bundle savedInstanceState) {
      super.onCreate(savedInstanceState);
      // requesting to turn the title OFF
      requestWindowFeature(Window.FEATURE_NO_TITLE);
      // making it full screen
      getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
      WindowManager.LayoutParams.FLAG_FULLSCREEN);

      // Create our Preview view and set it as the content of our
      // Activity
      mGLSurfaceView = new GLSurfaceView(this);
      DodecahedronRenderer renderer = new DodecahedronRenderer();
      mGLSurfaceView.setRenderer(renderer);
      setContentView(mGLSurfaceView);
    }
    
    @Override
    protected void onResume() {
      // Ideally a game should implement onResume() and onPause()
      // to take appropriate action when the activity looses focus
      super.onResume();
      mGLSurfaceView.onResume();
    }
    
    @Override
    protected void onPause() {
      // Ideally a game should implement onResume() and onPause()
      // to take appropriate action when the activity looses focus
      super.onPause();
      mGLSurfaceView.onPause();
    }
    
    class DodSurfaceView extends GLSurfaceView {
    	private final float TOUCH_SCALE_FACTOR = 180.0f / 320;
    	private DodecahedronRenderer renderer;
    	private float previousX;
    	private float previousY;

        public DodSurfaceView(Context context){
            super(context);
            renderer = new DodecahedronRenderer();
            // Set the Renderer for drawing on the GLSurfaceView
            setRenderer(new DodecahedronRenderer());
            // Render the view only when there is a change
            //setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
        }
        
        @Override
        public boolean onTouchEvent(MotionEvent e) {
        	// MotionEvent reports input details from the touch screen
        	// and other input controls. Here, we are only interested
        	// in events where the touch position has changed.
        	float x = e.getX();
        	float y = e.getY();
        	switch (e.getAction()) {
        		case MotionEvent.ACTION_MOVE:
        			float dx = x - previousX;
        			float dy = y - previousY;
        	// reverse direction of rotation above the mid-line
        			if (y > getHeight() / 2)
        				dx = dx * -1;
        	// reverse direction of rotation to left of the mid-line
        			if (x < getWidth() / 2)
        				dy = dy * -1;
        			renderer.anglex += (dx + dy) * TOUCH_SCALE_FACTOR;
        			renderer.anglez += (dx + dy) * TOUCH_SCALE_FACTOR;
        			requestRender();
        	}
        	previousX = x;
        	previousY = y;
        	return true;
        }
    }
}