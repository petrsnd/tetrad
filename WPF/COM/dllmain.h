// dllmain.h : Declaration of module class.

class CCOMModule : public ATL::CAtlDllModuleT< CCOMModule >
{
public :
	DECLARE_LIBID(LIBID_COMLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_COM, "{7AA058E4-1C5F-4D05-A4ED-2FF71B7CD657}")
};

extern class CCOMModule _AtlModule;
