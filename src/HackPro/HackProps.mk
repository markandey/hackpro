
HackProps.dll: dlldata.obj HackPro_p.obj HackPro_i.obj
	link /dll /out:HackProps.dll /def:HackProps.def /entry:DllMain dlldata.obj HackPro_p.obj HackPro_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \
.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del HackProps.dll
	@del HackProps.lib
	@del HackProps.exp
	@del dlldata.obj
	@del HackPro_p.obj
	@del HackPro_i.obj
