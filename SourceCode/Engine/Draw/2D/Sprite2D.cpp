#include "Sprite2D.h"
#include "SpriteCommon.h"
#include"TextureManager.h"

void Sprite2D::Initialize(TextureHandle Handle)
{
	handle = Handle;
	size.x = (float)TextureManager::GetTextureData(Handle).texResource->GetDesc().Width;
	size.y = (float)TextureManager::GetTextureData(Handle).texResource->GetDesc().Height;
	matProjection = Matrix4::OrthoGraphicProjection(0.0f, DxWindow::window_width, 0.0f, DxWindow::window_height, 0.0f, 1.0f);
	CreateVertexIndexBuffer();
	Wt.CreateConstBuffer();
}

void Sprite2D::Update()
{
	Wt.UpdateSpriteMatrix(matProjection);
}

void Sprite2D::Draw(float x, float y, float x2, float y2,bool flipX, bool flipY)
{
	int isFlipX, isFlipY;
	if (flipX == false)isFlipX = 1;
	else isFlipX = -1;
	if (flipY == false)isFlipY = 1;
	else isFlipY = -1;

	float left =  x * isFlipX;
	float right = x2 * isFlipX;
	float top = y * isFlipY;
	float bottom =y2 * isFlipY;



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

	
	SpriteCommon::DrawCommand(handle, vertexBuffer->GetView(), indexBuffer->GetView(), &Wt);
}

void Sprite2D::Draw(Vector2 anchor, bool flipX, bool flipY)
{
	int isFlipX, isFlipY;
	if (flipX == false)isFlipX = 1;
	else isFlipX = -1;
	if (flipY == false)isFlipY = 1;
	else isFlipY = -1;

	float left = ((0.0f - anchor.x) * size.x) * isFlipX;
	float right = ((1.0f - anchor.x) * size.x) * isFlipX;
	float top = ((0.0f - anchor.y) * size.y) * isFlipY;
	float bottom = ((1.0f - anchor.y) * size.y) * isFlipY;



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

	SpriteCommon::DrawCommand(handle, vertexBuffer->GetView(), indexBuffer->GetView(), &Wt);

}

void Sprite2D::DrawClip(Vector2 ClipPos, Vector2 ClipSize, bool flipX, bool flipY)
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
		{{left,top,0.0f}	,{(ClipPos.x - ClipSize.x) / size.x,(ClipPos.y - ClipSize.y) / size.y}	},
		{{left,bottom,0.0f}	,{(ClipPos.x - ClipSize.x) / size.x,(ClipPos.y + ClipSize.y) / size.y}	},
		{{right,top,0.0f}	,{(ClipPos.x + ClipSize.x) / size.x,(ClipPos.y - ClipSize.y) / size.y}	},
		{{right,bottom,0.0f},{(ClipPos.x + ClipSize.x) / size.x,(ClipPos.y + ClipSize.y) / size.y}}
	};
	uint32_t indices[] =
	{
		1,0,3,
		2,3,0,
	};

	vertexBuffer->Update(vertices);

	indexBuffer->Update(indices);

	Update();

	SpriteCommon::DrawCommand(handle, vertexBuffer->GetView(), indexBuffer->GetView(), &Wt);

}

void Sprite2D::SetTexture(TextureHandle Handle)
{
	handle = Handle;
	size.x = (float)TextureManager::GetTextureData(Handle).texResource->GetDesc().Width;
	size.y = (float)TextureManager::GetTextureData(Handle).texResource->GetDesc().Height;
}



void Sprite2D::CreateVertexIndexBuffer()
{
	vertexBuffer = make_unique<VertexBuffer>();
	vertexBuffer->Create(4, sizeof(VertexPos));

	indexBuffer = make_unique<IndexBuffer>();
	indexBuffer->Create(6);
}

