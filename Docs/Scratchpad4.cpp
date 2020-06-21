  /**
   * Find Entities that have all of the specified Components and assign them
   * to the given parameters.
   *
   * @code
   * ComponentHandle<Position> position;
   * ComponentHandle<Direction> direction;
   * for (Entity entity : entity_manager.entities_with_components(position, direction)) {
   *   // Use position and component here.
   * }
   * @endcode
   */
  template <typename ... Components>
  UnpackingView<Components...> entities_with_components(ComponentHandle<Components> & ... components) {
    auto mask = component_mask<Components...>();
    return UnpackingView<Components...>(this, mask, components...);
  }

  -----------------------------------------------------------------------

    template <typename ... Components>
  class UnpackingView {
   public:
    struct Unpacker {
      explicit Unpacker(ComponentHandle<Components> & ... handles) :
          handles(std::tuple<ComponentHandle<Components> & ...>(handles...)) {}

      void unpack(entityx::Entity &entity) const {
        unpack_<0, Components...>(entity);
      }


    private:
      template <int N, typename C>
      void unpack_(entityx::Entity &entity) const {
        std::get<N>(handles) = entity.component<C>();
      }

      template <int N, typename C0, typename C1, typename ... Cn>
      void unpack_(entityx::Entity &entity) const {
        std::get<N>(handles) = entity.component<C0>();
        unpack_<N + 1, C1, Cn...>(entity);
      }

      std::tuple<ComponentHandle<Components> & ...> handles;
    };

    class Iterator : public ViewIterator<Iterator> {
    public:
      Iterator(EntityManager *manager, const ComponentMask mask, uint32_t index, const Unpacker &unpacker) : ViewIterator<Iterator>(manager, mask, index), unpacker_(unpacker) {
        ViewIterator<Iterator>::next();
      }

      void next_entity(Entity &entity) {
        unpacker_.unpack(entity);
      }

    private:
      const Unpacker &unpacker_;
    };


    Iterator begin() { return Iterator(manager_, mask_, 0, unpacker_); }
    Iterator end() { return Iterator(manager_, mask_, static_cast<uint32_t>(manager_->capacity()), unpacker_); }
    const Iterator begin() const { return Iterator(manager_, mask_, 0, unpacker_); }
    const Iterator end() const { return Iterator(manager_, mask_, static_cast<uint32_t>(manager_->capacity()), unpacker_); }


   private:
    friend class EntityManager;

    UnpackingView(EntityManager *manager, ComponentMask mask, ComponentHandle<Components> & ... handles) :
        manager_(manager), mask_(mask), unpacker_(handles...) {}

    EntityManager *manager_;
    ComponentMask mask_;
    Unpacker unpacker_;
  };

  --------------------------------------------------------------------------------------------

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