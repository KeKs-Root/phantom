#include "D3D11Renderer.h"
#include "Tools.h"
#include "Classes.h"

D3D11Renderer::D3D11Renderer(IDXGISwapChain *SwapChain) {
	this->swapChain = SwapChain;

	this->device = NULL;
	this->deviceContext = NULL;
	this->mVS = NULL;
	this->mPS = NULL;
	this->transparency = NULL;
	this->mInputLayout = NULL;
	this->mVertexBuffer = NULL;

	this->stateSaver = new D3D11StateSaver();
}

D3D11Renderer::~D3D11Renderer() {
	SAFE_DELETE(this->stateSaver);
	SAFE_RELEASE(this->mVS);
	SAFE_RELEASE(this->mPS);
	SAFE_RELEASE(this->transparency);
	SAFE_RELEASE(this->mInputLayout);
	SAFE_RELEASE(this->mVertexBuffer);
	SAFE_RELEASE(this->swapChain);
	SAFE_RELEASE(this->device);
	SAFE_RELEASE(this->deviceContext);
}

HMODULE D3D11Renderer::Initialize() {
	HRESULT hr;
	HMODULE D3DModule;

	if ( !this->swapChain )
		return false;

	this->swapChain->GetDevice(__uuidof( this->device ), (void**)&this->device);
	if ( !this->device )
		return false;

	this->device->GetImmediateContext(&this->deviceContext);
	if ( !this->deviceContext )
		return false;

	//this->m_font = new SpriteFont(this->device, PuristaFont, sizeof(PuristaFont));

	//this->DX11SpriteBatch = std::make_unique<SpriteBatch>(this->deviceContext);

	//this->DX11SpriteBatch.reset(new SpriteBatch(deviceContext));


	typedef HRESULT(__stdcall* D3DCompile_t)( LPCVOID pSrcData, SIZE_T SrcDataSize, LPCSTR pSourceName, const D3D_SHADER_MACRO *pDefines, ID3DInclude *pInclude, LPCSTR pEntrypoint, LPCSTR pTarget, UINT Flags1, UINT Flags2, ID3DBlob **ppCode, ID3DBlob *ppErrorMsgs );
	
	D3DModule = GetD3DCompiler();
	D3DCompile_t myD3DCompile = (D3DCompile_t)GetProcAddress(D3DModule, "D3DCompile");
	if ( !myD3DCompile )
		return false;

	ID3D10Blob *VS, *PS;
	hr = myD3DCompile(D3D11FillShader, sizeof(D3D11FillShader), NULL, NULL, NULL, "VS", "vs_4_0", 0, 0, &VS, NULL);
	if ( FAILED(hr) )
		return false;


	hr = this->device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &this->mVS);
	if ( FAILED(hr) ) {
		SAFE_RELEASE(VS);
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	hr = this->device->CreateInputLayout(layout, ARRAYSIZE(layout), VS->GetBufferPointer(), VS->GetBufferSize(), &this->mInputLayout);
	SAFE_RELEASE(VS);
	if ( FAILED(hr) )
		return false;

	myD3DCompile(D3D11FillShader, sizeof(D3D11FillShader), NULL, NULL, NULL, "PS", "ps_4_0", 0, 0, &PS, NULL);
	if ( FAILED(hr) )
		return false;

	hr = this->device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &this->mPS);
	if ( FAILED(hr) ) {
		SAFE_RELEASE(PS);
		return false;
	}

	D3D11_BUFFER_DESC bufferDesc;

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = 4 * sizeof(COLOR_VERTEX);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;

	hr = this->device->CreateBuffer(&bufferDesc, NULL, &this->mVertexBuffer);
	if ( FAILED(hr) )
		return false;

	D3D11_BLEND_DESC blendStateDescription;
	ZeroMemory(&blendStateDescription, sizeof(blendStateDescription));

	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	hr = this->device->CreateBlendState(&blendStateDescription, &this->transparency);

	if ( FAILED(hr) )
		return false;

	return D3DModule;
}

