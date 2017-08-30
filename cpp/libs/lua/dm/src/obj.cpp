#include "stdafx.h"
#include "obj.h"
#include <afxdisp.h>
#include <cassert>

class MyDispatchDriver
{
public:
    IDispatch* p;
    MyDispatchDriver()
    {
        p = NULL;
    }
    MyDispatchDriver(IDispatch* lp)
    {
        if ((p = lp) != NULL)
            p->AddRef();
    }
    ~MyDispatchDriver() { if (p) p->Release(); }
    HRESULT GetIDOfName(LPCOLESTR lpsz, DISPID* pdispid)
    {
        return p->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpsz, 1, LOCALE_USER_DEFAULT, pdispid);
    }
    HRESULT Invoke0(DISPID dispid, VARIANT* pvarRet = NULL)
    {
        DISPPARAMS dispparams = { NULL, NULL, 0, 0};
        return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
    }
    HRESULT Invoke0(LPCOLESTR lpszName, VARIANT* pvarRet = NULL)
    {
        HRESULT hr = -1;
        DISPID dispid;
        if (p == NULL) return hr;
        hr = GetIDOfName(lpszName, &dispid);
        if (SUCCEEDED(hr))
            hr = Invoke0(dispid, pvarRet);
        return hr;
    }
    HRESULT InvokeN(DISPID dispid, VARIANT* pvarParams, int nParams, VARIANT* pvarRet = NULL)
    {
        DISPPARAMS dispparams = { pvarParams, NULL, nParams, 0};
        return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
    }
    HRESULT InvokeN(LPCOLESTR lpszName, VARIANT* pvarParams, int nParams, VARIANT* pvarRet = NULL)
    {
        HRESULT hr = -1;
        DISPID dispid;
        if (p == NULL) return hr;
        hr = GetIDOfName(lpszName, &dispid);
        if (SUCCEEDED(hr))
            hr = InvokeN(dispid, pvarParams, nParams, pvarRet);
        return hr;
    }
};

dmsoft::dmsoft()
{
    CLSID clsid;
    IUnknown * pUnknown = NULL;
    HRESULT hr;

    obj = NULL;
    hr = ::CLSIDFromProgID( L"dm.dmsoft", &clsid );
    if (FAILED(hr))
    {
		assert(false);
        return;
    }

    hr = ::CoCreateInstance( clsid, NULL, CLSCTX_ALL, IID_IUnknown, (LPVOID *)&pUnknown);
    if (FAILED(hr))
    {
		assert(false);
        return;
    }

    pUnknown->QueryInterface(IID_IDispatch, (void **)&obj);
    if (pUnknown) pUnknown->Release();
}

dmsoft::~dmsoft()
{
        if (obj) obj->Release();
}

