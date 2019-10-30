#include <App\WinapiApp.h>
#include <Graphics\Quad.hpp>

#define CLASS_CSTEXPR static constexpr auto

enum class FractalType:
	byte
{
	MANDELBROT = 0,
	JULIA
};

class FractalGenerator;
class FractalControls;

class FractalGenerator:
	public Misc::Unique
{
	friend class FractalControls;

	typedef std::unique_ptr<FractalGenerator> FractalGenPtr;
	typedef std::shared_ptr<FractalControls>  FractalCtrlPtr;

	static FractalGenPtr k_instance;
	static bool			 k_isInit;

	static constexpr cstring TAG = "Fractal";

	static constexpr auto DEFAULT_WIDTH  = 512u;
	static constexpr auto DEFAULT_HEIGHT = 512u;

	static constexpr auto DEF_ZOOM	=  2.3f;
	static constexpr auto DEF_OFF_X = -1.7f;
	static constexpr auto DEF_OFF_Y = -1.2f;
	static constexpr auto DEF_ITER  =  600u;

	static constexpr math::vec2f DEF_JULIA = { 0.285f, 0.01f };

	FractalCtrlPtr m_controlWindow;

	FractalGenerator(HINSTANCE instance, WNDPROC proc);

	std::unique_ptr<WinapiApp> m_application;

	cstring			m_name				{ "Fractal Generator" };
	float			m_zoom				{ DEF_ZOOM };
	math::vec2f		m_offset			{ DEF_OFF_X, DEF_OFF_Y };
	math::vec3f		m_colorModifier		{1.f};
	uint32			m_maxIterations		{ DEF_ITER };
	FractalType		m_fractal			{FractalType::MANDELBROT};
	math::vec2f		m_juliaConstant		{DEF_JULIA};

	math::vec2u		m_viewport;
	math::vec4f		m_clearColor{1.f, 0.f, 0.f, 1.f};


	Graphics::QuadPtr			m_screenCanvas;
	Graphics::ShaderProgramPtr	m_fractalShader;

	bool CreateApplication(HINSTANCE instance, WNDPROC proc);
	bool InitializeGraphics();

	void SetLoop();

	void Draw();
	void Flush();

	public:

		static bool				IsInitialized();

		static bool				Init(HINSTANCE instance, WNDPROC proc);
		static FractalGenPtr&	GetInstance();

		void Run();
		void UpdateViewport();

		void ResetView();

		void SetMaxIterations	(uint32 maxIter);
		void SetFractalType		(FractalType fractal);

		void SetOffsetX(float value, bool isOffset = false);
		void SetOffsetY(float value, bool isOffset = false);

		void SetZoom			(float value,	bool isOffset = false);
		void SetRedModifier		(float value,	bool isOffset = false);
		void SetGreenModifier	(float value,	bool isOffset = false);
		void SetBlueModifier	(float value,	bool isOffset = false);
};

class FractalControls:
	public Misc::Noncopyable
{
	static LRESULT __stdcall ControlProc
	(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);

	static uint16_t s_instanceCount;

	WNDCLASSEX	m_windowClass;
	std::string m_className;
	HINSTANCE	m_instance;
	HWND		m_windowHandle;

	CLASS_CSTEXPR WIN_TYPE		= WS_EX_DLGMODALFRAME | WS_EX_TOPMOST | DS_MODALFRAME;
	CLASS_CSTEXPR WIN_OPTS		= WS_VISIBLE | WS_SYSMENU | WS_CAPTION | WS_POPUP;
	CLASS_CSTEXPR DLG_SIZE		= Resolution{400u, 400u};
	CLASS_CSTEXPR CLASS_NAME	= "FractalControlDialog";
	CLASS_CSTEXPR WIN_TITLE		= "Controls";

	bool CreateControlWindow();

	public:

		 FractalControls(HINSTANCE instance);
		 ~FractalControls() {};


};