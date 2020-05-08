#ifndef __TEMPLATE_H__
#define __TEMPLATE_H__

#include <string>
using namespace std;
namespace regular_expressions
{
    template<typename T>
    class default_instance
    {
    public:
        static T& Instance()
        {
            if ( m_instance == NULL )
                m_instance = new T;
            return *m_instance;
        }
    private:
        static T* m_instance;
    };

    template<typename T> T* default_instance<T>::m_instance = NULL;
    class match_true
    {
    public:
        template<typename T>
        bool IsMatch(T& val, T val2)
        {
            return true;
        }

        int Advance()
        {
            return 0;
        }
    };

    class match_false
    {
    public:
        template<typename T>
        bool IsMatch(T& val, T val2)
        {
            return false;
        }

        int Advance()
        {
            return 0;
        }
    };

    template<typename T>
    int ChacatersNeedAdvance(T& composableClass)
    {
        return composableClass.Advance();
    }

    template<typename T>
    int ChacatersNeedAdvance(T* composableClass)
    {
        return composableClass->Advance();
    }

    template<>
    int ChacatersNeedAdvance<char>(char& composableClass)
    {
        return 1;
    }

    template<>
    int ChacatersNeedAdvance<char>(char* composableClass)
    {
        return 1;
    }

    template<typename It, typename M>
    bool DoMatch(It& iter, It& end, M& matcher)
    {
        return matcher.IsMatch(iter, end);
    }

    template<>
    bool DoMatch<string::iterator, char>(string::iterator& iter, string::iterator& end, char& matcher)
    {
        if ( iter == end )
            return false;
        bool ret = matcher == *iter;
        if ( ret )
        {
            iter++;
        }
        return ret;
    }

    template<>
    bool DoMatch<string::const_iterator, char>(string::const_iterator& iter, string::const_iterator& end, char& matcher)
    {
        if ( iter == end )
            return false;
        bool ret = matcher == *iter;
        if ( ret )
        {
            iter++;
        }
        return ret;
    }

    template<>
    bool DoMatch<char, char>(char& iter, char& end, char& matcher)
    {
        if ( iter == end )
            return false;
        bool ret = matcher == iter;
        if ( ret )
        {
            iter++;
        }
        return ret;
    }

    template< >
    bool DoMatch<string::iterator, string>(string::iterator& iter, string::iterator& end, string& matcher)
    {
        bool ret = true;
        string::iterator& savedIter = iter;
        string::const_iterator miter = matcher.begin();
        for (; ((miter != matcher.end()) && (savedIter != end)); ++miter, ++savedIter)
        {
            ret = (*miter) == (*savedIter);
        }
        if ( (savedIter == end) && (miter != matcher.end()) )
            ret = false;
        if ( ret )
        {
            while ( iter != savedIter )
                iter++;
        }
        return ret;
    }

    template< >
    bool DoMatch<string::const_iterator, string>(string::const_iterator& iter, string::const_iterator& end, string& matcher)
    {
        bool ret = true;
        string::const_iterator& savedIter = iter;
        string::const_iterator miter = matcher.begin();
        for (; ((miter != matcher.end()) && (savedIter != end)); ++miter, ++savedIter)
        {
            ret = (*miter) == (*savedIter);
        }
        if ( (savedIter == end) && (miter != matcher.end()) )
            ret = false;
        if ( ret )
        {
            while ( iter != savedIter )
                iter++;
        }
        return ret;
    }

    template<typename T1,
        typename T2 = T1,
        typename T3 = T1,
        typename T4 = T1>
    class any_characters
    {
    public:
        any_characters(T1 val1,
            T2 val2 = default_instance<T2>::Instance(),
            T3 val3 = default_instance<T3>::Instance(),
            T4 val4 = default_instance<T4>::Instance()) : m_val1(val1), m_val2(val2), m_val3(val3), m_val4(val4)
        {
        }

        any_characters(const any_characters& other)
            : m_val1(other.m_val1),
            m_val2(other.m_val2),
            m_val3(other.m_val3),
            m_val4(other.m_val4)
        {
        }
    public:
        template<typename It>
        bool IsMatch(It& iter, It end)
        {
            if ( iter == end )
                return false;
            return DoMatch(iter, end, m_val1) ||
                DoMatch(iter, end, m_val2) ||
                DoMatch(iter, end, m_val3) ||
                DoMatch(iter, end, m_val4);
        }