extern D3D11Renderer *renderer;

extern void draw_rect2(float x, float y, float width, float height, ColorX Color);

typedef BOOLEAN(__fastcall* WorldToScreen_t)( Vector3 * WorldPos, float* x, float* y );
WorldToScreen_t World2Screen = nullptr;

extern float ScreenWidth;
extern float ScreenHight;

BOOLEAN D3D11Renderer::W2S(Vector3 WorldPos, Vector3& ScrPos) {


	float x = 0.0f;
	float y = 0.0f;

	if ( static_cast<BOOLEAN>( World2Screen(&WorldPos, &x, &y) ) ) {
		ScrPos.x = x * ScreenWidth;
		ScrPos.y = y * ScreenHight;
		return TRUE;
	}
	return false;
}

void D3D11Renderer::FillRect(float x, float y, float w, float h, ColorX color, bool Engine_Rect) {
	if ( !Tools->IsValid(deviceContext) )
		return;

	if ( !Engine_Rect ) {

		int a = color.A & 0xff;
		int r = color.R & 0xff;
		int g = color.G & 0xff;
		int b = color.B & 0xff;

		UINT viewportNumber = 1;

		D3D11_VIEWPORT vp;

		this->deviceContext->RSGetViewports(&viewportNumber, &vp);

		float x0 = x;
		float y0 = y;
		float x1 = x + w;
		float y1 = y + h;

		float xx0 = 2.0f * ( x0 - 0.5f ) / vp.Width - 1.0f;
		float yy0 = 1.0f - 2.0f * ( y0 - 0.5f ) / vp.Height;
		float xx1 = 2.0f * ( x1 - 0.5f ) / vp.Width - 1.0f;
		float yy1 = 1.0f - 2.0f * ( y1 - 0.5f ) / vp.Height;

		COLOR_VERTEX* v = NULL;
		D3D11_MAPPED_SUBRESOURCE mapData;

		if ( FAILED(this->deviceContext->Map(this->mVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapData)) )
			return;

		v = (COLOR_VERTEX*)mapData.pData;

		v[0].Position.x = (float)x0;
		v[0].Position.y = (float)y0;
		v[0].Position.z = 0;
		v[0].Color = D3DXCOLOR(
			( (float)r / 255.0f ),
			( (float)g / 255.0f ),
			( (float)b / 255.0f ),
			( (float)a / 255.0f ));

		v[1].Position.x = (float)x1;
		v[1].Position.y = (float)y1;
		v[1].Position.z = 0;
		v[1].Color = D3DXCOLOR(
			( (float)r / 255.0f ),
			( (float)g / 255.0f ),
			( (float)b / 255.0f ),
			( (float)a / 255.0f ));

		v[0].Position.x = xx0;
		v[0].Position.y = yy0;
		v[0].Position.z = 0;
		v[0].Color = D3DXCOLOR(
			( (float)r / 255.0f ),
			( (float)g / 255.0f ),
			( (float)b / 255.0f ),
			( (float)a / 255.0f ));

		v[1].Position.x = xx1;
		v[1].Position.y = yy0;
		v[1].Position.z = 0;
		v[1].Color = D3DXCOLOR(
			( (float)r / 255.0f ),
			( (float)g / 255.0f ),
			( (float)b / 255.0f ),
			( (float)a / 255.0f ));

		v[2].Position.x = xx0;
		v[2].Position.y = yy1;
		v[2].Position.z = 0;
		v[2].Color = D3DXCOLOR(
			( (float)r / 255.0f ),
			( (float)g / 255.0f ),
			( (float)b / 255.0f ),
			( (float)a / 255.0f ));

		v[3].Position.x = xx1;
		v[3].Position.y = yy1;
		v[3].Position.z = 0;
		v[3].Color = D3DXCOLOR(
			( (float)r / 255.0f ),
			( (float)g / 255.0f ),
			( (float)b / 255.0f ),
			( (float)a / 255.0f ));


		this->deviceContext->Unmap(this->mVertexBuffer, NULL);

		UINT Stride = sizeof(COLOR_VERTEX);
		UINT Offset = 0;

		this->deviceContext->IASetVertexBuffers(0, 1, &this->mVertexBuffer, &Stride, &Offset);
		this->deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		this->deviceContext->IASetInputLayout(this->mInputLayout);

		this->deviceContext->VSSetShader(this->mVS, 0, 0);
		this->deviceContext->PSSetShader(this->mPS, 0, 0);
		this->deviceContext->GSSetShader(NULL, 0, 0);
		this->deviceContext->Draw(4, 0);
	} else {
		draw_rect2(x, y, w, h, color);
	}

}


