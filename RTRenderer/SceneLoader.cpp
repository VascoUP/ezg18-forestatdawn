#include "SceneLoader.h"

enum LightType {
	SPOT, POINT, DIRECTIONAL
};

const std::string TRANSFORMS_KEY = "transforms";
const std::string TRANSFORM_KEY = "transform";

const std::string POSITION_KEY = "translation";
const std::string ROTATION_KEY = "rotation";
const std::string SCALE_KEY = "scale";

const std::string AMBIENT_KEY = "ambient";
const std::string DIFFUSE_COLOR_KEY = "diffcolor";
const std::string DIFFUSE_INTENSITY_KEY = "diffintensity";
const std::string SPECULAR_COLOR_KEY = "speccolor";
const std::string SPECULAR_INTENSITY_KEY = "specintensity";
const std::string CONSTANT_KEY = "constant";
const std::string LINEAR_KEY = "linear";
const std::string EXPONENT_KEY = "exponent";
const std::string EDGE_KEY = "edge";
const std::string RED_KEY = "red";
const std::string GREEN_KEY = "green";
const std::string BLUE_KEY = "blue";
const std::string X_KEY = "x";
const std::string Y_KEY = "y";
const std::string Z_KEY = "z";

const std::string LIGHTS_KEY = "lights";
const std::string DIRECTIONAL_LIGHT_KEY = "dlight";
const std::string POINT_LIGHTS_KEY = "plights";
const std::string POINT_LIGHT_KEY = "plight_%d";
const std::string SPOT_LIGHTS_KEY = "slights";
const std::string SPOT_LIGHT_KEY = "slight_%d";

const std::string MODELS_KEY = "models";

const std::string SHADERS_KEY = "shaders";
const std::string SHADER_KEY = "shader_%d";
const std::string VERTEX_SHADER_KEY = "vertex";
const std::string FRAGMENT_SHADER_KEY = "fragment";

void LoadLight(LightType type, nlohmann::json light, GLRenderer * meshRenderer, Transform* rootObject) {
	GLfloat diffIntensity = light[DIFFUSE_INTENSITY_KEY];
	GLfloat diffRed = light[DIFFUSE_COLOR_KEY][RED_KEY];
	GLfloat diffGreen = light[DIFFUSE_COLOR_KEY][GREEN_KEY];
	GLfloat diffBlue = light[DIFFUSE_COLOR_KEY][BLUE_KEY];
	GLfloat specIntensity = light[SPECULAR_INTENSITY_KEY];
	GLfloat specRed = light[SPECULAR_COLOR_KEY][RED_KEY];
	GLfloat specGreen = light[SPECULAR_COLOR_KEY][GREEN_KEY];
	GLfloat specBlue = light[SPECULAR_COLOR_KEY][BLUE_KEY]; 
	
	Transform *lt;
	GLfloat pitch;
	GLfloat yaw;
	GLfloat roll;
	GLfloat xPos;
	GLfloat yPos;
	GLfloat zPos;
	GLfloat constant;
	GLfloat linear;
	GLfloat exponent;
	GLfloat edge;

	switch (type) {
	case DIRECTIONAL:
		pitch = light[ROTATION_KEY][X_KEY];
		yaw = light[ROTATION_KEY][Y_KEY];
		roll = light[ROTATION_KEY][Z_KEY];
		lt = new Transform(rootObject);
		lt->Rotate(pitch, yaw, roll);
		meshRenderer->SetDirectionalLight(
			new DirectionalLight(lt, 
				4096, 4096,
				4096, 4096,
				diffIntensity, diffRed, diffGreen, diffBlue, 
				specIntensity, specRed, specGreen, specBlue));
		break;
	case POINT:
		xPos = light[POSITION_KEY][X_KEY];
		yPos = light[POSITION_KEY][Y_KEY];
		zPos = light[POSITION_KEY][Z_KEY];
		constant = light[CONSTANT_KEY];
		linear = light[LINEAR_KEY];
		exponent = light[EXPONENT_KEY];
		lt = new Transform(rootObject);
		lt->Translate(glm::vec3(xPos, yPos, zPos));
		meshRenderer->AddPointLight(
			new PointLight(lt,
				0.01f, 100.0f,
				512, 512,
				512, 512,
				constant, linear, exponent, 
				diffIntensity, diffRed, diffGreen, diffBlue, 
				specIntensity, specRed, specGreen, specBlue));
		break;
	case SPOT:
		pitch = light[ROTATION_KEY][X_KEY];
		yaw = light[ROTATION_KEY][Y_KEY];
		roll = light[ROTATION_KEY][Z_KEY];
		xPos = light[POSITION_KEY][X_KEY];
		yPos = light[POSITION_KEY][Y_KEY];
		zPos = light[POSITION_KEY][Z_KEY];
		constant = light[CONSTANT_KEY];
		linear = light[LINEAR_KEY];
		exponent = light[EXPONENT_KEY];
		edge = light[EDGE_KEY];
		lt = new Transform(rootObject);
		lt->Translate(glm::vec3(xPos, yPos, zPos));
		lt->Rotate(pitch, yaw, roll);
		meshRenderer->AddSpotLight(
			new SpotLight(lt,
				0.01f, 10.0f,
				512, 512,
				512, 512,
				edge, constant, linear, exponent, 
				diffIntensity, diffRed, diffGreen, diffBlue, 
				specIntensity, specRed, specGreen, specBlue));
		break;
	}
}

