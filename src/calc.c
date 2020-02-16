#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include "stack.h"

// Stream
typedef struct {
	size_t len;
	char* pointer;
	char currentChar;
} Stream;

Stream* streamCreate(char* buffer)
{
	Stream* stream = malloc(sizeof(Stream));
	if (stream) {
		stream->len = strlen(buffer);
		stream->currentChar = 0;
		stream->pointer = buffer;
	}
	return stream;
}

void streamFree(Stream* stream)
{
	free(stream);
}

char streamGet(Stream* stream)
{
	stream->currentChar = *stream->pointer;
	if (stream->currentChar != 0)
		stream->pointer++;
	return stream->currentChar;
}
char streamCurrent(Stream* stream)
{
	return stream->currentChar;
}

int streamEOF(Stream* stream)
{
	return !stream->currentChar;
}

size_t streamGetLenth(Stream* stream)
{
	return stream->len;
}

// parser

typedef enum {
	tokError, tokIdent, tokNumber, tokPlus, tokMinus, tokMult, tokDiv, tokPow, tokSqrt,
	tokLeftPar, tokRightPar, tokComma
} TokenKind;

typedef struct {
	TokenKind kind;
	char* value;
	double dval;
} Token;

Token* tokenCreate(TokenKind kind, char* value)
{
	Token* token = malloc(sizeof(Token));
	if (token) {
		token->kind = kind;
		token->value = malloc(strlen(value) + 1);
		if (token->value)
			strcpy(token->value, value);
	}
	return token;
}

void tokenFree(Token* token)
{
	free(token->value);
	free(token);
}

typedef struct {
	Stream* stream;
} Parser;

Parser* parserCreate(Stream* stream)
{
	Parser* parser = malloc(sizeof(Parser));
	if (parser) {
		parser->stream = stream;
		streamGet(stream);  // Inicjujemy wczytuj¹c pierwszy znak
	}
	return parser;
}

void parserFree(Parser* parser)
{
	free(parser->stream);
	free(parser);
}

int parserEOF(Parser* parser)
{
	return streamEOF(parser->stream);
}

int isnumber(char c)
{
	return isdigit(c) || c == '.';
}

// pobiera token ze strumienia
Token* parserGetToken(Parser* parser)
{
	Stream* stream = parser->stream;
	if (streamEOF(stream))
		return NULL;

	TokenKind tokenKind = tokError;
	static char symbol[1024];
	symbol[0] = 0;
	char* psymbol = symbol;

	char c = streamCurrent(stream);
	while (isspace(c)) {
		c = streamGet(stream);
	}

	if (isalpha(c)) {
		do {
			*psymbol++ = c;
			c = streamGet(stream);
		} while (isalpha(c));
		*psymbol = 0;
		if (strcmp(symbol, "sqrt") == 0) {
			tokenKind = tokSqrt;
		}
		else if (strcmp(symbol, "pow") == 0) {
			tokenKind = tokPow;
		}
	}
	else if (isnumber(c)) {
		tokenKind = tokNumber;
		do {
			*psymbol++ = c;
			c = streamGet(stream);
		} while (isnumber(c));
		*psymbol = 0;
	}
	else if (c == '+') {
		tokenKind = tokPlus;
		*psymbol++ = c;
		*psymbol = 0;
		streamGet(stream);
	}
	else if (c == '-') {
		tokenKind = tokMinus;
		*psymbol++ = c;
		*psymbol = 0;
		streamGet(stream);
	}
	else if (c == '*') {
		tokenKind = tokMult;
		*psymbol++ = c;
		*psymbol = 0;
		streamGet(stream);
	}
	else if (c == '/') {
		tokenKind = tokDiv;
		*psymbol++ = c;
		*psymbol = 0;
		streamGet(stream);
	}
	else if (c == '(') {
		tokenKind = tokLeftPar;
		*psymbol++ = c;
		*psymbol = 0;
		streamGet(stream);
	}
	else if (c == ')') {
		tokenKind = tokRightPar;
		*psymbol++ = c;
		*psymbol = 0;
		streamGet(stream);
	}
	else if (c == ',') {
		tokenKind = tokComma;
		*psymbol++ = c;
		*psymbol = 0;
		streamGet(stream);
	}
	else {
		tokenKind = tokError;
	}
	return tokenCreate(tokenKind, symbol);
}

static void add_to_output(char* input, char* output)
{
	strcat(output, input);
	strcat(output, " ");
}

// zwraca priorytet operatora
int getPriority(TokenKind kind) {
	switch (kind) {
	case tokPlus:
	case tokMinus:
		return 1;
	case tokMult:
	case tokDiv:
		return 2;
	default:
		return 0;
	}
}

static void error(const char* message)
{
	printf(message);
	printf("\n");
	exit(EXIT_FAILURE);
}

