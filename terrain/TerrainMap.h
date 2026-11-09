#pragma once
#include <memory>
#include <string>
#include <vector>
#include "NeutralObject.h"
#include "Terrain.h"
#include "TerrainProxy.h"


class TerrainMap {
public:
    TerrainMap(int width, int height);   


    TerrainMap(const TerrainMap& other);
    TerrainMap(TerrainMap&& other) = default;         
    TerrainMap& operator=(const TerrainMap& other) = delete;
    TerrainMap& operator=(TerrainMap&& other) = delete;

    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
    bool inBounds(int x, int y) const { return x >= 0 && x < width_ && y >= 0 && y < height_; }

    void setTerrain(int x, int y, std::unique_ptr<Terrain> terrain);
    
    void loadFromText(const std::vector<std::string>& rows);

    const Terrain& getTerrain(int x, int y) const;       
    const Terrain& getRealTerrain(int x, int y) const;  
    const TerrainProxy& getProxy(int x, int y) const;

    void placeNeutralObject(int x, int y, std::unique_ptr<NeutralObject> object);
    NeutralObject* getNeutralObjectAt(int x, int y) const;   

private:
    void checkCell(int x, int y) const;
    void allocate();

    int width_;
    int height_;
    std::vector<std::vector<std::unique_ptr<Terrain> > > cells_;
    std::vector<std::vector<std::unique_ptr<TerrainProxy> > > proxies_;
    std::vector<std::vector<std::unique_ptr<NeutralObject> > > objects_;
};
