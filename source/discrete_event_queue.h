#pragma once

#include <algorithm>
#include <iterator>    // std::input_iterator_tag
#include <memory>      // std::uses_allocator
#include <queue>
#include <type_traits>
#include <utility>
#include <vector>


namespace DEQDetail
{
    // from GCC vector.h which is under GPL3
    template<class T, class Alloc, class Predicate>
    constexpr auto erase_if(std::vector<T, Alloc>& c, Predicate p) -> typename std::vector<T, Alloc>::size_type
    {
        const auto old_size = c.size();
        c.erase(std::remove_if(c.begin(), c.end(), p), c.end());
        return old_size - c.size();
    }
}


// A container adapter that sorts elements by a firing time in discrete ticks, maintains an internal timer in ticks, and
// reports and removes an event when the timer has reached its firing time.

// Based on the design of std::priority_queue from GCC, which is under GPL3.

// Container must support the same operations as required for a container by std::priority_queue. See
// https://en.cppreference.com/w/cpp/container/priority_queue for an in-depth explanation of these requirements. In
// short, the following member functions are expected: begin, end, empty, size, push_back, emplace_back, erase, clear.

template<class Type,
         class Projection,
         class Container = std::vector<Type>>
class discrete_event_queue
{
    using container_it = typename Container::iterator;

    template<class Alloc>
	using Uses = typename std::enable_if_t<std::uses_allocator_v<Container, Alloc>>;

    template<class B1, class B2>
    using And = typename std::conditional_t<B1::value, B2, B1>;

    static_assert(std::is_same_v<Type, typename Container::value_type>,
                  "Type must be the same as the value type of the underlying container.");

    // static_assert(std::is_same_v<unsigned, typename std::invoke_result_t<Projection, Type>>,
    //               "Return value of the projection of Type must be an unsigned integer.");

public:
    // Member types ----------------------------------------------------------------------------------------------------
    using value_type     = typename Container::value_type;
    using reference      = typename Container::reference;
    using size_type      = typename Container::size_type;
    using container_type = Container;

    // Constructors ----------------------------------------------------------------------------------------------------
    template<class Cont = Container, class Requires = typename std::enable_if_t<
             And<std::is_default_constructible<Projection>, std::is_default_constructible<Cont>>::value>>
    discrete_event_queue()
        : discrete_event_queue{Projection{}, Container{}}
    {}

    explicit
    discrete_event_queue(const Projection& proj, const Container&& cont = Container{})
        : unfired_events{std::move(cont)}, get_fire_time{proj}
    {
        std::make_heap(this->unfired_events.begin(), this->unfired_events.end(), event_greater{});
        cache_next_fire_time();
    }

    explicit
    discrete_event_queue(const Projection& proj, const Container& cont)
        : discrete_event_queue{proj, Container{cont}}
    {}

    discrete_event_queue(const discrete_event_queue&) = default;
    discrete_event_queue(discrete_event_queue&&) = default;

    template<class Alloc, class Requires = Uses<Alloc>>
    explicit
    discrete_event_queue(const Alloc& alloc)
        : discrete_event_queue{Projection{}, Container{alloc}}
    {}

    template<class Alloc, class Requires = Uses<Alloc>>
    discrete_event_queue(const Projection& proj, const Alloc& alloc)
        : discrete_event_queue{proj, Container{alloc}}
    {}

    template<class Alloc, class Requires = Uses<Alloc>>
    discrete_event_queue(const Projection& proj, const Container& cont, const Alloc& alloc)
        : discrete_event_queue{proj, Container{cont, alloc}}
    {}

    template<class Alloc, class Requires = Uses<Alloc>>
    discrete_event_queue(const Projection& proj, Container&& cont, const Alloc& alloc)
        : discrete_event_queue{proj, Container{std::move(cont), alloc}}
    {}

    template<class Alloc, class Requires = Uses<Alloc>>
    discrete_event_queue(const discrete_event_queue& other, const Alloc& alloc)
        : discrete_event_queue{other.get_fire_time, other.unfired_events, alloc}
    {}