void D3D11Renderer::DrawLine(float x1, float y1, float x2, float y2, ColorX color) {
	if ( this->deviceContext == NULL )
		return;

	int a = color.A & 0xff;
	int r = color.R & 0xff;
	int g = color.G & 0xff;
	int b = color.B & 0xff;

	UINT viewportNumber = 1;

	D3D11_VIEWPORT vp;

	this->deviceContext->RSGetViewports(&viewportNumber, &vp);

	float xx0 = 2.0f * ( x1 - 0.5f ) / vp.Width - 1.0f;
	float yy0 = 1.0f - 2.0f * ( y1 - 0.5f ) / vp.Height;
	float xx1 = 2.0f * ( x2 - 0.5f ) / vp.Width - 1.0f;
	float yy1 = 1.0f - 2.0f * ( y2 - 0.5f ) / vp.Height;

	COLOR_VERTEX* v = NULL;

	D3D11_MAPPED_SUBRESOURCE mapData;

	if ( FAILED(this->deviceContext->Map(this->mVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapData)) )
		return;

	v = (COLOR_VERTEX*)mapData.pData;

	v[0].Position.x = xx0;
	v[0].Position.y = yy0;

	v[0].Position.z = 0;
	v[0].Color = D3DXCOLOR(
		( (float)r / 255.0f ),
		( (float)g / 255.0f ),
		( (float)b / 255.0f ),
		( (float)a / 255.0f ));

	v[1].Position.x = xx1;
	v[1].Position.y = yy1;
	v[1].Position.z = 0;
	v[1].Color = D3DXCOLOR(
		( (float)r / 255.0f ),
		( (float)g / 255.0f ),
		( (float)b / 255.0f ),
		( (float)a / 255.0f ));

	this->deviceContext->Unmap(this->mVertexBuffer, NULL);

	UINT Stride = sizeof(COLOR_VERTEX);
	UINT Offset = 0;

	this->deviceContext->IASetVertexBuffers(0, 1, &this->mVertexBuffer, &Stride, &Offset);
	this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	this->deviceContext->IASetInputLayout(this->mInputLayout);

	this->deviceContext->VSSetShader(this->mVS, 0, 0);
	this->deviceContext->PSSetShader(this->mPS, 0, 0);
	this->deviceContext->GSSetShader(NULL, 0, 0);
	this->deviceContext->Draw(2, 0);
}

void D3D11Renderer::DrawCircle(int x0, int y0, float radius, float thickness, ColorX color) {
	int x = radius, y = 0;
	int radiusError = 1 - x;

	while ( x >= y ) {
		this->FillRect(x + x0, y + y0, thickness, thickness, color);
		this->FillRect(y + x0, x + y0, thickness, thickness, color);
		this->FillRect(-x + x0, y + y0, thickness, thickness, color);
		this->FillRect(-y + x0, x + y0, thickness, thickness, color);
		this->FillRect(-x + x0, -y + y0, thickness, thickness, color);
		this->FillRect(-y + x0, -x + y0, thickness, thickness, color);
		this->FillRect(x + x0, -y + y0, thickness, thickness, color);
		this->FillRect(y + x0, -x + y0, thickness, thickness, color);
		y++;
		if ( radiusError < 0 ) {
			radiusError += 2 * y + 1;
		} else {
			x--;
			radiusError += 2 * ( y - x + 1 );
		}
	}
}

