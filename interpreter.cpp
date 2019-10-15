#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>

struct Token {
  enum Type { integer, plus, minus, lparen, rparen } type_;
  std::string text_;

  explicit Token(Type type, std::string text) : type_{type}, text_{text} {}

  friend std::ostream &operator<<(std::ostream &os, const Token &obj);
};

std::ostream& operator<<(std::ostream &os, const Token &obj) {
  return os << "'" << obj.text_ << "'";
}

std::vector<Token> lex(const std::string &input) {
  std::vector<Token> result;

  for (unsigned i = 0; i < input.size(); ++i) {
    switch (input[i]) {
    case '+':
      result.push_back(Token{Token::plus, "+"});
      break;
    case '-':
      result.push_back(Token{Token::minus, "-"});
      break;
    case '(':
      result.push_back(Token{Token::lparen, "("});
      break;
    case ')':
      result.push_back(Token{Token::rparen, ")"});
      break;
    default:
      std::ostringstream buffer;
      buffer << input[i];
      for (unsigned j = i + 1; j < input.size(); ++j) {
        if (std::isdigit(input[j])) {
          buffer << input[j];
          ++i;
        } else {
          result.push_back(Token{Token::integer, buffer.str()});
          break;
        }
      }
    }
  }
  return result;
}

struct Element {
  virtual int eval() const = 0;
};

struct Integer : Element {
  int value_;
  explicit Integer(const int value) : value_(value) {}
  int eval() const override {return value_;}
};

struct BinaryOperation : Element {
  enum Type { addition, subtraction } type;
  std::shared_ptr<Element> lhs, rhs;

  int eval() const override {
    if(type == addition)
      return lhs->eval() + rhs->eval();
    return lhs->eval() - rhs->eval();
  }
};

std::shared_ptr<Element> parse(const std::vector<Token>& tokens) {
  auto result = std::make_unique<BinaryOperation>();
  bool have_lhs = false;
  for(unsigned i = 0; i < tokens.size(); ++i) {
    auto token = tokens[i];
    switch(token.type_) {
      default:
        break;
      case Token::integer: 
      {
        int value = std::stoi(token.text_);
        auto integer = std::make_shared<Integer>(value);
        if(!have_lhs) {
          result->lhs = integer;
          have_lhs = true;
        }
        else
          result->rhs = integer;
      }
      case Token::plus:
        result->type = BinaryOperation::addition;
        break;
      case Token::minus:
        result->type = BinaryOperation::subtraction;
        break;
      case Token::lparen:
        unsigned j = i;
        for(; j < tokens.size(); ++j)
          if(tokens[j].type_ == Token::rparen)
            break;

        std::vector<Token> subExpression(&tokens[i+1], &tokens[j]);
        auto element = parse(subExpression);
        if(!have_lhs) {
          result->lhs = element;
          have_lhs= true;
        }
        else
        {
            result->rhs = element;
        }
        i = j;
    }
  }
  return result;
}

int main() {
  std::string input { "(1+2)+(3+4)"};
  auto tokens = lex(input);
  auto parsed = parse(tokens); 
  std::cout << input << "=" << parsed->eval() << std::endl;
  return 0;
}