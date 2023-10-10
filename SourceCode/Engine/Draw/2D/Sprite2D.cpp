#include "Sprite2D.h"

void Sprite2D::Initialize(SpriteCommon* _spritecommon,uint32_t handle)
{
	spritecommon = _spritecommon;
	tex.reset(TextureManager::GetTextureData(handle));

	CreateVertexIndexBuffer();
	Wt.CreateConstBuffer();

	
}

void Sprite2D::Update()
{
	Wt.UpdateSpriteMatrix(spritecommon->Getmat());
}

void Sprite2D::Draw(XMFLOAT2 anchor, bool flipX, bool flipY)
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

	spritecommon->DrawCommand(tex.get(), vertexBuffer->GetView(), indexBuffer->GetView(), &Wt);

}

void Sprite2D::DrawClip(XMFLOAT2 ClipPos, XMFLOAT2 ClipSize, bool flipX, bool flipY)
{
	int isFlipX, isFlipY;
	if (flipX == false)isFlipX = 1;
	else isFlipX = -1;
	if (flipY == false)isFlipY = 1;
	else isFlipY = -1;

	float left = -ClipSize.x * isFlipX;
	float right = ClipSize.x * isFlipX;
	float top = -ClipSize.y * isFlipY;
	float bottom = ClipSize.y * isFlipY;



	VertexPos vertices[] =
	{
		{{left,top,0.0f}	,{(ClipPos.x - ClipSize.x) / tex->width,(ClipPos.y - ClipSize.y) / tex->height}	},
		{{left,bottom,0.0f}	,{(ClipPos.x - ClipSize.x) / tex->width,(ClipPos.y + ClipSize.y) / tex->height}	},
		{{right,top,0.0f}	,{(ClipPos.x + ClipSize.x) / tex->width,(ClipPos.y - ClipSize.y) / tex->height}	},
		{{right,bottom,0.0f},{(ClipPos.x + ClipSize.x) / tex->width,(ClipPos.y + ClipSize.y) / tex->height}}
	};
	uint32_t indices[] =
	{
		1,0,3,
		2,3,0,
	};

	vertexBuffer->Update(vertices);

	indexBuffer->Update(indices);

	Update();

	spritecommon->DrawCommand(tex.get(), vertexBuffer->GetView(), indexBuffer->GetView(), &Wt);

}



void Sprite2D::CreateVertexIndexBuffer()
{
	vertexBuffer = make_unique<VertexBuffer>();
	vertexBuffer->Create(4, sizeof(VertexPos));

	indexBuffer = make_unique<IndexBuffer>();
	indexBuffer->Create(6);
}

