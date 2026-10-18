#include <gtest/gtest.h>
#include <xgrammar/xgrammar.h>

using namespace xgrammar;

TEST(EmptyExcludeToken, GrammarRoundTripAndMatcherRollback) {
  auto grammar = Grammar::FromEBNF("root ::= ExcludeToken()*");
  auto recovered = Grammar::FromEBNF(grammar.ToString());
  TokenizerInfo tokenizer({"a", "b", "<eos>"}, VocabType::RAW, 3, std::vector<int32_t>{2});
  GrammarCompiler compiler(tokenizer);
  GrammarMatcher matcher(compiler.CompileGrammar(recovered));
  EXPECT_FALSE(matcher.AcceptToken(3));
  ASSERT_TRUE(matcher.AcceptToken(0));
  ASSERT_TRUE(matcher.AcceptToken(1));
  ASSERT_TRUE(matcher.AcceptToken(2));
  EXPECT_TRUE(matcher.IsTerminated());
  matcher.Rollback(1);
  EXPECT_FALSE(matcher.IsTerminated());
  EXPECT_TRUE(matcher.AcceptToken(0));
}

TEST(EmptyExcludeToken, AnyTokensStructuralTagCompilesThroughGrammarRoundTrip) {
  TokenizerInfo tokenizer({"a", "b", "<eos>"}, VocabType::RAW, 3, std::vector<int32_t>{2});
  GrammarCompiler compiler(tokenizer);
  auto compiled = compiler.CompileStructuralTag(
      R"({"type":"structural_tag","format":{"type":"any_tokens"}})"
  );
  auto recovered = Grammar::FromEBNF(compiled.GetGrammar().ToString());
  GrammarMatcher matcher(compiler.CompileGrammar(recovered));
  ASSERT_TRUE(matcher.AcceptToken(0));
  ASSERT_TRUE(matcher.AcceptToken(1));
  EXPECT_TRUE(matcher.AcceptToken(2));
}

TEST(EmptyExcludeToken, NonemptyExclusionsAndInvalidArgumentsRemainEnforced) {
  TokenizerInfo tokenizer({"a", "b", "<eos>"}, VocabType::RAW, 3, std::vector<int32_t>{2});
  GrammarCompiler compiler(tokenizer);
  GrammarMatcher matcher(compiler.CompileGrammar(Grammar::FromEBNF("root ::= ExcludeToken(1)*")));
  EXPECT_FALSE(matcher.AcceptToken(1));
  EXPECT_TRUE(matcher.AcceptToken(0));
  EXPECT_TRUE(matcher.AcceptToken(2));
  EXPECT_ANY_THROW(Grammar::FromEBNF("root ::= ExcludeToken(-1)"));
  EXPECT_ANY_THROW(Grammar::FromEBNF("root ::= ExcludeToken(\"a\")"));
  EXPECT_ANY_THROW(Grammar::FromEBNF("root ::= ExcludeToken(token=1)"));
}
