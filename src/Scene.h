#pragma once

#include "ofMain.h"

#include "Scene.h"

class Scene {

	ofShader gBufferShader, deferredShader;

	ofFbo gBuffer;

	ofEasyCam cam;

	vector<ofLight> lights;
	ofMaterial material;

	ofParameter<bool> bDrawGBuffer;
	
	public:
		ofParameterGroup parameters;
		Scene();

		void setupGBuffer();
		void loadShaders();

		void beginGBuffer(const ofEasyCam& cam);
		void endGBuffer();

		void render();

		void drawGBuffer();
		
		void addLight(const ofLight& light);
		void setLight(const ofLight& light);
		void setMaterial(ofMaterial material);
};