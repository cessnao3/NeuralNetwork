#ifndef __IO_TILE_GRID__
#define __IO_TILE_GRID__

#include "tiles/tile_manager.h"

#include <vector>
#include <cstdint>

/// <summary>
/// Defines the road grid/track state manager
/// </summary>
class RoadGrid
{
public:
    /// <summary>
    /// Defines the parameters required for a basic grid location
    /// </summary>
    struct GridLoc
    {
    public:
        /// <summary>
        /// Defines the actual road tile pointer
        /// </summary>
        RoadTile* tile = nullptr;

        /// <summary>
        /// Defines the x location of the tile, for the left edge
        /// </summary>
        uint32_t x = 0;

        /// <summary>
        /// Defines the y location of the tile, for the top edge
        /// </summary>
        uint32_t y = 0;

        /// <summary>
        /// Defines the type of the tile
        /// </summary>
        RoadTileManager::RoadTileType type = RoadTileManager::RoadTileType::GRASS;

        /// <summary>
        /// Provides the center X location for the tile
        /// </summary>
        /// <returns>tile center X location</returns>
        const uint32_t get_center_x() const;

        /// <summary>
        /// Provides the center Y location for the tile
        /// </summary>
        /// <returns>tile center Y location</returns>
        const uint32_t get_center_y() const;
    };

public:
    /// <summary>
    /// Defines the height and width parameters for the tile grid
    /// </summary>
    /// <param name="width">the number of tiles in the width dimension</param>
    /// <param name="height">the number of tiles in the height dimension</param>
    RoadGrid(
        size_t width,
        size_t height);

    /// <summary>
    /// Initializes all tile bitmaps contained in the grid
    /// </summary>
    void init_bitmaps();

    /// <summary>
    /// Resets any tile pointers stored within grid locations if necessary.
    /// This would be required after copy-constructing or similar
    /// </summary>
    void reset_tile_pointers();

    /// <summary>
    /// Provides the grid location at a given row/col index
    /// </summary>
    /// <param name="row">the selected row</param>
    /// <param name="col">the selected column</param>
    /// <returns>the grid location associated with the row/col</returns>
    GridLoc* at(const size_t row, const size_t col);

    /// <summary>
    /// Provides the grid location at a given index
    /// </summary>
    /// <param name="ind">the selected index</param>
    /// <returns>the grid location associated with the index</returns>
    GridLoc* at(const size_t ind);

    /// <summary>
    /// Provides the grid location at a given row/col index
    /// </summary>
    /// <param name="row">the selected row</param>
    /// <param name="col">the selected column</param>
    /// <returns>the grid location associated with the row/col</returns>
    const GridLoc* at(const size_t row, const size_t col) const;

    /// <summary>
    /// Provides the grid location at a given index
    /// </summary>
    /// <param name="ind">the selected index</param>
    /// <returns>the grid location associated with the index</returns>
    const GridLoc* at(const size_t ind) const;

    /// <summary>
    /// Provides a pointer constant for the tile at the given grid location
    /// </summary>
    /// <param name="x">the x coordinate of the tile</param>
    /// <param name="y">the y coordinate of the tile</param>
    /// <returns>the tile, or nullptr if none exists</returns>
    const GridLoc* get_for_xy(const double x, const double y) const;

    /// <summary>
    /// Provides a pointer constant for the tile at the given grid location
    /// </summary>
    /// <param name="x">the x coordinate of the tile</param>
    /// <param name="y">the y coordinate of the tile</param>
    /// <returns>the tile, or nullptr if none exists</returns>
    GridLoc* get_for_xy(const double x, const double y);

    /// <summary>
    /// Sets the given location to be a tile of the given type
    /// </summary>
    /// <param name="row">the row index</param>
    /// <param name="col">the column index</param>
    /// <param name="type">the tile type to set</param>
    void set(const size_t row, const size_t col, const RoadTileManager::RoadTileType type);

    /// <summary>
    /// Sets the given location to be a tile of the given type
    /// </summary>
    /// <param name="ind">the row index</param>
    /// <param name="type">the tile type to set</param>
    void set(const size_t ind, const RoadTileManager::RoadTileType type);

    /// <summary>
    /// Returns the tile width of the grid
    /// </summary>
    /// <returns>grid tile width</returns>
    size_t get_width() const;

    /// <summary>
    /// Returns the tile height of the grid
    /// </summary>
    /// <returns>grid tile height</returns>
    size_t get_height() const;

    /// <summary>
    /// Sets the starting index for the car
    /// </summary>
    /// <param name="start_ind">new car starting index</param>
    void set_start_ind(const size_t start_ind);

    /// <summary>
    /// Sets the starting index for the car
    /// </summary>
    /// <param name="row">new car row starting index</param>
    /// <param name="col">new car column starting index</param>
    void set_start_ind(const size_t row, const size_t col);

    /// <summary>
    /// Provides the overall grid starting index for the car
    /// </summary>
    /// <returns>car starting index</returns>
    size_t get_start_ind() const;

protected:
    /// <summary>
    /// Provides a pointer constant for the tile at the given grid location
    /// </summary>
    /// <param name="x">the x coordinate of the tile</param>
    /// <param name="y">the y coordinate of the tile</param>
    /// <returns>the tile, or nullptr if none exists</returns>
    const GridLoc* get_for_xy_const(const double x, const double y) const;

    /// <summary>
    /// Converts a row/column to an index
    /// </summary>
    /// <param name="row">row index</param>
    /// <param name="col">col index</param>
    /// <returns>overall index</returns>
    size_t rc_to_ind(const size_t row, const size_t col) const;

    /// <summary>
    /// Converts an overall index to a row
    /// </summary>
    /// <param name="ind">overall index</param>
    /// <returns>associated row value</returns>
    size_t ind_to_r(const size_t ind) const;

    /// <summary>
    /// Converts an overall index to a column
    /// </summary>
    /// <param name="ind">overall index</param>
    /// <returns>associated column value</returns>
    size_t ind_to_c(const size_t ind) const;

protected:
    /// <summary>
    /// Defines the tile-width of the grid
    /// </summary>
    size_t width;

    /// <summary>
    /// Defines the tile-height of the grid
    /// </summary>
    size_t height;

    /// <summary>
    /// Defines the car starting index
    /// </summary>
    size_t start_ind;

    /// <summary>
    /// Defines the grid tiles
    /// </summary>
    std::vector<GridLoc> tiles;

    /// <summary>
    /// Defines the grid manager to use
    /// </summary>
    RoadTileManager manager;
};

#endif
