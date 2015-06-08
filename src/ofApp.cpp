#include "ofApp.h"

void ofApp::setup(){
	setupLight();
	setupMaterial();

	// scene.addLight(light);
	scene.setLight(light);
	scene.setMaterial(material);

	extSphere.setup();

	cam.orbit(0, -10, 300);
	ofVec3f p = cam.getPosition();
	cam.setPosition(p.x, p.y + 20.0, p.z);
	// cam.setupPerspective(true, 60, 1, 2000);

	gui.setup(scene.parameters);
}

void ofApp::setupLight(){
	lightLat = 80;
	lightLon = -60;
	lightDist = 200;
	light.orbit(lightLat, lightLon, lightDist);
	light.lookAt(ofVec3f(0.0));
	light.setAmbientColor(ofFloatColor(1.0, 0.6, 0.5, 1.0));
}

void ofApp::setupMaterial(){
	material.setDiffuseColor(ofFloatColor(1.0, 1.0, 0.5, 1.0));
	material.setAmbientColor(ofFloatColor(0.1, 0.1, 0.1, 1.0) * material.getDiffuseColor());
	material.setSpecularColor(ofFloatColor(1.0, 0.7, 0.7, 1.0));
	material.setShininess(200.0);
}

void ofApp::update(){

}

void ofApp::draw(){
	ofBackgroundGradient( ofColor(40,40,40), ofColor(0,0,0), OF_GRADIENT_CIRCULAR);	
		
	ofEnableDepthTest();

	scene.beginGBuffer(cam);
	cam.begin();
		drawGrid();
		drawScene();
		drawLight();
		cam.end();
	scene.endGBuffer();

	ofDisableDepthTest();
	scene.render();

	// ofSetColor(ofColor::white);
	// scene.drawGBuffer();
	gui.draw();
}

void ofApp::drawScene(){
	ofSetColor(material.getAmbientColor());
	// ofDrawSphere(0, 0, 0, 150);
	extSphere.draw();
}

void ofApp::drawLight(){
	ofPushMatrix();
		ofSetColor(light.getAmbientColor());
		ofVec3f p = light.getPosition();
		ofTranslate(p);
		ofDrawArrow(p, p-light.getLookAtDir(), 10);
	ofPopMatrix();
}

void ofApp::drawGrid(){
	ofSetColor(ofColor(60));
	ofPushMatrix();
		ofRotate(90, 0, 0, -1);
		ofDrawGridPlane(500, 10);
	ofPopMatrix();	
}

void ofApp::keyPressed(int key){
	switch(key){
		case 'r':
			scene.loadShaders();
			break;
		case 356:
			lightLat += 3;
			light.orbit(lightLat, lightLon, lightDist);
			scene.setLight(light);
			break;
		case 357:
			lightLon -= 3;
			lightLon = max(-90, lightLon);
			light.orbit(lightLat, lightLon, lightDist);
			scene.setLight(light);
			break;
		case 358:
			lightLat -= 3;
			light.orbit(lightLat, lightLon, lightDist);
			scene.setLight(light);
			break;
		case 359:
			lightLon += 3;
			lightLon = min(90, lightLon);
			light.orbit(lightLat, lightLon, lightDist);
			scene.setLight(light);
			break;
		case 360:
			lightDist += 5;
			light.dolly(5.0);
			scene.setLight(light);
			break;
		case 361:
			lightDist -= 5;
			light.dolly(-5.0);
			scene.setLight(light);
			break;
		default:
			break;
	}
}