#define PI 3.14159265

void D3D11Renderer::Circle(int X, int Y, int radius, int numSides, ColorX Circle) {
	D3DXVECTOR2 Line[128];
	float Step = PI * 2.0 / numSides;
	int Count = 0;
	for ( float a = 0; a < PI*2.0; a += Step ) {
		float X1 = radius * cos(a) + X;
		float Y1 = radius * sin(a) + Y;
		float X2 = radius * cos(a + Step) + X;
		float Y2 = radius * sin(a + Step) + Y;
		Line[Count].x = X1;
		Line[Count].y = Y1;
		Line[Count + 1].x = X2;
		Line[Count + 1].y = Y2;
		Count += 2;
		this->DrawLine(X1, Y1, X2, Y2, Circle);
	};
}

void D3D11Renderer::DrawBorder(float x, float y, float w, float h, float px, ColorX BorderColor) {
	FillRect(x - px, y - px, ( w + ( px + px ) ), ( h + ( px + px ) ), BorderColor);
}

void D3D11Renderer::DrawBorder2(float x, float y, float w, float h, float thickness, ColorX BorderColor, bool Engine_Rect) {
	//Top horiz line
	FillRect(x, y, w, thickness, BorderColor, Engine_Rect);
	//Left vertical line
	FillRect(x, y, thickness, h, BorderColor, Engine_Rect);
	//right vertical line
	FillRect(( x + w ), y, thickness, h, BorderColor, Engine_Rect);
	//bottom horiz line
	FillRect(x, y + h, w + thickness, thickness, BorderColor, Engine_Rect);
}

void D3D11Renderer::DrawBox(float x, float y, float w, float h, ColorX BoxColor, ColorX BorderColor) {
	//DrawBorder(x, y, w, h, 1, BorderColor);


	FillRect(x + w, y, 1, h+1, BorderColor);
	FillRect(x - 1, y, 1, h+1, BorderColor);
	FillRect(x, y, w, 1, BorderColor);
	FillRect(x, y + h, w, 1, BorderColor);


	FillRect(x + 1, y + 1, w, h, BoxColor);
}

void D3D11Renderer::DrawAimCross(float x, float y, int Size, ColorX CrossColor) {
	DrawBorder2(x - ( Size / 2 ), y - ( Size / 2 ), Size, Size, 1, CrossColor);
	DrawLine(x - ( Size / 2 ), y - ( Size / 2 ), x + ( Size / 2 ), y + ( Size / 2 ), CrossColor);
	DrawLine(x + ( Size / 2 ), y - ( Size / 2 ), x - ( Size / 2 ), y + ( Size / 2 ), CrossColor);
}

void D3D11Renderer::DrawHealthBar(float x, float y, float w, float health, float max) {
	this->DrawHealthBar(x, y, w, 2, health, max);
}

void D3D11Renderer::DrawHealthBar(float x, float y, float w, float h, float health, float max, bool Engine_Rect, bool Side) {
	if ( !Side ) {
		if ( !max )
			return;

		if ( w < 5 )
			return;

		if ( health < 0 )
			health = 0;

		float ratio = health / max;

		ColorX col = ColorX(255, (uchar)( 255 - 255 * ratio ), (uchar)( 255 * ratio ), 0);//Color

		float step = ( w / max );
		float draw = ( step * health );

		FillRect(x, y, w, h, ColorX(80, 0, 0, 0), Engine_Rect); //Background
		FillRect(x, y, draw, h, col, Engine_Rect); //Healthbar
		DrawBorder2(x - 1, y - 1, w + 1, h + 1, 1, ColorX(255, 0, 0, 0), Engine_Rect); //Border

	} else {
		if ( !max )
			return;

		if ( h < 5 )
			return;

		if ( health < 0 )
			health = 0;

		float ratio = health / max;

		ColorX col = ColorX(255, (uchar)( 255 - 255 * ratio ), (uchar)( 255 * ratio ), 0);//Color

		float step = ( h / max );
		float draw = ( step * health );

		FillRect(x, y, w, h, ColorX(80, 0, 0, 0), Engine_Rect); //Background
		FillRect(x, y, w, draw, col, Engine_Rect); //Healthbar
		DrawBorder2(x - 1, y - 1, w + 1, h + 1, 1, ColorX(255, 0, 0, 0), Engine_Rect); //Border
	}


}