void LoadModels(nlohmann::json jsonObject, GLRenderer * meshRenderer) {
	std::vector<std::string> modelLocations = jsonObject[MODELS_KEY];
	for (size_t i = 0; i < modelLocations.size(); i++) {
		Model *model = new Model(modelLocations[i].c_str());
		model->Load();
		GLModelRenderer* modelRenderer = new GLModelRenderer();
		modelRenderer->SetRenderable(model);
		meshRenderer->AddObjectRenderer(modelRenderer);
	}
}

void LoadShaders(nlohmann::json shaders, GLRenderer* meshRenderer) {
	std::string jsonStr = shaders.dump();
	int i = 0;
	char locBuff[100] = { "\0" };
	snprintf(locBuff, sizeof(locBuff), SHADER_KEY.c_str(), i);

	nlohmann::json obj = shaders[locBuff];

	while (obj.type_name() != "null") {
		DefaultShader *shader = new DefaultShader();
		std::string vertexLocation = obj[VERTEX_SHADER_KEY];
		std::string fragmentLocation = obj[FRAGMENT_SHADER_KEY];
		shader->CreateFromFiles(vertexLocation.c_str(), fragmentLocation.c_str());
		meshRenderer->AddShader(shader);

		i++;
		snprintf(locBuff, sizeof(locBuff), POINT_LIGHT_KEY.c_str(), i);
		obj = shaders[locBuff];
	}
}

void LoadTransforms(nlohmann::json transform, GLRenderer* meshRenderer, Transform* rootObject) {

}

