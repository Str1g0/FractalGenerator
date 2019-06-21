#include "WinapiApp.h"

uint16 WinapiApp::s_instanceCount = 0u;

HGLRC WinapiApp::CreateFakeContext()
{
	static constexpr size_t PFD_SIZE		= sizeof(PIXELFORMATDESCRIPTOR);
	static constexpr uint16 DUMMY_VERSION	= 1U;

	PIXELFORMATDESCRIPTOR dummyContextFormat;
	ZeroMemory(&dummyContextFormat, sizeof(dummyContextFormat));

	dummyContextFormat.nSize		= PFD_SIZE;
	dummyContextFormat.nVersion		= DUMMY_VERSION;
	dummyContextFormat.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	dummyContextFormat.iPixelType	= PFD_TYPE_RGBA;
	dummyContextFormat.cColorBits	= m_contextSettings.getColorSize();
	dummyContextFormat.cAlphaBits	= m_contextSettings.alphaSize;
	dummyContextFormat.cDepthBits	= m_contextSettings.depthSize;
	dummyContextFormat.cStencilBits = m_contextSettings.stencilSize;
	dummyContextFormat.iLayerType	= PFD_MAIN_PLANE;

	HGLRC dummyContext	{NULL};

	SetPixelFormat(m_device, ChoosePixelFormat(m_device, &dummyContextFormat), &dummyContextFormat);
	dummyContext = wglCreateContext(m_device);

	ASSERT(dummyContext, "Failed to create the dummy context!");

	return dummyContext;
}

std::vector<int> WinapiApp::GetWGLAttributes(bool contextAtrbs)
{
	std::vector<int> wglAttributes;
	/*
	WGL_DRAW_TO_WINDOW_ARB
	WGL_DRAW_TO_BITMAP_ARB
	WGL_ACCELERATION_ARB
	WGL_NEED_PALETTE_ARB
	WGL_NEED_SYSTEM_PALETTE_ARB
	WGL_SWAP_LAYER_BUFFERS_ARB
	WGL_SWAP_METHOD_ARB
	WGL_NUMBER_OVERLAYS_ARB
	WGL_NUMBER_UNDERLAYS_ARB
	WGL_SHARE_DEPTH_ARB
	WGL_SHARE_STENCIL_ARB
	WGL_SHARE_ACCUM_ARB
	WGL_SUPPORT_GDI_ARB
	WGL_SUPPORT_OPENGL_ARB
	WGL_DOUBLE_BUFFER_ARB
	WGL_STEREO_ARB
	WGL_PIXEL_TYPE_ARB
	WGL_COLOR_BITS_ARB
	WGL_RED_BITS_ARB
	WGL_GREEN_BITS_ARB
	WGL_BLUE_BITS_ARB
	WGL_ALPHA_BITS_ARB
	WGL_ACCUM_BITS_ARB
	WGL_ACCUM_RED_BITS_ARB
	WGL_ACCUM_GREEN_BITS_ARB
	WGL_ACCUM_BLUE_BITS_ARB
	WGL_ACCUM_ALPHA_BITS_ARB
	WGL_DEPTH_BITS_ARB
	WGL_STENCIL_BITS_ARB
	WGL_AUX_BUFFERS_ARB
	*/

	if (contextAtrbs)
	{
		wglAttributes.push_back(WGL_CONTEXT_MAJOR_VERSION_ARB);
		wglAttributes.push_back(m_contextSettings.versionMajor);

		wglAttributes.push_back(WGL_CONTEXT_MINOR_VERSION_ARB);
		wglAttributes.push_back(m_contextSettings.versionMinor);

		wglAttributes.push_back(WGL_CONTEXT_FLAGS_ARB);
		wglAttributes.push_back(WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB);

		wglAttributes.push_back(WGL_CONTEXT_PROFILE_MASK_ARB);
		wglAttributes.push_back(WGL_CONTEXT_CORE_PROFILE_BIT_ARB);

		wglAttributes.push_back(0);
		
		return wglAttributes;
	}

	wglAttributes.push_back(WGL_DRAW_TO_WINDOW_ARB);
	wglAttributes.push_back(GL_TRUE);

	wglAttributes.push_back(WGL_SUPPORT_OPENGL_ARB);
	wglAttributes.push_back(GL_TRUE);

	wglAttributes.push_back(WGL_SWAP_METHOD_ARB);
	wglAttributes.push_back(WGL_SWAP_EXCHANGE_ARB);

	wglAttributes.push_back(WGL_DOUBLE_BUFFER_ARB);
	wglAttributes.push_back(GL_TRUE);

	wglAttributes.push_back(WGL_ACCELERATION_ARB);
	wglAttributes.push_back(WGL_FULL_ACCELERATION_ARB);

	wglAttributes.push_back(WGL_COLOR_BITS_ARB);
	wglAttributes.push_back(m_contextSettings.getColorSize());

	wglAttributes.push_back(WGL_RED_BITS_ARB);
	wglAttributes.push_back(m_contextSettings.redSize);

	wglAttributes.push_back(WGL_GREEN_BITS_ARB);
	wglAttributes.push_back(m_contextSettings.greenSize);

	wglAttributes.push_back(WGL_BLUE_BITS_ARB);
	wglAttributes.push_back(m_contextSettings.blueSize);

	wglAttributes.push_back(WGL_ALPHA_BITS_ARB);
	wglAttributes.push_back(m_contextSettings.alphaSize);

	wglAttributes.push_back(WGL_DEPTH_BITS_ARB);
	wglAttributes.push_back(m_contextSettings.depthSize);

	wglAttributes.push_back(WGL_STENCIL_BITS_ARB);
	wglAttributes.push_back(m_contextSettings.stencilSize);

	if (m_contextSettings.alphaSize)
	{
		wglAttributes.push_back(WGL_PIXEL_TYPE_ARB);
		wglAttributes.push_back(WGL_TYPE_RGBA_ARB);
	}
	
	if (m_contextSettings.antiAliasing != MSAA::NONE)
	{
		wglAttributes.push_back(WGL_SAMPLE_BUFFERS_ARB);
		wglAttributes.push_back(1);

		wglAttributes.push_back(WGL_SAMPLES_ARB);
		wglAttributes.push_back(static_cast<int>(m_contextSettings.antiAliasing));
	}

	wglAttributes.push_back(0);

	return wglAttributes;
}

