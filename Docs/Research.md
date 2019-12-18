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

Logging Reesearch
- https://www.google.nl/search?q=logging+macros+c%2B%2B&oq=logging+macros+c%2B%2B&aqs=chrome..69i57.2333j0j7&sourceid=chrome&ie=UTF-8
- https://stackoverflow.com/questions/19415845/a-better-log-macro-using-template-metaprogramming
- http://www.drdobbs.com/cpp/a-lightweight-logger-for-c/240147505
- https://coderwall.com/p/v6u7jq/a-simplified-logging-system-using-macros
- https://github.com/dmcrodrigues/macro-logger
- https://github.com/dmcrodrigues/macro-logger/blob/master/macrologger.h
- https://en.cppreference.com/w/cpp/chrono/system_clock
- https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
- https://en.cppreference.com/w/cpp/locale/time_put
- https://stackoverflow.com/questions/16357999/current-date-and-time-as-string
- https://stackoverflow.com/questions/24923164/macro-expands-correctly-but-gives-me-expected-expression-error
- http://www.cplusplus.com/doc/tutorial/preprocessor/
- https://stackoverflow.com/questions/8520560/get-a-file-name-from-a-path

---

Reflection, Self-Registering of Classes, Factory - https://www.one-tab.com/page/P785SLAUQiaYL53dTcGoOQ
- http://www.vollmann.com/en/pubs/meta/meta/meta.html
- https://www.reddit.com/r/learnprogramming/comments/1jzlon/cwhy_exactly_is_rtti_and_typeid_bad/
- http://www.nirfriedman.com/2018/04/29/unforgettable-factory/
- http://coliru.stacked-crooked.com/a/11473a649e402831
- http://www.dorodnic.com/blog/2014/03/29/abstract-factory/
- https://stackoverflow.com/questions/281818/unmangling-the-result-of-stdtype-infoname
- https://stackoverflow.com/questions/35273312/two-separate-keys-map-to-a-single-entry-in-a-stdmap
- https://stackoverflow.com/questions/32685540/unordered-map-with-pair-as-key-not-compiling
- https://stackoverflow.com/questions/7222143/unordered-map-hash-function-c
- https://stackoverflow.com/questions/33386672/boostcoredemangle-on-visual-studio-typeid-name

- https://preshing.com/20180116/a-primitive-reflection-system-in-cpp-part-1/
- https://www.bfilipek.com/2018/02/factory-selfregister.html
- http://www.dorodnic.com/blog/2014/03/29/abstract-factory/
- https://www.codeproject.com/Articles/567242/AplusC-b-bplusObjectplusFactory
- http://www.nirfriedman.com/2018/04/29/unforgettable-factory/
- https://stackoverflow.com/questions/9859390/use-data-type-class-type-as-key-in-a-map
- https://en.cppreference.com/w/cpp/utility/functional/function

- https://www.google.nl/search?ei=KkjMW7SCEtCcsAe5h5ngBw&q=dynamic+factory+patterns+c%2B%2B&oq=dynamic+factory+patterns+c%2B%2B&gs_l=psy-ab.3...2296.3834.0.4098.0.0.0.0.0.0.0.0..0.0....0...1c.1.64.psy-ab..0.0.0....0.YCQGhBP2Qwo
- https://stackoverflow.com/questions/5120768/how-to-implement-the-factory-method-pattern-in-c-correctly
- https://stackoverflow.com/questions/38644146/choose-template-based-on-run-time-string-in-c
- https://en.cppreference.com/w/cpp/language/typeid

- https://stackoverflow.com/questions/24702235/c-stdmap-holding-any-type-of-value
- https://en.cppreference.com/w/cpp/utility/variant
- http://www.cplusplus.com/reference/typeinfo/type_info/hash_code/
- https://blog.noctua-software.com/object-factory-c++.html
- http://www.cs.sjsu.edu/~pearce/modules/lectures/oop/types/reflection/prototype.htm
- https://stackoverflow.com/questions/18701798/building-and-accessing-a-list-of-types-at-compile-time



--------------------------------------------------------------

NEW ECES:

- https://www.draw.io/?state=%7B%22ids%22:%5B%2213GcS4CyEwjydRQzCYbarpUUDdfwmE0Ix%22%5D,%22action%22:%22open%22,%22userId%22:%22107182148045614620852%22%7D#G13GcS4CyEwjydRQzCYbarpUUDdfwmE0Ix

- https://github.com/alecthomas/entityx
- https://github.com/alecthomas/entityx/blob/master/entityx/Entity.cc
- https://github.com/redxdev/ECS
- https://github.com/redxdev/ECS/blob/master/ECS.h
- https://www.youtube.com/watch?v=tONOW7Luln8

- https://kevinushey.github.io/blog/2016/01/27/introduction-to-c++-variadic-templates/

- http://t-machine.org/index.php/2014/03/08/data-structures-for-entity-systems-contiguous-memory/
- https://en.cppreference.com/w/cpp/language/parameter_pack

- https://stackoverflow.com/questions/5533354/what-does-a-call-to-this-template-somename-do
- https://stackoverflow.com/questions/36612596/tuple-to-parameter-pack
- https://en.cppreference.com/w/cpp/utility/tuple/get
- https://en.cppreference.com/w/cpp/utility/tuple

- https://github.com/mtrebi/memory-allocators

- http://umich.edu/~eecs381/handouts/UMLNotationSummary.pdf
- https://www.gamedev.net/forums/topic/691504-ecs-mega-entity-array-vs-many-component-arrays/?page=3
- https://kevinushey.github.io/blog/2016/01/27/introduction-to-c++-variadic-templates/

- https://stackoverflow.com/questions/24015710/for-loop-over-template-arguments-types