std::string SimulateJSONObject() {
	nlohmann::json simulateScene;
	simulateScene["ambient"] = 0.05f;

	nlohmann::json simulateDLight;
	simulateDLight["diffintensity"] = 0.8f;
	simulateDLight["specintensity"] = 0.8f;
	simulateDLight["diffcolor"]["red"] = 0.9f;
	simulateDLight["diffcolor"]["green"] = 0.9f;
	simulateDLight["diffcolor"]["blue"] = 1.0f;
	simulateDLight["speccolor"]["red"] = 0.9f;
	simulateDLight["speccolor"]["green"] = 0.9f;
	simulateDLight["speccolor"]["blue"] = 1.0f;
	simulateDLight["rotation"]["x"] = -0.5f;
	simulateDLight["rotation"]["y"] = -0.5f;
	simulateDLight["rotation"]["z"] = 0.0f;
	
	nlohmann::json simulatePLight0;
	simulatePLight0["diffintensity"] = 0.6f;
	simulatePLight0["specintensity"] = 0.6f;
	simulatePLight0["diffcolor"]["red"] = 0.9f;
	simulatePLight0["diffcolor"]["green"] = 1.0f;
	simulatePLight0["diffcolor"]["blue"] = 0.9f;
	simulatePLight0["speccolor"]["red"] = 0.9f;
	simulatePLight0["speccolor"]["green"] = 1.0f;
	simulatePLight0["speccolor"]["blue"] = 0.9f;
	simulatePLight0["translation"]["x"] = 8.0f;
	simulatePLight0["translation"]["y"] = 0.5f;
	simulatePLight0["translation"]["z"] = 0.0f;
	simulatePLight0["constant"] = 0.1f;
	simulatePLight0["linear"] = 0.05f;
	simulatePLight0["exponent"] = 0.02f;

	nlohmann::json simulatePLight1;
	simulatePLight1["diffintensity"] = 0.6f;
	simulatePLight1["specintensity"] = 0.6f;
	simulatePLight1["diffcolor"]["red"] = 0.9f;
	simulatePLight1["diffcolor"]["green"] = 1.0f;
	simulatePLight1["diffcolor"]["blue"] = 0.9f;
	simulatePLight1["speccolor"]["red"] = 0.8f;
	simulatePLight1["speccolor"]["green"] = 1.0f;
	simulatePLight1["speccolor"]["blue"] = 0.8f;
	simulatePLight1["translation"]["x"] = -2.0f;
	simulatePLight1["translation"]["y"] = 0.5f;
	simulatePLight1["translation"]["z"] = -2.0f;
	simulatePLight1["constant"] = 0.1f;
	simulatePLight1["linear"] = 0.05f;
	simulatePLight1["exponent"] = 0.02f;
	
	nlohmann::json simulateLights;
	simulateLights["dlight"] = simulateDLight;
	simulateLights["plights"]["plight_0"] = simulatePLight0;
	simulateLights["plights"]["plight_1"] = simulatePLight1;

	simulateScene["lights"] = simulateLights;

	// https://free3d.com/3d-model/mountain-6839.html
	simulateScene["models"] = { "Models/uh60.obj", "Models/Tree.obj", "Models/Tree_02.obj", "Models/everest.obj" };

	nlohmann::json simulatedShader;
	simulatedShader["shader_0"]["vertex"] = "Shaders/shader.vert";
	simulatedShader["shader_0"]["fragment"] = "Shaders/shader.frag";

	simulateScene["shaders"] = simulatedShader;

	return simulateScene.dump();
}

