#include "Scene.h"

Scene::Scene(){
	setupGBuffer();

	loadShaders();

	lights.resize(1);

	parameters.setName("Renderer");
	parameters.add(bDrawGBuffer.set("Draw GBuffer", true));
}

void Scene::loadShaders(){
	gBufferShader.load("shaders/gBuffer_vert.glsl", "shaders/gBuffer_frag.glsl");
	deferredShader.load("shaders/deferred_vert.glsl", "shaders/deferred_frag.glsl");
}

void Scene::setupGBuffer(){
	ofFbo::Settings gBufferSettings;
	// gBufferSettings.width = ofGetWidth();
	// gBufferSettings.height = ofGetHeight();
	gBufferSettings.width = ofNextPow2(ofGetWidth());
	gBufferSettings.height = ofNextPow2(ofGetHeight());
	// gBufferSettings.numColorbuffers = 3;		// how many color buffers to create
	for(int i = 0; i < 3; ++i){
		gBufferSettings.colorFormats.push_back(GL_RGBA32F);		// format of the color attachments for MRT.
	}
	gBufferSettings.useDepth = true;				// whether to use depth buffer or not
	gBufferSettings.useStencil = false;				// whether to use stencil buffer or not
	gBufferSettings.depthStencilAsTexture = true;			// use a texture instead of a renderbuffer for depth (useful to draw it or use it in a shader later)
	// gBufferSettings.textureTarget = GL_TEXTURE_RECTANGLE_ARB;			// GL_TEXTURE_2D or GL_TEXTURE_RECTANGLE_ARB
	gBufferSettings.textureTarget = GL_TEXTURE_2D;			// GL_TEXTURE_2D or GL_TEXTURE_RECTANGLE_ARB
	// gBufferSettings.internalformat = GL_RGBA32F;			// GL_RGBA, GL_RGBA16F_ARB, GL_RGBA32F_ARB, GL_LUMINANCE32F_ARB etc.
	gBufferSettings.depthStencilInternalFormat = GL_DEPTH_COMPONENT32; 	// GL_DEPTH_COMPONENT(16/24/32)
	gBufferSettings.wrapModeHorizontal = GL_REPEAT;		// GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER etc.
	gBufferSettings.wrapModeVertical = GL_REPEAT;		// GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER etc.
	gBufferSettings.minFilter = GL_LINEAR;				// GL_NEAREST, GL_LINEAR etc.
	gBufferSettings.maxFilter = GL_LINEAR;				// GL_NEAREST, GL_LINEAR etc.
	// gBufferSettings.numSamples = 4;
	gBuffer.allocate(gBufferSettings);
}

void Scene::addLight(const ofLight& light){
	lights.push_back(light);
	lights.back().setPosition(light.getPosition());
}

void Scene::setLight(const ofLight& light){
	lights[0] = light;
	lights[0].setPosition(light.getPosition());
}

void Scene::setMaterial(ofMaterial _material){
	material = _material;
}

void Scene::beginGBuffer(const ofEasyCam& _cam){
	cam = _cam;
	gBuffer.begin();
		gBuffer.activateAllDrawBuffers();
			// cam.begin();
		gBufferShader.begin();
			ofClear(0, 0, 0, 255);
			gBufferShader.setUniform1f("zFar", cam.getFarClip());
				// ofSetColor(material.getAmbientColor());
}

void Scene::endGBuffer(){
		gBufferShader.end();
			// cam.end();
	gBuffer.end();
}

// void Scene::begin(){
// 	phongShader.begin();
// 		ofVec3f p = light.getPosition();
// 		phongShader.setUniform3f("lightPosition", p.x, p.y, p.z);
// 		// Set light uniforms
// 		ofFloatColor c(light.getAmbientColor());
// 		phongShader.setUniform3f("light.color", c.r, c.g, c.b);
// 		phongShader.setUniform1f("light.power", 1.0);
// 		// Set material uniforms
// 		ofFloatColor ac(material.getAmbientColor());
// 		phongShader.setUniform3f("material.ambient", ac.r, ac.g, ac.b);
// 		ofFloatColor dc(material.getDiffuseColor());
// 		phongShader.setUniform3f("material.diffuse", dc.r, dc.g, dc.b);
// 		ofFloatColor sc(material.getSpecularColor());
// 		phongShader.setUniform3f("material.specular", sc.r, sc.g, sc.b);
// 		phongShader.setUniform1f("material.shininess", material.getShininess());
// }

void Scene::render(){

	deferredShader.begin();
		deferredShader.setUniformTexture("normalMap", gBuffer.getTextureReference(0), 0);
		deferredShader.setUniformTexture("depthMap", gBuffer.getTextureReference(1), 1);
		// deferredShader.setUniformTexture("depthMap", gBuffer.getDepthTexture(), 1);
		deferredShader.setUniformTexture("colorMap", gBuffer.getTextureReference(2), 2);
		deferredShader.setUniform2f("res", gBuffer.getWidth(), gBuffer.getHeight());
		deferredShader.setUniform1f("zNear", cam.getNearClip());
		deferredShader.setUniform1f("zFar", cam.getFarClip());
		deferredShader.setUniform1f("fov", cam.getFov());
		deferredShader.setUniform1f("aspectRatio", cam.getAspectRatio());
		deferredShader.setUniformMatrix4f("camModelViewMatrix", cam.getModelViewMatrix());
		// ofLog() << cam.getModelViewMatrix();

		// Set material uniforms
		// ofFloatColor ac(material.getAmbientColor());
		// deferredShader.setUniform3f("material.ambient", ac.r, ac.g, ac.b);
		ofFloatColor dc(material.getDiffuseColor());
		deferredShader.setUniform3f("material.diffuse", dc.r, dc.g, dc.b);
		ofFloatColor sc(material.getSpecularColor());
		deferredShader.setUniform3f("material.specular", sc.r, sc.g, sc.b);
		deferredShader.setUniform1f("material.shininess", material.getShininess());

		for(auto& l : lights){
			ofVec3f p = l.getPosition();
			// ofVec3f p = -l.getPosition();
			deferredShader.setUniform3f("lightPosition", p.x, p.y, p.z);
			ofFloatColor c(l.getAmbientColor());
			deferredShader.setUniform3f("light.color", c.r, c.g, c.b);
			deferredShader.setUniform1f("light.power", 1.0);
		}
		// cam.begin();
		gBuffer.draw(0, 0);
		// cam.end();
	deferredShader.end();

	if(bDrawGBuffer){
		drawGBuffer();
	}
}

void Scene::drawGBuffer(){
	ofSetColor(ofColor::white);
	float ratio = ofGetWidth() / ofGetHeight();
	float h = 200.0;
	float y = ofGetHeight() - h;
	float w = h * ratio;
	gBuffer.getTextureReference(0).draw(0, y, w, h);
	gBuffer.getTextureReference(1).draw(w, y, w, h);
	// gBuffer.getDepthTexture().draw(w, 0, w, h);
	gBuffer.getTextureReference(2).draw(2 * w, y, w, h);
}