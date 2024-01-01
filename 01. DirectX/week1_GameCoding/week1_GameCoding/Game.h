#pragma once
class Game
{
public:
	Game();
	~Game();

public:
	void Init(HWND hwnd);
	void Update();
	void Render();

private:
	void RenderBegin();
	void RenderEnd();

private:
	HWND _hwnd;
	uint32 _width = 0;
	uint32 _height = 0;

private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void SetViewport();

private:
	//DX : 전체적인 틀(함수)를 이해하자

	// >> : device
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _deviceContext;
	/*
	- I : Interface로 사용하는 것
	- Delete 하는 것이 아니라 Release를 해야한다
	AddRef 사용 -> 다른 곳에서 사용하면 AddRef가 1 증가하고, Release하면 1 감소
	AddRef랑 Release짝이 안맞으면? -> 메모리 누수 -> 수동으로 하는 것은 바람직하지 않음
	=> 일일이 관리하는 것이 아니고 스마트 포인터로 관리 -> ComPtr<> 사용 (wrl.h 헤더를 통해 사용)
	- 컴인터페이스 무엇을 하는 것인가
	다양한 GPU는 제조사, 모델을 통일화 하는 규격으로 다이렉트x에서 지정된 것만 집중할 수 있는 '인터페이스'

	- _device : 리소스 생성
	- _deviceContext : 리소스 사용
	*/
	ComPtr<IDXGISwapChain> _swapChain = nullptr;
	/*
	- 더블버퍼링 : 그리는것과 화면에 보이는 것과 동시에 진행되면 끊겨서 보이는거 해결
		여러 프레임에 정보가 뒤석이는걸 방지
	- 스왑체인도 더블버퍼링과 유사한 개념으로 최종 결과물 렌더링과 연관
	*/
	// << :

	// >> : Render Target View
	ComPtr<ID3D11RenderTargetView> _renderTargetView;
	// << :

	// >> : Misc
	D3D11_VIEWPORT _viewport = { 0 }; //화면의 크기를 묘사해주는 자료
	float _clearColor[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
	// <<
};

