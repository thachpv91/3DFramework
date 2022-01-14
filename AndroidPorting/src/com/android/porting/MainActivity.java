package com.android.porting;

import android.app.Activity;
import android.os.Bundle;


public class MainActivity extends Activity {

	GL2JNIView mView;
	
	@Override protected void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		mView = new GL2JNIView(getApplication());
		setContentView(mView);
	}
	
	@Override protected void onPause() {
		super.onPause();
		mView.onPause();
	}
	
	@Override protected void onResume() {
		super.onResume();
		mView.onResume();
	}
}