
#ifndef Macro_h__
#define Macro_h__

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x, message)                                               \
	{                                                           \
		HRESULT hr = (x);                                       \
		if(FAILED(hr))                                          \
		{                                                       \
			MessageBox(0, message, 0, 0);  \
		}                                                       \
	}
#endif

#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 

#define NULL_CHECK( _ptr)	\
{if( _ptr == 0){__asm { int 3 };return;}}

#define NULL_CHECK_RETURN( _ptr, _return)	\
{if( _ptr == 0){__asm { int 3 };return _return;}}

#define NULL_CHECK_MSG( _ptr, _message )		\
{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);__asm { int 3 };}}

#define NULL_CHECK_RETURN_MSG( _ptr, _return, _message )	\
{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);__asm { int 3 };return _return;}}


#define FAILED_CHECK(_hr)	if( ((HRESULT)(_hr)) < 0 )	\
{ __asm { int 3 }; return E_FAIL;}

#define FAILED_CHECK_RETURN(_hr, _return)	if( ((HRESULT)(_hr)) < 0 )		\
{ __asm { int 3 }; return _return;}

#define FAILED_CHECK_MSG( _hr, _message)	if( ((HRESULT)(_hr)) < 0 )	\
{ MessageBox(NULL, _message, L"System Message",MB_OK); __asm { int 3 };return E_FAIL;}

#define FAILED_CHECK_RETURN_MSG( _hr, _return, _message)	if( ((HRESULT)(_hr)) < 0 )	\
{ MessageBox(NULL, _message, L"System Message",MB_OK); __asm { int 3 };return _return;}


#define NO_COPY(CLASSNAME)							\
	private:										\
	CLASSNAME(const CLASSNAME&);					\
	CLASSNAME& operator = (const CLASSNAME&);		

#define DECLARE_SINGLETON(CLASSNAME)				\
	NO_COPY(CLASSNAME)								\
	private:										\
		static CLASSNAME*	m_pInstance;			\
		explicit			CLASSNAME(void);		\
	public:											\
		static CLASSNAME*	GetInstance( void );	\
		static void DestroyInstance( void );		\
		~CLASSNAME(void);

#define IMPLEMENT_SINGLETON(CLASSNAME)				\
	CLASSNAME*	CLASSNAME::m_pInstance = NULL;		\
	CLASSNAME*	CLASSNAME::GetInstance( void )	{	\
	if(NULL == m_pInstance) {					\
	m_pInstance = new CLASSNAME;			\
	}											\
	return m_pInstance;							\
	}												\
	void CLASSNAME::DestroyInstance( void ) {		\
	if(NULL != m_pInstance)	{					\
	m_pInstance->Release();					\
	m_pInstance = NULL;						\
	}											\
	}

#endif // Macro_h__