void WinapiApp::RegisterWindowClass()
{
	constexpr size_t WINCLASS_SIZE = sizeof(WNDCLASSEX);
	ZeroMemory(&m_windowClass, WINCLASS_SIZE);

	m_className = CLASS_NAME + '_' + STR(s_instanceCount++);

	m_windowClass.lpszClassName = m_className.c_str();
	m_windowClass.cbSize = WINCLASS_SIZE;
	m_windowClass.hInstance = m_instance;
	m_windowClass.hbrBackground = HBRUSH(COLOR_WINDOW);
	m_windowClass.hIcon = m_iconBig;
	m_windowClass.hIconSm = m_iconSmall;
	m_windowClass.hCursor = m_cursor;
	m_windowClass.lpfnWndProc = m_callback;

	bool registrationSucceded = RegisterClassEx(&m_windowClass);
	ASSERT(registrationSucceded, "Failed to register window class: %s", m_className.c_str());
}

WinapiApp::WinapiApp(AppOptions args):
	m_callback(args.callback),
	m_title(args.name),
	m_resolution(args.resolution),
	m_hasGLContext(args.createContext),
	m_contextSettings(args.contextAttribs),
	m_instance(args.instance),
	m_limiter(stm::milliseconds(FRAME_TARGET))
{
	RegisterWindowClass();
}

void WinapiApp::CreateAppWindow()
{
	m_windowHandle =
		CreateWindowEx(
			WS_EX_CLIENTEDGE,
			m_className.c_str(),
			m_title.c_str(),
			WS_OVERLAPPEDWINDOW,
			0, 0,
			m_resolution.width,
			m_resolution.height,
			NULL,
			NULL,
			m_instance,
			NULL
		);

	ASSERT(m_windowHandle, "Failed to create window: %s, of type %s", m_title, m_className.c_str());

	m_device = GetDC(m_windowHandle);
	ASSERT(m_device, "The device cannot be null!");

	ShowWindow(m_windowHandle, SW_SHOWNORMAL);
	UpdateWindow(m_windowHandle);
}

void WinapiApp::InitWindow()
{
	CreateAppWindow();

	if(m_hasGLContext)
		CreateOpenGLContext();
}

void WinapiApp::DestroyAppWindow()
{
	if (!DestroyWindow(m_windowHandle))
	{
		LOG_ERR(LOG_TAG, "Failed to kill window: %s of type %s!", m_title.c_str(), m_className.c_str());
	}
}

