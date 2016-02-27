#include <iostream>
#include <string>
#include <Windows.h>
#include "Registry.h"

/// Swing Registry Struct
struct SwingRegistryKeys
{	
	std::wstring Drive = TEXT("Z:");												// Unused
	std::wstring Exe = TEXT("SWING95.EXE");											// Executable
	std::wstring Language = TEXT("1");												// 1 = German, 0 = English
	std::wstring Path = TEXT("G:\\Swing");											// Installation path
	std::wstring Uninstall = TEXT("C:\\WINDOWS\\ISUN0407.EXE -f");					// Uninstall Path
	LPCWSTR KeyPathLocation = TEXT("SOFTWARE\\Software2000\\Swing\\Data");			// Key Path
	std::wstring Version = TEXT("1.20");											// Swing Version
} keys;

/// Prueft die Registry auf Fehlende Schluessel
void CheckRegistry()
{
	// Wichtig: Wieder loeschen
	SwingRegistryKeys *keys = new SwingRegistryKeys();

	std::cout << "Ueberpruefe Registry auf Fehler..." << std::endl;

	// Registry Ueberpruefen
	HKEY hKey;

	// Schluessel erhalten
	auto e = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Software2000\\Swing\\Data", 0, 0x20019u, &hKey);

	// Nicht vorhanden?
	if(e != ERROR_SUCCESS)
	{
		// Meldung
		std::cout << "Fehlercode: " << e << ", Registrys sind anscheinend nicht vorhanden. Sollen die Schluessel nun erstellt werden? y/n" << std::endl;
		//RegQueryValueExA(hKey, "Path", 0, nullptr, Data, &DataBuffer);

		// Y / N Erfassen
		char in = std::getchar();

		// y?
		if (in == 'y')
		{
			RegCloseKey(hKey);

			HKEY hNewKey;			// Der Hauptschlusesel
			DWORD dwDisposition;	// Disposition

			std::cout << "Erstelle Schluessel... (" << keys->KeyPathLocation << ")" << std::endl;

			// Hauptschluessel erstellen
			auto regCreateResult = RegCreateKeyEx(HKEY_LOCAL_MACHINE, keys->KeyPathLocation, 0, NULL, 0, KEY_WRITE, NULL, &hNewKey, &dwDisposition);

			// Alle Keys setzen (Oben Definiert)
			// Wichtig hier: Key wird mit UNICODE eingefuegt. Deshalb wird einerseits ein Wide String benoetigt und mit der Groesse von dem Widechar multipliziert.
			// Alle Keys siehe oben
			RegSetValueEx(hNewKey, L"Drive", NULL, REG_SZ, (const BYTE*)keys->Drive.c_str(), (keys->Drive.size() + 1) * sizeof(wchar_t));
			RegSetValueEx(hNewKey, L"Exe", NULL, REG_SZ, (const BYTE*) keys->Exe.c_str(), (keys->Exe.size() + 1) * sizeof(wchar_t));
			RegSetValueEx(hNewKey, L"Language", NULL, REG_SZ, (const BYTE*)keys->Language.c_str(), (keys->Language.size() + 1) * sizeof(wchar_t));
			RegSetValueEx(hNewKey, L"Path", NULL, REG_SZ, (const BYTE*)keys->Path.c_str(), (keys->Path.size() + 1) * sizeof(wchar_t));
			RegSetValueEx(hNewKey, L"Uninstall", NULL, REG_SZ, (const BYTE*)keys->Uninstall.c_str(), (keys->Uninstall.size() + 1) * sizeof(wchar_t));
			RegSetValueEx(hNewKey, L"Version", NULL, REG_SZ, (const BYTE*)keys->Version.c_str(), (keys->Version.size() + 1) * sizeof(wchar_t));

			//std::cout << regCreateError;
			std::cout << "Registry OK -> Falls der Fehler erneut auftritt, dieses Programm bitte als Administrator ausfuehren" << std::endl;
		}
		else
		{
			// Selber machen
			std::cout << "Schluessel nicht erstellt. Dann bitte selber einfuegen auf " << keys->KeyPathLocation << "!" << std::endl;
		}
	}
	// Andererseits
	else
	{
		// Schon vorhanden
		std::cout << "Registry vorhanden, ueberspringe." << std::endl;
	}

	// Speicher Freigeben
	delete keys;
	
	// Auf weitere Eingabe warten
	// std::cin.get();
}