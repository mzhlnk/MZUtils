
#ifndef _MZUTIL
#define _MZUTIL

namespace mzUtil
{

	LPCSTR	StringPtr(LPCSTR p) { return p; }
	LPCWSTR	StringPtr(LPCWSTR p) { return p; }
	LPCSTR	StringPtr(const std::string& str) { return str.c_str(); }
	LPCWSTR	StringPtr(const std::wstring& str) { return str.c_str(); }

	template<class ContainerT>
	void Split(ContainerT& container, LPCTSTR text, LPCTSTR delimiter)
	{
		PTSTR ptmp = _tcsdup(text);
#ifdef _tcstok_s
		for (PTSTR pnext = NULL, pstr = _tcstok_s(ptmp, delimiter, &pnext); pstr; pstr = _tcstok_s(NULL, delimiter, &pnext)) {
#else
		for (PTSTR pstr = _tcstok(ptmp, delimiter); pstr; pstr = _tcstok(NULL, delimiter)) {
#endif // _tcstok_s
			container.push_back(pstr);
		}
		free(ptmp);
	}

	template<class ContainerT, class PairT>
	void Split(ContainerT& container, LPCTSTR text, const PairT& delimiter)
	{
		PTSTR ptmp = _tcsdup(text);
		LPCTSTR delimiter1 = StringPtr(delimiter.first);
#ifdef _tcstok_s
		for (PTSTR pnext = NULL, pstr = _tcstok_s(ptmp, delimiter1, &pnext); pstr; pstr = _tcstok_s(NULL, delimiter1, &pnext))
#else
		for (PTSTR pstr = _tcstok(ptmp, delimiter1); pstr; pstr = _tcstok(NULL, delimiter1))
#endif // _tcstok_s
		{
			PTSTR ptr = pstr;
			for (LPCTSTR delimiter2 = StringPtr(delimiter.second); *ptr && !_tcschr(delimiter2, *ptr); ptr++);
			if (ptr && *ptr)
			{
				*ptr++ = _T('\0');
				container.push_back(PairT(pstr, ptr));
			}
			else
			{
				container.push_back(PairT(pstr, _T("")));
			}
		}
		free(ptmp);
	}

}

#endif
