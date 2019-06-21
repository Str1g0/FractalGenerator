#include "FractalGenerator.h"

#include<Windowsx.h>
#include<Commctrl.h>
#include<Commdlg.h>

std::unique_ptr<FractalGenerator>	 FractalGenerator::k_instance	{nullptr};
bool								 FractalGenerator::k_isInit		{false};
uint16_t							 FractalControls::s_instanceCount = 0u;

FractalGenerator::FractalGenerator(HINSTANCE instance, WNDPROC proc)//:
	//m_controlWindow(std::make_shared<FractalControls>(instance))
{
	CreateApplication(instance, proc);
	InitializeGraphics();

	LOG_DBG(TAG, "FractalGenerator has been initialized");
}

bool FractalGenerator::CreateApplication(HINSTANCE instance, WNDPROC proc)
{
	ContextArgs contextAtrb		{ 0 };
	contextAtrb.blueSize		= contextAtrb.redSize = contextAtrb.greenSize = contextAtrb.alphaSize = 8u;
	contextAtrb.antiAliasing	= MSAA::X4;
	contextAtrb.depthSize		= 24;
	contextAtrb.stencilSize		= 8;
	contextAtrb.versionMajor	= 3;
	contextAtrb.versionMinor	= 3;

	AppOptions options			{ 0 };
	options.instance			= instance;
	options.name				= m_name;
	options.resolution			= { DEFAULT_WIDTH, DEFAULT_HEIGHT };
	options.callback			= proc;
	options.createContext		= true;
	options.contextAttribs		= contextAtrb;

	Misc::ToggleConsole();
	m_application.reset(new WinapiApp(options));

	if (!m_application)
		return false;

	constexpr uint32 offsetX = 100u; 
	constexpr uint32 offsetY = 100u;

	m_application->InitWindow();
	m_application->SetWindowPosition(offsetX, offsetY);

	return true;
}

bool FractalGenerator::InitializeGraphics()
{
	Graphics::ShaderCode vertexCode;
	Graphics::ShaderCode fragmentCode;

	vertexCode.LoadFromFile("../Resources/QuadVertex.glsl");
	fragmentCode.LoadFromFile("../Resources/MandelbrotFragment.glsl");

	Graphics::ShaderStagePtr vertex		= std::make_shared<Graphics::ShaderStage>(Graphics::ShaderType::VERTEX, vertexCode);
	Graphics::ShaderStagePtr fragment	= std::make_shared<Graphics::ShaderStage>(Graphics::ShaderType::FRAGMENT, fragmentCode);

	m_fractalShader = std::make_shared<Graphics::ShaderProgram>();

	if (!m_fractalShader || !vertex || !fragment)
		return false;

	m_fractalShader->AddStage(vertex,	Graphics::ShaderType::VERTEX);
	m_fractalShader->AddStage(fragment, Graphics::ShaderType::FRAGMENT);

	m_fractalShader->LinkProgram();

	m_fractalShader->RegisterUniform("u_maxIter",			Graphics::ValueType::UINT);
	m_fractalShader->RegisterUniform("u_zoom",				Graphics::ValueType::FLOAT);
	m_fractalShader->RegisterUniform("u_offset",			Graphics::ValueType::VEC2F);
	m_fractalShader->RegisterUniform("u_canvas",			Graphics::ValueType::VEC2U);
	m_fractalShader->RegisterUniform("u_fractalType",		Graphics::ValueType::BOOL);
	m_fractalShader->RegisterUniform("u_colorModifier",		Graphics::ValueType::VEC3F);
	m_fractalShader->RegisterUniform("u_juliaConstant",		Graphics::ValueType::VEC2F);

	m_screenCanvas	= std::make_shared<Graphics::Quad>();

	UpdateViewport();

	return true;
}

