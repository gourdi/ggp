#ifndef __GGO_BUFFER__
#define __GGO_BUFFER__

namespace ggo
{
  template <typename data_t>
  class buffer final
  {
  public:

    buffer(size_t size)
    {
      _buffer = new data_t[size];
      _end = _buffer + size;
    }

    buffer(buffer && b)
    {
      _buffer = b._buffer;
      _end = b._end;

      b._buffer = nullptr;
    }

    ~buffer()
    {
      delete[] _buffer;
    }

    // Remove copy a move.
    buffer(const buffer & b) = delete;

    void operator=(const buffer & b) = delete;
    void operator=(buffer && b) = delete;

    // Raw buffer access.
    data_t *        data()        { return _buffer; }
    const data_t *  data() const  { return _buffer; }

    // Ran
    const data_t &  operator[](int i) const { return _buffer[i]; }
    data_t &        operator[](int i)       { return _buffer[i]; }

    // Iterators helpers.
    data_t *        begin()       { return _buffer; }
    const data_t *  begin() const { return _buffer; }
    data_t *        end()         { return _end; }
    const data_t *  end() const   { return _end; }

  private:

    data_t *  _buffer;
    data_t *  _end;
  };
}

#endif
