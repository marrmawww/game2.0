#include "TerrainMap.h"
#include <stdexcept>
#include "Forest.h"
#include "Mountains.h"
#include "Plains.h"
#include "Swamp.h"


void TerrainMap::allocate() {
    cells_.resize(height_);
    proxies_.resize(height_);
    objects_.resize(height_);
    for (int y = 0; y < height_; ++y) {
        cells_[y].resize(width_);
        proxies_[y].resize(width_);
        objects_[y].resize(width_);
    }
}

TerrainMap::TerrainMap(int width, int height) {
    if (width <= 0 || height <= 0) throw std::invalid_argument("Карта: неверные размеры");
    width_ = width;
    height_ = height;
    allocate();
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            setTerrain(x, y, std::unique_ptr<Terrain>(new Plains()));
        }
    }
}

TerrainMap::TerrainMap(const TerrainMap& other) {
    width_ = other.width_;
    height_ = other.height_;
    allocate();
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            setTerrain(x, y, other.cells_[y][x]->clone());
            if (other.objects_[y][x]) {
                objects_[y][x] = other.objects_[y][x]->clone();
            }
        }
    }
}

void TerrainMap::checkCell(int x, int y) const {
    if (!inBounds(x, y)) throw std::out_of_range("Карта: клетка вне поля");
}

void TerrainMap::setTerrain(int x, int y, std::unique_ptr<Terrain> terrain) {
    checkCell(x, y);
    cells_[y][x] = std::move(terrain);

    proxies_[y][x].reset(new TerrainProxy(cells_[y][x].get()));
}

void TerrainMap::loadFromText(const std::vector<std::string>& rows) {
    if (static_cast<int>(rows.size()) != height_) throw std::invalid_argument("Карта: неверное число строк");
    for (int y = 0; y < height_; ++y) {
        if (static_cast<int>(rows[y].size()) != width_) throw std::invalid_argument("Карта: неверная длина строки");
        for (int x = 0; x < width_; ++x) {
            char c = rows[y][x];
            if (c == '.')      setTerrain(x, y, std::unique_ptr<Terrain>(new Plains()));
            else if (c == '*') setTerrain(x, y, std::unique_ptr<Terrain>(new Forest()));
            else if (c == '~') setTerrain(x, y, std::unique_ptr<Terrain>(new Swamp()));
            else if (c == '^') setTerrain(x, y, std::unique_ptr<Terrain>(new Mountains()));
            else throw std::invalid_argument("Карта: неизвестный символ ландшафта");
        }
    }
}

const Terrain& TerrainMap::getTerrain(int x, int y) const {
    checkCell(x, y);
    return *proxies_[y][x];
}

const Terrain& TerrainMap::getRealTerrain(int x, int y) const {
    checkCell(x, y);
    return *cells_[y][x];
}

const TerrainProxy& TerrainMap::getProxy(int x, int y) const {
    checkCell(x, y);
    return *proxies_[y][x];
}

void TerrainMap::placeNeutralObject(int x, int y, std::unique_ptr<NeutralObject> object) {
    checkCell(x, y);
    objects_[y][x] = std::move(object);
}

NeutralObject* TerrainMap::getNeutralObjectAt(int x, int y) const {
    checkCell(x, y);
    return objects_[y][x].get();
}
