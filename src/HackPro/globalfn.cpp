#include "globalfn.h"
#include "stdafx.h"

typedef struct {
    USHORT Length;
    USHORT MaxLen;
    USHORT *Buffer;
} UNICODE_STRING;




struct process_info {
    ULONG NextEntryDelta;
    ULONG ThreadCount;
    ULONG Reserved1[6];
    LARGE_INTEGER CreateTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER KernelTime;
    UNICODE_STRING ProcessName;
    ULONG BasePriority;
    ULONG ProcessId;
    // etc.
};

typedef unsigned long (__stdcall *NtQuerySystemInformation)(
		IN ULONG SysInfoClass,
                IN OUT PVOID SystemInformation,
                IN ULONG SystemInformationLength,
                OUT PULONG RetLen
                );
typedef LONG (__stdcall *RtlCompareUnicodeString)(IN UNICODE_STRING *,IN UNICODE_STRING *,IN ULONG CaseInsensitve);

__int32 GetIpVal(BYTE f1,BYTE f2,BYTE f3,BYTE f4)
{
			__int32 a=255*255*255*f1+255*255*f2+255*f3+f4;
			return a;
}

void IncrIp(BYTE *f1,BYTE *f2,BYTE *f3,BYTE *f4)
{
		if((*f4)<255)
		{
			(*f4)++;
		}
		else
		{
			if((*f3)<255)
			{
				*f4=0;
				(*f3)++;
			}
			else
			{
				if((*f2)<255)
				{
					(*f4)=0;
					(*f3)=0;
					(*f2)++;
				}
				else
				{
					if((*f1)<255)
					{
						*f4=0;*f3=0;*f2=0;(*f1)++;
					}
					else
					{
						*f4=0;*f3=0;*f2=0;*f1=0;
					}
				}
			}
		}


}
	//============**************************==========================********************
int find_pid (DWORD *ppid,char* progname)
{
    HINSTANCE hNtDll;
    unsigned long rc;
    ULONG ulNeed = 0;
    void *buf = NULL;
    size_t len = 0;
    int ret = 0;

    hNtDll = LoadLibrary ("NTDLL");
    if (!hNtDll)
        return 0;

   NtQuerySystemInformation pNtQuerySystemInformation = (NtQuerySystemInformation)GetProcAddress (hNtDll, "NtQuerySystemInformation");
    if (!pNtQuerySystemInformation)
        return 0;

   RtlCompareUnicodeString pRtlCompareUnicodeString = (RtlCompareUnicodeString)GetProcAddress (hNtDll, "RtlCompareUnicodeString");
    if (!pRtlCompareUnicodeString)
        return 0;

    do {
        len += 2000;
        buf = realloc (buf, len);
        if (!buf)
            return 0;
        rc = pNtQuerySystemInformation (5, buf, len, &ulNeed);
    } while (rc == 0xc0000004);  // STATUS_INFO_LEN_MISMATCH

    if (rc <0) {
        free (buf);
        return 0;
    }

    //
    // Ok, now slog through the data looking for our guy.
    //
    {
        process_info *p = (process_info *)buf;
        int done = 0;
        UNICODE_STRING lsass = { 18, 20, (USHORT*)progname };

        while (!done) {
            if ((p->ProcessName.Buffer!= 0)
                && (pRtlCompareUnicodeString (&lsass, &p->ProcessName, 1) == 0)) {
                    *ppid = p->ProcessId;
                    ret = 1;
                    goto exit;
            }
            done = p->NextEntryDelta == 0;
            p = (struct process_info *)(((char *)p) + p->NextEntryDelta);
        }
    }

 exit:
    free (buf);
    FreeLibrary (hNtDll);

    return ret;
}
