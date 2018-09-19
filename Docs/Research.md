# Digital - Research

---

## ECS / Core

### Core Loop
- http://gameprogrammingpatterns.com/game-loop.html
- https://robbinmarcus.blogspot.com/2016/07/the-next-step-in-entity-component.html
- https://gafferongames.com/post/fix_your_timestep/
- https://www.reddit.com/r/gamedev/comments/22k6pl/fixed_time_step_vs_variable_time_step/

Note #1; Might want to implement a form of semi fixed time step. This would ensure that the physics wouldn't mess up and should be consistent due to fixed step. The rendering would be separate from the time step and will push out pixels as fast as possible.

The renderer basically creates time for the game loop to consume. With some added lerp/calculations, this can be smoothed out.

Question is, does this also work with "normal" games or does this only apply to physics simulations?

### ECS Basic Information
- https://github.com/junkdog/artemis-odb/wiki/Introduction-to-Entity-Systems
- http://cowboyprogramming.com/2007/01/05/evolve-your-heirachy/
- (Rendering Logic) https://gamedev.stackexchange.com/questions/50531/entity-component-based-engine-rendering-separation-from-logic
- http://t-machine.org/index.php/2007/09/03/entity-systems-are-the-future-of-mmog-development-part-1/

### ECS Tobias Sten
- https://githubcom/tobias-stein/EntityComponentSystem/tree/master/EntityComponentSystem/include/ECS
- https://www.gamedev.net/articles/programming/general-and-gameplay-programming/the-entity-component-system-c-game-design-pattern-part-1-r4803/
- https://tsprojectsblog.wordpress.com/portfolio/entity-component-system/

### ECS Redxdv
- https://github.com/redxdev/ECS

##

- Check if Entity is valid to be updated by the system. Avoids dynamic_cast checking
- System lighter getter function.


## Thinking space
- Use
- entity creates comp and suscribes to itself
- entity stores comp as shared ptr in a unordered map (xx, shared_ptr)
    - unordered map (typeinfo, shared_ptr)-> Should work on runtime, but when saving/loading?
    - entity getter; entity tries to find typeinfo of class that you want to get and returns weak_ptr if it can ! otherwise a nullptr.
    - entity can only store unique components, no components of the same type can be present in the map.
- ECS Input update - Separate Thread
- ECS ResourceMangement update?
- ECS loops & updates systems:
    - System Update:
        - key-lock check for early exit
        - getcomp weakptr and check lock
        - do update on data from components
- ECS rendering Update - Separate Thread?


## Static Init Reflection C++ Proporsol
- http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0194r0.pdf

## Id System class (Simlpe Incremental ID Counting)
- Entity Simple 32bit int.
- Component 32bit int.
- System 8bit int.

- https://gamedev.stackexchange.com/questions/47724/how-should-ids-be-assigned-in-an-entity-component-system


## Questions:
- How to update ecs
- id system
- componentmanager/system component manager?
    - Giving systesm a list of components that they can store.
- Component ownership
    - Entities will hold components in a map
    - Entities are only just "glue" to keep thing together/structured.

---