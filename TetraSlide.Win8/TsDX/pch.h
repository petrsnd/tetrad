//
// pch.h
// Header for standard system include files.
//

#pragma once

#include <winerror.h>
#include <d3d11_1.h>
#include <d2d1_1.h>
#include <d2d1helper.h>
#include <dwrite_1.h>
#include <Wincodec.h>
#include <wrl.h>
#include <windows.ui.xaml.media.dxinterop.h>

#include <ppltasks.h>

// If you don't put this in here, then the xamltypeinfo.g.cpp source complains  that it cannot find your control
#include "GameText.h"
#include "GameBackground.h"

#include "DxHelper.h"
