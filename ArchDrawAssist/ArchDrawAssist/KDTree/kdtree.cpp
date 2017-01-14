

#include <stdlib.h>
#include <float.h>

#include "kdtree.h"

#define SWAP_POINTS(a,b) \
		MyPoint tmp = points[a];\
	    points[a] = points[b];\
	    points[b] = tmp;


double					__queryOffsets[3];
VECTOR3					__queryPosition;

KdTree::KdTree(const VECTOR3 *positions, const unsigned int nOfPositions, const unsigned int maxBucketSize) {
	_bucketSize			= maxBucketSize;
//	_positions			= positions;
	_nOfPositions		= nOfPositions;
	_points				= new MyPoint[nOfPositions];
	_neighbours			= NULL;
	_nOfFoundNeighbours	= 0;
	_nOfNeighbours		= 0;
	_queryPriorityQueue	= new PQueue();
	for (unsigned int i=0; i<nOfPositions; i++) {
		VECTOR3 v = positions[i];
		_points[i].SetX((float)v.GetFirst());
		_points[i].SetY((float)v.GetSecond());
		_points[i].SetZ((float)v.GetThird());
		_points[i].SetIndex(i);
	}
	computeEnclosingBoundingBox(_boundingBoxLowCorner, _boundingBoxHighCorner);
	_root = new KdNode();
	VECTOR3 maximum, minimum;
	getSpread(_points, nOfPositions, maximum, minimum);
	createTree(*_root, 0, nOfPositions, maximum, minimum);
	setNOfNeighbours(1);
}

KdTree::KdTree(const vector<VECTOR3>& positions, const unsigned int maxBucketSize) {
	_bucketSize			= maxBucketSize;
//	_positions			= positions;
//	_nOfPositions		= nOfPositions;
	_nOfPositions		= positions.size();
	
	_points				= new MyPoint[_nOfPositions];
	_neighbours			= NULL;
	_nOfFoundNeighbours	= 0;
	_nOfNeighbours		= 0;
	_queryPriorityQueue	= new PQueue();
	
	for (unsigned int i=0; i<_nOfPositions; i++) {
		VECTOR3 v = positions[i];
		_points[i].SetX((float)v.GetFirst());
		_points[i].SetY((float)v.GetSecond());
		_points[i].SetZ((float)v.GetThird());
		_points[i].SetIndex(i);
	}
	computeEnclosingBoundingBox(_boundingBoxLowCorner, _boundingBoxHighCorner);
	_root = new KdNode();
	VECTOR3 maximum, minimum;
	getSpread(_points, _nOfPositions, maximum, minimum);
	createTree(*_root, 0, _nOfPositions, maximum, minimum);
	setNOfNeighbours(1);
	
}
// KdTree::KdTree(const vector<vertex>& positions, const unsigned int maxBucketSize) {
// 	_bucketSize			= maxBucketSize;
// 	//	_positions			= positions;
// 	//	_nOfPositions		= nOfPositions;
// 	_nOfPositions		= positions.size();
// 
// 	_points				= new MyPoint[_nOfPositions];
// 	_neighbours			= NULL;
// 	_nOfFoundNeighbours	= 0;
// 	_nOfNeighbours		= 0;
// 	_queryPriorityQueue	= new PQueue();
// 
// 	for (unsigned int i=0; i<_nOfPositions; i++) {
// 		_points[i].pos.Set(positions[i].x, positions[i].x, positions[i].x);
// 		_points[i].index = i;
// 	}
// 	computeEnclosingBoundingBox(_boundingBoxLowCorner, _boundingBoxHighCorner);
// 	_root = new KdNode();
// 	VECTOR3 maximum, minimum;
// 	getSpread(_points, _nOfPositions, maximum, minimum);
// 	createTree(*_root, 0, _nOfPositions, maximum, minimum);
// 	setNOfNeighbours(1);
// 
// }
KdTree::KdTree(vector<Vertex>& positions, const unsigned int maxBucketSize) {
	_bucketSize			= maxBucketSize;
	//	_positions			= positions;
	//	_nOfPositions		= nOfPositions;
	_nOfPositions		= positions.size();

	_points				= new MyPoint[_nOfPositions];
	_neighbours			= NULL;
	_nOfFoundNeighbours	= 0;
	_nOfNeighbours		= 0;
	_queryPriorityQueue	= new PQueue();

	for (unsigned int i=0; i<_nOfPositions; i++) {
		Vertex v = positions[i];
		_points[i].SetX(v.X());
		_points[i].SetY(v.Y());
		_points[i].SetZ(v.Z());
		_points[i].SetIndex(i);
	}
	computeEnclosingBoundingBox(_boundingBoxLowCorner, _boundingBoxHighCorner);
	_root = new KdNode();
	VECTOR3 maximum, minimum;
	getSpread(_points, _nOfPositions, maximum, minimum);
	createTree(*_root, 0, _nOfPositions, maximum, minimum);
	setNOfNeighbours(1);

}

