#include "scriptfile.h"

#define UNSAFE_UTF8_ALLOWED
#include <unicode/utf8converter.h>
#undef UNSAFE_UTF8_ALLOWED

#include <new>
#include <assert.h>
#include <string>
#include <string.h>
#include <stdio.h>

#ifdef _WIN32_WCE
#include <windows.h> // For GetModuleFileName
#ifdef GetObject
#undef GetObject
#endif
#endif

using namespace std;

BEGIN_AS_NAMESPACE

CScriptFile *ScriptFile_Factory()
{
    return new CScriptFile();
}

void ScriptFile_Factory_Generic(asIScriptGeneric *gen)
{
	*(CScriptFile**)gen->GetAddressOfReturnLocation()	= ScriptFile_Factory();
}

void ScriptFile_AddRef_Generic(asIScriptGeneric *gen)
{
	CScriptFile *file = (CScriptFile*)gen->GetObject();
	file->AddRef();
}

void ScriptFile_Release_Generic(asIScriptGeneric *gen)
{
	CScriptFile *file = (CScriptFile*)gen->GetObject();
	file->Release();
}

void ScriptFile_Open_Generic(asIScriptGeneric *gen)
{
	CScriptFile *file = (CScriptFile*)gen->GetObject();
	std::wstring *f = static_cast<std::wstring*>(gen->GetArgAddress(0));
	std::wstring *m = static_cast<std::wstring*>(gen->GetArgAddress(1));
	int r = file->Open(*f, *m);
	gen->SetReturnDWord(r);
}

void ScriptFile_Close_Generic(asIScriptGeneric *gen)
{
	CScriptFile *file = (CScriptFile*)gen->GetObject();
	int r = file->Close();
	gen->SetReturnDWord(r);
}

void ScriptFile_GetSize_Generic(asIScriptGeneric *gen)
{
	CScriptFile *file = (CScriptFile*)gen->GetObject();
	int r = file->GetSize();
	gen->SetReturnDWord(r);
}

void ScriptFile_ReadString_Generic(asIScriptGeneric *gen)
{
	CScriptFile *file = (CScriptFile*)gen->GetObject();
	int len = gen->GetArgDWord(0);
	std::wstring *str = static_cast<std::wstring*>(gen->GetArgAddress(1));
	len = file->ReadString(len, *str);
	gen->SetReturnDWord(len);
}

void ScriptFile_ReadLine_Generic(asIScriptGeneric *gen)
{
	CScriptFile *file = (CScriptFile*)gen->GetObject();
	std::wstring *str = static_cast<std::wstring*>(gen->GetArgAddress(0));
	int len = file->ReadLine(*str);
	gen->SetReturnDWord(len);
}

void ScriptFile_ReadInt_Generic(asIScriptGeneric *gen)
{
	CScriptFile *file = (CScriptFile*)gen->GetObject();
	asUINT bytes = *(asUINT*)gen->GetAddressOfArg(0);
	*(asINT64*)gen->GetAddressOfReturnLocation() = file->ReadInt(bytes);
}

void ScriptFile_ReadUInt_Generic(asIScriptGeneric *gen)
{
	CScriptFile *file = (CScriptFile*)gen->GetObject();
	asUINT bytes = *(asUINT*)gen->GetAddressOfArg(0);
	*(asQWORD*)gen->GetAddressOfReturnLocation() = file->ReadUInt(bytes);
}

void ScriptFile_ReadFloat_Generic(asIScriptGeneric *gen)
{
	CScriptFile *file = (CScriptFile*)gen->GetObject();
	*(float*)gen->GetAddressOfReturnLocation() = file->ReadFloat();
}

void ScriptFile_ReadDouble_Generic(asIScriptGeneric *gen)
{
	CScriptFile *file = (CScriptFile*)gen->GetObject();
	*(double*)gen->GetAddressOfReturnLocation() = file->ReadDouble();
}

void ScriptFile_WriteString_Generic(asIScriptGeneric *gen)
{
	CScriptFile *file = (CScriptFile*)gen->GetObject();
	std::wstring *str = static_cast<std::wstring*>(gen->GetArgAddress(0));
	gen->SetReturnDWord(file->WriteString(*str));
}

void ScriptFile_WriteInt_Generic(asIScriptGeneric *gen)
{
	CScriptFile *file = (CScriptFile*)gen->GetObject();
	asINT64 val = *(asINT64*)gen->GetAddressOfArg(0);
	asUINT bytes = *(asUINT*)gen->GetAddressOfArg(1);
	*(int*)gen->GetAddressOfReturnLocation() = file->WriteInt(val, bytes);
}

