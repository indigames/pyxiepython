#include "pyxie.h"
#include "Window.h"
#include <python.h>
#include <thread>
#include <mutex>

#include "pyxieApplication.h"
#include "pyxieFios.h"
#include "pyxieTouchManager.h"
#include "pyxieResourceManager.h"
#include "pyxieRenderContext.h"
#include "Backyard.h"

class TestApp : public pyxie::pyxieApplication
{
public:
	TestApp() {
		
		pyxie::pyxieFios::Instance().SetRoot();
	}
	~TestApp() {}

	bool onInit(DeviceHandle dh) {
		if (pyxieApplication::onInit(dh) == false) return false;
		pyxie::Backyard::New();
		return true;
	}
	void onShutdown() {
		pyxie::Backyard::Delete();
		pyxieApplication::onShutdown();
	}
	bool onUpdate(){
		pyxie::Backyard::Instance().SyncMain();
		if (pyxieApplication::onUpdate() == false) return false;
		return true;
	}
	void onRender(){
		//pyxie::Backyard::Instance().Render();
	}
};
TestApp app;

HINSTANCE hInstance;
DeviceHandle deviceHandle;
HWND hWndOrg;

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

std::thread* mainWindow_th;
std::mutex  mainWindow_mtx;
std::condition_variable mainWindow_cv;

bool changeWindow = false;
int windowMode = 0;
int Cx = 480;
int Cy = 640;

void pyxieShowWindow(bool show, int cx, int cy) {
	changeWindow = true;
	if (show) windowMode = SW_SHOW;
	else windowMode = SW_HIDE;
	Cx = cx;
	Cy = cy;
}


void CreateApprication() {
	mainWindow_th = new std::thread(CreateMyWindow);
	std::unique_lock<std::mutex>  mainWindow_lk(mainWindow_mtx);
	mainWindow_cv.wait(mainWindow_lk);
}

void DeleteApplication()
{
	::PostMessage(hWndOrg, WM_CLOSE, 0, 0);
}

void  CreateMyWindow(){

	hInstance = GetModuleHandle(nullptr);

	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = 0;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	//wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TESTAPP);
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = L"pyxie_class";
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	ATOM atom = RegisterClassExW(&wcex);
	if (!atom) return;


	hWndOrg = CreateWindowExW(
		0,									//_In_ DWORD dwExStyle,
		L"pyxie_class",						//_In_opt_ LPCWSTR lpClassName,
		L"pyxie",							//_In_opt_ LPCWSTR lpWindowName,
		WS_OVERLAPPED | WS_THICKFRAME,		//_In_ DWORD dwStyle,
		CW_USEDEFAULT,						//_In_ int X,
		0,									//_In_ int Y,
		CW_USEDEFAULT,						//_In_ int nWidth,
		0,									//_In_ int nHeight,
		nullptr,							//_In_opt_ HWND hWndParent,
		nullptr,							//_In_opt_ HMENU hMenu,
		hInstance,							//_In_opt_ HINSTANCE hInstance,
		0);									//_In_opt_ LPVOID lpParam);
	if (!hWndOrg) return;

	ShowWindow(hWndOrg, SW_HIDE);
	UpdateWindow(hWndOrg);



	MSG msg;
	for (;;) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			if (GetMessage(&msg, NULL, 0, 0) > 0) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
				break;
		}
		else{
			if (changeWindow) {
				RECT rw, rc;
				::GetWindowRect(hWndOrg, &rw);
				::GetClientRect(hWndOrg, &rc);
				int new_width = (rw.right - rw.left) - (rc.right - rc.left) + Cx;
				int new_height = (rw.bottom - rw.top) - (rc.bottom - rc.top) + Cy;
				SetWindowPos(hWndOrg, HWND_TOP, 0, 0, new_width, new_height, SWP_NOMOVE);
				ShowWindow(hWndOrg, windowMode);
				UpdateWindow(hWndOrg);
				//CreateContext(GetDC(hWndOrg));
				changeWindow = false;
			}
			if (!app.onUpdate()) {
				SendMessage(hWndOrg, WM_DESTROY, 0, 0);
			}
			else {
			//	app.onRender();
				SwapBuffers(deviceHandle);
			}
		}
	}
}

void Sync()
{
}

void Finalize()
{
	::PostMessage(hWndOrg, WM_CLOSE, 0, 0);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	unsigned short int x = -10, y = -10;

	switch (message)
	{
	case WM_CREATE:
		deviceHandle = GetDC(hWnd);
		app.onInit(deviceHandle);
		mainWindow_cv.notify_one();
		break;
	case WM_SIZE:
		app.onSize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_DESTROY:
		app.onShutdown();
		PostQuitMessage(0);
		break;

	case WM_LBUTTONDOWN:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		pyxie::pyxieTouchManager::Instance().UpdateTouches(0, x, y, 1);
		break;

	case WM_LBUTTONUP:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		pyxie::pyxieTouchManager::Instance().UpdateTouches(1, x, y, 1);
		break;

	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON) {
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			pyxie::pyxieTouchManager::Instance().UpdateTouches(0, x, y, 1);
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason) {

	case DLL_PROCESS_ATTACH: // DLLがプロセスのアドレス空間にマッピングされた。
		break;

	case DLL_THREAD_ATTACH: // スレッドが作成されようとしている。
		break;

	case DLL_THREAD_DETACH: // スレッドが破棄されようとしている。
		break;

	case DLL_PROCESS_DETACH: // DLLのマッピングが解除されようとしている。
		break;

	}
	return TRUE;
}

PyMODINIT_FUNC _PyInit_pyxie();

PyMODINIT_FUNC PyInit_pyxie() {
	CreateApprication();
	return _PyInit_pyxie();
}