void WinapiApp::MainLoop()
{
	while (GetMessage(&m_currentMessage, m_windowHandle, 0, 0) > 0)
	{
		if (m_limitFPS)
			m_limiter.OnStartFrame();

		m_frameTick();

		if (m_hasGLContext) {
			SwapBuffers(m_device);
		}

		if (m_limitFPS)
			m_limiter.OnEndFrame();

		TranslateMessage(&m_currentMessage);
		DispatchMessage(&m_currentMessage);
	}
}

void WinapiApp::SetFrameTick(std::function<void()> tick)
{
	m_frameTick = tick;
}

void WinapiApp::SetWindowPosition(uint32 const anchor_x, uint32 const anchor_y)
{
	if (m_windowHandle)
	{ 
		SetWindowPos(m_windowHandle, 
					 HWND_TOP, 
					 anchor_x, anchor_y, 
					 anchor_x + m_resolution.width,
					 anchor_y + m_resolution.height, 
					 SWP_SHOWWINDOW);
	}
}

math::vec2u WinapiApp::GetWindowSize()
{
	RECT windowBounds;
	GetWindowRect(m_windowHandle, &windowBounds);

	math::vec2i size{ windowBounds.right  - windowBounds.left,
					  windowBounds.bottom - windowBounds.top  };

	return {(uint32)size.x, (uint32)size.y};
}

void WinapiApp::CreateOpenGLContext()
{
	HGLRC fakeContext = CreateFakeContext();
	ASSERT(wglMakeCurrent(m_device, fakeContext), LOG_TAG, "Failed to set context!");

	constexpr uint8 MAX_FORMATS		= 1u;
	constexpr int8	INVALID_FORMAT	= -1;

	int	 chosenPixelFormat	= INVALID_FORMAT;
	UINT noFoundFormats		= 0u;

	PFNWGLCHOOSEPIXELFORMATARBPROC tempWglChooseFormat =
		(PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");

	std::vector<int> wglAttributes = GetWGLAttributes();

	ASSERT(
		tempWglChooseFormat(m_device,
			wglAttributes.data(),
			NULL,
			MAX_FORMATS,
			&chosenPixelFormat,
			&noFoundFormats) != FALSE,
			"Failed to chose a format"
	);

	LOG_INFO(LOG_TAG, "pixel format = %i", chosenPixelFormat);
	ASSERT((chosenPixelFormat!= INVALID_FORMAT), "Failed to select a pixel format!");

	PFNWGLCREATECONTEXTATTRIBSARBPROC tempWGLCreateContext =
		(PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(fakeContext);

	DestroyAppWindow();
	CreateAppWindow();

	PIXELFORMATDESCRIPTOR chosenPFD{0};
	SetPixelFormat(m_device, chosenPixelFormat, &chosenPFD);

	auto contextAttribs = GetWGLAttributes(true);
	m_openGLContext = tempWGLCreateContext(m_device, 0, contextAttribs.data());

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(fakeContext);

	ASSERT(wglMakeCurrent(m_device, m_openGLContext), "Failed to create context");

	ASSERT(gladLoadGL(), "Failed to load OpenGL functions!");
	ASSERT(gladLoadWGL(m_device), "Failed to load WGL functions!");

	std::string version = cstring(glGetString(GL_VERSION));

	LOG_DBG("OpenGL", "Detected version: %s", version.c_str());

	constexpr int V_MAJOR = 1;
	constexpr int V_MINOR = 3;

	if (version.find(std::to_string(contextAttribs[V_MAJOR])) == std::string::npos or
		version.find(std::to_string(contextAttribs[V_MINOR])) == std::string::npos)
	{
		LOG_FTL("OpenGL", "The hardware doesn't support OpenGL %i %i", contextAttribs[V_MAJOR], contextAttribs[V_MINOR]);
	}

	m_hasGLContext = true;
}

FramerateLimiter::FramerateLimiter(stm::milliseconds cap):
	m_targetTime(cap),
	m_lastFrameTime(stm::milliseconds(0))
{
}

void FramerateLimiter::OnStartFrame()
{
	if (m_lastFrameTime.count() &&
		m_targetTime > m_lastFrameTime)
	{
		std::this_thread::sleep_for(m_targetTime - m_lastFrameTime);
	}

	m_begin = clock::now();
}

void FramerateLimiter::OnEndFrame()
{
	m_end = clock::now();
	m_lastFrameTime = stm::duration_cast<stm::milliseconds>(m_end - m_begin);
}
