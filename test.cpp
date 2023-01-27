#include <vector>
#include "finite_automaton.h"
#include "gtest/gtest.h"

using std::string;
using std::vector;

TEST(BASIC_TEST, TEST1) {
    NFA nfa(3, {'a', 'b', 'c'}, {{0, 0}, {0, 1}, {1, 0}, {0, 2}, {2, 2}}, {"a", "b", "c", "a", "b"},
            {false, true, true});
    DFA dfa(nfa);
    ASSERT_EQ(dfa.size(), 5);
    ASSERT_EQ(dfa.eps(), 3);
    ASSERT_FALSE(dfa.is_in(""));
    ASSERT_TRUE(dfa.is_in("bca"));
    ASSERT_FALSE(dfa.is_in("aaaaaac"));
    ASSERT_TRUE(dfa.is_in("aaaaaaaaabcbcaaaaaaaaabcbcbcabbbbbbbbbbbb"));
    ASSERT_TRUE(dfa.is_in("abcb"));
}

TEST(BASIC_TEST, TEST2) {
    NFA nfa(1, {}, {}, {}, {true});
    DFA dfa(nfa);
    ASSERT_TRUE(dfa.is_in(""));
    ASSERT_FALSE(dfa.is_in("a"));
    ASSERT_FALSE(dfa.is_in("b"));
    ASSERT_FALSE(dfa.is_in("baacasdacas"));
}

TEST(EPS_PATHS_TEST, TEST1) {
    NFA nfa(6, {'a', 'b', 'c'}, {{0, 1}, {1, 2}, {1, 3}, {0, 4}, {4, 5}}, {"", "a", "b", "", "c"},
            {false, false, true, true, false, true});
    DFA dfa(nfa);
    ASSERT_EQ(dfa.size(), 4);
    ASSERT_FALSE(dfa.is_in(""));
    ASSERT_TRUE(dfa.is_in("a"));
    ASSERT_TRUE(dfa.is_in("b"));
    ASSERT_TRUE(dfa.is_in("c"));
    ASSERT_FALSE(dfa.is_in("ab"));
    ASSERT_FALSE(dfa.is_in("acaba"));
    dfa.minimize();
    ASSERT_EQ(dfa.size(), 2);
    ASSERT_FALSE(dfa.is_in(""));
    ASSERT_TRUE(dfa.is_in("a"));
    ASSERT_TRUE(dfa.is_in("b"));
    ASSERT_TRUE(dfa.is_in("c"));
    ASSERT_FALSE(dfa.is_in("ab"));
    ASSERT_FALSE(dfa.is_in("acaba"));
}

TEST(EPS_PATHS_TEST, TEST2) {
    NFA nfa(3, {'a'}, {{0, 1}, {1, 2}}, {"", "a"}, {false, true, true});
    DFA dfa(nfa);
    ASSERT_EQ(dfa.size(), 2);
    ASSERT_TRUE(dfa.is_in(""));
    ASSERT_TRUE(dfa.is_in("a"));
    ASSERT_FALSE(dfa.is_in("aa"));
}

TEST(LONG_WORDS_TEST, TEST1) {
    NFA nfa(2, {'a', 'b', 'c'}, {{0, 1}, {1, 0}}, {"ab", "bc"}, {true, false});
    DFA dfa(nfa);
    ASSERT_EQ(dfa.size(), 4);
    dfa.minimize();
    ASSERT_EQ(dfa.size(), 4);
    ASSERT_TRUE(dfa.is_in(""));
    ASSERT_TRUE(dfa.is_in("abbc"));
    ASSERT_TRUE(dfa.is_in("abbcabbcabbcabbcabbcabbcabbcabbcabbcabbc"));
    ASSERT_FALSE(dfa.is_in("a"));
    ASSERT_FALSE(dfa.is_in("ab"));
    ASSERT_FALSE(dfa.is_in("abb"));
    ASSERT_FALSE(dfa.is_in("abbcabbcabbcabbcabbcabbcabbcabbcabbcabbcb"));
    ASSERT_FALSE(dfa.is_in("abbcabbcabbcabbcabbcabbcabbcabbcabbcabbca"));
    ASSERT_FALSE(dfa.is_in("abbcabbcabbcabbcabbcabbcabbcabbcabbcabbcc"));
}

