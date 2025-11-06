#include <stdio.h>
#include <stdlib.h>

enum act {ACT_ERR, ACT_ADD=1, ACT_RM, ACT_LIST, ACT_EXIT};

static void banner(void) __attribute__((nothrow, cold));
static enum act getact(void) __attribute__((nothrow));
static int again(void) __attribute__((nothrow));
static void exit_program(void) __attribute__((nothrow, noreturn));

int main(void)
{
	enum act act;

	banner();
	while (1) {
		if ((act = getact()) == ACT_ERR) {
			fputs("Opção inválida! Tente novamente.\n", stdout);
			continue;
		}

		if (!again())
			exit_program();
	}
}

static void banner(void)
{
	fputs("================================\n", 	stdout);
	fputs("   Controle de Estoque\n", 		stdout);
	fputs("================================\n", 	stdout);
}

static enum act getact(void)
{
	enum act act;

	fputs("Selecione uma opção:\n", stdout);
	fputs("1. Adicionar Item\n", 	stdout);
	fputs("2. Remover Item\n", 	stdout);
	fputs("3. Listar Estoque\n", 	stdout);
	fputs("4. Sair\n", 		stdout);
	fputs("Opção: ", 		stdout);
	if (scanf("%d", (int *)&act) != 1 || act < 1 || act > 4)
		act = ACT_ERR;
	getchar();
	return act;
}

static int again(void)
{
	char ret;

	while (1) {
		fputs("Deseja realizar outra operação? (s/n): ", stdout);
		scanf("%c", &ret); getchar();
		if (ret == 's' || ret == 'S' || ret == 'n' || ret == 'N')
			return ret == 's' || ret == 'S';
	}
}

static void exit_program(void)
{
	fputs("Obrigado por usar o Controle de Estoques! Até a próxima.\n", stdout);
	exit(EXIT_SUCCESS);
	__builtin_unreachable();
}
