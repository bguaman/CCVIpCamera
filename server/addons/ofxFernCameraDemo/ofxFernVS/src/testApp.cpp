/*
如何做自己的marker
For anyone who wants to use her/his own images: 
http://forum.openframeworks.cc/index.php?topic=3483.15
1. within the project find the folder with files model.bmp, model.bmp.roi, model.bmp.detector_data and model.bmp.tracker_data
2. delete the model.bmp.detector_data and model.bmp.tracker_data files
3. replace model.bmp with your own model.bmp
4. edit model.bmp.roi to reflect the position of marker in your model.bmp (to get the idea open Theo's model.bmp in photoshop and his model.bmp.roi in texteditor to see which corner corresponds to which line in model.bmp.roi, the values are in pixels)
5. run the ofxFernCameraDemo and it will generate new model.bmp.detector_data and model.bmp.tracker_data files. this will take some time (a few minutes on my mac mini 2GHz) and it helps to run it from within XCode and have the console open to see the progress going on..
6. thats it! the demo should now work with your marker from model.bmp! if it doesnt work, check the horizontal mirroring of the image - if your model.bmp is mirrored with respect to camera input the marker won't be recognized..
hope this helps ..
now, maybe Theo would give some advice on which factors of the tracked marker help to make the tracking as robust as in his app :)
*/
//修改过msv.cc 2011.9.18


#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	
	int camW = 640;
	int camH = 480;

	fern.initTracker("model.bmp",camW, camH);
	grabber.initGrabber(camW, camH);
	colorImg.allocate(camW, camH);
	img.allocate(camW, camH);
	
}

//--------------------------------------------------------------
void testApp::update(){
	
	grabber.update();	
	if( grabber.isFrameNew() ){
		colorImg = grabber.getPixels();
		img = colorImg;
		fern.update(img);
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(0xFFFFFF);
	fern.draw(0, 0, 640, 480);

	ofDrawBitmapString("keys 1-5 do stuff\nfps: "+ofToString(ofGetFrameRate(), 2), 10, 500);
	
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	fern.keyPressed(key);
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

