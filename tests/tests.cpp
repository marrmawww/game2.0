// Простые автоматические проверки (без сторонних библиотек).
#include <iostream>
#include "Elf.h"
#include "GameController.h"
#include "Logger.h"
#include "NeutralOperators.h"

static int failed = 0;
static int total = 0;

#define CHECK(cond)                                                          \
    do {                                                                     \
        ++total;                                                             \
        if (!(cond)) { ++failed; std::cout << "ПРОВАЛ: " #cond " (строка " << __LINE__ << ")\n"; } \
    } while (0)

static void testTerrain() {
    TerrainMap map(3, 1);
    map.loadFromText(std::vector<std::string>(1, "*~^"));
    const Terrain& forest = map.getTerrain(0, 0);
    const Terrain& swamp = map.getTerrain(1, 0);
    const Terrain& mountains = map.getTerrain(2, 0);

    // горы: только летающие и альпинисты
    CHECK(!mountains.isPassable(UnitKind::Swordsman));
    CHECK(!mountains.isPassable(UnitKind::Cataphract));
    CHECK(mountains.isPassable(UnitKind::ElfRanger));
    CHECK(mountains.isPassable(UnitKind::GryphonRider));
    // лес: тяжёлые не проходят, бонус защиты есть, пехота медленнее
    CHECK(!forest.isPassable(UnitKind::Cataphract));
    CHECK(forest.isPassable(UnitKind::Swordsman));
    CHECK(forest.getDefenseBonus(UnitKind::Swordsman) > 0.0);
    CHECK(forest.getSpeedModifier(UnitKind::Swordsman) < 1.0);
    // болото: замедляет всех, кроме летающих
    CHECK(swamp.getSpeedModifier(UnitKind::Swordsman) <= 0.5);
    CHECK(swamp.getSpeedModifier(UnitKind::HorseArcher) <= 0.5);
    CHECK(swamp.getStuckChance(UnitKind::Swordsman) > 0.0);
    CHECK(swamp.getSpeedModifier(UnitKind::GryphonRider) == 1.0);
}

static void testProxy() {
    TerrainMap map(1, 1);
    map.loadFromText(std::vector<std::string>(1, "^"));
    const TerrainProxy& proxy = map.getProxy(0, 0);
    // getTerrain отдаёт именно прокси
    CHECK(&map.getTerrain(0, 0) == &proxy);
    CHECK(!proxy.isPassable(UnitKind::Swordsman));
    CHECK(proxy.cacheMisses() == 1);
    CHECK(!proxy.isPassable(UnitKind::Swordsman));
    CHECK(proxy.cacheHits() == 1);                  // второй ответ — из кеша
    CHECK(proxy.isPassable(UnitKind::GryphonRider)); // летающий — в обход реального ландшафта
    CHECK(proxy.flyingBypasses() == 1);
}

static void testBaseCounter() {
    Base base("Тест", 'B', 0, 0, 50, 2, 1);
    std::shared_ptr<Unit> a = base.createUnit(UnitKind::Swordsman, 1, 1);
    std::shared_ptr<Unit> b = base.createUnit(UnitKind::LightArcher, 2, 2);
    CHECK(base.getAliveCount() == 2);
    CHECK(base.createUnit(UnitKind::Spearman, 3, 3) == nullptr);   // лимит
    a->takeTrueDamage(1000);                                        // гибель -> Destroyed
    CHECK(base.getAliveCount() == 1);
    CHECK(base.getDestroyedHistory().size() == 1);
    a->takeTrueDamage(10);                                          // повторной гибели нет
    CHECK(base.getAliveCount() == 1);
    CHECK(base.createUnit(UnitKind::Spearman, 3, 3) != nullptr);    // лимит освободился
    base.takeDamage(30);                                            // 30 - защита 1
    CHECK(base.getHealth() == 21);
    base.purgeDead();
    CHECK(base.getUnits().size() == 2);
}

static void testObserverLifetime() {
    std::shared_ptr<Unit> unit;
    {
        Base base("Временная", 'B', 0, 0, 50, 3, 0);
        unit = base.createUnit(UnitKind::Swordsman, 0, 1);
        CHECK(unit->observerCount() == 1);
    }   // база уничтожена раньше юнита
    CHECK(unit->observerCount() == 0);
    CHECK(unit->getOwner() == nullptr);
    unit->takeTrueDamage(5);   // не должно упасть
    CHECK(unit->getHealth() == 95);

    // копия юнита ни на кого не подписана и получает новый id
    std::unique_ptr<Unit> copy = unit->clone();
    CHECK(copy->observerCount() == 0);
    CHECK(copy->getId() != unit->getId());
}

static void testNeutralObjects() {
    Base base("Тест", 'B', 0, 0, 50, 10, 0);
    std::shared_ptr<Unit> sword = base.createUnit(UnitKind::Swordsman, 1, 1);
    std::shared_ptr<Unit> archer = base.createUnit(UnitKind::LightArcher, 1, 2);
    std::shared_ptr<Unit> horse = base.createUnit(UnitKind::HorseArcher, 1, 3);
    std::shared_ptr<Unit> elf = base.createUnit(UnitKind::ElfDruid, 1, 4);

    TreasureChest chest;
    *sword + chest;
    CHECK(base.getResources().gold == 50);
    *sword + chest;                         // второй раз пусто
    CHECK(base.getResources().gold == 50);

    Trap trap(25);
    *sword - trap;
    CHECK(sword->getHealth() == 75);

    EnergySource source;
    *sword / source;
    CHECK(sword->getHealth() == sword->getMaxHealth());
    sword->takeTrueDamage(10);
    *sword / source;                         // этот юнит уже лечился: проверка посещения
    CHECK(sword->getHealth() == sword->getMaxHealth() - 10);
    CHECK(source.hasVisited(sword->getId()));
    CHECK(!source.hasVisited(archer->getId()));

    // Стратегия: один алтарь, разные эффекты
    Altar altar;
    int attackBefore = sword->getAttackValue();
    int rangeBefore = archer->getEffectiveRange();
    int speedBefore = horse->getSpeed();
    elf->takeTrueDamage(20);
    int elfHealthBefore = elf->getHealth();
    *sword * altar;
    *archer * altar;
    *horse * altar;
    *elf * altar;
    *elf * altar;                            // повторно — нельзя
    CHECK(sword->getAttackValue() > attackBefore);
    CHECK(archer->getEffectiveRange() > rangeBefore);
    CHECK(horse->getSpeed() > speedBefore);
    CHECK(sword->getEffectiveRange() == 1);    // у пехоты дальность не менялась
    CHECK(elf->getHealth() > elfHealthBefore); // эльфу алтарь восстанавливает здоровье
    // бонусы временные
    for (int i = 0; i < 3; ++i) sword->endTurn();
    CHECK(sword->getAttackValue() == attackBefore);
}

static void testMovement() {
    GameController game(6, 3, 1);
    game.map().loadFromText(std::vector<std::string>{ "..*^..", "......", "..~..." });
    Base& base = game.addBase("Б", 'B', 0, 0, 50, 10, 0);
    std::shared_ptr<Unit> sword = game.spawnUnit(base, UnitKind::Swordsman, 3, 1);
    CHECK(!game.moveUnit(*sword, 0, -1));                         // горы (3,0)
    CHECK(sword->getX() == 3 && sword->getY() == 1);              // не сдвинулся
    CHECK(game.moveUnit(*sword, -1, -1));                         // лес (2,0): цена 2 = все очки
    CHECK(sword->getMovePoints() == 0.0);
    CHECK(!game.moveUnit(*sword, -1, 0));                         // очков нет
    game.endTurn();
    CHECK(sword->getMovePoints() == 2.0);                         // новый ход
    std::shared_ptr<Unit> cata = game.spawnUnit(base, UnitKind::Cataphract, 1, 0);
    CHECK(!game.moveUnit(*cata, 1, 0));                           // тяжёлый в лес не идёт
    CHECK(cata->getX() == 1);
    std::shared_ptr<Unit> gryphon = game.spawnUnit(base, UnitKind::GryphonRider, 4, 1);
    CHECK(game.moveUnit(*gryphon, -1, -1));                       // летит над горами (3,0)
    CHECK(gryphon->getX() == 3 && gryphon->getY() == 0);
}

static void testMapCopy() {
    TerrainMap original(2, 1);
    original.loadFromText(std::vector<std::string>(1, "*^"));
    original.placeNeutralObject(0, 0, std::unique_ptr<NeutralObject>(new Trap(10)));
    TerrainMap copy(original);                       // глубокое копирование
    copy.setTerrain(0, 0, original.getRealTerrain(1, 0).clone());
    CHECK(original.getRealTerrain(0, 0).getName() == "Лес");
    CHECK(copy.getRealTerrain(0, 0).getName() == "Горы");
    CHECK(copy.getNeutralObjectAt(0, 0) != original.getNeutralObjectAt(0, 0));
    TerrainMap moved(std::move(copy));               // перемещение
    CHECK(moved.getWidth() == 2);
}

int main() {
    setConsoleLog(false);
    testTerrain();
    testProxy();
    testBaseCounter();
    testObserverLifetime();
    testNeutralObjects();
    testMovement();
    testMapCopy();
    std::cout << "Проверок: " << total << ", провалено: " << failed << "\n";
    return failed == 0 ? 0 : 1;
}