// konwertuje wyrazenie w notacji infiksowej do notacji postfiksowej (onp)
// np: 1 + 1 -> 1 1 +
char* infix_to_onp(char* infix)
{
	char* postfix = malloc(4 * strlen(infix));
	if (postfix) {
		postfix[0] = 0;
		PStack stack = stackCreate();
		Parser* parser = parserCreate(streamCreate(infix));
		while (!parserEOF(parser)) {
			Token* token = parserGetToken(parser);
			if (token) {
				//printf("%s\n", token->value);
				switch (token->kind) {
				case tokNumber:
					add_to_output(token->value, postfix);
					tokenFree(token);
					break;
				case tokPlus:
				case tokMinus:
				case tokMult:
				case tokDiv:
					while (!stackIsEmpty(stack)) {
						Token* tok = stackPeek(stack);
						if (getPriority(token->kind) <= getPriority(tok->kind)) {
							tok = stackPop(stack);
							add_to_output(tok->value, postfix);
							tokenFree(tok);
						}
						else
							break;
					}
					stackPush(stack, token);
					break;
				case tokSqrt:
				case tokPow:
					stackPush(stack, token);
					break;
				case tokComma:
					while (!stackIsEmpty(stack)) {
						Token* tok = stackPeek(stack);
						if (tok->kind != tokLeftPar) {
							tok = stackPop(stack);
							add_to_output(tok->value, postfix);
							tokenFree(tok);
						}
						else
							break;
					}
					break;
				case tokLeftPar:
					stackPush(stack, token);
					break;
				case tokRightPar:
					while (!stackIsEmpty(stack)) {
						Token* tok = stackPeek(stack);
						if (tok->kind != tokLeftPar) {
							tok = stackPop(stack);
							add_to_output(tok->value, postfix);
							tokenFree(tok);
						}
						else {
							tok = stackPop(stack);
							tokenFree(tok);
							if (!stackIsEmpty(stack)) {
								tok = stackPop(stack);
								add_to_output(tok->value, postfix);
								tokenFree(tok);
							}
							break;
						}
					}
					break;
				case tokError:
					error("Niespodziewany znak");
				}
			}
		}
		while (!stackIsEmpty(stack)) {
			Token* tok = stackPop(stack);
			add_to_output(tok->value, postfix);
			tokenFree(tok);
		}
		parserFree(parser);
		assert(stackIsEmpty(stack));
		stackFree(stack);
	}
	return postfix;
}

double eval_(TokenKind kind, double a, double b) {
	switch (kind) {
	case tokPlus:
		return a + b;
	case tokMinus:
		return a - b;
	case tokMult:
		return a * b;
	case tokDiv:
		return a / b;
	default:
		return 0.0;
	}
}

double eval_onp(char* onp)
{
	Token* a;
	Token* b;
	Token* tok;
	PStack stack = stackCreate();
	Parser* parser = parserCreate(streamCreate(onp));
	while (!parserEOF(parser)) {
		Token* token = parserGetToken(parser);
		if (token) {
			//printf("%s\n", token->value);
			switch (token->kind) {
			case tokNumber:
				token->dval = atof(token->value);
				stackPush(stack, token);
				break;
			case tokPlus:
			case tokMinus:
			case tokMult:
			case tokDiv:
				a = stackPop(stack);
				b = stackPop(stack);
				tok = tokenCreate(tokNumber, "");
				tok->dval = eval_(token->kind, b->dval, a->dval);
				stackPush(stack, tok);
				tokenFree(a);
				tokenFree(b);
				break;
			case tokSqrt:
				a = stackPop(stack); // argument sqrt
				tok = tokenCreate(tokNumber, "");
				tok->dval = sqrt(a->dval);
				stackPush(stack, tok);
				tokenFree(a);
				break;
			case tokPow:
				a = stackPop(stack); // drugi argument pow
				b = stackPop(stack); // pierwszy argument pow
				tok = tokenCreate(tokNumber, "");
				tok->dval = pow(b->dval, a->dval);
				stackPush(stack, tok);
				tokenFree(a);
				tokenFree(b);
				break;
			}
		}
	}
	double res = 0.0;
	if (!stackIsEmpty(stack)) {
		tok = stackPop(stack);
		res = tok->dval;
		tokenFree(tok);
	}
	parserFree(parser);
	assert(stackIsEmpty(stack));
	stackFree(stack);
	return res;
}

char* getArgsAsString(int argc, char* argv[])
{
	int i;
	size_t count = 1;
	for (i = 1; i < argc; ++i)
		count += strlen(argv[i]) + 1;
	char* str = malloc(count);
	if (str) {
		str[0] = 0;
		for (i = 1; i < argc; ++i)
			strcat(str, argv[i]);
	}
	return str;
}

void listArgv(int argc, char* argv[])
{
	int i;
	for (i = 0; i < argc; ++i){
		printf("%s\n", argv[i]);
	}
}

double eval(const char* expr)
{
	char* postfix = infix_to_onp(expr);
	if (postfix) {
		double res = eval_onp(postfix);
		free(postfix);
		return res;
	}
}


int _main(int argc, char* argv[])
{
    //listArgv(argc, argv);
	if (argc > 1) {
		char* infix = getArgsAsString(argc, argv);
		//printf("%s\n", infix);
		char* postfix = infix_to_onp(infix);
		if (postfix) {
			//printf("%s\n", postfix);
			double val = eval_onp(postfix);
			printf("%f", val);
			free(postfix);
		}
	}
	else {
		printf("wywolanie:\n");
		printf("calc \"wyrazenie\"\n");
	}
	return 0;
}
