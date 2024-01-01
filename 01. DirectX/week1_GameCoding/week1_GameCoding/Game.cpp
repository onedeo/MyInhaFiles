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
	// 렌더 단계 : IA - VS - RS - PS - OM
	RenderBegin(); // 그릴 준비 (도화지 초기화 등)

	// >> 그리기 로직



	RenderEnd(); //그림 끝
}

void Game::RenderBegin()
{
	// >> : 버퍼 색 초기화
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr); // OM : OutputMerger (마지막 단계)
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);
	_deviceContext->RSSetViewports(1, &_viewport);
}

void Game::RenderEnd()
{
	HRESULT hr = _swapChain->Present(1, 0); // [중요] 완성된 백 버퍼를 전면 버퍼로 출력
	CHECK(hr);
}

void Game::CreateDeviceAndSwapChain()
{
	// >> : 스왑체인 (더블 버퍼링 세팅)
	DXGI_SWAP_CHAIN_DESC desc; //백버퍼(스왑체인)를 담는 버퍼 관리
	ZeroMemory(&desc, sizeof(desc)); //memeset과 동일한 기능 (0으로 초기화)
	{
		desc.BufferDesc.Width = _width;
		desc.BufferDesc.Height = _height;
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0; //멀티 샘플링과 관련 : 픽셀이 자연스럽지 못할때 (사선이 계단현상 등) 어떻게 할건지
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //버퍼 사용 용도
		desc.BufferCount = 1; // 버퍼 사용 개수
		desc.OutputWindow = _hwnd; // 윈도우 핸들
		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}
	// << :

	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE, //그래픽 카드를 사용, 만들고자 하는거에 따라 변경 (CPU도 같이 사용할건지)
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		_swapChain.GetAddressOf(),// _device스마트 포인터라서 자체를 받으려면 .Get(), 주소를 받으려면 GetAddressOf()사용
		_device.GetAddressOf(),
		nullptr,
		_deviceContext.GetAddressOf()
	);

	CHECK(hr); //성공 여부 (define 참고, assert 사용)
	
}

void Game::CreateRenderTargetView()
{
	// >> : 후면 버퍼링에 그려달라는 거를 설정하는 함수
	HRESULT hr;

	ComPtr<ID3D11Texture2D> backBuffer = nullptr; //Texture : PNG 파일 같은거라고 생각
	hr = _swapChain->GetBuffer(0, _uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	CHECK(hr);

	_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	CHECK(hr);

	//후면 버퍼를 backBuffer에 넣어주고 _renderTargetView에 넣어준 개념(?)

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