TEST(CONVERTING_TO_COMPLETE_TEST, TEST1) {
    NFA nfa(4, {'a', 'b', 'c'}, {{0, 1}, {1, 1}, {0, 2}, {2, 1}, {1, 3}, {2, 3}}, {"a", "b", "b", "c", "a", "c"},
            {false, false, true, true});
    DFA dfa(nfa);
    ASSERT_EQ(dfa.size(), 5);
    ASSERT_FALSE(dfa.is_in(""));
    ASSERT_TRUE(dfa.is_in("bca"));
    ASSERT_FALSE(dfa.is_in("bcaa"));
    ASSERT_FALSE(dfa.is_in("a"));
    ASSERT_TRUE(dfa.is_in("aa"));
    ASSERT_TRUE(dfa.is_in("abbbbbbbbbbbbbbbbbbbbbbba"));
    ASSERT_TRUE(dfa.is_in("b"));
    ASSERT_TRUE(dfa.is_in("bc"));
    CDFA cdfa(dfa);
    ASSERT_EQ(cdfa.size(), 6);
    ASSERT_FALSE(cdfa.is_in(""));
    ASSERT_TRUE(cdfa.is_in("bca"));
    ASSERT_FALSE(cdfa.is_in("bcaa"));
    ASSERT_FALSE(cdfa.is_in("a"));
    ASSERT_TRUE(cdfa.is_in("aa"));
    ASSERT_TRUE(cdfa.is_in("abbbbbbbbbbbbbbbbbbbbbbba"));
    ASSERT_TRUE(cdfa.is_in("b"));
    ASSERT_TRUE(cdfa.is_in("bc"));
}

TEST(CONVERTING_TO_COMPLEMENT_TEST, TEST1) {
    NFA nfa(4, {'a', 'b', 'c'}, {{0, 1}, {1, 1}, {0, 2}, {2, 1}, {1, 3}, {2, 3}}, {"a", "b", "b", "c", "a", "c"},
            {false, false, true, true});
    CDFA cdfa(nfa);
    ASSERT_EQ(cdfa.size(), 6);
    ASSERT_FALSE(cdfa.is_in(""));
    ASSERT_TRUE(cdfa.is_in("bca"));
    ASSERT_FALSE(cdfa.is_in("bcaa"));
    ASSERT_FALSE(cdfa.is_in("a"));
    ASSERT_TRUE(cdfa.is_in("aa"));
    ASSERT_TRUE(cdfa.is_in("abbbbbbbbbbbbbbbbbbbbbbba"));
    ASSERT_TRUE(cdfa.is_in("b"));
    ASSERT_TRUE(cdfa.is_in("bc"));
    cdfa.convert_to_CDFA_complement();
    ASSERT_EQ(cdfa.size(), 6);
    ASSERT_TRUE(cdfa.is_in(""));
    ASSERT_FALSE(cdfa.is_in("bca"));
    ASSERT_TRUE(cdfa.is_in("bcaa"));
    ASSERT_TRUE(cdfa.is_in("a"));
    ASSERT_FALSE(cdfa.is_in("aa"));
    ASSERT_FALSE(cdfa.is_in("abbbbbbbbbbbbbbbbbbbbbbba"));
    ASSERT_FALSE(cdfa.is_in("b"));
    ASSERT_FALSE(cdfa.is_in("bc"));
}

TEST(SHOW_AUTOMATON_TEST, TEST1) {
    NFA nfa(6, {'a', 'b'}, {{0, 1}, {0, 4}, {1, 2}, {2, 1}, {1, 3}, {3, 3}, {4, 5}, {4, 5}, {5, 4}, {5, 4}},
            {"", "", "a", "b", "", "b", "a", "b", "a", "b"}, {false, false, false, true, true, false});
    DFA dfa(nfa);
    ASSERT_NO_THROW(dfa.show_automaton());
    CDFA cdfa(dfa);
    ASSERT_NO_THROW(cdfa.show_automaton());
    cdfa.minimize();
    ASSERT_NO_THROW(cdfa.show_automaton());
}

