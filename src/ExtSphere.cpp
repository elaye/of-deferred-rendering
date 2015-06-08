#include "ExtSphere.h"

void ExtSphere::setup(){
	parameters.setName("Parameters");
	parameters.add(scale.set("Scale", 0.5, 0.0, 1.0));
	parameters.add(radius.set("Radius", 150.0, 10.0, 400.0));
	parameters.add(extRadius.set("Extrusion radius", 10.0, -100.0, 100.0));
	parameters.add(noiseScale.set("Noise scale", 1.0, 0.0, 2.0));
	parameters.add(noiseAmp.set("Noise amplitude", 3.0, 0.0, 10.0));

	scale.addListener(this, &ExtSphere::changeParameter);
	radius.addListener(this, &ExtSphere::changeParameter);
	extRadius.addListener(this, &ExtSphere::changeParameter);
	noiseScale.addListener(this, &ExtSphere::changeParameter);
	noiseAmp.addListener(this, &ExtSphere::changeParameter);

	sphere.setMode(OF_PRIMITIVE_TRIANGLES);
	faces.setMode(OF_PRIMITIVE_TRIANGLES);

	generateMesh();
}

void ExtSphere::generateMesh(){
	ofVboMesh icosphere = ofVboMesh::icosphere(radius, 3);

	vector<ofMeshFace> facesTriangles;
	vector<ofMeshFace> sphereTriangles;

	for(auto& face : icosphere.getUniqueFaces()){
		ofMeshFace extFace = extrudeFace(face);
		facesTriangles.push_back(extFace);

		ofMeshFace t0;
		t0.setHasNormals(true);
		t0.setVertex(0, face.getVertex(0));
		t0.setVertex(1, face.getVertex(2));
		t0.setVertex(2, extFace.getVertex(0));
		t0.getFaceNormal();
		sphereTriangles.push_back(t0);

		ofMeshFace t1;
		t1.setHasNormals(true);
		t1.setVertex(0, extFace.getVertex(2));
		t1.setVertex(1, extFace.getVertex(0));
		t1.setVertex(2, face.getVertex(2));
		sphereTriangles.push_back(t1);

		ofMeshFace t2;
		t2.setHasNormals(true);
		t2.setVertex(0, face.getVertex(0));
		t2.setVertex(1, extFace.getVertex(1));
		t2.setVertex(2, face.getVertex(1));
		sphereTriangles.push_back(t2);

		ofMeshFace t3;
		t3.setHasNormals(true);
		t3.setVertex(0, face.getVertex(0));
		t3.setVertex(1, extFace.getVertex(0));
		t3.setVertex(2, extFace.getVertex(1));
		sphereTriangles.push_back(t3);

		ofMeshFace t4;
		t4.setHasNormals(true);
		t4.setVertex(0, face.getVertex(1));
		t4.setVertex(1, extFace.getVertex(2));
		t4.setVertex(2, face.getVertex(2));
		sphereTriangles.push_back(t4);

		ofMeshFace t5;
		t5.setHasNormals(true);
		t5.setVertex(0, face.getVertex(1));
		t5.setVertex(1, extFace.getVertex(1));
		t5.setVertex(2, extFace.getVertex(2));
		sphereTriangles.push_back(t5);
	}

	faces.setFromTriangles(facesTriangles);
	sphere.setFromTriangles(sphereTriangles, true);
}

ofMeshFace ExtSphere::extrudeFace(const ofMeshFace& face){
	ofMeshFace extFace;
	ofVec3f n = -face.getFaceNormal();
	// n.normalize();
	// ofVec3f faceCenter = radius * n;
	ofVec3f faceCenter;
	for(int i = 0; i < 3; ++i){
		faceCenter += face.getVertex(i);
	}
	faceCenter /= 3.0;

	for(int i = 0; i < 3; ++i){
		// Shrink face
		// ofVec3f u = face.getVertex(i).getNormalized() - n;
		// ofVec3f v = face.getVertex(i) + scale * u.normalize();
		ofVec3f u = face.getVertex(i) - faceCenter;
		ofVec3f v = faceCenter + scale * u;
		// Extrude face
		ofVec3f c = noiseScale * faceCenter / radius;
		v += extRadius * noiseAmp * ofNoise(c.x, c.y, c.z) * n;
		v += extRadius * n;
		// ofVec3f v = face.getVertex(i) - (radius + extRadius + scale) * n;
		extFace.setVertex(i, v);
		extFace.setNormal(i, n);
	}
	return extFace;
}

ofVec3f ExtSphere::getFaceNormal(vector<ofVec3f>& face){
	ofVec3f v1 = face[1] - face[0];
	ofVec3f v2 = face[2] - face[0];
	ofVec3f n= v1.cross(v2);
	n.normalize();
	return n;
}

void ExtSphere::update(){

}

void ExtSphere::draw(){
	sphere.draw();
	faces.draw();
}

void ExtSphere::changeParameter(float& v){
	faces.clear();
	sphere.clear();
	generateMesh();
}