void D3D11Renderer::DrawArmorBar(float x, float y, float w, float Armor, float max) {
	this->DrawArmorBar(x, y, w, 2, Armor, max);
}

void D3D11Renderer::DrawArmorBar(float x, float y, float w, float h, float Armor, float max, bool Engine_Rect, bool Side) {
	if ( !Side ) {
		if ( !max )
			return;

		if ( w < 5 )
			return;

		if ( Armor < 0 )
			Armor = 0;

		float ratio = Armor / max;

		ColorX col = ColorX(255, (uchar)( 255 - 255 * ratio ), 0, (uchar)( 255 * ratio ));//Color

		float step = ( w / max );
		float draw = ( step * Armor );

		FillRect(x, y, w, h, ColorX(80, 0, 0, 0), Engine_Rect);; //Background
		FillRect(x, y, draw, h, col, Engine_Rect);;//Armorhbar
		DrawBorder2(x - 1, y - 1, w + 1, h + 1, 1, ColorX(255, 0, 0, 0), Engine_Rect);//Border
	} else {
		if ( !max )
			return;

		if ( h < 5 )
			return;

		if ( Armor < 0 )
			Armor = 0;

		float ratio = Armor / max;

		ColorX col = ColorX(255, (uchar)( 255 - 255 * ratio ), 0, (uchar)( 255 * ratio ));//Color

		float step = ( h / max );
		float draw = ( step * Armor );

		FillRect(x, y, w, h, ColorX(80, 0, 0, 0), Engine_Rect);; //Background
		FillRect(x, y, w, draw, col, Engine_Rect);;//Armorhbar
		DrawBorder2(x - 1, y - 1, w + 1, h + 1, 1, ColorX(255, 0, 0, 0), Engine_Rect);//Border
	}
}

void D3D11Renderer::DrawDot(FLOAT X, FLOAT Y, FLOAT Width, FLOAT Height, ColorX dColor, bool Center, bool Engine_Rect) {
	if ( Center ) {
		FillRect(X - 1.0f - ( Width / 2 ), Y - 1.0f - ( Height / 2.0f ), Width + 2.0f, Height + 2.0f, ColorX(255, 0.0f, 0.0f, 0.0f), Engine_Rect);
		FillRect(X - ( Width / 2 ), Y - ( Height / 2 ), Width, Height, dColor, Engine_Rect);
	} else {
		FillRect(X - 1.0f, Y - 1.0f, Width + 2.0f, Height + 2.0f, ColorX(255, 0.0f, 0.0f, 0.0f), Engine_Rect);
		FillRect(X, Y, Width, Height, dColor, Engine_Rect);
	}
}

