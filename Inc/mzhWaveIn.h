#pragma once

#ifndef _MZH_REC
#define _MZH_REC

#include <Windows.h>
#include <mmsystem.h>
#include <vector>
#include <mzhBuffer.h>

_MZH_BEGIN

template<typename T>
class CWaveInT
{
public:
	CWaveInT(_MZH CSharedBlockBufferT<T>& buffer)
		: m_hWaveIn(NULL)
		, m_bRec(FALSE)
		, m_Buffer(buffer)
	{
	}
	virtual ~CWaveInT()
	{
		Close();
	}

protected:
	HWAVEIN m_hWaveIn;
protected:
	_MZH CEvent m_Event;
	_MZH CSharedBlockBufferT<T>& m_Buffer;

protected:
	static void CALLBACK WIProc(HWAVEIN hwi, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
	{
		CWaveInT* pThis = (CWaveInT*)dwInstance;
		ASSERT(pThis);
		switch (uMsg)
		{
			case WIM_DATA:
				ASSERT(hwi == pThis->m_hWaveIn);
				pThis->OnData((LPWAVEHDR)dwParam1);
				break;
			case WIM_OPEN:
				pThis->OnOpen();
				break;
			case WIM_CLOSE:
				ASSERT(hwi == pThis->m_hWaveIn);
				pThis->OnClose();
				break;
			default:
				ASSERT(FALSE);
				break;
		}
	}

protected:
	void OnOpen()
	{
		TRACE0("::>> Open =====>>>>>>>\n");
	}
	void OnClose()
	{
		TRACE0("::>> Close =====<<<<<<\n");
	}
	void OnData(LPWAVEHDR lphdr)
	{
		ASSERT(lphdr && !(lphdr->dwBytesRecorded % sizeof(T)));
		TRACE2("::>> Data: %XH, %d >>\n", lphdr->dwFlags, lphdr->dwBytesRecorded);
		LPSTR lpDib = lphdr->lpData;
		UINT nLen = lphdr->dwBytesRecorded / sizeof(T);
		VERIFY(m_Buffer.Append((T*)lphdr->lpData, lphdr->dwBytesRecorded / sizeof(T)));
		if (m_bRec)
		{
			MMRESULT mmres = waveInAddBuffer(m_hWaveIn, lphdr, sizeof(WAVEHDR));
			ASSERT(mmres == MMSYSERR_NOERROR);
		}
		else
		{
			m_Event.SetEvent();
		}
	}

protected:
	std::vector<WAVEHDR> m_wavhdr;
	BOOL ResizeBuffer(SIZE_T BufCount = 0, SIZE_T BufLen = 0)
	{
		for(auto& var : m_wavhdr)
			FREE_PTR(var.lpData);
		m_wavhdr.clear();
		WAVEHDR hdr = {};
		hdr.dwBufferLength = BufLen;
		for(SIZE_T i(0); i < BufCount; i++)
		{
			hdr.lpData = (LPSTR)malloc(hdr.dwBufferLength);
			if(hdr.lpData)
				m_wavhdr.push_back(hdr);
			else
				break;
		}
		return m_wavhdr.size() == BufCount;
	}

public:
	//WAVEFORMATEX wfx = {};
	//{
	//	wfx.wFormatTag = WAVE_FORMAT_PCM;
	//	wfx.nChannels = 1;//wic.wChannels;
	//	wfx.nSamplesPerSec = 8000;	// 8KHz
	//	wfx.nAvgBytesPerSec = 8000 * sizeof(T);
	//	wfx.nBlockAlign = sizeof(T);
	//	wfx.wBitsPerSample = sizeof(T) * 8;
	//}
	MMRESULT Open(const WAVEFORMATEX& wfx, SIZE_T BufLen = 2048, SIZE_T BufCount = 2, UINT uDevID = WAVE_MAPPER)
	{
#ifdef DEBUG
		UINT nNum = waveInGetNumDevs();
		for (UINT idx(0); idx < nNum; idx++)
		{
			WAVEINCAPS wic = {};
			MMRESULT mres = waveInGetDevCaps(idx, &wic, sizeof(wic));
			if (MMSYSERR_NOERROR != mres)
			{
				TRACE1("failed to get device caps: %d\n", mres);
				continue;
			}
			TRACE1("DEV: %s\n", wic.szPname);
		}
#endif // DEBUG
		MMRESULT mres = waveInOpen(&m_hWaveIn, uDevID, &wfx, (DWORD_PTR)WIProc, (DWORD_PTR)this, CALLBACK_FUNCTION);
		if (MMSYSERR_NOERROR != mres)
		{
			TRACE1("Failed to open: %d\n", mres);
		}
		else if(!ResizeBuffer(BufCount, BufLen))
		{
			mres = MMSYSERR_NOMEM;
		}
		else
		{
			for each (auto& var in m_wavhdr)
			{
				mres = waveInPrepareHeader(m_hWaveIn, &var, sizeof(var));
				if(MMSYSERR_NOERROR != mres)
				{
					ASSERT(0);
					break;
				}
			}
		}
		return mres;
	}
	void Close()
	{
		if (m_hWaveIn)
		{
			if (IsRecording())
			{
				VERIFY(Stop() == MMSYSERR_NOERROR);
			}
			for(auto& var : m_wavhdr)
			{
				ASSERT((var.dwFlags & WHDR_INQUEUE) == 0);
				MMRESULT mmres = waveInUnprepareHeader(m_hWaveIn, &var, sizeof(var));
				ASSERT(mmres == MMSYSERR_NOERROR);
				FREE_PTR(var.lpData)
			}
			m_wavhdr.clear();

			MMRESULT mmres = waveInClose(m_hWaveIn);
			ASSERT(mmres == MMSYSERR_NOERROR);
			m_hWaveIn = NULL;
		}
	}

protected:
	BOOL m_bRec;
public:
	BOOL IsRecording() { return m_bRec; }
public:
	MMRESULT Start()
	{
		ASSERT(!m_bRec);
		for (auto& var : m_wavhdr)
		{
			MMRESULT mmres = waveInAddBuffer(m_hWaveIn, &var, sizeof(var));
			ASSERT(mmres == MMSYSERR_NOERROR);
		}

		MMRESULT mmres = waveInStart(m_hWaveIn);
		ASSERT(mmres == MMSYSERR_NOERROR);
		if (mmres == MMSYSERR_NOERROR)
		{
			m_bRec = TRUE;
		}
		return mmres;
	}
	MMRESULT Stop()
	{
		ASSERT(m_bRec);
		m_bRec = FALSE;
		TRACE0("========Stopping========\n");
		m_Event.ResetEvent();
		for (BOOL bRun(TRUE); bRun; m_Event.Lock(1000))
		{
			bRun = FALSE;
			for (auto& var : m_wavhdr)
			{
				if (var.dwFlags & WHDR_INQUEUE)
				{
					bRun = TRUE;
					break;
				}
			}
		}
		MMRESULT mmres = waveInStop(m_hWaveIn);
		TRACE1("========Stop(%d)========\n", mmres);
		ASSERT(mmres == MMSYSERR_NOERROR);
		if (mmres == MMSYSERR_NOERROR)
		{
			//m_bRec = FALSE;
		}
		return mmres;
	}
	MMRESULT Reset()
	{
		MMRESULT mmres = waveInReset(m_hWaveIn);
		ASSERT(mmres == MMSYSERR_NOERROR);
		return mmres;
	}
};

_MZH_END

#endif
