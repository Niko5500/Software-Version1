cmake_minimum_required(VERSION 3.12)

add_executable(mygame
    main.cpp
    Enemy.cpp
    Fight.cpp
    Game.cpp
    Hero.cpp
    Grotte.cpp
    GrotteFactory.cpp
    EnemyFactory.cpp
    Weapon.cpp
    WeaponFactory.cpp
    Armory.cpp
)

# Link direkte med sqlite3
target_link_libraries(mygame PRIVATE sqlite3)
