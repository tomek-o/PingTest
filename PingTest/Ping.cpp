//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Ping.h"
#include "Log.h"
#include <winsock2.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)


namespace
{

typedef struct tagIPINFO
{
	unsigned char Ttl; 			// Time To Live
	unsigned char Tos; 			// Type Of Service
	unsigned char IPFlags; 		// IP flags
	unsigned char OptSize; 		// Size of options data
	unsigned char FAR *Options; // Options data buffer
}IPINFO, *PIPINFO;

typedef struct tagICMPECHO
{
	unsigned long Source; 		// Source address
	unsigned long Status; 		// IP status
	unsigned long RTTime; 		// Round trip time in milliseconds
	unsigned short DataSize; 	// Reply data size
	unsigned short Reserved; 	// Unknown
	void FAR *pData; 			// Reply data buffer
	IPINFO ipInfo; 				// Reply options
}ICMPECHO, *PICMPECHO;


// ICMP.DLL Export Function Pointers
HANDLE (WINAPI *pIcmpCreateFile)(VOID) = NULL;
BOOL (WINAPI *pIcmpCloseHandle)(HANDLE) = NULL;
DWORD (WINAPI *pIcmpSendEcho)(HANDLE,DWORD,LPVOID,WORD,PIPINFO,LPVOID,DWORD,DWORD) = NULL;

HANDLE hndlIcmp = NULL; 			// LoadLibrary() handle to ICMP.DLL
HANDLE hndlFile = NULL; 			// Handle for IcmpCreateFile()

}


int Ping::init(void)
{
	WSADATA wsaData; 			// WSADATA

	hndlIcmp = LoadLibrary("ICMP.DLL");
	if (hndlIcmp == NULL)
	{
		ShowMessage("Error loading ICMP.DLL");
		return -1;
	}
	// Retrieve ICMP function pointers
	pIcmpCreateFile = (HANDLE (WINAPI *)(void))	GetProcAddress(hndlIcmp,"IcmpCreateFile");
	pIcmpCloseHandle = (BOOL (WINAPI *)(HANDLE)) GetProcAddress(hndlIcmp,"IcmpCloseHandle");
	pIcmpSendEcho = (DWORD (WINAPI *)(HANDLE,DWORD,LPVOID,WORD,PIPINFO,LPVOID,DWORD,DWORD))	GetProcAddress(hndlIcmp,"IcmpSendEcho");
	// Check all the function pointers
	if (pIcmpCreateFile == NULL || pIcmpCloseHandle == NULL || pIcmpSendEcho == NULL)
	{
		ShowMessage("Error getting ICMP function address");
		FreeLibrary(hndlIcmp);
		return -1;
	}

	// Init WinSock
	int nRet = WSAStartup(0x0101, &wsaData );
	if (nRet)
	{
		ShowMessage("WSAStartup() error");
		WSACleanup();
		FreeLibrary(hndlIcmp);
		return -1;
	}
	// Check WinSock version
	if (wsaData.wVersion != 0x0101)
	{
		ShowMessage("WinSock v 1.1 not supported");
		WSACleanup();
		FreeLibrary(hndlIcmp);
		return -1;
	}

	hndlFile = pIcmpCreateFile();
	if (hndlFile == NULL)
	{
		ShowMessage("Failed to create pIcmpFile");
		return -1;
	}

	return 0;
}

void Ping::shutdown(void)
{
	if (hndlFile)
	{
		// Close the echo request file handle
		pIcmpCloseHandle(hndlFile);
		hndlFile = NULL;
		FreeLibrary(hndlIcmp);
		hndlIcmp = NULL;
		WSACleanup();
	}
}

int Ping::run(const char* ip, unsigned int requestSize)
{
	struct in_addr iaDest; 		// Internet address structure
	IPINFO ipInfo; 				// IP Options structure
	DWORD dwRet; 				// DWORD return code
	iaDest.s_addr = inet_addr(ip);
	if (iaDest.s_addr == INADDR_NONE)
	{
		return -1;
	}

	unsigned char icmpEchoBuffer[2048];
	struct tagICMPECHO *icmpEcho = (struct tagICMPECHO*)icmpEchoBuffer;

	// Set default values
	ipInfo.Ttl = 255;
	ipInfo.Tos = 0;
	ipInfo.IPFlags = 0;
	ipInfo.OptSize = 0;
	ipInfo.Options = NULL;

#if 0
	unsigned char requestBuffer[/* 1024 */ 30];
#else
	unsigned char requestBuffer[1024];
#endif
	if (requestSize > sizeof(requestBuffer))
		requestSize = sizeof(requestBuffer);
	for (unsigned int i=0; i<requestSize; i++)
	{
		requestBuffer[i] = static_cast<unsigned char>(rand());
	}

	// Reqest an ICMP echo
	dwRet = pIcmpSendEcho(
		hndlFile, // Handle from IcmpCreateFile()
		(DWORD)iaDest.s_addr, // Destination IP address
	#if 0
		NULL, // Pointer to buffer to send
		0, // Size of buffer in bytes
	#else
		(void*)requestBuffer,
		static_cast<WORD>(requestSize),
	#endif
		&ipInfo, // Request options
		&icmpEchoBuffer, // Reply buffer
		(DWORD)sizeof(icmpEchoBuffer),		                                             
		(DWORD)1000 // Time to wait in milliseconds
	);

	if (dwRet)
	{
		// Print the results
		unsigned char* replyData = icmpEchoBuffer + sizeof(*icmpEcho);
		if (memcmp(replyData, requestBuffer, requestSize))
		{
			LOG("PING error: echoed data mismatch");
        	return -2;
		}
		iaDest.s_addr = icmpEcho->Source;
	#if 0
		if (icmpEcho->RTTime > 0)
			txt.sprintf("Reply: Time=%ldms TTL=%d", icmpEcho->RTTime, icmpEcho->ipInfo.Ttl);
		else
			txt.sprintf("Reply: Time<1ms TTL=%d", icmpEcho->ipInfo.Ttl);
	#endif
		return 0;
	} else {
		//txt.sprintf("Error: icmpEcho.Status=%ld", icmpEcho->Status);
		LOG("PING Error: icmpEcho.Status=%ld (requestSize = %u)", icmpEcho->Status, requestSize);
		return -1;
	}
	
}