void ScriptFile_WriteUInt_Generic(asIScriptGeneric *gen)
{
	CScriptFile *file = (CScriptFile*)gen->GetObject();
	asQWORD val = *(asQWORD*)gen->GetAddressOfArg(0);
	asUINT bytes = *(asUINT*)gen->GetAddressOfArg(1);
	*(int*)gen->GetAddressOfReturnLocation() = file->WriteUInt(val, bytes);
}

void ScriptFile_WriteFloat_Generic(asIScriptGeneric *gen)
{
	CScriptFile *file = (CScriptFile*)gen->GetObject();
	float val = *(float*)gen->GetAddressOfArg(0);
	*(int*)gen->GetAddressOfReturnLocation() = file->WriteFloat(val);
}

void ScriptFile_WriteDouble_Generic(asIScriptGeneric *gen)
{
	CScriptFile *file = (CScriptFile*)gen->GetObject();
	double val = *(double*)gen->GetAddressOfArg(0);
	*(int*)gen->GetAddressOfReturnLocation() = file->WriteDouble(val);
}

void ScriptFile_IsEOF_Generic(asIScriptGeneric *gen)
{
	CScriptFile *file = (CScriptFile*)gen->GetObject();
	bool r = file->IsEOF();
	gen->SetReturnByte(r);
}

void ScriptFile_GetPos_Generic(asIScriptGeneric *gen)
{
	CScriptFile *file = (CScriptFile*)gen->GetObject();
	gen->SetReturnDWord(file->GetPos());
}

void ScriptFile_SetPos_Generic(asIScriptGeneric *gen)
{
	CScriptFile *file = (CScriptFile*)gen->GetObject();
	int pos = (int)gen->GetArgDWord(0);
	gen->SetReturnDWord(file->SetPos(pos));
}

void ScriptFile_MovePos_Generic(asIScriptGeneric *gen)
{
	CScriptFile *file = (CScriptFile*)gen->GetObject();
	int delta = (int)gen->GetArgDWord(0);
	gen->SetReturnDWord(file->MovePos(delta));
}

