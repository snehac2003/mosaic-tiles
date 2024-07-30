#include <iostream>
#include <map>

#include "maptiles.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{

    int rows = theSource.getRows();
    int cols = theSource.getColumns();

    MosaicCanvas* mosaic = new MosaicCanvas(rows, cols);

    vector<Point<3>> avgCols;
    map<Point<3>, TileImage*> c;

    for (const TileImage& tile : theTiles) {
        Point<3> avgCol = convertToXYZ(tile.getAverageColor());
        avgCols.push_back(avgCol);
        c[avgCol] = const_cast<TileImage*>(&tile);
    }

    KDTree<3> colorTree(avgCols);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Point<3> tc = convertToXYZ(theSource.getRegionColor(i,j));
            Point<3> nc = colorTree.findNearestNeighbor(tc);
            mosaic->setTile(i,j,c[nc]);
        }
    }
    return mosaic;

}
