## Intro
ECSystems contain components, (sub)systems, and entities.

I wanted to create (sub)systems that check if entities have certain components. Checking with if-statements would be easy, but might not be fast due to consistently looking up the components. So, with some researching and brainstorming, I decided that a bit-key(Entity) and a bit-lock(System) might work.

## Basics
- When components subscribe to an entity. I update the key of the entity. This key is different from the id of an entity.
- (Sub)Systems will be able to construct/receive a key in their constructor.

## But how does the key lock system work?
Keys and locks are basically 32-bit/64-bit variables. Each bit of the variable represents a component. 
(Note: A 32-bit, 64-bit, or xx-bit variable depending if it actually would be faster. Has to do with CPU cache magic.)
(Note: A limit of the system would be that there are only xx-bit components supported.)

When a system wants to update, the operation will go as follows; (will use a 4-bit key to explain.)
Entity's key var    = 1001; (Each bit represents a component, Transform, Sprite, etc.)
System's lock var   = 1000. (Each bit represents a component. This lock expects a component that has been assigned to the fourth bit. It doesn't care about other components.)

With a simple bitwise operation. I can see if the key fits in the lock.
(When a lock has with a bit that is true, it means it wants the component to be present in the entity. A bit that is 0/false means that it doesn't care about that component being present.)
((EKey & SysKey) == SysKey) - If True ~ It fits. Otherwise not.
e.g.; ((1001 & 1000) == 1000) It is true, because 1000 == 1000
e.g.; ((0001 & 1000) == 1000) Is false  , because 0000 !== 1000

If the calculation is true, the entity has the components that the system expects, and it is able to update it. Otherwise, it will just skip to the next entity.

(Note; Maybe need to find a way to make looping over all the entities faster?)

## But how does one dynamically create keys/locks without hardcoding components to each bit?
(Note; Will maybe explore some metaprogramming if that could be another solution or how to manipulate the order of static initialization, which isn't really possible so that will be magic.)
At static initialization, combined with a macro, such as; "EnableComponent(class name)", I can get the typeid of a class and save it in a vector or array[64] of size_t's. The order when these ids get added to this vector will represent the position inside of the 64bit variable.

This vector/array would be used combined with a getter templated function and an update function. For instance; 
int GetCompKeyPos<"class">(); possibily.
bool UpdateLock(bool add, int comp_pos);

### Getter function:
template <class T>
int GetCompKeyPos() {
    for (auto id : comp_id_array) {
        id == typeid(T);
    }
}

### Key Creation:
Whenever a component subscribes to an entity. The entity will get the class of the id and search up the position that it needs to be inside of its key variable. It will get the position with the function and with some more bitwise operations/update key function, you can "position" the bit correctly. And voila, an updated key that represents what components an entity has.

### Lock Creation:
Same way, but then in the constructor of the system. The user just specifies which component classes need to be added to the key. 
bool UpdateLock(bool add, int comp_pos);
UpdateLock(true, GetCompKeyPos<Transform>());
UpdateLock(true, GetCompKeyPos<Physics>());


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