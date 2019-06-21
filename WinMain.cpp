#ifndef _WIN32
#error "Only Windows is supported!"
#endif

//static_assert(__cplusplus > 199711L, "Error, doesn't support c++11")

//if this fails the compiler cannot run the program
constexpr bool does_support_constexpr_is_cpp_11_or_higher = true;

#include <FractalGenerator.h>
#include "Utils/Stopwatch.h"

#define WGL_PROC(name) PFNWGL ## WGL ## name ## PROC

LRESULT _stdcall MainWindowProc(HWND winHandle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static constexpr auto DEF_ZOOM		= 1.f;
	static constexpr auto DEF_OFF_X		= 0.1f;
	static constexpr auto DEF_OFF_Y		= 0.1f;

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
						FractalGenerator::GetInstance()->SetOffsetX(-offsetUnit.x * zoomUnit, true);
						break;
					case VK_RIGHT:
						FractalGenerator::GetInstance()->SetOffsetX(offsetUnit.x * zoomUnit, true);
						break;
					case VK_DOWN:
						FractalGenerator::GetInstance()->SetOffsetY(-offsetUnit.y * zoomUnit, true);
						break;
					case VK_UP:
						FractalGenerator::GetInstance()->SetOffsetY(offsetUnit.y * zoomUnit, true);
						break;

					case VK_SHIFT:
						toggleFractals = toggleFractals == FractalType::MANDELBROT ?
							FractalType::JULIA : FractalType::MANDELBROT;
						FractalGenerator::GetInstance()->SetFractalType(toggleFractals);
						break;

					case VK_ADD:
						zoomUnit *= 1.2f;
						FractalGenerator::GetInstance()->SetZoom(zoomUnit, true);
						break;

					case VK_SUBTRACT:
						zoomUnit *= 0.85f;
						FractalGenerator::GetInstance()->SetZoom(-zoomUnit, true);
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
					zoomUnit *= 1.2f;
					FractalGenerator::GetInstance()->SetZoom(zoomUnit, true);
				}

				else if (HIWORD(wParam) >= -120)
				{
					zoomUnit *= 0.8f;
					FractalGenerator::GetInstance()->SetZoom(-zoomUnit, true);
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