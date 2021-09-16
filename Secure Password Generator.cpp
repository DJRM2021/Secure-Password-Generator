/* Secure Password Generator ver.2.0
 * This program generates passwords.
 * Programmer: DJRM2021
 * Build 2.0.0.0
 * Target Architecture: x86 X64
 * Compiled using Visual Studio 2010
 * Last Modified: June 19, 2011
 */
#include "resource.h" // The original "About" dialog box is lost together with the old Visual Studio Solution file.
#include <Windows.h>
#include <WindowsX.h>
#include <string>
#include <fstream>
#include <time.h>
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"") // for visual style

LRESULT CALLBACK WindowProc (HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK AboutDialogProc (HWND, UINT, WPARAM, LPARAM);

void PasswordGen (int);
void PrintPassword (void);
void PrintPassword10 (void);
void Maintain (void);
void Clear (void);

HWND hTxtBx;
HWND hGrpBx;
HWND hRadBtn1;
HWND hRadBtn2;
HWND hRadBtn3;
HWND hRadBtn4;
HWND hRadBtn5;
HWND hBtn;
HWND hStat2;
HWND hChkBx;
HWND hBtnClr;
HINSTANCE hInstance;
std::wstring password;
wchar_t text[100];
wchar_t pass[1000];

const wchar_t VeryWeakChars [] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
							  'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

const wchar_t WeakChars [] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
							  'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1',
					          '2', '3', '4', '5', '6', '7', '8', '9'};

const wchar_t GoodChars [] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
						      'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
						      'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '1',
					          '2', '3', '4', '5', '6', '7', '8', '9'};

const wchar_t StrongChars [] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
							    'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
							    'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1',
							    '2', '3', '4', '5', '6', '7', '8', '9', '`', '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '-', '='};

const wchar_t VeryStrongChars [] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
							        'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
							        'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1',
							        '2', '3', '4', '5', '6', '7', '8', '9', '`', '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '-', '=', '{', '}', '[', ']', '\\', '|', '<', '>', ',', '.', '?', '/'};