void D3D11Renderer::DrawCross(FLOAT X, FLOAT Y, FLOAT Width, FLOAT Height, ColorX dColor, bool Engine_Rect, bool Rainbow) {
	if ( Rainbow ) {
		FillRect(X - 20, Y, 40, 1, PURPLE, Engine_Rect);//Purple
		FillRect(X, Y - 20, 1, 40, PURPLE, Engine_Rect);

		FillRect(X - 17, Y, 34, 1, BLUE, Engine_Rect);//Blue
		FillRect(X, Y - 17, 1, 34, BLUE, Engine_Rect);

		FillRect(X - 14, Y, 28, 1, CYAN, Engine_Rect);//Cyan
		FillRect(X, Y - 14, 1, 28, CYAN, Engine_Rect);

		FillRect(X - 11, Y, 22, 1, GREEN, Engine_Rect);//Green
		FillRect(X, Y - 11, 1, 22, GREEN, Engine_Rect);

		FillRect(X - 9, Y, 18, 1, YELLOW, Engine_Rect);//Yellow
		FillRect(X, Y - 9, 1, 18, YELLOW, Engine_Rect);

		FillRect(X - 6, Y, 12, 1, ORANGE, Engine_Rect);//Orange
		FillRect(X, Y - 6, 1, 12, ORANGE, Engine_Rect);

		FillRect(X - 3, Y, 6, 1, RED, Engine_Rect);//Red
		FillRect(X, Y - 3, 1, 6, RED, Engine_Rect);
	} else {
		FillRect(X - Width, Y - 0.5f, ( Width * 2.0f ), 1.0f, dColor, Engine_Rect);
		FillRect(X - 0.5f, Y - Height, 1.0f, ( Height * 2.0f ), dColor, Engine_Rect);
	}
}

void D3D11Renderer::DrawCursor(FLOAT X, FLOAT Y, bool Engine_Rect) {
	FillRect(X, Y, 5.0f, 5.0f, ColorX(255, 255, 255, 255));
	FillRect(X + 5.0f, Y + 5.0f, 2.0f, 2.0f, ColorX(255, 255, 255, 255));
	FillRect(X + 7.0f, Y + 7.0f, 2.0f, 2.0f, ColorX(255, 255, 255, 255));
	FillRect(X + 9.0f, Y + 9.0f, 2.0f, 2.0f, ColorX(255, 255, 255, 255));

}


float D3D11Renderer::GetWidth() {
	D3D11_VIEWPORT vp;
	UINT nvp = 1;
	this->deviceContext->RSGetViewports(&nvp, &vp);
	return vp.Width;
}

float D3D11Renderer::GetHeight() {
	D3D11_VIEWPORT vp;
	UINT nvp = 1;
	this->deviceContext->RSGetViewports(&nvp, &vp);
	return vp.Height;
}
#pragma warning(disable:4996)


extern SpriteFont* m_font;
extern std::unique_ptr<SpriteBatch> DX11SpriteBatch;

void D3D11Renderer::BeginScene() {
	this->restoreState = false;
	if ( SUCCEEDED(this->stateSaver->saveCurrentState(this->deviceContext)) )
		this->restoreState = true;

	float blendFactor[] = {0.0f, 0.0f, 0.0f, 0.0f};
	this->deviceContext->OMSetBlendState(this->transparency, blendFactor, 0xffffffff);
	this->deviceContext->IASetInputLayout(this->mInputLayout);

	DX11SpriteBatch->Begin(SpriteSortMode_Deferred);
}

void D3D11Renderer::EndScene() {
	DX11SpriteBatch->End();
	if ( this->restoreState )
		this->stateSaver->restoreSavedState();
}

