#include "Includes.h"
#include "time.h"

unsigned long Font = NULL;
DWORD dwGlowBase = NULL;
BYTE bSendPacket = NULL;
DWORD dwOrigCreateMove = NULL;
ScreenSize_t ScreenSize;

void __stdcall new_RunCommand(CEntity* pPlayer, CUserCmd* cmd, IMoveHelper* pMoveHelper)
{
	typedef void(__stdcall* RunCommand_t)(CEntity*, CUserCmd*, IMoveHelper*);
	Interface.PredictionVMT->Function<RunCommand_t>(19)(pPlayer, cmd, pMoveHelper);

	if (Interface.MoveHelper == NULL && pMoveHelper != NULL)
		Interface.MoveHelper = pMoveHelper;
}

void __declspec(naked) __fastcall hkCreateMove(void* thisptr, void* eax, int sequence_number, float input_sample_frametime, bool active)
{
	__asm
	{
		mov     bSendPacket, bl
		push    ebp
		mov     ebp, esp
		sub     esp, 8
		pusha
		push[ebp + 10h]
		push[ebp + 0Ch]
		push[ebp + 08h]
		call    dwOrigCreateMove
		push[ebp + 10h]
		push[ebp + 0Ch]
		push[ebp + 08h]
		call    new_CreateMove
		popa
		mov     bl, bSendPacket
		mov     esp, ebp
		pop     ebp
		retn    0Ch
	}
}

template<typename T> T FindInterface(std::string szModuleName, std::string szInterfaceName)
{
	HMODULE hMod = GetModuleHandleA(szModuleName.c_str());

	if (!hMod)
		return nullptr;

	CreateInterfaceFn CreateInterface = (CreateInterfaceFn)GetProcAddress(hMod, "CreateInterface");

	if (!CreateInterface)
		return nullptr;

	void* pReturn = nullptr;

	char* szInterface = new char[1024];

	for (int i = 0; i <= 999; ++i)
	{
		sprintf(szInterface, "%s%03i", szInterfaceName.c_str(), i);
		pReturn = CreateInterface(szInterface, NULL);

		if (pReturn)
		{
 
			Utilities::DebugPrint("Found Interface %s at 0x%x.", szInterface, (DWORD)pReturn);
 
			break;
		}
	}

	delete[] szInterface;

 
	if (pReturn == nullptr)
		Utilities::DebugPrint("Found no interface for %s.", szInterfaceName.c_str());
 

	return reinterpret_cast<T>(pReturn);
}

void Init(void)
{
	if (AllocConsole())
	{
		freopen("CONOUT$", "wt", stdout);
		SetConsoleTitleA("GOHook Internal");
		system("color C");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

	}

	for (HMODULE hEngine = NULL; hEngine == NULL; Sleep(100))
		hEngine = GetModuleHandleW(L"engine.dll");

	for (HMODULE hClient = NULL; hClient == NULL; Sleep(100))
		hClient = GetModuleHandleW(L"client_panorama.dll");

	Interface.BaseClient = FindInterface<IBaseClient*>("client_panorama.dll", "VClient");
	DWORD dwBaseClient = (DWORD)Interface.BaseClient;
	Interface.BaseClientVMT = new CVMTHook(&dwBaseClient);

	DWORD dwInputPointer = Utilities::FindPattern(Interface.BaseClientVMT->Function<DWORD>(22), 0x100, (BYTE*)"\x24\xB9", "xx");

	if (dwInputPointer != NULL)
	{
		dwInputPointer += 0x2;
		Interface.Input = *(CInput**)dwInputPointer;
	}

	Interface.EntityList = FindInterface<IClientEntityList*>("client_panorama.dll", "VClientEntityList");
	Interface.Prediction = FindInterface<CPrediction*>("client_panorama.dll", "VClientPrediction");
	Interface.GameMovement = FindInterface<CGameMovement*>("client_panorama.dll", "GameMovement");
	DWORD dwPrediction = (DWORD)Interface.Prediction;
	Interface.PredictionVMT = new CVMTHook(&dwPrediction);

	Interface.EngineClient = FindInterface<IVEngineClient*>("engine.dll", "VEngineClient");
	Interface.EngineTrace = FindInterface<IEngineTrace*>("engine.dll", "EngineTraceClient");
	Interface.ModelInfo = FindInterface<IVModelInfo*>("engine.dll", "VModelInfoClient");
	Interface.ModelRender = FindInterface<IVModelRender*>("engine.dll", "VEngineModel");
	Interface.RenderView = FindInterface<IVRenderView*>("engine.dll", "VEngineRenderView");
	DWORD dwModelRender = (DWORD)Interface.ModelRender;
	Interface.ModelRenderVMT = new CVMTHook(&dwModelRender);

	Interface.MaterialSystem = FindInterface<IMaterialSystem*>("MaterialSystem.dll", "VMaterialSystem");
	DWORD dwMaterialSystem = (DWORD)Interface.MaterialSystem;
	Interface.MaterialSystemVMT = new CVMTHook(&dwMaterialSystem);

	void* PhysicsSurfaceProps = FindInterface<void*>("vphysics.dll", "VPhysicsSurfaceProps");

	Interface.Surface = FindInterface<ISurface*>("vguimatsurface.dll", "VGUI_Surface");

	Interface.Panel = FindInterface<IPanel*>("vgui2.dll", "VGUI_Panel");
	DWORD dwPanelVMT = (DWORD)Interface.Panel;
	Interface.PanelVMT = new CVMTHook(&dwPanelVMT);

	void* Cvar = FindInterface<void*>("vstdlib.dll", "VEngineCvar");

	Interface.GlobalVars = (CGlobalVarsBase*)(**(DWORD**)(Utilities::FindPattern((DWORD)GetModuleHandleA("client_panorama.dll"), 0x7FFFFF, (BYTE*)"\xA1\x00\x00\x00\x00\x8B\x40\x10\x89\x41\x04", "x????xxxxxx") + 0x1));

	Utilities::DebugPrint("Found GlobalVars at 0x%x", (DWORD)Interface.GlobalVars);

	Interface.NetVars->Init();
	Interface.NetVars->GetOffsets();

	Font = Interface.Surface->CreateFont();
	Interface.Surface->SetFontGlyphSet(Font, "Comic Sans MS", 14, 0, 0, 0, FONTFLAG_OUTLINE);

	Interface.EngineClient->GetScreenSize(ScreenSize.Width, ScreenSize.Height);

	Interface.PredictionVMT->Hook((DWORD)new_RunCommand, 19);
	dwOrigCreateMove = Interface.BaseClientVMT->Function<DWORD>(22); //8/10/18 panorama 21->22
	Interface.BaseClientVMT->Hook((DWORD)hkCreateMove, 22); // 8/10/18 panorama 21->22
	//Interface.BaseClientVMT->Hook((DWORD)new_FrameStageNotify, 37); //don't need framestagenotify hook for anything atm
	Interface.PanelVMT->Hook((DWORD)new_PaintTraverse, 41); // painttraverse hook used for menu overlay, ESP etc

	ExitThread(1);
}

int __stdcall DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpvReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Init, 0, 0, 0);
		break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}

	return 1;
}