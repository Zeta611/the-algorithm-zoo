

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#define STK_CAP 1000

typedef enum Sym { ADD, SUB, MUL, DIV, POW, LPR, RPR } Sym;

static const char SYM_NAMES[][4] = {"ADD", "SUB", "MUL", "DIV",
				    "POW", "LPR", "RPR"};

static const enum {
	LASSOC,
	RASSOC,
	NASSOC
} SYM_ASSOC[] = {LASSOC, LASSOC, LASSOC, LASSOC, RASSOC, NASSOC, NASSOC};

static const int SYM_PREC[] = {0, 0, 1, 1, 2, -1, -1};

static const Sym SYM_TBL[] = {
    ['+'] = ADD, ['-'] = SUB, ['*'] = MUL, ['/'] = DIV,
    ['^'] = POW, ['('] = LPR, [')'] = RPR};

typedef struct Token {
	enum { EOF_T, SYM_T, NUM_T } type;
	union {
		Sym sym;
		long num;
	} u;
} Token;

static inline int op_cmp(Sym a, Sym b) { return SYM_PREC[a] - SYM_PREC[b]; }

static inline bool isop(int c)
{
	return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

static inline bool ispar(int c) { return c == '(' || c == ')'; }

Token gettok()
{
	int ch;
	do {
		ch = getchar();
	} while (isspace(ch));

	if (isop(ch) || ispar(ch)) {
		return (struct Token){SYM_T, .u.sym = SYM_TBL[ch]};
	}
	if (ch == EOF || !isdigit(ch)) {
		return (struct Token){EOF_T};
	}

	ungetc(ch, stdin);
	long num;
	if (scanf("%ld", &num) <= 0) {
		return (struct Token){EOF_T};
	}
	return (struct Token){NUM_T, .u.num = num};
}

typedef struct Stack {
	Sym *top;
	Sym container[STK_CAP];
	size_t size;
} Stack;

bool push(Sym val, Stack *s)
{
	if (s->size == STK_CAP) {
		return false;
	}
	++s->size;
	if (!s->top) {
		s->top = s->container;
		*s->top = val;
	} else {
		*++s->top = val;
	}
	return true;
}

bool pop(Stack *s)
{
	if (!s->size) {
		return false;
	}
	if (s->size == 1) {
		s->size = 0;
		s->top = NULL;
	} else {
		--s->size;
		--s->top;
	}
	return true;
}

bool shunting_yard()
{
	Stack stk = {0};
	int c;

	while (true) {
		c = getchar();
		if (c == EOF || c == '\n') {
			break;
		}
		ungetc(c, stdin);

		Token tok = gettok();
		switch (tok.type) {
		case EOF_T:
			return false;
		case SYM_T:

			if (tok.u.sym == LPR) {
				push(tok.u.sym, &stk);
			} else if (tok.u.sym == RPR) {

				while (stk.size && *stk.top != LPR) {
					printf("%s ", SYM_NAMES[*stk.top]);
					pop(&stk);
				}
				if (!stk.size || *stk.top != LPR) {

					printf("MALFORMED EQ\n");
					return true;
				}
				pop(&stk);

			} else {

				while (stk.size && *stk.top != LPR &&
				       *stk.top != RPR) {
					Sym s = *stk.top;
					int cmp = op_cmp(tok.u.sym, s);
					if (cmp < 0) {
						printf("%s ", SYM_NAMES[s]);
						pop(&stk);
					} else if (cmp == 0 &&
						   SYM_ASSOC[s] == LASSOC) {
						printf("%s ", SYM_NAMES[s]);
						pop(&stk);
					} else {
						break;
					}
				}
				push(tok.u.sym, &stk);
			}

			break;
		case NUM_T:
			printf("%ld ", tok.u.num);
			break;
		}
	}

	while (stk.size) {
		Sym s = *stk.top;
		if (s == LPR) {
			printf("MALFORMED EQ\n");
			return true;
		}
		printf("%s ", SYM_NAMES[s]);
		pop(&stk);
	}

	if (c == '\n') {
		putchar('\n');
		return true;
	} else {
		return false;
	}
}

int main(void)
{

	int c;
	while (true) {
		printf("> ");
		if ((c = getchar()) == EOF) {
			putchar('\n');
			return 0;
		}
		ungetc(c, stdin);
		if (!shunting_yard()) {
			putchar('\n');
			return 0;
		}
	}
}
