#pragma once

#include "Misc.h"

class WinapiApp
{
	std::string m_className;
	WNDCLASSEX  m_windowClass;
	HWND		m_handle;
	MSG			m_msg;

	struct Icon 
	{
		std::string m_src;
		HICON		m_iconBig;
		HICON		m_iconSmall;

		explicit Icon();
	};

	struct Cursor
	{
		std::string m_src;
		HCURSOR		m_cursorHandle;

		explicit Cursor();
	};
	
	Icon	m_Icon;
	Cursor	m_Cursor;

	math::Vector2<uint16>	m_resolution;
	std::string				m_title;

	public:

		WinapiApp();
};