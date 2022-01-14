#include "HelloTriangles.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"


HelloTriangles::HelloTriangles(void)
{
}

HelloTriangles::~HelloTriangles(void)
{
	this->Destroy();
}

void HelloTriangles::Init(const char *path)
{
	InputManager::CreateInstance();

	ResourceManager::CreateInstance();
	ResourceManager::GetInstance()->Load(path, "Resources.txt");

	SceneManager::CreateInstance();
	SceneManager::GetInstance()->Load(path, "Level0.txt");

	//Vertex verticesData[3];

	////init object 1
	//verticesData[0].position =  Vector3(0.0f, 0.0f, 0.5f);
	//verticesData[1].position = Vector3(0.0f, 0.0f, -0.5f);
	//verticesData[2].position = Vector3(0.0f, 0.5f, 0.0f);

	//verticesData[0].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	//verticesData[1].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	//verticesData[2].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

	////buffer object 1
	//object[0]->Init(verticesData, 3);
	//object[0]->position = Vector3(0.2f, 0.0f, -10.0f);
	//object[0]->rotation = Vector3(0.0f, 0.0f, 0.0f);
	//object[0]->scale = Vector3(1.0f, 1.0f, 5.0f);

	////init object 2
	//verticesData[0].color = Vector4(1.0f, 1.0f, 0.0f, 1.0f);
	//verticesData[1].color = Vector4(0.0f, 1.0f, 1.0f, 1.0f);
	//verticesData[2].color = Vector4(1.0f, 0.0f, 1.0f, 1.0f);

	//object[1]->Init(verticesData, 3);
	//object[1]->position = Vector3(-0.2f, 0.0f, 10.0f);
	//object[1]->rotation = Vector3(0.0f, 0.0f, 0.0f);
	//object[1]->scale = Vector3(1.0f, 1.0f, 5.0f);

	////init object 3
	//object[2]->Init(verticesData, 3);
	//object[2]->position = Vector3(0.0f, 0.0f, -13.0f);
	//object[2]->rotation = Vector3(0.0f, 1.0f, 0.0f);
	//object[2]->scale = Vector3(1.0f, 1.0f, 0.5f);
	//Vector3 position, rotation, scale;

	//position = Vector3(0.9f, 0.0f, -10.0f);
	//rotation = Vector3(0.0f, 0.0f, 0.0f);
	//scale = Vector3(1.0f, 1.0f, 1.0f);
	//object[0]->Init(position, rotation, scale);
	//object[0]->SetShader(resMan->GetShaderElementAt(0));
	//object[0]->SetModel(resMan->GetModelElementAt(0));
	//object[0]->SetTexture(resMan->GetTextureElementAt(0));

	//position = Vector3(-0.9f, 0.0f, -10.0f);
	//rotation = Vector3(0.0f, 0.0f, 0.0f);
	//scale = Vector3(1.0f, 1.0f, 1.0f);
	//object[1]->Init(position, rotation, scale);
	//object[1]->SetShader(resMan->GetShaderElementAt(0));
	//object[1]->SetModel(resMan->GetModelElementAt(1));
	//object[1]->SetTexture(resMan->GetTextureElementAt(1));

	//position = Vector3(0.0f, 0.0f, -10.0f);
	//rotation = Vector3(0.0f, 0.0f, 0.0f);
	//scale = Vector3(1.0f, 1.0f, 1.0f);
	//object[2]->Init(position, rotation, scale);
	//object[2]->SetShader(resMan->GetShaderElementAt(0));
	//object[2]->SetModel(resMan->GetModelElementAt(2));
	//object[2]->SetTexture(resMan->GetTextureElementAt(2));

	////init camera
	//position = Vector3(0.0f, 10.0f, -10.0f);
	//rotation = Vector3(-1.57f, 0.0f, 0.0f);
	//scale = Vector3(5.0f, 5.0f, 5.0f);
	//camera->Init(position, rotation, scale);
}
void HelloTriangles::Update()
{
	SceneManager::GetInstance()->Update();
	
	//GameObject *examObject = SceneManager::GetInstance()->GetGameObject("WOMAN1");
	   

}

void HelloTriangles::Render()
{
	SceneManager::GetInstance()->Render();
}

void HelloTriangles::Destroy()
{
	InputManager::DestroyInstance();
	ResourceManager::DestroyInstance();
	SceneManager::DestroyInstance();
}