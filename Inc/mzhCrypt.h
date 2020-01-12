#pragma once

#ifndef _MZH_CRYPT
#define _MZH_CRYPT

#include <wincrypt.h>
#include <mzhBase.h>

_MZH_BEGIN

template<bool t_bManaged>
class CCryptContextT
{
public:
	CCryptContextT(HCRYPTPROV hProv = NULL)
		: m_hProv(hProv)
	{}
	~CCryptContextT()
	{
		if (t_bManaged)
			Release();
	}

protected:
	HCRYPTPROV	m_hProv;

public:
	operator HCRYPTPROV() const { return m_hProv; }

public:
	BOOL Acquire(LPCTSTR szContainer = NULL, LPCTSTR szProvider = NULL, DWORD dwProvType = PROV_RSA_FULL, DWORD dwFlags = 0)
	{
		ASSERT(NULL == m_hProv);
		return CryptAcquireContext(&m_hProv, szContainer, szProvider, dwProvType, dwFlags);
	}
	void Release()
	{
		if (m_hProv)
		{
			VERIFY(CryptReleaseContext(m_hProv, 0));
			m_hProv = NULL;
		}
	}

};

typedef CCryptContextT<true> CCryptContext;


template<bool t_bManaged>
class CCryptHashT
{
public:
	CCryptHashT(HCRYPTHASH hHash = NULL)
		: m_hHash(NULL)
	{}
	~CCryptHashT()
	{
		if (t_bManaged)
			Destroy();
	}

protected:
	HCRYPTHASH	m_hHash;

public:
	operator HCRYPTHASH() const { return m_hHash; }

public:
	BOOL Create(HCRYPTPROV hProv, ALG_ID Algid, HCRYPTKEY hKey = NULL, DWORD dwFlags = 0)
	{
		ASSERT(NULL == m_hHash);
		return CryptCreateHash(hProv, Algid, hKey, dwFlags, &m_hHash);
	}
	BOOL HashData(LPCBYTE pbData, DWORD dwDataLen, DWORD dwFlags = 0)
	{
		return CryptHashData(m_hHash, pbData, dwDataLen, dwFlags);
	}
	BOOL GetParam(DWORD dwParam, LPBYTE pbData, DWORD& dwDataLen, DWORD dwFlags = 0)
	{
		return CryptGetHashParam(m_hHash, dwParam, pbData, &dwDataLen, dwFlags);
	}
	void Destroy()
	{
		if (m_hHash)
		{
			VERIFY(CryptDestroyHash(m_hHash));
			m_hHash = NULL;
		}
	}
};

typedef CCryptHashT<true> CCryptHash;

template<bool t_bManaged>
class CCryptKeyT
{
public:
	CCryptKeyT(HCRYPTKEY hKey = NULL)
		: m_hKey(hKey)
	{ }
	~CCryptKeyT()
	{
		if (t_bManaged)
			Destroy();
	}

protected:
	HCRYPTKEY	m_hKey;

public:
	operator HCRYPTKEY() const { return m_hKey; }

public:
	BOOL Derive(HCRYPTPROV hProv, ALG_ID Algid, HCRYPTHASH hHash, DWORD dwFlags)
	{
		ASSERT(NULL == m_hKey);
		return CryptDeriveKey(hProv, Algid, hHash, dwFlags, &m_hKey);
	}
	BOOL SetParam(DWORD dwParam, LPCBYTE pbData, DWORD dwFlags)
	{
		return CryptSetKeyParam(m_hKey, dwParam, pbData, dwFlags);
	}
	BOOL Encrypt(HCRYPTHASH hHash, BOOL Final, DWORD dwFlags, LPBYTE pbData, DWORD& dwDataLen, DWORD dwBufLen)
	{
		return CryptEncrypt(m_hKey, hHash, Final, dwFlags, pbData, &dwDataLen, dwBufLen);
	}
	BOOL Decrypt(HCRYPTHASH hHash, BOOL Final, DWORD dwFlags, LPBYTE pbData, DWORD& dwDataLen)
	{
		return CryptDecrypt(m_hKey, hHash, Final, dwFlags, pbData, &dwDataLen);
	}
	void Destroy()
	{
		if (m_hKey)
		{
			VERIFY(CryptDestroyKey(m_hKey));
			m_hKey = NULL;
		}
	}
};

typedef CCryptKeyT<true> CCryptKey;


_MZH_END

#endif
