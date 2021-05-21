#include "simple_vector.h"

// Tests
#include "tests.h"

int main(void) {
	//Test1();

	{
		SimpleVector<int> v(5);
		assert(v.GetSize() == 5u);
		assert(v.GetCapacity() == 5u);
		assert(!v.IsEmpty());
		v.Clear();
		assert(v.begin() == v.end());
		for (size_t i = 0; i < v.GetSize(); ++i) {
			assert(v[i] == 0);
		}
	}
	return 0;
}