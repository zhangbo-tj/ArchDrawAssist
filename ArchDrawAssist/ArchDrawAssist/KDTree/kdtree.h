#ifndef __KDTREE_H_
#define __KDTREE_H_

#include "Point.h"
#include "priorityqueue.h"
#include "matrix.h"
#include <vector>

using namespace std;

#define SQ(x) (x*x)
#define MYNZERO 1e-20

typedef MaxPriorityQueue<int, double> PQueue;
typedef PQueue::Element  Neighbour;

// typedef struct point {
// 	VECTOR3		pos;
// 	int			index;
// } MyPoint;
//typedef class Point MyPoint;

/**
 * abstract node class
 * base class for leaves and nodes
 *
 * @author Richard Keiser
 * @version 2.0
 */ 
class KdTreeNode {
public:
	virtual void queryNode(double rd, PQueue* queryPriorityQueue) = 0;
	virtual void print_node() = 0;

	KdTreeNode(){}
	virtual ~KdTreeNode(){}
};

class KdNode : public KdTreeNode {
public:
	KdNode(){
		_lchild = NULL;
		_rchild = NULL;
	}
	virtual ~KdNode() {
		if(_lchild) delete _lchild;
		if(_rchild) delete _rchild;
	}
	KdTreeNode*			_lchild;	
	KdTreeNode*			_rchild;
	double			_cutVal;	
	unsigned char	_dim;			
	void queryNode(double rd, PQueue* queryPriorityQueue);
	
	//debug	
	void print_node();
};


class KdLeaf : public KdTreeNode {
public:
	MyPoint*		_points;

	unsigned int	_nOfElements;

	void queryNode(double rd, PQueue* queryPriorityQueue);

	//debug
	void print_node();
};




/**
 * An efficient k-d tree for 3 dimensions
 * It is very similar to the k-d tree 
 * described by Arya and Mount in ``Algorithms
 *	for fast vector quantization,'' Proc.  of DCC '93: Data Compression
 *	Conference, eds. J. A. Storer and M. Cohn, IEEE Press, 1993, 381-390
 *  and their ANN software library
 */
class KdTree {

public:
	KdTree(const VECTOR3 *positions, const unsigned int nOfPositions, const unsigned int maxBucketSize);

	KdTree(const vector<VECTOR3>& positions, const unsigned int maxBucketSize);

	KdTree(vector<MyPoint>& positions, const unsigned int maxBucketSize);

	KdTree(vector<Vertex>& positions, const unsigned int maxBucketSize);

	~KdTree();

	void queryPosition(const VECTOR3 &position);
	void queryRange(const VECTOR3 &position, const double maxSqrDistance);
	void setNOfNeighbours (const unsigned int newNOfNeighbours);
	inline unsigned int getNeighbourPositionIndex (const unsigned int i); 
	inline VECTOR3 getNeighbourPosition(const unsigned int i); //尽量不要用此函数
	inline double getSquaredDistance (const unsigned int i);
	inline unsigned int getNOfFoundNeighbours();

protected:
	double computeBoxDistance(const VECTOR3 &q, const VECTOR3 &lo, const VECTOR3 &hi);
	void computeEnclosingBoundingBox(VECTOR3 &lowCorner, VECTOR3 &hiCorner);
	void createTree(KdNode &node, int start, int end, VECTOR3 maximum, VECTOR3 minimum);


private:
	
	MyPoint*					_points;
	const VECTOR3*				_positions;
	Neighbour*					_neighbours;
	int							_bucketSize;
	KdNode*						_root;
	unsigned int				_nOfFoundNeighbours,
								_nOfNeighbours,
								_nOfPositions;
	PQueue*						_queryPriorityQueue;
	VECTOR3						_boundingBoxLowCorner;
	VECTOR3						_boundingBoxHighCorner;

	void getMinMax(MyPoint *points, int nOfPoints, int dim, double &min, double &max);
	void splitAtMid(MyPoint *points, int nOfPoints, int dim, double cutVal, int &br1, int &br2);
	void getSpread(MyPoint* points, int nOfPoints, VECTOR3 &maximum, VECTOR3 &minimum);
};

inline unsigned int KdTree::getNOfFoundNeighbours() {
	return _nOfFoundNeighbours;
}

inline unsigned int KdTree::getNeighbourPositionIndex(const unsigned int neighbourIndex) {
	return _neighbours[neighbourIndex].index;
}

//-----------------------------------------------------
//this function is not efficient, try your best to avoid to use it.
inline VECTOR3 KdTree::getNeighbourPosition(const unsigned int neighbourIndex) {
	
	for (unsigned int i = 0; i < _nOfPositions; i++) {
		if (_points[i].X() == _neighbours[neighbourIndex].index) {
			return VECTOR3(_points[i].X(),_points[i].Y(),_points[i].Z());
			//return _points[i].pos;
		}
	}
	return VECTOR3(0, 0, 0);

//	VECTOR3 ret = _points[_neighbours[neighbourIndex].index].pos;
//	return ret;
}

inline double KdTree::getSquaredDistance (const unsigned int neighbourIndex) {
	return _neighbours[neighbourIndex].weight;
}

#endif