        int Advance()
        {
            int ret = 0;
            int count = 0;
            count = ChacatersNeedAdvance(m_val1);
            if ( count > ret ) ret = count;
            count = ChacatersNeedAdvance(m_val2);
            if ( count > ret ) ret = count;
            count = ChacatersNeedAdvance(m_val3);
            if ( count > ret ) ret = count;
            count = ChacatersNeedAdvance(m_val4);
            if ( count > ret ) ret = count;
            if ( ret == 0 )
                ret = 1;
            return ret;
        }

        template<typename It>
        void AdvanceIterator(It& iter, It end)
        {
            int count = ChacatersNeedAdvance(this);
            while ( --count >= 0 )
            {
                if ( iter == end )
                    break;
                iter++;
            }
        }

    private:
        T1 m_val1;
        T2 m_val2;
        T3 m_val3;
        T4 m_val4;
    };

    template<>
    class any_characters<char>
    {
    public:
        any_characters(char val1,
            char val2 = '\0',
            char val3 = '\0',
            char val4 = '\0') : m_val1(val1), m_val2(val2), m_val3(val3), m_val4(val4)
        {
        }

        any_characters(const any_characters& other)
            : m_val1(other.m_val1),
            m_val2(other.m_val2),
            m_val3(other.m_val3),
            m_val4(other.m_val4)
        {
        }
    public:
        template<typename It>
        bool IsMatch(It& iter, It end)
        {
            if ( iter == end )
                return false;
            return m_val1 == *iter ||
                m_val2 == *iter ||
                m_val3 == *iter ||
                m_val4 == *iter;
        }

        int Advance()
        {
            return 1;
        }

        template<typename It>
        void AdvanceIterator(It& iter, It end)
        {
            int count = ChacatersNeedAdvance(this);
            while ( --count >= 0 )
            {
                if ( iter == end )
                    break;
                iter++;
            }
        }
    private:
        char m_val1;
        char m_val2;
        char m_val3;
        char m_val4;
    };

    template<typename T1,
        typename T2 = match_true,
        typename T3 = match_true,
        typename T4 = match_true>
    class all_characters
    {
    public:
        all_characters(T1 val1,
            T2 val2 = default_instance<T2>::Instance(),
            T3 val3 = default_instance<T3>::Instance(),
            T4 val4 = default_instance<T4>::Instance()) : m_val1(val1), m_val2(val2), m_val3(val3), m_val4(val4)
        {
        }

        all_characters(const all_characters& other)
            : m_val1(other.m_val1),
            m_val2(other.m_val2),
            m_val3(other.m_val3),
            m_val4(other.m_val4)
        {
        }
    public:
        template<typename It>
        bool IsMatch(It& iter, It end)
        {
            if ( iter == end )
                return false;
            bool ret = true;
            It savedIter = iter;
            ret = ret && DoMatch(savedIter, end, m_val1);
            ret = ret && DoMatch(savedIter, end, m_val2);
            ret = ret && DoMatch(savedIter, end, m_val3);
            ret = ret && DoMatch(savedIter, end, m_val4);
            if ( ret )
            {
                while ( iter != savedIter )
                    ++iter;
            }
            return ret;
        }

        int Advance()
        {
            int ret = 0;
            int count = 0;
            count = ChacatersNeedAdvance(m_val1);
            ret += count;
            count = ChacatersNeedAdvance(m_val2);
            ret += count;
            count = ChacatersNeedAdvance(m_val3);
            ret += count;
            count = ChacatersNeedAdvance(m_val4);
            ret += count;
            return ret;
        }

        template<typename It>
        void AdvanceIterator(It& iter, It end)
        {
            PrivateAdvanceIteratorHelper(iter, end, *this);
        }

    private:
        template<typename It, typename M>
        void PrivateAdvanceIteratorHelper(It& iter, It end, M& matcher)
        {
            int count = ChacatersNeedAdvance(matcher);
            while ( --count >= 0 )
            {
                if ( iter == end )
                    break;
                iter++;
            }
        }

    private:
        T1 m_val1;
        T2 m_val2;
        T3 m_val3;
        T4 m_val4;
    };

    template<typename T>
    class OpenRightEndRangeComparer
    {
    public:
        OpenRightEndRangeComparer(T lowend, T highend) : m_lowend(lowend), m_highend(highend)
        {
        }

