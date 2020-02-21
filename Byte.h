#pragma once

#include "Shared.h"
#include "Statistics.h"
#include <ostream>
#include <iomanip>


template<unsigned char Radix = 255>//指示该位的最大值
//无符号单字节数
class Byte {
private:
	unsigned char data;
public:
	explicit INLINED Byte(unsigned long that) {
		this->data = (unsigned char)that;
		return;
	}
	explicit INLINED Byte(int that) {
		this->data = (unsigned char)ABS(that % (Radix + 1));
		return;
	}
	explicit INLINED Byte(unsigned int that) {
		this->data = (unsigned char)(that % (Radix + 1));
		return;
	}
	explicit INLINED Byte(unsigned char that) {
		this->data = that;
		return;
	}
	explicit INLINED Byte(const bool that = false) {
		this->data = (that) ?
			('\001') : ('\000');
		return;
	}
	explicit INLINED operator unsigned long()const {
		return (unsigned long)this->data;
	}
	explicit INLINED operator int()const {
		return (int)this->data;
	}
	explicit INLINED operator bool()const {
		if (this == nullptr)
		{
			return false;
		}
		return (this->data > 0) ? true : false;
	}
	INLINED Byte operator*=(int that) {
		this->data *= that;
		return*this;
	}
	INLINED Byte operator*(int that) {
		Byte Res(this->data);
		Res.data *= that;
		return Res;
	}
	INLINED Byte operator=(long that) {
		this->data = (unsigned char)(that % (Radix + 1));
		return *this;
	}
	INLINED Byte operator=(int that) {
		this->data = (unsigned char)(that % (Radix + 1));
		return *this;
	}
	INLINED Byte operator=(bool that) {
		this->data = (that) ? 1 : 0;
		return *this;
	}
	INLINED bool operator==(bool that) const {
		return (
			((bool)this->data == that) ?
			true : false
			);
	}
	INLINED bool operator==(const Byte & that)const {
		return (
			(this->data > 0 && that.data > 0
				||
				(this->data == 0 && that.data == 0)
				) ?
			true : false
			);
	}
	INLINED Byte operator+=(Byte that) {
		if (Radix == 1)
		{
			this->data = (that.data > 0) ? (!this->data) : this->data;
		}
		else
		{
			this->data = (this->data + that.data) % (Radix + 1);
		}
		return *this;
	}
	INLINED Byte operator+(Byte that) const {
		if (Radix == 1)
		{
			return (this->data == that.data) ?
				Byte(false) : Byte(false);
		}
		else
		{
			return Byte(this->data + that.data);
		}
	}
	INLINED Byte operator!() const {
		return (
			(this->data > 0) ?
			Byte(false) : Byte(true))
			;
	}
	INLINED bool operator>(const Byte & that)const {
		if (Radix == 1)
		{
			return(this->data > 0 && that.data == 0) ? true : false;
		}
		else
		{
			return(this->data > that.data) ? true : false;
		}
	}
	INLINED bool operator>=(const Byte & that) const {
		if (Radix == 1)
		{
			return(this->data > 0) ? true : false;
		}
		else
		{
			return(this->data >= that.data) ? true : false;
		}
	}
	//自动将输出进制复位为10
	friend INLINED std::ostream& operator<<(
		std::ostream & out,
		const Byte & that
		) {
		if (Radix == 255)
		{
			out << std::setbase(16)
				<< ((int)that.data) / 16
				<< ((int)that.data) % 16;
		}
		else if (Radix == 99)
		{
			out << std::setbase(10)
				<< ((int)that.data) / 10
				<< ((int)that.data) % 10;
		}
		else if (&that != nullptr)
		{
			if (that.data < 10)
			{
				out << (unsigned short int)(that.data);
			}
			else if (that.data < 36)
			{
				out << (char)(that.data - 10 + 'a');
			}
			else if (that.data < 62)
			{
				out << (char)(that.data - 36 + 'A');
			}
		}
		out << std::setbase(10);
		return out;
	}
};