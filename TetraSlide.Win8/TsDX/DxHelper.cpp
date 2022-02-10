#include "pch.h"
#include "DxHelper.h"
//#include "DxAppxFontLoader.h"
#include "RunTask.h"

#include <array>
#include <algorithm>
#include <iterator>
#include <sstream>

namespace TsDX
{

DxUtil::Ptr DxUtil::Instance()
{
    static auto inst = DxUtil::Ptr( new DxUtil() );
    return inst;
}
DxUtil::DxUtil()
{
    // Font Loader Registration
    //ThrowIfFailed( DxUtil::GetDWriteFactory()->RegisterFontFileLoader( DxAppxFontFileLoader::Instance().Get() ) );
    //ThrowIfFailed( DxUtil::GetDWriteFactory()->RegisterFontCollectionLoader( DxAppxFontCollectionLoader::Instance().Get() ) );
}
DxUtil::~DxUtil()
{
    // Font Loader Unregistration
    //ThrowIfFailed( DxUtil::GetDWriteFactory()->UnregisterFontFileLoader( DxAppxFontFileLoader::Instance().Get() ) );
    //ThrowIfFailed( DxUtil::GetDWriteFactory()->UnregisterFontCollectionLoader( DxAppxFontCollectionLoader::Instance().Get() ) );
}
// Factory Access
Microsoft::WRL::ComPtr< IDWriteFactory1 > DxUtil::GetDWriteFactory()
{
    if ( m_dwFactory == nullptr )
        ThrowIfFailed( DWriteCreateFactory( DWRITE_FACTORY_TYPE_SHARED, __uuidof( IDWriteFactory ), &m_dwFactory ) );
    return m_dwFactory;
}
Microsoft::WRL::ComPtr< ID2D1Factory1 > DxUtil::GetD2DFactory()
{
    if ( m_d2dFactory == nullptr )
        ThrowIfFailed( D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof( ID2D1Factory1 ), &m_d2dFactory ) );
    return m_d2dFactory;
}
Microsoft::WRL::ComPtr< IWICImagingFactory > DxUtil::GetWICImagingFactory()
{
    if ( m_wicImagingFactory == nullptr )
        ThrowIfFailed( CoCreateInstance( CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS( &m_wicImagingFactory ) ) );
    return m_wicImagingFactory;
}
// Direct2D
Microsoft::WRL::ComPtr< ID2D1PathGeometry > DxUtil::CreatePathGeometry()
{
    Microsoft::WRL::ComPtr< ID2D1PathGeometry > geometry;
    ThrowIfFailed( DxUtil::Instance()->GetD2DFactory()->CreatePathGeometry( &geometry ) );
    return geometry;
}
Microsoft::WRL::ComPtr< IWICBitmapSource > DxUtil::GetWICBitmapSourceFromFile( const std::wstring& fileUri )
{
    HRESULT hr = S_OK;
    auto uri = ref new Windows::Foundation::Uri( ref new Platform::String( fileUri.c_str() ) );
    auto fileOp = Windows::Storage::StorageFile::GetFileFromApplicationUriAsync( uri );
    auto storageFile = RunTask( concurrency::create_task( fileOp ), &hr );
    if ( SUCCEEDED( hr ) )
    {
        Microsoft::WRL::ComPtr< IWICBitmapDecoder > wicDecoder;
        std::wstring wzFilename( storageFile->Path->Data(), storageFile->Path->Length() );
        auto wic = DxUtil::Instance()->GetWICImagingFactory();
        hr = wic->CreateDecoderFromFilename( wzFilename.c_str(), NULL, GENERIC_READ, 
                                             WICDecodeMetadataCacheOnDemand, &wicDecoder );
        if ( SUCCEEDED( hr ) )
        {
            UINT frameCount = 0;
            if ( SUCCEEDED( wicDecoder->GetFrameCount( &frameCount ) ) )
            {
                Microsoft::WRL::ComPtr< IWICBitmapFrameDecode > frame;
                if ( SUCCEEDED( wicDecoder->GetFrame( frameCount - 1, &frame ) ) )
                {
                    Microsoft::WRL::ComPtr< IWICFormatConverter > converter;
                    if ( SUCCEEDED( DxUtil::Instance()->GetWICImagingFactory()->CreateFormatConverter( &converter ) ) )
                    {
                        if ( SUCCEEDED( converter->Initialize( frame.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone,
                                                               NULL, 0.f, WICBitmapPaletteTypeCustom ) ) )
                            return converter;
                    }
                }
            }
        }
    }
    return nullptr;
}
// DirectWrite
Microsoft::WRL::ComPtr< IDWriteFontFace > DxUtil::CreateFontFaceByName( const std::wstring& fontName,
                                                                        DWRITE_FONT_WEIGHT fontWeight,
                                                                        DWRITE_FONT_STRETCH fontStretch,
                                                                        DWRITE_FONT_STYLE fontStyle )
{
    Microsoft::WRL::ComPtr< IDWriteFontFace > dwFontFace;
    auto dwFontFile = LoadFontFromResource( fontName, fontWeight, fontStretch, fontStyle );
    if ( dwFontFile )
    {
        IDWriteFontFile* dwFontFileArray[] = { dwFontFile.Get() };
        HRESULT hr = DxUtil::GetDWriteFactory()->CreateFontFace( DWRITE_FONT_FACE_TYPE_TRUETYPE, 1, dwFontFileArray, 0,
                                                                 DWRITE_FONT_SIMULATIONS_NONE, &dwFontFace );
        if ( SUCCEEDED( hr ) )
            return dwFontFace;
    }
    auto dwFont = LoadFontFromSystem( fontName, fontWeight, fontStretch, fontStyle );
    if ( dwFont )
    {
        ThrowIfFailed( dwFont->CreateFontFace( &dwFontFace ) );
        return dwFontFace;
    }
    dwFont = LoadFontFromSystem( L"Courier New", fontWeight, fontStretch, fontStyle );
    if ( dwFont )
    {
        ThrowIfFailed( dwFont->CreateFontFace( &dwFontFace ) );
        return dwFontFace;
    }
    std::wstringstream msg;
    msg << L"Unable to locate font '" << fontName << "'";
    throw ref new Platform::Exception( E_INVALIDARG, ref new Platform::String( msg.str().c_str() ) );
}
// Private Helpers
Microsoft::WRL::ComPtr< IDWriteFontFile > DxUtil::LoadFontFromResource( const std::wstring& fontName, DWRITE_FONT_WEIGHT fontWeight, 
                                                                        DWRITE_FONT_STRETCH fontStretch, DWRITE_FONT_STYLE fontStyle )
{
    Microsoft::WRL::ComPtr< IDWriteFontFile > dwFontFile;
    if ( fontName.empty() )
        return dwFontFile;
    try
    {
        // FontLoader implementation... It doesn't work and no one at Microsoft will tell me why --Dan
        //Microsoft::WRL::ComPtr< IDWriteFontFileEnumerator > dwFontEnumerator;
        //ThrowIfFailed( DxAppxFontCollectionLoader::Instance()->CreateEnumeratorFromKey( DxUtil::Instance()->GetDWriteFactory().Get(),
        //                                                                                static_cast< const void* >( fontName.c_str() ),
        //                                                                                static_cast< UINT32 >( fontName.length() * sizeof( wchar_t ) ),
        //                                                                                &dwFontEnumerator ) );
        //BOOL hasCurrentFile = FALSE;
        //ThrowIfFailed( dwFontEnumerator->MoveNext( &hasCurrentFile ) );
        //if ( hasCurrentFile )
        //    ThrowIfFailed( dwFontEnumerator->GetCurrentFontFile( &dwFontFile ) );
        HRESULT hr = S_OK;
        auto uri = ref new Windows::Foundation::Uri( ref new Platform::String( fontName.c_str() ) );
        auto fileOp = Windows::Storage::StorageFile::GetFileFromApplicationUriAsync( uri );
        auto storageFile = RunTask( concurrency::create_task( fileOp ), &hr );
        if ( SUCCEEDED( hr ) )
        {
            hr = DxUtil::Instance()->GetDWriteFactory()->CreateFontFileReference( storageFile->Path->Data(), NULL, &dwFontFile );
            if ( FAILED( hr ) )
                dwFontFile = nullptr;
        }
    }
    catch ( Platform::COMException^ )
    {}
    catch( ... )
    {}
    return dwFontFile;
}
Microsoft::WRL::ComPtr< IDWriteFont > DxUtil::LoadFontFromSystem( const std::wstring& fontName, DWRITE_FONT_WEIGHT fontWeight, 
                                                                  DWRITE_FONT_STRETCH fontStretch, DWRITE_FONT_STYLE fontStyle )
{
    Microsoft::WRL::ComPtr< IDWriteFont > dwFont;
    if ( fontName.empty() )
        return dwFont;
    try
    {
        Microsoft::WRL::ComPtr< IDWriteFontCollection > dwFontCollection;
        ThrowIfFailed( DxUtil::Instance()->GetDWriteFactory()->GetSystemFontCollection( &dwFontCollection, FALSE ) );
        UINT32 idx = 0;
        BOOL found = FALSE;
        ThrowIfFailed( dwFontCollection->FindFamilyName( fontName.c_str(), &idx, &found ) );
        Microsoft::WRL::ComPtr< IDWriteFontFamily > dwFontFamily;
        ThrowIfFailed( dwFontCollection->GetFontFamily( idx, &dwFontFamily ) );
        ThrowIfFailed( dwFontFamily->GetFirstMatchingFont( fontWeight, fontStretch, fontStyle, &dwFont ) );
    }
    catch ( Platform::COMException^ )
    {}
    catch ( ... )
    {}
    return dwFont;
}