void BuildScene(GLRenderer * meshRenderer, Transform* root, GLWindow* glWindow, bool isCinematic) {
	// -- Helicopter --
	Transform *transform = new Transform(root);
	transform->SetStatic(false);
	transform->Scale(0.5f);
	transform->Translate(glm::vec3(0.0f, 10.0f, 0.0f));
	transform->Rotate(-1.57f, 3.14f, 0.0f);
	Material* mat = new Material(1.0f, 50, 1.0f, 1.0f, 1.0f);
	meshRenderer->AddMeshRenderer(new GLObject(transform, mat, 0));
	transform->AddUpdatable(new HelicopterController(transform, 7.0f, 1.0f));

	// -- Type 1 trees --
	transform = new Transform(root);
	transform->Translate(glm::vec3(10.0f, 0.0f, 0.0f));
	mat = new Material(0.8f, 50, 1.0f, 1.0f, 1.0f);
	meshRenderer->AddMeshRenderer(new GLObject(transform, mat, 1));

	transform = new Transform(root);
	transform->Translate(glm::vec3(-5.0f, 0.0f, -5.0f));
	mat = new Material(0.8f, 50, 1.0f, 1.0f, 1.0f);
	meshRenderer->AddMeshRenderer(new GLObject(transform, mat, 1));

	transform = new Transform(root);
	transform->Translate(glm::vec3(-1.0f, 0.0f, 5.0f));
	mat = new Material(0.8f, 50, 1.0f, 1.0f, 1.0f);
	meshRenderer->AddMeshRenderer(new GLObject(transform, mat, 1));

	transform = new Transform(root);
	transform->Translate(glm::vec3(2.0f, 0.0f, -3.0f));
	mat = new Material(0.8f, 50, 1.0f, 1.0f, 1.0f);
	meshRenderer->AddMeshRenderer(new GLObject(transform, mat, 1));

	//transform = new Transform(root);
	//transform->Translate(glm::vec3(2.0f, 0.0f, 10.0f));
	//mat = new Material(0.8f, 50, 1.0f, 1.0f, 1.0f);
	//meshRenderer->AddMeshRenderer(new GLObject(transform, mat, 1));

	//transform = new Transform(root);
	//transform->Translate(glm::vec3(-5.0f, 0.0f, -5.0f));
	//mat = new Material(0.8f, 50, 1.0f, 1.0f, 1.0f);
	//meshRenderer->AddMeshRenderer(new GLObject(transform, mat, 1));

	//transform = new Transform(root);
	//transform->Translate(glm::vec3(5.0f, 0.0f, 5.0f));
	//mat = new Material(0.8f, 50, 1.0f, 1.0f, 1.0f);
	//meshRenderer->AddMeshRenderer(new GLObject(transform, mat, 1));

	// -- Type 2 trees --
	transform = new Transform(root);
	transform->Translate(glm::vec3(0.0f, 0.0f, 2.0f));
	mat = new Material(0.8f, 50, 1.0f, 1.0f, 1.0f);
	meshRenderer->AddMeshRenderer(new GLObject(transform, mat, 2));

	transform = new Transform(root);
	transform->Translate(glm::vec3(3.0f, 0.0f, -1.0f));
	mat = new Material(0.8f, 50, 1.0f, 1.0f, 1.0f);
	meshRenderer->AddMeshRenderer(new GLObject(transform, mat, 2));

	transform = new Transform(root);
	transform->Translate(glm::vec3(-5.0f, 0.0f, 1.0f));
	mat = new Material(0.8f, 50, 1.0f, 1.0f, 1.0f);
	meshRenderer->AddMeshRenderer(new GLObject(transform, mat, 2));

	transform = new Transform(root);
	transform->Translate(glm::vec3(2.0f, 0.0f, -5.0f));
	mat = new Material(0.8f, 50, 1.0f, 1.0f, 1.0f);
	meshRenderer->AddMeshRenderer(new GLObject(transform, mat, 2));

	transform = new Transform(root);
	transform->Translate(glm::vec3(3.0f, 0.0f, 8.0f));
	mat = new Material(0.8f, 50, 1.0f, 1.0f, 1.0f);
	meshRenderer->AddMeshRenderer(new GLObject(transform, mat, 2));

	//transform = new Transform(root);
	//transform->Translate(glm::vec3(7.0f, 0.0f, -2.0f));
	//mat = new Material(0.8f, 50, 1.0f, 1.0f, 1.0f);
	//meshRenderer->AddMeshRenderer(new GLObject(transform, mat, 2));

	//transform = new Transform(root);
	//transform->Translate(glm::vec3(8.0f, 0.0f, 2.0f));
	//mat = new Material(0.8f, 50, 1.0f, 1.0f, 1.0f);
	//meshRenderer->AddMeshRenderer(new GLObject(transform, mat, 2));

	//transform = new Transform(root);
	//transform->Translate(glm::vec3(6.0f, 0.0f, -5.0f));
	//mat = new Material(0.8f, 50, 1.0f, 1.0f, 1.0f);
	//meshRenderer->AddMeshRenderer(new GLObject(transform, mat, 2));

	// Moutain
	transform = new Transform(root);
	transform->Scale(40.0f);
	transform->Translate(glm::vec3(10.0f, -1.0f, -10.0f));
	Texture* tex = new Texture("Textures/mountainTex.png");
	tex->LoadTexture();
	mat = new Material(1.0f, 100, 1.0f, 1.0f, 1.0f, tex);
	meshRenderer->AddMeshRenderer(new GLObject(transform, mat, 3));

	{
		// -- Terrain --
		TerrainMesh* terrain = TerrainMesh::CreateInstance();
		GLMeshRenderer* renderer = new GLMeshRenderer();
		renderer->SetRenderable(terrain);
		meshRenderer->AddObjectRenderer(renderer);
		transform = new Transform(root);
		mat = new Material(0.2f, 2.0f, 1.0f, 1.0f, 1.0f);
		meshRenderer->AddMeshRenderer(new GLObject(transform, mat, terrain->GetIndex()));
	}

	// Camera object is not static
	transform = new Transform(root);
	transform->SetStatic(false);
	transform->Translate(glm::vec3(0.0f, 5.0f, -5.0f));
	transform->AddUpdatable(Camera::CreateInstance(transform, glWindow));

	if(!isCinematic)
		transform->AddUpdatable(new CameraController(transform, 5.0f, 5.0f));
	else {
		std::vector<KeyFrame>* keyFrames = new std::vector<KeyFrame>();
		KeyFrame* kf = new KeyFrame(); kf->deltaTime = 0.0f;
		kf->position = glm::vec3(-28.875362f, 2.862897f, -18.041098f); kf->rotation = glm::vec3(0.046448f, -5.315226f, 0.000000f);
		keyFrames->push_back(*kf);
		kf = new KeyFrame(); kf->deltaTime = 5.0f;
		kf->position = glm::vec3(-15.327765f, 1.731312f, -5.199117f); kf->rotation = glm::vec3(-0.115507f, -4.878115f, 0.000000f);
		keyFrames->push_back(*kf);
		kf = new KeyFrame(); kf->deltaTime = 5.0f;
		kf->position = glm::vec3(-7.341257f, 1.202060f, -1.114085f); kf->rotation = glm::vec3(0.092232f, -5.395631f, 0.000000f);
		keyFrames->push_back(*kf);
		kf = new KeyFrame(); kf->deltaTime = 5.0f;
		kf->position = glm::vec3(-5.010897f, 1.067676f, -1.449261f); kf->rotation = glm::vec3(0.091000f, -6.191921f, 0.000000f);
		keyFrames->push_back(*kf);
		kf = new KeyFrame(); kf->deltaTime = 5.0f;
		kf->position = glm::vec3(-2.262272f, 1.067676f, -0.195358f); kf->rotation = glm::vec3(0.152200f, -7.645885f, 0.000000f);
		keyFrames->push_back(*kf);
		kf = new KeyFrame(); kf->deltaTime = 5.0f;
		kf->position = glm::vec3(-2.262272f, 1.067676f, -0.195358f); kf->rotation = glm::vec3(0.152367f, -5.006465f, 0.000000f);
		keyFrames->push_back(*kf);
		kf = new KeyFrame(); kf->deltaTime = 5.0f;
		kf->position = glm::vec3(-1.783493f, 0.552925f, -0.074246f); kf->rotation = glm::vec3(0.079541f, -4.985230f, 0.000000f);
		keyFrames->push_back(*kf);
		kf = new KeyFrame(); kf->deltaTime = 5.0f;
		kf->position = glm::vec3(-0.030454f, 0.454973f, -0.810777f); kf->rotation = glm::vec3(0.064178f, -6.256577f, 0.000000f);
		keyFrames->push_back(*kf);
		kf = new KeyFrame(); kf->deltaTime = 5.0f;
		kf->position = glm::vec3(1.409825f, 0.454973f, -0.080610f); kf->rotation = glm::vec3(0.028798f, -7.496764f, 0.000000f);
		keyFrames->push_back(*kf);
		kf = new KeyFrame(); kf->deltaTime = 5.0f;
		kf->position = glm::vec3(4.626716f, 0.498400f, 2.444399f); kf->rotation = glm::vec3(0.017401f, -8.480882f, 0.000000f);
		keyFrames->push_back(*kf);
		kf = new KeyFrame(); kf->deltaTime = 5.0f;
		kf->position = glm::vec3(7.982166f, 0.702529f, 8.565200f); kf->rotation = glm::vec3(-0.097604f, -9.598670f, 0.000000f);
		keyFrames->push_back(*kf);
		kf = new KeyFrame(); kf->deltaTime = 5.0f;
		kf->position = glm::vec3(6.796952f, 0.892327f, 1.489373f); kf->rotation = glm::vec3(0.047980f, -8.563046f, 0.000000f);
		keyFrames->push_back(*kf);
		kf = new KeyFrame(); kf->deltaTime = 0.5f;
		kf->position = glm::vec3(6.796952f, 0.892327f, 1.489373f); kf->rotation = glm::vec3(0.047980f, -8.563046f, 0.000000f);
		keyFrames->push_back(*kf);
		kf = new KeyFrame(); kf->deltaTime = 5.0f;
		kf->position = glm::vec3(10.646914f, 3.687983f, -5.437775f); kf->rotation = glm::vec3(-0.399553f, -9.763049f, 0.000000f);
		keyFrames->push_back(*kf);
		kf = new KeyFrame(); kf->deltaTime = 5.0f;
		kf->position = glm::vec3(15.224915f, 13.397384f, -15.589467f); kf->rotation = glm::vec3(-0.726606f, -9.851732f, 0.000000f);
		keyFrames->push_back(*kf);
		kf = new KeyFrame(); kf->deltaTime = 5.0f;
		kf->position = glm::vec3(22.064701f, 19.724745f, -25.869707f); kf->rotation = glm::vec3(0.468682f, -13.301495f, 0.000000f);
		keyFrames->push_back(*kf);
		kf = new KeyFrame(); kf->deltaTime = 5.0f;
		kf->position = glm::vec3(15.891579f, 12.788266f, -11.988614f); kf->rotation = glm::vec3(0.210817f, -13.278668f, 0.000000f);
		keyFrames->push_back(*kf);
		transform->AddUpdatable(new AnimateKeyFrame(transform, keyFrames));

		// -- Animate Spheres --
		GLCubeMapRenderer* cubemapRenderer = meshRenderer->GetCubemapRenderer();
		// -- Refract sphere --
		keyFrames = new std::vector<KeyFrame>();
		kf = new KeyFrame(); kf->deltaTime = 0.0f;
		kf->position = glm::vec3(0.0f, 0.5f, 1.0f); kf->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		keyFrames->push_back(*kf);
		kf = new KeyFrame(); kf->deltaTime = 10.0f;
		kf->position = glm::vec3(0.0f, 0.5f, 1.0f); kf->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		keyFrames->push_back(*kf);
		kf = new KeyFrame(); kf->deltaTime = 10.0f;
		kf->position = glm::vec3(0.0f, 0.5f, 1.0f); kf->rotation = glm::vec3(0.0f, 3.14f, 0.0f);
		keyFrames->push_back(*kf);
		kf = new KeyFrame(); kf->deltaTime = 10.0f;
		kf->position = glm::vec3(0.0f, 0.5f, 1.0f); kf->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		keyFrames->push_back(*kf);
		kf = new KeyFrame(); kf->deltaTime = 15.0f;
		kf->position = glm::vec3(0.0f, 0.5f, 1.0f); kf->rotation = glm::vec3(0.0f, 3.14f, 0.0f);
		keyFrames->push_back(*kf);
		kf = new KeyFrame(); kf->deltaTime = 5.0f;
		kf->position = glm::vec3(7.0f, 0.5f, 3.0f); kf->rotation = glm::vec3(0.0f, 3.14f, 0.0f);
		keyFrames->push_back(*kf);
		kf = new KeyFrame(); kf->deltaTime = 5.0f;
		kf->position = glm::vec3(5.0f, 0.5f, 0.0f); kf->rotation = glm::vec3(0.0f, 3.14f, 0.0f);
		keyFrames->push_back(*kf);
		cubemapRenderer->GetRefractTransform()->AddUpdatable(new AnimateKeyFrame(cubemapRenderer->GetRefractTransform(), keyFrames));
		// -- Reflect sphere --
		keyFrames = new std::vector<KeyFrame>();
		glm::vec3(0.0f, 0.5f, 0.0f);
		kf = new KeyFrame(); kf->deltaTime = 0.0f;
		kf->position = glm::vec3(-5.0f, 1.0f, 0.0f); kf->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		keyFrames->push_back(*kf);
		kf = new KeyFrame(); kf->deltaTime = 5.0f;
		kf->position = glm::vec3(-5.0f, 1.0f, 0.0f); kf->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		keyFrames->push_back(*kf);
		kf = new KeyFrame(); kf->deltaTime = 20.0f;
		kf->position = glm::vec3(-5.0f, 1.0f, 0.0f); kf->rotation = glm::vec3(0.0f, 3.14f, 0.0f);
		keyFrames->push_back(*kf);
		kf = new KeyFrame(); kf->deltaTime = 10.0f;
		kf->position = glm::vec3(-2.0f, 1.0f, -2.0f); kf->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		keyFrames->push_back(*kf);
		kf = new KeyFrame(); kf->deltaTime = 10.0f;
		kf->position = glm::vec3(2.0f, 1.0f, -5.0f); kf->rotation = glm::vec3(0.0f, 3.14f, 0.0f);
		keyFrames->push_back(*kf);
		cubemapRenderer->GetReflectTransform()->AddUpdatable(new AnimateKeyFrame(cubemapRenderer->GetReflectTransform(), keyFrames));
	}

	root->AddUpdatable(new ActivateLights(transform));
}

