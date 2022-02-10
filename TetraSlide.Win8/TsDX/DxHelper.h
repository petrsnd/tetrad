#pragma once

#include "pch.h"
#include <memory>
#include <string>
#include <vector>

namespace TsDX
{

    class DxUtil
    {
    public:
        typedef std::shared_ptr< DxUtil > Ptr;
        static DxUtil::Ptr Instance();
        ~DxUtil();

        // Factory Access
        Microsoft::WRL::ComPtr< IDWriteFactory1 > GetDWriteFactory();
        Microsoft::WRL::ComPtr< ID2D1Factory1 > GetD2DFactory();
        Microsoft::WRL::ComPtr< IWICImagingFactory > GetWICImagingFactory();

        // Direct2D
        Microsoft::WRL::ComPtr< ID2D1PathGeometry > CreatePathGeometry();
        Microsoft::WRL::ComPtr< IWICBitmapSource > GetWICBitmapSourceFromFile( const std::wstring& fileUri );

        // DirectWrite
        Microsoft::WRL::ComPtr< IDWriteFontFace > CreateFontFaceByName( const std::wstring& fontName,
                                                                        DWRITE_FONT_WEIGHT fontWeight = DWRITE_FONT_WEIGHT_NORMAL,
                                                                        DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL,
                                                                        DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL );
    private:
        DxUtil();

        Microsoft::WRL::ComPtr< IDWriteFontFile > LoadFontFromResource( const std::wstring& fontName, DWRITE_FONT_WEIGHT fontWeight,
                                                                        DWRITE_FONT_STRETCH fontStretch, DWRITE_FONT_STYLE fontStyle );
        Microsoft::WRL::ComPtr< IDWriteFont > LoadFontFromSystem( const std::wstring& fontName, DWRITE_FONT_WEIGHT fontWeight,
                                                                  DWRITE_FONT_STRETCH fontStretch, DWRITE_FONT_STYLE fontStyle );

        Microsoft::WRL::ComPtr< IDWriteFactory1 > m_dwFactory;
        Microsoft::WRL::ComPtr< ID2D1Factory1 > m_d2dFactory;
        Microsoft::WRL::ComPtr< IWICImagingFactory2 > m_wicImagingFactory;
    };

    void ThrowIfFailed( HRESULT hr );

    // Conversions
    FLOAT ConvertFontSizeToDIP( FLOAT points );
    RECT ConvertFoundationRectToNativeRect( Windows::Foundation::Rect rect );
    D2D1_COLOR_F ConvertUiColorToD2D1ColorF( Windows::UI::Color color );

    // Direct2D


    // DirectWrite
    std::vector< UINT16 > GetGlyphIndicesForFontFace( Microsoft::WRL::ComPtr< IDWriteFontFace >& font, const std::wstring& text );

} // TsDX
