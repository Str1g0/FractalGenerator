#ifndef _WIN32
#error "Only Windows is supported!"
#endif

#include <FractalGenerator.h>
#include "Utils/Stopwatch.h"

#define WGL_PROC(name) PFNWGL ## WGL ## name ## PROC

LRESULT _stdcall MainWindowProc(HWND winHandle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static constexpr auto DEF_ZOOM			= 1.f;
	static constexpr auto DEF_ZOOM_UNIT		= .01f;
	static constexpr auto DEF_OFF_X			= .1f;
	static constexpr auto DEF_OFF_Y			= .1f;

	static float		zoomUnit		= DEF_ZOOM;
	static math::vec2f	offsetUnit		= { DEF_OFF_X, DEF_OFF_Y };
	static FractalType	toggleFractals	= FractalType::MANDELBROT;

	switch (msg)
	{
		case WM_CREATE:
			break;

		case WM_KEYDOWN:
		{
			if (FractalGenerator::IsInitialized())
			{
				switch (wParam)
				{	
					case VK_ESCAPE:
						std::exit(EXIT_SUCCESS);

					case VK_LEFT:
						FractalGenerator::GetInstance()->SetOffsetX(-offsetUnit.x * std::abs(zoomUnit), true);
						break;
					case VK_RIGHT:
						FractalGenerator::GetInstance()->SetOffsetX(offsetUnit.x * std::abs(zoomUnit), true);
						break;
					case VK_DOWN:
						FractalGenerator::GetInstance()->SetOffsetY(-offsetUnit.y * std::abs(zoomUnit), true);
						break;
					case VK_UP:
						FractalGenerator::GetInstance()->SetOffsetY(offsetUnit.y * std::abs(zoomUnit), true);
						break;

					case VK_SHIFT:
						toggleFractals = toggleFractals == FractalType::MANDELBROT ?
							FractalType::JULIA : FractalType::MANDELBROT;
						FractalGenerator::GetInstance()->SetFractalType(toggleFractals);
						break;

					case VK_ADD:
						zoomUnit += DEF_ZOOM_UNIT;
						FractalGenerator::GetInstance()->SetZoom(zoomUnit);
						break;

					case VK_SUBTRACT:
						zoomUnit += DEF_ZOOM_UNIT;
						FractalGenerator::GetInstance()->SetZoom(-zoomUnit);
						break;

					case VK_HOME:
						zoomUnit	= DEF_ZOOM;
						offsetUnit	= { DEF_OFF_X, DEF_OFF_Y };
						FractalGenerator::GetInstance()->ResetView();
						break;

					case VK_F1:
						Misc::ToggleConsole();
						break;
				}
			}
		}
		break;

		case WM_MOUSEWHEEL:
		{
			if (FractalGenerator::IsInitialized())
			{
				if (HIWORD(wParam) == 120)
				{
					zoomUnit += DEF_ZOOM_UNIT;
					FractalGenerator::GetInstance()->SetZoom(zoomUnit);
				}

				else if (HIWORD(wParam) >= -120)
				{
					zoomUnit -= DEF_ZOOM_UNIT;
					FractalGenerator::GetInstance()->SetZoom(zoomUnit);
				}
			}
		}
		break;

		case WM_SIZE:
		{
			if (FractalGenerator::IsInitialized())
			{
				FractalGenerator::GetInstance()->UpdateViewport();
			}
		}
		break;

		case WM_CLOSE:
			DestroyWindow(winHandle);
			break;

		case WM_DESTROY:
			PostQuitMessage(EXIT_SUCCESS);
			break;

		default:
			return DefWindowProc(winHandle, msg, wParam, lParam);
	}

	return TRUE;
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE not_used,
	LPSTR cmdLine, int iCmdLine)
{
	FractalGenerator::Init(instance, MainWindowProc);

	FractalGenerator::GetInstance()->SetMaxIterations(300u);
	FractalGenerator::GetInstance()->SetZoom(0.01f, true);
	FractalGenerator::GetInstance()->Run();

	return 0;
}