TEST(REGULAR_EXPRESSIONS_TEST, TEST1) {
    NFA nfa(4, {'a', 'b'}, {{0, 1}, {1, 0}, {0, 2}, {2, 0}, {2, 3}, {0, 3}}, {"a", "b", "b", "a", "a", "a"},
            {true, false, false, true});
    DFA dfa(nfa);
    ASSERT_EQ(dfa.regular_expression(), "(ab+(ba)(ba)*(ab))*(1+1+a+(ba)(ba)*(1+a))");
    CDFA cdfa(dfa);
    cdfa.minimize();
    ASSERT_EQ(cdfa.regular_expression(), "(ab+ba)*(1+1+a)");
}

TEST(REGULAR_EXPRESSIONS_TEST, TEST2) {
    NFA nfa(6, {'a', 'b', 'c'}, {{0, 1}, {1, 2}, {1, 3}, {0, 4}, {4, 5}}, {"", "a", "b", "", "c"},
            {false, false, true, true, false, true});
    DFA dfa(nfa);
    ASSERT_EQ(dfa.regular_expression(), "(a+b+c)");
}

TEST(REGULAR_EXPRESSIONS_TEST, TEST3) {
    NFA nfa(1, {'a', 'b', 'c'}, {{0, 0}}, {""}, {false});
    DFA dfa(nfa);
    ASSERT_EQ(dfa.regular_expression(), "");
}

TEST(BUILD_NFA_BY_REGULAR_EXPRESSION_TEST, TEST1) {
    NFA nfa("ab.");  // ab
    DFA dfa(nfa);
    CDFA cdfa(dfa);
    cdfa.minimize();
    ASSERT_EQ(cdfa.regular_expression(), "((ab))");
    ASSERT_TRUE(cdfa.is_in("ab"));
    ASSERT_FALSE(cdfa.is_in("a"));
    ASSERT_FALSE(cdfa.is_in("b"));
    ASSERT_FALSE(cdfa.is_in(""));
    ASSERT_FALSE(cdfa.is_in("abb"));
    ASSERT_FALSE(cdfa.is_in("aaab"));
}

TEST(BUILD_NFA_BY_REGULAR_EXPRESSION_TEST, TEST2) {
    NFA nfa("aabc1*+..+");  // a* + abc + 1
    DFA dfa(nfa);
    CDFA cdfa(dfa);
    cdfa.minimize();
    ASSERT_EQ(cdfa.regular_expression(), "(1+1+a+(aa)(a)*(1+a(a)*)+((ab)c))");
    ASSERT_TRUE(cdfa.is_in(""));
    ASSERT_TRUE(cdfa.is_in("a"));
    ASSERT_TRUE(cdfa.is_in("aaaaaaaaaaaaaaaaaaaaaaaaa"));
    ASSERT_TRUE(cdfa.is_in("abc"));
    ASSERT_FALSE(cdfa.is_in("b"));
    ASSERT_FALSE(cdfa.is_in("c"));
    ASSERT_FALSE(cdfa.is_in("bc"));
    ASSERT_FALSE(cdfa.is_in("ab"));
    ASSERT_FALSE(cdfa.is_in("aaabc"));
}

TEST(BUILD_NFA_BY_REGULAR_EXPRESSION_TEST, TEST3) {
    NFA nfa("(ab+)(ab+).");  // (a+b)(a+b)
    DFA dfa(nfa);
    CDFA cdfa(dfa);
    cdfa.minimize();
    ASSERT_EQ(dfa.regular_expression(), "((aa+ba)+(ab+bb))");
    ASSERT_EQ(cdfa.regular_expression(), "(((a+b)(a+b)+(a+b)(a+b)))");
    ASSERT_TRUE(cdfa.is_in("aa"));
    ASSERT_TRUE(cdfa.is_in("ab"));
    ASSERT_TRUE(cdfa.is_in("ba"));
    ASSERT_TRUE(cdfa.is_in("bb"));
    ASSERT_FALSE(cdfa.is_in("aaa"));
    ASSERT_FALSE(cdfa.is_in("bbb"));
    ASSERT_FALSE(cdfa.is_in("a"));
    ASSERT_FALSE(cdfa.is_in("b"));
    ASSERT_FALSE(cdfa.is_in(""));
}

