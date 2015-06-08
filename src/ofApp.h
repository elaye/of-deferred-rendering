#pragma once

#include "ofMain.h"

#include "ofxGui.h"

#include "Scene.h"
#include "ExtSphere.h"

class ofApp : public ofBaseApp{

	ofEasyCam cam;
	Scene scene;

	ofLight light;
	ofMaterial material;

	ExtSphere extSphere;

	int lightLat, lightLon, lightDist;

	ofxPanel gui;

	public:
		void setup();
		void setupLight();
		void setupMaterial();
		void update();
		void draw();
		void drawScene();
		void drawLight();
		void drawGrid();
		void keyPressed(int key);
};
