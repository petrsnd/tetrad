#pragma once

#include "pch.h"
#include "DirectWriteImageSource.h"
#include "CppDependencyProperty.h"

#include <functional>


namespace TsDX
{

[Windows::Foundation::Metadata::WebHostHidden]
public ref class GameText sealed : public Windows::UI::Xaml::Controls::Control
{
    DEFINE_DEPENDENCY_PROPERTY( Platform::String^, Text )
    DEFINE_DEPENDENCY_PROPERTY( Platform::String^, Font )
    DEFINE_DEPENDENCY_PROPERTY( Windows::UI::Color, OutlineColor )
    DEFINE_DEPENDENCY_PROPERTY( Windows::UI::Color, FillColor )
    DEFINE_DEPENDENCY_PROPERTY( int, StrokeWidth )
public:
    GameText();

    virtual void OnApplyTemplate() override;
    virtual Windows::Foundation::Size MeasureOverride( Windows::Foundation::Size size ) override;
    virtual Windows::Foundation::Size ArrangeOverride( Windows::Foundation::Size size ) override;

private:
    void OnMapSurfaceSizeChanged( Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e );

    void PrepareTextResources();
    void CreateSurface();
    void Draw();
    void LoadFontAsync();
    void TriggerRedraw();

    Windows::UI::Xaml::Controls::Border^ m_borderElement;
    DirectWriteImageSource^ m_dwImageSource;
    Windows::Foundation::EventRegistrationToken m_sizeChangedToken;

    Microsoft::WRL::ComPtr< IDWriteFontFace > m_dwFontFace;
    Microsoft::WRL::ComPtr< ID2D1PathGeometry > m_textGeometry;
    Windows::Foundation::Size m_textSize;
};

} // TsDX
