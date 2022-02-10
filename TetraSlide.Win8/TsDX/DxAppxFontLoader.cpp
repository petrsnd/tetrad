#include "pch.h"
#include "DxAppxFontLoader.h"
#include "RunTask.h"

#include <string>
#include <vector>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

namespace TsDX
{

class DxAppxFontFileStream : public TsDx::TsIUnknownImpl< IDWriteFontFileStream >
{
public:
    DxAppxFontFileStream( const std::wstring& appxUrl ) :
        m_appxUrl( appxUrl )
    {}
    virtual ~DxAppxFontFileStream()
    {}
    HRESULT Initialize()
    {
        HRESULT hr = S_OK;
        auto uri = ref new Windows::Foundation::Uri( ref new Platform::String( m_appxUrl.c_str() ) );
        auto fileOp = Windows::Storage::StorageFile::GetFileFromApplicationUriAsync( uri );
        m_file = RunTask( concurrency::create_task( fileOp ), &hr );
        if ( SUCCEEDED( hr ) )
        {
            auto propsOp = m_file->GetBasicPropertiesAsync();
            m_props = RunTask( concurrency::create_task( propsOp ), &hr );
            if ( SUCCEEDED( hr ) )
            {
                auto readOp = Windows::Storage::FileIO::ReadBufferAsync( m_file );
                auto buffer = RunTask( concurrency::create_task( readOp ), &hr );
                if ( SUCCEEDED( hr ) )
                {
                    auto reader = Windows::Storage::Streams::DataReader::FromBuffer( buffer );
                    while ( reader->UnconsumedBufferLength )
                        m_bytes.push_back( reader->ReadByte() );
                }
            }
        }
        return hr;
    }
    STDMETHOD( ReadFileFragment )( _Outptr_result_bytebuffer_( fragmentSize ) void const** fragmentStart,
                                   UINT64 fileOffset,
                                   UINT64 fragmentSize,
                                   _Out_ void** fragmentContext )
    {
        if ( fileOffset <= m_bytes.size() && fragmentSize <= m_bytes.size() - fileOffset )
        {
            *fragmentStart = static_cast< BYTE const* >( &m_bytes[static_cast< size_t >( fileOffset )] );
            *fragmentContext = NULL;
            return S_OK;
        }
        else
        {
            *fragmentStart = NULL;
            *fragmentContext = NULL;
            return E_FAIL;
        }
    }
    STDMETHOD_( void, ReleaseFileFragment )( void* fragmentContext )
    {}
    STDMETHOD( GetFileSize )( _Out_ UINT64* fileSize )
    {
        if ( m_props )
        {
            *fileSize = m_props->Size;
            return S_OK;
        }
        return OLE_E_BLANK;
    }
    STDMETHOD( GetLastWriteTime )( _Out_ UINT64* lastWriteTime )
    {
        if ( m_props )
        {
            *lastWriteTime = m_props->DateModified.UniversalTime;
            return S_OK;
        }
        return OLE_E_BLANK;
    }
private:
    std::wstring m_appxUrl;
    Windows::Storage::StorageFile^ m_file;
    Windows::Storage::FileProperties::BasicProperties^ m_props;
    std::vector< unsigned char > m_bytes;
};

class DxAppxFontFileEnumerator : public TsDx::TsIUnknownImpl< IDWriteFontFileEnumerator >
{
public:
    explicit DxAppxFontFileEnumerator( IDWriteFactory* factory ) :
        m_factory( factory ),
        m_nextIndex( 0 )
    {}
    virtual ~DxAppxFontFileEnumerator()
    {}
    HRESULT Initialize( const std::wstring& collectionStr )
    {
        m_appxUrls.clear();
        boost::algorithm::split( m_appxUrls, collectionStr, boost::is_any_of( L"\t " ), boost::token_compress_on );
        return S_OK;
    }
    STDMETHOD( MoveNext )( _Out_ BOOL* hasCurrentFile )
    {
        HRESULT hr = S_OK;

        *hasCurrentFile = FALSE;
        m_currentFile = nullptr;

        if ( m_nextIndex < m_appxUrls.size() )
        {
            hr = m_factory->CreateCustomFontFileReference( m_appxUrls[m_nextIndex].c_str(), static_cast< UINT32 >( m_appxUrls[m_nextIndex].length() * sizeof( wchar_t ) ),
                                                           DxAppxFontFileLoader::Instance().Get(), &m_currentFile );
            if ( SUCCEEDED( hr ) )
            {
                *hasCurrentFile = TRUE;
                m_nextIndex++;
            }
        }

        return hr;
    }
    STDMETHOD( GetCurrentFontFile )( _Out_ IDWriteFontFile** fontFile )
    {
        *fontFile = TsDx::SafeAcquire( m_currentFile.Get() );
        return ( m_currentFile != NULL ) ? S_OK : E_FAIL;
    }
private:
    Microsoft::WRL::ComPtr< IDWriteFactory > m_factory;
    std::vector< std::wstring> m_appxUrls;
    size_t m_nextIndex;
     Microsoft::WRL::ComPtr< IDWriteFontFile > m_currentFile;
};



Microsoft::WRL::ComPtr< IDWriteFontFileLoader > DxAppxFontFileLoader::Instance()
{
    static auto inst = Microsoft::WRL::ComPtr< IDWriteFontFileLoader >( new DxAppxFontFileLoader() );
    return inst;
}
DxAppxFontFileLoader::DxAppxFontFileLoader()
{}
DxAppxFontFileLoader::~DxAppxFontFileLoader()
{}
HRESULT STDMETHODCALLTYPE DxAppxFontFileLoader::CreateStreamFromKey( _In_reads_bytes_( fontFileReferenceKeySize ) void const* fontFileReferenceKey,
                                                                     UINT32 fontFileReferenceKeySize,
                                                                     _Out_ IDWriteFontFileStream** fontFileStream )
{
    *fontFileStream = NULL;
    HRESULT hr = S_OK;

    std::wstring appxUrl( static_cast< wchar_t const* >( fontFileReferenceKey ), fontFileReferenceKeySize / sizeof( wchar_t ) );
    DxAppxFontFileStream* stream = new( std::nothrow ) DxAppxFontFileStream( appxUrl );
    if ( stream == NULL )
        return E_OUTOFMEMORY;

    hr = stream->Initialize();
    if ( FAILED( hr ) )
        delete stream;
    else
        *fontFileStream = TsDx::SafeAcquire( stream );

    return hr;
}



Microsoft::WRL::ComPtr< IDWriteFontCollectionLoader > DxAppxFontCollectionLoader::Instance()
{
    static auto inst = Microsoft::WRL::ComPtr< IDWriteFontCollectionLoader >( new DxAppxFontCollectionLoader() );
    return inst;
}
DxAppxFontCollectionLoader::DxAppxFontCollectionLoader()
{}
DxAppxFontCollectionLoader::~DxAppxFontCollectionLoader()
{}
HRESULT STDMETHODCALLTYPE DxAppxFontCollectionLoader::CreateEnumeratorFromKey( IDWriteFactory* factory,
                                                                               _In_reads_bytes_( collectionKeySize ) void const* collectionKey,
                                                                               UINT32 collectionKeySize,
                                                                               _Out_ IDWriteFontFileEnumerator** fontFileEnumerator )
{
    *fontFileEnumerator = NULL;
    HRESULT hr = S_OK;

    DxAppxFontFileEnumerator* enumerator = new( std::nothrow ) DxAppxFontFileEnumerator( factory );
    if ( enumerator == NULL )
        return E_OUTOFMEMORY;

    std::wstring collectionStr( static_cast< wchar_t const* >( collectionKey ), collectionKeySize / sizeof( wchar_t ) );
    hr = enumerator->Initialize( collectionStr );
    if ( FAILED( hr ) )
        delete enumerator;
    else
        *fontFileEnumerator = TsDx::SafeAcquire( enumerator );

    return hr;
}

} // TsDX