TEST(BUILD_NFA_BY_REGULAR_EXPRESSION_TEST, TEST4) {
    NFA nfa("(ab+)(ab+).");  // (a+b)(a+b)
    DFA dfa(nfa);
    CDFA cdfa(dfa);
    cdfa.minimize();
    ASSERT_EQ(dfa.regular_expression(), "((aa+ba)+(ab+bb))");
    ASSERT_EQ(cdfa.regular_expression(), "(((a+b)(a+b)+(a+b)(a+b)))");
    ASSERT_TRUE(cdfa.is_in("aa"));
    ASSERT_TRUE(cdfa.is_in("ab"));
    ASSERT_TRUE(cdfa.is_in("ba"));
    ASSERT_TRUE(cdfa.is_in("bb"));
    ASSERT_FALSE(cdfa.is_in("aaa"));
    ASSERT_FALSE(cdfa.is_in("bbb"));
    ASSERT_FALSE(cdfa.is_in("a"));
    ASSERT_FALSE(cdfa.is_in("b"));
    ASSERT_FALSE(cdfa.is_in(""));
}

TEST(BUILD_NFA_BY_REGULAR_EXPRESSION_TEST, TEST5) {
    NFA nfa("");
    DFA dfa(nfa);
    CDFA cdfa(dfa);
    cdfa.minimize();
    ASSERT_EQ(dfa.regular_expression(), "");
    ASSERT_EQ(cdfa.regular_expression(), "");
    ASSERT_FALSE(cdfa.is_in("aa"));
    ASSERT_FALSE(cdfa.is_in("ab"));
    ASSERT_FALSE(cdfa.is_in("ba"));
    ASSERT_FALSE(cdfa.is_in("bb"));
    ASSERT_FALSE(cdfa.is_in("aaa"));
    ASSERT_FALSE(cdfa.is_in("bbb"));
    ASSERT_FALSE(cdfa.is_in("a"));
    ASSERT_FALSE(cdfa.is_in("b"));
    ASSERT_FALSE(cdfa.is_in(""));
}

TEST(LONGEST_SUBSTRING_SEARCH, TEST1) {
    NFA nfa("");
    DFA dfa(nfa);
    CDFA cdfa(dfa);
    cdfa.minimize();
    ASSERT_EQ(cdfa.longest_substring(""), 0);
    ASSERT_EQ(cdfa.longest_substring("anwiefcwhagncfahiweh"), 0);
}

TEST(LONGEST_SUBSTRING_SEARCH, TEST2) {
    // (a + b)* + abc
    NFA nfa("(ab+)abc*+..");
    DFA dfa(nfa);
    CDFA cdfa(dfa);
    cdfa.minimize();
    ASSERT_EQ(cdfa.longest_substring("ababababababbbbbbbab"), 20);
    ASSERT_EQ(cdfa.longest_substring("abcbacababab"), 6);
    ASSERT_EQ(cdfa.longest_substring(""), 0);
    ASSERT_EQ(cdfa.longest_substring("abcabc"), 3);
    ASSERT_EQ(cdfa.longest_substring("nnnnnmmmmm"), 0);
    ASSERT_EQ(cdfa.longest_substring("bnnnnnmmmmm"), 1);
    ASSERT_EQ(cdfa.longest_substring("nnnnnmmmmma"), 1);
    ASSERT_EQ(cdfa.longest_substring("nnnnnabmmmmma"), 2);
    ASSERT_EQ(find_longest_substring_in_language("(ab+)abc*+..", "abcbacababab"), 6);
}

