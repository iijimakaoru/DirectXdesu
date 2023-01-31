#include "Player.h"
#include "KInput.h"
#include "Boss.h"
#include "Matrix4.h"
#include "ParticleManager.h"
#include "KInput.h"
#include <imgui.h>

Player* Player::nowPlayer = nullptr;

Player::Player()
{

}

void Player::Init(KModel* model, KGPlin* pipeline)
{
	object = std::make_unique<KObject3d>();
	object->Initialize();
	object->SetPipeline(pipeline);
	object->LoadModel(model);
	object->transform.scale = { 10,10,10 };
}

void Player::Update(ViewProjection& viewProjection)
{
	char bufK[255] = "KeyBord";
	char bufG[255] = "GamePad";
	ImGui::Text("ControllerMode");
	if (ImGui::Button("ChangeController"))
	{
		if (mode == ControllerMode::KeyBord)
		{
			mode = ControllerMode::GamePad;
		}
		else if (mode == ControllerMode::GamePad)
		{
			mode = ControllerMode::KeyBord;
		}
		else
		{

		}
	}
	if (mode == ControllerMode::KeyBord)
	{
		ImGui::InputText("mode", bufK, IM_ARRAYSIZE(bufK));
	}
	else if (mode == ControllerMode::GamePad)
	{
		ImGui::InputText("mode", bufG, IM_ARRAYSIZE(bufG));
	}
	else
	{

	}

	KInput* input = KInput::GetInstance();

	if (mode == ControllerMode::KeyBord)
	{
		if (input->IsPush(DIK_RIGHT))
		{
			object->transform.pos.x += 1.0f;
		}
		if (input->IsPush(DIK_LEFT))
		{
			object->transform.pos.x -= 1.0f;
		}

		if (input->IsPush(DIK_UP))
		{
			object->transform.pos.z += 1.0f;
		}
		if (input->IsPush(DIK_DOWN))
		{
			object->transform.pos.z -= 1.0f;
		}
	}
	else if (mode == ControllerMode::GamePad)
	{
		if (input->GetPadConnect())
		{
			Vector2 stick = input->GetPadLStick();
			
			if (stick.x != 0 || stick.y != 0)
			{
				stick.normalize();

				object->transform.pos.x += 1 * stick.x;
				object->transform.pos.z += 1 * stick.y;
			}
		}
	}
	else
	{

	}

	object->Update(viewProjection);
}

void Player::Draw()
{
	object->Draw();
}

void Player::Damage()
{

}

void Player::AttackHit()
{

}
