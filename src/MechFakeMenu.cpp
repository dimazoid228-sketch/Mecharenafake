#include <windows.h>

static HWND g_menu = nullptr;
static bool g_visible = true;
static bool g_esp = false, g_ammo = false, g_super = false;

static void Toggle(int id, bool& v, const char* name) {
    v = !v;
    char s[128];
    wsprintfA(s, "[ %s ]  %s", v ? "ON" : "OFF", name);
    SetWindowTextA(GetDlgItem(g_menu, id), s);
}

LRESULT CALLBACK Proc(HWND h, UINT m, WPARAM w, LPARAM l) {
    if (m == WM_COMMAND) {
        if (LOWORD(w) == 1001) Toggle(1001, g_esp, "ESP");
        if (LOWORD(w) == 1002) Toggle(1002, g_ammo, "Патроны");
        if (LOWORD(w) == 1003) Toggle(1003, g_super, "Беск. супер");
        return 0;
    }
    if (m == WM_CLOSE) { ShowWindow(h, SW_HIDE); g_visible = false; return 0; }
    return DefWindowProcA(h,m,w,l);
}

DWORD WINAPI Thread(LPVOID) {
    WNDCLASSA wc{};
    wc.lpfnWndProc = Proc;
    wc.hInstance = GetModuleHandleA(nullptr);
    wc.lpszClassName = "MechFakeMenuClass";
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    RegisterClassA(&wc);

    g_menu = CreateWindowExA(
        WS_EX_TOPMOST, "MechFakeMenuClass", "MECH FAKE MENU",
        WS_POPUP | WS_CAPTION | WS_SYSMENU,
        250,180,360,300,nullptr,nullptr,wc.hInstance,nullptr);

    CreateWindowA("STATIC","MECH FAKE MENU",WS_VISIBLE|WS_CHILD,
        20,15,300,30,g_menu,nullptr,wc.hInstance,nullptr);
    CreateWindowA("BUTTON","[ OFF ]  ESP",WS_VISIBLE|WS_CHILD,
        20,60,300,40,g_menu,(HMENU)1001,wc.hInstance,nullptr);
    CreateWindowA("BUTTON","[ OFF ]  Патроны",WS_VISIBLE|WS_CHILD,
        20,110,300,40,g_menu,(HMENU)1002,wc.hInstance,nullptr);
    CreateWindowA("BUTTON","[ OFF ]  Беск. супер",WS_VISIBLE|WS_CHILD,
        20,160,300,40,g_menu,(HMENU)1003,wc.hInstance,nullptr);
    CreateWindowA("STATIC","DEMO / EDUCATIONAL MENU",WS_VISIBLE|WS_CHILD,
        20,220,300,25,g_menu,nullptr,wc.hInstance,nullptr);

    ShowWindow(g_menu, SW_SHOW);

    MSG msg{};
    while (GetMessageA(&msg,nullptr,0,0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
        if (GetAsyncKeyState(VK_INSERT) & 1) {
            g_visible = !g_visible;
            ShowWindow(g_menu, g_visible ? SW_SHOW : SW_HIDE);
        }
    }
    return 0;
}

BOOL APIENTRY DllMain(HMODULE h, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(h);
        CreateThread(nullptr,0,Thread,nullptr,0,nullptr);
    }
    return TRUE;
}
