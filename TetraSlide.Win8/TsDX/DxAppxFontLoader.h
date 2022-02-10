#pragma once

#include "pch.h"
#include "TsIUnknownImpl.h"

namespace TsDX
{

class DxAppxFontFileLoader : public TsDx::TsIUnknownImpl< IDWriteFontFileLoader >
{
public:
    static Microsoft::WRL::ComPtr< IDWriteFontFileLoader > Instance();
private:
    DxAppxFontFileLoader();
public:
    virtual ~DxAppxFontFileLoader();
    STDMETHOD( CreateStreamFromKey )( _In_reads_bytes_( fontFileReferenceKeySize ) void const* fontFileReferenceKey,
                                      UINT32 fontFileReferenceKeySize,
                                      _Out_ IDWriteFontFileStream** fontFileStream );
};

class DxAppxFontCollectionLoader : public TsDx::TsIUnknownImpl< IDWriteFontCollectionLoader >
{
public:
    static Microsoft::WRL::ComPtr< IDWriteFontCollectionLoader > Instance();
private:
    DxAppxFontCollectionLoader();
public:
    virtual ~DxAppxFontCollectionLoader();
    STDMETHOD( CreateEnumeratorFromKey )( IDWriteFactory* factory,
                                          _In_reads_bytes_( collectionKeySize ) void const* collectionKey,
                                          UINT32 collectionKeySize,
                                          _Out_ IDWriteFontFileEnumerator** fontFileEnumerator );
};

} // TsDX
