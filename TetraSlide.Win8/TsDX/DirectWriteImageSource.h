#pragma once

#include "pch.h"
#include "DxImageSourceBase.h"

namespace TsDX
{

    public ref class DirectWriteImageSource sealed : DxImageSourceBase
    {
    internal:
        DirectWriteImageSource( int pixelWidth, int pixelHeight, bool isOpaque,
                                Microsoft::WRL::ComPtr< ID2D1PathGeometry >& textGeometry, float textWidth, float textHeight );

    public:
        void BeginDraw();
        void RenderText( Windows::UI::Color outlineBrush, Windows::UI::Color fillBrush, float strokeWidth );

    private:
        Microsoft::WRL::ComPtr< ID2D1PathGeometry > m_textGeometry;

        float m_textWidth;
        float m_textHeight;
    };

} // TsDX