CString dmsoft::Ver()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"Ver",&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::SetPath(const CString& path)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(path);

    hr = spDisp.InvokeN(L"SetPath",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::Ocr(long x1, long y1, long x2, long y2, const CString& color, double sim)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    VARIANT vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    hr = spDisp.InvokeN(L"Ocr",pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::FindStr(long x1, long y1, long x2, long y2, const CString& str, const CString& color, double sim, long * x, long * y)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[9];
    VARIANT vResult;
    VARIANT t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[8] = COleVariant(x1);
    pn[7] = COleVariant(y1);
    pn[6] = COleVariant(x2);
    pn[5] = COleVariant(y2);
    pn[4] = COleVariant(str);
    pn[3] = COleVariant(color);
    pn[2] = COleVariant(sim);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    hr = spDisp.InvokeN(L"FindStr",pn,9,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::GetResultCount(const CString& str)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(str);

    hr = spDisp.InvokeN(L"GetResultCount",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::GetResultPos(const CString& str, long index, long * x, long * y)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;
    VARIANT t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[3] = COleVariant(str);
    pn[2] = COleVariant(index);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    hr = spDisp.InvokeN(L"GetResultPos",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::StrStr(const CString& s, const CString& str)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(s);
    pn[0] = COleVariant(str);

    hr = spDisp.InvokeN(L"StrStr",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::SendCommand(const CString& cmd)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(cmd);

    hr = spDisp.InvokeN(L"SendCommand",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::UseDict(long index)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(index);

    hr = spDisp.InvokeN(L"UseDict",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::GetBasePath()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"GetBasePath",&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::SetDictPwd(const CString& pwd)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(pwd);

    hr = spDisp.InvokeN(L"SetDictPwd",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::OcrInFile(long x1, long y1, long x2, long y2, const CString& pic_name, const CString& color, double sim)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    VARIANT vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(pic_name);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    hr = spDisp.InvokeN(L"OcrInFile",pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::Capture(long x1, long y1, long x2, long y2, const CString& file_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    VARIANT vResult;

    pn[4] = COleVariant(x1);
    pn[3] = COleVariant(y1);
    pn[2] = COleVariant(x2);
    pn[1] = COleVariant(y2);
    pn[0] = COleVariant(file_name);

    hr = spDisp.InvokeN(L"Capture",pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::KeyPress(long vk)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(vk);

    hr = spDisp.InvokeN(L"KeyPress",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::KeyDown(long vk)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(vk);

    hr = spDisp.InvokeN(L"KeyDown",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::KeyUp(long vk)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(vk);

    hr = spDisp.InvokeN(L"KeyUp",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::LeftClick()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"LeftClick",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::RightClick()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"RightClick",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::MiddleClick()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"MiddleClick",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::LeftDoubleClick()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"LeftDoubleClick",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::LeftDown()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"LeftDown",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::LeftUp()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"LeftUp",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::RightDown()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"RightDown",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::RightUp()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"RightUp",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::MoveTo(long x,long y)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(x);
    pn[0] = COleVariant(y);

    hr = spDisp.InvokeN(L"MoveTo",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::MoveR(long rx,long ry)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(rx);
    pn[0] = COleVariant(ry);

    hr = spDisp.InvokeN(L"MoveR",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::GetColor(long x,long y)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(x);
    pn[0] = COleVariant(y);

    hr = spDisp.InvokeN(L"GetColor",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::GetColorBGR(long x,long y)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(x);
    pn[0] = COleVariant(y);

    hr = spDisp.InvokeN(L"GetColorBGR",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::RGB2BGR(const CString& rgb_color)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(rgb_color);

    hr = spDisp.InvokeN(L"RGB2BGR",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::BGR2RGB(const CString& bgr_color)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(bgr_color);

    hr = spDisp.InvokeN(L"BGR2RGB",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::UnBindWindow()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"UnBindWindow",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::CmpColor(long x, long y, const CString& color, double sim)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(x);
    pn[2] = COleVariant(y);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    hr = spDisp.InvokeN(L"CmpColor",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::ClientToScreen(long hwnd,long * x,long * y)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;
    VARIANT t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[2] = COleVariant(hwnd);
    t0.vt = VT_I4;
    t0.lVal = *x;
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    t1.vt = VT_I4;
    t1.lVal = *y;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    hr = spDisp.InvokeN(L"ClientToScreen",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::ScreenToClient(long hwnd,long * x,long * y)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;
    VARIANT t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[2] = COleVariant(hwnd);
    t0.vt = VT_I4;
    t0.lVal = *x;
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    t1.vt = VT_I4;
    t1.lVal = *y;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    hr = spDisp.InvokeN(L"ScreenToClient",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::ShowScrMsg(long x1, long y1, long x2, long y2, const CString& msg, const CString& color)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    VARIANT vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(msg);
    pn[0] = COleVariant(color);

    hr = spDisp.InvokeN(L"ShowScrMsg",pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::SetMinRowGap(long row_gap)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(row_gap);

    hr = spDisp.InvokeN(L"SetMinRowGap",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::SetMinColGap(long col_gap)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(col_gap);

    hr = spDisp.InvokeN(L"SetMinColGap",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FindColor(long x1, long y1, long x2, long y2, const CString& color, double sim, long dir, long * x, long * y)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[9];
    VARIANT vResult;
    VARIANT t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[8] = COleVariant(x1);
    pn[7] = COleVariant(y1);
    pn[6] = COleVariant(x2);
    pn[5] = COleVariant(y2);
    pn[4] = COleVariant(color);
    pn[3] = COleVariant(sim);
    pn[2] = COleVariant(dir);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    hr = spDisp.InvokeN(L"FindColor",pn,9,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::FindColorEx(long x1, long y1, long x2, long y2, const CString& color, double sim, long dir)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    VARIANT vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    hr = spDisp.InvokeN(L"FindColorEx",pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::SetWordLineHeight(long line_height)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(line_height);

    hr = spDisp.InvokeN(L"SetWordLineHeight",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::SetWordGap(long word_gap)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(word_gap);

    hr = spDisp.InvokeN(L"SetWordGap",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::SetRowGapNoDict(long row_gap)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(row_gap);

    hr = spDisp.InvokeN(L"SetRowGapNoDict",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::SetColGapNoDict(long col_gap)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(col_gap);

    hr = spDisp.InvokeN(L"SetColGapNoDict",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::SetWordLineHeightNoDict(long line_height)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(line_height);

    hr = spDisp.InvokeN(L"SetWordLineHeightNoDict",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::SetWordGapNoDict(long word_gap)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(word_gap);

    hr = spDisp.InvokeN(L"SetWordGapNoDict",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::GetWordResultCount(const CString& str)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(str);

    hr = spDisp.InvokeN(L"GetWordResultCount",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::GetWordResultPos(const CString& str, long index, long * x, long * y)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;
    VARIANT t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[3] = COleVariant(str);
    pn[2] = COleVariant(index);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    hr = spDisp.InvokeN(L"GetWordResultPos",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::GetWordResultStr(const CString& str, long index)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(str);
    pn[0] = COleVariant(index);

    hr = spDisp.InvokeN(L"GetWordResultStr",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::GetWords(long x1, long y1, long x2, long y2, const CString& color, double sim)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    VARIANT vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    hr = spDisp.InvokeN(L"GetWords",pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::GetWordsNoDict(long x1, long y1, long x2, long y2, const CString& color)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    VARIANT vResult;

    pn[4] = COleVariant(x1);
    pn[3] = COleVariant(y1);
    pn[2] = COleVariant(x2);
    pn[1] = COleVariant(y2);
    pn[0] = COleVariant(color);

    hr = spDisp.InvokeN(L"GetWordsNoDict",pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::SetShowErrorMsg(long show)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(show);

    hr = spDisp.InvokeN(L"SetShowErrorMsg",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::GetClientSize(long hwnd,long * width,long * height)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;
    VARIANT t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[2] = COleVariant(hwnd);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    hr = spDisp.InvokeN(L"GetClientSize",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        *width = t0.lVal;
        *height = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::MoveWindow(long hwnd,long x,long y)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(x);
    pn[0] = COleVariant(y);

    hr = spDisp.InvokeN(L"MoveWindow",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::GetColorHSV(long x,long y)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(x);
    pn[0] = COleVariant(y);

    hr = spDisp.InvokeN(L"GetColorHSV",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::GetAveRGB(long x1,long y1,long x2,long y2)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(x1);
    pn[2] = COleVariant(y1);
    pn[1] = COleVariant(x2);
    pn[0] = COleVariant(y2);

    hr = spDisp.InvokeN(L"GetAveRGB",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::GetAveHSV(long x1,long y1,long x2,long y2)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(x1);
    pn[2] = COleVariant(y1);
    pn[1] = COleVariant(x2);
    pn[0] = COleVariant(y2);

    hr = spDisp.InvokeN(L"GetAveHSV",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::GetForegroundWindow()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"GetForegroundWindow",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::GetForegroundFocus()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"GetForegroundFocus",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::GetMousePointWindow()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"GetMousePointWindow",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::GetPointWindow(long x,long y)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(x);
    pn[0] = COleVariant(y);

    hr = spDisp.InvokeN(L"GetPointWindow",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::EnumWindow(long parent, const CString& title, const CString& class_name, long filter)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(parent);
    pn[2] = COleVariant(title);
    pn[1] = COleVariant(class_name);
    pn[0] = COleVariant(filter);

    hr = spDisp.InvokeN(L"EnumWindow",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::GetWindowState(long hwnd,long flag)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(flag);

    hr = spDisp.InvokeN(L"GetWindowState",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::GetWindow(long hwnd,long flag)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(flag);

    hr = spDisp.InvokeN(L"GetWindow",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::GetSpecialWindow(long flag)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(flag);

    hr = spDisp.InvokeN(L"GetSpecialWindow",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::SetWindowText(long hwnd, const CString& text)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(text);

    hr = spDisp.InvokeN(L"SetWindowText",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::SetWindowSize(long hwnd,long width,long height)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(width);
    pn[0] = COleVariant(height);

    hr = spDisp.InvokeN(L"SetWindowSize",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::GetWindowRect(long hwnd,long * x1,long * y1,long * x2,long * y2)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    VARIANT vResult;
    VARIANT t0,t1,t2,t3;

    VariantInit(&t0);
    VariantInit(&t1);
    VariantInit(&t2);
    VariantInit(&t3);

    pn[4] = COleVariant(hwnd);
    pn[3].vt = VT_BYREF|VT_VARIANT;
    pn[3].pvarVal = &t0;
    pn[2].vt = VT_BYREF|VT_VARIANT;
    pn[2].pvarVal = &t1;
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t2;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t3;

    hr = spDisp.InvokeN(L"GetWindowRect",pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        *x1 = t0.lVal;
        *y1 = t1.lVal;
        *x2 = t2.lVal;
        *y2 = t3.lVal;
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::GetWindowTitle(long hwnd)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(hwnd);

    hr = spDisp.InvokeN(L"GetWindowTitle",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::GetWindowClass(long hwnd)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(hwnd);

    hr = spDisp.InvokeN(L"GetWindowClass",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::SetWindowState(long hwnd,long flag)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(flag);

    hr = spDisp.InvokeN(L"SetWindowState",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::CreateFoobarRect(long hwnd,long x,long y,long w,long h)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    VARIANT vResult;

    pn[4] = COleVariant(hwnd);
    pn[3] = COleVariant(x);
    pn[2] = COleVariant(y);
    pn[1] = COleVariant(w);
    pn[0] = COleVariant(h);

    hr = spDisp.InvokeN(L"CreateFoobarRect",pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::CreateFoobarRoundRect(long hwnd,long x,long y,long w,long h,long rw,long rh)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    VARIANT vResult;

    pn[6] = COleVariant(hwnd);
    pn[5] = COleVariant(x);
    pn[4] = COleVariant(y);
    pn[3] = COleVariant(w);
    pn[2] = COleVariant(h);
    pn[1] = COleVariant(rw);
    pn[0] = COleVariant(rh);

    hr = spDisp.InvokeN(L"CreateFoobarRoundRect",pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::CreateFoobarEllipse(long hwnd,long x,long y,long w,long h)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    VARIANT vResult;

    pn[4] = COleVariant(hwnd);
    pn[3] = COleVariant(x);
    pn[2] = COleVariant(y);
    pn[1] = COleVariant(w);
    pn[0] = COleVariant(h);

    hr = spDisp.InvokeN(L"CreateFoobarEllipse",pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::CreateFoobarCustom(long hwnd, long x, long y, const CString& pic, const CString& trans_color, double sim)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    VARIANT vResult;

    pn[5] = COleVariant(hwnd);
    pn[4] = COleVariant(x);
    pn[3] = COleVariant(y);
    pn[2] = COleVariant(pic);
    pn[1] = COleVariant(trans_color);
    pn[0] = COleVariant(sim);

    hr = spDisp.InvokeN(L"CreateFoobarCustom",pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FoobarFillRect(long hwnd, long x1, long y1, long x2, long y2, const CString& color)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    VARIANT vResult;

    pn[5] = COleVariant(hwnd);
    pn[4] = COleVariant(x1);
    pn[3] = COleVariant(y1);
    pn[2] = COleVariant(x2);
    pn[1] = COleVariant(y2);
    pn[0] = COleVariant(color);

    hr = spDisp.InvokeN(L"FoobarFillRect",pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FoobarDrawText(long hwnd, long x, long y, long w, long h, const CString& text, const CString& color, long align)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    VARIANT vResult;

    pn[7] = COleVariant(hwnd);
    pn[6] = COleVariant(x);
    pn[5] = COleVariant(y);
    pn[4] = COleVariant(w);
    pn[3] = COleVariant(h);
    pn[2] = COleVariant(text);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(align);

    hr = spDisp.InvokeN(L"FoobarDrawText",pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FoobarDrawPic(long hwnd, long x, long y, const CString& pic, const CString& trans_color)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    VARIANT vResult;

    pn[4] = COleVariant(hwnd);
    pn[3] = COleVariant(x);
    pn[2] = COleVariant(y);
    pn[1] = COleVariant(pic);
    pn[0] = COleVariant(trans_color);

    hr = spDisp.InvokeN(L"FoobarDrawPic",pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FoobarUpdate(long hwnd)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(hwnd);

    hr = spDisp.InvokeN(L"FoobarUpdate",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FoobarLock(long hwnd)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(hwnd);

    hr = spDisp.InvokeN(L"FoobarLock",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FoobarUnlock(long hwnd)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(hwnd);

    hr = spDisp.InvokeN(L"FoobarUnlock",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FoobarSetFont(long hwnd, const CString& font_name, long size, long flag)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(font_name);
    pn[1] = COleVariant(size);
    pn[0] = COleVariant(flag);

    hr = spDisp.InvokeN(L"FoobarSetFont",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FoobarTextRect(long hwnd,long x,long y,long w,long h)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    VARIANT vResult;

    pn[4] = COleVariant(hwnd);
    pn[3] = COleVariant(x);
    pn[2] = COleVariant(y);
    pn[1] = COleVariant(w);
    pn[0] = COleVariant(h);

    hr = spDisp.InvokeN(L"FoobarTextRect",pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FoobarPrintText(long hwnd, const CString& text, const CString& color)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(text);
    pn[0] = COleVariant(color);

    hr = spDisp.InvokeN(L"FoobarPrintText",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FoobarClearText(long hwnd)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(hwnd);

    hr = spDisp.InvokeN(L"FoobarClearText",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FoobarTextLineGap(long hwnd,long gap)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(gap);

    hr = spDisp.InvokeN(L"FoobarTextLineGap",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::Play(const CString& file_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(file_name);

    hr = spDisp.InvokeN(L"Play",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FaqCapture(long x1,long y1,long x2,long y2,long quality,long delay,long time)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    VARIANT vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(quality);
    pn[1] = COleVariant(delay);
    pn[0] = COleVariant(time);

    hr = spDisp.InvokeN(L"FaqCapture",pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FaqRelease(long handle)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(handle);

    hr = spDisp.InvokeN(L"FaqRelease",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::FaqSend(const CString& server, long handle, long request_type, long time_out)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(server);
    pn[2] = COleVariant(handle);
    pn[1] = COleVariant(request_type);
    pn[0] = COleVariant(time_out);

    hr = spDisp.InvokeN(L"FaqSend",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::Beep(long fre,long delay)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(fre);
    pn[0] = COleVariant(delay);

    hr = spDisp.InvokeN(L"Beep",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FoobarClose(long hwnd)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(hwnd);

    hr = spDisp.InvokeN(L"FoobarClose",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::MoveDD(long dx,long dy)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(dx);
    pn[0] = COleVariant(dy);

    hr = spDisp.InvokeN(L"MoveDD",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FaqGetSize(long handle)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(handle);

    hr = spDisp.InvokeN(L"FaqGetSize",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::LoadPic(const CString& pic_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(pic_name);

    hr = spDisp.InvokeN(L"LoadPic",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FreePic(const CString& pic_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(pic_name);

    hr = spDisp.InvokeN(L"FreePic",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::GetScreenData(long x1,long y1,long x2,long y2)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(x1);
    pn[2] = COleVariant(y1);
    pn[1] = COleVariant(x2);
    pn[0] = COleVariant(y2);

    hr = spDisp.InvokeN(L"GetScreenData",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FreeScreenData(long handle)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(handle);

    hr = spDisp.InvokeN(L"FreeScreenData",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::WheelUp()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"WheelUp",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::WheelDown()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"WheelDown",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::SetMouseDelay(const CString& tpe, long delay)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(delay);

    hr = spDisp.InvokeN(L"SetMouseDelay",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::SetKeypadDelay(const CString& tpe, long delay)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(delay);

    hr = spDisp.InvokeN(L"SetKeypadDelay",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::GetEnv(long index, const CString& name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(index);
    pn[0] = COleVariant(name);

    hr = spDisp.InvokeN(L"GetEnv",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::SetEnv(long index, const CString& name, const CString& value)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(index);
    pn[1] = COleVariant(name);
    pn[0] = COleVariant(value);

    hr = spDisp.InvokeN(L"SetEnv",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::SendString(long hwnd, const CString& str)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(str);

    hr = spDisp.InvokeN(L"SendString",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::DelEnv(long index, const CString& name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(index);
    pn[0] = COleVariant(name);

    hr = spDisp.InvokeN(L"DelEnv",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::GetPath()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"GetPath",&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::SetDict(long index, const CString& dict_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(index);
    pn[0] = COleVariant(dict_name);

    hr = spDisp.InvokeN(L"SetDict",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FindPic(long x1, long y1, long x2, long y2, const CString& pic_name, const CString& delta_color, double sim, long dir, long * x, long * y)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[10];
    VARIANT vResult;
    VARIANT t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[9] = COleVariant(x1);
    pn[8] = COleVariant(y1);
    pn[7] = COleVariant(x2);
    pn[6] = COleVariant(y2);
    pn[5] = COleVariant(pic_name);
    pn[4] = COleVariant(delta_color);
    pn[3] = COleVariant(sim);
    pn[2] = COleVariant(dir);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    hr = spDisp.InvokeN(L"FindPic",pn,10,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
	assert(false);
    return 0;
}

CString dmsoft::FindPicEx(long x1, long y1, long x2, long y2, const CString& pic_name, const CString& delta_color, double sim, long dir)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    VARIANT vResult;

    pn[7] = COleVariant(x1);
    pn[6] = COleVariant(y1);
    pn[5] = COleVariant(x2);
    pn[4] = COleVariant(y2);
    pn[3] = COleVariant(pic_name);
    pn[2] = COleVariant(delta_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    hr = spDisp.InvokeN(L"FindPicEx",pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::SetClientSize(long hwnd,long width,long height)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(width);
    pn[0] = COleVariant(height);

    hr = spDisp.InvokeN(L"SetClientSize",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::ReadInt(long hwnd, const CString& addr, long tpe)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(tpe);

    hr = spDisp.InvokeN(L"ReadInt",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

float dmsoft::ReadFloat(long hwnd, const CString& addr)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(addr);

    hr = spDisp.InvokeN(L"ReadFloat",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.fltVal;
    }
    return 0.0f;
}

double dmsoft::ReadDouble(long hwnd, const CString& addr)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(addr);

    hr = spDisp.InvokeN(L"ReadDouble",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.dblVal;
    }
    return 0.0;
}

CString dmsoft::FindInt(long hwnd, const CString& addr_range, long int_value_min, long int_value_max, long tpe)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    VARIANT vResult;

    pn[4] = COleVariant(hwnd);
    pn[3] = COleVariant(addr_range);
    pn[2] = COleVariant(int_value_min);
    pn[1] = COleVariant(int_value_max);
    pn[0] = COleVariant(tpe);

    hr = spDisp.InvokeN(L"FindInt",pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::FindFloat(long hwnd, const CString& addr_range, float float_value_min, float float_value_max)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr_range);
    pn[1] = COleVariant(float_value_min);
    pn[0] = COleVariant(float_value_max);

    hr = spDisp.InvokeN(L"FindFloat",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::FindDouble(long hwnd, const CString& addr_range, double double_value_min, double double_value_max)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr_range);
    pn[1] = COleVariant(double_value_min);
    pn[0] = COleVariant(double_value_max);

    hr = spDisp.InvokeN(L"FindDouble",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::FindString(long hwnd, const CString& addr_range, const CString& string_value, long tpe)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr_range);
    pn[1] = COleVariant(string_value);
    pn[0] = COleVariant(tpe);

    hr = spDisp.InvokeN(L"FindString",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::GetModuleBaseAddr(long hwnd, const CString& module_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(module_name);

    hr = spDisp.InvokeN(L"GetModuleBaseAddr",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::MoveToEx(long x,long y,long w,long h)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(x);
    pn[2] = COleVariant(y);
    pn[1] = COleVariant(w);
    pn[0] = COleVariant(h);

    hr = spDisp.InvokeN(L"MoveToEx",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::MatchPicName(const CString& pic_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(pic_name);

    hr = spDisp.InvokeN(L"MatchPicName",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::AddDict(long index, const CString& dict_info)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(index);
    pn[0] = COleVariant(dict_info);

    hr = spDisp.InvokeN(L"AddDict",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::EnterCri()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"EnterCri",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::LeaveCri()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"LeaveCri",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::WriteInt(long hwnd, const CString& addr, long tpe, long v)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr);
    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(v);

    hr = spDisp.InvokeN(L"WriteInt",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::WriteFloat(long hwnd, const CString& addr, float v)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(v);

    hr = spDisp.InvokeN(L"WriteFloat",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::WriteDouble(long hwnd, const CString& addr, double v)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(v);

    hr = spDisp.InvokeN(L"WriteDouble",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::WriteString(long hwnd, const CString& addr, long tpe, const CString& v)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr);
    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(v);

    hr = spDisp.InvokeN(L"WriteString",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::AsmAdd(const CString& asm_ins)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(asm_ins);

    hr = spDisp.InvokeN(L"AsmAdd",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::AsmClear()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"AsmClear",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::AsmCall(long hwnd,long mode)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(mode);

    hr = spDisp.InvokeN(L"AsmCall",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FindMultiColor(long x1, long y1, long x2, long y2, const CString& first_color, const CString& offset_color, double sim, long dir, long * x, long * y)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[10];
    VARIANT vResult;
    VARIANT t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[9] = COleVariant(x1);
    pn[8] = COleVariant(y1);
    pn[7] = COleVariant(x2);
    pn[6] = COleVariant(y2);
    pn[5] = COleVariant(first_color);
    pn[4] = COleVariant(offset_color);
    pn[3] = COleVariant(sim);
    pn[2] = COleVariant(dir);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    hr = spDisp.InvokeN(L"FindMultiColor",pn,10,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::FindMultiColorEx(long x1, long y1, long x2, long y2, const CString& first_color, const CString& offset_color, double sim, long dir)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    VARIANT vResult;

    pn[7] = COleVariant(x1);
    pn[6] = COleVariant(y1);
    pn[5] = COleVariant(x2);
    pn[4] = COleVariant(y2);
    pn[3] = COleVariant(first_color);
    pn[2] = COleVariant(offset_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    hr = spDisp.InvokeN(L"FindMultiColorEx",pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::AsmCode(long base_addr)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(base_addr);

    hr = spDisp.InvokeN(L"AsmCode",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::Assemble(const CString& asm_code, long base_addr, long is_upper)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(asm_code);
    pn[1] = COleVariant(base_addr);
    pn[0] = COleVariant(is_upper);

    hr = spDisp.InvokeN(L"Assemble",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::SetWindowTransparent(long hwnd,long v)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(v);

    hr = spDisp.InvokeN(L"SetWindowTransparent",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::ReadData(long hwnd, const CString& addr, long length)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(length);

    hr = spDisp.InvokeN(L"ReadData",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::WriteData(long hwnd, const CString& addr, const CString& data)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(data);

    hr = spDisp.InvokeN(L"WriteData",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::FindData(long hwnd, const CString& addr_range, const CString& data)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr_range);
    pn[0] = COleVariant(data);

    hr = spDisp.InvokeN(L"FindData",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::SetPicPwd(const CString& pwd)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(pwd);

    hr = spDisp.InvokeN(L"SetPicPwd",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::Log(const CString& info)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(info);

    hr = spDisp.InvokeN(L"Log",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::FindStrE(long x1, long y1, long x2, long y2, const CString& str, const CString& color, double sim)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    VARIANT vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(str);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    hr = spDisp.InvokeN(L"FindStrE",pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::FindColorE(long x1, long y1, long x2, long y2, const CString& color, double sim, long dir)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    VARIANT vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    hr = spDisp.InvokeN(L"FindColorE",pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::FindPicE(long x1, long y1, long x2, long y2, const CString& pic_name, const CString& delta_color, double sim, long dir)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    VARIANT vResult;

    pn[7] = COleVariant(x1);
    pn[6] = COleVariant(y1);
    pn[5] = COleVariant(x2);
    pn[4] = COleVariant(y2);
    pn[3] = COleVariant(pic_name);
    pn[2] = COleVariant(delta_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    hr = spDisp.InvokeN(L"FindPicE",pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::FindMultiColorE(long x1, long y1, long x2, long y2, const CString& first_color, const CString& offset_color, double sim, long dir)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    VARIANT vResult;

    pn[7] = COleVariant(x1);
    pn[6] = COleVariant(y1);
    pn[5] = COleVariant(x2);
    pn[4] = COleVariant(y2);
    pn[3] = COleVariant(first_color);
    pn[2] = COleVariant(offset_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    hr = spDisp.InvokeN(L"FindMultiColorE",pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::SetExactOcr(long exact_ocr)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(exact_ocr);

    hr = spDisp.InvokeN(L"SetExactOcr",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::ReadString(long hwnd, const CString& addr, long tpe, long length)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr);
    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(length);

    hr = spDisp.InvokeN(L"ReadString",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::FoobarTextPrintDir(long hwnd,long dir)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(dir);

    hr = spDisp.InvokeN(L"FoobarTextPrintDir",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::OcrEx(long x1, long y1, long x2, long y2, const CString& color, double sim)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    VARIANT vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    hr = spDisp.InvokeN(L"OcrEx",pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::SetDisplayInput(const CString& mode)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(mode);

    hr = spDisp.InvokeN(L"SetDisplayInput",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::GetTime()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"GetTime",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::GetScreenWidth()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"GetScreenWidth",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::GetScreenHeight()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"GetScreenHeight",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::BindWindowEx(long hwnd, const CString& display, const CString& mouse, const CString& keypad, const CString& public_desc, long mode)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    VARIANT vResult;

    pn[5] = COleVariant(hwnd);
    pn[4] = COleVariant(display);
    pn[3] = COleVariant(mouse);
    pn[2] = COleVariant(keypad);
    pn[1] = COleVariant(public_desc);
    pn[0] = COleVariant(mode);

    hr = spDisp.InvokeN(L"BindWindowEx",pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::GetDiskSerial()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"GetDiskSerial",&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::Md5(const CString& str)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(str);

    hr = spDisp.InvokeN(L"Md5",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::GetMac()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"GetMac",&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::ActiveInputMethod(long hwnd, const CString& id)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(id);

    hr = spDisp.InvokeN(L"ActiveInputMethod",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::CheckInputMethod(long hwnd, const CString& id)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(id);

    hr = spDisp.InvokeN(L"CheckInputMethod",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FindInputMethod(const CString& id)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(id);

    hr = spDisp.InvokeN(L"FindInputMethod",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::GetCursorPos(long * x,long * y)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;
    VARIANT t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    hr = spDisp.InvokeN(L"GetCursorPos",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::BindWindow(long hwnd, const CString& display, const CString& mouse, const CString& keypad, long mode)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    VARIANT vResult;

    pn[4] = COleVariant(hwnd);
    pn[3] = COleVariant(display);
    pn[2] = COleVariant(mouse);
    pn[1] = COleVariant(keypad);
    pn[0] = COleVariant(mode);

    hr = spDisp.InvokeN(L"BindWindow",pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FindWindow(const CString& class_name, const CString& title_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(class_name);
    pn[0] = COleVariant(title_name);

    hr = spDisp.InvokeN(L"FindWindow",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::GetScreenDepth()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"GetScreenDepth",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::SetScreen(long width,long height,long depth)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(width);
    pn[1] = COleVariant(height);
    pn[0] = COleVariant(depth);

    hr = spDisp.InvokeN(L"SetScreen",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::ExitOs(long tpe)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(tpe);

    hr = spDisp.InvokeN(L"ExitOs",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::GetDir(long tpe)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(tpe);

    hr = spDisp.InvokeN(L"GetDir",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::GetOsType()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"GetOsType",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FindWindowEx(long parent, const CString& class_name, const CString& title_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(parent);
    pn[1] = COleVariant(class_name);
    pn[0] = COleVariant(title_name);

    hr = spDisp.InvokeN(L"FindWindowEx",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::SetExportDict(long index, const CString& dict_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(index);
    pn[0] = COleVariant(dict_name);

    hr = spDisp.InvokeN(L"SetExportDict",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::GetCursorShape()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"GetCursorShape",&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::DownCpu(long rate)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(rate);

    hr = spDisp.InvokeN(L"DownCpu",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::GetCursorSpot()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"GetCursorSpot",&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::SendString2(long hwnd, const CString& str)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(str);

    hr = spDisp.InvokeN(L"SendString2",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FaqPost(const CString& server, long handle, long request_type, long time_out)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(server);
    pn[2] = COleVariant(handle);
    pn[1] = COleVariant(request_type);
    pn[0] = COleVariant(time_out);

    hr = spDisp.InvokeN(L"FaqPost",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::FaqFetch()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"FaqFetch",&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::FetchWord(long x1, long y1, long x2, long y2, const CString& color, const CString& word)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    VARIANT vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(word);

    hr = spDisp.InvokeN(L"FetchWord",pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::CaptureJpg(long x1, long y1, long x2, long y2, const CString& file_name, long quality)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    VARIANT vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(quality);

    hr = spDisp.InvokeN(L"CaptureJpg",pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FindStrWithFont(long x1, long y1, long x2, long y2, const CString& str, const CString& color, double sim, const CString& font_name, long font_size, long flag, long * x, long * y)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[12];
    VARIANT vResult;
    VARIANT t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[11] = COleVariant(x1);
    pn[10] = COleVariant(y1);
    pn[9] = COleVariant(x2);
    pn[8] = COleVariant(y2);
    pn[7] = COleVariant(str);
    pn[6] = COleVariant(color);
    pn[5] = COleVariant(sim);
    pn[4] = COleVariant(font_name);
    pn[3] = COleVariant(font_size);
    pn[2] = COleVariant(flag);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    hr = spDisp.InvokeN(L"FindStrWithFont",pn,12,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::FindStrWithFontE(long x1, long y1, long x2, long y2, const CString& str, const CString& color, double sim, const CString& font_name, long font_size, long flag)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[10];
    VARIANT vResult;

    pn[9] = COleVariant(x1);
    pn[8] = COleVariant(y1);
    pn[7] = COleVariant(x2);
    pn[6] = COleVariant(y2);
    pn[5] = COleVariant(str);
    pn[4] = COleVariant(color);
    pn[3] = COleVariant(sim);
    pn[2] = COleVariant(font_name);
    pn[1] = COleVariant(font_size);
    pn[0] = COleVariant(flag);

    hr = spDisp.InvokeN(L"FindStrWithFontE",pn,10,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::FindStrWithFontEx(long x1, long y1, long x2, long y2, const CString& str, const CString& color, double sim, const CString& font_name, long font_size, long flag)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[10];
    VARIANT vResult;

    pn[9] = COleVariant(x1);
    pn[8] = COleVariant(y1);
    pn[7] = COleVariant(x2);
    pn[6] = COleVariant(y2);
    pn[5] = COleVariant(str);
    pn[4] = COleVariant(color);
    pn[3] = COleVariant(sim);
    pn[2] = COleVariant(font_name);
    pn[1] = COleVariant(font_size);
    pn[0] = COleVariant(flag);

    hr = spDisp.InvokeN(L"FindStrWithFontEx",pn,10,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::GetDictInfo(const CString& str, const CString& font_name, long font_size, long flag)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(str);
    pn[2] = COleVariant(font_name);
    pn[1] = COleVariant(font_size);
    pn[0] = COleVariant(flag);

    hr = spDisp.InvokeN(L"GetDictInfo",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::SaveDict(long index, const CString& file_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(index);
    pn[0] = COleVariant(file_name);

    hr = spDisp.InvokeN(L"SaveDict",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::GetWindowProcessId(long hwnd)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(hwnd);

    hr = spDisp.InvokeN(L"GetWindowProcessId",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::GetWindowProcessPath(long hwnd)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(hwnd);

    hr = spDisp.InvokeN(L"GetWindowProcessPath",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::LockInput(long locks)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(locks);

    hr = spDisp.InvokeN(L"LockInput",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::GetPicSize(const CString& pic_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(pic_name);

    hr = spDisp.InvokeN(L"GetPicSize",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::GetID()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"GetID",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::CapturePng(long x1, long y1, long x2, long y2, const CString& file_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    VARIANT vResult;

    pn[4] = COleVariant(x1);
    pn[3] = COleVariant(y1);
    pn[2] = COleVariant(x2);
    pn[1] = COleVariant(y2);
    pn[0] = COleVariant(file_name);

    hr = spDisp.InvokeN(L"CapturePng",pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::CaptureGif(long x1, long y1, long x2, long y2, const CString& file_name, long delay, long time)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    VARIANT vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(file_name);
    pn[1] = COleVariant(delay);
    pn[0] = COleVariant(time);

    hr = spDisp.InvokeN(L"CaptureGif",pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::ImageToBmp(const CString& pic_name, const CString& bmp_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(pic_name);
    pn[0] = COleVariant(bmp_name);

    hr = spDisp.InvokeN(L"ImageToBmp",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FindStrFast(long x1, long y1, long x2, long y2, const CString& str, const CString& color, double sim, long * x, long * y)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[9];
    VARIANT vResult;
    VARIANT t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[8] = COleVariant(x1);
    pn[7] = COleVariant(y1);
    pn[6] = COleVariant(x2);
    pn[5] = COleVariant(y2);
    pn[4] = COleVariant(str);
    pn[3] = COleVariant(color);
    pn[2] = COleVariant(sim);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    hr = spDisp.InvokeN(L"FindStrFast",pn,9,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::FindStrFastEx(long x1, long y1, long x2, long y2, const CString& str, const CString& color, double sim)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    VARIANT vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(str);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    hr = spDisp.InvokeN(L"FindStrFastEx",pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::FindStrFastE(long x1, long y1, long x2, long y2, const CString& str, const CString& color, double sim)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    VARIANT vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(str);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    hr = spDisp.InvokeN(L"FindStrFastE",pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::EnableDisplayDebug(long enable_debug)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(enable_debug);

    hr = spDisp.InvokeN(L"EnableDisplayDebug",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::CapturePre(const CString& file_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(file_name);

    hr = spDisp.InvokeN(L"CapturePre",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::RegEx(const CString& code, const CString& Ver, const CString& ip)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(code);
    pn[1] = COleVariant(Ver);
    pn[0] = COleVariant(ip);

    hr = spDisp.InvokeN(L"RegEx",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::GetMachineCode()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"GetMachineCode",&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::SetClipboard(const CString& data)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(data);

    hr = spDisp.InvokeN(L"SetClipboard",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::GetClipboard()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"GetClipboard",&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::GetNowDict()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"GetNowDict",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::Is64Bit()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"Is64Bit",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::GetColorNum(long x1, long y1, long x2, long y2, const CString& color, double sim)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    VARIANT vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    hr = spDisp.InvokeN(L"GetColorNum",pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::EnumWindowByProcess(const CString& process_name, const CString& title, const CString& class_name, long filter)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(process_name);
    pn[2] = COleVariant(title);
    pn[1] = COleVariant(class_name);
    pn[0] = COleVariant(filter);

    hr = spDisp.InvokeN(L"EnumWindowByProcess",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::GetDictCount(long index)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(index);

    hr = spDisp.InvokeN(L"GetDictCount",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::GetLastError()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"GetLastError",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::GetNetTime()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"GetNetTime",&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::EnableGetColorByCapture(long en)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(en);

    hr = spDisp.InvokeN(L"EnableGetColorByCapture",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::CheckUAC()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"CheckUAC",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::SetUAC(long uac)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(uac);

    hr = spDisp.InvokeN(L"SetUAC",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::DisableFontSmooth()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"DisableFontSmooth",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::CheckFontSmooth()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"CheckFontSmooth",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::SetDisplayAcceler(long level)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(level);

    hr = spDisp.InvokeN(L"SetDisplayAcceler",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FindWindowByProcess(const CString& process_name, const CString& class_name, const CString& title_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(process_name);
    pn[1] = COleVariant(class_name);
    pn[0] = COleVariant(title_name);

    hr = spDisp.InvokeN(L"FindWindowByProcess",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FindWindowByProcessId(long process_id, const CString& class_name, const CString& title_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(process_id);
    pn[1] = COleVariant(class_name);
    pn[0] = COleVariant(title_name);

    hr = spDisp.InvokeN(L"FindWindowByProcessId",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::ReadIni(const CString& section, const CString& key, const CString& file_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(section);
    pn[1] = COleVariant(key);
    pn[0] = COleVariant(file_name);

    hr = spDisp.InvokeN(L"ReadIni",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::WriteIni(const CString& section, const CString& key, const CString& v, const CString& file_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(section);
    pn[2] = COleVariant(key);
    pn[1] = COleVariant(v);
    pn[0] = COleVariant(file_name);

    hr = spDisp.InvokeN(L"WriteIni",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::RunApp(const CString& path, long mode)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(path);
    pn[0] = COleVariant(mode);

    hr = spDisp.InvokeN(L"RunApp",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::delay(long mis)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(mis);

    hr = spDisp.InvokeN(L"delay",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FindWindowSuper(const CString& spec1, long flag1, long type1, const CString& spec2, long flag2, long type2)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    VARIANT vResult;

    pn[5] = COleVariant(spec1);
    pn[4] = COleVariant(flag1);
    pn[3] = COleVariant(type1);
    pn[2] = COleVariant(spec2);
    pn[1] = COleVariant(flag2);
    pn[0] = COleVariant(type2);

    hr = spDisp.InvokeN(L"FindWindowSuper",pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::ExcludePos(const CString& all_pos, long tpe, long x1, long y1, long x2, long y2)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    VARIANT vResult;

    pn[5] = COleVariant(all_pos);
    pn[4] = COleVariant(tpe);
    pn[3] = COleVariant(x1);
    pn[2] = COleVariant(y1);
    pn[1] = COleVariant(x2);
    pn[0] = COleVariant(y2);

    hr = spDisp.InvokeN(L"ExcludePos",pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::FindNearestPos(const CString& all_pos, long tpe, long x, long y)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(all_pos);
    pn[2] = COleVariant(tpe);
    pn[1] = COleVariant(x);
    pn[0] = COleVariant(y);

    hr = spDisp.InvokeN(L"FindNearestPos",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::SortPosDistance(const CString& all_pos, long tpe, long x, long y)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(all_pos);
    pn[2] = COleVariant(tpe);
    pn[1] = COleVariant(x);
    pn[0] = COleVariant(y);

    hr = spDisp.InvokeN(L"SortPosDistance",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::FindPicMem(long x1, long y1, long x2, long y2, const CString& pic_info, const CString& delta_color, double sim, long dir, long * x, long * y)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[10];
    VARIANT vResult;
    VARIANT t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[9] = COleVariant(x1);
    pn[8] = COleVariant(y1);
    pn[7] = COleVariant(x2);
    pn[6] = COleVariant(y2);
    pn[5] = COleVariant(pic_info);
    pn[4] = COleVariant(delta_color);
    pn[3] = COleVariant(sim);
    pn[2] = COleVariant(dir);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    hr = spDisp.InvokeN(L"FindPicMem",pn,10,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::FindPicMemEx(long x1, long y1, long x2, long y2, const CString& pic_info, const CString& delta_color, double sim, long dir)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    VARIANT vResult;

    pn[7] = COleVariant(x1);
    pn[6] = COleVariant(y1);
    pn[5] = COleVariant(x2);
    pn[4] = COleVariant(y2);
    pn[3] = COleVariant(pic_info);
    pn[2] = COleVariant(delta_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    hr = spDisp.InvokeN(L"FindPicMemEx",pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::FindPicMemE(long x1, long y1, long x2, long y2, const CString& pic_info, const CString& delta_color, double sim, long dir)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    VARIANT vResult;

    pn[7] = COleVariant(x1);
    pn[6] = COleVariant(y1);
    pn[5] = COleVariant(x2);
    pn[4] = COleVariant(y2);
    pn[3] = COleVariant(pic_info);
    pn[2] = COleVariant(delta_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    hr = spDisp.InvokeN(L"FindPicMemE",pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::AppendPicAddr(const CString& pic_info, long addr, long size)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(pic_info);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(size);

    hr = spDisp.InvokeN(L"AppendPicAddr",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::WriteFile(const CString& file_name, const CString& content)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(content);

    hr = spDisp.InvokeN(L"WriteFile",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::Stop(long id)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(id);

    hr = spDisp.InvokeN(L"Stop",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::SetDictMem(long index,long addr,long size)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(index);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(size);

    hr = spDisp.InvokeN(L"SetDictMem",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::GetNetTimeSafe()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"GetNetTimeSafe",&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::ForceUnBindWindow(long hwnd)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(hwnd);

    hr = spDisp.InvokeN(L"ForceUnBindWindow",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::ReadIniPwd(const CString& section, const CString& key, const CString& file_name, const CString& pwd)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(section);
    pn[2] = COleVariant(key);
    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(pwd);

    hr = spDisp.InvokeN(L"ReadIniPwd",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::WriteIniPwd(const CString& section, const CString& key, const CString& v, const CString& file_name, const CString& pwd)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    VARIANT vResult;

    pn[4] = COleVariant(section);
    pn[3] = COleVariant(key);
    pn[2] = COleVariant(v);
    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(pwd);

    hr = spDisp.InvokeN(L"WriteIniPwd",pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::DecodeFile(const CString& file_name, const CString& pwd)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(pwd);

    hr = spDisp.InvokeN(L"DecodeFile",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::KeyDownChar(const CString& key_str)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(key_str);

    hr = spDisp.InvokeN(L"KeyDownChar",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::KeyUpChar(const CString& key_str)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(key_str);

    hr = spDisp.InvokeN(L"KeyUpChar",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::KeyPressChar(const CString& key_str)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(key_str);

    hr = spDisp.InvokeN(L"KeyPressChar",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::KeyPressStr(const CString& key_str, long delay)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(key_str);
    pn[0] = COleVariant(delay);

    hr = spDisp.InvokeN(L"KeyPressStr",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::EnableKeypadPatch(long en)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(en);

    hr = spDisp.InvokeN(L"EnableKeypadPatch",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::EnableKeypadSync(long en,long time_out)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(en);
    pn[0] = COleVariant(time_out);

    hr = spDisp.InvokeN(L"EnableKeypadSync",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::EnableMouseSync(long en,long time_out)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(en);
    pn[0] = COleVariant(time_out);

    hr = spDisp.InvokeN(L"EnableMouseSync",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::DmGuard(long en, const CString& tpe)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(en);
    pn[0] = COleVariant(tpe);

    hr = spDisp.InvokeN(L"DmGuard",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FaqCaptureFromFile(long x1, long y1, long x2, long y2, const CString& file_name, long quality)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    VARIANT vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(quality);

    hr = spDisp.InvokeN(L"FaqCaptureFromFile",pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::FindIntEx(long hwnd, const CString& addr_range, long int_value_min, long int_value_max, long tpe, long steps, long multi_thread, long mode)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    VARIANT vResult;

    pn[7] = COleVariant(hwnd);
    pn[6] = COleVariant(addr_range);
    pn[5] = COleVariant(int_value_min);
    pn[4] = COleVariant(int_value_max);
    pn[3] = COleVariant(tpe);
    pn[2] = COleVariant(steps);
    pn[1] = COleVariant(multi_thread);
    pn[0] = COleVariant(mode);

    hr = spDisp.InvokeN(L"FindIntEx",pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::FindFloatEx(long hwnd, const CString& addr_range, float float_value_min, float float_value_max, long steps, long multi_thread, long mode)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    VARIANT vResult;

    pn[6] = COleVariant(hwnd);
    pn[5] = COleVariant(addr_range);
    pn[4] = COleVariant(float_value_min);
    pn[3] = COleVariant(float_value_max);
    pn[2] = COleVariant(steps);
    pn[1] = COleVariant(multi_thread);
    pn[0] = COleVariant(mode);

    hr = spDisp.InvokeN(L"FindFloatEx",pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::FindDoubleEx(long hwnd, const CString& addr_range, double double_value_min, double double_value_max, long steps, long multi_thread, long mode)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    VARIANT vResult;

    pn[6] = COleVariant(hwnd);
    pn[5] = COleVariant(addr_range);
    pn[4] = COleVariant(double_value_min);
    pn[3] = COleVariant(double_value_max);
    pn[2] = COleVariant(steps);
    pn[1] = COleVariant(multi_thread);
    pn[0] = COleVariant(mode);

    hr = spDisp.InvokeN(L"FindDoubleEx",pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::FindStringEx(long hwnd, const CString& addr_range, const CString& string_value, long tpe, long steps, long multi_thread, long mode)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    VARIANT vResult;

    pn[6] = COleVariant(hwnd);
    pn[5] = COleVariant(addr_range);
    pn[4] = COleVariant(string_value);
    pn[3] = COleVariant(tpe);
    pn[2] = COleVariant(steps);
    pn[1] = COleVariant(multi_thread);
    pn[0] = COleVariant(mode);

    hr = spDisp.InvokeN(L"FindStringEx",pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::FindDataEx(long hwnd, const CString& addr_range, const CString& data, long steps, long multi_thread, long mode)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    VARIANT vResult;

    pn[5] = COleVariant(hwnd);
    pn[4] = COleVariant(addr_range);
    pn[3] = COleVariant(data);
    pn[2] = COleVariant(steps);
    pn[1] = COleVariant(multi_thread);
    pn[0] = COleVariant(mode);

    hr = spDisp.InvokeN(L"FindDataEx",pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::EnableRealMouse(long en,long mousedelay,long mousestep)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(en);
    pn[1] = COleVariant(mousedelay);
    pn[0] = COleVariant(mousestep);

    hr = spDisp.InvokeN(L"EnableRealMouse",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::EnableRealKeypad(long en)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(en);

    hr = spDisp.InvokeN(L"EnableRealKeypad",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::SendStringIme(const CString& str)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(str);

    hr = spDisp.InvokeN(L"SendStringIme",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FoobarDrawLine(long hwnd, long x1, long y1, long x2, long y2, const CString& color, long style, long width)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    VARIANT vResult;

    pn[7] = COleVariant(hwnd);
    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(color);
    pn[1] = COleVariant(style);
    pn[0] = COleVariant(width);

    hr = spDisp.InvokeN(L"FoobarDrawLine",pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::FindStrEx(long x1, long y1, long x2, long y2, const CString& str, const CString& color, double sim)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    VARIANT vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(str);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    hr = spDisp.InvokeN(L"FindStrEx",pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::IsBind(long hwnd)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(hwnd);

    hr = spDisp.InvokeN(L"IsBind",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::SetDisplayDelay(long t)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(t);

    hr = spDisp.InvokeN(L"SetDisplayDelay",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::GetDmCount()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"GetDmCount",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::DisableScreenSave()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"DisableScreenSave",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::DisablePowerSave()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"DisablePowerSave",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::SetMemoryHwndAsProcessId(long en)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(en);

    hr = spDisp.InvokeN(L"SetMemoryHwndAsProcessId",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FindShape(long x1, long y1, long x2, long y2, const CString& offset_color, double sim, long dir, long * x, long * y)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[9];
    VARIANT vResult;
    VARIANT t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[8] = COleVariant(x1);
    pn[7] = COleVariant(y1);
    pn[6] = COleVariant(x2);
    pn[5] = COleVariant(y2);
    pn[4] = COleVariant(offset_color);
    pn[3] = COleVariant(sim);
    pn[2] = COleVariant(dir);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    hr = spDisp.InvokeN(L"FindShape",pn,9,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::FindShapeE(long x1, long y1, long x2, long y2, const CString& offset_color, double sim, long dir)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    VARIANT vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(offset_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    hr = spDisp.InvokeN(L"FindShapeE",pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::FindShapeEx(long x1, long y1, long x2, long y2, const CString& offset_color, double sim, long dir)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    VARIANT vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(offset_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    hr = spDisp.InvokeN(L"FindShapeEx",pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::FindStrS(long x1, long y1, long x2, long y2, const CString& str, const CString& color, double sim, long * x, long * y)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[9];
    VARIANT vResult;
    VARIANT t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[8] = COleVariant(x1);
    pn[7] = COleVariant(y1);
    pn[6] = COleVariant(x2);
    pn[5] = COleVariant(y2);
    pn[4] = COleVariant(str);
    pn[3] = COleVariant(color);
    pn[2] = COleVariant(sim);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    hr = spDisp.InvokeN(L"FindStrS",pn,9,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::FindStrExS(long x1, long y1, long x2, long y2, const CString& str, const CString& color, double sim)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    VARIANT vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(str);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    hr = spDisp.InvokeN(L"FindStrExS",pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::FindStrFastS(long x1, long y1, long x2, long y2, const CString& str, const CString& color, double sim, long * x, long * y)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[9];
    VARIANT vResult;
    VARIANT t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[8] = COleVariant(x1);
    pn[7] = COleVariant(y1);
    pn[6] = COleVariant(x2);
    pn[5] = COleVariant(y2);
    pn[4] = COleVariant(str);
    pn[3] = COleVariant(color);
    pn[2] = COleVariant(sim);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    hr = spDisp.InvokeN(L"FindStrFastS",pn,9,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::FindStrFastExS(long x1, long y1, long x2, long y2, const CString& str, const CString& color, double sim)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    VARIANT vResult;

    pn[6] = COleVariant(x1);
    pn[5] = COleVariant(y1);
    pn[4] = COleVariant(x2);
    pn[3] = COleVariant(y2);
    pn[2] = COleVariant(str);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    hr = spDisp.InvokeN(L"FindStrFastExS",pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::FindPicS(long x1, long y1, long x2, long y2, const CString& pic_name, const CString& delta_color, double sim, long dir, long * x, long * y)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[10];
    VARIANT vResult;
    VARIANT t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[9] = COleVariant(x1);
    pn[8] = COleVariant(y1);
    pn[7] = COleVariant(x2);
    pn[6] = COleVariant(y2);
    pn[5] = COleVariant(pic_name);
    pn[4] = COleVariant(delta_color);
    pn[3] = COleVariant(sim);
    pn[2] = COleVariant(dir);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    hr = spDisp.InvokeN(L"FindPicS",pn,10,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::FindPicExS(long x1, long y1, long x2, long y2, const CString& pic_name, const CString& delta_color, double sim, long dir)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[8];
    VARIANT vResult;

    pn[7] = COleVariant(x1);
    pn[6] = COleVariant(y1);
    pn[5] = COleVariant(x2);
    pn[4] = COleVariant(y2);
    pn[3] = COleVariant(pic_name);
    pn[2] = COleVariant(delta_color);
    pn[1] = COleVariant(sim);
    pn[0] = COleVariant(dir);

    hr = spDisp.InvokeN(L"FindPicExS",pn,8,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::ClearDict(long index)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(index);

    hr = spDisp.InvokeN(L"ClearDict",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::GetMachineCodeNoMac()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"GetMachineCodeNoMac",&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::GetClientRect(long hwnd,long * x1,long * y1,long * x2,long * y2)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    VARIANT vResult;
    VARIANT t0,t1,t2,t3;

    VariantInit(&t0);
    VariantInit(&t1);
    VariantInit(&t2);
    VariantInit(&t3);

    pn[4] = COleVariant(hwnd);
    pn[3].vt = VT_BYREF|VT_VARIANT;
    pn[3].pvarVal = &t0;
    pn[2].vt = VT_BYREF|VT_VARIANT;
    pn[2].pvarVal = &t1;
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t2;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t3;

    hr = spDisp.InvokeN(L"GetClientRect",pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        *x1 = t0.lVal;
        *y1 = t1.lVal;
        *x2 = t2.lVal;
        *y2 = t3.lVal;
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::EnableFakeActive(long en)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(en);

    hr = spDisp.InvokeN(L"EnableFakeActive",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::GetScreenDataBmp(long x1,long y1,long x2,long y2,long * data,long * size)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    VARIANT vResult;
    VARIANT t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    hr = spDisp.InvokeN(L"GetScreenDataBmp",pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        *data = t0.lVal;
        *size = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::EncodeFile(const CString& file_name, const CString& pwd)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(pwd);

    hr = spDisp.InvokeN(L"EncodeFile",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::GetCursorShapeEx(long tpe)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(tpe);

    hr = spDisp.InvokeN(L"GetCursorShapeEx",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::FaqCancel()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"FaqCancel",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::IntToData(long int_value,long tpe)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(int_value);
    pn[0] = COleVariant(tpe);

    hr = spDisp.InvokeN(L"IntToData",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::FloatToData(float float_value)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(float_value);

    hr = spDisp.InvokeN(L"FloatToData",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::DoubleToData(double double_value)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(double_value);

    hr = spDisp.InvokeN(L"DoubleToData",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::StringToData(const CString& string_value, long tpe)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(string_value);
    pn[0] = COleVariant(tpe);

    hr = spDisp.InvokeN(L"StringToData",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::SetMemoryFindResultToFile(const CString& file_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(file_name);

    hr = spDisp.InvokeN(L"SetMemoryFindResultToFile",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::EnableBind(long en)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(en);

    hr = spDisp.InvokeN(L"EnableBind",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::SetSimMode(long mode)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(mode);

    hr = spDisp.InvokeN(L"SetSimMode",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::LockMouseRect(long x1,long y1,long x2,long y2)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(x1);
    pn[2] = COleVariant(y1);
    pn[1] = COleVariant(x2);
    pn[0] = COleVariant(y2);

    hr = spDisp.InvokeN(L"LockMouseRect",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::SendPaste(long hwnd)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(hwnd);

    hr = spDisp.InvokeN(L"SendPaste",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::IsDisplayDead(long x1,long y1,long x2,long y2,long t)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    VARIANT vResult;

    pn[4] = COleVariant(x1);
    pn[3] = COleVariant(y1);
    pn[2] = COleVariant(x2);
    pn[1] = COleVariant(y2);
    pn[0] = COleVariant(t);

    hr = spDisp.InvokeN(L"IsDisplayDead",pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::GetKeyState(long vk)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(vk);

    hr = spDisp.InvokeN(L"GetKeyState",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::CopyFile(const CString& src_file, const CString& dst_file, long over)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(src_file);
    pn[1] = COleVariant(dst_file);
    pn[0] = COleVariant(over);

    hr = spDisp.InvokeN(L"CopyFile",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::IsFileExist(const CString& file_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(file_name);

    hr = spDisp.InvokeN(L"IsFileExist",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::DeleteFile(const CString& file_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(file_name);

    hr = spDisp.InvokeN(L"DeleteFile",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::MoveFile(const CString& src_file, const CString& dst_file)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(src_file);
    pn[0] = COleVariant(dst_file);

    hr = spDisp.InvokeN(L"MoveFile",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::CreateFolder(const CString& folder_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(folder_name);

    hr = spDisp.InvokeN(L"CreateFolder",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::DeleteFolder(const CString& folder_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(folder_name);

    hr = spDisp.InvokeN(L"DeleteFolder",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::GetFileLength(const CString& file_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(file_name);

    hr = spDisp.InvokeN(L"GetFileLength",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::ReadFile(const CString& file_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(file_name);

    hr = spDisp.InvokeN(L"ReadFile",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::WaitKey(long key_code,long time_out)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(key_code);
    pn[0] = COleVariant(time_out);

    hr = spDisp.InvokeN(L"WaitKey",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::DeleteIni(const CString& section, const CString& key, const CString& file_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(section);
    pn[1] = COleVariant(key);
    pn[0] = COleVariant(file_name);

    hr = spDisp.InvokeN(L"DeleteIni",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::DeleteIniPwd(const CString& section, const CString& key, const CString& file_name, const CString& pwd)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(section);
    pn[2] = COleVariant(key);
    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(pwd);

    hr = spDisp.InvokeN(L"DeleteIniPwd",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::EnableSpeedDx(long en)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(en);

    hr = spDisp.InvokeN(L"EnableSpeedDx",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::EnableIme(long en)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(en);

    hr = spDisp.InvokeN(L"EnableIme",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::Reg(const CString& code, const CString& Ver)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(code);
    pn[0] = COleVariant(Ver);

    hr = spDisp.InvokeN(L"Reg",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::SelectFile()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"SelectFile",&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::SelectDirectory()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"SelectDirectory",&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::LockDisplay(long locks)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(locks);

    hr = spDisp.InvokeN(L"LockDisplay",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FoobarSetSave(long hwnd, const CString& file_name, long en, const CString& header)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(file_name);
    pn[1] = COleVariant(en);
    pn[0] = COleVariant(header);

    hr = spDisp.InvokeN(L"FoobarSetSave",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::EnumWindowSuper(const CString& spec1, long flag1, long type1, const CString& spec2, long flag2, long type2, long sort)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[7];
    VARIANT vResult;

    pn[6] = COleVariant(spec1);
    pn[5] = COleVariant(flag1);
    pn[4] = COleVariant(type1);
    pn[3] = COleVariant(spec2);
    pn[2] = COleVariant(flag2);
    pn[1] = COleVariant(type2);
    pn[0] = COleVariant(sort);

    hr = spDisp.InvokeN(L"EnumWindowSuper",pn,7,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::DownloadFile(const CString& url, const CString& save_file, long timeout)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(url);
    pn[1] = COleVariant(save_file);
    pn[0] = COleVariant(timeout);

    hr = spDisp.InvokeN(L"DownloadFile",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::EnableKeypadMsg(long en)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(en);

    hr = spDisp.InvokeN(L"EnableKeypadMsg",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::EnableMouseMsg(long en)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(en);

    hr = spDisp.InvokeN(L"EnableMouseMsg",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::RegNoMac(const CString& code, const CString& Ver)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(code);
    pn[0] = COleVariant(Ver);

    hr = spDisp.InvokeN(L"RegNoMac",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::RegExNoMac(const CString& code, const CString& Ver, const CString& ip)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(code);
    pn[1] = COleVariant(Ver);
    pn[0] = COleVariant(ip);

    hr = spDisp.InvokeN(L"RegExNoMac",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::SetEnumWindowDelay(long delay)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(delay);

    hr = spDisp.InvokeN(L"SetEnumWindowDelay",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FindMulColor(long x1, long y1, long x2, long y2, const CString& color, double sim)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    VARIANT vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    hr = spDisp.InvokeN(L"FindMulColor",pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::GetDict(long index,long font_index)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(index);
    pn[0] = COleVariant(font_index);

    hr = spDisp.InvokeN(L"GetDict",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::GetBindWindow()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"GetBindWindow",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FoobarStartGif(long hwnd, long x, long y, const CString& pic_name, long repeat_limit, long delay)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    VARIANT vResult;

    pn[5] = COleVariant(hwnd);
    pn[4] = COleVariant(x);
    pn[3] = COleVariant(y);
    pn[2] = COleVariant(pic_name);
    pn[1] = COleVariant(repeat_limit);
    pn[0] = COleVariant(delay);

    hr = spDisp.InvokeN(L"FoobarStartGif",pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FoobarStopGif(long hwnd, long x, long y, const CString& pic_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(x);
    pn[1] = COleVariant(y);
    pn[0] = COleVariant(pic_name);

    hr = spDisp.InvokeN(L"FoobarStopGif",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FreeProcessMemory(long hwnd)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(hwnd);

    hr = spDisp.InvokeN(L"FreeProcessMemory",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::ReadFileData(const CString& file_name, long start_pos, long end_pos)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(file_name);
    pn[1] = COleVariant(start_pos);
    pn[0] = COleVariant(end_pos);

    hr = spDisp.InvokeN(L"ReadFileData",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::VirtualAllocEx(long hwnd,long addr,long size,long tpe)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr);
    pn[1] = COleVariant(size);
    pn[0] = COleVariant(tpe);

    hr = spDisp.InvokeN(L"VirtualAllocEx",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::VirtualFreeEx(long hwnd,long addr)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(addr);

    hr = spDisp.InvokeN(L"VirtualFreeEx",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::GetCommandLine(long hwnd)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(hwnd);

    hr = spDisp.InvokeN(L"GetCommandLine",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::TerminateProcess(long pid)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(pid);

    hr = spDisp.InvokeN(L"TerminateProcess",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::GetNetTimeByIp(const CString& ip)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(ip);

    hr = spDisp.InvokeN(L"GetNetTimeByIp",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::EnumProcess(const CString& name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(name);

    hr = spDisp.InvokeN(L"EnumProcess",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::GetProcessInfo(long pid)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(pid);

    hr = spDisp.InvokeN(L"GetProcessInfo",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::ReadIntAddr(long hwnd,long addr,long tpe)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(tpe);

    hr = spDisp.InvokeN(L"ReadIntAddr",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::ReadDataAddr(long hwnd,long addr,long length)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(length);

    hr = spDisp.InvokeN(L"ReadDataAddr",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

double dmsoft::ReadDoubleAddr(long hwnd,long addr)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(addr);

    hr = spDisp.InvokeN(L"ReadDoubleAddr",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.dblVal;
    }
    return 0.0;
}

float dmsoft::ReadFloatAddr(long hwnd,long addr)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(hwnd);
    pn[0] = COleVariant(addr);

    hr = spDisp.InvokeN(L"ReadFloatAddr",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.fltVal;
    }
    return 0.0f;
}

CString dmsoft::ReadStringAddr(long hwnd,long addr,long tpe,long length)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr);
    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(length);

    hr = spDisp.InvokeN(L"ReadStringAddr",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::WriteDataAddr(long hwnd, long addr, const CString& data)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(data);

    hr = spDisp.InvokeN(L"WriteDataAddr",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::WriteDoubleAddr(long hwnd,long addr,double v)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(v);

    hr = spDisp.InvokeN(L"WriteDoubleAddr",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::WriteFloatAddr(long hwnd,long addr,float v)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(addr);
    pn[0] = COleVariant(v);

    hr = spDisp.InvokeN(L"WriteFloatAddr",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::WriteIntAddr(long hwnd,long addr,long tpe,long v)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr);
    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(v);

    hr = spDisp.InvokeN(L"WriteIntAddr",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::WriteStringAddr(long hwnd, long addr, long tpe, const CString& v)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(addr);
    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(v);

    hr = spDisp.InvokeN(L"WriteStringAddr",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::Delays(long min_s,long max_s)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(min_s);
    pn[0] = COleVariant(max_s);

    hr = spDisp.InvokeN(L"Delays",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FindColorBlock(long x1, long y1, long x2, long y2, const CString& color, double sim, long count, long width, long height, long * x, long * y)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[11];
    VARIANT vResult;
    VARIANT t0,t1;

    VariantInit(&t0);
    VariantInit(&t1);

    pn[10] = COleVariant(x1);
    pn[9] = COleVariant(y1);
    pn[8] = COleVariant(x2);
    pn[7] = COleVariant(y2);
    pn[6] = COleVariant(color);
    pn[5] = COleVariant(sim);
    pn[4] = COleVariant(count);
    pn[3] = COleVariant(width);
    pn[2] = COleVariant(height);
    pn[1].vt = VT_BYREF|VT_VARIANT;
    pn[1].pvarVal = &t0;
    pn[0].vt = VT_BYREF|VT_VARIANT;
    pn[0].pvarVal = &t1;

    hr = spDisp.InvokeN(L"FindColorBlock",pn,11,&vResult);
    if (SUCCEEDED(hr))
    {
        *x = t0.lVal;
        *y = t1.lVal;
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::FindColorBlockEx(long x1, long y1, long x2, long y2, const CString& color, double sim, long count, long width, long height)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[9];
    VARIANT vResult;

    pn[8] = COleVariant(x1);
    pn[7] = COleVariant(y1);
    pn[6] = COleVariant(x2);
    pn[5] = COleVariant(y2);
    pn[4] = COleVariant(color);
    pn[3] = COleVariant(sim);
    pn[2] = COleVariant(count);
    pn[1] = COleVariant(width);
    pn[0] = COleVariant(height);

    hr = spDisp.InvokeN(L"FindColorBlockEx",pn,9,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::OpenProcess(long pid)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(pid);

    hr = spDisp.InvokeN(L"OpenProcess",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::EnumIniSection(const CString& file_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(file_name);

    hr = spDisp.InvokeN(L"EnumIniSection",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::EnumIniSectionPwd(const CString& file_name, const CString& pwd)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(pwd);

    hr = spDisp.InvokeN(L"EnumIniSectionPwd",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::EnumIniKey(const CString& section, const CString& file_name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(section);
    pn[0] = COleVariant(file_name);

    hr = spDisp.InvokeN(L"EnumIniKey",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::EnumIniKeyPwd(const CString& section, const CString& file_name, const CString& pwd)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(section);
    pn[1] = COleVariant(file_name);
    pn[0] = COleVariant(pwd);

    hr = spDisp.InvokeN(L"EnumIniKeyPwd",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::SwitchBindWindow(long hwnd)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(hwnd);

    hr = spDisp.InvokeN(L"SwitchBindWindow",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::InitCri()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"InitCri",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::SendStringIme2(long hwnd, const CString& str, long mode)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(hwnd);
    pn[1] = COleVariant(str);
    pn[0] = COleVariant(mode);

    hr = spDisp.InvokeN(L"SendStringIme2",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::EnumWindowByProcessId(long pid, const CString& title, const CString& class_name, long filter)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(pid);
    pn[2] = COleVariant(title);
    pn[1] = COleVariant(class_name);
    pn[0] = COleVariant(filter);

    hr = spDisp.InvokeN(L"EnumWindowByProcessId",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

CString dmsoft::GetDisplayInfo()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"GetDisplayInfo",&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::EnableFontSmooth()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"EnableFontSmooth",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::OcrExOne(long x1, long y1, long x2, long y2, const CString& color, double sim)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[6];
    VARIANT vResult;

    pn[5] = COleVariant(x1);
    pn[4] = COleVariant(y1);
    pn[3] = COleVariant(x2);
    pn[2] = COleVariant(y2);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    hr = spDisp.InvokeN(L"OcrExOne",pn,6,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::SetAero(long en)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(en);

    hr = spDisp.InvokeN(L"SetAero",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FoobarSetTrans(long hwnd, long trans, const CString& color, double sim)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[4];
    VARIANT vResult;

    pn[3] = COleVariant(hwnd);
    pn[2] = COleVariant(trans);
    pn[1] = COleVariant(color);
    pn[0] = COleVariant(sim);

    hr = spDisp.InvokeN(L"FoobarSetTrans",pn,4,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::EnablePicCache(long en)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(en);

    hr = spDisp.InvokeN(L"EnablePicCache",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

CString dmsoft::GetInfo(const CString& cmd, const CString& param)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[2];
    VARIANT vResult;

    pn[1] = COleVariant(cmd);
    pn[0] = COleVariant(param);

    hr = spDisp.InvokeN(L"GetInfo",pn,2,&vResult);
    if (SUCCEEDED(hr))
    {
        return  (BSTR)vResult.pbstrVal;
    }
    return  _T("");
}

long dmsoft::FaqIsPosted()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"FaqIsPosted",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::LoadPicByte(long addr, long size, const CString& name)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[3];
    VARIANT vResult;

    pn[2] = COleVariant(addr);
    pn[1] = COleVariant(size);
    pn[0] = COleVariant(name);

    hr = spDisp.InvokeN(L"LoadPicByte",pn,3,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::MiddleDown()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"MiddleDown",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::MiddleUp()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"MiddleUp",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::FaqCaptureString(const CString& str)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(str);

    hr = spDisp.InvokeN(L"FaqCaptureString",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::VirtualProtectEx(long hwnd,long addr,long size,long tpe,long old_protect)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[5];
    VARIANT vResult;

    pn[4] = COleVariant(hwnd);
    pn[3] = COleVariant(addr);
    pn[2] = COleVariant(size);
    pn[1] = COleVariant(tpe);
    pn[0] = COleVariant(old_protect);

    hr = spDisp.InvokeN(L"VirtualProtectEx",pn,5,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::SetMouseSpeed(long speed)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(speed);

    hr = spDisp.InvokeN(L"SetMouseSpeed",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::GetMouseSpeed()
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    VARIANT vResult;


    hr = spDisp.Invoke0(L"GetMouseSpeed",&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

long dmsoft::EnableMouseAccuracy(long en)
{
    MyDispatchDriver spDisp(obj);
    HRESULT hr;
    COleVariant pn[1];
    VARIANT vResult;

    pn[0] = COleVariant(en);

    hr = spDisp.InvokeN(L"EnableMouseAccuracy",pn,1,&vResult);
    if (SUCCEEDED(hr))
    {
        return vResult.lVal;
    }
    return 0;
}

