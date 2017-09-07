package cse520.icosahedron;

import android.app.Activity;
import android.os.Bundle;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

public class MainActivity extends Activity {
	private GLSurfaceView mGLView;
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		mGLView = new IcoSurfaceView(this);
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
	
	class IcoSurfaceView extends GLSurfaceView {
		private final float TOUCH_SCALE_FACTOR = 180.0f / 320;
		private IcoRenderer renderer;
		private float previousX;
		private float previousY;
		
		public IcoSurfaceView(Context context){
			super(context);
			// set the renderer member
			renderer = new IcoRenderer();
			setRenderer(renderer);
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
					if (y > getHeight() / 2) {
						dx = dx	* -1 ;
						//dy = dy * -1;
					}
					// reverse direction of rotation to left of the mid-line
					if (x < getWidth() / 2) {
						dy = dy	* -1 ;
						//dx = dx * -1;
					}
					renderer.angleX += (dx + dy)	* TOUCH_SCALE_FACTOR;
					renderer.angleZ += (dx + dy)	* TOUCH_SCALE_FACTOR;
					requestRender();
			}
			previousX = x;
			previousY = y;
			return true;
		}
	}
}