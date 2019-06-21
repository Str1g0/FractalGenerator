#pragma once

#include <Util.h>
#include <App\Logging.h>

#include <Graphics\OpenGL_Util.hpp>

enum class MSAA :
	byte
{
	NONE,
	X2	= 2u,
	X4	= 4u,
	X8	= 8u,
	X16	= 16u
};

struct ContextArgs
{
	uint8  versionMajor;
	uint8  versionMinor;
	uint8  redSize;
	uint8  greenSize;
	uint8  blueSize;
	uint8  alphaSize;
	uint8  depthSize;
	uint8  stencilSize;
	MSAA   antiAliasing;

	constexpr uint8 inline getColorSize() {
		return redSize + greenSize + blueSize + alphaSize;
	}
};

struct Resolution
{
	union { uint32 x, width {0u}; };
	union { uint32 y, height{0u}; };

	inline constexpr Resolution() = default;

	inline constexpr Resolution(uint32 width, uint32 height) :
		width(width),
		height(height) 
	{};

	uint32 constexpr inline Area() {
		return width * height;
	}

	constexpr bool inline operator==(Resolution other) {
		return x == other.x && y == other.y;
	}

	constexpr bool inline operator!=(Resolution other) {
		return !(*this == other);
	}

	constexpr bool inline operator<(Resolution other) {
		return x * y < other.x * other.y;
	}

	constexpr bool inline operator>(Resolution other) {
		return x * y > other.x * other.y;
	}

	constexpr bool inline operator>=(Resolution other) {
		return x * y >= other.x * other.y;
	}

	constexpr bool inline operator<=(Resolution other) {
		return x * y <= other.x * other.y;
	}
};

namespace stm = std::chrono;

struct FramerateLimiter
{
	typedef
		std::conditional<stm::high_resolution_clock::is_steady,
		stm::high_resolution_clock,
		stm::steady_clock>::type
		clock;

	FramerateLimiter(stm::milliseconds cap);

	void OnStartFrame();
	void OnEndFrame();

	private:

	stm::time_point<clock> m_begin;
	stm::time_point<clock> m_end;

	stm::milliseconds m_targetTime;
	stm::milliseconds m_lastFrameTime;
};

struct AppOptions
{
	HINSTANCE		instance;
	std::string		name;
	std::string		icon;
	std::string		cursor;
	ContextArgs		contextAttribs;
	Resolution		resolution;
	bool			createContext;
	WNDPROC			callback;
};

class WinapiApp
{
	static constexpr cstring CLASS_NAME		= "WinapiApp";
	static constexpr cstring LOG_TAG		= "WinApp";
	static constexpr uint16	 FRAME_TARGET	= 1000 / 60;

	static uint16 s_instanceCount;

	WNDCLASSEX		m_windowClass;
	HWND			m_windowHandle;
	MSG				m_currentMessage;
	WNDPROC			m_callback;
	HDC				m_device;

	std::string m_className = CLASS_NAME;

	std::string	m_title;
	Resolution	m_resolution;

	bool m_hasGLContext = false;

	ContextArgs m_contextSettings;
	HGLRC		m_openGLContext	{nullptr};
	HINSTANCE	m_instance		{nullptr};

	HICON	m_iconBig	{NULL};
	HICON	m_iconSmall	{NULL};
	HCURSOR m_cursor	{NULL};

	std::function<void()> m_frameTick = [](){};

	FramerateLimiter m_limiter;
	bool			 m_limitFPS{true};

	HGLRC					CreateFakeContext();
	std::vector<int>		GetWGLAttributes(bool contextAtrbs = false);

	void RegisterWindowClass();

	void CreateAppWindow();
	void CreateOpenGLContext();

	public:

	WinapiApp(AppOptions args);

	void InitWindow();
	void DestroyAppWindow();

	void MainLoop();

	bool HasContext();

	void SetFrameTick(std::function<void()> tick);
	void SetWindowPosition(uint32 const anchor_x, uint32 const anchor_y);

	math::vec2u GetWindowSize();
};