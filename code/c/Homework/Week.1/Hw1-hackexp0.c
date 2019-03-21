#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

int tokenIdx = 0;
char *tokens;

int E();
int F();
int T();

void error(char *msg) {
  printf("%s", msg);
  assert(0);
}

char ch() {
  char c = tokens[tokenIdx];
  return c;
}

char next() {
  char c = ch();
  tokenIdx++;
  return c;
}

int isNext(char *set) {
  char c = ch();
  return (c!='\0' && strchr(set, c)!=NULL);
}

int nextTemp() {
  static int tempIdx = 0;
  return tempIdx++;
}

void genOp1(int i, char c) {
  printf("# t%d=%c\n", i, c);
  // t1=3 轉成 @3; D=A; @t1; M=D
  // t1=x 轉成 @x; D=M; @t1; M=M
  printf("@%c\n", c);
  char AM = (isdigit(c)) ? 'A':'M';
  printf("D=%c\n", AM);
 // printf("D=A\n");
  printf("@t%d\n", i);
  printf("M=D\n");
}

void genOp2(int i, int i1, char op, int i2) {
  printf("# t%d=t%d%ct%d\n", i, i1, op, i2);
  // t0=t1+t2 轉成 @t1; D=M; @t2; D=D+M; @t0; M=D;
  printf("@t%d\n", i1);
  printf("D=M\n");
  printf("@t%d\n", i2);
  printf("D=D%cM\n", op);
  printf("@t%d\n", i);
  printf("M=D\n");
}

// E = T ([+-] T)*
int E() {
  int i1 = T();
  while (isNext("+-")) {
    char op=next();
    int i2 = T();
    int i = nextTemp();
    genOp2(i, i1, op, i2);
    i1 = i;
  }
  return i1;
}

// T = F ([*/] F)*
int T() {
  int f1 = F();
  while (isNext("*/")) {
    char op=next();
    int f2 = F();
    int f = nextTemp();
    printf("t%d=t%d%ct%d\n", f, f1, op, f2);
    f1 = f;
  }
  return f1;
}

// F = Number | Id | '(' E ')'
int F() {
  int f;
  char c = ch();
  if (isdigit(ch()) || isalpha(ch())) {
    next(); // skip c
    f = nextTemp();
    genOp1(f, c);
  } else if (c=='(') { // '(' E ')'
    next();
    f = E();
    assert(ch()==')');
    next();
  } else {
    error("F = (E) | Number fail!");
  }
  return f; 
}

void parse(char *str) {
  tokens = str;
  E();
}

int main(int argc, char * argv[]) {
  printf("=== EBNF Grammar =====\n");
  printf("E=F ([+-] F)*\n");
  printf("T=F ([*/] F)*\n");
  printf("F=Number | '(' E ')'\n");
  printf("==== parse:%s ========\n", argv[1]);
  parse(argv[1]);
}