void FractalGenerator::SetLoop()
{
	m_application->SetFrameTick
	(
		[this]()
		{
			m_fractalShader->SetUniformUint	("u_maxIter",		m_maxIterations);
			m_fractalShader->SetUniformBool	("u_fractalType",	(bool)m_fractal);
			m_fractalShader->SetUniformFloat("u_zoom",			m_zoom);
			m_fractalShader->SetUniform2f	("u_offset",		m_offset);
			m_fractalShader->SetUniform2u	("u_canvas",		m_viewport);
			m_fractalShader->SetUniform3f	("u_colorModifier",	m_colorModifier);
			m_fractalShader->SetUniform2f	("u_juliaConstant",	m_juliaConstant);
			
			this->Draw();
		}
	);
}

void FractalGenerator::Draw()
{
	Flush();
	m_screenCanvas->Draw(m_fractalShader);
}

void FractalGenerator::Flush()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(m_clearColor.r,
				 m_clearColor.g,
				 m_clearColor.b,
				 m_clearColor.a);
}

bool FractalGenerator::IsInitialized()
{
	return k_isInit;
}

bool FractalGenerator::Init(HINSTANCE instance, WNDPROC proc)
{
	if (!k_isInit)
	{
		k_instance.reset(new FractalGenerator(instance, proc));
		return k_isInit = k_instance ? true : false;
	}

	return true;
}

FractalGenerator::FractalGenPtr& FractalGenerator::GetInstance()
{
	ASSERT(k_isInit, "Instance has not been initialized!");
	return k_instance;
}

void FractalGenerator::Run()
{
	SetLoop();
	m_application->MainLoop();
}

void FractalGenerator::UpdateViewport()
{
	m_viewport = m_application->GetWindowSize();
	glViewport(0, 0, m_viewport.x, m_viewport.y);

	//LOG_DBG(TAG, "Viewport has changed %u : %u", m_viewport.x, m_viewport.y);
}

void FractalGenerator::ResetView()
{
	LOG_INFO(TAG, "Fractal view has been resetted.");

	m_zoom			= { DEF_ZOOM };
	m_offset		= { DEF_OFF_X, DEF_OFF_Y };
	m_colorModifier	= { 1.f, 1.f, 1.f };
	m_maxIterations	= { DEF_ITER };
}

void FractalGenerator::SetMaxIterations(uint32 maxIter)
{
	m_maxIterations = maxIter;
}

void FractalGenerator::SetFractalType(FractalType fractal)
{
	switch (fractal)
	{
		case FractalType::MANDELBROT:	 LOG_INFO(TAG, "Fractal changed to Mandelbrot Set"); break;
		case FractalType::JULIA:		 LOG_INFO(TAG, "Fractal changed to Julia Set");		 break;
		default: ASSERT(false, "Invalid fractal type")
	}

	m_fractal = fractal;
}

void FractalGenerator::SetOffsetX(float value, bool isOffset)
{
	LOG_INFO(TAG, "Offset changed: %f : %f", m_offset.x, m_offset.y);

	isOffset ?
		m_offset.x += value :
		m_offset.x  = value;
}

void FractalGenerator::SetOffsetY(float value, bool isOffset)
{
	LOG_INFO(TAG, "Offset changed: %f : %f", m_offset.x, m_offset.y);

	isOffset ?
		m_offset.y += value :
		m_offset.y  = value;
}


void FractalGenerator::SetZoom(float value, bool isOffset)
{
	if (m_zoom + value <= 0) return;

	LOG_INFO(TAG, "Zoom changed: %f", m_zoom);

	isOffset ?
		m_zoom += value :
		m_zoom  = value ;
}

void FractalGenerator::SetRedModifier(float value, bool isOffset)
{
	isOffset ?
		m_colorModifier.r += value :
		m_colorModifier.r  = value ;
}

void FractalGenerator::SetGreenModifier(float value, bool isOffset)
{
	isOffset ?
		m_colorModifier.g += value :
		m_colorModifier.g  = value ;
}

void FractalGenerator::SetBlueModifier(float value, bool isOffset)
{
	isOffset ?
		m_colorModifier.b += value :
		m_colorModifier.b  = value ;
}