KdTree::KdTree(vector<MyPoint>& positions, const unsigned int maxBucketSize) {
	_bucketSize			= maxBucketSize;
	//	_positions			= positions;
	//	_nOfPositions		= nOfPositions;
	_nOfPositions		= positions.size();

	_points				= new MyPoint[_nOfPositions];
	_neighbours			= NULL;
	_nOfFoundNeighbours	= 0;
	_nOfNeighbours		= 0;
	_queryPriorityQueue	= new PQueue();

	for (unsigned int i=0; i<_nOfPositions; i++) {

		MyPoint v = positions[i];
		_points[i].SetX(v.X());
		_points[i].SetY(v.Y());
		_points[i].SetZ(v.Z());
		_points[i].SetIndex(i);
	}
	computeEnclosingBoundingBox(_boundingBoxLowCorner, _boundingBoxHighCorner);
	_root = new KdNode();
	VECTOR3 maximum, minimum;
	getSpread(_points, _nOfPositions, maximum, minimum);
	createTree(*_root, 0, _nOfPositions, maximum, minimum);
	setNOfNeighbours(1);

}

KdTree::~KdTree() {
	delete _root;
	delete[] _points;
	if (_neighbours) {
		delete _neighbours;
	}
	delete _queryPriorityQueue;
}

void KdTree::computeEnclosingBoundingBox(VECTOR3 &lowCorner, VECTOR3 &hiCorner) {
	VECTOR3 tmp;
	//hiCorner = lowCorner = _points[0].pos;	
	hiCorner.Set(_points[0].X(),_points[0].Y(),_points[0].Z());
	lowCorner.Set(_points[0].X(),_points[0].Y(),_points[0].Z());
	for (unsigned int i=1; i<_nOfPositions; i++) {
//		tmp = _positions[i];
		//tmp = _points[i].pos;
		tmp.Set(_points[i].X(),_points[i].Y(),_points[i].Z());
		if (hiCorner[0] < tmp[0]) {
			hiCorner[0] = tmp[0];
		}
		else if (lowCorner[0] > tmp[0]) {
			lowCorner[0] = tmp[0];
		}
		if (hiCorner[1] < tmp[1]) {
			hiCorner[1] = tmp[1];
		}
		else if (lowCorner[1] > tmp[1]) {
			lowCorner[1] = tmp[1];
		}
		if (hiCorner[2] < tmp[2]) {
			hiCorner[2] = tmp[2];
		}
		else if (lowCorner[2] > tmp[2]) {
			lowCorner[2] = tmp[2];
		}
	}		 
}

double KdTree::computeBoxDistance(const VECTOR3 &q, const VECTOR3 &lo, const VECTOR3 &hi) {
	register double dist = 0.0;
	register double t;

	if (q(0) < lo(0)) {
		t = lo(0) - q(0);
		dist = t*t;
	}
	else if (q(0) > hi(0)) {
		t = q(0) - hi(0);
		dist = t*t;
	}
	if (q(1) < lo(1)) {
		t = lo(1) - q(1);
		dist += t*t;
	}
	else if (q(1) > hi(1)) {
		t = q(1) - hi(1);
		dist += t*t;
	}
	if (q(2) < lo(2)) {
		t = lo(2) - q(2);
		dist += t*t;
	}
	else if (q(2) > hi(2)) {
		t = q(2) - hi(2);
		dist += t*t;
	}

	return dist;
}

void KdTree::queryPosition(const VECTOR3 &position) {
	if (!_neighbours) {
		return;
	}
	__queryOffsets[0] = __queryOffsets[1] = __queryOffsets[2] = 0.0;
	_queryPriorityQueue->init();
	_queryPriorityQueue->insert(-1, FLT_MAX);
	__queryPosition = position;
	double dist = computeBoxDistance(position, _boundingBoxLowCorner, _boundingBoxHighCorner);
	_root->queryNode(dist, _queryPriorityQueue);
	_nOfFoundNeighbours = 0;

	for(int i=_nOfNeighbours-1; i>=0; i--) {
		_neighbours[i] = _queryPriorityQueue->getMax();
		if (_neighbours[i].index == -1) {
			break;
		}
		_nOfFoundNeighbours++;
		_queryPriorityQueue->removeMax();
	}
}

