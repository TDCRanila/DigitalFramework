  /**
   * Find Entities that have all of the specified Components.
   *
   * @code
   * for (Entity entity : entity_manager.entities_with_components<Position, Direction>()) {
   *   ComponentHandle<Position> position = entity.component<Position>();
   *   ComponentHandle<Direction> direction = entity.component<Direction>();
   *
   *   ...
   * }
   * @endcode
   */
  template <typename ... Components>
  View<Components...> entities_with_components() {
    auto mask = component_mask<Components ...>();
    return View<Components...>(this, mask);
  }

-----------------------------------------------------------------------

template <typename ... Components> using View = TypedView<false, Components...>;

--------------------------------------------------------------------------

  template <bool All, typename ... Components>
  class TypedView: public BaseView<All> {
  public:
    template <typename T> struct identity { typedef T type; };

    void each(typename identity<std::function<void(Entity entity, Components&...)>>::type f) {
      for (auto it : *this)
        f(it, *(it.template component<Components>().get())...);
    }

  private:
    friend class EntityManager;

    explicit TypedView(EntityManager *manager) : BaseView<All>(manager) {}
    TypedView(EntityManager *manager, ComponentMask mask) : BaseView<All>(manager, mask) {}
  };

 ------------------------------------------------------------------------------------

   template <bool All>
  class BaseView {
  public:
    class Iterator : public ViewIterator<Iterator, All> {
    public:
      Iterator(EntityManager *manager, const ComponentMask mask, uint32_t index) : ViewIterator<Iterator, All>(manager, mask, index) {
        ViewIterator<Iterator, All>::next();
      }

      void next_entity(Entity &entity) {}
    };

    Iterator begin() { return Iterator(manager_, mask_, 0); }
    Iterator end() { return Iterator(manager_, mask_, uint32_t(manager_->capacity())); }
    const Iterator begin() const { return Iterator(manager_, mask_, 0); }
    const Iterator end() const { return Iterator(manager_, mask_, manager_->capacity()); }

  private:
    friend class EntityManager;

    explicit BaseView(EntityManager *manager) : manager_(manager) { mask_.set(); }
    BaseView(EntityManager *manager, ComponentMask mask) :
        manager_(manager), mask_(mask) {}

    EntityManager *manager_;
    ComponentMask mask_;
  };

  -----------------------------------------------------------------------------------

    /// An iterator over a view of the entities in an EntityManager.
  /// If All is true it will iterate over all valid entities and will ignore the entity mask.
  template <class Delegate, bool All = false>
  class ViewIterator : public std::iterator<std::input_iterator_tag, Entity::Id> {
   public:
    Delegate &operator ++() {
      ++i_;
      next();
      return *static_cast<Delegate*>(this);
    }
    bool operator == (const Delegate& rhs) const { return i_ == rhs.i_; }
    bool operator != (const Delegate& rhs) const { return i_ != rhs.i_; }
    Entity operator * () { return Entity(manager_, manager_->create_id(i_)); }
    const Entity operator * () const { return Entity(manager_, manager_->create_id(i_)); }

   protected:
    ViewIterator(EntityManager *manager, uint32_t index)
        : manager_(manager), i_(index), capacity_(manager_->capacity()), free_cursor_(~0UL) {
      if (All) {
        std::sort(manager_->free_list_.begin(), manager_->free_list_.end());
        free_cursor_ = 0;
      }
    }
    ViewIterator(EntityManager *manager, const ComponentMask mask, uint32_t index)
        : manager_(manager), mask_(mask), i_(index), capacity_(manager_->capacity()), free_cursor_(~0UL) {
      if (All) {
        std::sort(manager_->free_list_.begin(), manager_->free_list_.end());
        free_cursor_ = 0;
      }
    }

    void next() {
      while (i_ < capacity_ && !predicate()) {
        ++i_;
      }

      if (i_ < capacity_) {
        Entity entity = manager_->get(manager_->create_id(i_));
        static_cast<Delegate*>(this)->next_entity(entity);
      }
    }

    inline bool predicate() {
      return (All && valid_entity()) || (manager_->entity_component_mask_[i_] & mask_) == mask_;
    }

    inline bool valid_entity() {
      const std::vector<uint32_t> &free_list = manager_->free_list_;
      if (free_cursor_ < free_list.size() && free_list[free_cursor_] == i_) {
        ++free_cursor_;
        return false;
      }
      return true;
    }

    EntityManager *manager_;
    ComponentMask mask_;
    uint32_t i_;
    size_t capacity_;
    size_t free_cursor_;
  };