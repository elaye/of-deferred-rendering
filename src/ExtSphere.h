#pragma once

#include "ofMain.h"

class ExtSphere{

	ofParameter<float> radius, extRadius, scale;
	ofParameter<float> noiseScale, noiseAmp;

	ofVboMesh sphere;
	ofVboMesh faces;

	public:
		ofParameterGroup parameters; 
		void setup();
		void update();
		void draw();

		ofVec3f getFaceNormal(vector<ofVec3f>& face);
		ofMeshFace extrudeFace(const ofMeshFace& face);
		void generateMesh();

		void changeParameter(float& v);
		
};