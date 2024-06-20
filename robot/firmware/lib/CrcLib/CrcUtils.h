#ifndef CrcUtils_h
#define CrcUtils_h

namespace CrcUtility {
template <class T> struct PatternItem {
    int dur;
    T val;

    static constexpr PatternItem<T> END = { -1 };
};
template <class T> PatternItem<T> constexpr PatternItem<T>::END;

template <class T> class Pattern {
public:
    Pattern<T>()
        : _timeLeft(0)
    {
    }

    virtual PatternItem<T> Next() = 0;

    bool Update(unsigned int delta, void (*cb)(T item, void* arg), void* arg)
    {
        _timeLeft -= delta;
        if (_timeLeft > 0)
            return false;

        auto next = Next();
        cb(next.val, arg);
        if (next.dur < 0) { // Sentinel
            return true;
        }
        _timeLeft += (long)next.dur * 1000L;
        return false;
    }

private:
    long _timeLeft;
};

template <class T> class SimplePattern : public Pattern<T> {
    using Pattern<T>::Pattern;

public:
    SimplePattern(const PatternItem<T>* items, bool repeat)
        : _items(items)
        , _current(0)
        , _repeat(repeat)
    {
    }
    PatternItem<T> Next() override
    {
        auto item = _items[_current];
        if (item.dur >= 0) { // Sentinel
            _current++;
        } else if (_repeat) {
            _current = 0;
            return Next();
        }
        return item;
    }

private:
    const PatternItem<T>* _items;
    unsigned short _current;
    bool _repeat;
};
}
#endif
