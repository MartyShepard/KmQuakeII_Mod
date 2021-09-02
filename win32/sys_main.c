/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

// sys_main.c

#include <windows.h>
#include "../qcommon/qcommon.h"
#include <SDL2/SDL_main.h>

#ifdef _WIN32
	#include "winquake.h"
#endif

int main(int argc, char **argv)
{
#ifdef _WIN32
	// Setup DPI awareness
	Sys_SetHighDPIMode();

	// crappy argument parser can't parse.
	for (int i = 0; i < argc; i++)
	{

		// Borderless?
		if (strcmp(argv[i], "-borderless") == 0)
		{
			borderless = true;
		}
		// Inject a custom data dir.
		// Weiteres Verzeichnis als Suchpfad Hinzufügen
		if (strcmp(argv[i], "-datadir") == 0)
		{
			// Mkay, did the user give us an argument?
			if (i != (argc - 1))
			{
				DWORD attrib;
				WCHAR wpath[MAX_OSPATH];

				MultiByteToWideChar(CP_UTF8, 0, argv[i + 1], -1, wpath, MAX_OSPATH);
				attrib = GetFileAttributesW(wpath);

				if (attrib != INVALID_FILE_ATTRIBUTES)
				{
					if (!(attrib & FILE_ATTRIBUTE_DIRECTORY))
					{
						printf("-datadir %s is not a directory\n", argv[i + 1]);
						return 1;
					}

					Q_strncpyz(datadir, argv[i + 1], MAX_OSPATH);
				}
				else
				{
					printf("-datadir %s could not be found\n", argv[i + 1]);
					return 1;
				}
			}
			else
			{
				printf("-datadir needs an argument\n");
				return 1;
			}
		}

		// Inject a custom data dir.
		if (strcmp(argv[i], "-addondir") == 0)
		{
			// Mkay, did the user give us an argument?
			if (i != (argc - 1))
			{
				DWORD attrib;
				WCHAR wpath[MAX_OSPATH];

				MultiByteToWideChar(CP_UTF8, 0, argv[i + 1], -1, wpath, MAX_OSPATH);
				attrib = GetFileAttributesW(wpath);

				if (attrib != INVALID_FILE_ATTRIBUTES)
				{
					if (!(attrib & FILE_ATTRIBUTE_DIRECTORY))
					{
						printf("-addondir %s is not a directory\n", argv[i + 1]);
						return 1;
					}

					Q_strncpyz(addondir, argv[i + 1], MAX_OSPATH);
				}
				else
				{
					printf("-addondir %s could not be found\n", argv[i + 1]);
					return 1;
				}
			}
			else
			{
				printf("-addondir needs an argument\n");
				return 1;
			}
		}

	}
	// Init console window
	Sys_InitDedConsole();
#endif

	Com_Printf("%s %4.2f %s %s %s\n", ENGINE_NAME, VERSION, CPUSTRING, BUILDSTRING, __DATE__); //mxd. ENGINE_NAME, Version

	// Init engine subsystems
	Qcommon_Init(argc, argv);
	int oldtime = Sys_Milliseconds();

	// Main window message loop
	int time, newtime;
	while (true)
	{
		// If at a full screen console, don't update unless needed
		if (minimized || (dedicated && dedicated->integer))
			Sys_Sleep(1); //mxd. Was Sleep(1);

		// DarkOne's CPU usage fix
		while (true)
		{
			newtime = Sys_Milliseconds();
			time = newtime - oldtime;
			if (time > 0) break;
			Sys_Sleep(0); //mxd. Was Sleep(0); // may also use Sleep(1); to free more CPU, but it can lower your fps
		}

		Qcommon_Frame(time);
		oldtime = newtime;
	}
}