void KdNode::queryNode(double rd, PQueue* queryPriorityQueue) {
	register double old_off = __queryOffsets[_dim];
	register double new_off = __queryPosition[_dim] - _cutVal;
	if (new_off < 0) {
		_lchild->queryNode(rd, queryPriorityQueue);
		rd = rd - SQ(old_off) + SQ(new_off);
		if (rd <= queryPriorityQueue->getMaxWeight()) {
			__queryOffsets[_dim] = new_off;
			_rchild->queryNode(rd, queryPriorityQueue);
			__queryOffsets[_dim] = old_off;
		}
	}
	else {
		_rchild->queryNode(rd, queryPriorityQueue);
		rd = rd - SQ(old_off) + SQ(new_off);
		if (rd <= queryPriorityQueue->getMaxWeight()) {
			__queryOffsets[_dim] = new_off;
			_lchild->queryNode(rd, queryPriorityQueue);
			__queryOffsets[_dim] = old_off;
		}
	}

}

void KdLeaf::queryNode(double rd, PQueue* queryPriorityQueue) {
	double sqrDist;
	//use pointer arithmetic to speed up the linear traversing
	MyPoint* point = _points;
	for (register unsigned int i=0; i<_nOfElements; i++) {
//		sqrDist = (point->pos - __queryPosition).getSquaredLength();
		VECTOR3 tempPoint = VECTOR3(point->X(),point->Y(),point->Z());
		sqrDist = dot(tempPoint - __queryPosition, tempPoint - __queryPosition);
		
		if (sqrDist < queryPriorityQueue->getMaxWeight()) {
			queryPriorityQueue->insert(point->Index(), sqrDist);
		}
		point++;
	}		
}

void KdTree::queryRange(const VECTOR3 &position, const double maxSqrDistance) {
	if (!_neighbours) {
		return;
	}
	__queryOffsets[0] = __queryOffsets[1] = __queryOffsets[2] = 0.0;
	_queryPriorityQueue->init();
	_queryPriorityQueue->insert(-1, maxSqrDistance);
	__queryPosition = position;

	double dist = computeBoxDistance(position, _boundingBoxLowCorner, _boundingBoxHighCorner);	
	_root->queryNode(dist, _queryPriorityQueue);
	_nOfFoundNeighbours = 0;

	int cnt = _queryPriorityQueue->getNofElements() - 1;

	while(cnt >= 0) {
		_neighbours[cnt] = _queryPriorityQueue->getMax();
		if (_neighbours[cnt].index != -1) {
			_nOfFoundNeighbours++;
		}
		_queryPriorityQueue->removeMax();
		cnt--;
	}
}


void KdTree::setNOfNeighbours (const unsigned int newNOfNeighbours) {
	if (newNOfNeighbours != _nOfNeighbours) {
		_nOfNeighbours = newNOfNeighbours;
		if (_neighbours) {
			delete _neighbours;
		}
		_queryPriorityQueue->setSize(_nOfNeighbours);
		_nOfNeighbours = newNOfNeighbours;
		_neighbours = new 	Neighbour[_nOfNeighbours];
		_nOfFoundNeighbours = 0;
	}
}


