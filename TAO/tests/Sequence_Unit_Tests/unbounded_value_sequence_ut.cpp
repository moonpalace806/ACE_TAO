/**
 * @file
 *
 * @brief Unit test for unbounded sequences of value types (integers,
 *        structures, etc.)
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "testing_allocation_traits.hpp"
#include "testing_range_checking.hpp"

#include "unbounded_value_sequence.hpp"

#include "value_sequence_tester.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace boost::unit_test_framework;
using namespace TAO;

typedef details::value_traits<int,true> tested_element_traits;
typedef details::unbounded_allocation_traits<int,true> tested_allocation_traits;
typedef details::range_checking<int,true> range;

typedef unbounded_value_sequence<int> tested_sequence;

struct Tester
  : public boost::enable_shared_from_this<Tester>
{
  typedef tested_sequence::value_type value_type;

  void add_all(test_suite * ts)
  {
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_ulong_constructor, shared_from_this()));

    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_copy_constructor_from_ulong,
                shared_from_this()));

    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_assignment_from_ulong,
                shared_from_this()));

    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_exception_in_ulong_constructor,
                shared_from_this()));

    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_set_length_less_than_maximum,
                shared_from_this()));

    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_set_length_more_than_maximum,
                shared_from_this()));

    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_exception_in_set_length,
                shared_from_this()));

    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_buffer_constructor_default,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_buffer_constructor_false,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_buffer_constructor_true,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_replace_default,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_replace_false,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_replace_true,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_get_buffer_const,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_get_buffer_false,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_get_buffer_true_with_release_false,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_get_buffer_true_with_release_true,
                shared_from_this()));

  }

  void test_copy_constructor_from_ulong()
  {
    long a = tested_allocation_traits::allocbuf_calls;
    long f = tested_allocation_traits::freebuf_calls;
    {
      tested_sequence x(16);
      BOOST_CHECK_EQUAL(++a, tested_allocation_traits::allocbuf_calls);
      x.length(8);

      BOOST_CHECK_EQUAL(CORBA::ULong(16), x.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(8), x.length());
      BOOST_CHECK_EQUAL(true, x.release());

      tested_sequence y(x);
      BOOST_CHECK_EQUAL(++a, tested_allocation_traits::allocbuf_calls);
      BOOST_CHECK_EQUAL(CORBA::ULong(16), y.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(8), y.length());
      BOOST_CHECK_EQUAL(true, y.release());
    }
    f += 2;
    BOOST_CHECK_EQUAL(  f, tested_allocation_traits::freebuf_calls);
  }

  void test_assignment_from_ulong()
  {
    long a = tested_allocation_traits::allocbuf_calls;
    long f = tested_allocation_traits::freebuf_calls;
    {
      tested_sequence x(16);
      x.length(8);
      BOOST_CHECK_EQUAL(++a, tested_allocation_traits::allocbuf_calls);
      BOOST_CHECK_EQUAL(CORBA::ULong(16), x.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(8), x.length());
      BOOST_CHECK_EQUAL(true, x.release());

      tested_sequence y;
      BOOST_CHECK_EQUAL(a, tested_allocation_traits::allocbuf_calls);

      y = x;
      BOOST_CHECK_EQUAL(++a, tested_allocation_traits::allocbuf_calls);
      BOOST_CHECK_EQUAL(++f, tested_allocation_traits::freebuf_calls);
      BOOST_CHECK_EQUAL(CORBA::ULong(16), y.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(8), y.length());
      BOOST_CHECK_EQUAL(true, y.release());
    }
    f += 2;
    BOOST_CHECK_EQUAL(  f, tested_allocation_traits::freebuf_calls);
  }

  void test_ulong_constructor()
  {
    long a = tested_allocation_traits::allocbuf_calls;
    long f = tested_allocation_traits::freebuf_calls;
    {
      tested_sequence x(16);

      BOOST_CHECK_EQUAL(CORBA::ULong(16), x.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(0), x.length());
      BOOST_CHECK_EQUAL(true, x.release());
    }
    BOOST_CHECK_EQUAL(++a, tested_allocation_traits::allocbuf_calls);
    BOOST_CHECK_EQUAL(++f, tested_allocation_traits::freebuf_calls);
  }

  void test_exception_in_ulong_constructor()
  {
    long a = tested_allocation_traits::allocbuf_calls;
    long f = tested_allocation_traits::freebuf_calls;
    {
      tested_allocation_traits::calls_until_failure_in_allocbuf = 1;
      BOOST_CHECK_THROW(tested_sequence x(16), testing_exception);
      BOOST_CHECK_EQUAL(++a, tested_allocation_traits::allocbuf_calls);
    }
    BOOST_CHECK_EQUAL(  f, tested_allocation_traits::freebuf_calls);
  }

  void test_set_length_less_than_maximum()
  {
    long a = tested_allocation_traits::allocbuf_calls;
    long f = tested_allocation_traits::freebuf_calls;
    {
      tested_sequence x(16);

      x.length(8);
      BOOST_CHECK_EQUAL(CORBA::ULong(16), x.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(8), x.length());
      BOOST_CHECK_EQUAL(true, x.release());
    }
    BOOST_CHECK_EQUAL(++a, tested_allocation_traits::allocbuf_calls);
    BOOST_CHECK_EQUAL(++f, tested_allocation_traits::freebuf_calls);
  }

  void test_set_length_more_than_maximum()
  {
    long a = tested_allocation_traits::allocbuf_calls;
    long f = tested_allocation_traits::freebuf_calls;
    {
      tested_sequence x(16);
      BOOST_CHECK_EQUAL(++a, tested_allocation_traits::allocbuf_calls);

      x.length(32);
      BOOST_CHECK_EQUAL(++a, tested_allocation_traits::allocbuf_calls);
      BOOST_CHECK_EQUAL(++f, tested_allocation_traits::freebuf_calls);

      BOOST_CHECK_EQUAL(CORBA::ULong(32), x.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(32), x.length());
      BOOST_CHECK_EQUAL(true, x.release());
    }
    BOOST_CHECK_EQUAL(++f, tested_allocation_traits::freebuf_calls);
  }

  void test_exception_in_set_length()
  {
    long f = tested_allocation_traits::freebuf_calls;
    {
      tested_sequence x;

      long a = tested_allocation_traits::allocbuf_calls;
      tested_allocation_traits::calls_until_failure_in_allocbuf = 1;
      BOOST_CHECK_THROW(x.length(8), testing_exception);
      BOOST_CHECK_EQUAL(++a, tested_allocation_traits::allocbuf_calls);
    }
    BOOST_CHECK_EQUAL(++f, tested_allocation_traits::freebuf_calls);
  }

  value_type * alloc_and_init_buffer()
  {
    value_type * buf = tested_sequence::allocbuf(8);
    buf[0] = 1; buf[1] = 4; buf[2] = 9; buf[3] = 16;

    return buf;
  }

  void test_buffer_constructor_default()
  {
    value_type * buffer = alloc_and_init_buffer();

    long a = tested_allocation_traits::allocbuf_calls;
    long f = tested_allocation_traits::freebuf_calls;
    {
      tested_sequence a(8, 4, buffer);
      BOOST_CHECK_EQUAL(CORBA::ULong(8), a.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(4), a.length());
      BOOST_CHECK_EQUAL(buffer, a.get_buffer());
      BOOST_CHECK_EQUAL(int( 1), a[0]);
      BOOST_CHECK_EQUAL(int( 4), a[1]);
      BOOST_CHECK_EQUAL(int( 9), a[2]);
      BOOST_CHECK_EQUAL(int(16), a[3]);
      BOOST_CHECK_EQUAL(false, a.release());
    }
    BOOST_CHECK_EQUAL(a, tested_allocation_traits::allocbuf_calls);
    BOOST_CHECK_EQUAL(f, tested_allocation_traits::freebuf_calls);
    tested_sequence::freebuf(buffer);
  }

  void test_buffer_constructor_false()
  {
    value_type * buffer = alloc_and_init_buffer();
    long a = tested_allocation_traits::allocbuf_calls;
    long f = tested_allocation_traits::freebuf_calls;
    {
      tested_sequence a(8, 4, buffer, false);
      BOOST_CHECK_EQUAL(CORBA::ULong(8), a.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(4), a.length());
      BOOST_CHECK_EQUAL(buffer, a.get_buffer());
      BOOST_CHECK_EQUAL(int( 1), a[0]);
      BOOST_CHECK_EQUAL(int( 4), a[1]);
      BOOST_CHECK_EQUAL(int( 9), a[2]);
      BOOST_CHECK_EQUAL(int(16), a[3]);
      BOOST_CHECK_EQUAL(false, a.release());
    }
    BOOST_CHECK_EQUAL(  a, tested_allocation_traits::allocbuf_calls);
    BOOST_CHECK_EQUAL(  f, tested_allocation_traits::freebuf_calls);
    tested_sequence::freebuf(buffer);
  }

  void test_buffer_constructor_true()
  {
    value_type * buffer = alloc_and_init_buffer();
    long a = tested_allocation_traits::allocbuf_calls;
    long f = tested_allocation_traits::freebuf_calls;
    {
      tested_sequence a(8, 4, buffer, true);
      BOOST_CHECK_EQUAL(CORBA::ULong(8), a.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(4), a.length());
      BOOST_CHECK_EQUAL(buffer, a.get_buffer());
      BOOST_CHECK_EQUAL(int( 1), a[0]);
      BOOST_CHECK_EQUAL(int( 4), a[1]);
      BOOST_CHECK_EQUAL(int( 9), a[2]);
      BOOST_CHECK_EQUAL(int(16), a[3]);
      BOOST_CHECK_EQUAL(true, a.release());
    }
    BOOST_CHECK_EQUAL(  a, tested_allocation_traits::allocbuf_calls);
    BOOST_CHECK_EQUAL(++f, tested_allocation_traits::freebuf_calls);
  }

  void test_replace_default()
  {
    value_type * buffer = alloc_and_init_buffer();

    long c = tested_allocation_traits::allocbuf_calls;
    long f = tested_allocation_traits::freebuf_calls;
    {
      tested_sequence a;
      a.replace(8, 4, buffer);
      BOOST_CHECK_EQUAL(  c, tested_allocation_traits::allocbuf_calls);
      BOOST_CHECK_EQUAL(++f, tested_allocation_traits::freebuf_calls);

      BOOST_CHECK_EQUAL(CORBA::ULong(8), a.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(4), a.length());
      BOOST_CHECK_EQUAL(buffer, a.get_buffer());
      BOOST_CHECK_EQUAL(int( 1), a[0]);
      BOOST_CHECK_EQUAL(int( 4), a[1]);
      BOOST_CHECK_EQUAL(int( 9), a[2]);
      BOOST_CHECK_EQUAL(int(16), a[3]);
      BOOST_CHECK_EQUAL(false, a.release());
    }
    BOOST_CHECK_EQUAL(  c, tested_allocation_traits::allocbuf_calls);
    BOOST_CHECK_EQUAL(  f, tested_allocation_traits::freebuf_calls);
    tested_sequence::freebuf(buffer);
  }

  void test_replace_false()
  {
    value_type * buffer = alloc_and_init_buffer();
    long c = tested_allocation_traits::allocbuf_calls;
    long f = tested_allocation_traits::freebuf_calls;

    {
      tested_sequence a;
      a.replace(8, 4, buffer, false);
      BOOST_CHECK_EQUAL(  c, tested_allocation_traits::allocbuf_calls);
      BOOST_CHECK_EQUAL(++f, tested_allocation_traits::freebuf_calls);

      BOOST_CHECK_EQUAL(CORBA::ULong(8), a.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(4), a.length());
      BOOST_CHECK_EQUAL(buffer, a.get_buffer());
      BOOST_CHECK_EQUAL(int( 1), a[0]);
      BOOST_CHECK_EQUAL(int( 4), a[1]);
      BOOST_CHECK_EQUAL(int( 9), a[2]);
      BOOST_CHECK_EQUAL(int(16), a[3]);
      BOOST_CHECK_EQUAL(false, a.release());
    }
    BOOST_CHECK_EQUAL(  c, tested_allocation_traits::allocbuf_calls);
    BOOST_CHECK_EQUAL(  f, tested_allocation_traits::freebuf_calls);
    tested_sequence::freebuf(buffer);
  }

  void test_replace_true()
  {
    value_type * buffer = alloc_and_init_buffer();
    long c = tested_allocation_traits::allocbuf_calls;
    long f = tested_allocation_traits::freebuf_calls;

    {
      tested_sequence a;
      a.replace(8, 4, buffer, true);
      BOOST_CHECK_EQUAL(  c, tested_allocation_traits::allocbuf_calls);
      BOOST_CHECK_EQUAL(++f, tested_allocation_traits::freebuf_calls);

      BOOST_CHECK_EQUAL(CORBA::ULong(8), a.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(4), a.length());
      BOOST_CHECK_EQUAL(buffer, a.get_buffer());
      BOOST_CHECK_EQUAL(int( 1), a[0]);
      BOOST_CHECK_EQUAL(int( 4), a[1]);
      BOOST_CHECK_EQUAL(int( 9), a[2]);
      BOOST_CHECK_EQUAL(int(16), a[3]);
      BOOST_CHECK_EQUAL(true, a.release());
    }
    BOOST_CHECK_EQUAL(  c, tested_allocation_traits::allocbuf_calls);
    BOOST_CHECK_EQUAL(++f, tested_allocation_traits::freebuf_calls);
  }

  void test_get_buffer_const()
  {
    value_type * buffer = alloc_and_init_buffer();
    tested_sequence a(8, 4, buffer, true);
    tested_sequence const & b = a;
    BOOST_CHECK_EQUAL(b.get_buffer(), buffer);
  }

  void test_get_buffer_default()
  {
    value_type * buffer = alloc_and_init_buffer();
    tested_sequence a(8, 4, buffer, true);
    BOOST_CHECK_EQUAL(a.get_buffer(), buffer);
  }

  void test_get_buffer_false()
  {
    value_type * buffer = alloc_and_init_buffer();
    tested_sequence a(8, 4, buffer, true);
    BOOST_CHECK_EQUAL(a.get_buffer(), buffer);
  }

  void test_get_buffer_true_with_release_false()
  {
    value_type * buffer = alloc_and_init_buffer();
    tested_sequence a(8, 4, buffer, false);
    BOOST_CHECK_EQUAL(static_cast<int*>(0), a.get_buffer(true));
    tested_sequence::freebuf(buffer);
  }

  void test_get_buffer_true_with_release_true()
  {
    value_type * buffer = alloc_and_init_buffer();
    long c = tested_allocation_traits::allocbuf_calls;
    long f = tested_allocation_traits::freebuf_calls;
    {
      tested_sequence a(8, 4, buffer, true);
      BOOST_CHECK_EQUAL(buffer, a.get_buffer(true));

      tested_sequence const & b = a;
      BOOST_CHECK_EQUAL(0UL, b.maximum());
      BOOST_CHECK_EQUAL(0UL, b.length());
      BOOST_CHECK(0 != b.get_buffer());

      BOOST_CHECK_EQUAL(++c, tested_allocation_traits::allocbuf_calls);

      BOOST_CHECK(buffer != b.get_buffer());
    }
    BOOST_CHECK_EQUAL(  c, tested_allocation_traits::allocbuf_calls);
    BOOST_CHECK_EQUAL(++f, tested_allocation_traits::freebuf_calls);
    tested_sequence::freebuf(buffer);
  }

};

test_suite *
init_unit_test_suite(int, char*[])
{
  std::auto_ptr<test_suite> ts(
      BOOST_TEST_SUITE("unbounded value sequence unit test"));

  {
    boost::shared_ptr<Tester> tester(new Tester);
    tester->add_all(ts.get());
  }

  {
    typedef value_sequence_tester<tested_sequence,tested_allocation_traits> common;
    boost::shared_ptr<common> tester(new common);
    tester->add_all(ts.get());
  }

  return ts.release();
}

#if 0
// This is just to convince MPC that I do not need a main() to have a
// program.
int main() {}
#endif