void ThrowIfFailed( HRESULT hr )
{
    if ( FAILED( hr ) )
        throw ref new Platform::COMException( hr );
}

// Conversions
FLOAT ConvertFontSizeToDIP( FLOAT fontSize )
{
    return ( fontSize / 72.0f ) * 96.0f;
}
RECT ConvertFoundationRectToNativeRect( Windows::Foundation::Rect rect )
{
    RECT rectNative = { 0 };
    rectNative.top = static_cast< LONG >( rect.Top );
    rectNative.left = static_cast< LONG >( rect.Left );
    rectNative.bottom = static_cast< LONG >( rect.Bottom );
    rectNative.right = static_cast< LONG >( rect.Right );
    return rectNative;
}
D2D1_COLOR_F ConvertUiColorToD2D1ColorF( Windows::UI::Color color )
{
    D2D1_COLOR_F colorf;
    colorf.a = color.A / 255.0f;
    colorf.r = color.R / 255.0f;
    colorf.g = color.G / 255.0f;
    colorf.b = color.B / 255.0f;
    return colorf;
}
// Direct2D
// DirectWrite
std::vector< UINT16 > GetGlyphIndicesForFontFace( Microsoft::WRL::ComPtr< IDWriteFontFace >& dwFontFace, const std::wstring& text )
{
    std::vector< UINT > codePoints;
    // TODO: replace below with UTF16 to UTF32 conversion for code points
    std::transform( std::begin( text ), std::end( text ), std::back_inserter( codePoints ), []( const wchar_t c ) -> UINT
        { return static_cast< UINT >( c ); } );
    std::vector< UINT16 > glyphIndices( codePoints.size(), 0 );
    dwFontFace->GetGlyphIndices( &codePoints[0], static_cast< UINT32 >( codePoints.size() ), &glyphIndices[0] );
    return glyphIndices;
}

} // TsDX
