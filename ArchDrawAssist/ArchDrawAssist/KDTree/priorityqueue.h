#ifndef __PRIORITYQUEUE_H_
#define __PRIORITYQUEUE_H_
#include <stdlib.h>
/**
 * Fix sized maximum priority queue 
 */
template<class Index, class Weight>
class MaxPriorityQueue {
public:
	typedef struct el{
		Index	index;
		Weight weight;
	} Element;

	MaxPriorityQueue() {
		_queue = NULL;
	}

	~MaxPriorityQueue() {
		if (_queue) {
			delete[] _queue;
		}
	}

	void setSize(int size) {
		if (_queue) {
			delete[] _queue;
		}
		_queue = new Element[size+1];
		_nOfElements = size;
		init();
	}

	inline void init() {
		_current = 0;
	}

	inline bool isEmpty() {
		return _current == 0;
	}

	inline bool isFull() {
		return _current==(_nOfElements);
	}

	inline int getNofElements() {
		return _current;
	}

	inline void insert(Index index, Weight weight) {
		if (isFull()) {
			// if full replace max
			_queue[1].index = index;
			_queue[1].weight = weight;
			restore(1, _nOfElements);
		} 
		else {
			_current++;
			_queue[_current].index = index;
			_queue[_current].weight = weight;
			register int i=_current;
			while(i>1 && (_queue[i].weight > _queue[i>>1].weight)) {
				swap(&_queue[i], &_queue[i>>1]);
				i >>= 1;
			}
		}
	}

	inline Element getMax() {
//		ASSERT(!isEmpty());
		return _queue[1];
	}

	inline Index getMaxIndex() {
//		ASSERT(!isEmpty());
		return _queue[1].index;
	}

	inline Weight getMaxWeight() {
//		ASSERT(!isEmpty());
		return _queue[1].weight;
	}

	inline void removeMax() {
//		ASSERT(!isEmpty());
		_queue[1] = _queue[_current];
		restore(1, _current);
		_current--;
	}

	inline Element* getQueue() {
		return _queue+1;
	}


protected:
	inline void restore(register int L, register int R) {
		register int i, j;
		i = L;
		while (i <= (R>>1)) {
			if( 2*i < R && _queue[2*i+1].weight > _queue[2*i].weight) {
				j = 2*i+1;
			}
			else {
				j = 2*i;
			}
			if (_queue[j].weight > _queue[i].weight) {
				swap(&_queue[j], &_queue[i]);
				swap(&i, &j);

			}
			else {
				i = R;
			}
		}
	}

private:
	Element* _queue;
	int _nOfElements;
	int _current;

	inline void swap(Element* m1, Element* m2) {
		Element tmp;
		tmp = *m2;
		*m2 = *m1;
		*m1 = tmp;
	}

	inline void swap(int *i, int *j) {
		int tmp = *i;
		*i = *j;
		*j = tmp;
	}
			
};

/**
 * Fix sized minimum priority queue 
 */
template<class Index, class Weight>
class MinPriorityQueue{

	
public:
	typedef struct el{
		Index	index;
		Weight	weight;
	} Element;

	MinPriorityQueue() {
		_queue = NULL;
	}

	~MinPriorityQueue() {
		if (_queue) {
			delete[] _queue;
		}
	}

	void setSize(int size) {
		if (_queue) {
			delete[] _queue;
		}
		_queue = new Element[size+1];
		_nOfElements = size;
		init();
	}

	inline void init() {
		_current = 0;
	}
	
	inline bool isEmpty() {
		return _current == 0;
	}

	inline bool isFull() {
		return _current==(_nOfElements);
	}

	inline int getNofElements() {
		return _current;
	}

	inline void insert(Index index, Weight weight) {
		if (isFull()) {
			// if full replace max
			_queue[1].index = index;
			_queue[1].weight = weight;
			restore(1, _nOfElements);
		} 
		else {
			_current++;
			_queue[_current].index = index;
			_queue[_current].weight = weight;
			register int i=_current;
			while(i>1 && (_queue[i].weight < _queue[i>>1].weight)) {
				swap(&_queue[i], &_queue[i>>1]);
				i >>= 1;
			}
		}
	}

	inline Element getMin() {
		ASSERT(!isEmpty());
		return _queue[1];
	}

	inline Index getMinIndex() {
		ASSERT(!isEmpty());
		return _queue[1].index;
	}

	inline Weight getMinWeight() {
		ASSERT(!isEmpty());
		return _queue[1].weight;
	}

	 inline void removeMin() {
		ASSERT(!isEmpty());
		_queue[1] = _queue[_current];
		restore(1, _current);
		_current--;
	}

	inline Element* getQueue() {
		return _queue+1;
	}


protected:
	inline void restore(register int L, register int R) {
		register int i, j;
		i = L;
		while (i <= (R>>1)) {
			if( 2*i < R && _queue[2*i+1].weight < _queue[2*i].weight) {
				j = 2*i+1;
			}
			else {
				j = 2*i;
			}
			if (_queue[j].weight < _queue[i].weight) {
				swap(&_queue[j], &_queue[i]);
				swap(&i, &j);

			}
			else {
				i = R;
			}
		}
	}

private:
	Element* _queue;
	int _nOfElements;
	int _current;

	inline void swap(Element* m1, Element* m2) {
		Element tmp;
		tmp = *m2;
		*m2 = *m1;
		*m1 = tmp;
	}

	inline void swap(int *i, int *j) {
		int tmp = *i;
		*i = *j;
		*j = tmp;
	}
			
};

#endif

