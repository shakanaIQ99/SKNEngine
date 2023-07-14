#include "PostEffect.h"
#include <d3dx12.h>

using namespace DirectX;

void PostEffect::Initialize(SpriteCommon* spritecommon, WorldTransform* wt, uint32_t handle)
{
	Sprite2D::Initialize(spritecommon, wt, handle);
	Update();
}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdlist, XMFLOAT2 anchor, bool flipX, bool flipY)
{
	int isFlipX, isFlipY;
	if (flipX == false)isFlipX = 1;
	else isFlipX = -1;
	if (flipY == false)isFlipY = 1;
	else isFlipY = -1;

	float left = ((0.0f - anchor.x) * tex->width) * isFlipX;
	float right = ((1.0f - anchor.x) * tex->width) * isFlipX;
	float top = ((0.0f - anchor.y) * tex->height) * isFlipY;
	float bottom = ((1.0f - anchor.y) * tex->height) * isFlipY;

	VertexPos vertices[] =
	{
		{{left,top,0.0f},{0.0f,0.0f}	},
		{{left,bottom,0.0f},{0.0f,1.0f}	},
		{{right,top,0.0f},{1.0f,0.0f}	},
		{{right,bottom,0.0f},{1.0f,1.0f}}
	};
	uint32_t indices[] =
	{
		1,0,3,
		2,3,0,
	};

	vertexBuffer->Update(vertices);

	indexBuffer->Update(indices);

	Update();

	spritecommon->DrawCommand(tex, vertexBuffer->GetView(), indexBuffer->GetView(), Wt);
}