    public:
        bool InRange(T val)
        {
            return (val - m_lowend > 0) && (m_highend - val >= 0);
        }

    private:
        T m_lowend;
        T m_highend;
    };

    template<typename T>
    class CloseEndsRangeComparer
    {
    public:
        CloseEndsRangeComparer(T lowend, T highend) : m_lowend(lowend), m_highend(highend)
        {
        }

    public:
        bool InRange(T val)
        {
            return (val - m_lowend > 0) && (m_highend - val > 0);
        }

    private:
        T m_lowend;
        T m_highend;
    };

    template<typename T>
    class OpenLeftEndRangeComparer
    {
    public:
        OpenLeftEndRangeComparer(T lowend, T highend) : m_lowend(lowend), m_highend(highend)
        {
        }

    public:
        bool InRange(T val)
        {
            return (val - m_lowend >= 0) && (m_highend - val > 0);
        }

    private:
        T m_lowend;
        T m_highend;
    };

    template<typename T>
    class OpenEndsRangeComparer
    {
    public:
        OpenEndsRangeComparer(T lowend, T highend) : m_lowend(lowend), m_highend(highend)
        {
        }

    public:
        bool InRange(T val)
        {
            return (val - m_lowend >= 0) && (m_highend - val >= 0);
        }

    private:
        T m_lowend;
        T m_highend;
    };

    template<typename T, typename C = OpenEndsRangeComparer<T> >
    class range
    {
    public:
        range(T lowend, T highend) :
            m_lowend(lowend),
            m_highend(highend),
            m_comparer(lowend, highend) 
        {
        }
        
        range(const range& other) : m_lowend(other.m_lowend),
            m_highend(other.m_highend),
            m_comparer(m_lowend, m_highend)
        {
        }

    public:
        template<typename It>
        bool IsMatch(It& iter, It end)
        {
            if ( iter == end )
                return false;
            else
                return m_comparer.InRange(*iter);
        }

        int Advance()
        {
            return 0;
        }

    private:
        T m_lowend;
        T m_highend;
        C m_comparer;
    };

    // A*
    template<typename M>
    class aterisk_quantifier
    {
    public:
        aterisk_quantifier(M matcher) : m_matcher(matcher)
        {
        }

        aterisk_quantifier(const aterisk_quantifier& other) : m_matcher(other.m_matcher)
        {
        }
    public:
        template<typename It>
        bool IsMatch(It& iter, It end)
        {
            It& savedIter = iter;
            while ( m_matcher.IsMatch(savedIter, end) ) 
                ;
            
            return true;
        }

        int Advance()
        {
            return 0;
        }

        template<typename It>
        void AdvanceIterator(It& iter, It end)
        {
        }
    private:
        M m_matcher;
    };

    // A+
    template<typename M>
    class plus_quantifier
    {
    public:
        plus_quantifier(M matcher) : m_matcher(matcher)
        {
        }

        plus_quantifier(const plus_quantifier& other) : m_matcher(other.m_matcher)
        {
        }

    public:
        template<typename It>
        bool IsMatch(It& iter, It end)
        {
            bool ret = true;
            bool atleastonmatches = false;
            do
            {
                It& savedIter = iter;
                ret = ret && m_matcher.IsMatch(savedIter, end);
                if ( ret )
                {
                    atleastonmatches = true;
                    int count = ChacatersNeedAdvance(m_matcher);
                    while ( --count >= 0 )
                        iter++;
                }
            } while ( ret );
            return atleastonmatches;
        }

        int Advance()
        {
            return 0;
        }

        template<typename It>
        void AdvanceIterator(It& iter, It end)
        {
        }

    private:
        M m_matcher;
    };

    // A?
    template<typename M>
    class question_mark_quantifier
    {
    public:
        question_mark_quantifier(M matcher) : m_matcher(matcher)
        {
        }

        question_mark_quantifier(const question_mark_quantifier& other) : m_matcher(other.m_matcher)
        {
        }
    public:
        template<typename It>
        bool IsMatch(It& iter, It end)
        {
            m_matcher.IsMatch(iter, end);
            return true;
        }

        int Advance()
        {
            return 0;
        }

        template<typename It>
        void AdvanceIterator(It& iter, It end)
        {
        }

    private:
        M m_matcher;
    };
}
#endif