#pragma once

#include <map>
#include <vector>

#include "cs225/PNG.h"

#include "kdtree.h"
#include "mosaiccanvas.h"
#include "sourceimage.h"
#include "tileimage.h"

using namespace cs225;

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage> & theTiles);
