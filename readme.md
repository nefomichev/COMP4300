
# Lecture 1 

Objective oriented design is not working for game development due to the lack of flexibility. On the other hand, object oriented programming works. 

Instead of using OOP design better use ECS Game programming.

#### ECS uses Composition-based desgin.
- **E**ntity: any object in the game: Player, Platform, Bullet
- **C**omponent: Properties attached to the entities. Position, Texture ... . Components are pure data.
- **S**ystem: Code / logic drives the behavior: Movement, Rendering, Sound
#### Engine Architecture

GameEngine: main class
- Scene: world map, battle, area
    - Systems: functions to manage the changes
    - Entity Manager: Factory design pattern. Ask manager to create, destroy, manage memory ... . 
        - Entity: container to store components
            - Component
