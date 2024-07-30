#pragma once

#include <vector>
#include <iterator>
#include <utility> // new
#include <algorithm> // new

#include "util/coloredout.h"

#include "cs225/point.h"

using std::vector;
using std::string;
using std::ostream;
using std::cout;
using std::endl;


template <int Dim>
class KDTree
{
  private:
    
    struct KDTreeNode
    {
      Point<Dim> point;
      KDTreeNode *left, *right;

      KDTreeNode() : point(), left(NULL), right(NULL) {}
      KDTreeNode(const Point<Dim> &point) : point(point), left(NULL), right(NULL) {}
    };

  public:
    
    KDTree(const vector<Point<Dim>>& newPoints);

    KDTree(const KDTree<Dim>& other);

    KDTree const &operator=(const KDTree<Dim>& rhs);

    
    ~KDTree();

    
    Point<Dim> findNearestNeighbor(const Point<Dim>& query) const;

    void printTree(ostream& out = cout,
                   colored_out::enable_t enable_bold = colored_out::COUT,
                   int modWidth = -1) const;

    

    void clear(KDTreeNode *root);
    Point<Dim> neighborHelper(const Point<Dim>& query, KDTreeNode* current, int currentDim) const;
    KDTreeNode* buildTree(std::vector<Point<Dim>>& pts, int left, int right, int dimension);



  private:

    KDTreeNode* copy(KDTreeNode *node2);

    KDTreeNode *root;
    size_t size;

    int getPrintData(KDTreeNode * subroot) const;

    void printTree(KDTreeNode * subroot, std::vector<std::string>& output,
                   int left, int top, int width, int currd) const;

};


template <int Dim>
bool shouldReplace(const Point<Dim>& target, const Point<Dim>& currentBest,
                    const Point<Dim>& potential);
  
template <int Dim>
bool smallerDimVal(const Point<Dim>& first, const Point<Dim>& second,
                    int curDim);

template <typename RandIter, typename Comparator>
void select(RandIter begin, RandIter end, RandIter k, Comparator cmp);


template <typename RandIter, typename Comparator>
RandIter partition(RandIter start, RandIter end, RandIter pivot, Comparator cmp);


#include "kdtree.hpp"
#include "kdtree_extras.hpp"
