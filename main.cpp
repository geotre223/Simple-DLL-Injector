#include <Windows.h>
#include <iostream>
#include <string>

int main()
{
    std::string DLLpath;
    DWORD PID;
	HANDLE hProcess;

	std::cout << "Please write in the DLL path : ";
	std::cin >> DLLpath;
	std::cout << "\nAnd the PID : ";
	std::cin >> PID;

	hProcess = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD, FALSE, PID);
	if (!hProcess)
	{
		std::cout << "\nERROR: Couldn't open desired process.";
		return 1;
	}

	void* p = VirtualAllocEx(hProcess, nullptr, DLLpath.size() + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!p)
    {
        std::cout << "\nERROR: Couldn't allocate memory.";
        return 1;
    }

    if (!WriteProcessMemory(hProcess, p, DLLpath.c_str(), DLLpath.size() + 1, nullptr))
    {
        std::cout << "\nERROR: Couldn't write process memory.";
        return 1;
    }

	HANDLE hThread = CreateRemoteThread(hProcess, nullptr, 0,
		(LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA"),
		p, 0, nullptr);

	CloseHandle(hThread);
	CloseHandle(hProcess);

	return 0;
}