void SceneLoader::Load(const char* filename, GLRenderer * meshRenderer, Transform* rootObject, GLWindow* glWindow, bool isCinematic) {
	// Todo: Read from file
	std::string fileText = SimulateJSONObject();

	nlohmann::json sceneDescription = nlohmann::json::parse(fileText);
	float ambientIntensity = sceneDescription[AMBIENT_KEY];
	meshRenderer->SetAmbient(ambientIntensity);
	nlohmann::json lights = sceneDescription[LIGHTS_KEY];
	
	// -- Parse direct lights --
	nlohmann::json obj_1 = lights[DIRECTIONAL_LIGHT_KEY];

	if(obj_1.type_name() != "null") {
		LoadLight(DIRECTIONAL, obj_1, meshRenderer, rootObject);
	}

	// -- Parse point lights --
	int i = 0;
	char locBuff[100] = { "\0" };
	obj_1 = lights[POINT_LIGHTS_KEY];

	snprintf(locBuff, sizeof(locBuff), POINT_LIGHT_KEY.c_str(), i);
	nlohmann::json obj_2 = obj_1[locBuff];

	while (obj_2.type_name() != "null") {
		LoadLight(POINT, obj_2, meshRenderer, rootObject);

		i++;
		snprintf(locBuff, sizeof(locBuff), POINT_LIGHT_KEY.c_str(), i);
		obj_2 = obj_1[locBuff];
	}

	// -- Parse spot lights --
	i = 0;
	obj_1 = lights[SPOT_LIGHTS_KEY];

	snprintf(locBuff, sizeof(locBuff), SPOT_LIGHT_KEY.c_str(), i);
	obj_2 = obj_1[locBuff];

	while (obj_2.type_name() != "null") {
		LoadLight(SPOT, obj_2, meshRenderer, rootObject);

		i++;
		snprintf(locBuff, sizeof(locBuff), SPOT_LIGHT_KEY.c_str(), i);
		obj_2 = obj_1[locBuff];
	}

	// -- Parse models --
	LoadModels(sceneDescription, meshRenderer);

	// -- Parse shaders --
	LoadShaders(sceneDescription[SHADERS_KEY], meshRenderer);

	// -- Parse transforms --
	nlohmann::json transforms = sceneDescription[TRANSFORMS_KEY];
	if(transforms.type_name() != "null") LoadTransforms(transforms, meshRenderer, rootObject);

	BuildScene(meshRenderer, rootObject, glWindow, isCinematic);
}