std::wstring ConvertToWStr(const std::string& s) {
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

#include "Tools.h"
void D3D11Renderer::Draw2dText(float x, float y, XMVECTORF32 Colortxt, float scale, bool Center, char *szText, ...) {
	va_list va_alist;
	char logbuf[32] = "";
	va_start(va_alist, szText);
	_vsnprintf(logbuf, sizeof(logbuf) - strlen(logbuf), szText, va_alist);
	va_end(va_alist);
	XMFLOAT2 origin(0, 0);
	XMFLOAT2 pos = {x, y};
	std::wstring text = ConvertToWStr(logbuf);

	if ( Center ) {
		XMVECTOR size = m_font->MeasureString(text.c_str());
		float sizeX = XMVectorGetX(size);
		float sizeY = XMVectorGetY(size);
		origin = XMFLOAT2(sizeX / 2, sizeY / 2);
	} else {
		XMVECTOR size = m_font->MeasureString(text.c_str());
		float sizeY = XMVectorGetY(size);
		origin = XMFLOAT2(0.0f, sizeY / 2);
	}
	m_font->DrawString(DX11SpriteBatch.get(), text.c_str(), pos, Colortxt, 0.f, origin, scale, SpriteEffects_None);
}

bool D3D11Renderer::Draw3DBox(Vector3 Position, Vector4 Rotation, ColorX BoxColor) {
	float rot = acos(Rotation.x) * 180 / D3DX_PI;
	if ( asin(Rotation.y) * 180 / D3DX_PI < 0 ) rot *= -1;

	Vector3 HeadPos = Vector3(Position.x, Position.y, Position.z + 0.852f);
	Vector3 FeetPos = Vector3(Position.x, Position.y, Position.z - 1.0f);

	Vector3 t[8] = {Vector3()};

	Vector4 Cosines;
	Vector4 Sines;
	Vector4 Corners = Vector4(XMConvertToRadians(rot - 45), XMConvertToRadians(rot - 135), XMConvertToRadians(rot + 45), XMConvertToRadians(rot + 135));
	Cosines.x = cos(Corners.x);
	Cosines.y = cos(Corners.y);
	Cosines.z = cos(Corners.z);
	Cosines.w = cos(Corners.w);
	Sines.x = sin(Corners.x);
	Sines.y = sin(Corners.y);
	Sines.z = sin(Corners.z);
	Sines.w = sin(Corners.w);

	float radius = 0.5f;

	Vector3 HeadLeftForward = HeadPos + Vector3(Cosines.x * radius, Sines.x * radius, 0.f);
	Vector3 HeadLeftBackward = HeadPos + Vector3(Cosines.y * radius, Sines.y * radius, 0.f);
	Vector3 HeadRightForward = HeadPos + Vector3(Cosines.z * radius, Sines.z * radius, 0.f);
	Vector3 HeadRightBackward = HeadPos + Vector3(Cosines.w * radius, Sines.w * radius, 0.f);

	Vector3 FeetLeftForward = FeetPos + Vector3(Cosines.x * radius, Sines.x * radius, 0.f);
	Vector3 FeetLeftBackward = FeetPos + Vector3(Cosines.y * radius, Sines.y * radius, 0.f);
	Vector3 FeetRightForward = FeetPos + Vector3(Cosines.z * radius, Sines.z * radius, 0.f);
	Vector3 FeetRightBackward = FeetPos + Vector3(Cosines.w * radius, Sines.w * radius, 0.f);


	if ( this->W2S(HeadLeftForward, t[0]) && this->W2S(HeadLeftBackward, t[1]) && this->W2S(HeadRightBackward, t[2]) && this->W2S(HeadRightForward, t[3]) ) {
		if ( this->W2S(FeetLeftForward, t[4]) && this->W2S(FeetLeftBackward, t[5]) && this->W2S(FeetRightBackward, t[6]) && this->W2S(FeetRightForward, t[7]) ) {
			for ( int i = 0; i < 4; i++ ) {
				renderer->DrawLine(t[i].x, t[i].y, t[i + 4].x, t[i + 4].y, BoxColor);

				if ( i != 3 ) renderer->DrawLine(t[i].x, t[i].y, t[i + 1].x, t[i + 1].y, BoxColor);
				else		renderer->DrawLine(t[3].x, t[3].y, t[0].x, t[0].y, BoxColor);
			}

			for ( int i = 4; i < 8; i++ ) {
				if ( i != 7 ) renderer->DrawLine(t[i].x, t[i].y, t[i + 1].x, t[i + 1].y, BoxColor);
				else		renderer->DrawLine(t[7].x, t[7].y, t[4].x, t[4].y, BoxColor);
			}
		}
	}
	return true;
}