LRESULT FractalControls::ControlProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
	const math::vec2u ANCHOR   {10u, 10u};
	const math::vec2u OFFSET_L {120u, 80u};
	const math::vec2u OFFSET_B {120u, 100u};

	struct Placement
	{
		math::vec2u label;
		math::vec2u control;
	};

	auto PlaceControl = [=](uint16_t i, uint16_t j) 
		-> Placement
	{
		Placement location;
		location.label   = {OFFSET_L.x * i, OFFSET_L.y * j};
		location.control = {OFFSET_B.x * i, OFFSET_B.y * j};

		return location;
	};

	HWND lbl_iter;
	HWND btn_iter;

	switch (msg)
	{
		case WM_CREATE:
		{
			auto loc = PlaceControl(1, 1);
			lbl_iter = CreateWindow("Static", "Iterations", WS_CHILD | WS_VISIBLE | WS_BORDER,
									loc.label.x, loc.label.y, 100, 20, handle, (HMENU)-1, NULL, NULL);

			btn_iter = CreateWindow("EDIT", "Iterations", WS_CHILD | WS_VISIBLE | ES_NUMBER | WS_BORDER,
				loc.control.x, loc.control.y, 100, 20, handle, (HMENU)-1, NULL, NULL);
			
			loc = PlaceControl(1, 2);

			HWND lbl_iter1 = CreateWindow("Static", "Iterations", WS_CHILD | WS_VISIBLE | WS_BORDER,
				loc.label.x, loc.label.y, 100, 20, handle, (HMENU)-1, NULL, NULL);

			HWND btn_iter2 = CreateWindow("EDIT", "Iterations", WS_CHILD | WS_VISIBLE | ES_NUMBER | WS_BORDER,
				loc.control.x, loc.control.y, 100, 20, handle, (HMENU)-1, NULL, NULL);

			break;
		}

		case WM_CLOSE:
			DestroyWindow(handle);
			break;

		case WM_DESTROY:
			PostQuitMessage(EXIT_SUCCESS);
			break;

		default:
			return DefWindowProc(handle, msg, wparam, lparam);
	}

	return TRUE;
}

bool FractalControls::CreateControlWindow()
{
	constexpr size_t WINCLASS_SIZE = sizeof(WNDCLASSEX);
	ZeroMemory(&m_windowClass, WINCLASS_SIZE);

	m_className = CLASS_NAME + '_' + STR(s_instanceCount++);

	m_windowClass.lpszClassName = m_className.c_str();
	m_windowClass.cbSize		= WINCLASS_SIZE;
	m_windowClass.hInstance		= m_instance;
	m_windowClass.hbrBackground = HBRUSH(COLOR_WINDOW);
	m_windowClass.lpfnWndProc	= (WNDPROC)ControlProc;

	int registrationSucceded = RegisterClassEx(&m_windowClass);
	
	if (!registrationSucceded)
	{
		LOG_ERR("Failed to register window class: %s", m_className.c_str());
		return false;
	}

	ASSERT(registrationSucceded, "Failed to register window class: %s", m_className.c_str());

	m_windowHandle =
		CreateWindowEx(
			WIN_TYPE,
			m_className.c_str(),
			WIN_TITLE,
			WIN_OPTS,
			0, 0,
			DLG_SIZE.width,
			DLG_SIZE.height,
			NULL,
			NULL,
			m_instance,
			NULL
		);

	if (!m_windowHandle)
	{
		LOG_ERR("Failed to create window: %s, of type %s", WIN_TITLE, m_className.c_str());
		return false;
	}
	
	SetWindowPos(m_windowHandle, HWND_TOP, 100, 100, DLG_SIZE.width, DLG_SIZE.height, SWP_SHOWWINDOW);

	ShowWindow(m_windowHandle, SW_SHOWNORMAL);
	UpdateWindow(m_windowHandle);

	return true;
}

FractalControls::FractalControls(HINSTANCE instance):
	m_instance(instance)
{
	CreateControlWindow();
}
