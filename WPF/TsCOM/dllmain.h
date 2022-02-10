// dllmain.h : Declaration of module class.

class CTsCOMModule : public ATL::CAtlDllModuleT< CTsCOMModule >
{
public :
	DECLARE_LIBID(LIBID_TsCOMLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_TSCOM, "{2B399E31-20D5-4FB1-8D59-030E734C184A}")
};

extern class CTsCOMModule _AtlModule;
