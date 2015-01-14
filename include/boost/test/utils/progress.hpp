//  boost progress.hpp header file  ------------------------------------------//

//  Copyright Beman Dawes 1994-99.  
  
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

// Derived from originial implementation in Boost.Timer (now deprecated)

#ifndef BOOST_TEST_UTILS_PROGRESS_DISPLAY_HPP
#define BOOST_TEST_UTILS_PROGRESS_DISPLAY_HPP

#include <boost/noncopyable.hpp>
#include <iostream>           // for ostream, cout, etc
#include <string>             // for string

namespace boost {
namespace unit_test {
namespace timer {

class progress_display : private noncopyable {
public:
    explicit        progress_display( unsigned long expected_count, std::ostream& os = std::cout )
    : m_os(os)
    {
        restart( expected_count );
    }

    ///  Effects: display appropriate scale
    ///  Postconditions: count()==0, expected_count()==expected_count
    void            restart( unsigned long expected_count )
    {
        m_count = m_next_tic_count = m_tic = 0;
        m_expected_count = expected_count;

        m_os << "\n0%   10   20   30   40   50   60   70   80   90   100%"
                "\n|----|----|----|----|----|----|----|----|----|----|"
             << std::endl;  // endl implies flush, which ensures display

        if( !m_expected_count ) 
            m_expected_count = 1; // prevent divide by zero
    } // restart

    ///  Effects: Display appropriate progress tic if needed.
    ///  Postconditions: count()== original count() + increment
    ///  Returns: count().
    unsigned long   operator+=( unsigned long increment )
    {
        m_count += increment;
        if( m_count >= m_next_tic_count )
            display_tic();

        return m_count;
    }

    unsigned long   operator++()            { return operator+=( 1 ); }
    unsigned long   count() const           { return m_count; }
    unsigned long   expected_count() const  { return m_expected_count; }

private:
    void            display_tic()
    {
        // use of floating point ensures that both large and small counts
        // work correctly.  static_cast<>() is also used several places
        // to suppress spurious compiler warnings.
        unsigned int tics_needed = static_cast<unsigned int>( (static_cast<double>(m_count)/m_expected_count)*50.0 );
        do { 
            m_os << '*' << std::flush; 
        } while ( ++m_tic < tics_needed );
        
        m_next_tic_count = static_cast<unsigned long>((m_tic/50.0)*m_expected_count);
        
        if( m_count == m_expected_count ) {
            if( m_tic < 51 )
                m_os << '*';
            m_os << std::endl;
        }
    } // display_tic

    // Data members
    std::ostream&   m_os;
    unsigned long   m_count;
    unsigned long   m_expected_count;
    unsigned long   m_next_tic_count;
    unsigned int    m_tic;
};

} // namespace timer
} // namespace unit_test
} // namespace boost

#endif  // BOOST_TEST_UTILS_PROGRESS_DISPLAY_HPP
