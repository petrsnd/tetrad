#pragma once

#include "pch.h"
#include "DxTiledBitmapImageSource.h"
#include "CppDependencyProperty.h"

#include <functional>


namespace TsDX
{

[Windows::Foundation::Metadata::WebHostHidden]
public ref class GameBackground sealed : public Windows::UI::Xaml::Controls::Control
{
    DEFINE_DEPENDENCY_PROPERTY( Platform::String^, Image )
public:
    GameBackground();

    virtual void OnApplyTemplate() override;
    virtual Windows::Foundation::Size MeasureOverride( Windows::Foundation::Size size ) override;
    virtual Windows::Foundation::Size ArrangeOverride( Windows::Foundation::Size size ) override;

private:
    void OnMapSurfaceSizeChanged( Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e );

    void CreateSurface();
    void Draw();
    void LoadImageAsync();
    void TriggerRedraw();

    Windows::UI::Xaml::Controls::Border^ m_borderElement;
    DxTiledBitmapImageSource^ m_tiledImageSource;
    Windows::Foundation::EventRegistrationToken m_sizeChangedToken;

    Microsoft::WRL::ComPtr< IWICBitmapSource > m_wicBitmapSource;
};

} // TsDX
