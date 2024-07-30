#include <utility>
#include <algorithm>
#include <deque>

using namespace std;

// DONE
template <int Dim>
bool smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim)
{
  
    
    if (curDim < 0 || curDim >= Dim) {
      return false;
    }
    if (first[curDim] != second[curDim]) {
      return first[curDim] < second[curDim];
    }
    return first < second;
    
 }

// DONE
template <int Dim>
bool shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential)
{
    
    
     
     double one = 0;
     double two = 0;
     for (int i = 0; i < Dim; i++) {
      double one_mult = potential[i] - target[i];
      double two_mult = currentBest[i] - target[i];
      one += one_mult * one_mult;
      two += two_mult * two_mult;
     }
     if (one == two) {
      return potential < currentBest;
     }
     return one < two;
     
}

// DONE
template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints) 
{
    
     std::vector<Point<Dim>> pts(newPoints.begin(), newPoints.end());
     root = buildTree(pts, 0, newPoints.size()-1, 0);
     
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::buildTree(std::vector<Point<Dim>>& pts, int left, int right, int dimension) {
  if (left > right || pts.empty()) {
    return NULL;
  }
  int mid = (left+right)/2;
  select(pts.begin()+left, pts.begin()+right+1, pts.begin()+mid, [dimension](const Point<Dim>& a, const Point<Dim>& b) {
      return smallerDimVal(a,b,dimension);
  });

  int nd = (dimension+1)%Dim;
  KDTreeNode* node = new KDTreeNode(pts[mid]);
  node->left = buildTree(pts, left, mid-1, nd);
  node->right = buildTree(pts, mid+1, right, nd);
  return node;
}


// DONE
template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
 
   size = other.size;
   //copy(this->root, other.root);
   root = copy(other.root);
}

// DONE
template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  
  if (this != &rhs) {
    clear(root);
    root = copy(rhs.root);
    size = rhs.size;
  }
  return *this;
}

// DONE
template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::copy(KDTreeNode *node2) {
  if (node2 == NULL) {
    return NULL;
  }
  KDTreeNode* node1 = new KDTreeNode(node2->point);
  node1->left = copy(node2->left);
  node1->right = copy(node2->right);
  return node1;
}


// DONE
template <int Dim>
KDTree<Dim>::~KDTree() {

   clear(root);
}

// DONE
template <int Dim>
void KDTree<Dim>::clear(KDTreeNode *node) { 

  if (!node) {
    return;
  }
  clear(node->left);
  clear(node->right);
  delete node;
}

// DONE
template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
   
    return neighborHelper(query, root, 0);
}



template <int Dim>
Point<Dim> KDTree<Dim>::neighborHelper(const Point<Dim>& query, KDTreeNode * node, int dimension) const {
  if (node == nullptr) {
    return Point<Dim>();
  }
  Point<Dim> best;
  Point<Dim> ob;
  double bd = 0;
  bool gl = smallerDimVal(query, node->point, dimension);
  KDTreeNode* nextNode = gl ? node->left : node->right;
  if (nextNode) {
    best = neighborHelper(query, nextNode, (dimension+1)%Dim);
  } else {
    best = node->point;
  }
  if (shouldReplace(query, best, node->point)) {
    best = node->point;
  }
  for (int i = 0; i < Dim; i++) {
    bd += (best[i]-query[i]) * (best[i]-query[i]);
  }
  double temp = std::abs(node->point[dimension]-query[dimension]);
  if (temp * temp <= bd) {
    KDTreeNode* on = gl ? node->right : node->left;
    if (on) {
      ob = neighborHelper(query, on, (dimension+1)%Dim);
      if (shouldReplace(query, best, ob)) {
        best=ob;
      }
    }
  }
  return best;
}


// DONE
template <typename RandIter, typename Comparator>
void select(RandIter start, RandIter end, RandIter k, Comparator cmp) {
  if (start == end) {
    return;
  }
  RandIter pivot = start + (end-start) / 2;
  pivot = partition(start, end, pivot, cmp);
  if (k == pivot) {
    return;
  }
  if (k < pivot) {
    select(start, pivot, k, cmp);
  } else {
    select(pivot+1, end, k, cmp);
  }
}


// DONE
template <typename RandIter, typename Comparator>
RandIter partition(RandIter start, RandIter end, RandIter pivot, Comparator cmp) {
  std::iter_swap(pivot, end-1);
  RandIter storeIndex = start;
  for (RandIter i = start; i != end; ++i) {
    if (cmp(*i, *(end-1))) {
      std::iter_swap(i, storeIndex);
      storeIndex++;
    }
  }
  std::iter_swap(storeIndex, end-1);
  return storeIndex;
}