    template<class Alloc, class Requires = Uses<Alloc>>
    discrete_event_queue(discrete_event_queue&& other, const Alloc& alloc)
        : discrete_event_queue{std::move(other.get_fire_time), std::move(other.unfired_events), alloc}
    {}

    template<class InputIt>
    discrete_event_queue(InputIt first, InputIt last,
                         const Projection& proj, const Container& cont)
        : discrete_event_queue{first, last, proj, Container{cont}}
    {}

    template<class InputIt>
    discrete_event_queue(InputIt first, InputIt last,
                         const Projection& proj = Projection{}, Container&& cont = Container{})
        : discrete_event_queue{proj,
                               [&]() { Container c{std::move(cont)}; c.insert(c.end(), first, last); return c; }()}
    {}

    discrete_event_queue& operator=(const discrete_event_queue&) = default;
    discrete_event_queue& operator=(discrete_event_queue&&) = default;

    // Capacity --------------------------------------------------------------------------------------------------------
    bool empty() const
    {
        return unfired_events.empty();
    }

    size_type size() const
    {
        return unfired_events.size();
    }

    bool events_were_fired() const
    {
        return !fired_events.empty();
    }

    // Element Access --------------------------------------------------------------------------------------------------
    unsigned ticks() const
    {
        return tick;
    }

    unsigned get_next_fire_time() const
    {
        return next_fire_time;
    }

    Container get_fired_events() const
    {
        return fired_events;
    }

    Container get_events() const
    {
        Container out;
        Container copy = unfired_events;

        for (auto last = copy.end(); last != copy.begin(); --last)
        {
            out.push_back(copy.front());
            std::pop_heap(copy.begin(), last, event_greater{});
        }

        return out;
    }

    // Modifiers -------------------------------------------------------------------------------------------------------
    void advance()
    {
        ++tick;
        fire_ready_events();
    }

    void advance(unsigned count)
    {
        tick += count;
        fire_ready_events();
    }

    void advance_to_next_event()
    {
        tick = next_fire_time;
        fire_ready_events();
    }

    void reset_ticks()
    {
        tick = -1;
    }

    void push(const value_type& v)
    {
        push(value_type{v});
    }

    void push(value_type&& v)
    {
        unfired_events.push_back(std::move(v));
        std::push_heap(unfired_events.begin(), unfired_events.end(), event_greater{});

        cache_next_fire_time();
    }

    template<class... Args>
    void emplace(Args&&... args)
    {
        unfired_events.emplace_back(std::forward<Args>(args)...);
        std::push_heap(unfired_events.begin(), unfired_events.end(), event_greater{});

        cache_next_fire_time();
    }

    size_type erase(const value_type& t)
    {
        return erase_if([=](const value_type& v) { return v == t; });
    }

    template<class UnaryPredicate>
    size_type erase_if(UnaryPredicate p)
    {
        auto count = DEQDetail::erase_if(unfired_events, p);

        if (count > 0) {
            std::make_heap(unfired_events.begin(), unfired_events.end(), event_greater{});
            cache_next_fire_time();
        }

        return count;
    }

    void clear()
    {
        unfired_events.clear();
        cache_next_fire_time();
    }

    void reset()
    {
        unfired_events.clear();
        fired_events.clear();
        tick = -1;
        next_fire_time = -1;
    }

    void swap(discrete_event_queue& other)
        noexcept(And<std::is_nothrow_swappable<Projection>, std::is_nothrow_swappable<Container>>::value)
    {
        using std::swap;
        swap(unfired_events, other.unfired_events);
        swap(get_fire_time,  other.get_fire_time);
        swap(tick,           other.tick);
        swap(fired_events,   other.fired_events);
        swap(next_fire_time, other.next_fire_time);
    }

private:
    Container  unfired_events;
    Projection get_fire_time;
    unsigned   tick = -1; // -1 allows users to start the clock at 0 with the first call to advance

    Container fired_events;
    unsigned  next_fire_time = -1; // should begin at largest number so nothing gets fired


    class event_greater
    {
        Projection projection{}; // (default construction)