void RegisterScriptFile_Native(asIScriptEngine *engine)
{
    int r;

    r = engine->RegisterObjectType("file", 0, asOBJ_REF); assert( r >= 0 );
    r = engine->RegisterObjectBehaviour("file", asBEHAVE_FACTORY, "file @f()", asFUNCTION(ScriptFile_Factory), asCALL_CDECL); assert( r >= 0 );
    r = engine->RegisterObjectBehaviour("file", asBEHAVE_ADDREF, "void f()", asMETHOD(CScriptFile,AddRef), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectBehaviour("file", asBEHAVE_RELEASE, "void f()", asMETHOD(CScriptFile,Release), asCALL_THISCALL); assert( r >= 0 );

    r = engine->RegisterObjectMethod("file", "int open(const string &in, const string &in)", asMETHOD(CScriptFile,Open), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("file", "int close()", asMETHOD(CScriptFile,Close), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("file", "int getSize() const", asMETHOD(CScriptFile,GetSize), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("file", "bool isEndOfFile() const", asMETHOD(CScriptFile,IsEOF), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("file", "int readString(uint, string &out)", asMETHOD(CScriptFile,ReadString), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("file", "int readLine(string &out)", asMETHOD(CScriptFile,ReadLine), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("file", "int64 readInt(uint)", asMETHOD(CScriptFile,ReadInt), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("file", "uint64 readUInt(uint)", asMETHOD(CScriptFile,ReadUInt), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("file", "float readFloat()", asMETHOD(CScriptFile,ReadFloat), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("file", "double readDouble()", asMETHOD(CScriptFile,ReadDouble), asCALL_THISCALL); assert( r >= 0 );
#if AS_WRITE_OPS == 1
	r = engine->RegisterObjectMethod("file", "int writeString(const string &in)", asMETHOD(CScriptFile,WriteString), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("file", "int writeInt(int64, uint)", asMETHOD(CScriptFile,WriteInt), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("file", "int writeUInt(uint64, uint)", asMETHOD(CScriptFile,WriteUInt), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("file", "int writeFloat(float)", asMETHOD(CScriptFile,WriteFloat), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("file", "int writeDouble(double)", asMETHOD(CScriptFile,WriteDouble), asCALL_THISCALL); assert( r >= 0 );
#endif
	r = engine->RegisterObjectMethod("file", "int getPos() const", asMETHOD(CScriptFile,GetPos), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("file", "int setPos(int)", asMETHOD(CScriptFile,SetPos), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("file", "int movePos(int)", asMETHOD(CScriptFile,MovePos), asCALL_THISCALL); assert( r >= 0 );

	r = engine->RegisterObjectProperty("file", "bool mostSignificantByteFirst", offsetof(PODCScriptFile, mostSignificantByteFirst)); assert( r >= 0 );
}

void RegisterScriptFile_Generic(asIScriptEngine *engine)
{
	int r;

	r = engine->RegisterObjectType("file", 0, asOBJ_REF); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("file", asBEHAVE_FACTORY, "file @f()", asFUNCTION(ScriptFile_Factory_Generic), asCALL_GENERIC); assert( r >= 0 );
    r = engine->RegisterObjectBehaviour("file", asBEHAVE_ADDREF, "void f()", asFUNCTION(ScriptFile_AddRef_Generic), asCALL_GENERIC); assert( r >= 0 );
    r = engine->RegisterObjectBehaviour("file", asBEHAVE_RELEASE, "void f()", asFUNCTION(ScriptFile_Release_Generic), asCALL_GENERIC); assert( r >= 0 );

    r = engine->RegisterObjectMethod("file", "int open(const string &in, const string &in)", asFUNCTION(ScriptFile_Open_Generic), asCALL_GENERIC); assert( r >= 0 );
    r = engine->RegisterObjectMethod("file", "int close()", asFUNCTION(ScriptFile_Close_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("file", "int getSize() const", asFUNCTION(ScriptFile_GetSize_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("file", "bool isEndOfFile() const", asFUNCTION(ScriptFile_IsEOF_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("file", "int readString(uint, string &out)", asFUNCTION(ScriptFile_ReadString_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("file", "int readLine(string &out)", asFUNCTION(ScriptFile_ReadLine_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("file", "int64 readInt(uint)", asFUNCTION(ScriptFile_ReadInt_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("file", "uint64 readUInt(uint)", asFUNCTION(ScriptFile_ReadUInt_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("file", "float readFloat()", asFUNCTION(ScriptFile_ReadFloat_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("file", "double readDouble()", asFUNCTION(ScriptFile_ReadDouble_Generic), asCALL_GENERIC); assert( r >= 0 );
#if AS_WRITE_OPS == 1
	r = engine->RegisterObjectMethod("file", "int writeString(const string &in)", asFUNCTION(ScriptFile_WriteString_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("file", "int writeInt(int64, uint)", asFUNCTION(ScriptFile_WriteInt_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("file", "int writeUInt(uint64, uint)", asFUNCTION(ScriptFile_WriteUInt_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("file", "int writeFloat(float)", asFUNCTION(ScriptFile_WriteFloat_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("file", "int writeDouble(double)", asFUNCTION(ScriptFile_WriteDouble_Generic), asCALL_GENERIC); assert( r >= 0 );
#endif
	r = engine->RegisterObjectMethod("file", "int getPos() const", asFUNCTION(ScriptFile_GetPos_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("file", "int setPos(int)", asFUNCTION(ScriptFile_SetPos_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("file", "int movePos(int)", asFUNCTION(ScriptFile_MovePos_Generic), asCALL_GENERIC); assert( r >= 0 );

	r = engine->RegisterObjectProperty("file", "bool mostSignificantByteFirst", offsetof(PODCScriptFile, mostSignificantByteFirst)); assert( r >= 0 );
}

void RegisterScriptFile(asIScriptEngine *engine)
{
	if( strstr(asGetLibraryOptions(), "AS_MAX_PORTABILITY") )
		RegisterScriptFile_Generic(engine);
	else
		RegisterScriptFile_Native(engine);
}

CScriptFile::CScriptFile()
{
    refCount = 1;
    file = 0;
	mostSignificantByteFirst = false;
}

CScriptFile::~CScriptFile()
{
    Close();
}

void CScriptFile::AddRef() const
{
    ++refCount;
}

void CScriptFile::Release() const
{
    if( --refCount == 0 )
        delete this;
}

int CScriptFile::Open(const std::wstring &filename, const std::wstring &mode)
{
    // Close the previously opened file handle
    if( file )
        Close();

    std::wstring myFilename = filename;

    // Validate the mode
	wstring m;
#if AS_WRITE_OPS == 1
    if( mode != L"r" && mode != L"w" && mode != L"a" )
#else
	if( mode != L"r" )
#endif
        return -1;
	else
		m = mode;

#ifdef _WIN32_WCE
    // no relative pathing on CE
    wchar_t buf[MAX_PATH];
    static TCHAR apppath[MAX_PATH] = TEXT("");
    if (!apppath[0])
    {
        GetModuleFileName(NULL, apppath, MAX_PATH);
        
        int appLen = _tcslen(apppath);
        while (appLen > 1)
        {
            if (apppath[appLen-1] == TEXT('\\'))
                break;
            appLen--;
        }

        // Terminate the string after the trailing backslash
        apppath[appLen] = TEXT('\0');
    }
#ifdef _UNICODE
    #pragma warning("Program supports Unicode compilation only") 
#else
    memcpy(buf, apppath, strlen(apppath));
#endif
    myFilename = buf + myFilename;
#endif


	// By default windows translates "\r\n" to "\n", but we want to read the file as-is.
	m += L"b";

    // Open the file
#if _MSC_VER >= 1400 // MSVC 8.0 / 2005
	_wfopen_s(&file, myFilename.c_str(), m.c_str());
#else
    file = fopen(myFilename.c_str(), m.c_str());
#endif
    if( file == 0 )
        return -1;

    return 0;
}

int CScriptFile::Close()
{
    if( file == 0 )
        return -1;

    fclose(file);
    file = 0;

    return 0;
}

int CScriptFile::GetSize() const
{
	if( file == 0 )
		return -1;

	int pos = ftell(file);
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, pos, SEEK_SET);

	return size;
}

int CScriptFile::GetPos() const
{
	if( file == 0 )
		return -1;

	return ftell(file);
}
 
int CScriptFile::SetPos(int pos)
{
	if( file == 0 )
		return -1;

	int r = fseek(file, pos, SEEK_SET);

	// Return -1 on error
	return r ? -1 : 0;
}

int CScriptFile::MovePos(int delta)
{
	if( file == 0 )
		return -1;

	int r = fseek(file, delta, SEEK_CUR);

	// Return -1 on error
	return r ? -1 : 0;
}

int CScriptFile::ReadString(unsigned int length, std::wstring &str)
{
	if( file == 0 )
		return 0;

	// Read the string // TODO: read file as UTF8 - without string -> wstring conversion
	std::string ascii_str;
	ascii_str.resize(length);
	int ascii_size = (int)fread(&ascii_str[0], 1, length, file); 
	ascii_str.resize(ascii_size);

	str = utf8::converter(ascii_str).wstr();
	return ascii_size;
}

int CScriptFile::ReadLine(std::wstring &str)
{
	if( file == 0 )
		return 0;

	// Read until the first new-line character // TODO: read file as UTF8 - without string -> wstring conversion
	std::string ascii_str = "";
	char buf[256];

	do
	{
		// Get the current position so we can determine how many characters were read
		int start = ftell(file);

		// Set the last byte to something different that 0, so that we can check if the buffer was filled up
		buf[255] = 1;

		// Read the line (or first 255 characters, which ever comes first)
		fgets(buf, 256, file);
		
		// Get the position after the read
		int end = ftell(file);

		// Add the read characters to the output buffer
		ascii_str.append(buf, end-start);
	}
	while( !feof(file) && buf[255] == 0 && buf[254] != '\n' );

	str = utf8::converter(ascii_str).wstr();
	return int(ascii_str.size());
}

asINT64 CScriptFile::ReadInt(asUINT bytes)
{
	if( file == 0 )
		return 0;

	if( bytes > 8 ) bytes = 8;
	if( bytes == 0 ) return 0;

	unsigned char buf[8];
	fread(buf, bytes, 1, file);

	asINT64 val = 0;
	if( mostSignificantByteFirst )
	{
		unsigned int n = 0;
		for( ; n < bytes; n++ )
			val |= asQWORD(buf[n]) << ((bytes-n-1)*8);
		if( buf[0] & 0x80 )
			for( ; n < 8; n++ )
				val |= asQWORD(0xFF) << (n*8);
	}
	else
	{
		unsigned int n = 0;
		for( ; n < bytes; n++ )
			val |= asQWORD(buf[n]) << (n*8);
		if( buf[0] & 0x80 )
			for( ; n < 8; n++ )
				val |= asQWORD(0xFF) << (n*8);
	}

	return val;
}

asQWORD CScriptFile::ReadUInt(asUINT bytes)
{
	if( file == 0 )
		return 0;

	if( bytes > 8 ) bytes = 8;
	if( bytes == 0 ) return 0;

	unsigned char buf[8];
	fread(buf, bytes, 1, file);

	asQWORD val = 0;
	if( mostSignificantByteFirst )
	{
		unsigned int n = 0;
		for( ; n < bytes; n++ )
			val |= asQWORD(buf[n]) << ((bytes-n-1)*8);
	}
	else
	{
		unsigned int n = 0;
		for( ; n < bytes; n++ )
			val |= asQWORD(buf[n]) << (n*8);
	}

	return val;
}

float CScriptFile::ReadFloat()
{
	if( file == 0 )
		return 0;

	unsigned char buf[4];
	fread(buf, 4, 1, file);

	asUINT val = 0;
	if( mostSignificantByteFirst )
	{
		unsigned int n = 0;
		for( ; n < 4; n++ )
			val |= asQWORD(buf[n]) << ((3-n)*8);
	}
	else
	{
		unsigned int n = 0;
		for( ; n < 4; n++ )
			val |= asQWORD(buf[n]) << (n*8);
	}

	return *reinterpret_cast<float*>(&val);
}

double CScriptFile::ReadDouble()
{
	if( file == 0 )
		return 0;

	unsigned char buf[8];
	fread(buf, 8, 1, file);

	asQWORD val = 0;
	if( mostSignificantByteFirst )
	{
		unsigned int n = 0;
		for( ; n < 8; n++ )
			val |= asQWORD(buf[n]) << ((7-n)*8);
	}
	else
	{
		unsigned int n = 0;
		for( ; n < 8; n++ )
			val |= asQWORD(buf[n]) << (n*8);
	}

	return *reinterpret_cast<double*>(&val);
}

bool CScriptFile::IsEOF() const
{
	if( file == 0 )
		return true;

	return feof(file) ? true : false;
}

#if AS_WRITE_OPS == 1
int CScriptFile::WriteString(const std::wstring &str)
{
	if( file == 0 )
		return -1;

	std::string ascii_str = utf8::converter(str).str();

	// Write the entire string // TODO: write to file as UTF8 - without wstring -> string conversion
	size_t r = fwrite(&ascii_str[0], 1, str.length(), file);

	return int(r);
}

int CScriptFile::WriteInt(asINT64 val, asUINT bytes)
{
	if( file == 0 )
		return 0;

	unsigned char buf[8];
	if( mostSignificantByteFirst )
	{
		for( unsigned int n = 0; n < bytes; n++ )
			buf[n] = (val >> ((bytes-n-1)*8)) & 0xFF;
	}
	else
	{
		for( unsigned int n = 0; n < bytes; n++ )
			buf[n] = (val >> (n*8)) & 0xFF;
	}

	size_t r = fwrite(&buf, bytes, 1, file);
	return int(r);
}

int CScriptFile::WriteUInt(asQWORD val, asUINT bytes)
{
	if( file == 0 )
		return 0;

	unsigned char buf[8];
	if( mostSignificantByteFirst )
	{
		for( unsigned int n = 0; n < bytes; n++ )
			buf[n] = (val >> ((bytes-n-1)*8)) & 0xFF;
	}
	else
	{
		for( unsigned int n = 0; n < bytes; n++ )
			buf[n] = (val >> (n*8)) & 0xFF;
	}

	size_t r = fwrite(&buf, bytes, 1, file);
	return int(r);
}

int CScriptFile::WriteFloat(float f)
{
	if( file == 0 )
		return 0;

	unsigned char buf[4];
	asUINT val = *reinterpret_cast<asUINT*>(&f);
	if( mostSignificantByteFirst )
	{
		for( unsigned int n = 0; n < 4; n++ )
			buf[n] = (val >> ((3-n)*4)) & 0xFF;
	}
	else
	{
		for( unsigned int n = 0; n < 4; n++ )
			buf[n] = (val >> (n*8)) & 0xFF;
	}

	size_t r = fwrite(&buf, 4, 1, file);
	return int(r);
}

int CScriptFile::WriteDouble(double d)
{
	if( file == 0 )
		return 0;

	unsigned char buf[8];
	asQWORD val = *reinterpret_cast<asQWORD*>(&d);
	if( mostSignificantByteFirst )
	{
		for( unsigned int n = 0; n < 8; n++ )
			buf[n] = (val >> ((7-n)*8)) & 0xFF;
	}
	else
	{
		for( unsigned int n = 0; n < 8; n++ )
			buf[n] = (val >> (n*8)) & 0xFF;
	}

	size_t r = fwrite(&buf, 8, 1, file);
	return int(r);
}
#endif


END_AS_NAMESPACE
