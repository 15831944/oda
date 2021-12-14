#if (defined(__BCPLUSPLUS__) || defined(__BORLANDC__)||defined(_WIN32_WCE))
#define _SA_annotes3(n,pp1,pp2,pp3)
#define _Group_impl_(annos)
#define _SAL_nop_impl_ X
#define _Group_(annos)                 _Group_impl_(annos _SAL_nop_impl_)
#define _SAL2_Source_(Name, args, annotes) _SA_annotes3(SAL_name, #Name, "", "2") _Group_(annotes _SAL_nop_impl_)
#define _Pre1_impl_(p1)
#define _Pre_valid_impl_
#define _Deref_pre1_impl_(p1)
#define _In_                            _SAL2_Source_(_In_, (), _Pre1_impl_(__notnull_impl_notref) _Pre_valid_impl_ _Deref_pre1_impl_(__readaccess_impl_notref))
#define _In_z_                          _SAL2_Source_(_In_z_, (),     _In_     _Pre1_impl_(__zterm_impl))
#define _Out_                                  _SAL2_Source_(_Out_, (),     _Out_impl_)
#define _ACRTIMP __declspec(dllimport)
_ACRTIMP int* __cdecl _errno(void);
#if !(defined(__BCPLUSPLUS__) || defined(__BORLANDC__))
#define errno (*_errno())
#endif
#define _Check_return_           _SAL2_Source_(_Check_return_, (), _Check_return_impl_)
#if !(defined(__BCPLUSPLUS__) || defined(__BORLANDC__))
#define _CRT_DEPRECATE_TEXT(_Text) __declspec(deprecated(_Text))
#define _CRT_NONSTDC_DEPRECATE(_NewName) _CRT_DEPRECATE_TEXT(             \
            "The POSIX name for this item is deprecated. Instead, use the ISO C " \
            "and C++ conformant name: " #_NewName ". See online help for details.")
_Check_return_ _CRT_NONSTDC_DEPRECATE(_stricmp)
#endif
_ACRTIMP int __cdecl stricmp(
    _In_z_ char const* _String1,
    _In_z_ char const* _String2
    );
#define strcasecmp stricmp

#define _Inout_updates_z_(size)                _SAL2_Source_(_Inout_updates_z_, (size), _Pre_cap_(size)         _Pre_valid_impl_ _Post_valid_impl_ _Pre1_impl_(__zterm_impl) _Post1_impl_(__zterm_impl))
#if !(defined(__BCPLUSPLUS__) || defined(__BORLANDC__))
_CRT_NONSTDC_DEPRECATE(_swab)
#endif
_ACRTIMP void __cdecl swab(
    _Inout_updates_z_(_SizeInBytes) char* _Buf1,
    _Inout_updates_z_(_SizeInBytes) char* _Buf2,
    _In_                            int   _SizeInBytes
    );
    /*_Check_return_ __inline float __CRTDECL powf(_In_ float _X, _In_ float _Y)
    {
        return (float)pow(_X, _Y);
    }*/
#endif
