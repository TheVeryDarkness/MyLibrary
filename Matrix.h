#include <amp.h>

using concurrency::array;
using concurrency::array_view;
using concurrency::index;
using concurrency::parallel_for_each;
using std::vector;


template<
	typename Data,
	//These define the size of the matrix
	unsigned long m, unsigned long n
>
class Matrix
{
public:
	Data** data;
	inline Matrix() {
		data = new Data * ();
		for (long i = 0; i < m; i++)
		{
			data[i] = new Data[n]();
		}
		return;
	}
	typedef Data(__stdcall* Value)();
	inline Matrix(Value) {
		data = new Data * ();
		for (long i = 0; i < m; i++)
		{
			data[i] = new Data[n](*Value);
		}
		return;
	}
	inline ~Matrix() {
		for (long i = 0; i < m; i++)
		{
			delete[] data[i];
		}
		delete[] data;
		return;
	}
	//Remember to release the memory.
	inline Matrix operator+(const Matrix& Operand) {
		Matrix Result = Matrix();
		for (long i = 0; i < m; i++)
		{
			Data __row[n];
			for (long j = 0; j < n; j++)
			{
				__row[j] = Operand.data[i][j];
			}
			Data row[n];//save the data of this matrix.
			for (long j = 0; j < n; j++)
			{
				row[j] = this->data[i][j];
			}
			//save the data of result matrix.
			array_view<int, 1> _row(n, Result.data[i]);
			parallel_for_each(
				_row.extent,
				[&__row,&row,&_row](index<1> j) restrict(amp)
				{
					_row[j] = row[j] + __row[j];
				}
			);
		}
		return Result;
	}
private:
};