HFONT hFont1 = CreateFont (16, 0, 0, 0, 500, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, TEXT("Arial"));

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdline, int nShowCmd)
{
	UNREFERENCED_PARAMETER (hPrevInstance);
	UNREFERENCED_PARAMETER (lpCmdline);
	HWND hWnd;
	WNDCLASSEX wcx;
	ZeroMemory (&wcx, sizeof (WNDCLASSEX));
	wcx.cbSize = sizeof (wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WindowProc;
	wcx.hInstance = hInstance;
	wcx.hCursor = LoadCursor (NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH) COLOR_WINDOW;
	wcx.lpszMenuName = MAKEINTRESOURCE (IDR_MENU1);
	wcx.hIcon = LoadIcon (hInstance, MAKEINTRESOURCE (IDI_ICON1));
	wcx.hIconSm = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcx.lpszClassName = L"WindowClass1";
	
	RegisterClassEx (&wcx);

	hWnd = CreateWindowEx (NULL, L"WindowClass1", L"Secure Password Generator ver.2.0", WS_SYSMENU | WS_MINIMIZEBOX, GetSystemMetrics (SM_CXSCREEN) / 2 - 285, GetSystemMetrics (SM_CYSCREEN) / 2 - 250, 570, 500, NULL, NULL, hInstance, NULL);
	SendMessage (hTxtBx, WM_SETFONT, (WPARAM) hFont1, TRUE);
	SendMessage (hRadBtn1, WM_SETFONT, (WPARAM) hFont1, TRUE);
	SendMessage (hRadBtn2, WM_SETFONT, (WPARAM) hFont1, TRUE);
	SendMessage (hRadBtn3, WM_SETFONT, (WPARAM) hFont1, TRUE);
	SendMessage (hRadBtn4, WM_SETFONT, (WPARAM) hFont1, TRUE);
	SendMessage (hGrpBx, WM_SETFONT, (WPARAM) hFont1, TRUE);
	SendMessage (hBtn, WM_SETFONT , (WPARAM) hFont1, TRUE);
	SendMessage (hStat2, WM_SETFONT , (WPARAM) hFont1, TRUE);
	SendMessage (hChkBx, WM_SETFONT , (WPARAM) hFont1, TRUE);
	SendMessage (hBtnClr, WM_SETFONT , (WPARAM) hFont1, TRUE);
	SendMessage (hRadBtn5, WM_SETFONT , (WPARAM) hFont1, TRUE);
	SendMessage (hChkBx, BM_SETCHECK , BST_CHECKED, 0);
	ShowWindow (hWnd, SW_SHOW);
	UpdateWindow (hWnd);
	MSG msg;

	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	} 
	return (int) msg.wParam;
}
LRESULT CALLBACK WindowProc (HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
		case WM_DESTROY:
		{
			PostQuitMessage (0);
			return 0;
		} break;

		case WM_CREATE:
			{
				hGrpBx = CreateWindowEx (NULL, L"BUTTON", L"Choose password strength", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 10, 10, 545, 50, hWnd, NULL, hInstance, NULL);
				hRadBtn1 = CreateWindowEx (NULL, L"BUTTON", L"Weak", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_TABSTOP, 155, 30, 75, 25, hWnd, (HMENU) 2, hInstance, NULL);
				hRadBtn2 = CreateWindowEx (NULL, L"BUTTON", L"Good", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON  | WS_TABSTOP, 240, 30, 75, 25, hWnd, (HMENU) 3, hInstance, NULL);
				hRadBtn3 = CreateWindowEx (NULL, L"BUTTON", L"Strong", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON  | WS_TABSTOP, 330, 30, 75, 25, hWnd, (HMENU) 4, hInstance, NULL);
				hRadBtn4 = CreateWindowEx (NULL, L"BUTTON", L"Very Strong", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON  | WS_TABSTOP, 425, 30, 100, 25, hWnd, (HMENU) 5, hInstance, NULL); 
				hRadBtn5 = CreateWindowEx (NULL, L"BUTTON", L"Very Weak", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON  | WS_TABSTOP, 50, 30, 85, 25, hWnd, (HMENU) 8, hInstance, NULL); 
				hChkBx = CreateWindowEx (NULL, L"BUTTON", L"Generate 10 passwords (Default)", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 10, 420, 250, 25, hWnd, (HMENU) 6, hInstance, NULL); 
				hStat2 = CreateWindowEx (NULL, L"Static", L"Password:", WS_VISIBLE | WS_CHILD, 10, 70, 100, 25, hWnd, NULL, hInstance, NULL);
				hTxtBx = CreateWindowEx (WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_VISIBLE | WS_CHILD | ES_LEFT | ES_AUTOHSCROLL | WS_TABSTOP | ES_MULTILINE | ES_READONLY, 10, 90, 543, 320, hWnd, NULL, hInstance, NULL);
				hBtn = CreateWindowEx (NULL, L"BUTTON", L"Generate", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 400, 420, 75, 25, hWnd, (HMENU) 1, hInstance, NULL);
				hBtnClr = CreateWindowEx (NULL, L"BUTTON", L"Clear", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 480, 420, 75, 25, hWnd, (HMENU) 7, hInstance, NULL);
			} break;

		case WM_COMMAND:
		{
			switch (LOWORD (wParam)) // Check which control was pressed
			{
				case 1: // The "Generate" Button
				{
					if (SendMessage (hRadBtn1, BM_GETCHECK, 0, 0) == BST_CHECKED || SendMessage (hRadBtn2, BM_GETCHECK, 0, 0) == BST_CHECKED || SendMessage (hRadBtn3, BM_GETCHECK, 0, 0) == BST_CHECKED 
						|| SendMessage (hRadBtn4, BM_GETCHECK, 0, 0) == BST_CHECKED || SendMessage (hRadBtn5, BM_GETCHECK, 0, 0) == BST_CHECKED) // check if a password strength is selected
					{
						if (SendMessage (hChkBx, BM_GETCHECK, 0, 0) == BST_CHECKED) // Check if the "Generate 10 passwords" is selected and perform events
						{
							if (SendMessage (hRadBtn1, BM_GETCHECK, 0, 0) == BST_CHECKED)
							{
								Clear ();
								for (int x = 1; x <= 10; x++)
								{
									PasswordGen (1);
									PrintPassword10 ();
								}
								MessageBeep (MB_OK);
							}
							else if (SendMessage (hRadBtn2, BM_GETCHECK, 0, 0) == BST_CHECKED)
							{
								Clear ();
								for (int x = 1; x <= 10; x++)
								{
									PasswordGen (2);
									PrintPassword10 ();
								}
								MessageBeep (MB_OK);
							}
							else if (SendMessage (hRadBtn3, BM_GETCHECK, 0, 0) == BST_CHECKED)
							{
								Clear ();
								for (int x = 1; x <= 10; x++)
								{
									PasswordGen (3);
									PrintPassword10 ();
								}
								MessageBeep (MB_OK);
							}
							else if (SendMessage (hRadBtn4, BM_GETCHECK, 0, 0) == BST_CHECKED)
							{
								Clear ();
								for (int x = 1; x <= 10; x++)
								{
									PasswordGen (4);
									PrintPassword10 ();
								}
								MessageBeep (MB_OK);
							}
							else if (SendMessage (hRadBtn5, BM_GETCHECK, 0, 0) == BST_CHECKED)
							{
								Clear ();
								for (int x = 1; x <= 10; x++)
								{
									PasswordGen (5);
									PrintPassword10 ();
								}
								MessageBeep (MB_OK);
							}
						}
						else
						PrintPassword ();
					}
					else
					{
						Edit_GetText (hTxtBx, text, 100);
						if (wcsncmp (text, L"", 100) == 0)
						{
							MessageBox (hWnd, L"Select a password strength.", L"Secure Password Generator ver.2.0", MB_OK | MB_ICONEXCLAMATION);
						}
					}
				} break;

				case 2: // Radio Button1
				{
					Maintain ();
					goto A;
				}

				case 3: // Radio Button2
				{
					Maintain ();
					goto B;
				} 

				case 4: // Radio Button3
				{
					Maintain ();
					goto C;
				} 

				case 5: // Radio Button4
				{
					Maintain ();
					goto D;
				} 
				
				case 6: //Check box
				{
					Maintain ();
				} break;

				case 7: // Clear button
				{
					Clear ();
				} break;
				
				case 8: // Raduio button 5
				{
					Maintain ();
					goto E;
				} break;

				case ID_HELP_ABOUT40004:
				{
					DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, AboutDialogProc);
				} break;

				case ID_HELP_ABOUT40008:
				{
					DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, AboutDialogProc);
				} break;

				case ID_EXIT:
				{
					PostQuitMessage (0);
					return 0;
				} break;

				case ID_Export:
				{
					Edit_GetText (hTxtBx, pass, 1000);
					if (wcscmp (pass, L"") == 0)
					{
						MessageBox (hWnd, L"No valid passwords. Click \"Generate\" to generate passwords.", L"Secure Password Generator ver.2.0", MB_ICONEXCLAMATION | MB_OK);
					}
					else
					{
						std::wofstream passwordOut ("Generated Password.txt", std::ios::out);
						passwordOut << pass;
						passwordOut.close ();
						MessageBox (hWnd, L"Generated Password/s export complete.\nExported passwords are located in the\ninstallation folder of this application.", L"Secure Password Generator ver.2.0", MB_OK | MB_ICONINFORMATION);
					}
				} break;
			}
			switch (HIWORD (wParam)) // Check for events
			{
				case BN_CLICKED:
				{
					A:
					if (SendMessage (hRadBtn1, BM_GETCHECK, 0, 0) == BST_CHECKED)
					{
						PasswordGen (1);
					}
					B:
					if (SendMessage (hRadBtn2, BM_GETCHECK, 0, 0) == BST_CHECKED)
					{
						PasswordGen (2);
					}
					C:
					if (SendMessage (hRadBtn3, BM_GETCHECK, 0, 0) == BST_CHECKED)
					{
						PasswordGen (3);
					}
					D:
					if (SendMessage (hRadBtn4, BM_GETCHECK, 0, 0) == BST_CHECKED)
					{
						PasswordGen (4);
					}
					E:
					if (SendMessage (hRadBtn5, BM_GETCHECK, 0, 0) == BST_CHECKED)
					{
						PasswordGen (5);
					}
				} break;
		    }
	    } break;
	}
	return (DefWindowProc (hWnd, Message, wParam, lParam));
}
void PasswordGen (int X) // Fucntion for generating the password based from the "password strength" selection.
{
	switch (X)
	{
		case 1: // Weak
		{
			srand ((unsigned int) time (NULL) + rand ()); // Seed the rand () function.
			for (X = 1; X <= 10; X++)
			{
				password += WeakChars[rand () % 36];
			} 
		} break;

		case 2: // Good
		{
			srand ((unsigned int) time (NULL) + rand ()); // Seed the rand () function.
			for (X = 1; X <= 20; X++)
			{
				password += GoodChars[rand () % 61];
			} 
		} break;

		case 3: //Strong
		{
			srand ((unsigned int) time (NULL) + rand ()); // Seed the rand () function.
			for (X = 1; X <= 40; X++)
			{
				password += StrongChars[rand () % 78];
			} 
		} break;

		case 4: // Very Strong
		{
			srand ((unsigned int) time (NULL) + rand ()); // Seed the rand () function.
			for (X = 1; X <= 80; X++)
			{
				password += VeryStrongChars[rand () % 90]; 
			}
		} break;

		case 5: // Very Weak
		{
			srand ((unsigned int) time (NULL) + rand ()); // Seed the rand () function.
			for (X = 1; X <= 5; X++)
			{
				password += VeryWeakChars[rand () % 26]; 
			}
		} break;
	}
}
void PrintPassword ()
{
	Edit_SetText (hTxtBx, L"");
	Edit_SetText (hTxtBx, password.c_str ()); // Print the Password.
	password = L"";
	MessageBeep (MB_OK);
}
void PrintPassword10 ()
{
	password += L"\r\n\r\n";
	Edit_SetText (hTxtBx, password.c_str ()); // Print the Password.
}
void Maintain ()
{
	password = L"";
}
void Clear ()
{
	Edit_SetText (hTxtBx, L"");
	password = L"";
}
INT_PTR CALLBACK AboutDialogProc (HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam) // DialogBox
{
	UNREFERENCED_PARAMETER (lParam);
	switch (Message)
	{
		case WM_INITDIALOG:
		{
			MessageBeep (MB_ICONINFORMATION);
			return (INT_PTR)TRUE;
		} break;

		case WM_COMMAND:
		{
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			} 
		} break;
	}
	return (INT_PTR)FALSE;
}
