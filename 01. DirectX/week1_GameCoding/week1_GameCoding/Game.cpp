#include "pch.h"
#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_width = GWinSizeX;
	_height = GWinSizeY;

	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewport();
}

void Game::Update()
{
}

void Game::Render()
{
	// ���� �ܰ� : IA - VS - RS - PS - OM
	RenderBegin(); // �׸� �غ� (��ȭ�� �ʱ�ȭ ��)

	// >> �׸��� ����



	RenderEnd(); //�׸� ��
}

void Game::RenderBegin()
{
	// >> : ���� �� �ʱ�ȭ
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr); // OM : OutputMerger (������ �ܰ�)
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);
	_deviceContext->RSSetViewports(1, &_viewport);
}

void Game::RenderEnd()
{
	HRESULT hr = _swapChain->Present(1, 0); // [�߿�] �ϼ��� �� ���۸� ���� ���۷� ���
	CHECK(hr);
}

void Game::CreateDeviceAndSwapChain()
{
	// >> : ����ü�� (���� ���۸� ����)
	DXGI_SWAP_CHAIN_DESC desc; //�����(����ü��)�� ��� ���� ����
	ZeroMemory(&desc, sizeof(desc)); //memeset�� ������ ��� (0���� �ʱ�ȭ)
	{
		desc.BufferDesc.Width = _width;
		desc.BufferDesc.Height = _height;
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0; //��Ƽ ���ø��� ���� : �ȼ��� �ڿ������� ���Ҷ� (�缱�� ������� ��) ��� �Ұ���
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //���� ��� �뵵
		desc.BufferCount = 1; // ���� ��� ����
		desc.OutputWindow = _hwnd; // ������ �ڵ�
		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}
	// << :

	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE, //�׷��� ī�带 ���, ������� �ϴ°ſ� ���� ���� (CPU�� ���� ����Ұ���)
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		_swapChain.GetAddressOf(),// _device����Ʈ �����Ͷ� ��ü�� �������� .Get(), �ּҸ� �������� GetAddressOf()���
		_device.GetAddressOf(),
		nullptr,
		_deviceContext.GetAddressOf()
	);

	CHECK(hr); //���� ���� (define ����, assert ���)
	
}

void Game::CreateRenderTargetView()
{
	// >> : �ĸ� ���۸��� �׷��޶�� �Ÿ� �����ϴ� �Լ�
	HRESULT hr;

	ComPtr<ID3D11Texture2D> backBuffer = nullptr; //Texture : PNG ���� �����Ŷ�� ����
	hr = _swapChain->GetBuffer(0, _uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	CHECK(hr);

	_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	CHECK(hr);

	//�ĸ� ���۸� backBuffer�� �־��ְ� _renderTargetView�� �־��� ����(?)

}

void Game::SetViewport()
{
	_viewport.TopLeftX = 0.0f;
	_viewport.TopLeftY = 0.0f;
	_viewport.Width = static_cast<float>(_width);
	_viewport.Height = static_cast<float>(_height);
	_viewport.MinDepth = 0.0f;
	_viewport.MaxDepth = 1.0f;
}
