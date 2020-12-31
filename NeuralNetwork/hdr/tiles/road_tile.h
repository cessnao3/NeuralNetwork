#ifndef __IO_ROAD_TILE__
#define __IO_ROAD_TILE__

#include <allegro5/allegro.h>
#include <cstdint>

/// <summary>
/// Defines the base class for a generic road tile
/// </summary>
class RoadTile
{
public:
    /// <summary>
    /// Constructs and initializes the road tile
    /// </summary>
    RoadTile();

    /// <summary>
    /// Copy constructor to create a new road tile from another
    /// </summary>
    /// <param name="other">the other road tile to check</param>
    RoadTile(const RoadTile& other);

    /// <summary>
    /// Assignment operator to copy a road tile
    /// </summary>
    /// <param name="other">the road tile to copy</param>
    /// <returns>a reference to the current road tile</returns>
    RoadTile& operator=(const RoadTile& other);

    /// <summary>
    /// Returns the bitmap associated with the tile, or nullptr if not
    /// initialized
    /// </summary>
    /// <returns>the tile's bitmap</returns>
    ALLEGRO_BITMAP* get_bitmap() const;

    /// <summary>
    /// Checks to see if the point requested, in tile-relative coordinates,
    /// is within the tile
    /// </summary>
    /// <param name="x">x location</param>
    /// <param name="y">y location</param>
    /// <returns>true if the point is on the road</returns>
    virtual bool point_on_road(
        const double x,
        const double y) const = 0;

    /// <summary>
    /// Initializes the bitmap
    /// </summary>
    virtual void init_bitmap();

    /// <summary>
    /// Destructs the road tile
    /// </summary>
    ~RoadTile();

protected:
    /// <summary>
    /// Performs any additional drawing required for the road tile
    /// </summary>
    virtual void draw_bitmap() = 0;

    /// <summary>
    /// Prepares the bitmap for drawing
    /// </summary>
    virtual void prepare_bitmap();

    /// <summary>
    /// Finishes any final layer on top of the drawn bitmap
    /// </summary>
    virtual void finish_bitmap();

protected:
    /// <summary>
    /// Returns the color defined for the road
    /// </summary>
    /// <returns>the road colo</returns>
    static ALLEGRO_COLOR get_road_color();

    /// <summary>
    /// Returns the color defines for the grass
    /// </summary>
    /// <returns>the grass color</returns>
    static ALLEGRO_COLOR get_grass_color();

protected:
    /// <summary>
    /// Stores the tile's bitmap
    /// </summary>
    ALLEGRO_BITMAP* bitmap = nullptr;

public:
    /// <summary>
    /// Defines the overall tile size
    /// </summary>
    static const uint32_t TILE_SIZE;

    /// <summary>
    /// Defines the overall road width
    /// </summary>
    static const uint32_t ROAD_WIDTH;

protected:
    /// <summary>
    /// Defines the larger road with/radius value from an edge or corner
    /// </summary>
    static const uint32_t ROAD_WIDTH_LARGE;

    /// <summary>
    /// Defines the smaller road with/radius value from an edge or corner
    /// </summary>
    static const uint32_t ROAD_WIDTH_SMALL;
};

#endif
