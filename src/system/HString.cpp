//
//                 __  __            __           
//                / / / /__  __ ____/ /_____ ____ 
//               / /_/ // / / // __  // ___// __ \
//              / __  // /_/ // /_/ // /   / /_/ /
//             /_/ /_/ \__, / \__,_//_/    \____/ 
//                    /____/                      
//
//              The Hydro Programming Language
//
//        © 2020 Shockazoid, Inc. All Rights Reserved.
//

#include "HString.hpp"
#include "../vm/HvmEnv.hpp"

namespace hydro
{

HString::HString(hstring value) : HObject{hvm_get_instance(), hvm_get_instance()->core()->StringClass()}
{
	_len = value->_len;
	_buffer = new char16_t[_len + 1];

	// deep copy
	for (uint32_t i = 0; i < _len; i++)
		_buffer[i] = value->_buffer[i];

	_buffer[_len] = '\0';
}

HString::HString(const char *value) : HObject{hvm_get_instance(), hvm_get_instance()->core()->StringClass()}
{
	_len = (int)std::strlen(value);
	unsigned size = _len + 1;
	_buffer = new char16_t[size];

	// deep copy
	for (unsigned i = 0; i < size; i++)
		_buffer[i] = value[i];
}

HString::HString(const char16_t *value) : HObject{hvm_get_instance(), hvm_get_instance()->core()->StringClass()}
{
	_len = HString::strlen(value);
	unsigned size = _len + 1;
	_buffer = new char16_t[size];

	// deep copy
	for (unsigned i = 0; i < size; i++)
		_buffer[i] = value[i];
}

HString::HString(std::string value) : HObject{hvm_get_instance(), hvm_get_instance()->core()->StringClass()}
{
	_len = (int)value.length();
	_buffer = new char16_t[_len];

	// deep copy
	for (unsigned i = 0; i < _len; i++)
		_buffer[i] = value[i];
}

HString::HString(HvmEnv *env, HClass *vclass) : HObject{env, vclass}
{
	_len = 0;
	_buffer = new char16_t[1];
	_buffer[0] = '\0';
}

HString::HString(HvmEnv *env, HClass *vclass, hstring value) : HObject{env, vclass}
{
	_len = value->_len;
	_buffer = new char16_t[_len + 1];

	// deep copy
	for (uint32_t i = 0; i < _len; i++)
		_buffer[i] = value->_buffer[i];

	_buffer[_len] = '\0';
}

HString::HString(HvmEnv *env, HClass *vclass, const char *value) : HObject{env, vclass}
{
	_len = (int)std::strlen(value);
	unsigned size = _len + 1;
	_buffer = new char16_t[size];

	// deep copy
	for (unsigned i = 0; i < size; i++)
		_buffer[i] = value[i];
}

HString::HString(HvmEnv *env, HClass *vclass, const char16_t *value) : HObject{env, vclass}
{
	_len = HString::strlen(value);
	unsigned size = _len + 1;
	_buffer = new char16_t[size];

	// deep copy
	for (unsigned i = 0; i < size; i++)
		_buffer[i] = value[i];
}

HString::HString(HvmEnv *env, HClass *vclass, std::string value) : HObject{env, vclass}
{
	_len = (int)value.length();
	_buffer = new char16_t[_len];

	// deep copy
	for (unsigned i = 0; i < _len; i++)
		_buffer[i] = value[i];
}

HString::~HString()
{
	delete[] _buffer;
}

void HString::construct(hvalue value)
{
    HObject::construct();
    
    if(is_string(value))
    {
        // copy
        hstring s = value; // cast
        _len = s->_len;
        _buffer = new char16_t[_len];
        for(uint32_t i = 0; i < _len; i++)
            _buffer[i] = s->_buffer[i];
    }
}

unsigned HString::strlen(const char16_t *value)
{
	unsigned n = 0;

	while (value[n])
		n++;

	return n;
}

hstring HString::substr(uint32_t pos, int32_t length)
{
    int32_t size = length > 0 ? length : _len + length;
    int32_t mySize = size - pos;
    
    if(size - pos <= 0)
        return new HString{_env, _clss, ""};
    
    char16_t *buf = new char16_t[mySize];
    assert(buf);
    
    // copy
    for(int32_t i = 0; i < size && i + pos < _len; i++)
        buf[i] = _buffer[i + pos];
    
    buf[size] = '\0';
    return new HString{_env, _clss, buf};
}

/*
     bool HString::operator==(const HString &s) const
     {
     if (s.len != len)
     return false;
     
     for (int i = 0; i < len; i++)
     if (s.buffer[i] != buffer[i])
     return false;
     
     // equality
     return true;
     }
     
     bool HString::operator!=(const HString &s) const
     {
     if (s._len != _len)
     return true;
     
     for (int i = 0; i < _len; i++)
     if (s._buffer[i] == _buffer[i])
     return false;
     
     // inequality
     return true;
     }
     
     HString HString::operator+(const HString &s) const
     {
     // concat
     const int sLen = (int)std::strlen(s);
     const int n = len + sLen;
     char16_t *buf = new char16_t[n + 1];
     int i;
     
     for (i = 0; i < len; i++)
     buf[i] = buf[i];
     
     for (; i < n; i++)
     buf[i] = s[i - len];
     
     buf[n + 1] = '\0';
     
     return *(new HString(buf));
     }
     
     HString HString::operator+(const char *s) const
     {
     // concat
     const int sLen = (int)std::strlen(s);
     const int n = len + sLen;
     char16_t *buf = new char16_t[n + 1];
     int i;
     
     for (i = 0; i < len; i++)
     buf[i] = buf[i];
     
     for (; i < n; i++)
     buf[i] = s[i - len];
     
     buf[n + 1] = '\0';
     
     return *(new HString(buf));
     }
     
     HString HString::operator+(const std::string s) const
     {
     // concat
     const int n = len + (int)s.length();
     char16_t *buf = new char16_t[n + 1];
     int i;
     
     for (i = 0; i < len; i++)
     buf[i] = buf[i];
     
     for (; i < n; i++)
     buf[i] = s[i - len];
     
     buf[n + 1] = '\0';
     
     return *(new HString(buf));
     }
     
     HString HString::toString() const { return buffer; }
     
     bool operator==(const std::string &lhs, const string &rhs)
     {
     const int len = (int)rhs.length();
     if (lhs.length() != len)
     return false;
     
     for (int i = 0; i < len; i++)
     if (lhs[i] != rhs[i])
     return false;
     
     return true;
     }
     
     bool operator==(const string &lhs, const std::string &rhs)
     {
     const int len = (int)rhs.length();
     if (lhs.length() != len)
     return false;
     
     for (int i = 0; i < len; i++)
     if (lhs[i] != rhs[i])
     return false;
     
     return true;
     }
     
     bool operator!=(const std::string &lhs, const string &rhs)
     {
     const int len = (int)rhs.length();
     if (lhs.length() != len)
     return true;
     
     for (int i = 0; i < len; i++)
     if (lhs[i] == rhs[i])
     return false;
     
     return true;
     }
     
     bool operator!=(const string &lhs, const std::string &rhs)
     {
     const int len = (int)rhs.length();
     if (lhs.length() != len)
     return true;
     
     for (int i = 0; i < len; i++)
     if (lhs[i] == rhs[i])
     return false;
     
     return true;
     }
     
     std::ostream &operator<<(std::ostream &out, const string &rhs)
     {
     // copy
     std::string result = rhs;
     
     // output
     out << result;
     
     return out;
     }
     
     string operator+(const char *lhs, const string &rhs)
     {
     const unsigned size = (unsigned)std::strlen(lhs) + rhs.length();
     char *tmp = new char[size];
     unsigned i, j, n = (unsigned)std::strlen(lhs);
     
     for (i = 0; i < n; i++)
     tmp[i] = lhs[i];
     
     for (j = 0; j < size; j++, i++)
     tmp[i] = rhs[j];
     
     return tmp;
     }
     
     string operator+(const string &lhs, const char *rhs)
     {
     const unsigned size = lhs.length() + (unsigned)std::strlen(rhs);
     char *tmp = new char[size];
     unsigned i, j, n = lhs.length();
     
     for (i = 0; i < n; i++)
     tmp[i] = lhs[i];
     
     for (j = 0; j < size; j++, i++)
     tmp[i] = rhs[j];
     
     return tmp;
     }
     */

void hydro_system_String_constructor(hstring instance) {}

} // namespace hydro