    public:
        bool operator()(const Type& lhs, const Type& rhs) const
        {
            // Should return true if lhs should be ordered behind rhs.
            // We want the smallest fire time to be first, therefore we use greater than. On a tie the order of
            // insertion should be maintained, so we do not want to compare equality.
            return std::invoke(projection, lhs) > std::invoke(projection, rhs);
        }
    };

    unsigned lead_fire_time() const
    {
        return std::invoke(get_fire_time, unfired_events.front());
    }

    void cache_next_fire_time()
    {
        next_fire_time = unfired_events.empty() ? -1 : lead_fire_time();
    }

    bool fire_next_ready_event()
    {
        if (unfired_events.empty() || tick < lead_fire_time())    return false;

        fired_events.push_back(unfired_events.front());

        std::pop_heap(unfired_events.begin(), unfired_events.end(), event_greater{});
        unfired_events.pop_back();

        return true;
    }

    void fire_ready_events()
    {
        fired_events.clear();

        // This additional check is an optimization, since checking a cached value is potentially faster than projecting
        // on every tick (at the least, caching will not slow it down).
        if (tick < next_fire_time)    return;

        while (fire_next_ready_event());

        cache_next_fire_time();
    }
};


namespace DEQDetail
{
    template<class B1, class B2>
    using And = typename std::conditional_t<B1::value, B2, B1>;


    // from GCC stl_iterator_base_types.h which is under GPL3

    template<class I>
    using iterator_category_t = typename std::iterator_traits<I>::iterator_category;

    template<class I>
    using RequireInputIter =
      std::enable_if_t<std::is_convertible_v<iterator_category_t<I>, std::input_iterator_tag>>;


    // from GCC alloc_traits.h which is under GPL3

    template<class Alloc, class = void>
    struct is_allocator : std::false_type
    {};

    template<class Alloc>
    struct is_allocator<Alloc, std::void_t<typename Alloc::value_type,
	                                       decltype(std::declval<Alloc&>().allocate(std::size_t{}))>>
        : std::true_type
    {};

    template<class Alloc>
    using RequireAllocator = typename std::enable_if_t<is_allocator<Alloc>::value, Alloc>;

    template<class Alloc>
    using RequireNotAllocator = typename std::enable_if_t<!is_allocator<Alloc>::value, Alloc>;
}


template<class Projection, class Container,
    typename = DEQDetail::RequireNotAllocator<Projection>,
	typename = DEQDetail::RequireNotAllocator<Container>>
discrete_event_queue(Projection, Container)
-> discrete_event_queue<typename Container::value_type, Projection, Container>;

template<
    class InputIterator,
    class Projection,
    class T = typename std::iterator_traits<InputIterator>::value_type,
    class Container = std::vector<T>,
    typename = DEQDetail::RequireInputIter<InputIterator>,
    typename = DEQDetail::RequireNotAllocator<Projection>,
    typename = DEQDetail::RequireNotAllocator<Container>>
discrete_event_queue(InputIterator, InputIterator, Projection = Projection{}, Container = Container{})
-> discrete_event_queue<T, Projection, Container>;

template<class Projection, class Container, class Allocator,
    typename = DEQDetail::RequireNotAllocator<Projection>,
    typename = DEQDetail::RequireNotAllocator<Container>,
    typename = DEQDetail::RequireAllocator<Allocator>>
discrete_event_queue(Projection, Container, Allocator)
-> discrete_event_queue<typename Container::value_type, Projection, Container>;


template<class T, class Projection, class Container>
inline typename std::enable_if_t<DEQDetail::And<std::is_swappable<Projection>, std::is_swappable<Container>>::value>
swap(discrete_event_queue<T, Projection, Container>& x,
	 discrete_event_queue<T, Projection, Container>& y)
    noexcept(noexcept(x.swap(y)))
{
    x.swap(y);
}

template<class T, class Projection, class Container, class Alloc>
struct std::uses_allocator<discrete_event_queue<T, Projection, Container>, Alloc>
    : public std::uses_allocator<Container, Alloc>::type
{};