void KdTree::createTree(KdNode &node, int start, int end, VECTOR3 maximum, VECTOR3 minimum) 
{
	int	mid;
	int n = end-start;
	VECTOR3 diff = maximum - minimum;
	short dim;
	// get longest axe
	if (diff[0] > diff[1]) {
		if (diff[0] > diff[2]) { dim = 0;}	// x-axe is longest axe 	
		else { dim = 2; }					// z-axe is longest axe
	}
	else { 
		if (diff[1] > diff[2]) { dim = 1;}	// y-axe is longest axe
		else {	dim = 2; }					// z-axe is longest axe
	}
	
	node._dim = dim;
	double bestCut = (maximum[dim]+minimum[dim])/2.0;
	double min, max;
	getMinMax(_points+start, n, dim, min, max);	// find min/max coordinates
	if (bestCut < min)		// slide to min or max as needed
		node._cutVal = min;
    else if (bestCut > max)
		node._cutVal = max;
    else
		node._cutVal = bestCut;

    int br1, br2;
	splitAtMid(_points+start, n, dim, node._cutVal, br1, br2);	// permute points accordingly

	if (bestCut < min) mid = start+1;
    else if (bestCut > max) mid = end-1;
    else if (br1 > n/2.0) mid = start+br1;
    else if (br2 < n/2.0) mid = start+br2;
    else mid = start + (n>>1);

	if (mid-start <= _bucketSize || (maximum[dim] - minimum[dim]) < MYNZERO ) {
//	if (mid-start <= _bucketSize) {
		// new leaf
		KdLeaf* leaf = new KdLeaf();
		node._lchild = leaf;
		leaf->_points = (_points+start);
		leaf->_nOfElements = mid-start;
	}
	else {
		// new node
		KdNode* childNode = new KdNode();
		node._lchild = childNode;
		double oldMax = maximum[dim];
		maximum[dim] = node._cutVal;
		createTree(*childNode, start, mid, maximum, minimum);
		maximum[dim] = oldMax;
	}

	if (end-mid <= _bucketSize || (maximum[dim] - minimum[dim]) < MYNZERO ) {
//	if (end-mid <= _bucketSize) {
		// new leaf
		KdLeaf* leaf = new KdLeaf();
		node._rchild = leaf;
		leaf->_points = (_points+mid);
		leaf->_nOfElements = end-mid;
	}
	else {
		// new node
		minimum[dim] = node._cutVal;
		KdNode* childNode = new KdNode();
		node._rchild = childNode;
		createTree(*childNode, mid, end, maximum, minimum);
	}
}

void KdTree::getSpread(MyPoint* points, int nOfPoints, VECTOR3 &maximum, VECTOR3 &minimum) {
	//VECTOR3 pos = points->pos;
	double* pos = points->ToDouble();
	maximum = VECTOR3(pos[0], pos[1], pos[2]);
	minimum = VECTOR3(pos[0], pos[1], pos[2]);
	points++;
	for (int i = 1; i < nOfPoints; i++) {
		pos = points->ToDouble();
		if (pos[0] < minimum[0]) {
			minimum[0] = pos[0];
		}
		if (pos[0] > maximum[0]) {
			maximum[0] = pos[0];
		}
		if (pos[1] < minimum[1]) {
			minimum[1] = pos[1];
		}
		if (pos[1] > maximum[1]) {
			maximum[1] = pos[1];
		}
		if (pos[2] < minimum[2]) {
			minimum[2] = pos[2];
		}
		if (pos[2] > maximum[2]) {
			maximum[2] = pos[2];
		}
		points++;
	}
}

void KdTree::getMinMax(MyPoint *points, int nOfPoints, int dim, double &min, double &max) {
	double* pos = points->ToDouble();
	min = pos[dim];
	max = pos[dim];
	points++;
	for (int i=1; i<nOfPoints; i++) {
		if (pos[dim] < min) {
			min = pos[dim];
		}
		else if (pos[dim] > max) {
			max = pos[dim];
		}
		points++;
	}
}


void KdTree::splitAtMid(MyPoint *points, int nOfPoints, int dim, double cutVal, int &br1, int &br2) {
    int l = 0;
    int r = nOfPoints-1;
	double* pos = points->ToDouble();
    for(;;) {				// partition points[0..n-1] about the cut value
		while (l < nOfPoints &&pos[dim] < cutVal) {
			l++;
		}
		while (r >= 0 && pos[dim] >= cutVal) {
			r--;
		}
		if (l > r) 
			break;
		SWAP_POINTS(l,r);
		l++; 
		r--;
    }
    br1 = l;			// now: points[0..br1-1] < cutVal <= points[br1..n-1]
    r = nOfPoints-1;
    for(;;) {				// partition points[br1..n-1] about cutVal
		while (l < nOfPoints &&pos[dim] <= cutVal) { 
			l++;
		}
		while (r >= br1 && pos[dim] > cutVal) {
			r--;
		}
		if (l > r) 
			break;
		SWAP_POINTS(l,r);
		l++; 
		r--;
    }
    br2 = l;			// now: points[br1..br2-1] == cutVal < points[br2..n-1]
}


void KdNode::print_node(){
	cout<<"_dim: "<<(int)_dim<<" _cutVal: "<<_cutVal<<endl;
	cout<<"lchild: "<<endl;
	_lchild->print_node();
	
	cout<<"rchild: "<<endl;	
	_rchild->print_node();
}

void KdLeaf::print_node()
{
	MyPoint* point = _points;
	for(register unsigned int i=0; i<_nOfElements; i++){
		cout<<point->Index()<<" ";
		point ++;
	}
	cout<<endl;
}