void SceneLoader::Store(const char* filename, GLRenderer * meshRenderer, Transform * transform) {
	nlohmann::json simulateScene;
	simulateScene["ambient"] = meshRenderer->GetAmbient();

	DirectionalLight* dLight = meshRenderer->GetDirectionalLight();
	nlohmann::json simulateDLight;
	simulateDLight["diffintensity"] = dLight->GetDiffuseIntensity();
	simulateDLight["specintensity"] = dLight->GetSpecularIntensity();
	glm::vec3 tmpVec3 = dLight->GetDiffuseColor();
	simulateDLight["diffcolor"]["red"] = tmpVec3.r;
	simulateDLight["diffcolor"]["green"] = tmpVec3.g;
	simulateDLight["diffcolor"]["blue"] = tmpVec3.b;
	tmpVec3 = dLight->GetSpecularColor();
	simulateDLight["speccolor"]["red"] = tmpVec3.r;
	simulateDLight["speccolor"]["green"] = tmpVec3.g;
	simulateDLight["speccolor"]["blue"] = tmpVec3.b;
	tmpVec3 = dLight->GetTransform()->GetRotation();
	simulateDLight["rotation"]["x"] = tmpVec3.x;
	simulateDLight["rotation"]["y"] = tmpVec3.y;
	simulateDLight["rotation"]["z"] = tmpVec3.z;

	nlohmann::json simulatePLight0;
	simulatePLight0["diffintensity"] = 0.7f;
	simulatePLight0["specintensity"] = 0.7f;
	simulatePLight0["diffcolor"]["red"] = 0.0f;
	simulatePLight0["diffcolor"]["green"] = 1.0f;
	simulatePLight0["diffcolor"]["blue"] = 0.0f;
	simulatePLight0["speccolor"]["red"] = 1.0f;
	simulatePLight0["speccolor"]["green"] = 0.0f;
	simulatePLight0["speccolor"]["blue"] = 0.0f;
	simulatePLight0["translation"]["x"] = 0.0f;
	simulatePLight0["translation"]["y"] = 1.0f;
	simulatePLight0["translation"]["z"] = 0.0f;
	simulatePLight0["constant"] = 0.3f;
	simulatePLight0["linear"] = 0.2f;
	simulatePLight0["exponent"] = 0.1f;

	nlohmann::json simulatePLight1;
	simulatePLight1["diffintensity"] = 0.7f;
	simulatePLight1["specintensity"] = 0.7f;
	simulatePLight1["diffcolor"]["red"] = 1.0f;
	simulatePLight1["diffcolor"]["green"] = 1.0f;
	simulatePLight1["diffcolor"]["blue"] = 1.0f;
	simulatePLight1["speccolor"]["red"] = 1.0f;
	simulatePLight1["speccolor"]["green"] = 1.0f;
	simulatePLight1["speccolor"]["blue"] = 1.0f;
	simulatePLight1["translation"]["x"] = 2.0f;
	simulatePLight1["translation"]["y"] = 1.0f;
	simulatePLight1["translation"]["z"] = 4.0f;
	simulatePLight1["constant"] = 0.1f;
	simulatePLight1["linear"] = 0.05f;
	simulatePLight1["exponent"] = 0.02f;

	nlohmann::json simulateLights;
	simulateLights["dlight"] = simulateDLight;
	simulateLights["plights"]["plight_0"] = simulatePLight0;
	simulateLights["plights"]["plight_1"] = simulatePLight1;

	simulateScene["lights"] = simulateLights;

	simulateScene["models"] = { "Models/uh60.obj", "Models/Tree.obj", "Models/Tree_02.obj" };

	nlohmann::json simulatedShader;
	simulatedShader["shader_0"]["vertex"] = "Shaders/shader.vert";
	simulatedShader["shader_0"]["fragment"] = "Shaders/shader.frag";

	simulateScene["shaders"] = simulatedShader;

}
