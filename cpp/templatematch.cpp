/*
  版权声明：本文为CSDN博主「知平软件」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
  原文链接：https://blog.csdn.net/Donjuan/article/details/3851649
*/
// 编译命令：g++ -o templatematch templatematch.cpp 
#include "template.hpp"
#include <iostream>
using namespace std;
using namespace regular_expressions;

typedef any_characters<range<char>, range<char>, range<char>, char> word_class;
void MatchEmail(const string &email)
{
    // regex: [A-za-z0-9_-]+@[A-za-z0-9_-]+.[[A-za-z0-9_-]+(.[A-za-z0-9_-]+)*
    range<char> upcases('A', 'Z');
    range<char> lowcases('a', 'z');
    range<char> digit('0', '9');
    any_characters<char, char> temp('_', '-');
    // [A-Za-z0-9_]
    word_class word(upcases, lowcases, digit, '_');
    // [A-Za-z0-9_-]+@[A-Za-z0-9_-]+.[A-Za-z0-9_-]+(.[A-Za-z0-9_-]+)*
    // [A-Za-z0-9_-]+
    plus_quantifier<word_class> wordplus(word);
    // .[A-Za-z0-9_-]+
    all_characters<char, plus_quantifier<word_class> > dotwordplus('.', wordplus);
    // (.[A-Za-z0-9_-]+)*
    aterisk_quantifier<all_characters<char, plus_quantifier<word_class> > > dotwordaterisk(dotwordplus);
    // (.[A-Za-z0-9_-]+)?
    question_mark_quantifier<all_characters<char, plus_quantifier<word_class> > > dotwordquestion(dotwordplus);
    all_characters<plus_quantifier<word_class>,
           all_characters<char, plus_quantifier<word_class> >,
           question_mark_quantifier<all_characters<char, plus_quantifier<word_class> > > > at_right(wordplus, dotwordplus, dotwordquestion);
    all_characters<plus_quantifier<word_class>,
           char,
           all_characters<plus_quantifier<word_class>,
           all_characters<char, plus_quantifier<word_class> >,
           question_mark_quantifier<all_characters<char, plus_quantifier<word_class> > > > > fmtEmail(wordplus, '@', at_right);
    bool matches = true;
    for ( string::const_iterator iter = email.begin(); iter != email.end(); )
    {
           int count = ChacatersNeedAdvance(fmtEmail);
           if ( (email.end() - iter) < count )
        {
                  matches = false;
                  break;
           }
           matches = matches && fmtEmail.IsMatch(iter, email.end());
           fmtEmail.AdvanceIterator(iter, email.end());
    }
    if ( matches )
    {
           cout << "[MATCH]: " << email << endl;
    }
    else
    {
           cout << "[UNMATCH]: " << email << endl;
    }
}

int main(int argc, char* argv[])
{
    MatchEmail("abc");
    MatchEmail("b");
    MatchEmail("c");
    MatchEmail("A");
    MatchEmail("Z");
    MatchEmail("zzz");
    MatchEmail("9");
    MatchEmail("0");
    MatchEmail("10");
    MatchEmail("-");
    MatchEmail("_");
    MatchEmail("!");
    MatchEmail("@");
    MatchEmail("a@c.d");
    MatchEmail("ab@cd.dd");
    MatchEmail("yimin_shi@hotmail.com");
    MatchEmail("yimin_shi@hotmail.com.cn");
    MatchEmail("yimin_shi@hotmail.com.org.cn");
    MatchEmail("yimin_shi@hotmail.com.org.cn.cn.cn.cn.cn.cn");
    MatchEmail("yimin_shi.yimin_shi@hotmail.com");
    return 0;
}  