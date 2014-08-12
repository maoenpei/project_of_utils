

#include "SortAlgorithm.h"

NS_DEF_NARAN{

	MergeSortAlgorithm::~MergeSortAlgorithm()
	{}

	arr(ImageRect *) MergeSortAlgorithm::sort(arr(ImageRect *) objects)
	{
		arr(ImageRect *) exchange, swap;
		exchange.alloc(objects.count());
		
		// merge sort algorithm
		int total = objects.count();
		int unit = 1;
		while(unit < total){
			for (int i = 0; i<total; i += unit * 2){
				if (i + unit >= total){
					memcpy(exchange.get() + i, objects.get() + i, (total - i) * sizeof(ImageRect *));
					continue;
				}
				int k = i;
				int j = i+unit;
				int limit = (i + 2 * unit < total ? i + 2 * unit : total);
				int n = i;
				while(k<i+unit && j<limit){
					if (compareGreaterThan(objects[j], objects[k])){
						exchange[n++] = objects[j++];
					}else{
						exchange[n++] = objects[k++];
					}
				}
				memcpy(exchange.get() + n, objects.get() + (k<i+unit ? k : j), (limit - n) * sizeof(ImageRect *));
			}
			swap = exchange;
			exchange = objects;
			objects = swap;
			unit *= 2;
		}

		return objects;
	}
}

