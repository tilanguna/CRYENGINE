// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#if defined(INCLUDE_OPENVR_SDK)

namespace CryVR
{
namespace OpenVR
{
class Device;

class Resources
{
public:
	static Device* GetAssociatedDevice() { return ms_pRes ? ms_pRes->m_pDevice : 0; }

	static void    Init();
	static void    PostInit();
	static void    Shutdown();

private:
	Resources();
	~Resources();

private:
	static Resources* ms_pRes;
	static bool       ms_libInitialized;

private:
	Device* m_pDevice;     // Attached device (only one supported at the moment)
};
} // namespace OpenVR
} // namespace CryVR

#endif               //defined(INCLUDE_OPENVR_SDK)
