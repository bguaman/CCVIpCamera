#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	mjpeg.connect("http://root:root@192.168.64.28/axis-cgi/mjpg/video.cgi");
	//client.connect("http://83.161.133.106/axis-cgi/mjpg/video.cgi?resolution=480x300&dummy=1300696302973");
}

//--------------------------------------------------------------
void testApp::update(){

	mjpeg.grabFrame();
}

//--------------------------------------------------------------
void testApp::draw(){

	mjpeg.draw(20, 20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

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

//--------------------------------------------------------------

//--------------------------------------------------------------
